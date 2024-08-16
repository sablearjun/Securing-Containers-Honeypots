import { Router } from 'express';
import { yarafile_add, yarafile_list } from 'controllers/yara';
import { checkJwt } from 'middleware/checkJwt';
import { checkRole } from 'middleware/checkRole';
import fs from 'fs';
import multer from 'multer'

const router = Router();

const storage = multer.diskStorage({
    destination: function (req, file, cb) {
        cb(null, 'uploads/yara/')
    },
    filename: function (req, file, cb) {
      const uniqueSuffix = Date.now() + '$' + String(req.query.username) + '$'+ String(req.query.platform) + '$' + file.originalname;
      cb(null, file.fieldname + '$' + uniqueSuffix)
  }
})

const upload = multer({ storage: storage })

// router.post("/file_upload", upload.single("file"), yarafile_add);
router.post("/file_upload",[checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], upload.single("file"), (req, res, next) => {
  console.log("Req", req.file);
  if (req.file) {
    // Continue to the route handler for yarafile_add
    return next();
  } else {
    return res.status(400).json({ status: "Fail", message: "File not provided in the request" });
  }
},      // Continue to the route handler for yarafile_add
yarafile_add
);

router.get("/list", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], yarafile_list);
// router.delete("/delete", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)],  yarafile_delete);

export default router;
import { Router } from 'express';
import { ipfile_add, ipfile_list } from 'controllers/ip';
import { checkJwt } from 'middleware/checkJwt';
import { checkRole } from 'middleware/checkRole';
import fs from 'fs';
import multer from 'multer'

const router = Router();

const storage = multer.diskStorage({
    destination: function (req, file, cb) {
        cb(null, 'uploads/ip/')
    },
    filename: function (req, file, cb) {
      const uniqueSuffix = Date.now() + '$' + String(req.query.username) + '$' + file.originalname;
      cb(null, file.fieldname + '$' + uniqueSuffix)
  }
})

const upload = multer({ storage: storage })

// router.post("/file_upload", upload.single("file"), ipfile_add);
router.post("/file_upload",[checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], upload.single("file"), (req, res, next) => {
      console.log("Req", req.file);
      if (req.file) {
        // Continue to the route handler for ipfile_add
        return next();
      } else {
        return res.status(400).json({ status: "Fail", message: "File not provided in the request" });
      }
    },      // Continue to the route handler for ipfile_add
    ipfile_add
);

router.get("/list", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], ipfile_list);
// router.delete("/delete", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)],  ipfile_delete);

export default router;
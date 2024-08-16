import { Router } from 'express';
import { blog_list, blog_add, blog_update, blog_delete } from 'controllers/blogs';
import { checkJwt } from 'middleware/checkJwt';
import { checkRole } from 'middleware/checkRole';

const router = Router();

router.get("/list", blog_list);
router.post("/add", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], blog_add);
router.post("/update", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)],  blog_update);
router.delete("/delete", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)],  blog_delete);

export default router;
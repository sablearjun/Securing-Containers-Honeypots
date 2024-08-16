import { Router } from 'express';
import { table_list, config_list, config_add, config_update, config_delete } from 'controllers/config';
import { checkJwt } from 'middleware/checkJwt';
import { checkRole } from 'middleware/checkRole';

const router = Router();

router.get("/table_name", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], table_list);
router.get("/list", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], config_list);
router.post("/add", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], config_add);
router.post("/update", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)],  config_update);
router.delete("/delete", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)],  config_delete);

export default router;
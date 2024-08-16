import { Router } from 'express';
import { tables_list, rule_list, rule_add, rule_update, rule_delete } from 'controllers/python_rule';
import { checkJwt } from 'middleware/checkJwt';
import { checkRole } from 'middleware/checkRole';

const router = Router();
router.get("/table_list", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], tables_list);
router.get("/list_python_rule", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], rule_list);
router.post("/add_python_rule", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], rule_add);
router.post("/update_python_rule", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], rule_update);
router.delete("/delete_python_rule", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], rule_delete);

export default router;
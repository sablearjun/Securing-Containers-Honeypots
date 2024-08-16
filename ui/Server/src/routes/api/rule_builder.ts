import { Router } from 'express';
import { rule_builder_rule_list, rule_builder_rule_add, rule_builder_rule_update, rule_builder_rule_delete } from 'controllers/rule_builder';
import { checkJwt } from 'middleware/checkJwt';
import { checkRole } from 'middleware/checkRole';

const router = Router();

router.get("/rule_list", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], rule_builder_rule_list);
router.post("/rule_add", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], rule_builder_rule_add);
router.post("/rule_update", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], rule_builder_rule_update);
router.delete("/rule_delete", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], rule_builder_rule_delete);

export default router;
import { Router } from 'express';
import { stat_active_nodes_count, stat_nodes_count, stat_node_rule_count, stat_max_severity_node_count } from 'controllers/stat';
import { checkJwt } from 'middleware/checkJwt';
import { checkRole } from 'middleware/checkRole';

const router = Router();

router.get("/active_node_count", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], stat_active_nodes_count);
router.get("/nodes_count", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], stat_nodes_count);
router.get("/node_rule_count", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], stat_node_rule_count);
router.get("/max_severity_node_count", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], stat_max_severity_node_count);

export default router;
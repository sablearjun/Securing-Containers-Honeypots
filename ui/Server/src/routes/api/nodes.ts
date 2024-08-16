import { Router } from 'express';
import { node_log_list, node_programs_list, nodes_config, nodes_delete, nodes_health, nodes_historical_data, nodes_ip, nodes_list, nodes_memory_health, nodes_query, nodes_reenroll, vul_programs_list } from 'controllers/nodes';
import { checkJwt } from 'middleware/checkJwt';
import { checkRole } from 'middleware/checkRole';

const router = Router();

router.get('/log_list', [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], node_log_list);
router.post('/config', [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], nodes_config);
router.get('/delete', [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], nodes_delete);
router.get('/health', [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], nodes_health);
router.post('/historical_data', [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], nodes_historical_data);
router.get('/nodes_ip', [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], nodes_ip);
router.get('/list', [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], nodes_list);
router.get('/memory_health', [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], nodes_memory_health);
router.get('/query', [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], nodes_query);
router.post('/reenroll', [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], nodes_reenroll);
router.get('/programs_list', [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], node_programs_list);
router.get('/vul_programs_list', vul_programs_list);

export default router;
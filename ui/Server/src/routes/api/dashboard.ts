import { Router } from 'express';
import { nodes_count, events_count, os_count, severity_count, mitre_analysis, alert_count, high_risk_count, node_alert_count, weekly_alerts_count, nodes_count_sse } from 'controllers/dashboard';
import { checkJwt } from 'middleware/checkJwt';
import { checkRole } from 'middleware/checkRole';
import { validatorEdit } from 'middleware/validation/users';


const router = Router();
const cors = require('cors');
const corsOptions = {
    origin : process.env.FRONTEND_URL,
};
router.get('/nodes_count' ,[checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], nodes_count);
router.get('/nodes_count/sse',cors(corsOptions), nodes_count_sse);
router.get('/events_count',[checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], events_count);
router.get('/os_count' , [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], os_count);
router.get('/weekly_alerts_count' , [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], weekly_alerts_count);
router.get('/severity_count' , [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], severity_count);
router.get('/mitre_analysis',[checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], mitre_analysis);
router.get('/alert_count', [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], alert_count);
router.get('/high_risk_count', [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], high_risk_count);
router.get('/node_alert_count' , [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], node_alert_count);


export default router;
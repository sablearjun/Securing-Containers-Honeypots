import { Router } from 'express';
import { schedule_query_send, schedule_query_status, schedule_query_response } from 'controllers/scheduled_queries';
import { checkJwt } from 'middleware/checkJwt';
import { checkRole } from 'middleware/checkRole';

const router = Router();

router.post("/schedule", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], schedule_query_send);
router.post("/status", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], schedule_query_status);
router.post("/response", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], schedule_query_response);

export default router;
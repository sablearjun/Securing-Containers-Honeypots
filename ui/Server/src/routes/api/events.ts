import { Router } from 'express';
import { event_list, event_query, event_update, ept, event_list_sse, event_assignee, event_myalert } from 'controllers/events';
import { checkJwt } from 'middleware/checkJwt';
import { checkRole } from 'middleware/checkRole';
import { event_process_tree } from 'controllers/events/event_process_tree';

const router = Router();

router.get('/list', [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], event_list);
router.get('/query',[checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], event_query);
router.post('/update', [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], event_update);
// router.get('/graph', [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], event_process_tree);
router.post('/ept', [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], ept);
router.get('/list/sse', [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], event_list_sse);
router.get('/assignee', [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], event_assignee);
router.get('/list/myalert', [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], event_myalert);
export default router;
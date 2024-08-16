import { Router } from 'express';
import { get_tactic, get_techniques, get_sub_technique } from 'controllers/threat_hunting';
import { checkJwt } from 'middleware/checkJwt';
import { checkRole } from 'middleware/checkRole';

const router = Router();

router.get('/tactic', [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], get_tactic);
router.get('/technique', [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], get_techniques);
router.get('/sub_technique', [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], get_sub_technique);
// router.get('threats/', ThreatViewList.as_view()),
// router.get('threat/<int:pk>/', ThreatView.as_view()),

export default router;
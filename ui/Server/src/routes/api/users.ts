import { Router } from 'express';

import { user_list, user_delete, team_list, team_member_list, team_admin_list, assign_events, make_team, edit_permission, add_member, add_admin, user_details, team_deletion, user_update } from 'controllers/users';
import { checkJwt } from 'middleware/checkJwt';
import { checkRole } from 'middleware/checkRole';
import { validatorEdit } from 'middleware/validation/users';

const router = Router();

router.get('/user_list', [checkJwt, checkRole(['ADMINISTRATOR'], true)], user_list);
router.get('/user_details', [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], user_details);
router.post('/user_update', [checkJwt, checkRole(['ADMINISTRATOR'], true)], user_update);
router.delete('/user_delete', [checkJwt, checkRole(['ADMINISTRATOR'], true)], user_delete);

/*      ACCESS CONTROL      */

router.get('/team_list', [checkJwt, checkRole(['ADMINISTRATOR'], true)], team_list);
router.get('/team_member_list', [checkJwt, checkRole(['ADMINISTRATOR'], true)], team_member_list);
router.get('/team_admin_list', [checkJwt, checkRole(['ADMINISTRATOR'], true)], team_admin_list);
router.post('/assign_event', assign_events);
router.post('/make_team', [checkJwt, checkRole(['ADMINISTRATOR'], true)], make_team);
router.post('/edit_permission', [checkJwt, checkRole(['ADMINISTRATOR'], true)], edit_permission);
router.post('/add_member', [checkJwt, checkRole(['ADMINISTRATOR'], true)], add_member);
router.post('/add_admin', [checkJwt, checkRole(['ADMINISTRATOR'], true)], add_admin);
router.delete('/team_deletion', [checkJwt, checkRole(['ADMINISTRATOR'], true)], team_deletion);

export default router;

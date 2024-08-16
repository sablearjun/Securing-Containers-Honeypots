import { Router } from 'express';
import { list } from 'controllers/backend_rules';
import { checkJwt } from 'middleware/checkJwt';
import { checkRole } from 'middleware/checkRole';

const router = Router();

// router.get("/list", [checkJwt, checkRole(['ADMINISTRATOR', 'STANDARD'], true)], list);
router.get("/list", list);

export default router;
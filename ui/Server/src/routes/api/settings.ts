import { Router } from 'express';
import { data_retention_days, data_retention, manual_purge, data_retention_days_update } from 'controllers/settings';
import { checkJwt } from 'middleware/checkJwt';
import { checkRole } from 'middleware/checkRole';

const router = Router();

router.get("/data_retention_days", [checkJwt, checkRole(['ADMINISTRATOR'], true)], data_retention_days);
router.post("/data_retention_days/update", [checkJwt, checkRole(['ADMINISTRATOR'], true)], data_retention_days_update);
router.patch("/data_retention", [checkJwt, checkRole(['ADMINISTRATOR'], true)], data_retention);
router.delete("/manual_purge", [checkJwt, checkRole(['ADMINISTRATOR'], true)], manual_purge);

export default router;
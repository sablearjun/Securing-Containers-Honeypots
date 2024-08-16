import { Router } from 'express';

import page404 from './pages/404';

import api from './api';

const router = Router();

router.use(`/`, api);

router.use(page404);

export default router;

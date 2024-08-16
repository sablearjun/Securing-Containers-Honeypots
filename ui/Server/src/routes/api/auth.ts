import { Router } from 'express';

import { login, register, changePassword, renew_access_token, logout} from 'controllers/auth';
import { checkJwt } from 'middleware/checkJwt';
import { validatorLogin, validatorRegister, validatorChangePassword } from 'middleware/validation/auth';

const router = Router();

router.post('/login', [validatorLogin], login);
router.post('/register', [validatorRegister], register);
router.post('/change-password', [checkJwt, validatorChangePassword], changePassword);
router.post('/renew_access_token', [checkJwt], renew_access_token),
router.post('/logout', [checkJwt], logout)

export default router;

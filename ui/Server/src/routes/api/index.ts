import { Router } from 'express';

import auth from './auth';
import users from './users';
import backend_rules from './backend_rules';
import blogs from './blogs';
import config from './config';
import dashboard from './dashboard';
import ip from './ip';
import python_rule from './python_rule';
import rule_builder from './rule_builder';
import scheduled_queries from './scheduled_queries';
import settings from './settings';
import stat from './stat';
import threat_hunting from './threat_hunting';
import nodes from './nodes';
import event from './events';
import yara from './yara';

const router = Router();

router.get('/', (req, res)=>{
	res.status(200).send("Hey! What's up?");
});

router.use('/auth', auth);
router.use('/api', users);
router.use('/backend_rules', backend_rules);
router.use('/blogs', blogs);
router.use('/config', config);
router.use('/dashboard', dashboard);
router.use('/ip', ip);
router.use("/python_rules", python_rule);
router.use("/rule_builder", rule_builder);
router.use("/scheduled_queries", scheduled_queries);
router.use("/settings", settings);
router.use("/stat", stat);
router.use("/threat_hunting", threat_hunting);
router.use("/node", nodes);
router.use("/event", event);
router.use("/yara", yara);

export default router;

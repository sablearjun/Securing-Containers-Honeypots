#ifndef FLEET_BLOCKING_RULES_H
#define FLEET_BLOCKING_RULES_H

#include "../../common/EdrDataTypes.h"

bool RedisModifyingFiles(const BlockingEvent& event);

#endif //FLEET_BLOCKING_RULES_H

#ifndef ADMIN_LISTENER_H
#define ADMIN_LISTENER_H

#include "json/json.h"

void admin_message(JSONObj *msg, JSONObj *from, JSONObj *chat);

#endif


#ifndef MAIN_H
#define MAIN_H

#include "structs.h"

int v_id;
char v_username[username_size];
int location;
int level[99];
NPC NPCs[12000];
Object objects[15000];
int settings_selections[2];
skill skills[skill_count];

char *avail_actions[MAX_AVAIL_ACTIONS];

#endif
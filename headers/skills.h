#ifndef SKILLS_H
#define SKILLS_H

#include "structs.h"

Last_Xp_Drop last_xp_drop;
int use_skill_spot();
void init_skill_funcs();
int (*train_skill[skill_count])(int, int);

#endif
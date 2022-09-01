#ifndef LOCATIONS_H
#define LOCATIONS_H

#include "global.h"
#include "structs.h"

Location locations[MAX_LOCATIONS];

int objs_at_loc[MAX_LOCATIONS][MAX_OBJS_AT_LOCATION];

int npcs_at_loc[MAX_LOCATIONS][MAX_NPCS_AT_LOCATION];

Spwnd_Item_At_Loc items_at_loc[MAX_LOCATIONS][MAX_ITEMS_AT_LOCATION];

Tmp_Obj_At_Loc tmp_objs_at_loc[15][MAX_TMP_OBJS_AT_LOCATION];

Tmp_Item_At_Loc tmp_items_at_loc[15][MAX_TMP_ITEMS_AT_LOCATION];

#endif
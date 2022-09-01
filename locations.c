#include "headers/locations.h"
#include "headers/structs.h"

Location locations[MAX_LOCATIONS] = {
    { .name = "Intro - How'd you see this?\n\nLet me know at:\ndevon@devonmartin.net", .outside = false },
    { .name = "Gielinor Guide's House", .outside = true },
    { .name = "Outside Gielinor Guide's House", .outside = true },
    { .name = "Outside Master Chef's House", .outside = true }
};

int objs_at_loc[MAX_LOCATIONS][MAX_OBJS_AT_LOCATION] = {
    { -1 },
    { 3, 103, 346, 348, 374, 380, 596, 683, 873, 887, 888, 1090, 1096, 1158, 4618, -1 },
    { 3, 37, 42, 1104, 1162, 1169, 1187, 1192, 1193, 1276, 8462, -1 },
    { 3, 37, 154, 362, 1169, 1187, 1188, 1195, 1276, 1313, 3309, 8462, -1 }
};

int npcs_at_loc[MAX_LOCATIONS][MAX_NPCS_AT_LOCATION] = {
    { -1 },
    { 3308, -1 },
    { 8503, -1 },
    { 234, 235, -1 }
};

Spwnd_Item_At_Loc items_at_loc[MAX_LOCATIONS][MAX_ITEMS_AT_LOCATION] = {
    { { -1, -1 } },
    { { -1 } },
    { { -1 } },
    { { -1 } }
};

Tmp_Obj_At_Loc tmp_objs_at_loc[15][MAX_TMP_OBJS_AT_LOCATION] = {
    {{ .id = -1, .expiration = -1 }},
    {{ .id = -1, .expiration = -1 }},
    {{ .id = -1, .expiration = -1 }},
    {{ .id = -1, .expiration = -1 }}
};

Tmp_Item_At_Loc tmp_items_at_loc[15][MAX_TMP_ITEMS_AT_LOCATION] = {
    {{ .id = -1, .amount = 0, .expiration = -1 }},
    {{ .id = -1, .amount = 0, .expiration = -1 }},
    {{ .id = -1, .amount = 0, .expiration = -1 }},
    {{ .id = -1, .amount = 0, .expiration = -1 }}
};
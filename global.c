#include "game_functions.h"
#include "global.h"
#include "ids.h"

// Store all item data by id
item items[20000];
// Store xp required for each level
int level[99];
// Store character location
char location[50];
// Store all object data by id
object objects[15000];
// Store user's skill data
skill skills[skill_count];
// Store user's id and credentials
int v_id;
char v_password[password_size];
char v_username[username_size];
#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdbool.h>

typedef struct Location {
    char *name;
    bool outside;
} Location;

typedef struct Object {
    char *name;
    int id;
    char *examine;
    bool is_openable;
    bool is_searchable;
    bool is_skillable;
    int skill;
    int (*use_function)(int);
} Object;

typedef struct Item {
    char *name;
    int id;
    char *examine;
    bool is_stackable;
    bool is_notable;
    int note_id;
    bool is_wieldable;
    bool is_eatable;
    bool is_drinkable;
    bool is_interactable;
    int (*use_fuction)(int, int, int);
} Item;

typedef struct Spwnd_Item_At_Loc {
    int id;
    int amount;
} Spwnd_Item_At_Loc;

typedef struct Inv_Slot {
    int id;
    int amount;
} Inv_Slot;

// Precise xp is needed because xp to the 1/10th is tracked.
// The decimal is never displayed to the player. 
typedef struct skill
{
    char name[20];
    int id;
    unsigned int precise_xp;
    unsigned int real_xp;
    unsigned int next_level_xp;
    int level;
} skill;

typedef struct Skill_Element {
    int low;
    int high;
    int req;
} Skill_Element;

typedef struct Last_Xp_Drop {
    int skill;
    int xp;
} Last_Xp_Drop;

typedef struct NPC {
    char *name;
    int id;
    char *examine;
    bool talk;
    void (*talk_func)();
    int combat_level;
    bool attack;
} NPC;

typedef struct Tmp_Obj_At_Loc {
    int id;
    int expiration;
} Tmp_Obj_At_Loc;

typedef struct Tmp_Item_At_Loc {
    int id;
    int amount;
    int expiration;
} Tmp_Item_At_Loc;


#endif
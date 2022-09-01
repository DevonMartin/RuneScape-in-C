#ifndef GENERAL_MENUS_H
#define GENERAL_MENUS_H

#include "objects.h"
#include "main.h"

int display_options();
int more_actions();
int print_skills();
int skills_menu();
int print_location();
int inventory_menu();
void print_inventory(int green_index);

extern skill skills[skill_count];

#endif
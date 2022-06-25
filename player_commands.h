#ifndef PLAYER_COMMANDS_H
#define PLAYER_COMMANDS_H

#include "game_functions.h"
#include "ids.h"

int do_something(char input[], skill skills[], char location[], object objects[], int v_id);
int gielinor_guide_callback(void *NotUsed, int argc, char **argv, char **azColName);

int gielinor_guide_1;
int gielinor_guide_2;

#endif
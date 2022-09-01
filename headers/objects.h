#ifndef OBJECTS_H
#define OBJECTS_H

// #include "game_functions.h"
#include "global.h"
#include "locations.h"
#include "structs.h"

void init_objects();

int examine_object();

int search_object();
void (*search_obj[15000])();
void init_search();
void search_380(void);

int open_object();
void (*open_obj[15000])();
void init_open();
void open_3();
void open_37();
void open_103();
void open_348();

#endif
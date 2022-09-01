#ifndef GAME_FUNCTIONS_H
#define GAME_FUNCTIONS_H

#include "global.h"
#include "structs.h"

int tmp_num_retrieval;
char tmp_str[TMP_STR_SIZE];

void clearstdin(char *string);
void finput(char *input, int size);
int clearscreen();
void delay(float time);
void dialogue();
void print_blue(char *s);
void print_green(char *s);
void print_red(char *s);
void print_yellow(char *s);
double skill_success_odds(Skill_Element element, int level);
double cascade_skill_success_odds(Skill_Element elements[], int element_index, int level);
int current_time();
void clear_sql_tables();
int load_skills();
int load_inventory();
int update_item_from_slot(int slot, int item_id, int amount);
int load_settings();
int settings_menu();
int get_argc_from_db(char *table);
int update_num_in_table(char *table, char *row, int val);
int update_location(int new_loc);
int update_and_print_location(int new_loc);
int update_location_state(int location, int state);
int get_location_state(int location);
void initialize_game();

#endif
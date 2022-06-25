#ifndef GAME_FUNCTIONS_H
#define GAME_FUNCTIONS_H

void finput(char *input, int size);
void clearscreen();
void clearstdin(char *s);

// Precise xp is needed because xp to the 1/10th is tracked.
// The decimal is never displayed to the player. 
typedef struct skill
{
    char name[20];
    unsigned int precise_xp;
    unsigned int real_xp;
    unsigned int next_level_xp;
    int level;
} skill;

void initialize_level_calc(int level[]);
char update_location(int id, char *location);
void delay(float time);
void dialogue();
char new_player_intro(char *username, int id);
int load_character(int char_id);
int load_gielinor_guide(int v_id);
int gielinor_guide_callback(void *NotUsed, int argc, char **argv, char **azColName);
int update_table(char *row, char *table, int v_id);

#endif
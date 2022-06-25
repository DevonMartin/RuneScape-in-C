#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "login.h"
#include "player_commands.h"

extern item items[20000];
extern int level[99];
extern char location[50];
extern object objects[15000];
extern skill skills[skill_count];
extern int v_id;
extern char v_username[username_size];

int main(int argc, char *argv[])
{
    // Ensure usage is correct
    if (argc != 1)
    {
        printf("Usage: ./main\nrunescape.c error 1\n");
        return 1;
    }

    // Login() verifies credentials and returns user id
    v_id = login();
    if (v_id <= 0)
    {
        printf("runescape.c error 2\n");
        return 2;
    }

    // Initialize data for game and retrieve data for player's character
    initialize_level_calc(level);
    initialize_objects(objects);
    load_character(v_id);
    printf("Welcome to RuneScape, %s!\n\n", v_username);

    // If player is in spawn location, run introduction
    if (strcmp(location, "Tutorial Island Spawn") == 0)
    {
        if (new_player_intro(v_username, v_id) == '~')
        {
            printf("runescape.c error 3\n");
            return 3;
        }
    }

    char p_cmnd[35];
    do
    {
        // Load character data every time a command occurs to keep
        // up to date on changes from previous commands
        load_character(v_id);
        delay(2 * tick);
        // Run commands at user's request
        printf("What would you like to do?\n\n");
        finput(p_cmnd, sizeof(p_cmnd));
    } while (do_something(p_cmnd, skills, location, objects, v_id) == 0);

    // When loop is finally broken, user requested log out
    clearscreen();
    printf("Thank you for playing RuneScape, %s.\n", v_username);
    printf("See you next time!\n\n");
    dialogue();
    clearscreen();
    return 0;
}

// Callback function used to load player_stats db into memory
int load_char_callback(void *NotUsed, int argc, char **argv, char **azColName)
{    
    NotUsed = 0;

    if (argc > 0)
    {
        // Retrieve username and location
        strcpy(v_username, argv[1]);
        strcpy(location, argv[argc - 1]);
        // Retrieve each skill xp, and use it to populate Skills[]
        for (int i = 4; i < argc - 1; i++)
        {
            strcpy(skills[i-4].name, azColName[i]);
            skills[i-4].precise_xp = atoi(argv[i]);
            skills[i-4].real_xp = skills[i-4].precise_xp / 10;
            for (int j = 0; j < 99; j++)
            {
                if (skills[i-4].real_xp >= level[j])
                    continue;
                skills[i-4].level = j;
                skills[i-4].next_level_xp = level[j];
                break;
            }
        }
        return 0; // If all goes smoothly, return 0 for success
    }
    return -1; // If player_stats cannot be loaded, something went wrong. Return -1.
}

/** Project Goals:
 *
 * -Continue implementation until Tutorial Island
 *  is completable and Lumbridge is fully functional.
 * 
 * -Include all f2p skills, Lumbridge NPCs, at least
 *  one quest, combat and monster drop system, shop
 *  funtionality, and bank functionality.
 * 
 * -Land a job lol.
 * 
 * -At time of project submission, this project contains
 *  1,555 lines of code across 12 files, an imported
 *  sqlite3 .c file, and a relational database with 3
 *  linked tables storing user/character data.
 * 
 */
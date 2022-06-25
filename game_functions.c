#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "global.h"
#include "player_commands.h"
#include "runescape.h"
#include "sqlite3.h"

// Gets input from user, clears stdin, sends input to lowercase.
// Size should ALWAYS be sizeof(input).
void finput(char *input, int size)
{
    fgets(input, size, stdin);
    clearstdin(input);
    // Make response lowercase and remove \n char
    for (int i = 0; i < size; i++)
    {
        input[i] = tolower(input[i]);
        if (!isalnum(input[i]) && input[i] != ' ' && input[i] != '\'')
        {
            input[i] = 0;
        }
    }
}

// Clear screen based on user's device
void clearscreen()
{
    #ifdef _WIN32
        system("cls");
    #elif defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
        system("clear");
    #else
        #error "OS not supported."
    #endif
}

// Clears stdin after user is prompted for input to ensure
// no leftover characters have a future influence.
// string should ALWAYS be name of input just received.
void clearstdin(char *string)
{
    if (!strchr(string, '\n'))
    {
        while (fgetc(stdin) != '\n');
    } 
}

/* Create skill level array w/ xp req. for next level as value of index:
skill level. Formula for level calculation obtained from Jard_Y_Dooku at
https://forum.tip.it/topic/263459-runescape-experience-formula/ */
void initialize_level_calc(int level[])
{
    double total = 0;
    for (int i = 1; i < 99; i++)
    {
        total += floor(i + 300 * pow(2, i / 7.0));
        level[i] = floor(total / 4);
    }
}

char update_location(int id, char *location)
{
    // Open database and check that it was successful.
    sqlite3 *db;
    int rc;
    rc = sqlite3_open("runescape.db", &db);
    if (rc != SQLITE_OK)
    {
        printf("game_functions.c error 1\n");
        sqlite3_close(db);
        return '~';
    }
    char *err_msg = 0;

    // Insert users selected data to the db
    char *tmp = "UPDATE player_stats SET location = \"\" WHERE player_id = ";
    char sql1[strlen(tmp) + strlen(location) + sizeof(id)];
    snprintf(sql1, strlen(tmp) + strlen(location) + sizeof(id), "UPDATE player_stats SET location = \"%s\" WHERE player_id = %i", location, id);
    rc = sqlite3_exec(db, sql1, 0, 0, &err_msg);
    // If rc errors, something unexpected happened. Fatal Error
    if (rc != SQLITE_OK)
    {
        printf("game_functions.c error 2\n");
        sqlite3_close(db);
        return '~';
    }
    return ' ';
}

// Delays running of code for desired number of ticks.
// Each in-game "tick" is 0.6 seconds.
void delay(float time)
{
    // Converting time into milliseconds
    int milli_seconds = 1000000 * time;
  
    // Storing start time
    clock_t start_time = clock();
  
    // Constantly checking current time until it's later than start time + delay time
    while (clock() < start_time + milli_seconds);
}

// Requires user input to continue through code
void dialogue()
{
    char tmp[1];
    fgets(tmp, sizeof(tmp), stdin);
    clearstdin(tmp);
}

// Run the initial dialogue to explain the controls briefly for new players.
char new_player_intro(char *username, int id)
{
    delay(4 * tick);
    printf("Getting started\n");
    printf("---------------------------------------\n");
    printf("Before you begin, let's go over some of\n");
    printf("the basic controls of the game.\n");
    delay(5 * tick);
    printf("\nTo continue through dialogue, press the\n");
    printf("\"Return\" or \"Enter\" key on your keyboard\n");
    printf("while focused on the terminal window.\n");
    printf("Give it a shot now!\n");
    dialogue();
    clearscreen();
    printf("Way to go, %s!\n", username);
    printf("You're already getting the hang of this.\n");
    dialogue();
    clearscreen();
    printf("In RuneScape, everything you do will be\n");
    printf("per your response to a prompt in the terminal.\n");
    printf("\"What would you like to do?\"\n");
    dialogue();
    clearscreen();
    printf("There are many different things you can do, such as:\n");
    dialogue();
    printf("-talk\n");
    delay(1 * tick);
    printf("-walk\n");
    delay(1 * tick);
    printf("-train\n");
    delay(1 * tick);
    printf("-and so much more!\n");
    dialogue();
    clearscreen();
    printf("If you ever feel lost, all you have to do\n");
    printf("is type \"Help\" for a list of available actions.\n");
    dialogue();
    printf("\nIf you think you've got it, it's time to start your adventure!\n");
    dialogue();
    clearscreen();
    printf("Are you ready?\n\n");
    printf("1. Yes\n2. No\n");
    printf("(No will restart this dialogue)\n\n");
    char input[5];
    bool tried = false;
    
    // Ensure user is ok with intro and ready to begin playing
    do
    {
        if (tried == true)
        {
            printf("Type \"1\" or \"2\"\n");
        }
        else
        {
            tried = true;
        }
        finput(input, sizeof(input));
    } while (strcmp(input, "1") != 0 && strcmp(input, "2") != 0);

    // If ok, update location and return for the start of the game
    if (strcmp(input, "1") == 0)
    {
        if (update_location(id, "Gielinor Guide's House") == '~')
        {
            return '~';
        }
        clearscreen();
        printf("\n");
        delay(2 * tick);
        for (int i = 0; i < 3; i++)
        {
            clearscreen();
            for (int j = 0; j <= i; j++)
            {
                printf(".");
            }
            printf("\n");
            delay(2 * tick);
        }
        delay(4 * tick);
        printf("Suddenly, you find yourself in the Gielinor Guide's House.\n\n");
        delay(2 * tick);
        return ' ';
    }

    // Else, rerun intro
    clearscreen();
    return new_player_intro(username, id);
}

// Load username, skills, quests, etc. from db for user
int load_character(v_id)
{
    sqlite3 *db;
    int rc;
    rc = sqlite3_open("runescape.db", &db);
    if (rc != SQLITE_OK)
    {
        printf("game_functions.c error 3\n");
        sqlite3_close(db);
        return -1;
    }
    char *err_msg = 0;

    // Setup and execute a search for the id obtained through login
    char *tmp = "SELECT * FROM player_info JOIN player_stats ON id = player_stats.player_id WHERE id = ;";
    char sql[strlen(tmp) + sizeof(int)];
    snprintf(sql, strlen(tmp) + sizeof(int), "SELECT * FROM player_info JOIN player_stats ON id = player_stats.player_id WHERE id = %i;", v_id);
    rc = sqlite3_exec(db, sql, load_char_callback, 0, &err_msg);

    // If rc errors, player_stats couldn't be loaded for some reason. Exit.
    if (rc != SQLITE_OK)
    {
        printf("game_functions.c error 4\n");
        return -1;
    }
    return 0;
}

// Load tutorial_island data for a player
int load_gielinor_guide(int v_id)
{
    // Open database and check that it was successful.
    sqlite3 *db;
    int rc;
    rc = sqlite3_open("runescape.db", &db);
    if (rc != SQLITE_OK)
    {
        printf("game_functions.c error 5\n");
        sqlite3_close(db);
        return 1;
    }
    char *err_msg = 0;

    // Setup and execute a search for the tutorial_island data of a user
    char *tmp = "SELECT * FROM tutorial_island WHERE player_id = ;";
    char sql[strlen(tmp) + sizeof(int)];
    snprintf(sql, strlen(tmp) + sizeof(int), "SELECT * FROM tutorial_island WHERE player_id = %i;", v_id);
    rc = sqlite3_exec(db, sql, gielinor_guide_callback, 0, &err_msg);
    // If rc errors, something fatally went wrong.
    if (rc != SQLITE_OK)
    {
        // Close db and return error
        sqlite3_close(db);
        printf("game_functions.c error 6\n");
        return 1;
    }

    // Close db and return successfully
    sqlite3_close(db);
    return 0;
}

// Callback function used to retrieve Gielinor Guide data for player
int gielinor_guide_callback(void *NotUsed, int argc, char **argv, char **azColName)
{    
    NotUsed = 0;
    if (argc != 0) // Check if the user data was found
    {
        gielinor_guide_1 = atoi(argv[1]);
        gielinor_guide_2 = atoi(argv[2]);
        return 0; // If found, load data and return successfully
    }
    else // If id not found, fatal error. Return 1.
    {
        return 1;
    }
}

// Set any row of any table to 1
int update_table(char *row, char *table, int v_id)
{
    // Open database and check that it was successful.
    sqlite3 *db;
    int rc;
    rc = sqlite3_open("runescape.db", &db);
    if (rc != SQLITE_OK)
    {
        printf("game_functions.c update_table error 1\n");
        sqlite3_close(db);
        dialogue();
        return 1;
    }
    char *err_msg = 0;

    // Update input data for a player
    char *tmp = "UPDATE  SET  = 1 WHERE ID = ;";
    char sql[strlen(tmp) + sizeof(int) + strlen(row)];
    snprintf(sql, strlen(tmp) + sizeof(int) + strlen(row), "UPDATE %s SET %s = 1 WHERE ID = %i;", table, row, v_id);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    // If rc errors, something unexpected happened. Fatal Error
    if (rc != SQLITE_OK)
    {
        printf("game_functions.c update_table error 2\n");
        sqlite3_close(db);
        dialogue();
        return 1;
    }
    return 0;
}

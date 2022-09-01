#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "headers/game_functions.h"
#include "headers/general_menus.h"
#include "headers/items.h"
#include "headers/npcs.h"
#include "headers/skills.h"
#include "sqlite3.h"


// Clears stdin after user is prompted for input to ensure
// no leftover characters have a future influence.
// string should ALWAYS be name of input just received.
void clearstdin(char *string) {
    if (!strchr(string, '\n'))
        while (fgetc(stdin) != '\n');
}

// Gets input from user, clears stdin, sends input to lowercase.
// Size should ALWAYS be sizeof(input).
void finput(char *input, int size) {
    fgets(input, size, stdin);
    clearstdin(input);
    // Make response lowercase and remove \n char
    for (int i = 0; i < size; i++) {
        input[i] = tolower(input[i]);
        if (!isalnum(input[i]) && input[i] != ' ' && input[i] != '\'') {
            input[i] = 0;
        }
    }
}

// Clear screen based on user's device
int clearscreen() {
    #ifdef _WIN32
        system("cls");
    #elif defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
        system("clear");
    #else
        #error "OS not supported."
    #endif
    return 0;
}

/* Create skill level array w/ xp req. for next level as value of index:
skill level. Formula for level calculation obtained from Jard_Y_Dooku at
https://forum.tip.it/topic/263459-runescape-experience-formula/ */
void initialize_level_calc(int level[]) {
    double total = 0;
    for (int i = 1; i < 99; i++) {
        total += floor(i + 300 * pow(2, i / 7.0));
        level[i] = floor(total / 4);
    }
}

// Delays running of code for desired number of ticks.
// Each in-game "tick" is 0.6 seconds.
void delay(float by_time_in_s) {
    // Converting time into milliseconds
    int seconds_of_delay = CLOCKS_PER_SEC * by_time_in_s;
  
    // Storing start time
    clock_t start_time = clock();
  
    // Constantly checking current time until it's later than start time + delay time
    while (clock() < start_time + seconds_of_delay);
}

// Requires user input to continue through code
void dialogue() {
    char tmp[1];
    fgets(tmp, sizeof(tmp), stdin);
    clearstdin(tmp);
}

void print_reset() {
  printf("\033[0m");
}

void print_blue(char *s) {
    printf("\033[1;34m");
    printf("%s", s);
    print_reset();
}
 
void print_green(char *s) {
    printf("\033[1;32m");
    printf("%s", s);
    print_reset();
}

void print_red(char *s) {
    printf("\033[1;31m");
    printf("%s", s);
    print_reset();
}

void print_yellow(char *s) {
    printf("\033[1;33m");
    printf("%s", s);
    print_reset();
}

double skill_success_odds(Skill_Element element, int level) {
    double val = floor(element.low * (99 - level) / 98) + floor(element.high * (level - 1) / 98) + 1;
    double chance = ((val / 256) > 0) ? (val / 256) : 0;
    return (chance < 1) ? chance : 1;
}

double cascade_skill_success_odds(Skill_Element elements[], int element_index, int level) {
    double chance = 1;
    for (int i = 0; i <= element_index; i++) {
        Skill_Element e = elements[i];
        if (i == element_index) {
            chance *= skill_success_odds(e, level);
            return chance;
        } else if (level >= e.req) {
            chance *= 1 - skill_success_odds(e, level);
        }
    }
    return -1;
}

// Returns number of seconds since Jan. 1
int current_time() {
    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);
    return (((((timeinfo->tm_mon * 24) + timeinfo->tm_hour) * 60) + timeinfo->tm_min) * 60) + timeinfo->tm_sec;
}

// Selects all data from any table and uses any callback function to process data.
// Called exclusively by other SQL functions. 
// E.g. load_skills returns SELECT_ALL_SQL("skills", load_skills_callback).
int SELECT_ALL_SQL(char *table, int (*callback)(void *NotUsed, int argc, char **argv, char **azColName)) {
    // Open database and check that it was successful.
    sqlite3 *db;
    int rc;
    rc = sqlite3_open("runescape.db", &db);
    if (rc != SQLITE_OK)
    {
        printf("game_functions.c SELECT_ALL_SQL error 1\n");
        sqlite3_close(db);
        dialogue();
        return -1;
    }
    char *err_msg = 0;

    // Setup and execute a search for the input table and row
    char sql[75];
    snprintf(sql, 55, "SELECT * FROM %s WHERE player_id = %i;", table, v_id);
    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    sqlite3_close(db);

    // If rc == -1, something fatally went wrong.
    if (rc == -1) printf("game_functions.c SELECT_ALL_SQL error 2\n");
    return rc;
}

int load_skills_callback(void *NotUsed, int argc, char **argv, char **azColName) {
    NotUsed = 0;

    if (argc > 0)
    {
        // Retrieve each skill xp, and use it to populate Skills[]
        for (int i = 1; i < argc; i++)
        {
            strcpy(skills[i-1].name, azColName[i]);
            skills[i-1].id = i-1;
            skills[i-1].precise_xp = atoi(argv[i]);
            skills[i-1].real_xp = skills[i-1].precise_xp / 10;
            for (int j = 0; j < 99; j++)
            {
                if (skills[i-1].real_xp >= level[j])
                    continue;
                skills[i-1].level = j;
                skills[i-1].next_level_xp = level[j];
                break;
            }
        }
        return 0; // If all goes smoothly, return 0 for success
    }
    return -1; // If stats cannot be loaded, something went wrong. Return -1.
}

// Load skills of character into memory
int load_skills() {
    return SELECT_ALL_SQL("stats", load_skills_callback);
}

// Util func to reset db for testing purposes. USE WITH CAUTION, WILL CLEAR ALL SAVED DATA!
void clear_sql_tables() {
    char *tables[20] = {
        "credentials",
        "sqlite_sequence",
        "stats",
        "locations",
        "settings",
        "inventory_slot",
        "stop"
    };

    int i = 0;
    while (strcmp(tables[i], "stop")) {
        sqlite3 *db;
        int rc;
        rc = sqlite3_open("runescape.db", &db);
        if (rc != SQLITE_OK)
        {
            printf("game_functions.c clear_sql_tables error %i.1\n", i);
            sqlite3_close(db);
            return;
        }
        char *err_msg = 0;

        // Setup and execute a search for the id obtained through login
        char sql[50];
        snprintf(sql, 50, "DELETE FROM %s;", tables[i]);
        rc = sqlite3_exec(db, sql, load_skills_callback, 0, &err_msg);
        sqlite3_close(db);

        // If rc errors, player_stats couldn't be loaded for some reason. Exit.
        if (rc != SQLITE_OK)
        {
            printf("game_functions.c clear_sql_tables error %i.2\n", i);
        }
        i++;
    }
    printf("Tables successfully cleared.\n");
    return;
}

int load_inventory_callback(void *NotUsed, int argc, char **argv, char **azColName) {
    NotUsed = 0;

    if (argc > 0)
    {
        inv_slots[atoi(argv[1])].id = atoi(argv[2]);
        inv_slots[atoi(argv[1])].amount = atoi(argv[3]);
        return 0; // If all goes smoothly, return 0 for success
    }
    return -1; // If inventory cannot be loaded, something went wrong. Return -1.
}

// Load inventory of character into memory
int load_inventory() {
    // Open database and check that it was successful.
    sqlite3 *db;
    int rc;
    rc = sqlite3_open("runescape.db", &db);
    if (rc != SQLITE_OK)
    {
        printf("game_functions.c SELECT_ALL_SQL error 1\n");
        sqlite3_close(db);
        dialogue();
        return -1;
    }
    char *err_msg = 0;

    // Setup and execute a search for the input table and row
    for (int i = 0; i < 28; i++) {
        char sql[85];
        snprintf(sql, 85, "SELECT * FROM inventory_slot WHERE player_id = %i and slot_num = %i;", v_id, i);
        rc = sqlite3_exec(db, sql, load_inventory_callback, 0, &err_msg);

        // If rc == -1, something fatally went wrong.
        if (rc == -1) {
            printf("game_functions.c SELECT_ALL_SQL error 2\n");
            sqlite3_close(db);
            dialogue();
            return rc;
        }
    }
    sqlite3_close(db);
    return 0;
}

int update_item_from_slot(int slot, int item_id, int amount) {
    // Open database and check that it was successful.
    sqlite3 *db;
    int rc;
    rc = sqlite3_open("runescape.db", &db);
    if (rc != SQLITE_OK)
    {
        printf("game_functions.c update_item_from_slot error 1\n");
        sqlite3_close(db);
        return 1;
    }
    char *err_msg = 0;

    // Update input data for a player
    char sql[135];
    snprintf(sql, 135, "UPDATE inventory_slot SET item_id = %i, item_count = %i WHERE player_id = %i AND slot_num = %i;", item_id, amount, v_id, slot);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    // If rc errors, something unexpected happened. Fatal Error
    if (rc != SQLITE_OK)
    {
        printf("game_functions.c update_item_from_slot error 2\n");
        sqlite3_close(db);
        return 1;
    }
    load_inventory();
    return 0;
}

int load_settings_callback(void *NotUsed, int argc, char **argv, char **azColName) {
    NotUsed = 0;

    if (argc > 0)
    {
        // Retrieve each setting, and use it to populate Settings[]
        for (int i = 1; i < argc; i++)
        {
            settings_selections[i-1] = atoi(argv[i]);
        }
        return 0; // If all goes smoothly, return 0 for success
    }
    return -1; // If stats cannot be loaded, something went wrong. Return -1.
}

int load_settings() {
    return SELECT_ALL_SQL("settings", load_settings_callback);
}
 
int settings_menu_callback(void *NotUsed, int argc, char **argv, char **azColName) {
    NotUsed = 0;
    if (argc != 0) // Check if the user data was found
    {
        bool tried;
        char ans_str[4];
        int ans, i;
        while (true) {
            clearscreen();
            print_blue("Settings Menu\n\n");
            for (i = 1; i < argc; i++) {
                char *col = azColName[i];
                for (int j = 0; j < strlen(col); j++) {
                    col[j] = (col[j] == '_' ? ' ' : col[j]);
                }
                printf("%i. %s: %s\n", i, col, (!strcmp(argv[i], "0") ? "off" : "on"));
            }
            printf("\n%i. Back\n\n", i);

            if (tried) printf("Invalid selection. Please choose a number above.\n\n");
            else tried = true;

            finput(ans_str, sizeof(ans_str));
            ans = atoi(ans_str);
            if (ans > 0 && ans <= i) break;
        }
        if (ans == i) {
            clearscreen();
            load_settings();
            return 0;
        } else {
            for (i = 0; i < strlen(azColName[ans]); i++) {
                if (azColName[ans][i] == ' ') azColName[ans][i] = '_';
            }
            strcpy(tmp_str, azColName[ans]);
            tmp_num_retrieval = 1 - atoi(argv[ans]);
            load_settings();
            return 1;
        }
    }
    else return -1;
}

// Get column names from a table
int settings_menu() {
    return SELECT_ALL_SQL("settings", settings_menu_callback);
}

int argc_retrieval_callback(void *NotUsed, int argc, char **argv, char **azColName) {
    NotUsed = 0;
    if (argc != 0) // Check if the user data was found
    {
        tmp_num_retrieval = argc;
        return 0; // If found, load data and return successfully
    }
    else // If data not found, fatal error. Return 1.
    {
        return 1;
    }
}

// Find argc of a table
int get_argc_from_db(char *table) {
    // Open database and check that it was successful.
    sqlite3 *db;
    int rc;
    rc = sqlite3_open("runescape.db", &db);
    if (rc != SQLITE_OK)
    {
        printf("game_functions.c get_argc_from_db error 1\n");
        sqlite3_close(db);
        dialogue();
        return 1;
    }
    char *err_msg = 0;

    // Setup and execute a search for the input table and row
    int max_size = 45 + strlen(table);
    char sql[max_size];
    snprintf(sql, max_size, "SELECT * FROM %s WHERE player_id = %i;", table, v_id);
    rc = sqlite3_exec(db, sql, argc_retrieval_callback, 0, &err_msg);
    // If rc errors, something fatally went wrong.
    if (rc != SQLITE_OK)
    {
        // Close db and return error
        sqlite3_close(db);
        printf("game_functions.c get_argc_from_db error 2\n");
        return -1;
    }

    // Close db and return the number retrieved
    sqlite3_close(db);
    return tmp_num_retrieval;
}

// Set any row of any table to an input number val
int update_num_in_table(char *table, char *row, int val)
{
    // Open database and check that it was successful.
    sqlite3 *db;
    int rc;
    rc = sqlite3_open("runescape.db", &db);
    if (rc != SQLITE_OK)
    {
        printf("game_functions.c update_num_in_table error 1\n");
        sqlite3_close(db);
        return 1;
    }
    char *err_msg = 0;

    // Update input data for a player
    char sql[125];
    snprintf(sql, 125, "UPDATE %s SET %s = %i WHERE player_id = %i;", table, row, val, v_id);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    // If rc errors, something unexpected happened. Fatal Error
    if (rc != SQLITE_OK)
    {
        printf("game_functions.c update_num_in_table error 2\n");
        sqlite3_close(db);
        return 1;
    }
    return 0;
}

int update_location(int new_loc) {
    // Open database and check that it was successful.
    sqlite3 *db;
    int rc;
    rc = sqlite3_open("runescape.db", &db);
    if (rc != SQLITE_OK)
    {
        printf("game_functions.c update_location error 1\n");
        sqlite3_close(db);
        return 1;
    }
    char *err_msg = 0;

    // Update input data for a player
    char sql[85];
    snprintf(sql, 85, "UPDATE locations SET state = %i WHERE player_id = %i AND location = -1;", new_loc, v_id);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    // If rc errors, something unexpected happened. Fatal Error
    if (rc != SQLITE_OK)
    {
        printf("game_functions.c update_location error 2\n");
        sqlite3_close(db);
        return 1;
    }
    location = new_loc;
    return 0;
}

int update_and_print_location(int new_loc) {
    if (update_location(new_loc) == 1) return 1;
    return print_location();
}

int update_location_state(int location, int state) {
    // Open database and check that it was successful.
    sqlite3 *db;
    int rc;
    rc = sqlite3_open("runescape.db", &db);
    if (rc != SQLITE_OK)
    {
        printf("game_functions.c update_location_state error 1\n");
        sqlite3_close(db);
        return 1;
    }
    char *err_msg = 0;

    // Update input data for a player
    char sql[85];
    snprintf(sql, 85, "UPDATE locations SET state = %i WHERE player_id = %i AND location = %i;", state, v_id, location);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    // If rc errors, something unexpected happened. Fatal Error
    if (rc != SQLITE_OK)
    {
        printf("game_functions.c update_location_state error 2\n");
        sqlite3_close(db);
        return 1;
    }
    return 0;
}

int get_location_state_callback(void *NotUsed, int argc, char **argv, char **azColName) {    
    NotUsed = 0;
    if (argc != 0) // Check if the user data was found
    {
        tmp_num_retrieval = atoi(argv[0]);
        return 0; // If found, load data and return successfully
    }
    else // If data not found, fatal error. Return 1.
    {
        return 1;
    }
}

int get_location_state(int location) {
    // Open database and check that it was successful.
    sqlite3 *db;
    int rc;
    rc = sqlite3_open("runescape.db", &db);
    if (rc != SQLITE_OK)
    {
        printf("game_functions.c get_location_state error 1\n");
        sqlite3_close(db);
        dialogue();
        return 1;
    }
    char *err_msg = 0;

    // Setup and execute a search for the input table and row
    char sql[80];
    snprintf(sql, 80, "SELECT state FROM locations WHERE player_id = %i AND location = %i;", v_id, location);
    rc = sqlite3_exec(db, sql, get_location_state_callback, 0, &err_msg);
    // If rc errors, something fatally went wrong.
    if (rc != SQLITE_OK)
    {
        // Close db and return error
        sqlite3_close(db);
        printf("game_functions.c get_location_state error 2\n");
        return -1;
    }

    // Close db and return the number retrieved
    sqlite3_close(db);
    return tmp_num_retrieval;
}

// Runs various initialization functions to generate data for the game
void initialize_game() {
    initialize_level_calc(level);
    init_objects();
    init_npcs();
    init_items();
    init_search();
    init_open();
    init_skill_funcs();
    load_inventory();
    load_settings();
    load_skills();
    srand(time(NULL));
}

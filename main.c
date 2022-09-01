#include <stdio.h>
#include <stdlib.h>

#include "headers/game_functions.h"
#include "headers/general_menus.h"
#include "headers/login.h"
#include "headers/npcs.h"


int main(int argc, char *argv[])
{
    // Ensure usage is correct
    if (argc != 1)
    {
        printf("Usage: ./main\nmain.c error 1\n");
        return 1;
    }

    // Login() verifies credentials and returns user id
    v_id = login();
    if (v_id <= 0)
    {
        printf("main.c error 2\n");
        return 2;
    }


    // Initialize data for game
    initialize_game();

    printf("Welcome to RuneScape, %s!\n\n", v_username);
    delay(2 * tick);
    location = get_location_state(-1);

    // If player is in spawn location, run introduction
    if (location == 0)
    {
        delay(4 * tick);
        if (new_player_intro(v_username, v_id) == '~')
        {
            printf("main.c error 4\n");
            return 4;
        }
    }

    while (true) {
        if (display_options() == 1) {
            break;
        }
    }

    // When loop is finally broken, user requested log out
    clearscreen();
    printf("Thank you for playing RuneScape, %s.\n", v_username);
    printf("See you next time!\n\n");
    dialogue();
    clearscreen();
    return 0;
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
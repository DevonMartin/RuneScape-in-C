#include <stdio.h>
#include <stdlib.h>

#include "headers/game_functions.h"
#include "headers/general_menus.h"
#include "headers/login.h"
#include "headers/npcs.h"

// 4,115 lines of code in .c files as of v2.0, 9/1/22

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
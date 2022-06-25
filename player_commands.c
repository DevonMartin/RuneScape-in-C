#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "game_functions.h"
#include "global.h"
#include "player_commands.h"

// Runs command per users request based on location
int do_something(char input[], skill skills[], char location[], object objects[], int v_id)
{
    // Basic commands available anywhere
    if (true)
    {
        // "help" doesn't return, because more will print depending on location.
        // Tells user what actions are available.
        if (strcmp(input, "help") == 0)
        {
            printf("\nEverywhere:\n");
            printf("\"Skills\" will tell you your level, xp, and xp to next level for each skill.\n");
            printf("\"Location\" will tell you where you're at.\n");
            printf("\"Clear Screen\" will clear your terminal window.\n");
            printf("\"Log Out\" will exit the game.\n");
        }
        
        // "skills" prints skill data for user
        else if (strcmp(input, "skills") == 0)
        {
            printf("\n\n");
            for (int i = 0; i < skill_count; i++)
            {
                printf("%s: Level %i. Xp: %i. Xp to %i: %i.\n", skills[i].name, skills[i].level, skills[i].real_xp, skills[i].level + 1, skills[i].next_level_xp - skills[i].real_xp);
            }
            printf("\n");
            return 0;
        }
        
        // "location" prints user's location
        else if (strcmp(input, "location") == 0)
        {
            printf("\n\n%s\n\n", location);
            return 0;
        }
        
        // "clear screen" clears the screen
        else if (strcmp(input, "clear screen") == 0)
        {
            clearscreen();
            return 0;
        }
        
        // "log out" ends the program
        else if (strcmp(input, "log out") == 0)
        {
            return 1;
        }
    }

    // Commands only applicable to Gielinor Guide's House
    if (strcmp(location, "Gielinor Guide's House") == 0)
    {
        // Load necessary data for progression
        if (load_gielinor_guide(v_id) == 1)
        {
            return 1;
        }

        // Print "Help" for actions performable in this location
        if (strcmp(input, "help") == 0)
        {
            printf("\nIn %s:\n", location);
            printf("\"Talk\" will let you interact with nearby NPCs (non-player characters).\n");
            printf("\"Examine\" will give you information on things in the world.\n");
            printf("\"Open\" will let you open things in the world.\n");
            printf("\"Search\" will let you search things in the world.\n\n");
            return 0;
        }

        // Run dialogue with NPCs in this location
        else if (strcmp(input, "talk") == 0)
        {
            char answer[25];
            do
            {
                clearscreen();
                printf("Who do you want to talk to?\n\n");
                printf("1. Gielinor Guide\n\n");
                finput(answer, sizeof(answer));
            } while (strcmp(answer, "1") != 0);


            if (gielinor_guide_1 == 0)
            {
                clearscreen();
                printf("Gielinor Guide\n\n");
                printf("Greetings! I see you are a new arrival to the world of\n");
                printf("Gielinor. My job is to welcome all new visitors. So\n");
                printf("welcome!\n");
                dialogue();
                clearscreen();
                printf("Gielinor Guide\n\n");
                printf("You have already learned the first thing needed to\n");
                printf("succeed in this world: talking to other people!\n");
                dialogue();
                clearscreen();
                printf("Gielinor Guide\n\n");
                printf("You will find many inhabitants of this world have useful\n");
                printf("things to say to you. By typing \"Talk\" you can talk\n");
                printf("to them.\n");
                dialogue();
                clearscreen();
                printf("Gielinor Guide\n\n");
                printf("Now then, let's start by looking at your settings menu.\n");
                dialogue();
                char answer2[10];
                do
                {
                    clearscreen();
                    printf("Settings menu\n\n");
                    printf("Please type \"Settings\" now. This will display your\n");
                    printf("settings menu.\n\n");
                    finput(answer2, sizeof(answer2));
                } while (strcmp(answer2, "settings") != 0);
                
                clearscreen();
                printf("Settings menu\n\n");
                printf("Normally you would now see a variety of game settings,\n");
                printf("but they haven't been implemented yet.\n");
                printf("Talk to the Gielinor Guide to continue.\n\n");
                dialogue();
                update_table("gielinor_guide_1", "tutorial_island", v_id);
                gielinor_guide_1 = 1;
                return 0;
            }

            else if (gielinor_guide_2 != 1)
            {
                clearscreen();
                printf("Gielinor Guide\n\n");
                printf("Looks like you're making good progress! The menu\n");
                printf("you've just opened is one of many. You'll learn about\n");
                printf("the rest as you progress through the tutorial.\n");
                dialogue();
                clearscreen();
                printf("Gielinor Guide\n\n");
                printf("Anyway, I'd say it's time for you to go and meet your\n");
                printf("first instructor!\n");
                update_table("gielinor_guide_2", "tutorial_island", v_id);
                gielinor_guide_2 = 1;
                dialogue();
                clearscreen();
                printf("Moving on\n\n");
                printf("It's time to meet your first instructor. To continue, all you need\n");
                printf("to do is \"Open\" the door. Remember, if you ever need help, all\n");
                printf("you need to do is type \"Help\".\n");
                dialogue();
                return 0;
            }

            else
            {
                clearscreen();
                printf("Gielinor Guide\n\n");
                printf("Welcome back. You have already learnt the first thing\n");
                printf("needed to succeed in this world: talking to other people!\n");
                dialogue();
                clearscreen();
                printf("Gielinor Guide\n\n");
                printf("You will find many inhabitants of this world have useful\n");
                printf("things to say to you. By typing \"Talk\", you can talk\n");
                printf("to them.\n");
                dialogue();
                clearscreen();
                printf("Gielinor Guide\n\n");
                printf("To continue the tutorial go through that door over\n");
                printf("there and speak to your first instructor!\n");
                dialogue();
                clearscreen();
                printf("Moving on\n\n");
                printf("It's time to meet your first instructor. To continue, all you need\n");
                printf("to do is \"Open\" the door. Remember, if you ever need help, all\n");
                printf("you need to do is type \"Help\".\n");
                dialogue();
                return 0;
            }
        }
            
        // Run examine command until successfully completed
        else if (strcmp(input, "examine") == 0)
        {
            char answer[25] = "";
            int tried = false;
            do
            {
                if (tried == true)
                {
                    finput(answer, sizeof(answer));
                }
                else
                {
                    tried = true;
                }
            } while (examine_object(location, answer, objects) != 0);

            return 0;
        }

        // Run open command until successfully completed
        else if (strcmp(input, "open") == 0)
        {
            char answer[25] = "";
            int tried = false;
            do
            {
                if (tried == true)
                {
                    finput(answer, sizeof(answer));
                }
                else
                {
                    tried = true;
                }
            } while (open_object(location, answer, objects, v_id) != 0);

            return 0;
        }

        // Run search command until successfully completed
        else if (strcmp(input, "search") == 0)
        {
            char answer[25] = "";
            int tried = false;
            do
            {
                if (tried == true)
                {
                    finput(answer, sizeof(answer));
                }
                else
                {
                    tried = true;
                }
            } while (search_object(location, answer, objects) != 0);

            return 0;
        }
    }

    // Commands only applicable to Outside Gielinor Guide's House
    else if (strcmp(location, "Outside Gielinor Guide's House") == 0)
    {
        printf("\nYou've completed the game. Way to go!\n\n");
        return 0;
    }
   
    // If no command triggered, inform user of options
    printf("\nTry typing what you want to do.\n");
    printf("Need help? Type \"Help\"\n\n");
    return 0;
}

int gielinor_guide_1 = 0;
int gielinor_guide_2 = 0;
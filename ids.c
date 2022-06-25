#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_functions.h"
#include "global.h"
#include "ids.h"

// Load data for items
void initialize_items(item items[])
{
    // TODO. No items used in initial release.
}

// Load data for objects by id
void initialize_objects(object objects[])
{
    objects[103].name = "Closed chest";
    objects[103].examine = "I wonder what's inside.";
    objects[346].name = "Cabinet";
    objects[346].examine = "The perfect place to store things.";
    objects[348].name = "Drawers";
    objects[348].examine = "These open and close!";
    objects[374].name = "Hat stand";
    objects[374].examine = "A stand for hats!";
    objects[380].name = "Bookcase";
    objects[380].examine = "A good source of books!";
    objects[596].name = "Table";
    objects[596].examine = "A banquet could be eaten from this.";
    objects[683].name = "Grandfather clock";
    objects[683].examine = "Tick-tock, it's a clock.";
    objects[873].name = "Sink";
    objects[873].examine = "Ideal for washing things in.";
    objects[887].name = "Portrait";
    objects[887].examine = "A painting of the King looking royal.";
    objects[888].name = "Painting";
    objects[888].examine = "A beautiful landscape.";
    objects[1090].name = "Chair";
    objects[1090].examine = "A comfortable seat.";
    objects[1096].name = "Rocking chair";
    objects[1096].examine = "Good for rocking in!";
    objects[1158].name = "Potted plant";
    objects[1158].examine = "A large potted plant.";
    objects[1533].name = "Door";
    objects[1533].examine = "A nicely fitted door.";
    objects[2724].name = "Fireplace";
    objects[2724].examine = "A fire burns brightly here.";
}

// Allow players to get examine info from objects at their location
int examine_object(char location[], char *obj, object objects[])
{
    int num_of_objs = 0;
    int avail_objs[50];
    // Initialize available objects per location of user's character
    if (strcmp(location, "Gielinor Guide's House") == 0)
    {
        int tmp[50] = { -1, 103, 346, 348, 374, 380, 596, 683, 873, 887, 888, 1090, 1096, 1158, 1533, 2724, -1 };

        avail_objs[0] = tmp[0];
        int i = 0;
        do
        {
            i++;
            num_of_objs++;
            avail_objs[i] = tmp[i];
        } while (tmp[i] != -1);
    }

    clearscreen();

    // If user selects an available object, print it's examine info
    int obj_num = atoi(obj);
    if (obj_num < num_of_objs && obj_num > 0)
    {
        printf("%s:\n%s\n\n", objects[avail_objs[obj_num]].name, objects[avail_objs[obj_num]].examine);
        return 0;
    }

    // If user input something other than an available object, reprompt them
    if (strcmp(obj, "") != 0)
    {
        printf("Please choose one of the available objects to examine.\n");
    }

    printf("What would you like to examine?\n\n");
    int j = 1;

    // Print every available object w/ it's index in avail_objs[]
    do
    {
        printf("%i. %s\n", j, objects[avail_objs[j]].name);
        j++;
    } while (avail_objs[j] != -1);

    printf("\n");
    return 1;
}

// Allow players to search objects at their location
int search_object(char location[], char *obj, object objects[])
{
    int num_of_objs = 0;
    int avail_objs[50];
    init_search();
    // Initialize available objects per location of user's character
    if (strcmp(location, "Gielinor Guide's House") == 0)
    {
        int tmp[50] = { -1, 380, -1 };

        avail_objs[0] = tmp[0];
        int i = 0;
        do
        {
            i++;
            num_of_objs++;
            avail_objs[i] = tmp[i];
        } while (tmp[i] != -1);

    }

    clearscreen();

    // If user selects an available object, run it's search functiom
    int obj_num = atoi(obj);
    if (obj_num < num_of_objs && obj_num > 0)
    {
        (*search_obj[avail_objs[obj_num]])();
        return 0;
    }

    // If user input something other than an available object, reprompt them
    if (strcmp(obj, "") != 0)
    {
        printf("Please choose one of the available objects to search.\n");
    }

    printf("What would you like to search?\n\n");
    int j = 1;

    // Print every available object w/ it's index in avail_objs[]
    do
    {
        printf("%i. %s\n", j, objects[avail_objs[j]].name);
        j++;
    } while (avail_objs[j] != -1);

    printf("\n");
    return 1;
}

// Initialize search_obj array of functions
void init_search()
{
    search_obj[380] = search_380;
}

// Result of searching each possible item. Searches arranged in array of functions:
// void (*search_obj[15000])()
void search_380(void)
{
    // Each result of searching bookshelf is 1/3 chance per search
    clearscreen();
    printf("You search the books...\n");
    delay(2 * tick);
    clearscreen();
    int i = (rand() % 3);
    if (i == 0)
    {
        printf("None of them look very interesting.\n");
    }
    else if (i == 1)
    {
        printf("You find nothing to interest you.\n");
    }
    else
    {
        printf("You don't find anything that you'd ever want to read.\n");
    }
    dialogue();
    clearscreen();
    return;
}

// Allow players to open objects at their location
int open_object(char location[], char *obj, object objects[], int v_id)
{
    int num_of_objs = 0;
    int avail_objs[50];
    init_open();
    // Initialize available objects per location of user's character
    if (strcmp(location, "Gielinor Guide's House") == 0)
    {
        int tmp[50] = { -1, 103, 348, 1533, -1 };

        avail_objs[0] = tmp[0];
        int i = 0;
        do
        {
            i++;
            num_of_objs++;
            avail_objs[i] = tmp[i];
        } while (tmp[i] != -1);

    }

    // If user selects an available object, run it's open functiom
    int obj_num = atoi(obj);
    if (obj_num < num_of_objs && obj_num > 0)
    {
        clearscreen();
        (*open_obj[avail_objs[obj_num]])(v_id, location);
        return 0;
    }

    clearscreen();

    // If user input something other than an available object, reprompt them
    if (strcmp(obj, "") != 0)
    {
        printf("Please choose one of the available objects to open.\n");
    }

    printf("What would you like to open?\n\n");
    int j = 1;

    // Print every available object w/ it's index in avail_objs[]
    do
    {
        printf("%i. %s\n", j, objects[avail_objs[j]].name);
        j++;
    } while (avail_objs[j] != -1);

    printf("\n");
    return 1;
}

// Initialize open_obj array of functions
void init_open()
{
    open_obj[103] = open_103;
    open_obj[348] = open_348;
    open_obj[1533] = open_1533;
}

// Result of opening each possible item. Opens arranged in array of functions:
// void (*open_obj[15000])()
void open_103()
{
    // Result of opening a chest
    printf("You open the chest and search it...\n");
    delay(2 * tick);
    printf("...but find nothing.\n");
    dialogue();
    clearscreen();
}
void open_348()
{
    // Result of opening drawers
    printf("You open the drawers...\n");
    delay(2 * tick);
    printf("...but they're empty.\n");
    dialogue();
    clearscreen();
}
void open_1533(int v_id, char location[])
{
    // Result of opening a door. Will vary depending on location and character data
    if (strcmp(location, "Gielinor Guide's House") == 0)
    {
        // If player has finished Gielinor Guide dialogue, allow them through
        // and finish the game
        if (gielinor_guide_2 == 1)
        {
            for (int i = 0; i < 3; i++)
            {
                printf("You open the door and walk outside");
                clearscreen();
                for (int j = 0; j <= i; j++)
                {
                    printf(".");
                }
                printf("\n");
                delay(2 * tick);
            }
            delay(2 * tick);
            clearscreen();
            printf("Congratulations!\n\n");
            printf("You've completed my remake of RuneScape built entirely in C.\n");
            dialogue();
            clearscreen();
            printf("Congratulations!\n\n");
            printf("Currently, there is no formal end to the game. You are still welcome\n");
            printf("to play with the controls. However, in your new location, your options\n");
            printf("are limited.\n");
            dialogue();
            clearscreen();
            printf("Congratulations!\n\n");
            printf("I greatly appreciate you taking the time to discover what has taken me\n");
            printf("many many hours to create, despite how fast you completed it. My\n");
            printf("level of satisfaction from and the complexity of coding continues to\n");
            printf("amaze me every day.\n");
            dialogue();
            clearscreen();
            printf("PS: Are you hiring?\n");
            dialogue();
            clearscreen();
            update_location(v_id, "Outside Gielinor Guide's House");
            gielinor_guide_2 = 0;
        }
        // Else, inform user of requirement for door
        else
        {
            printf("You need to talk to the Gielinor Guide before you are allowed to\n");
            printf("proceed through this door.\n\n");
            dialogue();
            clearscreen();
        }
    }
}

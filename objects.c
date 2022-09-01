#include <stdio.h>
#include <stdlib.h>

#include "headers/game_functions.h"
#include "headers/general_menus.h"
#include "headers/items.h"
#include "headers/objects.h"
#include "headers/main.h"
#include "headers/skills.h"

int nothing_interesting_obj(int inv_slot) {
    return 1;
}

int cook_on_fire(int inv_slot) {
    if (inv_slots[inv_slot].id != 317) return 1;
    train_skill[COOKING](inv_slot, 3769);
    return 0;
}

int full_table(int inv_slot) {
    printf("The table appears to be in use.\n\n");
    dialogue();
    clearscreen();
    return 0;
}

void init_object(char *name, int id, char *examine, bool is_openable, bool is_searchable, bool is_skillable, int skill, int (*use_function)(int inv_slot)) {
    Object obj = objects[id];
    obj.name = name;
    obj.id = id;
    obj.examine = examine;
    obj.is_openable = is_openable;
    obj.is_searchable = is_searchable;
    obj.is_skillable = is_skillable;
    obj.skill = skill;
    obj.use_function = use_function;
    objects[id] = obj;
}

// Load data for objects by id
void init_objects() {
    // Tutorial Island Door
    init_object("Door", 3, "A nicely fitted door.", true, false, false, -1, nothing_interesting_obj);
    // Tutorial Island Gate
    init_object("Gate", 37, "A wooden gate.", true, false, false, -1, nothing_interesting_obj);
    // Tutorial Island Fishing Spot - Shrimp
    init_object("Fishing spot", 42, "I can see fish swimming in the water.", false, false, true, FISHING, nothing_interesting_obj);
    init_object("Closed chest", 103, "I wonder what's inside.", true, false, false, -1, nothing_interesting_obj);
    init_object("Rocks", 154, "Stoney!", false, false, true, MINING, nothing_interesting_obj);
    init_object("Cabinet", 346, "The perfect place to store things.", false, false, false, -1, nothing_interesting_obj);
    init_object("Drawers", 348, "These open and close!", true, false, false, -1, nothing_interesting_obj);
    init_object("Barrel", 362, "A wooden barrel for storage.", false, false, false, -1, nothing_interesting_obj);
    init_object("Hat stand", 374, "A stand for hats!", false, false, false, -1, nothing_interesting_obj);
    init_object("Bookcase", 380, "A good source of books!", false, true, false, -1, nothing_interesting_obj);
    init_object("Table", 596, "A banquet could be eaten from this.", false, false, false, -1, full_table);
    init_object("Grandfather clock", 683, "Tick-tock, it's a clock.", false, false, false, -1, nothing_interesting_obj);
    init_object("Sink", 873, "Ideal for washing things in.", false, false, false, -1, nothing_interesting_obj);
    init_object("Portrait", 887, "A painting of the King looking royal.", false, false, false, -1, nothing_interesting_obj);
    init_object("Painting", 888, "A beautiful landscape.", false, false, false, -1, nothing_interesting_obj);
    init_object("Chair", 1090, "A comfortable seat.", false, false, false, -1, nothing_interesting_obj);
    init_object("Rocking chair", 1096, "Good for rocking in!", false, false, false, -1, nothing_interesting_obj);
    init_object("Bench", 1104, "Sit back and relax...", false, false, false, -1, nothing_interesting_obj);
    init_object("Potted plant", 1158, "A large potted plant.", false, false, false, -1, nothing_interesting_obj);
    init_object("Bullrushes", 1162, "Found near the water's edge.", false, false, false, -1, nothing_interesting_obj);
    init_object("Daisies", 1169, "Commonly found in grassy areas.", false, false, false, -1, nothing_interesting_obj);
    init_object("Flower", 1187, "A rarely found flower.", false, false, false, -1, nothing_interesting_obj);
    init_object("Flower", 1188, "That's pretty.", false, false, false, -1, nothing_interesting_obj);
    init_object("Flowers", 1192, "You don't see that many of these.", false, false, false, -1, nothing_interesting_obj);
    init_object("Flowers", 1193, "I wonder what these are?", false, false, false, -1, nothing_interesting_obj);
    init_object("Flowers", 1195, "Very rare flowers.", false, false, false, -1, nothing_interesting_obj);
    // Tutorial Island Tree
    init_object("Tree", 1276, "One of the most common trees in Gielinor.", false, false, true, WOODCUTTING, nothing_interesting_obj);
    init_object("Plant", 1313, "A leafy shrub.", false, false, false, -1, nothing_interesting_obj);
    init_object("Fire", 3769, "Hot!", false, false, false, -1, cook_on_fire);
    init_object("Rockslide", 3309, "A deposit of rocks.", false, false, false, -1, nothing_interesting_obj);
    init_object("Fireplace", 4618, "A fire burns brightly here.", false, false, false, -1, nothing_interesting_obj);
    // Tutorial Island Oak
    init_object("Oak", 8462, "A beautiful old oak.", false, false, true, WOODCUTTING, nothing_interesting_obj);
}

// Allow players to get examine info from objects at their location
int examine_object() {

    bool tried = false;
    char examinee_str[4];
    int examinee;
    int std_items, tmp_items, std_npcs, std_objs, tmp_objs, display;

    while (true) {

        clearscreen();
        print_blue("What would you like to examine?\n\n");
        std_items = 0, tmp_items = 0, std_npcs = 0, std_objs = 0, tmp_objs = 0, display = 1;

        if (items_at_loc[location][0].id != -1 || tmp_items_at_loc[location][0].id != -1) {
            print_blue("Items:\n");
            if (items_at_loc[location][0].id != -1) {
                while (items_at_loc[location][std_items].id != -1) {
                    printf("%s%i. %s\n", display > 9 ? "" : " ", display, items[items_at_loc[location][std_items].id].name);
                    std_items++;
                    display++;
                }
            }
            if (tmp_items_at_loc[location][0].id != -1) {
                while (tmp_items_at_loc[location][tmp_items].id != -1) {
                    printf("%s%i. %s\n", display > 9 ? "" : " ", display, items[tmp_items_at_loc[location][tmp_items].id].name);
                    tmp_items++;
                    display++;
                }
            }
            printf("\n");
        }

        if (npcs_at_loc[location][std_npcs] != -1) {
            print_blue("NPCs:\n");
            while (npcs_at_loc[location][std_npcs] != -1) {
                printf("%s%i. %s\n", display > 9 ? "" : " ", display, NPCs[npcs_at_loc[location][std_npcs]].name);
                std_npcs++;
                display++;
            }
            printf("\n");
        }

        if (objs_at_loc[location][std_objs] != -1 || tmp_objs_at_loc[location][tmp_objs].id != -1) {
            print_blue("Objects:\n");

            if (objs_at_loc[location][std_objs] != -1) {
                while (objs_at_loc[location][std_objs] != -1) {
                    printf("%s%i. %s\n", display > 9 ? "" : " ", display, objects[objs_at_loc[location][std_objs]].name);
                    std_objs++;
                    display++;
                }
            }
            if (tmp_objs_at_loc[location][tmp_objs].id != -1) {
                while (tmp_objs_at_loc[location][tmp_objs].id != -1) {
                    printf("%s%i. %s\n", display > 9 ? "" : " ", display, objects[tmp_objs_at_loc[location][tmp_objs].id].name);
                    tmp_objs++;
                    display++;
                }
            }
            printf("\n");
        }

        
        printf("%s%i. Back\n\n", display > 9 ? "" : " ", display);

        if (tried) printf("Invalid selection. Please choose a number above.\n\n");
        else tried = true;
        finput(examinee_str, sizeof(examinee_str));
        examinee = atoi(examinee_str) - 1;

        if (examinee < display && examinee >= 0) break;
    }


    clearscreen();
    if (examinee < display - 1 && examinee >= 0) {

        if (examinee < std_items) {
            examine_item(items_at_loc[location][examinee].id);
        } else if (examinee < std_items + tmp_items) {
            examinee -= std_items;
            examine_item(tmp_items_at_loc[location][examinee].id);
        } else if (examinee < std_items + tmp_items + std_npcs) {
            examinee = examinee - std_items - tmp_items;
            NPC n = NPCs[npcs_at_loc[location][examinee]];
            printf("%s\n%s\n\n", n.name, n.examine);
        } else if (examinee < std_items + tmp_items + std_npcs + std_objs) {
            examinee = examinee - std_items - tmp_items - std_npcs;
            Object o = objects[objs_at_loc[location][examinee]];
            printf("%s\n%s\n\n", o.name, o.examine);
        } else {
            examinee = examinee - std_items - tmp_items - std_npcs - std_objs;
            Object tmp_o = objects[tmp_objs_at_loc[location][examinee].id];
            printf("%s\n%s\n\n", tmp_o.name, tmp_o.examine);
        }
        dialogue();
        clearscreen();
        return 0;

    } else return display_options();
}

// Allow players to search objects at their location
int search_object() {

    int objs_to_search[MAX_OBJS_AT_LOCATION];
    int i = 0, j = 0;
    while (objs_at_loc[location][i] != -1) {
        if (objects[objs_at_loc[location][i]].is_searchable) {
            objs_to_search[j] = objs_at_loc[location][i];
            j++;
        }
        i++;
    }

    bool tried = false;
    char obj_str[4];
    int obj;
    while (true) {
        clearscreen();
        print_blue("What would you like to search?\n\n");
        i = 0;
        while (i < j) {
            printf("%s%i. %s\n", i >= 9 ? "" : " ", i+1, objects[objs_to_search[i]].name);
            i++;
            if (i != 1 && (i-1) % 4 == 0) {
                printf("\n");
            }
        }

        printf("%s%i. Back\n\n", i >= 9 ? "" : " ", i+1);

        if (tried) printf("Invalid selection. Please choose a number above.\n\n");
        else tried = true;

        finput(obj_str, sizeof(obj_str));
        obj = atoi(obj_str) - 1;

        if (obj <= j && obj >= 0) break;
    }


    if (obj < j && obj >= 0) {
        clearscreen();
        (*search_obj[objs_to_search[obj]])();
        return 0;
    } else if (obj == j) {
        clearscreen();
        return display_options();
    } else {
        clearscreen();
        printf("Invalid object to search. Please select a number listed below.\n");
        return search_object();
    }
}

// Initialize search_obj array of functions
void init_search() {
    search_obj[380] = search_380;
}

// Result of searching each possible item. Searches arranged in array of functions:
// void (*search_obj[15000])()
void search_380(void) {
    // Each result of searching bookshelf is 1/3 chance per search
    clearscreen();
    printf("You search the books...\n\n");
    delay(2 * tick);
    clearscreen();
    int i = (rand() % 3);
    if (i == 0)
    {
        printf("None of them look very interesting.\n\n");
    }
    else if (i == 1)
    {
        printf("You find nothing to interest you.\n\n");
    }
    else
    {
        printf("You don't find anything that you'd ever want to read.\n\n");
    }
    dialogue();
    clearscreen();
    return;
}

// Allow players to open objects at their location
int open_object() {

    int objs_to_open[MAX_OBJS_AT_LOCATION];
    int i = 0, j = 0;
    while (objs_at_loc[location][i]) {
        if (objects[objs_at_loc[location][i]].is_openable) {
            objs_to_open[j] = objs_at_loc[location][i];
            j++;
        }
        i++;
    }

    bool tried = false;
    char obj_str[4];
    int obj;

    while (true) {
        clearscreen();
        print_blue("What would you like to open?\n\n");
        i = 0;
        while (i + 1 < j) {
            printf("%s%i. %s\n", i >= 9 ? "" : " ", i+1, objects[objs_to_open[i]].name);
            i++;
            if (i != 1 && (i-1) % 4 == 0) {
                printf("\n");
            }
        }

        printf("%s%i. Back\n\n", i >= 9 ? "" : " ", i+1);

        if (tried) printf("Invalid selection. Please choose a number above.\n\n");
        else tried = true;

        finput(obj_str, sizeof(obj_str));
        obj = atoi(obj_str) - 1;
        if (obj <= i && obj >= 0) break;
    }


    if (obj < i) {
        clearscreen();
        (*open_obj[objs_to_open[obj]])();
        return 0;
    } else {
        clearscreen();
        return display_options();
    }
}

// Initialize open_obj array of functions
void init_open() {
    open_obj[3] = open_3;
    open_obj[37] = open_37;
    open_obj[103] = open_103;
    open_obj[348] = open_348;
}

// Result of opening each possible item. Opens arranged in array of functions:
// void (*open_obj[15000])()
void open_3() {
    // Result of opening a door. Will vary depending on location and character data
    switch (location) {
        case 1:
            // If player has finished Gielinor Guide dialogue, allow them through to the next section

            if (get_location_state(1) == 2)
            {
                printf("You open the door and walk outside.\n\n");
                dialogue();
                update_and_print_location(2);
                if (get_location_state(2) == 0) {
                    print_blue("Moving around\n\n");
                    printf("Depending on your location, you can get around\n");
                    printf("by opening doors, walking to nearby towns, and more.\n");
                    printf("Talk to the survival expert to continue the tutorial.\n\n");
                    dialogue();
                    clearscreen();
                }
            }
            // Else, inform user of requirement for door
            else
            {
                printf("You need to talk to the Gielinor Guide before you are allowed to\n");
                printf("proceed through this door.\n\n");
                dialogue();
                clearscreen();
            }
            break;

        case 2:
            printf("You open the door and walk inside.\n\n");
            dialogue();
            update_and_print_location(1);
            break;

        case 3:
            printf("You try to open the door, but ");
            print_red("it's locked...\n\n");
            dialogue();
            clearscreen();
            printf("You hear a voice from inside the building:\n\n");
            dialogue();
            clearscreen();
            print_red("Master Chef\n\n");
            printf("Coming soon\n\n");
            dialogue();
            clearscreen();
            break;

    }
}
void open_37() {
    int state;
    switch (location) {
        case 2:
            state = get_location_state(2);
            if (state > 5) {
                printf("You open the gate and walk through.\n\n");
                if (state == 6) update_location_state(2, 7);
                dialogue();
                clearscreen();
                update_and_print_location(3);
            } else {
                printf("You need to talk to the Survival Guide and complete her tasks before\n");
                printf("you are allowed to proceed through this gate.\n\n");
                dialogue();
                clearscreen();
            }
            break;

        case 3:
            printf("You open the gate and walk through.\n\n");
            dialogue();
            clearscreen();
            update_and_print_location(2);
    }
}
void open_103() {
    // Result of opening a chest
    printf("You open the chest and search it...\n\n");
    delay(2 * tick);
    clearscreen();
    printf("You open the chest and search it...\n");
    printf("...but find nothing.\n\n");
    dialogue();
    clearscreen();
}
void open_348() {
    // Result of opening drawers
    printf("You open the drawers...\n\n");
    delay(2 * tick);
    clearscreen();
    printf("You open the drawers...\n");
    printf("...but they're empty.\n\n");
    dialogue();
    clearscreen();
}
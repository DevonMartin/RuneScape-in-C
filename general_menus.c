#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/game_functions.h"
#include "headers/general_menus.h"
#include "headers/global.h"
#include "headers/items.h"
#include "headers/locations.h"
#include "headers/npcs.h"
#include "headers/main.h"
#include "headers/skills.h"

void eval_options() {

    // Clear previous avail_actions
    int avail_actions_i = 0, i;
    for (i = 0; i < MAX_AVAIL_ACTIONS; i++) {
        avail_actions[i] = "";
    }

    // Check Talk by checking NPC's at current location for .talk == true
    if (npcs_at_loc[location][0] != -1) {
        i = 0;
        while (npcs_at_loc[location][i] != -1) {
            if (NPCs[npcs_at_loc[location][i]].talk == true) {
                avail_actions[avail_actions_i] = "Talk-to";
                avail_actions_i++;
                break;
            }
            i++;
        }
    }

    // Check Open, Search and Skill by cycling through items at location for .open == true, .search == true and .skill = true
    if (objs_at_loc[location][0] != -1) {
        i = 0;
        while (objs_at_loc[location][i] != -1) {
            if (objects[objs_at_loc[location][i]].is_skillable) {
                avail_actions[avail_actions_i] = "Train a Skill";
                avail_actions_i++;
                break;
            }
            i++;
        }
        i = 0;
        while (objs_at_loc[location][i] != -1) {
            if (objects[objs_at_loc[location][i]].is_openable) {
                avail_actions[avail_actions_i] = "Open";
                avail_actions_i++;
                break;
            }
            i++;
        }
        i = 0;
        while (objs_at_loc[location][i] != -1) {
            if (objects[objs_at_loc[location][i]].is_searchable) {
                avail_actions[avail_actions_i] = "Search";
                avail_actions_i++;
                break;
            }
            i++;
        }
    }

    // Check Pickup by seeing if any tmp_items_at_location exist for current location
    remove_expired_tmp_items();
    remove_expired_tmp_objs();
    if (tmp_items_at_loc[location][0].id != -1) {
        avail_actions[avail_actions_i] = "Pick Up";
        avail_actions_i++;
    }

    // Check Examine. If any object, item or NPC is in location, Examine is an option.
    if (npcs_at_loc[location][0] != -1 || objs_at_loc[location][0] != -1 || items_at_loc[location][0].id != -1 || tmp_items_at_loc[location][0].id != -1) {
        avail_actions[avail_actions_i] = "Examine";
        avail_actions_i++;
    }

    // Finish by adding More to the list, which will present options available anywhere
    // such as Location, Skills, Inventory, etc.
    avail_actions[avail_actions_i] = "More";
}

int settings() {
    int res = settings_menu();
    if (res == -1) {
        return 1;
    } else if (res == 0) {
        return more_actions();
    }
    update_num_in_table("settings", tmp_str, tmp_num_retrieval);
    clearscreen();
    return settings();
}

int log_out() {
    return 1;
}

int perform_action(int action) {

    // Ensure chosen action is valid
    if (0 > action || action >= MAX_AVAIL_ACTIONS || !strcmp(avail_actions[action], "")) return 2;

    char *a = avail_actions[action];

    if (!strcmp(a, "Talk-to")) {
        return talk_to_npc();
    } else if (!strcmp(a, "Open")) {
        return open_object();
    } else if (!strcmp(a, "Train a Skill")) {
        return use_skill_spot();
    } else if (!strcmp(a, "Search")) {
        return search_object();
    } else if (!strcmp(a, "Pick Up")) {
        return pickup_item();
    } else if (!strcmp(a, "Examine")) {
        return examine_object();
    } else {
        return more_actions();
    }
}

int display_options() {
    // See what actions can be performed at players location
    eval_options();

    // Display options and get input from user
    bool tried = false;
    while (true) {
        clearscreen();
        if (settings_selections[0] == 1 && last_xp_drop.xp != 0) {
            print_blue(skills[last_xp_drop.skill].name);
            print_blue(": +");
            char xp_str[7];
            snprintf(xp_str, 7, "%dxp\n\n", last_xp_drop.xp);
            print_blue(xp_str);
        }

        print_blue("What would you like to do?\n\n");
        int i = 0;
        while (strcmp(avail_actions[i], "")) {
            printf("%s%i. %s\n", i >= 9 ? "" : " ", i+1, avail_actions[i]);
            i++;
            if (i % 4 == 0 && strcmp(avail_actions[i], "")) {
                printf("\n");
            }
        }
        printf("\n");

        if (tried) printf("Invalid selection. Please choose a number above.\n\n");
        else tried = true;

        char action[3];
        finput(action, sizeof(action));

        int response = perform_action(atoi(action) - 1);

        if (response == 0 || response == 1) return response;
        if (response == 2) continue;
    }
}

int more_actions() {

    char *options[10];
    int (*option_functions[10])();
    int num_of_options;

    // Available actions depend on progression through Tutorial Island.
    // Top if statement is for default view.
    if (location > 2 || get_location_state(2) > 1) {
        
        num_of_options = 6;

        options[0] = "Inventory";
        options[1] = "Skills";
        options[2] = "Location";
        options[3] = "Settings";
        options[4] = "Log Out";
        options[5] = "Back";

        option_functions[0] = inventory_menu;
        option_functions[1] = skills_menu;
        option_functions[2] = print_location;
        option_functions[3] = settings;
        option_functions[4] = log_out;
        option_functions[5] = display_options;
        
    } else if (get_location_state(2) > 0) {

        num_of_options = 5;

        options[0] = "Inventory";
        options[1] = "Location";
        options[2] = "Settings";
        options[3] = "Log Out";
        options[4] = "Back";

        option_functions[0] = inventory_menu;
        option_functions[1] = print_location;
        option_functions[2] = settings;
        option_functions[3] = log_out;
        option_functions[4] = display_options;

    } else if (get_location_state(1) > 0) {

        num_of_options = 4;

        options[0] = "Location";
        options[1] = "Settings";
        options[2] = "Log Out";
        options[3] = "Back";

        option_functions[0] = print_location;
        option_functions[1] = settings;
        option_functions[2] = log_out;
        option_functions[3] = display_options;

    } else {

        num_of_options = 3;

        options[0] = "Location";
        options[1] = "Log Out";
        options[2] = "Back";

        option_functions[0] = print_location;
        option_functions[1] = log_out;
        option_functions[2] = display_options;
    }

    bool tried = false;
    char action_str[4];
    int action;
    while (true) {
        clearscreen();
        print_blue("More options: \n\n");
        int i = 0;
        while (i < num_of_options) {
            printf("%s%i. %s\n", i >= 9 ? "" : " ", i+1, options[i]);
            i++;
            if (i % 4 == 0) {
                printf("\n");
            }
        }
        printf("\n");

        if (tried) printf("Invalid selection. Please choose a number above.\n\n");
        else tried = true;

        finput(action_str, sizeof(action_str));
        action = atoi(action_str) - 1;

        if (action >= 0 && action < num_of_options) break;
    }

    return (*option_functions[action])();
}

int print_skills() {

    int i, j, k, dif, total_level = 0;
    int max_str_len = 0;

    for (i = 0; i < 28; i++) {
        int str_len = strlen(skills[i].name);
        if (str_len > max_str_len) max_str_len = str_len;
        total_level += skills[i].level;
    }

    int full_width = max_str_len + (25 * 3) + 1;
    
    clearscreen();

    for (i = 0; i < skill_count; i+=3) {
        for (j = 0; j < full_width; j++) {
            printf("-");
        }
        printf("\n");
        for (j = i; j < i+3 && j < skill_count; j++) {
            printf("| %s%i. %s: ", (j>=9) ? "" : " ", j+1, skills[j].name);
            dif = max_str_len - strlen(skills[j].name);
            for (k = 0; k < dif; k++) printf(" ");
            printf("Level %i%s", skills[j].level, (skills[j].level > 9 ? " " : "  "));
        }
        if (j == skill_count) {
            dif = 7;
            if (total_level > 999) dif += 2;
            else if (total_level > 99) dif += 1;
            printf("|      Total Level: %i", total_level); // 16 + len(total_level)
            for (i = 0; i < dif; i++) printf(" ");
            printf("|\n");
            break;
        }
        printf("|\n");
    }

    for (j = 0; j < full_width; j++) printf("-");

    return 0;
}

int skills_menu() {

    int ans = -1;
    bool tried = false;
    char ans_str[4];

    while (true) {

        if (ans == 0) return more_actions();
        if (ans > 0 && ans <= 28) {
            skill s = skills[ans - 1];
            clearscreen();
            printf("%s XP: %i\n", s.name, s.real_xp);
            printf("Next level at: %i\n", s.next_level_xp);
            printf("Remaining XP: %i\n\n", s.next_level_xp - s.real_xp);
            dialogue();
            clearscreen();
        }

        print_skills();

        printf("\n\nChoose a skill to get more info on it, or press Return/Enter to go back.\n\n");

        if (tried) {
            if (ans < 0 || ans > skill_count) printf("Invalid selection. Please choose a number above.\n\n");
        } else tried = true;

        finput(ans_str, sizeof(ans_str));
        ans = atoi(ans_str);
    }
    
}

int print_location() {
    clearscreen();
    printf("Your location:\n%s\n\n", locations[location].name);
    dialogue();
    clearscreen();
    return 0;
}

int inventory_menu() {

    while (inventory_menu_options() == 0) {
        continue;
    }
    return more_actions();
}

void print_inventory(int green_index) {

    int longest_item = 5, i, j;
    for (i = 0; i < 28; i++) {
        int id = inv_slots[i].id;
        if (id != -1) {
            int item_len = strlen(items[id].name);
            longest_item = (item_len > longest_item ? item_len : longest_item);
        }
    }

    int row_len = 29 + (longest_item * 4);

    i = 0;
    while (i <= 28) {
        for (j = 0; j < row_len; j++) {
            printf("-");
        }
        printf("\n");
        if (i < 28) {
            for (j = i; j < i + 4; j++) {
                printf("| ");
                if (j < 9) {
                    printf(" %i. ", j + 1);
                } else {
                    printf("%i. ", j + 1);
                }
                int id = inv_slots[j].id;
                char *name = (id == -1 ? "" : items[id].name);
                if (j == green_index) print_green(name);
                else print_yellow(name);
                int dif = longest_item - strlen(name);
                for (int k = 0; k < dif; k++) {
                    printf(" ");
                }
                printf(" ");
            }
        
        printf("|");
        }
        printf("\n");
        i += 4;
    }
}

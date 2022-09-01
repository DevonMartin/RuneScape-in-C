#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/game_functions.h"
#include "headers/general_menus.h"
#include "headers/global.h"
#include "headers/items.h"
#include "headers/locations.h"
#include "headers/main.h"
#include "headers/skills.h"

int choose_skill_action(skill skl) {

    int i, j;

    Object skill_spots[MAX_OBJS_AT_LOCATION];
    for (i = 0, j = 0; i < MAX_OBJS_AT_LOCATION; i++) {
        Object o = objects[objs_at_loc[location][i]];
        if (o.is_skillable == true && o.skill == skl.id) {
            skill_spots[j] = objects[objs_at_loc[location][i]];
            j++;
        }
    }

    skill_spots[j].name = "Stop";

    char s[50];
    snprintf(s, 50, "How would you like to train %s?\n\n", skills[skill_spots[0].skill].name);

    bool tried = false;
    char ans_str[4];
    int ans;
    while (true) {
        clearscreen();
        print_blue(s);
        i = 0;
        while (strcmp(skill_spots[i].name, "Stop")) {
            printf("%s%i. %s\n", i >= 9 ? "" : " ", i+1, skill_spots[i].name);
            i++;
            if (i % 4 == 0) {
                printf("\n");
            }
        }

        printf("%s%i. Back\n\n", i >= 9 ? "" : " ", i+1);

        if (tried) printf("Invalid selection. Please choose a number above.\n\n");
        else tried = true;

        finput(ans_str, sizeof(ans_str));
        ans = atoi(ans_str) - 1;
        if (ans >= 0 && ans <= i) break;
    }

    clearscreen();

    if (ans >= 0 && ans < i) return skill_spots[ans].id;
    else return -1;

}

// Select a skill to train, then a skill spot to use. Pass that skill spot
// to appropriate skilling function.
int use_skill_spot() {

    bool all_skills[skill_count];
    for (int i = 0; i < skill_count; i++) {
        all_skills[i] = false;
    }

    int i = 0;
    while (objs_at_loc[location][i] != -1) {

        Object o = objects[objs_at_loc[location][i]];
        if (o.is_skillable == true) {
            all_skills[o.skill] = true;
        }
        i++;
    }

    skill avail_skills[skill_count];
    i = 0;
    for (int j = 0; j < skill_count; j++) {
        if (all_skills[j] == true) {
            avail_skills[i] = skills[j];
            i++;
        }
    }

    skill tmp = {
        .name = "stop"
    };

    avail_skills[i] = tmp;

    bool tried = false;
    char obj_str[4];
    int obj;
    while (true) {
        clearscreen();
        print_blue("What skill would you like to train?\n\n");
        i = 0;
        while (strcmp(avail_skills[i].name, "stop")) {
            printf("%s%i. %s\n", i >= 9 ? "" : " ", i+1, avail_skills[i].name);
            i++;
            if (i % 4 == 0) {
                printf("\n");
            }
        }

        printf("%s%i. Back\n\n", i >= 9 ? "" : " ", i+1);

        if (tried) printf("Invalid selection. Please choose a number above.\n\n");
        else tried = true;

        finput(obj_str, sizeof(obj_str));
        obj = atoi(obj_str) - 1;
        if (obj >= 0 && obj <= i) break;
    }

    int skill_location_id;

    if (obj == i) return display_options();
    else skill_location_id = choose_skill_action(avail_skills[obj]);

    if (skill_location_id == -1) return use_skill_spot();

    return (*train_skill[avail_skills[obj].id])(skill_location_id, -1);
}

void level_up(int skill, int dif) {

    char *name = skills[skill].name, *a_or_an;
    if (name[0] == 'A' || name[0] == 'E' || name[0] == 'I' || name[0] == 'O' || name[0] == 'U') {
        a_or_an = "an";
    } else {
        a_or_an = "a";
    }

    char s[75];
    if (dif == 1) snprintf(s, 75, "Congratulations, you just advanced %s %s level.\n\n", a_or_an, name);
    else snprintf(s, 75, "Congratulations, you just advanced %i %s levels.\n\n", dif, name);
    clearscreen();
    print_blue(s);
    printf("Your %s level is now %i.\n\n", name, skills[skill].level);
    dialogue();
    clearscreen();
}

int gain_xp(int skill, int precise_xp) {
    int old_level = skills[skill].level;
    update_num_in_table("stats", skills[skill].name, skills[skill].precise_xp+precise_xp);
    load_skills();
    last_xp_drop.skill = skill;
    last_xp_drop.xp = precise_xp / 10;
    if (skills[skill].level != old_level) {
        level_up(skill, skills[skill].level - old_level);
        return 1;
    }
    return 0;
}

int train_cooking(int skill_material_inv_slot, int cooking_obj_id) {
    if (cooking_obj_id == 3769) {
        remove_expired_tmp_objs();
        if (!tmp_obj_exists(3769)) {
            print_red("The fire at your location appears to have burnt out!\n\n");
            dialogue();
            clearscreen();
            return 0;
        }
        if (inv_slots[skill_material_inv_slot].id == 317) {
            int state = get_location_state(2);
            if (location > 15 || state > 5) {
                Skill_Element e = { .req = 1, .low = 128, .high = 512 };
                double chance = skill_success_odds(e, skills[COOKING].level);

                if (location > 15) {

                } else {
                    print_blue("Please wait\n\n");
                    printf("Your character is now attempting to cook some shrimp. This will\n");
                    printf("only take a few seconds.\n\n");
                    delay(4 * tick);
                    clearscreen();
                    double target = (double)(rand() % 257) / 256;
                    remove_item_from_inv(skill_material_inv_slot);
                    if (chance > target) {
                        printf("You manage to cook some shrimp.\n\n");
                        get_item(315, 1);
                        dialogue();
                        clearscreen();
                        if (skills[COOKING].level < 3) gain_xp(COOKING, 300);
                        if (state == 6) {
                            print_blue("Moving on\n\n");
                            printf("Well done, you've just cooked another meal! Speak to the survival\n");
                            printf("expert if you want a recap, otherwise you can move on. \"Open\" the\n");
                            printf("gate and follow the path. Remember, you can check your\n");
                            printf("current location in \"More\" options.\n\n");
                            dialogue();
                            clearscreen();
                        }
                    } else {
                        printf("You accidently burn the shrimp.\n\n");
                        get_item(7954, 1);
                        dialogue();
                        clearscreen();
                        if (state == 6) {
                            print_blue("Moving on\n\n");
                            printf("Oh no, you've just burnt your meal! Speak to the survival expert\n");
                            printf("if you want a recap, otherwise you can move on. \"Open\" the\n");
                            printf("gate and follow the path. Remember, you can check your\n");
                            printf("current location in \"More\" options.\n\n");
                            dialogue();
                            clearscreen();
                        }
                    }
                }
            } else {
                print_blue("Please wait\n\n");
                printf("Your character is now attempting to cook some shrimp. This will\n");
                printf("only take a few seconds.\n\n");
                delay(4 * tick);
                clearscreen();
                remove_item_from_inv(skill_material_inv_slot);
                get_item(315, 1);
                gain_xp(COOKING, 300);
                update_location_state(2, 6);
                clearscreen();
                printf("You manage to cook some shrimp.\n\n");
                dialogue();
                clearscreen();
                print_blue("Moving on\n\n");
                printf("Well done, you've just cooked your first meal! Speak to the\n");
                printf("survival expert if you want a recap, otherwise you can move on.\n");
                printf("\"Open\" the gate and follow the path. Remember, you can\n");
                printf("check your current location in \"More\" options.\n\n");
                dialogue();
                clearscreen();
            } 
        }
    }
    return 0;
}

int train_firemaking(int skill_material_inv_slot, int not_used) {

    if (!locations[location].outside) {
        printf("You can't light a fire here.");
        dialogue();
        return 0;
    }

    int burn_time = (floor((rand() % 100 + 100) * tick)) + current_time();

    switch (inv_slots[skill_material_inv_slot].id) {

        case 1511:
            if (location < 15) {
                print_blue("Please wait\n\n");
                printf("Your character is now attempting to light a fire. This should only\n");
                printf("take a few seconds.\n\n");
                delay(7 * tick);
                clearscreen();
                Tmp_Obj_At_Loc obj = { .id = 3769, .expiration = burn_time};
                int state = get_location_state(2);
                if (state > 5) obj.expiration = current_time() + floor(12 * tick);
                add_tmp_obj(obj);
                remove_item_from_inv(skill_material_inv_slot);
                if (skills[FIREMAKING].level < 3) gain_xp(FIREMAKING, 400);
                if (state == 4) update_location_state(2, 5);
                if (get_location_state(2) == 5) {
                    print_blue("Cooking\n\n");
                    printf("Now it's time to get cooking. To do so, \"Use\" your shimp\n");
                    printf("in your inventory on the fire you just lit, which is now an object\n");
                    printf("at your location.\n\n");
                    dialogue();
                    clearscreen();
                }
            }
    }
    return 0;
}

int train_fishing(int skill_location_id, int not_used) {
    switch (skill_location_id) {

        // Tut Isle Fishing spot
        case 42:
            if (get_location_state(2) < 1) {
                clearscreen();
                printf("You cannot fish here yet. You must progress further in the tutorial.\n\n");
                dialogue();
                clearscreen();
                break;
            } else if (item_in_inv(303)) {
                if (inv_has_room()) {
                    clearscreen();
                    print_blue("Please wait\n\n");
                    printf("Your character is now attempting to catch some shrimp. Sit back\n");
                    printf("for a moment while he does all the hard work.\n\n");
                    delay(6 * tick);
                    clearscreen();
                    printf("You manage to catch some shrimp.\n\n");
                    get_item(317, 1);
                    dialogue();
                    if (skills[FISHING].level < 3) gain_xp(FISHING, 100);
                    if (get_location_state(2) == 1) {
                        char answer[3];

                        do {
                            clearscreen();
                            print_blue("You've gained some experience\n\n");
                            printf("Type the number next to \"More\" and then \"Skills\" to\n");
                            printf("see your skills menu.\n\n");
                            print_blue("What would you like to do?\n\n");
                            printf("1. Talk-to\n");
                            printf("2. Train a Skill\n");
                            printf("3. Open\n");
                            printf("4. Examine\n\n");
                            print_green("5. More\n\n");
                            finput(answer, sizeof(answer));
                        } while (strcmp(answer, "5"));
                        do {
                            clearscreen();
                            print_blue("You've gained some experience\n\n");
                            printf("Type the number next to \"More\" and then \"Skills\" to\n");
                            printf("see your skills menu.\n\n");
                            print_blue("More options\n\n");
                            printf("1. Inventory\n");
                            print_green("2. Skills\n");
                            printf("3. Location\n");
                            printf("4. Log Out\n\n");
                            printf("5. Back\n\n");
                            finput(answer, sizeof(answer));
                        } while (strcmp(answer, "2"));
                        clearscreen();
                        print_skills();
                        print_blue("\n\nSkills and Experience\n\n");
                        printf("On this menu you can view your skills. Your skills can be leveled\n");
                        printf("up by earning experience, which is gained by performing various\n");
                        printf("activities. As you level up your skills, you will earn new unlocks.\n");
                        printf("Speak to the survival expert to continue.\n\n");
                        dialogue();
                        update_location_state(2, 2);
                    }
                    clearscreen();
                    break;
                } else {
                    printf("Your inventory is too full to hold any more shrimp. To drop an item\n");
                    printf("open your inventory, select an item to drop, and then \"Drop\" it.\n\n");
                    dialogue();
                    clearscreen();
                }
            } else {
                printf("You need to use a net to catch these fish. Talk to the survival expert\n");
                printf("to get one.\n\n");
                dialogue();
                clearscreen();
            }
    }
    return 0;
}

int train_mining(int skill_location_id, int not_used) {
    bool tried = false;
    int ans;
    char ans_str[4];
    while (true) {
        clearscreen();
        print_blue("What would you like to do to the Rocks?\n\n");
        printf(" 1. Mine\n");
        printf(" 2. Prospect\n");
        printf(" 3. Back\n\n");
        if (tried) printf("Invalid selection. Please choose a number above.\n\n");
        else tried = true;
        finput(ans_str, sizeof(ans_str));
        ans = atoi(ans_str);
        if (ans > 0 && ans < 4) break;
    }
    clearscreen();
    if (ans == 3) return use_skill_spot();
    printf("There is currently no ore available in this rock.\n\n");
    dialogue();
    clearscreen();
    return 0;
}

int train_woodcutting(int skill_location_id, int not_used) {
    int state;
    switch (skill_location_id) {

        // Tut Isle Tree
        case 1276:
            state = get_location_state(2);
            if (state < 3) {
                clearscreen();
                printf("You cannot cut down this tree yet. You must progress further in\nthe tutorial.\n\n");
                dialogue();
                clearscreen();
                break;
            } else if (state < 7) {
                if (inv_has_room()) {
                    int logs_in_inv = 0;
                    for (int i = 0; i < 28; i++) {
                        if (inv_slots[i].id == 1511) logs_in_inv++;
                    }

                    if (logs_in_inv >= 3) {
                        printf("Perhaps you've got enough logs for now.\n\n");
                        dialogue();
                        clearscreen();
                        print_blue("Firemaking\n\n");
                        if (item_in_inv(590)) {
                            printf("Now that you have some logs, it's time to light a fire. First, select\n");
                            printf("the tinderbox in your inventory and \"Use\" it on the logs.\n\n");
                        } else {
                            printf("Now that you have some logs, it's time to light a fire. To do so,\n");
                            printf("you'll need a tinderbox. Talk to the survival expert to get one.\n\n");
                        }
                        dialogue();
                        clearscreen();
                        break;
                    }

                    if (item_in_inv(1351)) {
                        clearscreen();
                        print_blue("Please wait\n\n");
                        printf("Your character is now attempting to cut down the tree. Sit back\n");
                        printf("for a moment while he does all the hard work.\n\n");
                        delay(10 * tick);
                        clearscreen();
                        get_item(1511, 1);
                        if (get_location_state(2) == 3) {
                            update_location_state(2, 4);
                            int slot = -1;
                            for (int i = 0; i < 28; i++) {
                                if (inv_slots[i].id == 1511) {
                                    slot = i;
                                    break;
                                }
                            }
                            print_inventory(slot);
                        }                    
                        printf("You manage to cut some logs.\n\n");
                        dialogue();
                        if (skills[WOODCUTTING].level < 3) gain_xp(WOODCUTTING, 250);
                        clearscreen();
                        if (get_location_state(2) == 4) {
                            print_blue("Firemaking\n\n");
                            if (item_in_inv(590)) {
                                printf("Now that you have some logs, it's time to light a fire. First, select\n");
                                printf("the tinderbox in your inventory and \"Use\" it on the logs.\n\n");
                            } else {
                                printf("Now that you have some logs, it's time to light a fire. To do so,\n");
                                printf("you'll need a tinderbox. Talk to the survival expert to get one.\n\n");
                            }
                            dialogue();
                            clearscreen();
                        }
                        break;
                    } else {
                        printf("You need an axe to chop this tree. Talk to the survival expert to get\n");
                        printf("one.\n\n");
                        dialogue();
                        clearscreen();
                        break;
                    }
                } else {
                    printf("Your inventory is too full to hold any more logs. To drop an item\n");
                    printf("open your inventory, select an item to drop, and then \"Drop\" it.\n\n");
                    dialogue();
                    clearscreen();
                    break;
                }
            } else {
                printf("Perhaps you've done enough woodcutting now.\n\n");
                dialogue();
                clearscreen();
                break;
            }
        
        // Tut Isle Oak
        case 8462:
            if (get_location_state(2) < 3) {
                clearscreen();
                printf("You cannot cut down this tree yet. You must progress further in\nthe tutorial.\n\n");
                dialogue();
                clearscreen();
                break;
            } else {
                clearscreen();
                printf("You won't be able to chop oak trees until you have a Woodcutting\n");
                printf("level of 15. You'll advance to higher Woodcutting levels by chopping\n");
                printf("down normal trees. At higher Woodcutting levels you will find even\n");
                printf("more trees you can cut.\n\n");
                dialogue();
                clearscreen();
            }
    }
    return 0;
}

void init_skill_funcs() {
    train_skill[COOKING] = train_cooking;
    train_skill[FIREMAKING] = train_firemaking;
    train_skill[FISHING] = train_fishing;
    train_skill[MINING] = train_mining;
    train_skill[WOODCUTTING] = train_woodcutting;
}

// Fire burns out in 4 seconds if you've already successfully cooked and light a new log
// Cap is immediately after hitting level 3 regardless of xp
// Cap of logs is 3
// Items: small fishing net, bronze axe, tinderbox, raw shrimps, shrimps, burnt shrimp, ashes
// "Moving on" prompt will change depending on if you last cooked or burned a shrimp
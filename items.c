#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/game_functions.h"
#include "headers/general_menus.h"
#include "headers/items.h"
#include "headers/locations.h"
#include "headers/main.h"
#include "headers/skills.h"

// BEGIN item built-in functions
int nothing_interesting_item(int inv_slot_1, int inv_slot_2, int obj_id) {
    return 1;
}

int fish_shrimp(int inv_slot_1, int inv_slot_2, int obj_id) {

    if (inv_slots[inv_slot_1].id == 303 && inv_slot_2 == -1) {
        if (obj_id == 42) return train_skill[FISHING](obj_id, -1);
        else return 1;
    } else return 1;
}

int light_fire(int inv_slot_1, int inv_slot_2, int obj_id) {
    if (inv_slots[inv_slot_1].id == 590 && obj_id == -1) {
        if (inv_slots[inv_slot_2].id == 1511) return train_skill[FIREMAKING](inv_slot_2, -1);
        else return 1;
    } else return 1;
}
// END

void init_item(char *name, int id, char *examine, bool is_stackable, bool is_notable, int note_id, bool is_wieldable, bool is_eatable, bool is_drinkable, bool is_interactable, int (*use_function)(int inv_slot_1, int inv_slot_2, int obj_id)) {
    items[id].name = name;
    items[id].id = id;
    items[id].examine = examine;
    items[id].is_stackable = is_stackable;
    items[id].is_notable = is_notable;
    items[id].note_id = note_id;
    items[id].is_wieldable = is_wieldable;
    items[id].is_eatable = is_eatable;
    items[id].is_drinkable = is_drinkable;
    items[id].is_interactable = is_interactable;
    items[id].use_fuction = use_function;
}

void init_items() {
    init_item("Small fishing net", 303, "Useful for catching small fish.", false, true, 304, false, false, false, false, fish_shrimp);
    init_item("Tinderbox", 590, "Useful for lighting a fire.", false, true, 591, false, false, false, false, light_fire);
    init_item("Ashes", 592, "A heap of ashes", false, true, 593, false, false, false, false, nothing_interesting_item);
    init_item("Shrimps", 315, "Some nicely cooked shrimp.", false, true, 316, false, true, false, true, nothing_interesting_item);
    init_item("Raw shrimps", 317, "I should try cooking this.", false, true, 1352, false, false, false, false, nothing_interesting_item);
    init_item("Bronze axe", 1351, "A woodcutter's axe.", false, true, 1352, true, false, false, false, nothing_interesting_item);
    init_item("Logs", 1511, "A number of wooden logs.", false, true, 1512, false, false, false, false, nothing_interesting_item);
    init_item("Burnt shrimp", 7954, "Oops!", false, true, 7955, false, false, false, false, nothing_interesting_item);
}

bool item_in_inv(int item) {
    for (int i = 0; i < 28; i++) {
        if (inv_slots[i].id == item) {
            return true;
        }
    }
    return false;
}

bool inv_has_room() {
    for (int i = 0; i < 28; i++) {
        if (inv_slots[i].id == -1) {
            return true;
        }
    }
    return false;
}

bool inv_is_empty() {
    for (int i = 0; i < 28; i++) {
        if (inv_slots[i].id != -1) return false;
    }
    return true;
}

int get_item(int item, int amount) {
    if (items[item].is_stackable) {
        for (int i = 0; i < 28; i++) {
            if (inv_slots[i].id == item) {
                return update_item_from_slot(i, item, inv_slots[i].amount + amount);
            }
        }
    }

    if (inv_has_room()) {
        for (int i = 0; i < 28; i++) {
            if (inv_slots[i].id == -1) {
                return update_item_from_slot(i, item, amount);
            }
        }
    }

    return -1;
}

int pickup_item() {
    remove_expired_tmp_items();
    clearscreen();
    if (tmp_items_at_loc[location][0].id == -1) {
        print_red("All items that were on the ground have despawned!\n\n");
        dialogue();
        clearscreen();
        return 0;
    }

    bool tried = false;
    char ans_str[3];
    int ans, i;
    while (true) {
        clearscreen();
        print_blue("What would you like to pick up?\n\n");
        i = 0;
        while (tmp_items_at_loc[location][i].id != -1 && i < MAX_TMP_ITEMS_AT_LOCATION) {
            printf("%i. %s\n", i+1, items[tmp_items_at_loc[location][i].id].name);
            i++;
        }
        printf("\n%i. Back\n\n", i+1);

        if (tried) printf("Invalid selection. Please choose a number above.\n\n");
        else tried = true;
        finput(ans_str, sizeof(ans_str));
        ans = atoi(ans_str) - 1;
        if (ans >= 0 && ans <= i) break;
    }

    if (ans == i) {
        clearscreen();
        return display_options();
    }
    if (inv_has_room()) {
        get_item(tmp_items_at_loc[location][ans].id, tmp_items_at_loc[location][i].amount);
        remove_tmp_item(ans);
        remove_expired_tmp_items();
        if (tmp_items_at_loc[location][0].id != -1) return pickup_item();
    } else {
        printf("You don't have enough inventory space to hold that item.\n\n");
        dialogue();
    }
    clearscreen();
    return 0;
}

void eat_food(int inv_slot) {
    printf("Om nom nom\n\n");
    remove_item_from_inv(inv_slot);
    dialogue();
    clearscreen();
}

void move_item(int inv_slot) {
    char move_to_str[3];
    int move_to, id_1, id_2, amount_1, amount_2;
    print_inventory(inv_slot);
    printf("%s -> ", items[inv_slots[inv_slot].id].name);
    finput(move_to_str, sizeof(move_to_str));
    move_to = atoi(move_to_str) - 1;
    clearscreen();
    while (move_to < 0 || move_to >= 28) {
        clearscreen();
        print_inventory(inv_slot);
        printf("Please choose an inventory slot from 1 to 28.\n\n");
        printf("%s -> ", items[inv_slots[inv_slot].id].name);
        finput(move_to_str, sizeof(move_to_str));
        move_to = atoi(move_to_str) - 1;
    }
    if (inv_slot != move_to) {
        id_1 = inv_slots[inv_slot].id;
        id_2 = inv_slots[move_to].id;
        amount_1 = inv_slots[inv_slot].amount;
        amount_2 = inv_slots[move_to].amount;
        update_item_from_slot(move_to, id_1, amount_1);
        update_item_from_slot(inv_slot, id_2, amount_2);
    }
    clearscreen();
}

void remove_tmp_item(int loc) {
    
    if (tmp_items_at_loc[location][loc+1].id == -1) {
        tmp_items_at_loc[location][loc].id = -1;
        tmp_items_at_loc[location][loc].amount = 0;
        tmp_items_at_loc[location][loc].expiration = -1;
    } else {
        while (tmp_items_at_loc[location][loc].id != -1) {
            tmp_items_at_loc[location][loc] = tmp_items_at_loc[location][loc+1];
            loc++;
        }
    }
}

void remove_expired_tmp_items() {
    for (int i = 0; i < MAX_TMP_ITEMS_AT_LOCATION; i++) {
        Tmp_Item_At_Loc item = tmp_items_at_loc[location][i];
        if (item.id == -1) break;
        if (item.expiration <= current_time()) {
            item.id = -1;
            item.expiration = -1;
            tmp_items_at_loc[location][i] = item;
        }
    }
}

void add_tmp_item(Tmp_Item_At_Loc item) {
    remove_expired_tmp_items();
    int i;
    for (i = 0; i < MAX_TMP_ITEMS_AT_LOCATION - 1; i++) {
        if (tmp_items_at_loc[location][i].id == -1) {
            break;
        }
    }
    tmp_items_at_loc[location][i+1].id = -1;
    tmp_items_at_loc[location][i+1].expiration = -1;
    for (; i > 0; i--) {
        if (item.expiration < tmp_items_at_loc[location][i-1].expiration) {
            break;
        }
        tmp_items_at_loc[location][i] = tmp_items_at_loc[location][i-1];
    }
    tmp_items_at_loc[location][i] = item;
}

void remove_expired_tmp_objs() {
    int i;
    for (i = 0; i < MAX_TMP_OBJS_AT_LOCATION; i++) {
        Tmp_Obj_At_Loc obj = tmp_objs_at_loc[location][i];
        if (obj.id == -1) break;
        if (obj.expiration <= current_time()) {
            if (obj.id == 3769) {
                Tmp_Item_At_Loc item = { .id = 592, .expiration = obj.expiration + 180 };
                add_tmp_item(item);
            }
            obj.id = -1;
            obj.expiration = -1;
            tmp_objs_at_loc[location][i] = obj;
        }
    }
}

void add_tmp_obj(Tmp_Obj_At_Loc obj) {
    remove_expired_tmp_objs();
    int i;
    for (i = 0; i < MAX_TMP_OBJS_AT_LOCATION - 1; i++) {
        if (tmp_objs_at_loc[location][i].id == -1) {
            break;
        }
    }
    tmp_objs_at_loc[location][i+1].id = -1;
    tmp_objs_at_loc[location][i+1].expiration = -1;
    for (; i > 0; i--) {
        if (obj.expiration < tmp_objs_at_loc[location][i-1].expiration) {
            break;
        }
        tmp_objs_at_loc[location][i] = tmp_objs_at_loc[location][i-1];
    }
    tmp_objs_at_loc[location][i] = obj;
}

bool tmp_obj_exists(int obj_id) {
    int i = 0;
    while (tmp_objs_at_loc[location][i].id != -1) {
        if (tmp_objs_at_loc[location][i].id == obj_id) return true;
    }
    return false;
}

void use_item_on_item_in_inv(int inv_slot) {
    print_inventory(inv_slot);
    printf("Use ");
    print_yellow(items[inv_slots[inv_slot].id].name);
    printf(" -> ");
    char ans_str[4];
    finput(ans_str, sizeof(ans_str));
    int ans = atoi(ans_str) - 1;
    
    while (ans < 0 || ans >= 28 || inv_slots[ans].id == -1) {
        clearscreen();
        print_inventory(inv_slot);
        if (inv_slots[ans].id == -1) printf("That slot is empty!\n\n");
        else printf("Invalid selection. Please choose an available option.\n\n");
        printf("Use ");
        print_yellow(items[inv_slots[inv_slot].id].name);
        printf(" -> ");
        finput(ans_str, sizeof(ans_str));
        ans = atoi(ans_str) - 1;
    }
    clearscreen();
    if (ans == inv_slot) return;
    if (items[inv_slots[inv_slot].id].use_fuction(inv_slot, ans, -1) == 1 && items[inv_slots[ans].id].use_fuction(ans, inv_slot, -1) == 1) {
        printf("Nothing interesting happens.\n\n");
        dialogue();
        clearscreen();
    }
}

void remove_item_from_world(Spwnd_Item_At_Loc itm) {
    printf("No items yet spawned in world. Placeholder.\n\n");
    dialogue();
    clearscreen();
}

void use_item_on_something_nearby(int inv_slot) {
    Item item1 = items[inv_slots[inv_slot].id];
    char ans_str[4];
    int ans, std_objs, tmp_objs, std_items, tmp_items, display;
    bool tried = false;

    while (true) {
        remove_expired_tmp_items();
        remove_expired_tmp_objs();
        clearscreen();
        std_objs = 0, tmp_objs = 0, std_items = 0, tmp_items = 0, display = 1;
        if (objs_at_loc[location][0] != -1 || tmp_objs_at_loc[location][0].id != -1) {
            print_blue("Objects:\n");
            while (objs_at_loc[location][std_objs] != -1) {
                printf("%s%i. %s\n", display > 9 ? "" : " ", display, objects[objs_at_loc[location][std_objs]].name);
                std_objs++;
                display++;
            }
            
            while (tmp_objs_at_loc[location][tmp_objs].id != -1) {
                printf("%s%i. %s\n", display > 9 ? "" : " ", display, objects[tmp_objs_at_loc[location][tmp_objs].id].name);
                tmp_objs++;
                display++;
            }
            printf("\n");
        }

        if (items_at_loc[location][0].id != -1 || tmp_items_at_loc[location][0].id != -1) {
            print_blue("Items:\n");
            while (items_at_loc[location][std_items].id != -1) {
                printf("%s%i. %s\n", display > 9 ? "" : " ", display, items[items_at_loc[location][std_items].id].name);
                std_items++;
                display++;
            }
            
            while (tmp_items_at_loc[location][tmp_items].id != -1) {
                printf("%s%i. %s\n", display > 9 ? "" : " ", display, items[tmp_items_at_loc[location][tmp_items].id].name);
                tmp_items++;
                display++;
            }
            printf("\n");
        }

        if (tried) printf("\nInvalid selection. Please choose a number above.\n\n");
        else tried = true;

        printf("Use ");
        print_yellow(items[inv_slots[inv_slot].id].name);
        printf(" -> ");

        finput(ans_str, sizeof(ans_str));
        ans = atoi(ans_str);
        if (ans > 0 && ans < display) break;

    }
    
    clearscreen();
    Object obj;
    if (ans <= std_objs + tmp_objs) {
        if (ans <= std_objs) {
            obj = objects[objs_at_loc[location][ans-1]];
        } else if (ans <= std_objs + tmp_objs) {
            ans = ans - std_objs - 1;
            obj = objects[tmp_objs_at_loc[location][ans].id];
        }
        if (item1.use_fuction(inv_slot, -1, obj.id) == 1 && obj.use_function(inv_slot) == 1) {
            printf("Nothing interesting happens.\n\n");
            dialogue();
            clearscreen();
        }
        return;
    }
    
    ans = ans - std_objs - tmp_objs;
    Item item2;
    if (ans <= std_items) {
        inv_slots[28].id = items_at_loc[location][ans-1].id;
        inv_slots[28].amount = items_at_loc[location][ans-1].amount;
        item2 = items[inv_slots[28].id];
        if (item1.use_fuction(inv_slot, 28, -1) == 1 && item2.use_fuction(28, inv_slot, -1) == 1) {
            printf("Nothing interesting happens.\n\n");
            dialogue();
            clearscreen();
        } else {
            remove_item_from_world(items_at_loc[location][ans-1]);
            return;
        }
    } else {
        ans = ans - std_items - 1;
        inv_slots[28].id = tmp_items_at_loc[location][ans].id;
        inv_slots[28].amount = tmp_items_at_loc[location][ans].amount;
        item2 = items[inv_slots[28].id];
        if (item1.use_fuction(inv_slot, 28, -1) == 1) {
            printf("Nothing interesting happens.\n\n");
            dialogue();
            clearscreen();
        } else {
            remove_tmp_item(ans);
            return;
        }
    }
}

void use_item(int inv_slot) {
    bool nearby_objects = false;
    int items_in_inv = 0;
    remove_expired_tmp_objs();
    if (objs_at_loc[location][0] != -1 || tmp_objs_at_loc[location][0].id != -1 || items_at_loc[location][0].id != -1 ||tmp_items_at_loc[location][0].id != -1) nearby_objects = true;
    for (int i = 0; i < 28; i++) {
        if (inv_slots[i].id != -1) {
            items_in_inv += 1;
            if (items_in_inv == 2) break;
        }
    }
    if (nearby_objects && items_in_inv == 2) {
        bool tried = false;
        char ans_str[3];
        int ans;

        while (true) {
            clearscreen();
            print_blue("What would you like to use your ");
            print_blue(items[inv_slots[inv_slot].id].name);
            print_blue(" on?\n\n");
            printf(" 1. Another item in your inventory\n");
            printf(" 2. Something nearby\n\n");

            if (tried) printf("Invalid selection. Please choose a number above.\n\n");
            else tried = true;
            finput(ans_str, sizeof(ans_str));
            ans = atoi(ans_str);
            if (ans == 1 || ans == 2) break;
        }

        clearscreen();
        if (ans == 1) return use_item_on_item_in_inv(inv_slot);
        if (ans == 2) return use_item_on_something_nearby(inv_slot);
    } else if (nearby_objects) return use_item_on_something_nearby(inv_slot);
    else if (items_in_inv == 2) return use_item_on_item_in_inv(inv_slot);
    else printf("Nothing available to use your %s on!\n\n", items[inv_slots[inv_slot].id].name);
    dialogue();
    clearscreen();
}

void examine_item(int item) {
    printf("%s\n", items[item].name);
    printf("%s\n\n", items[item].examine);
}

void wield_item(int inv_slot) {
    if (location > 8 || get_location_state(8) > 0) {
        print_red("Todo\n");
        dialogue();
    } else {
        printf("You'll be told how to equip items later.\n\n");
        dialogue();
        clearscreen();
    }
}

void remove_item_from_inv(int inv_slot) {
    update_item_from_slot(inv_slot, -1, 0);
}

void drop_item(int inv_slot) {
    Tmp_Item_At_Loc item = { .id = inv_slot[inv_slots].id, .amount = inv_slot[inv_slots].amount, .expiration = current_time() + 300 * tick };
    if (location < 15) item.expiration = current_time() + 50 * tick;
    remove_item_from_inv(inv_slot);
    add_tmp_item(item);
}

void interact_with_item(int inv_slot) {

    Item item = items[inv_slots[inv_slot].id];
    char *options[8];
    int i = 0, ans;
    if (item.is_wieldable) {
        options[i] = "Wield";
        i++;
    } 
    if (item.is_eatable) {
        options[i] = "Eat";
        i++;
    }
    if (item.is_drinkable) {
        options[i] = "Drink";
        i++;
    }
    if (item.is_interactable) {
        ;
    }

    options[i] = "Move";
    options[i+1] = "Use";
    options[i+2] = "Drop";
    options[i+3] = "Examine";
    options[i+4] = "Stop";
    print_inventory(inv_slot);

    i = 0;
    while (strcmp(options[i], "Stop")) {
        printf("%i. %s\n", i+1, options[i]);
        i++;
    }

    printf("\n");
    char ans_str[3];
    finput(ans_str, sizeof(ans_str));
    ans = atoi(ans_str) - 1;

    while (ans < 0 || ans >= i) {
        clearscreen();
        print_inventory(inv_slot);

        i = 0;
        while (strcmp(options[i], "Stop")) {
            printf("%i. %s\n", i+1, options[i]);
            i++;
        }

        printf("\nInvalid interaction. Please choose a number above.\n\n");

        finput(ans_str, sizeof(ans_str));
        ans = atoi(ans_str) - 1;
    }
    clearscreen();
    
    if (!strcmp(options[ans], "Wield")) {
        wield_item(ans);
    } else if (!strcmp(options[ans], "Eat")) {
        eat_food(inv_slot);
    } else if (!strcmp(options[ans], "Move")) {
        move_item(inv_slot);
    } else if (!strcmp(options[ans], "Use")) {
        use_item(inv_slot);
    } else if (!strcmp(options[ans], "Drop")) {
        drop_item(inv_slot);
    } else if (!strcmp(options[ans], "Examine")) {
        examine_item(inv_slots[inv_slot].id);
        dialogue();
        clearscreen();
    }
    return;
}

int inventory_menu_options() {

    clearscreen();
    print_inventory(-1);
    char option_selection[3];
    int ans;

    if (!inv_is_empty()) {
        printf(" 1. Interact\n");
        printf(" 2. Back\n\n");


        while (true) {

            finput(option_selection, sizeof(option_selection));
            ans = atoi(option_selection);

            while (ans != 1) {
                if (ans == 2) {
                    clearscreen();
                    return 1;
                } else if (ans != 1) {
                    clearscreen();
                    print_inventory(-1);
                    printf(" 1. Interact\n");
                    printf(" 2. Back\n\n");
                    printf("Invalid selection. Please choose a number above.\n\n");
                    finput(option_selection, sizeof(option_selection));
                    ans = atoi(option_selection);
                }

            }

            clearscreen();
            print_inventory(-1);
            printf("Choose an item to interact with.\n\n");

            while (true) {
                char interact_selection[3];
                finput(interact_selection, sizeof(interact_selection));
                ans = atoi(interact_selection) - 1;

                clearscreen();
                if (ans >= 0 && ans < 28) {
                    if (inv_slots[ans].id != -1) {
                        interact_with_item(ans);
                        return 0;
                    } else {
                        clearscreen();
                        print_inventory(-1);
                        printf("Choose an item to interact with.\n\n");
                        printf("That slot is empty!\n\n");
                    }
                } else {
                    clearscreen();
                    print_inventory(-1);
                    printf("Choose an item to interact with.\n\n");
                    printf("Please choose an inventory slot from 1 to 28.\n\n");
                }
            }
        }
        return 0;
    } else {
        printf(" 1. Back\n\n");
        finput(option_selection, sizeof(option_selection));
        ans = atoi(option_selection);
        if (ans == 1) return 1;
        else {
            while (true) {
                clearscreen();
                print_inventory(-1);
                printf(" 1. Back\n\n");
                printf("Invalid selection. Please choose a number above.\n\n");
                finput(option_selection, sizeof(option_selection));
                ans = atoi(option_selection);
                if (ans == 1) return 1;
            }
        }
    }
}

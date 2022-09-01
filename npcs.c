#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "headers/items.h"
#include "headers/game_functions.h"
#include "headers/general_menus.h"
#include "headers/npcs.h"
#include "headers/main.h"

void NoTalkFunc() {}

// Run the initial dialogue to explain the controls briefly for new players.
char new_player_intro(char *username, int v_id)
{
    print_blue("Getting started\n\n");
    printf("Before you begin, let's go over some of\n");
    printf("the basic controls of the game.\n\n");
    delay(5 * tick);
    printf("To continue through dialogue, press the\n");
    printf("\"Return\" or \"Enter\" key on your keyboard\n");
    printf("while focused on the terminal window.\n");
    printf("Give it a shot now!\n\n");
    dialogue();
    clearscreen();
    print_blue("Getting started\n\n");
    printf("Way to go, %s!\n", username);
    printf("You're already getting the hang of this.\n\n");
    dialogue();
    clearscreen();
    print_blue("Getting started\n\n");
    printf("In RuneScape, everything you do will be\n");
    printf("per your response to a prompt in the terminal.\n");
    printf("\"");
    print_blue("What would you like to do?");
    printf("\"\n\n");
    dialogue();
    clearscreen();
    print_blue("Getting started\n\n");
    printf("There are many different things you can do, such as:\n");
    dialogue();
    printf("-skill\n");
    delay(1 * tick);
    printf("-quest\n");
    delay(1 * tick);
    printf("-trade\n\n");
    delay(1 * tick);
    printf("and so much more!\n\n");
    dialogue();
    clearscreen();
    print_blue("Getting started\n\n");
    printf("If you think you've got it, it's time to start your adventure!\n\n");
    dialogue();
    clearscreen();
    print_blue("Getting started\n\n");
    printf("Are you ready?\n\n");
    printf(" 1. Yes\n 2. No\n\n");
    printf("(No will restart this dialogue)\n\n");
    char ans[3];
    finput(ans, sizeof(ans));
    
    // Ensure user is ok with intro and ready to begin playing
    while (strcmp(ans, "1") && strcmp(ans, "2")) {
        clearscreen();
        print_blue("Getting started\n\n");
        printf("Are you ready?\n\n");
        printf(" 1. Yes\n 2. No\n\n");
        printf("(No will restart this dialogue)\n\n");
        printf("Type \"1\" or \"2\"\n\n");
        finput(ans, sizeof(ans));
    }

    // If ok, update location and return for the start of the game
    if (!strcmp(ans, "1"))
    {
        update_location(1);
        clearscreen();
        printf("\n\n");
        delay(2 * tick);
        for (int i = 0; i < 3; i++)
        {
            clearscreen();
            for (int j = 0; j <= i; j++)
            {
                printf(".");
            }
            printf("\n\n");
            delay(2 * tick);
        }
        delay(4 * tick);
        printf("Suddenly, you find yourself in the Gielinor Guide's House.\n\n");
        dialogue();
        clearscreen();
        return ' ';
    }

    // Else, rerun intro
    clearscreen();
    return new_player_intro(username, v_id);
}

void GielinorGuideTalk() {
    int state = get_location_state(1);
    clearscreen();

    if (state == 0)
    {
        print_red("Gielinor Guide\n\n");
        printf("Greetings! I see you are a new arrival to the world of\n");
        printf("Gielinor. My job is to welcome all new visitors. So\n");
        printf("welcome!\n\n");
        dialogue();
        clearscreen();
        print_red("Gielinor Guide\n\n");
        printf("You have already learned the first thing needed to\n");
        printf("succeed in this world: talking to other people!\n\n");
        dialogue();
        clearscreen();
        print_red("Gielinor Guide\n\n");
        printf("You will find many inhabitants of this world have useful\n");
        printf("things to say to you. By typing the number next to \"Talk-to\"\n");
        printf("you can talk to them.\n\n");
        dialogue();
        clearscreen();
        print_red("Gielinor Guide\n\n");
        printf("Now then, let's start by looking at your settings menu.\n\n");
        dialogue();

        char answer[3];
        bool tried = false;
        while (true) {
            clearscreen();
            print_blue("Settings menu\n\n");
            printf("Please type the number next to \"More\" now.\n");
            printf("This will display additional menu options.\n\n");
            printf("1. Talk-to\n");
            printf("2. Open\n");
            printf("3. Search\n");
            printf("4. Examine\n");
            print_green("\n5. More\n\n");
            
            if (tried == true) printf("Invalid selection. Please type the number next to \"More\".\n\n");
            else tried = true;

            finput(answer, sizeof(answer));
            int a = atoi(answer);
            if (a == 5) break;
        }

        tried = false;
        while (true) {
            clearscreen();
            print_blue("Settings menu\n\n");
            printf("Please type the number next to \"Settings\" now.\n");
            printf("This will display your settings menu.\n\n");
            printf("1. Location\n");
            print_green("2. Settings\n");
            printf("3. Log Out\n");
            printf("4. Back\n\n");

            if (tried) printf("Invalid selection. Please type the number next to \"Settings\".\n\n");
            else tried = true;

            finput(answer, sizeof(answer));
            int a = atoi(answer);
            if (a == 2) break;
        }
        
        clearscreen();
        print_blue("Settings menu\n\n");
        printf("(Choose the number of an option to toggle it)\n");
        printf("1. XP Counter: off\n");
        printf("2. Attackable Monsters: on\n\n\n");
        delay(2 * tick);
        printf("Above, you can see a variety of game settings.\n");
        printf("Talk to the Gielinor Guide to continue.\n\n");
        dialogue();
        clearscreen();
        update_location_state(1, 1);
        return;
    }

    else if (state == 1)
    {
        print_red("Gielinor Guide\n\n");
        printf("Looks like you're making good progress! The menu\n");
        printf("you've just opened is one of many. You'll learn about\n");
        printf("the rest as you progress through the tutorial.\n\n");
        dialogue();
        clearscreen();
        print_red("Gielinor Guide\n\n");
        printf("Anyway, I'd say it's time for you to go and meet your\n");
        printf("first instructor!\n\n");
        update_location_state(1, 2);
        dialogue();
        clearscreen();
        print_blue("Moving on\n\n");
        printf("It's time to meet your first instructor. To continue, all you need\n");
        printf("to do is \"Open\" the door.\n\n");
        dialogue();
        clearscreen();
        return;
    }

    else
    {
        print_red("Gielinor Guide\n\n");
        printf("Welcome back. You have already learnt the first thing\n");
        printf("needed to succeed in this world: talking to other people!\n\n");
        dialogue();
        clearscreen();
        print_red("Gielinor Guide\n\n");
        printf("You will find many inhabitants of this world have useful\n");
        printf("things to say to you. By typing the number next to \"Talk-to\",\n");
        printf("you can talk to them.\n\n");
        dialogue();
        clearscreen();
        print_red("Gielinor Guide\n\n");
        printf("To continue the tutorial go through that door over\n");
        printf("there and speak to your first instructor!\n\n");
        dialogue();
        clearscreen();
        print_blue("Moving on\n\n");
        printf("It's time to meet your first instructor. To continue, all you need\n");
        printf("to do is \"Open\" the door.\n\n");
        dialogue();
        clearscreen();
        return;
    }
}

void SurvivalExpertAxeTinderboxCheck() {

    if (!item_in_inv(303)) {
        if (inv_has_room()) get_item(303, 1);
    }

    if (!item_in_inv(1351) || !item_in_inv(590)) {
        if (!item_in_inv(1351)) {
            if (inv_has_room()) {
                get_item(1351, 1);
                if (!item_in_inv(590)) {
                    if (inv_has_room()) {
                        get_item(590, 1);
                        printf("The survival expert gives you a ");
                        print_blue("bronze axe");
                        printf(" and a\n");
                        print_blue("tinderbox");
                        printf(".\n\n");
                    } else {
                        printf("The survival expert gives you a ");
                        print_blue("bronze axe");
                        printf(". She would\n");
                        printf("have given you a tinderbox as well but you didn't have\n");
                        printf("room for it. To get one, drop something and talk to the\n");
                        printf("survival expert again.\n\n");
                    }
                } else {
                    printf("The survival expert gives you a ");
                    print_blue("bronze axe");
                    printf(".\n\n");
                }
            } else {
                printf("You need some tools for this section of the tutorial but you don't\n");
                printf("have enough inventory room. You'll need to drop something. To do\n");
                printf("so, open your inventory, select an item to interact with, and \"Drop\"\n");
                printf("it. Once you've done that, speak to the survival expert again.\n\n");
            }
        } else if (inv_has_room()) {
            get_item(590, 1);
            printf("The survival expert gives you a ");
            print_blue("tinderbox");
            printf(".\n\n");
        } else {
            printf("You need some tools for this section of the tutorial but you don't\n");
            printf("have enough inventory room. You'll need to drop something. To do\n");
            printf("so, open your inventory, select an item to interact with, and \"Drop\"\n");
            printf("it. Once you've done that, speak to the survival expert again.\n\n");
        }
        dialogue();
        clearscreen();
    }
}

// First selection menu
void SurvivalExpert_6_1() {
    bool tried = false;
    char ans_str[4];
    int ans;
    while (true) {
        clearscreen();
        print_red("Select an Option\n\n");
        printf(" 1. Tell me about my skills again.\n");
        printf(" 2. Tell me about Woodcutting again.\n");
        printf(" 3. Tell me about Firemaking again.\n");
        printf(" 4. More options...\n\n");
        if (tried) printf("Invalid selection. Please choose a number above.\n\n");
        else tried = true;
        finput(ans_str, sizeof(ans_str));
        ans = atoi(ans_str);
        if (ans > 0 && ans < 5) break;
    }
    clearscreen();
    switch (ans) {
        case 1:
            SurvivalExpert_6_4();
            break;
        case 2:
            SurvivalExpert_6_5();
            break;
        case 3:
            SurvivalExpert_6_6();
            break;
        case 4:
            SurvivalExpert_6_3();
            break;
    }
}
// Nothing thanks
void SurvivalExpert_6_2() {
    print_red(v_username);
    printf("\n\nNothing thanks.\n\n");
    dialogue();
    clearscreen();

}
// Second selection menu
void SurvivalExpert_6_3() {
    bool tried = false;
    char ans_str[4];
    int ans;
    while (true) {
        clearscreen();
        print_red("Select an Option\n\n");
        printf(" 1. Tell me about Fishing again.\n");
        printf(" 2. Tell me about Cooking again.\n");
        printf(" 3. Nothing thanks.\n");
        printf(" 4. Previous options...\n\n");
        if (tried) printf("Invalid selection. Please choose a number above.\n\n");
        else tried = true;
        finput(ans_str, sizeof(ans_str));
        ans = atoi(ans_str);
        if (ans > 0 && ans < 5) break;
    }
    clearscreen();
    switch (ans) {
        case 1:
            SurvivalExpert_6_7();
            break;
        case 2:
            SurvivalExpert_6_8();
            break;
        case 3:
            SurvivalExpert_6_2();
            break;
        case 4:
            SurvivalExpert_6_1();
            break;
    }
}
// Skills
void SurvivalExpert_6_4() {
    print_red(v_username);
    printf("\n\nTell me about my skills again.\n\n");
    dialogue();
    clearscreen();
    print_skills();
    print_red("\n\nSurvival Expert\n\n");
    printf("Every skill is listed in the skills menu. Here you can\n");
    printf("see what your current levels are and how much\n");
    printf("experience you have.\n\n");
    dialogue();
    clearscreen();
    print_skills();
    print_red("\n\nSurvival Expert\n\n");
    printf("If you type the number next to one of the skills the\n");
    printf("screen will refresh and show you the exact amount\n");
    printf("of experience you have and how much is needed to get\n");
    printf("to the next level.\n\n");
    dialogue();
    clearscreen();
    // TODO: Dialogue about skillguide popup available in-game by clicking on a skill
    print_red("Survival Expert\n\n");
    printf("Is there anything else you'd like to hear more about?\n\n");
    dialogue();
    clearscreen();

    bool tried = false;
    char ans_str[4];
    int ans;
    while (true) {
        clearscreen();
        print_red("Select an Option\n\n");
        printf(" 1. Tell me about Woodcutting again.\n");
        printf(" 2. Tell me about Firemaking again.\n");
        printf(" 3. Tell me about Fishing again.\n");
        printf(" 4. Tell me about Cooking again.\n");
        printf(" 5. Nothing thanks.\n\n");
        if (tried) printf("Invalid selection. Please choose a number above.\n\n");
        else tried = true;
        finput(ans_str, sizeof(ans_str));
        ans = atoi(ans_str);
        if (ans > 0 && ans < 6) break;
    }
    clearscreen();
    switch (ans) {
        case 1:
            SurvivalExpert_6_5();
            break;
        case 2:
            SurvivalExpert_6_6();
            break;
        case 3:
            SurvivalExpert_6_7();
            break;
        case 4:
            SurvivalExpert_6_8();
            break;
        case 5:
            SurvivalExpert_6_2();
            break;
    }
}
// Woodcutting
void SurvivalExpert_6_5() {
    print_red(v_username);
    printf("\n\nTell me about Woodcutting again.\n\n");
    dialogue();
    clearscreen();
    print_red("Survival Expert\n\n");
    printf("Woodcutting, eh? Don't worry, newcomer, it's really\n");
    printf("very easy. Simply equip your axe and \"Train a Skill\" on a\n");
    printf("nearby tree to chop away.\n\n");
    dialogue();
    clearscreen();
    print_red("Survival Expert\n\n");
    printf("As you explore the mainland you will discover many\n");
    printf("different kinds of trees that will require different\n");
    printf("Woodcutting levels to chop down.\n\n");
    dialogue();
    clearscreen();
    print_red("Survival Expert\n\n");
    printf("Logs are not only useful for making fires. Many\n");
    printf("archers use the skill known as Fletching to craft their\n");
    printf("own bows and arrows from trees.\n\n");
    dialogue();
    clearscreen();
    print_red("Survival Expert\n\n");
    printf("Is there anything else you'd like to hear more about?\n\n");
    dialogue();
    clearscreen();

    bool tried = false;
    char ans_str[4];
    int ans;
    while (true) {
        clearscreen();
        print_red("Select an Option\n\n");
        printf(" 1. Tell me about my skills again.\n");
        printf(" 2. Tell me about Firemaking again.\n");
        printf(" 3. Tell me about Fishing again.\n");
        printf(" 4. Tell me about Cooking again.\n");
        printf(" 5. Nothing thanks.\n\n");
        if (tried) printf("Invalid selection. Please choose a number above.\n\n");
        else tried = true;
        finput(ans_str, sizeof(ans_str));
        ans = atoi(ans_str);
        if (ans > 0 && ans < 6) break;
    }
    clearscreen();
    switch (ans) {
        case 1:
            SurvivalExpert_6_4();
            break;
        case 2:
            SurvivalExpert_6_6();
            break;
        case 3:
            SurvivalExpert_6_7();
            break;
        case 4:
            SurvivalExpert_6_8();
            break;
        case 5:
            SurvivalExpert_6_2();
            break;
    }
}
// Firemaking
void SurvivalExpert_6_6() {
    print_red(v_username);
    printf("\n\nTell me about Firemaking again.\n\n");
    dialogue();
    clearscreen();
    print_red("Survival Expert\n\n");
    printf("Certainly, newcomer. When you have logs simply use\n");
    printf("your tinderbox on them. If successful, you will start a\n");
    printf("fire.\n\n");
    dialogue();
    clearscreen();
    print_red("Survival Expert\n\n");
    printf("You can also set fire to logs you find lying on the floor\n");
    printf("already, and some other things can also be set alight...\n\n");
    dialogue();
    clearscreen();
    print_red("Survival Expert\n\n");
    printf("A tinderbox is always a useful item to keep around!\n\n");
    dialogue();
    clearscreen();
    print_red("Survival Expert\n\n");
    printf("Is there anything else you'd like to hear more about?\n\n");
    dialogue();
    clearscreen();

    bool tried = false;
    char ans_str[4];
    int ans;
    while (true) {
        clearscreen();
        print_red("Select an Option\n\n");
        printf(" 1. Tell me about my skills again.\n");
        printf(" 2. Tell me about Woodcutting again.\n");
        printf(" 3. Tell me about Fishing again.\n");
        printf(" 4. Tell me about Cooking again.\n");
        printf(" 5. Nothing thanks.\n\n");
        if (tried) printf("Invalid selection. Please choose a number above.\n\n");
        else tried = true;
        finput(ans_str, sizeof(ans_str));
        ans = atoi(ans_str);
        if (ans > 0 && ans < 6) break;
    }
    clearscreen();
    switch (ans) {
        case 1:
            SurvivalExpert_6_4();
            break;
        case 2:
            SurvivalExpert_6_5();
            break;
        case 3:
            SurvivalExpert_6_7();
            break;
        case 4:
            SurvivalExpert_6_8();
            break;
        case 5:
            SurvivalExpert_6_2();
            break;
    }
}
// Fishing
void SurvivalExpert_6_7() {
    print_red(v_username);
    printf("\n\nTell me about Fishing again.\n\n");
    dialogue();
    clearscreen();
    print_red("Survival Expert\n\n");
    printf("Ah, yes. Fishing! Fishing is undoubtedly one of the\n");
    printf("more popular hobbies here in Gielinor!\n\n");
    dialogue();
    clearscreen();
    print_red("Survival Expert\n\n");
    printf("Whenever you see sparkling waters, you can be sure\n");
    printf("there's probably some good fishing to be had there!\n\n");
    dialogue();
    clearscreen();
    print_red("Survival Expert\n\n");
    printf("Not only are fish absolutely delicious when cooked, they\n");
    printf("will also heal lost health.\n\n");
    dialogue();
    clearscreen();
    print_red("Survival Expert\n\n");
    printf("I would recommend everybody has a go at Fishing at\n");
    printf("least once in their lives!\n\n");
    dialogue();
    clearscreen();
    print_red("Survival Expert\n\n");
    printf("Is there anything else you'd like to hear more about?\n\n");
    dialogue();
    clearscreen();

    bool tried = false;
    char ans_str[4];
    int ans;
    while (true) {
        clearscreen();
        print_red("Select an Option\n\n");
        printf(" 1. Tell me about my skills again.\n");
        printf(" 2. Tell me about Woodcutting again.\n");
        printf(" 3. Tell me about Firemaking again.\n");
        printf(" 4. Tell me about Cooking again.\n");
        printf(" 5. Nothing thanks.\n\n");
        if (tried) printf("Invalid selection. Please choose a number above.\n\n");
        else tried = true;
        finput(ans_str, sizeof(ans_str));
        ans = atoi(ans_str);
        if (ans > 0 && ans < 6) break;
    }
    clearscreen();
    switch (ans) {
        case 1:
            SurvivalExpert_6_4();
            break;
        case 2:
            SurvivalExpert_6_5();
            break;
        case 3:
            SurvivalExpert_6_6();
            break;
        case 4:
            SurvivalExpert_6_8();
            break;
        case 5:
            SurvivalExpert_6_2();
            break;
    }
}
// Cooking
void SurvivalExpert_6_8() {
    print_red(v_username);
    printf("\n\nTell me about Cooking again.\n\n");
    dialogue();
    clearscreen();
    print_red("Survival Expert\n\n");
    printf("Yes, the most basic of survival techniques. Most simple\n");
    printf("foods can be used on a fire to cook them. If you're\n");
    printf("feeling a bit fancier, you can also use a range to cook\n");
    printf("the food instead.\n\n");
    dialogue();
    clearscreen();
    print_red("Survival Expert\n\n");
    printf("Eating cooked food will restore lost health. The harder\n");
    printf("something is to cook, the more it will heal you.\n\n");
    dialogue();
    clearscreen();
    print_red("Survival Expert\n\n");
    printf("Is there anything else you'd like to hear more about?\n\n");
    dialogue();
    clearscreen();

    bool tried = false;
    char ans_str[4];
    int ans;
    while (true) {
        clearscreen();
        print_red("Select an Option\n\n");
        printf(" 1. Tell me about my skills again.\n");
        printf(" 2. Tell me about Woodcutting again.\n");
        printf(" 3. Tell me about Firemaking again.\n");
        printf(" 4. Tell me about Fishing again.\n");
        printf(" 5. Nothing thanks.\n\n");
        if (tried) printf("Invalid selection. Please choose a number above.\n\n");
        else tried = true;
        finput(ans_str, sizeof(ans_str));
        ans = atoi(ans_str);
        if (ans > 0 && ans < 6) break;
    }
    clearscreen();
    switch (ans) {
        case 1:
            SurvivalExpert_6_4();
            break;
        case 2:
            SurvivalExpert_6_5();
            break;
        case 3:
            SurvivalExpert_6_6();
            break;
        case 4:
            SurvivalExpert_6_7();
            break;
        case 5:
            SurvivalExpert_6_2();
            break;
    }
}

void SurvivalExpertTalk() {
    int state = get_location_state(2);

    if (state == 0 || state == 1) {
        if (state == 0) {
            clearscreen();
            print_red("Survival Expert\n\n");
            printf("Hello there, newcomer. My name is Brynna. My job is\n");
            printf("to teach you about the skills you can use to survive in\n");
            printf("this world.\n\n");
            dialogue();
            clearscreen();
            print_red("Survival Expert\n\n");
            printf("The first skill we're going to look at is Fishing. There's\n");
            printf("some shrimp in this pond here. Let's try and catch\n");
            printf("some.\n\n");
            dialogue();
            clearscreen();
            printf("The survival expert gives you a ");
            print_blue("small fishing net");
            printf(".\n\n");
            dialogue();

            char answer[3];

            do {
                clearscreen();
                print_blue("You've been given an item\n\n");
                printf("To view the item you've been given, you'll need to open your\n");
                printf("inventory. To do so, type the number next to \"More\".\n\n");
                printf(" 1. Talk-to\n");
                printf(" 2. Train a Skill\n");
                printf(" 3. Open\n");
                printf(" 4. Examine\n");
                print_green("\n 5. More\n\n");
                finput(answer, sizeof(answer));
            } while (strcmp(answer, "5"));

            do {
                clearscreen();
                print_blue("You've been given an item\n\n");
                printf("Please type the number next to \"Inventory\" now.\n");
                printf("This will display your inventory menu.\n\n");
                print_green(" 1. Inventory\n");
                printf(" 2. Location\n");
                printf(" 3. Settings\n");
                printf(" 4. Log Out\n\n");
                printf(" 5. Back\n\n");
                finput(answer, sizeof(answer));
            } while (strcmp(answer, "1"));

            clearscreen();
            get_item(303, 1);
            update_location_state(2, 1);
        } else {
            print_red("Survival Expert\n\n");
            printf("Hello again. First up, we're going to do some fishing.\n");
            printf("There's some shrimp in this pond here. Let's try and\n");
            printf("catch some.\n\n");
            dialogue();
            clearscreen();
            if (!item_in_inv(303)) {
                get_item(303, 1);
                printf("The survival expert gives you a ");
                print_blue("small fishing net");
                printf(".\n");
                dialogue();
                clearscreen();
            }
        }
        int i;
        for (i = 0; i < 28; i++) {
            if (inv_slots[i].id == 303) {
                break;
            }
        }
        print_inventory(i);
        print_blue("Fishing\n\n");
        printf("This is your inventory. You can view all of your items here,\n");
        printf("including the net you've just been given. Let's use it to catch\n");
        printf("some shrimp. To start fishing, just type the number next to \"Train a Skill\"\n");
        printf("and follow the prompts.\n\n");
        dialogue();
        clearscreen();
        return;
    }
    if (state == 2) {
        if (!item_in_inv(303) && inv_has_room()) get_item(303, 1);
        print_red(v_username);
        printf("\n\nI've managed to catch some shrimp.\n\n");
        dialogue();
        clearscreen();
        print_red("Survival Expert\n\n");
        printf("Excellent work. Now that you have some shrimp, you're\n");
        printf("going to want to cook them. To do that, you'll need a\n");
        printf("fire. This brings us on to the Woodcutting and\n");
        printf("Firemaking skills.\n\n");
        dialogue();
        clearscreen();
        update_location_state(2, 3);
        if (inv_has_room() ) {
            get_item(1351, 1);
            if (inv_has_room()) {
                get_item(590, 1);
                printf("The survival expert gives you a ");
                print_blue("bronze axe");
                printf(" and a\n");
                print_blue("tinderbox");
                printf(".\n\n");
            } else {
                printf("The survival expert gives you a ");
                print_blue("bronze axe");
                printf(". She would\n");
                printf("have given you a tinderbox as well but you didn't have\n");
                printf("room for it. To get one, drop something and talk to the\n");
                printf("survival expert again.\n\n");
            }
        } else {
            printf("You need some tools for this section of the tutorial but you don't\n");
            printf("have enough inventory room. You'll need to drop something. To do\n");
            printf("so, open your inventory, select an item to interact with, and \"Drop\"\n");
            printf("it. Once you've done that, speak to the survival expert again.\n\n");
        }
        dialogue();
        clearscreen();
        print_blue("Woodcutting\n\n");
        printf("It's time to cook your shrimp. However, you require a fire to do\n");
        printf("that which means you need some logs. You can cut down trees\n");
        printf("using your Woodcutting skill, all you need is an axe. Give it a go\n");
        printf("by typing the number next to \"Train a Skill\" and following the prompts.\n\n");
        dialogue();
        clearscreen();
        return;
    }
    if (state == 3) {
        print_red("Survival Expert\n\n");
        printf("Next up you need to make a fire. First, you'll need to\n");
        printf("cut down a tree to get some logs.\n\n");
        dialogue();
        clearscreen();
        SurvivalExpertAxeTinderboxCheck();
        print_blue("Woodcutting\n\n");
        printf("It's time to cook your shrimp. However, you require a fire to do\n");
        printf("that which means you need some logs. You can cut down trees\n");
        if (item_in_inv(1351)) {
            printf("using your Woodcutting skill, all you need is an axe. Give it a go\n");
            printf("by typing the number next to \"Train a Skill\" and following the prompts.\n\n");
        } else {
            printf("using your Woodcutting skill, all you need is an axe. Talk to the\n");
            printf("survival expert to get one.\n\n");
        }
        dialogue();
        return;
    }
    if (state == 4 || state == 5) {
        print_red("Survival Expert\n\n");
        printf("Now that you have some shrimp, you're going to want\n");
        printf("to cook them. To do that, you'll need a fire.\n\n");
        dialogue();
        clearscreen();
        SurvivalExpertAxeTinderboxCheck();
        if (state == 4) {
            print_blue("Firemaking\n\n");
            if (item_in_inv(590)) {
                printf("Now that you have some logs, it's time to light a fire. First, select\n");
                printf("the tinderbox in your inventory and \"Use\" it on the logs.\n\n");
            } else {
                printf("Now that you have some logs, it's time to light a fire. To do so,\n");
                printf("you'll need a tinderbox. Talk to the survival expert to get one.\n\n");
            }
        } else if (state == 5) {
            print_blue("Cooking\n\n");
            printf("Now it's time to get cooking. To do so, \"Use\" your shimp\n");
            printf("in your inventory on the fire you just lit, which is now an object\n");
            printf("at your location.\n\n");
        }
        dialogue();
        clearscreen();
    }
    if (state == 6 || state == 7) {
        if (!item_in_inv(303) && inv_has_room()) get_item(303, 1);
        if (!item_in_inv(1351) && inv_has_room()) get_item(1351, 1);
        if (!item_in_inv(590) && inv_has_room()) get_item(590, 1);
        print_red("Survival Expert\n\n");
        printf("Hello again. Is there something you'd like to hear more\n");
        printf("about?\n\n");
        dialogue();
        SurvivalExpert_6_1();
    }
}

void init_npc(char *name, int id, char *examine, bool talk, void (*talk_func)()) {
    NPCs[id].name = name;
    NPCs[id].id = id;
    NPCs[id].examine = examine;
    NPCs[id].talk = talk;
    NPCs[id].talk_func = talk_func;
}

void init_npcs() {
    init_npc("Butterfly", 234, "It's a Blue Morpho Butterfly.", false, NoTalkFunc);
    init_npc("Butterfly", 235, "These look much better in the wild.", false, NoTalkFunc);
    init_npc("Gielinor Guide", 3308, "Your introduction to the world of Gielinor", true, GielinorGuideTalk);
    init_npc("Survival Expert", 8503, "Very much an outdoors type.", true, SurvivalExpertTalk);
}

int talk_to_npc() {

    bool tried = false;
    char npc_str[3];
    int i, npc;
    while (true) {
        clearscreen();
        print_blue("Who would you like to talk to?\n\n");
        i = 0;
        while (npcs_at_loc[location][i] != -1) {
            printf("%s%i. %s\n", i >= 9 ? "" : " ", i+1, NPCs[npcs_at_loc[location][i]].name);
            i++;
            if (i != 1 && (i-1) % 4 == 0) {
                printf("\n");
            }
        }
        printf("%s%i. Back\n\n", i >= 9 ? "" : " ", i + 1);

        if (tried) printf("Invalid selection. Please choose a number above.\n\n");
        else tried = true;

        finput(npc_str, sizeof(npc_str));
        npc = atoi(npc_str) - 1;

        if (npc >=0 && npc <= i) break;
    }

    clearscreen();

    if (npc == i) {
        return display_options();
    }

    NPCs[npcs_at_loc[location][npc]].talk_func(location);
    return 0;
}

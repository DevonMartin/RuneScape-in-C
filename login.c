#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/game_functions.h"
#include "headers/global.h"
#include "headers/login.h"
#include "headers/main.h"
#include "sqlite3.h"

// print "db not secure, use fake password"
void password_disclosure()
{
    printf("-----------------------------------------------------------------\n");
    printf("Note: Password is NOT hashed and is NOT protected in my database.\nPlease DO NOT reuse a password you have used previously and\nDO NOT assume this account or account information is secure.\n\nPassword: ");
}

// Check if user is logging in or creating a new account
void login_intro()
{
    char ans_str[4];
    int ans;
    bool tried = false; // If they type the wrong thing at least once, give them more instructions
    // User chooses to create an account or log in to an existing account
    while (true) {
        clearscreen();
        print_yellow("Welcome to RuneScape\n\n");
        printf("1. New User | 2. Existing User\n\n");
        if (tried == true) printf("Invalid selection. Please choose 1 or 2.\n\n");
        else tried = true;
        // Get response from user and clear stdin
        finput(ans_str, sizeof(ans_str));
        ans = atoi(ans_str);
        if (ans == 1 || ans == 2) break;
    }

    // Update global variable for use elsewhere
    if (ans == 1) login_bool = false;
    else login_bool = true;
    clearscreen();
}

// Check if a username meets criteria to be a username
char *check_username(char *username)
{
    int usr_len = strlen(username);
    // Check username for acceptability
    if (login_bool == false)
    {
        // Check length req
        if (usr_len > 1 && usr_len <= 13 && username[0] != ' ')
        {
            for (int i = 0;  i <= usr_len; i++)
            {
                // Ensure every letter is an acceptable letter
                if (isalnum(username[i]) || (username[i] == ' ' && username[i-1] != ' '))
                {
                    continue;
                }
                // If \n is hit, turn it NULL and break loop, for username is acceptable
                else if (username[i] == 10)
                {
                    username[i] = 0;
                    break;
                }
                // If loop not broken, username unacceptable. Tell user acceptable username rules
                clearscreen();
                printf("Username must be 12 characters or less,\n");
                printf("must be alphanumeric, and must not contain\n");
                printf("multiple spaces in a row. Please try again!\n\n");
                // Prompt user for new username, clear stdin, and recheck username until a username passes
                printf("Username: ");
                fgets(username, username_size, stdin);
                clearstdin(username);
                return check_username(username);
            }
        }
        else
        {
            clearscreen();
            printf("Username must be 12 characters or less,\n");
            printf("and must not start with a space.\n");
            printf("Please try again!\n\n");
            // Prompt user for new username, clear stdin, and recheck username until a username passes
            printf("Username: ");
            fgets(username, username_size, stdin);
            clearstdin(username);
            return check_username(username);
        }
    }
    else
    {
        for (int i = 0;  i <= usr_len; i++)
        {
            // If \n is hit, turn it NULL
            if (username[i] == 10)
            {
                username[i] = 0;
            }

        }
    }
    // Once a username is acceptable, ensure it is not already in use
    return search_username(username); 
}

// Check if password is acceptable.
char *check_password(char *password)
{
    // Get length of the password as one check
    int pswrd_len = strlen(password);
    if (pswrd_len >= 9 && pswrd_len <= 25 && password[pswrd_len - 1] == 10)
    {
        for (int i = 0;  i <= pswrd_len; i++)
        {
            // Ensure every letter is an acceptable letter
            if ((password[i] != '\'' && password[i] != '\"') && (password[i] != ' ' || (password[i] == ' ' && password[i-1] != ' ')))
            {
                continue;
            }
            // If \n is hit, turn it NULL and break loop, for password is acceptable
            else if (password[i] == 10)
            {
                password[i] = 0;
                break;
            }
            // If loop not broken, password unacceptable. Tell user acceptable password rules
            clearscreen();
            printf("Password must be 8 through 24 characters,\n");
            printf("must not contain multiple spaces in a row,\n");
            printf("and must not contain single or double quotes.\n");
            printf("Please try again!\n\n");
            // Prompt user for new password, clear stdin, and recheck password until a password passes
            password_disclosure();
            fgets(password, password_size, stdin);
            clearstdin(password);
            return check_password(password);
        }
    }
    // If length not suitable, inform user
    else
    {
        clearscreen();
        printf("Password must be from 8 to 24 characters long.\n");
        printf("Please try again!\n\n");
        // Prompt user for new password, clear stdin, and recheck password until a password passes
        password_disclosure();
        fgets(password, password_size, stdin);
        clearstdin(password);
        return check_password(password);
    }
    return password;
}

// Insert users chosen credentials into the db to create a new character
char *add_user(char *username, char *password)
{
    // Remove \n char of password
    for (int i = 0; i < strlen(password); i++)
    {
        if (password[i] == 10)
        {
            password[i] = 0;
        }
    }
    // Open database and check that it was successful.
    sqlite3 *db;
    int rc;
    rc = sqlite3_open("runescape.db", &db);
    if (rc != SQLITE_OK)
    {
        printf("login.c error 2\n");
        sqlite3_close(db);
        return "~";
    }
    char *err_msg = 0;

    // Insert users selected usrnm & pswrd to credentials
    char sql1[125];
    snprintf(sql1, 125, "INSERT INTO credentials (username, password) VALUES(\"%s\", \"%s\");", username, password);
    rc = sqlite3_exec(db, sql1, 0, 0, &err_msg);
    // If rc errors, something unexpected happened. Fatal Error
    if (rc != SQLITE_OK)
    {
        printf("login.c error 3\n");
        sqlite3_close(db);
        return "~";
    }
    
    // Retrieve newly created account ID
    login_bool = true;
    search_username(username);

    // Create stats data for new character
    char sql2[55];
    snprintf(sql2, 55, "INSERT INTO stats (player_id) VALUES(%i);", v_id);
    rc = sqlite3_exec(db, sql2, 0, 0, &err_msg);
    // If rc errors, something unexpected happened. Fatal Error
    if (rc != SQLITE_OK)
    {
        printf("login.c error 4\n");
        sqlite3_close(db);
        return "~";
    }

    // Create locations data for new character
    char sql3[70];
    for (int i = -1; i < 10; i++) {
        snprintf(sql3, 70, "INSERT INTO locations (player_id, location) VALUES(%i, %i);", v_id, i);
        rc = sqlite3_exec(db, sql3, 0, 0, &err_msg);
        // If rc errors, something unexpected happened. Fatal Error
        if (rc != SQLITE_OK)
        {
            printf("login.c error 5\n");
            sqlite3_close(db);
            return "~";
        }
    }

    // Create settings data for new character
    char sql4[50];
    snprintf(sql4, 50, "INSERT INTO settings (player_id) VALUES(%i);", v_id);
    rc = sqlite3_exec(db, sql4, 0, 0, &err_msg);
    // If rc errors, something unexpected happened. Fatal Error
    if (rc != SQLITE_OK)
    {
        printf("login.c error 6\n");
        sqlite3_close(db);
        return "~";
    }

    // Create inventory data for new character
    for (int i = 0; i < 28; i++) {
        char sql5[75];
        snprintf(sql5, 75, "INSERT INTO inventory_slot (player_id, slot_num) VALUES(%i, %i);", v_id, i);
        rc = sqlite3_exec(db, sql5, 0, 0, &err_msg);
        // If rc errors, something unexpected happened. Fatal Error
        if (rc != SQLITE_OK)
        {
            printf("login.c error 7\n");
            sqlite3_close(db);
            return "~";
        }
    }

    // If rc ok, users credentials should be added successfully.
    sqlite3_close(db);
    clearscreen();
    return username;
}

void create_db() {
    // Open database and check that it was successful.
    sqlite3 *db;
    int rc;

    rc = sqlite3_open("runescape.db", &db);
    if (rc != SQLITE_OK)
    {
        printf("login.c create_db error 1\n");
        sqlite3_close(db);
        dialogue();
        return;
    }
    char *err_msg = 0;

    char *sql1 = "CREATE TABLE IF NOT EXISTS \"credentials\" (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT NOT NULL, password TEXT NOT NULL);";
    rc = sqlite3_exec(db, sql1, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        printf("Fatal error 1 creating database file.\n\n");
        sqlite3_close(db);
        dialogue();
        return;
    }

    char *sql2 = "CREATE TABLE settings (player_id INTEGER, XP_Counter INTEGER DEFAULT 0, Attackable_Monsters INTEGER DEFAULT 1, FOREIGN KEY(player_id) REFERENCES player_info(id));";
    rc = sqlite3_exec(db, sql2, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        printf("Fatal error 2 creating database file.\n\n");
        sqlite3_close(db);
        dialogue();
        return;
    }

    char *sql3 = "CREATE TABLE inventory_slot (player_id INTEGER, slot_num INTEGER, item_id INTEGER DEFAULT -1, item_count INTEGER DEFAULT 0, FOREIGN KEY(player_id) REFERENCES player_info(id));";
    rc = sqlite3_exec(db, sql3, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        printf("Fatal error 3 creating database file.\n\n");
        sqlite3_close(db);
        dialogue();
        return;
    }

    char *sql4 = "CREATE TABLE locations (player_id INTEGER, location INTEGER, state INTEGER DEFAULT 0, FOREIGN KEY(player_id) REFERENCES player_info(id));";
    rc = sqlite3_exec(db, sql4, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        printf("Fatal error 4 creating database file.\n\n");
        sqlite3_close(db);
        dialogue();
        return;
    }

    char *sql5 = "CREATE TABLE IF NOT EXISTS \"stats\" (player_id INTEGER, Attack INTEGER DEFAULT 0, Hitpoints INTEGER DEFAULT 11540, Mining INTEGER DEFAULT 0, Strength INTEGER DEFAULT 0, Agility INTEGER DEFAULT 0, Smithing INTEGER DEFAULT 0, Defence INTEGER DEFAULT 0, Herblore INTEGER DEFAULT 0, Fishing INTEGER DEFAULT 0, Ranged INTEGER DEFAULT 0, Thieving INTEGER DEFAULT 0, Cooking INTEGER DEFAULT 0, Prayer INTEGER DEFAULT 0, Crafting INTEGER DEFAULT 0, Firemaking INTEGER DEFAULT 0, Magic INTEGER DEFAULT 0, Fletching INTEGER DEFAULT 0, Woodcutting INTEGER DEFAULT 0, Runecraft INTEGER DEFAULT 0, Slayer INTEGER DEFAULT 0, Farming INTEGER DEFAULT 0, Construction INTEGER DEFAULT 0, Hunter INTEGER DEFAULT 0, FOREIGN KEY(player_id) REFERENCES player_info(id));";
    rc = sqlite3_exec(db, sql5, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        printf("Fatal error 5 creating database file.\n\n");
        sqlite3_close(db);
        dialogue();
        return;
    }
    sqlite3_close(db);
    return;
}

void check_db() {
    // Open database and check that it was successful.
    sqlite3 *db;
    int rc;

    rc = sqlite3_open("runescape.db", &db);
    if (rc != SQLITE_OK)
    {
        printf("login.c check_db error 1\n");
        sqlite3_close(db);
        return;
    }
    char *err_msg = 0;


    // Setup and execute a search for the input username
    char *sql = "SELECT attack FROM stats WHERE player_id = 1;";
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    // If rc errors, the user is creating an account and their username is in use. Try again.
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return create_db();
    }

    // If player is creating an account, username is unique.
    // If player is logging in, the credentials for the provided username have been updated globally.
    sqlite3_close(db);
    return;
}

// Main login function that runs every other login function 
int login(void)
{
    check_db();

    // Log In or Create Account?
    login_intro();

    // Get username from user and clear stdin
    char username[username_size], tmpusr[username_size], password[password_size], tmppswrd[password_size];
    printf("Username: ");
    fgets(username, username_size, stdin);
    clearstdin(username);

    // User chose to create an account
    if (login_bool == false)
    {
        strcpy(tmpusr, check_username(username));
        strcpy(username, tmpusr);
        if (username[0] == '~') // check_username returns '~' for error
        {
            return -1;
        }
        clearscreen();
        password_disclosure();
        fgets(password, password_size, stdin);
        clearstdin(password);
        strcpy(tmppswrd, check_password(password));
        strcpy(password, tmppswrd);
        if (password[0] == '~') // check_username returns '~' for error
        {
            return -1;
        }

        // Ask user if they are okay with their input credentials
        char response[9];
        do
        {
            clearscreen();
            printf("Selected username: %s\nSelected password: %s\nPlease \"Confirm\" or \"Deny\" your account creation.\n", username, password);
            finput(response, sizeof(response));
        } while (strcmp(response, "confirm") != 0 && strcmp(response, "deny") != 0);

        // Add user to db and ensure it's success
        if (strcmp(response, "confirm") == 0)
        {
            char new_user_check[strlen(username)];
            strcpy(new_user_check, add_user(username, password));
            if (new_user_check[0] == '~')
            {
                return -1;
            }
            //  User successfully created acct. Return their id.
            return v_id;
        }
        // If user denies credentials, restart program
        return login();
    }

    // User chose to log in
    else if (login_bool == true)
    {
        int tries = 0;
        do
        {
            // After a failed attempt to login, inform user and get new username
            if (tries >= 1)
            {
                printf("Invalid username or password.\n\n");
                // Check if player wants to exit every third failed attempt
                if (tries % 3 == 0)
                {
                    char response[5];
                    bool tried = false;
                    do
                    {
                        clearscreen();
                        printf("Invalid username or password.\n\n");
                        if (tried == true) printf("Please type \"Yes\" or \"No\"\n");
                        else tried = true;
                        printf("Looks like you're having trouble logging in.\n");
                        printf("Would you like to exit? \"Yes\" or \"No\" ");
                        finput(response, sizeof(response));
                    } while (strcmp(response, "yes") != 0 && strcmp(response, "no") != 0);

                    if (strcmp(response, "yes") == 0)
                    {
                        return login();
                    }
                    clearscreen();
                }
                // Reprompt to reattempt login
                printf("Username: ");
                fgets(username, username_size, stdin);
                clearstdin(username);
            }
            tries++;

            // Check that input username works, and if so, copy it into username
            strcpy(tmpusr, check_username(username));
            strcpy(username, tmpusr);
            if (username[0] == '~') // check_username returns '~' for error
            {
                return -1;
            }

            // Once username confirmed ok, get a password and check it's usage
            printf("Password: ");
            fgets(password, password_size, stdin);
            clearstdin(password);

            if (strlen(username) || strlen(password)) {
                clearscreen();
                continue;
            }

            // Remove \n from password.
            for (int i = 0; i < strlen(password); i++)
            {
                if (password[i] == 10)
                {
                    password[i] = 0;
                }
            }
            clearscreen();
            // If input password matches returned password from username, finish login.
        } while (strcmp(password, v_password) != 0);
        
    }
    // v_id obtained from db in search_username -> callback
    return v_id;
}

// Callback function used to handle db->memory work
int login_callback(void *NotUsed, int argc, char **argv, char **azColName)
{    
    NotUsed = 0;
    // If logging in, retrieve data upon finding player's username
    if (login_bool)
    {
        if (argc != 0)
        {
            v_id = atoi(argv[0]);
            strcpy(v_username, argv[1]);
            strcpy(v_password, argv[2]);
        }
    }
    // If creating an account, return an error if the username is found
    else
    {
        if (argc != 0) // Check if the username was found
        {
            clearscreen();
            printf("Username already in use.\n"); // If found, tell user and return 1 to error
            return 1;
        }
    }
    return 0; // If not found, return successfully
}

// Check if user credentials exist, and if so, get them
char *search_username(char *username)
{
    // Open database and check that it was successful.
    sqlite3 *db;
    int rc;

    rc = sqlite3_open("runescape.db", &db);
    if (rc != SQLITE_OK)
    {
        printf("login.c error 1\n");
        sqlite3_close(db);
        return "~";
    }
    char *err_msg = 0;


    // Setup and execute a search for the input username
    char *tmp = "SELECT * FROM credentials WHERE username LIKE \"\"; ";
    char sql[strlen(tmp) + strlen(username)];
    snprintf(sql, strlen(tmp) + strlen(username), "SELECT * FROM credentials WHERE username LIKE \"%s\";", username);
    rc = sqlite3_exec(db, sql, login_callback, 0, &err_msg);
    // If rc errors, the user is creating an account and their username is in use. Try again.
    if (rc != SQLITE_OK)
    {
        printf("Username: ");
        fgets(username, username_size, stdin);
        clearstdin(username);
        // Close db and start from scratch, checking NEW username for acceptability
        sqlite3_close(db);
        return check_username(username);
    }

    // If player is creating an account, username is unique.
    // If player is logging in, the credentials for the provided username have been updated globally.
    sqlite3_close(db);
    return username;
}

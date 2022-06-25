#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_functions.h"
#include "global.h"
#include "login.h"
#include "sqlite3.h"

bool login_bool;
extern int v_id;
extern char v_username[username_size];
extern char v_password[password_size];

// Main login function that runs every other login function 
int login(void)
{
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
                        if (tried == true)
                        {
                            printf("Please type \"Yes\" or \"No\"\n");
                        }
                        tried = true;
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

// print "db not secure, use fake password"
void password_disclosure()
{
    printf("-----------------------------------------------------------------\n");
    printf("Note: Password is NOT hashed and is NOT protected in my database.\nPlease DO NOT reuse a password you have used previously and\nDO NOT assume this account or account information is secure.\n\nPassword: ");
}

// Check if user is logging in or creating a new account
void login_intro()
{
    // User chooses to create an account or log in to an existing account
    clearscreen();
    printf("========RuneScape========\n");
    printf("Create Account or Log In? ");
    char login_answer[16];
    bool tried = false; // If they type the wrong thing at least once, give them more instructions
    do
    {
        if (tried == true)
        {
            printf("Type \"Create Account\" or \"Log In\" ");
        }
        else
        {
            tried = true;
        }
        // Get response from user and clear stdin
        finput(login_answer, sizeof(login_answer));
    } while (strcmp(login_answer, "create account") != 0 && strcmp(login_answer, "log in") != 0);
    // Update global variable for use elsewhere
    if (strcmp(login_answer, "create account") == 0)
    {
        login_bool = false;
    }
    else
    {
        login_bool = true;
    }
}

// Check if a username meets criteria to be a username
char *check_username(char *username)
{
    int usr_len = strlen(username);
    // Check username for acceptability
    if (login_bool == false)
    {
        // Check length req
        if (usr_len <= 13 && username[0] != ' ')
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
    char *tmp = "SELECT * FROM player_info WHERE username LIKE \"\"; ";
    char sql[strlen(tmp) + strlen(username)];
    snprintf(sql, strlen(tmp) + strlen(username), "SELECT * FROM player_info WHERE username LIKE \"%s\";", username);
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

    // Insert users selected usrnm & pswrd to player_info
    char *tmp1 = "INSERT INTO player_info (username, password) VALUES(\"\", \"\");";
    char sql1[strlen(tmp1) + strlen(username) + strlen(password)];
    snprintf(sql1, strlen(tmp1) + strlen(username) + strlen(password), "INSERT INTO player_info (username, password) VALUES(\"%s\", \"%s\");", username, password);
    rc = sqlite3_exec(db, sql1, 0, 0, &err_msg);
    // If rc errors, something unexpected happened. Fatal Error
    if (rc != SQLITE_OK)
    {
        printf("login.c error 3\n");
        sqlite3_close(db);
        return "~";
    }
    
    // Retrieve newly created account data
    login_bool = true;
    search_username(username);

    // Create player_stats data for new character
    char *tmp2 = "INSERT INTO player_stats VALUES(,0,0,0,0,0,0,0,0,0,0,0,11540,0,0,0,0,0,0,0,0,0,0,0,\"Tutorial Island Spawn\");";
    char sql2[strlen(tmp2) + sizeof(int)];
    snprintf(sql2, strlen(tmp2) + sizeof(int), "INSERT INTO player_stats VALUES(%i,0,0,0,0,0,0,0,0,0,0,0,11540,0,0,0,0,0,0,0,0,0,0,0,\"Tutorial Island Spawn\");", v_id);
    rc = sqlite3_exec(db, sql2, 0, 0, &err_msg);
    // If rc errors, something unexpected happened. Fatal Error
    if (rc != SQLITE_OK)
    {
        printf("login.c error 4\n");
        sqlite3_close(db);
        return "~";
    }

    // Create tutorial_island data for new character
    char *tmp3 = "INSERT INTO tutorial_island VALUES(,0,0);";
    char sql3[strlen(tmp3) + sizeof(int)];
    snprintf(sql3, strlen(tmp3) + sizeof(int), "INSERT INTO tutorial_island VALUES(%i,0,0);", v_id);
    rc = sqlite3_exec(db, sql3, 0, 0, &err_msg);
    // If rc errors, something unexpected happened. Fatal Error
    if (rc != SQLITE_OK)
    {
        printf("login.c error 5\n");
        sqlite3_close(db);
        return "~";
    }

    // If rc ok, users credentials should be added successfully.
    sqlite3_close(db);
    clearscreen();
    return username;
}

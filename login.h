#ifndef LOGIN_H
#define LOGIN_H

bool login_bool;
int login(void);
void password_disclosure();
void login_intro();
char *check_username(char *username);
char *search_username(char *username);
int login_callback(void *NotUsed, int argc, char **argv, char **azColName);
char *check_password(char *password);
char *add_user(char *username, char *password);

#endif
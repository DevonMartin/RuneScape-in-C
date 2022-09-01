#ifndef LOGIN_H
#define LOGIN_H

bool login_bool;
int v_id;
char v_password[password_size];
int login(void);
char *search_username(char *username);

#endif
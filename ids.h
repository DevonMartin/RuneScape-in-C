#ifndef IDS_H
#define IDS_H

typedef struct item {
    char *name;
    char *examine;
} item;

typedef struct object {
    char *name;
    char *examine;
} object;

void initialize_items(item items[]);
void initialize_objects(object objects[]);
int examine_object(char location[], char *obj, object objects[]);
int search_object(char location[], char *obj, object objects[]);
void (*search_obj[15000])();
void init_search();
void search_380(void);
int open_object(char location[], char *obj, object objects[], int v_id);
void (*open_obj[15000])(int, char []);
void init_open();
void open_103();
void open_348();
void open_1533();
int gielinor_guide_1;
int gielinor_guide_2;

#endif
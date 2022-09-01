#ifndef ITEMS_H
#define ITEMS_H

#include "structs.h"

Item items[26500];
Inv_Slot inv_slots[29];
void init_items();
bool item_in_inv(int item);
bool inv_has_room();
int get_item(int item, int amount);
int pickup_item();
void remove_tmp_item(int loc);
void remove_expired_tmp_items();
void remove_expired_tmp_objs();
void add_tmp_item(Tmp_Item_At_Loc item);
void add_tmp_obj(Tmp_Obj_At_Loc obj);
bool tmp_obj_exists(int obj_id);
void examine_item(int item);
void remove_item_from_inv(int inv_slot);
int inventory_menu_options();

#endif
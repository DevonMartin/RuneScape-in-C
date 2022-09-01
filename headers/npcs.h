#ifndef NPCS_H
#define NPCS_H

#include "structs.h"

extern NPC NPCs[12000];

char new_player_intro(char *username, int v_id);
void SurvivalExpert_6_2();
void SurvivalExpert_6_3();
void SurvivalExpert_6_4();
void SurvivalExpert_6_5();
void SurvivalExpert_6_6();
void SurvivalExpert_6_7();
void SurvivalExpert_6_8();

void init_npcs();
int talk_to_npc();
#endif
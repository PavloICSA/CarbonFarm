#ifndef SIMPLE_UI_H
#define SIMPLE_UI_H

#include "input.h"
#include "compute.h"

// Simple console UI without ncurses dependency
int run_simple_ui(void);
void display_menu(void);
int get_menu_choice(void);
void pause_for_input(void);
int run_interactive_form(LegacyFarmData *farm);
void display_simple_results(const LegacyFarmData *farm, const EmissionResults *results);
void display_simple_help(void);

#endif
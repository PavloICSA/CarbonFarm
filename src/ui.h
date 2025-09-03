#ifndef UI_H
#define UI_H

#include "input.h"
#include "compute.h"

// UI Constants
#define MAX_INPUT_LEN 50
#define MENU_ITEMS 4

// Key definitions for cross-platform compatibility
#define KEY_UP 1001
#define KEY_DOWN 1002
#define KEY_LEFT 1003
#define KEY_RIGHT 1004
#define KEY_F1 1005
#define KEY_ENTER 13
#define KEY_ESC 27
#define KEY_BACKSPACE 8

// Menu options
typedef enum {
    MENU_INTERACTIVE = 0,
    MENU_CSV_FILE,
    MENU_HELP,
    MENU_EXIT
} MenuOption;

// UI State
typedef struct {
    int current_menu;
    int current_field;
    char input_buffer[MAX_INPUT_LEN];
    int input_pos;
    LegacyFarmData legacy_farm_data;  // UI uses legacy format for simplicity
    EmissionResults results;
    int show_results;
} UIState;

// Function declarations
int init_ui(void);
void cleanup_ui(void);
int run_main_menu(UIState *state);
int run_interactive_input(UIState *state);
void display_results(UIState *state);
void display_help(void);
void draw_box(int y, int x, int height, int width, const char *title);
void draw_field(int y, int x, const char *label, const char *value, int is_selected);
int get_string_input(int y, int x, int width, char *buffer, const char *prompt);
void show_message(const char *message, int is_error);

// Helper functions for console UI
void clear_screen(void);
void set_cursor_position(int row, int col);
void set_color(int color_code);
void reset_color(void);
int get_key(void);
void print_progress_bar(double value, double max_value, int width, const char* color);

#endif
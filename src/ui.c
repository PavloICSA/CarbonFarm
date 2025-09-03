#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <sys/ioctl.h>
#endif

#include "ui.h"
#include "input.h"
#include "compute.h"
#include "report.h"

// Console dimensions
static int console_width = 80;
static int console_height = 25;

// Color codes for cross-platform support
#define COLOR_NORMAL 0
#define COLOR_SELECTED 1
#define COLOR_HEADER 2
#define COLOR_ERROR 3
#define COLOR_SUCCESS 4

// ANSI Color codes for enhanced UI
#define ANSI_RESET "\033[0m"
#define ANSI_BOLD "\033[1m"
#define ANSI_DIM "\033[2m"
#define ANSI_UNDERLINE "\033[4m"

// Text colors
#define ANSI_BLACK "\033[30m"
#define ANSI_RED "\033[31m"
#define ANSI_GREEN "\033[32m"
#define ANSI_YELLOW "\033[33m"
#define ANSI_BLUE "\033[34m"
#define ANSI_MAGENTA "\033[35m"
#define ANSI_CYAN "\033[36m"
#define ANSI_WHITE "\033[37m"

// Bright colors
#define ANSI_BRIGHT_RED "\033[91m"
#define ANSI_BRIGHT_GREEN "\033[92m"
#define ANSI_BRIGHT_YELLOW "\033[93m"
#define ANSI_BRIGHT_BLUE "\033[94m"
#define ANSI_BRIGHT_MAGENTA "\033[95m"
#define ANSI_BRIGHT_CYAN "\033[96m"

// Background colors
#define ANSI_BG_RED "\033[41m"
#define ANSI_BG_GREEN "\033[42m"
#define ANSI_BG_YELLOW "\033[43m"
#define ANSI_BG_BLUE "\033[44m"
#define ANSI_BG_MAGENTA "\033[45m"
#define ANSI_BG_CYAN "\033[46m"

static const char *menu_items[] = {
    "Interactive Input",
    "Load CSV File",
    "Help",
    "Exit"};

static const char *field_labels[] = {
    "Farm Size (hectares)",
    "Crop Type",
    "Nitrogen (kg N/ha)",
    "Phosphorus (kg P2O5/ha)",
    "Potassium (kg K2O/ha)",
    "Manure (kg/ha)",
    "Diesel (L/ha)",
    "Irrigation (mm)",
    "Dairy Cows",
    "Pigs",
    "Chickens"};

#define NUM_FIELDS (sizeof(field_labels) / sizeof(field_labels[0]))

int init_ui(void)
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        console_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        console_height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
    
    // Enable ANSI escape sequences on Windows 10+
    DWORD dwMode = 0;
    GetConsoleMode(hConsole, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole, dwMode);
#else
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        console_width = w.ws_col;
        console_height = w.ws_row;
    }
#endif
    
    return 1;
}

void cleanup_ui(void)
{
    // Reset console colors and clear screen
    printf("\033[0m\033[2J\033[H");
    fflush(stdout);
}

// Helper functions for console UI
void clear_screen(void) {
    printf("\033[2J\033[H");
    fflush(stdout);
}

void set_cursor_position(int row, int col) {
    printf("\033[%d;%dH", row + 1, col + 1);
}

void set_color(int color_code) {
    switch (color_code) {
        case COLOR_NORMAL:
            printf(ANSI_RESET);
            break;
        case COLOR_SELECTED:
            printf(ANSI_BG_CYAN ANSI_BLACK); // Cyan background with black text
            break;
        case COLOR_HEADER:
            printf(ANSI_BOLD ANSI_BRIGHT_CYAN); // Bold bright cyan
            break;
        case COLOR_ERROR:
            printf(ANSI_BOLD ANSI_BRIGHT_RED); // Bold bright red
            break;
        case COLOR_SUCCESS:
            printf(ANSI_BOLD ANSI_BRIGHT_GREEN); // Bold bright green
            break;
    }
}

// Helper function to print a progress bar
void print_progress_bar(double value, double max_value, int width, const char* color) {
    if (max_value <= 0) max_value = 1; // Avoid division by zero
    
    int filled = (int)((value / max_value) * width);
    if (filled > width) filled = width;
    
    printf("%s[", color);
    for (int i = 0; i < width; i++) {
        if (i < filled) {
            printf("#");  // Use ASCII character instead of Unicode
        } else {
            printf("-");  // Use ASCII character instead of Unicode
        }
    }
    printf("]" ANSI_RESET " %.2f", value);
}

void reset_color(void) {
    printf(ANSI_RESET);
}

int get_key(void) {
#ifdef _WIN32
    int ch = _getch();
    if (ch == 0 || ch == 224) { // Extended key
        ch = _getch();
        switch (ch) {
            case 72: return KEY_UP;
            case 80: return KEY_DOWN;
            case 75: return KEY_LEFT;
            case 77: return KEY_RIGHT;
            case 59: return KEY_F1;
        }
    }
    return ch;
#else
    struct termios old_tio, new_tio;
    int ch;
    
    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
    
    ch = getchar();
    
    if (ch == 27) { // ESC sequence
        ch = getchar();
        if (ch == '[') {
            ch = getchar();
            switch (ch) {
                case 'A': ch = KEY_UP; break;
                case 'B': ch = KEY_DOWN; break;
                case 'C': ch = KEY_RIGHT; break;
                case 'D': ch = KEY_LEFT; break;
            }
        }
    }
    
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
    return ch;
#endif
}

void draw_box(int y, int x, int height, int width, const char *title)
{
    // Use simple ASCII characters for maximum compatibility
    // Draw top border
    set_cursor_position(y, x);
    printf("+");
    for (int i = 1; i < width - 1; i++) printf("-");
    printf("+");
    
    // Draw sides
    for (int i = 1; i < height - 1; i++) {
        set_cursor_position(y + i, x);
        printf("|");
        // Clear the middle area
        for (int j = 1; j < width - 1; j++) printf(" ");
        printf("|");
    }
    
    // Draw bottom border
    set_cursor_position(y + height - 1, x);
    printf("+");
    for (int i = 1; i < width - 1; i++) printf("-");
    printf("+");
    
    // Draw title
    if (title) {
        int title_len = strlen(title);
        int title_x = x + (width - title_len - 2) / 2;
        if (title_x > x && title_x + title_len + 2 < x + width) {
            set_cursor_position(y, title_x);
            printf("[ %s ]", title);
        }
    }
    
    fflush(stdout);
}

int run_main_menu(UIState *state)
{
    int ch;

    while (1)
    {
        clear_screen();

        // Header with decorative elements
        set_cursor_position(0, (console_width - 50) / 2);
        printf("==================================================");
        set_cursor_position(1, (console_width - 35) / 2);
        printf("    Farm Carbon Footprint Estimator");
        set_cursor_position(2, (console_width - 50) / 2);
        printf("==================================================");

        // Draw menu box
        int menu_height = MENU_ITEMS + 4;
        int menu_width = 40;
        int menu_y = 5;
        int menu_x = (console_width - menu_width) / 2;

        draw_box(menu_y, menu_x, menu_height, menu_width, "Main Menu");

        // Draw menu items
        for (int i = 0; i < MENU_ITEMS; i++)
        {
            set_cursor_position(menu_y + 2 + i, menu_x + 2);
            if (i == state->current_menu)
            {
                set_color(COLOR_SELECTED);
                printf(" %d. %s ", i + 1, menu_items[i]);
                reset_color();
            } else {
                printf(" %d. %s", i + 1, menu_items[i]);
            }
        }

        // Instructions
        set_cursor_position(console_height - 3, 2);
        printf("Use UP/DOWN arrows to navigate, ENTER to select, Q to quit");

        fflush(stdout);
        ch = get_key();

        switch (ch)
        {
        case KEY_UP:
            state->current_menu = (state->current_menu - 1 + MENU_ITEMS) % MENU_ITEMS;
            break;
        case KEY_DOWN:
            state->current_menu = (state->current_menu + 1) % MENU_ITEMS;
            break;
        case '\n':
        case '\r':
            switch (state->current_menu)
            {
            case MENU_INTERACTIVE:
                if (run_interactive_input(state))
                {
                    state->results = calculate_legacy_emissions(&state->legacy_farm_data);
                    state->show_results = 1;
                    display_results(state);
                }
                break;
            case MENU_CSV_FILE:
            {
                char filename[256];
                if (get_string_input(console_height / 2, 10, 50, filename, "Enter CSV filename: "))
                {
                    if (read_csv_input(filename, &state->legacy_farm_data) &&
                        validate_legacy_input(&state->legacy_farm_data))
                    {
                        state->results = calculate_legacy_emissions(&state->legacy_farm_data);
                        state->show_results = 1;
                        display_results(state);
                    }
                    else
                    {
                        show_message("Failed to load CSV file or invalid data", 1);
                    }
                }
            }
            break;
            case MENU_HELP:
                display_help();
                break;
            case MENU_EXIT:
                return 0;
            }
            break;
        case 'q':
        case 'Q':
            return 0;
        case '1':
            state->current_menu = MENU_INTERACTIVE;
            break;
        case '2':
            state->current_menu = MENU_CSV_FILE;
            break;
        case '3':
            state->current_menu = MENU_HELP;
            break;
        case '4':
            state->current_menu = MENU_EXIT;
            break;
        }
    }
}
int run_interactive_input(UIState *state)
{
    int ch;
    char temp_values[NUM_FIELDS][MAX_INPUT_LEN];

    // Initialize with default values
    snprintf(temp_values[0], MAX_INPUT_LEN, "%.1f", state->legacy_farm_data.farm_size);
    strncpy(temp_values[1], state->legacy_farm_data.crop_type, MAX_INPUT_LEN - 1);
    snprintf(temp_values[2], MAX_INPUT_LEN, "%.1f", state->legacy_farm_data.nitrogen_kg_ha);
    snprintf(temp_values[3], MAX_INPUT_LEN, "%.1f", state->legacy_farm_data.phosphorus_kg_ha);
    snprintf(temp_values[4], MAX_INPUT_LEN, "%.1f", state->legacy_farm_data.potassium_kg_ha);
    snprintf(temp_values[5], MAX_INPUT_LEN, "%.1f", state->legacy_farm_data.manure_kg_ha);
    snprintf(temp_values[6], MAX_INPUT_LEN, "%.1f", state->legacy_farm_data.diesel_l_ha);
    snprintf(temp_values[7], MAX_INPUT_LEN, "%.1f", state->legacy_farm_data.irrigation_mm);
    snprintf(temp_values[8], MAX_INPUT_LEN, "%d", state->legacy_farm_data.dairy_cows);
    snprintf(temp_values[9], MAX_INPUT_LEN, "%d", state->legacy_farm_data.pigs);
    snprintf(temp_values[10], MAX_INPUT_LEN, "%d", state->legacy_farm_data.chickens);

    state->current_field = 0;

    while (1)
    {
        clear_screen();

        // Header
        set_cursor_position(1, (console_width - 30) / 2);
        printf("Farm Data Input");

        // Draw input form
        int form_height = NUM_FIELDS + 6;
        int form_width = 60;
        int form_y = 3;
        int form_x = (console_width - form_width) / 2;

        draw_box(form_y, form_x, form_height, form_width, "Enter Farm Information");

        // Draw input fields
        for (int i = 0; i < NUM_FIELDS; i++)
        {
            int field_y = form_y + 2 + i;
            int is_selected = (i == state->current_field);

            set_cursor_position(field_y, form_x + 2);
            if (is_selected)
            {
                set_color(COLOR_SELECTED);
                printf(" %-23s: %s ", field_labels[i], temp_values[i]);
                reset_color();
            } else {
                printf(" %-23s: %s", field_labels[i], temp_values[i]);
            }
        }

        // Instructions
        set_cursor_position(console_height - 4, 2);
        printf("UP/DOWN: Navigate  ENTER: Edit field  F1: Calculate  ESC: Back to menu");
        set_cursor_position(console_height - 3, 2);
        printf("Crop types: wheat, maize, sunflower, soybean, potato, vegetables");

        fflush(stdout);
        ch = get_key();

        switch (ch)
        {
        case KEY_UP:
            state->current_field = (state->current_field - 1 + NUM_FIELDS) % NUM_FIELDS;
            break;
        case KEY_DOWN:
            state->current_field = (state->current_field + 1) % NUM_FIELDS;
            break;
        case '\n':
        case '\r':
        {
            char prompt[100];
            snprintf(prompt, sizeof(prompt), "Enter %s: ", field_labels[state->current_field]);
            if (get_string_input(console_height / 2, 10, 40, temp_values[state->current_field], prompt))
            {
                // Value updated in temp_values
            }
        }
        break;
        case KEY_F1:
            // Validate and save all values
            {
                LegacyFarmData temp_farm = {0};

                // Parse values
                temp_farm.farm_size = atof(temp_values[0]);
                strncpy(temp_farm.crop_type, temp_values[1], sizeof(temp_farm.crop_type) - 1);
                temp_farm.nitrogen_kg_ha = atof(temp_values[2]);
                temp_farm.phosphorus_kg_ha = atof(temp_values[3]);
                temp_farm.potassium_kg_ha = atof(temp_values[4]);
                temp_farm.manure_kg_ha = atof(temp_values[5]);
                temp_farm.diesel_l_ha = atof(temp_values[6]);
                temp_farm.irrigation_mm = atof(temp_values[7]);
                temp_farm.dairy_cows = atoi(temp_values[8]);
                temp_farm.pigs = atoi(temp_values[9]);
                temp_farm.chickens = atoi(temp_values[10]);

                if (validate_legacy_input(&temp_farm))
                {
                    state->legacy_farm_data = temp_farm;
                    return 1;
                }
                else
                {
                    show_message("Invalid input values. Please check your entries.", 1);
                }
            }
            break;
        case KEY_ESC:
            return 0;
        }
    }
}

void display_results(UIState *state)
{
    int ch;

    while (1)
    {
        clear_screen();

        // Header
        set_cursor_position(0, (console_width - 60) / 2);
        printf("============================================================");
        set_cursor_position(1, (console_width - 30) / 2);
        printf("Carbon Footprint Results");
        set_cursor_position(2, (console_width - 60) / 2);
        printf("============================================================");

        // Farm info box
        int info_width = 50;
        int info_x = 5;
        draw_box(4, info_x, 5, info_width, "Farm Information");
        set_cursor_position(5, info_x + 2);
        printf("Farm Size: %.1f ha", state->legacy_farm_data.farm_size);
        set_cursor_position(6, info_x + 2);
        printf("Crop Type: %s", state->legacy_farm_data.crop_type);
        set_cursor_position(7, info_x + 2);
        printf("Total Emissions: %.2f tCO2e", state->results.total_emissions);

        // Emissions breakdown box with progress bars
        int breakdown_width = 55;
        int breakdown_x = console_width - breakdown_width - 5;
        draw_box(4, breakdown_x, 11, breakdown_width, "Emission Breakdown");
        
        // Find max emission for scaling bars
        double max_emission = state->results.fertilizer_emissions;
        if (state->results.manure_emissions > max_emission) max_emission = state->results.manure_emissions;
        if (state->results.fuel_emissions > max_emission) max_emission = state->results.fuel_emissions;
        if (state->results.irrigation_emissions > max_emission) max_emission = state->results.irrigation_emissions;
        if (state->results.livestock_emissions > max_emission) max_emission = state->results.livestock_emissions;
        
        set_cursor_position(5, breakdown_x + 2);
        printf("Fertilizer: ");
        print_progress_bar(state->results.fertilizer_emissions, max_emission, 15, "");
        printf(" tCO2e");
        
        set_cursor_position(6, breakdown_x + 2);
        printf("Manure:     ");
        print_progress_bar(state->results.manure_emissions, max_emission, 15, "");
        printf(" tCO2e");
        
        set_cursor_position(7, breakdown_x + 2);
        printf("Fuel:       ");
        print_progress_bar(state->results.fuel_emissions, max_emission, 15, "");
        printf(" tCO2e");
        
        set_cursor_position(8, breakdown_x + 2);
        printf("Irrigation: ");
        print_progress_bar(state->results.irrigation_emissions, max_emission, 15, "");
        printf(" tCO2e");
        
        set_cursor_position(9, breakdown_x + 2);
        printf("Livestock:  ");
        print_progress_bar(state->results.livestock_emissions, max_emission, 15, "");
        printf(" tCO2e");
        
        set_cursor_position(11, breakdown_x + 2);
        printf("Per Hectare: %.2f tCO2e/ha", state->results.per_hectare_emissions);

        // Recommendations box
        int rec_width = console_width - 10;
        int rec_height = 9;
        int rec_y = 16;
        draw_box(rec_y, 5, rec_height, rec_width, "Sustainability Recommendations");

        // Recommendations
        int rec_line = rec_y + 2;
        if (state->results.per_hectare_emissions < 2.0)
        {
            set_cursor_position(rec_line++, 7);
            printf("LOW emissions (< 2 tCO2e/ha) - Excellent sustainability!");
            set_cursor_position(rec_line++, 7);
            printf("  - Continue current sustainable practices");
            set_cursor_position(rec_line++, 7);
            printf("  - Consider carbon sequestration opportunities");
            set_cursor_position(rec_line++, 7);
            printf("  - Share your methods with other farmers");
        }
        else if (state->results.per_hectare_emissions < 5.0)
        {
            set_cursor_position(rec_line++, 7);
            printf("MODERATE emissions (2-5 tCO2e/ha)");
            set_cursor_position(rec_line++, 7);
            printf("  - Focus on highest emission categories above");
            set_cursor_position(rec_line++, 7);
            printf("  - Consider precision agriculture techniques");
            set_cursor_position(rec_line++, 7);
            printf("  - Gradual improvements can make a big difference");
        }
        else
        {
            set_cursor_position(rec_line++, 7);
            printf("HIGH emissions (> 5 tCO2e/ha) - Action needed!");
            set_cursor_position(rec_line++, 7);
            printf("  - Reduce fertilizer use through soil testing");
            set_cursor_position(rec_line++, 7);
            printf("  - Optimize fuel consumption and machinery efficiency");
            set_cursor_position(rec_line++, 7);
            printf("  - Consider organic farming practices");
        }

        // Instructions
        set_cursor_position(console_height - 4, 2);
        printf("S: Save report to file  C: Continue with another farm  ESC: Back to main menu");

        fflush(stdout);
        ch = get_key();

        switch (ch)
        {
        case 's':
        case 'S':
            save_legacy_report_to_file(&state->legacy_farm_data, &state->results, "report.txt");
            show_message("Report saved to report.txt", 0);
            break;
        case 'c':
        case 'C':
            // Reset farm data for new analysis
            memset(&state->legacy_farm_data, 0, sizeof(state->legacy_farm_data));
            // Initialize default values
            state->legacy_farm_data.farm_size = 50.0;
            strcpy(state->legacy_farm_data.crop_type, "wheat");
            state->legacy_farm_data.nitrogen_kg_ha = 120.0;
            state->legacy_farm_data.phosphorus_kg_ha = 40.0;
            state->legacy_farm_data.potassium_kg_ha = 60.0;
            state->legacy_farm_data.manure_kg_ha = 2000.0;
            state->legacy_farm_data.diesel_l_ha = 80.0;
            state->legacy_farm_data.irrigation_mm = 50.0;
            state->legacy_farm_data.dairy_cows = 10;
            state->legacy_farm_data.pigs = 5;
            state->legacy_farm_data.chickens = 100;
            state->show_results = 0;
            show_message("Starting new farm analysis...", 0);
            return;
        case KEY_ESC:
            return;
        }
    }
}

void display_help(void)
{
    int ch;

    while (1)
    {
        clear_screen();

        // Header
        set_cursor_position(1, (console_width - 25) / 2);
        printf("Help & Information");

        // Help content
        int help_y = 3;
        set_cursor_position(help_y++, 2);
        printf("Farm Carbon Footprint Estimator - Help");
        set_cursor_position(help_y++, 2);
        printf("=====================================");
        help_y++;

        set_cursor_position(help_y++, 2);
        printf("This tool calculates CO2-equivalent emissions from farm activities:");
        help_y++;

        set_cursor_position(help_y++, 2);
        printf("INPUT FIELDS:");
        set_cursor_position(help_y++, 4);
        printf("- Farm Size: Total farm area in hectares");
        set_cursor_position(help_y++, 4);
        printf("- Crop Type: wheat, maize, sunflower, soybean, potato, vegetables");
        set_cursor_position(help_y++, 4);
        printf("- Fertilizers: Nitrogen (N), Phosphorus (P2O5), Potassium (K2O) per hectare");
        set_cursor_position(help_y++, 4);
        printf("- Manure: Organic matter applied per hectare (kg)");
        set_cursor_position(help_y++, 4);
        printf("- Diesel: Fuel consumption per hectare (liters)");
        set_cursor_position(help_y++, 4);
        printf("- Irrigation: Water used per hectare (mm)");
        set_cursor_position(help_y++, 4);
        printf("- Livestock: Number of dairy cows, pigs, and chickens");
        help_y++;

        set_cursor_position(help_y++, 2);
        printf("EMISSION FACTORS (based on IPCC guidelines):");
        set_cursor_position(help_y++, 4);
        printf("- Nitrogen: 6.3 kg CO2e/kg N");
        set_cursor_position(help_y++, 4);
        printf("- Phosphorus: 1.5 kg CO2e/kg P2O5");
        set_cursor_position(help_y++, 4);
        printf("- Potassium: 1.0 kg CO2e/kg K2O");
        set_cursor_position(help_y++, 4);
        printf("- Diesel: 2.68 kg CO2e/liter");
        set_cursor_position(help_y++, 4);
        printf("- Livestock: Cow 1000, Pig 200, Chicken 5 kg CO2e/year");

        set_cursor_position(console_height - 2, 2);
        printf("Press any key to return to main menu");

        fflush(stdout);
        ch = get_key();
        return;
    }
}

int get_string_input(int y, int x, int width, char *buffer, const char *prompt)
{
    int ch;
    int pos = strlen(buffer);

    while (1)
    {
        // Clear input area
        set_cursor_position(y, x);
        printf("%-*s", width + 20, "");
        set_cursor_position(y, x);
        printf("%s%s", prompt, buffer);
        fflush(stdout);

        ch = get_key();

        switch (ch)
        {
        case '\n':
        case '\r':
            return 1;
        case KEY_ESC:
            return 0;
        case KEY_BACKSPACE:
        case 127:
            if (pos > 0)
            {
                buffer[--pos] = '\0';
            }
            break;
        default:
            if (isprint(ch) && pos < MAX_INPUT_LEN - 1)
            {
                buffer[pos++] = ch;
                buffer[pos] = '\0';
            }
            break;
        }
    }
}

void show_message(const char *message, int is_error)
{
    int msg_y = console_height / 2;
    int msg_x = (console_width - strlen(message)) / 2;

    set_cursor_position(msg_y, msg_x);
    if (is_error)
    {
        set_color(COLOR_ERROR);
    }
    else
    {
        set_color(COLOR_SUCCESS);
    }

    printf("%s", message);
    reset_color();
    
    set_cursor_position(msg_y + 1, (console_width - 20) / 2);
    printf("Press any key...");

    fflush(stdout);
    get_key();
}
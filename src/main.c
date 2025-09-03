/*
 * Farm Carbon Footprint Estimator - Unified Entry Point with Startup Menu
 * 
 * For proper UTF-8 symbol display:
 * - Windows: Run 'chcp 65001' before executing the program
 * - Linux/Mac: UTF-8 works by default in most terminals
 * 
 * If you see corrupted characters, the program will still work
 * but symbols will be replaced with ASCII equivalents.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"
#include "compute.h"
#include "report.h"
#include "ui.h"
#include "simple_ui.h"

// ANSI color codes for enhanced display
#ifdef _WIN32
    #define COLOR_HEADER ""
    #define COLOR_WARNING ""
    #define COLOR_SUCCESS ""
    #define COLOR_RESET ""
#else
    #define COLOR_HEADER "\033[1;36m"    // Bright cyan
    #define COLOR_WARNING "\033[1;33m"   // Bright yellow
    #define COLOR_SUCCESS "\033[1;32m"   // Bright green
    #define COLOR_RESET "\033[0m"        // Reset
#endif

void display_startup_menu(void) {
    printf("%s", COLOR_HEADER);
    printf("========================================\n");
    printf("    Farm Carbon Footprint Estimator\n");
    printf("========================================%s\n", COLOR_RESET);
    printf("\n");
    printf("Select Mode of Operation:\n");
    printf("  1. CLI Mode (Multi-Crop Support, interactive inputs)\n");
    printf("  2. Simple UI Mode (Multi-Crop or Legacy choice)\n");
    printf("  3. Advanced UI Mode (Legacy Single-Crop)\n");
    printf("  4. Batch Mode (load from CSV file)\n");
    printf("  5. Help\n");
    printf("  6. Exit\n");
    printf("\n");
    printf("========================================\n");
    printf("Enter your choice: ");
}

int get_menu_selection(void) {
    int choice;
    char buffer[10];
    
    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &choice) == 1 && choice >= 1 && choice <= 6) {
                return choice;
            }
        }
        
        printf("%sInvalid choice. Please enter a number between 1 and 6: %s", COLOR_WARNING, COLOR_RESET);
    }
}

void showHelp(void) {
    printf("%s", COLOR_HEADER);
    printf("========================================\n");
    printf("    Farm Carbon Footprint Estimator\n");
    printf("           Help & Usage Guide\n");
    printf("========================================%s\n", COLOR_RESET);
    printf("\n");
    printf("This tool calculates greenhouse gas emissions from agricultural activities.\n");
    printf("\n");
    printf("Mode Descriptions:\n");
    printf("  1. CLI Mode: Interactive command-line interface with multi-crop support\n");
    printf("  2. Simple UI: Menu-driven interface, choose multi-crop or legacy mode\n");
    printf("  3. Advanced UI: Full interactive interface (legacy single-crop)\n");
    printf("  4. Batch Mode: Process data from CSV files automatically\n");
    printf("\n");
    printf("CSV File Path Examples:\n");
    printf("  - Relative path: data/sample_input.csv\n");
    printf("  - Absolute path: C:\\Users\\YourName\\Documents\\farm_data.csv\n");
    printf("  - Current directory: farm_data.csv\n");
    printf("  - Linux/macOS: /home/user/data/farm.csv\n");
    printf("\n");
    printf("CSV File Formats:\n");
    printf("  Legacy (single-crop):\n");
    printf("    farm_size,crop_type,nitrogen,phosphorus,potassium,manure,diesel,irrigation,cows,pigs,chickens\n");
    printf("    50.0,wheat,120.0,40.0,60.0,2000.0,80.0,500.0,10,5,100\n");
    printf("\n");
    printf("  Multi-crop:\n");
    printf("    crop_id,area,nitrogen,phosphorus,potassium,manure,diesel,irrigation,pesticide_id,pesticide_rate\n");
    printf("    1,10.0,120.0,60.0,30.0,2000.0,80.0,450.0,1,2.5\n");
    printf("\n");
    printf("For more information, see the README.md file.\n");
    printf("\n");
    printf("%sThank you for using Farm Carbon Footprint Estimator!%s\n", COLOR_SUCCESS, COLOR_RESET);
}

int runCliMode(void) {
    char mode_choice;
    
    // Interactive mode - ask user which mode they prefer
    printf("%s=== CLI Mode - Farm Carbon Footprint Estimator ===%s\n\n", COLOR_HEADER, COLOR_RESET);
    printf("Choose input mode:\n");
    printf("1. Multi-crop farm (NEW) - Support for multiple crops and pesticides\n");
    printf("2. Single-crop farm (Legacy) - Traditional single crop analysis\n");
    printf("\nEnter your choice (1 or 2): ");
    
    if (scanf(" %c", &mode_choice) != 1) {
        printf("Invalid input. Using multi-crop mode.\n");
        mode_choice = '1';
    }
    // Clear input buffer after mode selection
    while (getchar() != '\n' && !feof(stdin));
    
    if (mode_choice == '2') {
        // Legacy single-crop mode
        LegacyFarmData legacy_farm = {0};
        EmissionResults results;
        
        if (!read_legacy_interactive_input(&legacy_farm)) {
            printf("%sFailed to read input data.%s\n", COLOR_WARNING, COLOR_RESET);
            return 1;
        }
        
        if (!validate_legacy_input(&legacy_farm)) {
            printf("%sInput validation failed. Please check your values.%s\n", COLOR_WARNING, COLOR_RESET);
            return 1;
        }
        
        results = calculate_legacy_emissions(&legacy_farm);
        print_legacy_report(&legacy_farm, &results);
        
        char save_choice;
        printf("\nSave report to file? (y/n): ");
        if (scanf(" %c", &save_choice) == 1 && (save_choice == 'y' || save_choice == 'Y')) {
            save_legacy_report_to_file(&legacy_farm, &results, "report.txt");
            printf("%sReport saved to report.txt%s\n", COLOR_SUCCESS, COLOR_RESET);
        }
        
        // Ask user if they want to continue with another farm
        char continue_choice;
        printf("\nWould you like to analyze another farm? (y/n): ");
        if (scanf(" %c", &continue_choice) == 1 && (continue_choice == 'y' || continue_choice == 'Y')) {
            // Clear input buffer
            while (getchar() != '\n' && !feof(stdin));
            return runCliMode(); // Restart CLI mode
        }
    } else {
        // New multi-crop mode
        FarmData farm = {0};
        EmissionResults results;
        
        if (!read_interactive_input(&farm)) {
            printf("%sFailed to read input data.%s\n", COLOR_WARNING, COLOR_RESET);
            return 1;
        }
        
        if (!validate_input(&farm)) {
            printf("%sInput validation failed. Please check your values.%s\n", COLOR_WARNING, COLOR_RESET);
            return 1;
        }
        
        results = calculate_emissions(&farm);
        print_report(&farm, &results);
        
        char save_choice;
        printf("\nSave report to file? (y/n): ");
        if (scanf(" %c", &save_choice) == 1 && (save_choice == 'y' || save_choice == 'Y')) {
            save_report_to_file(&farm, &results, "report.txt");
            printf("%sReport saved to report.txt%s\n", COLOR_SUCCESS, COLOR_RESET);
        }
        
        // Ask user if they want to continue with another farm
        char continue_choice;
        printf("\nWould you like to analyze another farm? (y/n): ");
        if (scanf(" %c", &continue_choice) == 1 && (continue_choice == 'y' || continue_choice == 'Y')) {
            // Clear input buffer
            while (getchar() != '\n' && !feof(stdin));
            return runCliMode(); // Restart CLI mode
        }
    }
    
    return 0;
}

int runSimpleUiMode(void) {
    printf("%s=== Simple UI Mode - Farm Carbon Footprint Estimator ===%s\n", COLOR_HEADER, COLOR_RESET);
    return run_simple_ui();
}

int runAdvancedUiMode(void) {
    UIState state = {0};
    
    printf("%s=== Advanced UI Mode - Farm Carbon Footprint Estimator ===%s\n", COLOR_HEADER, COLOR_RESET);
    
    // Initialize UI
    if (!init_ui()) {
        printf("%sFailed to initialize Advanced UI. Falling back to CLI mode.%s\n", COLOR_WARNING, COLOR_RESET);
        return runCliMode();
    }
    
    // Initialize default legacy farm data for UI compatibility
    state.legacy_farm_data.farm_size = 50.0;
    strcpy(state.legacy_farm_data.crop_type, "wheat");
    state.legacy_farm_data.nitrogen_kg_ha = 120.0;
    state.legacy_farm_data.phosphorus_kg_ha = 40.0;
    state.legacy_farm_data.potassium_kg_ha = 60.0;
    state.legacy_farm_data.manure_kg_ha = 2000.0;
    state.legacy_farm_data.diesel_l_ha = 80.0;
    state.legacy_farm_data.irrigation_mm = 50.0;
    state.legacy_farm_data.dairy_cows = 10;
    state.legacy_farm_data.pigs = 5;
    state.legacy_farm_data.chickens = 100;
    
    // Run main UI loop
    run_main_menu(&state);
    
    // Cleanup
    cleanup_ui();
    
    return 0;
}

int runBatchMode(void) {
    char filename[256];
    LegacyFarmData legacy_farm = {0};
    EmissionResults results;
    
    printf("%s=== Batch Mode - Farm Carbon Footprint Estimator ===%s\n", COLOR_HEADER, COLOR_RESET);
    printf("\nThis mode processes farm data from a CSV file.\n");
    printf("File path examples:\n");
    printf("  - Relative path: data/sample_input.csv\n");
    printf("  - Absolute path: C:\\Users\\YourName\\Documents\\farm_data.csv\n");
    printf("  - Current directory: farm_data.csv\n");
    printf("\nEnter CSV file path: ");
    
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("%sError reading filename.%s\n", COLOR_WARNING, COLOR_RESET);
        return 1;
    }
    
    // Remove newline character
    filename[strcspn(filename, "\n")] = 0;
    
    printf("Reading input from CSV file: %s\n", filename);
    if (!read_csv_input(filename, &legacy_farm)) {
        printf("%sFailed to read input data from file: %s%s\n", COLOR_WARNING, filename, COLOR_RESET);
        return 1;
    }
    
    if (!validate_legacy_input(&legacy_farm)) {
        printf("%sInput validation failed. Please check your values in the CSV file.%s\n", COLOR_WARNING, COLOR_RESET);
        return 1;
    }
    
    results = calculate_legacy_emissions(&legacy_farm);
    print_legacy_report(&legacy_farm, &results);
    save_legacy_report_to_file(&legacy_farm, &results, "report.txt");
    printf("%sReport saved to report.txt%s\n", COLOR_SUCCESS, COLOR_RESET);
    
    // Ask user if they want to continue with another farm
    char continue_choice;
    printf("\nWould you like to analyze another farm? (y/n): ");
    if (scanf(" %c", &continue_choice) == 1 && (continue_choice == 'y' || continue_choice == 'Y')) {
        // Clear input buffer
        while (getchar() != '\n' && !feof(stdin));
        return runBatchMode(); // Restart batch mode
    }
    
    return 0;
}

int main(int argc, char *argv[]) {
    int choice;
    int continue_program = 1;
    
    // If command line arguments are provided, handle legacy mode for backward compatibility
    if (argc > 1) {
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            showHelp();
            return 0;
        } else if (strcmp(argv[1], "--ui") == 0) {
            return runAdvancedUiMode();
        } else if (strcmp(argv[1], "--simple") == 0) {
            return runSimpleUiMode();
        } else {
            // CSV file mode - legacy support
            LegacyFarmData legacy_farm = {0};
            EmissionResults results;
            
            printf("Reading input from CSV file: %s\n", argv[1]);
            if (!read_csv_input(argv[1], &legacy_farm)) {
                printf("%sFailed to read input data. Exiting.%s\n", COLOR_WARNING, COLOR_RESET);
                return 1;
            }
            
            if (!validate_legacy_input(&legacy_farm)) {
                printf("%sInput validation failed. Please check your values.%s\n", COLOR_WARNING, COLOR_RESET);
                return 1;
            }
            
            results = calculate_legacy_emissions(&legacy_farm);
            print_legacy_report(&legacy_farm, &results);
            save_legacy_report_to_file(&legacy_farm, &results, "report.txt");
            printf("%sReport saved to report.txt%s\n", COLOR_SUCCESS, COLOR_RESET);
            
            // Ask user if they want to continue with another farm
            char continue_choice;
            printf("\nWould you like to analyze another farm? (y/n): ");
            if (scanf(" %c", &continue_choice) == 1 && (continue_choice == 'y' || continue_choice == 'Y')) {
                // Clear input buffer
                while (getchar() != '\n' && !feof(stdin));
                // For command line mode, we can't easily restart, so just inform user
                printf("%sTo analyze another farm, please run the program again.%s\n", COLOR_WARNING, COLOR_RESET);
            }
            
            return 0;
        }
    }
    
    // Main program loop with startup menu
    while (continue_program) {
        printf("\n");
        display_startup_menu();
        choice = get_menu_selection();
        printf("\n");
        
        switch (choice) {
            case 1:
                runCliMode();
                break;
            case 2:
                runSimpleUiMode();
                break;
            case 3:
                runAdvancedUiMode();
                break;
            case 4:
                runBatchMode();
                break;
            case 5:
                showHelp();
                break;
            case 6:
                printf("%sThank you for using Farm Carbon Footprint Estimator!%s\n", COLOR_SUCCESS, COLOR_RESET);
                continue_program = 0;
                break;
            default:
                printf("%sInvalid choice. Please try again.%s\n", COLOR_WARNING, COLOR_RESET);
                continue;
        }
        
        if (continue_program && choice != 6) {
            char continue_choice;
            printf("\n%sReturn to main menu? (y/n): %s", COLOR_HEADER, COLOR_RESET);
            if (scanf(" %c", &continue_choice) == 1 && (continue_choice == 'n' || continue_choice == 'N')) {
                continue_program = 0;
                printf("%sThank you for using Farm Carbon Footprint Estimator!%s\n", COLOR_SUCCESS, COLOR_RESET);
            }
            // Clear input buffer
            while (getchar() != '\n' && !feof(stdin));
        }
    }
    
    return 0;
}
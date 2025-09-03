#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
#endif

#include "simple_ui.h"
#include "input.h"
#include "compute.h"
#include "report.h"

// Set this to 1 to use UTF-8 symbols, 0 for ASCII
// Windows users: run 'chcp 65001' before execution for UTF-8 support
#define USE_UTF8_SYMBOLS 0

static void clear_screen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause_for_input(void) {
    printf("\nPress Enter to continue...");
    while (getchar() != '\n');
}

void display_menu(void) {
    clear_screen();
    printf("================================================================\n");
    printf("              Farm Carbon Footprint Estimator                  \n");
    printf("================================================================\n");
    printf("                                                              \n");
    printf("  1. Interactive Input (Enter farm data manually)            \n");
    printf("  2. Load CSV File (Batch processing)                        \n");
    printf("  3. Help & Information                                       \n");
    printf("  4. Exit                                                     \n");
    printf("                                                              \n");
    printf("================================================================\n");
    printf("\nEnter your choice (1-4): ");
}

int get_menu_choice(void) {
    int choice;
    char buffer[10];
    
    if (fgets(buffer, sizeof(buffer), stdin)) {
        if (sscanf(buffer, "%d", &choice) == 1 && choice >= 1 && choice <= 4) {
            return choice;
        }
    }
    return -1;
}

int run_interactive_form(LegacyFarmData *farm) {
    char buffer[100];
    
    clear_screen();
    printf("================================================================\n");
    printf("                    Farm Data Input Form                       \n");
    printf("================================================================\n\n");
    
    printf("Enter farm information (press Enter after each value):\n");
    printf("Note: All values must be non-negative (>= 0)\n\n");
    
    // Clear input buffer first
    while (getchar() != '\n' && !feof(stdin));
    
    // Farm size
    do {
        printf("Farm size (hectares, > 0): ");
        if (fgets(buffer, sizeof(buffer), stdin)) {
            if (sscanf(buffer, "%lf", &farm->farm_size) == 1) {
                if (farm->farm_size <= 0) {
                    printf("Error: Farm size must be greater than 0.\n");
                }
            } else {
                printf("Error: Invalid input. Please enter a positive number.\n");
                farm->farm_size = -1; // Force retry
            }
        } else {
            printf("Error: Failed to read input.\n");
            return 0;
        }
    } while (farm->farm_size <= 0);
    
    // Crop type
    do {
        printf("Crop type (wheat/maize/soybean/sunflower/potato/rice/barley/rapeseed/sugar beet/vegetables): ");
        if (fgets(buffer, sizeof(buffer), stdin)) {
            // Remove newline character
            buffer[strcspn(buffer, "\n")] = 0;
            strncpy(farm->crop_type, buffer, sizeof(farm->crop_type) - 1);
            farm->crop_type[sizeof(farm->crop_type) - 1] = '\0';
            
            if (!is_valid_crop_type(farm->crop_type)) {
                printf("Error: Invalid crop type. Valid options: wheat, maize, soybean, sunflower, potato, rice, barley, rapeseed, sugar beet, vegetables\n");
            }
        } else {
            printf("Error: Invalid input for crop type.\n");
            return 0;
        }
    } while (!is_valid_crop_type(farm->crop_type));
    
    // Nitrogen fertilizer
    do {
        printf("Nitrogen fertilizer (kg N per hectare, >= 0): ");
        if (fgets(buffer, sizeof(buffer), stdin) && sscanf(buffer, "%lf", &farm->nitrogen_kg_ha) == 1) {
            if (farm->nitrogen_kg_ha < 0) {
                printf("Error: Nitrogen fertilizer amount cannot be negative.\n");
            }
        } else {
            printf("Error: Invalid input. Please enter a non-negative number.\n");
            farm->nitrogen_kg_ha = -1; // Force retry
        }
    } while (farm->nitrogen_kg_ha < 0);
    
    // Phosphorus fertilizer
    do {
        printf("Phosphorus fertilizer (kg P2O5 per hectare, >= 0): ");
        if (fgets(buffer, sizeof(buffer), stdin) && sscanf(buffer, "%lf", &farm->phosphorus_kg_ha) == 1) {
            if (farm->phosphorus_kg_ha < 0) {
                printf("Error: Phosphorus fertilizer amount cannot be negative.\n");
            }
        } else {
            printf("Error: Invalid input. Please enter a non-negative number.\n");
            farm->phosphorus_kg_ha = -1; // Force retry
        }
    } while (farm->phosphorus_kg_ha < 0);
    
    // Potassium fertilizer
    do {
        printf("Potassium fertilizer (kg K2O per hectare, >= 0): ");
        if (fgets(buffer, sizeof(buffer), stdin) && sscanf(buffer, "%lf", &farm->potassium_kg_ha) == 1) {
            if (farm->potassium_kg_ha < 0) {
                printf("Error: Potassium fertilizer amount cannot be negative.\n");
            }
        } else {
            printf("Error: Invalid input. Please enter a non-negative number.\n");
            farm->potassium_kg_ha = -1; // Force retry
        }
    } while (farm->potassium_kg_ha < 0);
    
    // Manure
    do {
        printf("Manure applied (kg per hectare, >= 0): ");
        if (fgets(buffer, sizeof(buffer), stdin) && sscanf(buffer, "%lf", &farm->manure_kg_ha) == 1) {
            if (farm->manure_kg_ha < 0) {
                printf("Error: Manure amount cannot be negative.\n");
            }
        } else {
            printf("Error: Invalid input. Please enter a non-negative number.\n");
            farm->manure_kg_ha = -1; // Force retry
        }
    } while (farm->manure_kg_ha < 0);
    
    // Diesel consumption
    do {
        printf("Diesel consumption (liters per hectare, >= 0): ");
        if (fgets(buffer, sizeof(buffer), stdin) && sscanf(buffer, "%lf", &farm->diesel_l_ha) == 1) {
            if (farm->diesel_l_ha < 0) {
                printf("Error: Diesel consumption cannot be negative.\n");
            }
        } else {
            printf("Error: Invalid input. Please enter a non-negative number.\n");
            farm->diesel_l_ha = -1; // Force retry
        }
    } while (farm->diesel_l_ha < 0);
    
    // Irrigation water
    do {
        printf("Irrigation water (mm, >= 0): ");
        if (fgets(buffer, sizeof(buffer), stdin) && sscanf(buffer, "%lf", &farm->irrigation_mm) == 1) {
            if (farm->irrigation_mm < 0) {
                printf("Error: Irrigation water amount cannot be negative.\n");
            }
        } else {
            printf("Error: Invalid input. Please enter a non-negative number.\n");
            farm->irrigation_mm = -1; // Force retry
        }
    } while (farm->irrigation_mm < 0);
    
    // Dairy cows
    do {
        printf("Number of dairy cows (>= 0): ");
        if (fgets(buffer, sizeof(buffer), stdin) && sscanf(buffer, "%d", &farm->dairy_cows) == 1) {
            if (farm->dairy_cows < 0) {
                printf("Error: Number of dairy cows cannot be negative.\n");
            }
        } else {
            printf("Error: Invalid input. Please enter a non-negative integer.\n");
            farm->dairy_cows = -1; // Force retry
        }
    } while (farm->dairy_cows < 0);
    
    // Pigs
    do {
        printf("Number of pigs (>= 0): ");
        if (fgets(buffer, sizeof(buffer), stdin) && sscanf(buffer, "%d", &farm->pigs) == 1) {
            if (farm->pigs < 0) {
                printf("Error: Number of pigs cannot be negative.\n");
            }
        } else {
            printf("Error: Invalid input. Please enter a non-negative integer.\n");
            farm->pigs = -1; // Force retry
        }
    } while (farm->pigs < 0);
    
    // Chickens
    do {
        printf("Number of chickens (>= 0): ");
        if (fgets(buffer, sizeof(buffer), stdin) && sscanf(buffer, "%d", &farm->chickens) == 1) {
            if (farm->chickens < 0) {
                printf("Error: Number of chickens cannot be negative.\n");
            }
        } else {
            printf("Error: Invalid input. Please enter a non-negative integer.\n");
            farm->chickens = -1; // Force retry
        }
    } while (farm->chickens < 0);
    
    return 1;
}

// Helper function to print a simple progress bar
void print_simple_progress_bar(double value, double max_value, int width, const char* color) {
    if (max_value <= 0) max_value = 1; // Avoid division by zero
    
    int filled = (int)((value / max_value) * width);
    if (filled > width) filled = width;
    
    printf("[");
    for (int i = 0; i < width; i++) {
        if (i < filled) {
            printf("#");
        } else {
            printf("-");
        }
    }
    printf("] %.2f", value);
}

void display_simple_results(const LegacyFarmData *farm, const EmissionResults *results) {
    const char* co2_unit = "tCO2e";
    const char* check_mark = "*";
    const char* warning = "!";
    const char* bullet = "-";
    
    clear_screen();
    
    // Header
    printf("================================================================\n");
    printf("                 Carbon Footprint Results                      \n");
    printf("================================================================\n\n");
    
    // Farm information
    printf("--- Farm Information -------------------------------------------\n");
    printf("  Farm Size: %.1f hectares\n", farm->farm_size);
    printf("  Crop Type: %s\n", farm->crop_type);
    printf("  Total Emissions: %.2f %s\n", results->total_emissions, co2_unit);
    printf("  Per Hectare: %.2f %s/ha\n", results->per_hectare_emissions, co2_unit);
    printf("----------------------------------------------------------------\n\n");
    
    // Emission breakdown with progress bars
    printf("--- Emission Breakdown with Visual Bars --------------------\n");
    
    // Find max emission for scaling bars
    double max_emission = results->fertilizer_emissions;
    if (results->manure_emissions > max_emission) max_emission = results->manure_emissions;
    if (results->fuel_emissions > max_emission) max_emission = results->fuel_emissions;
    if (results->irrigation_emissions > max_emission) max_emission = results->irrigation_emissions;
    if (results->livestock_emissions > max_emission) max_emission = results->livestock_emissions;
    
    printf("  Fertilizer:  ");
    print_simple_progress_bar(results->fertilizer_emissions, max_emission, 20, "");
    printf(" %s\n", co2_unit);
    
    printf("  Manure:      ");
    print_simple_progress_bar(results->manure_emissions, max_emission, 20, "");
    printf(" %s\n", co2_unit);
    
    printf("  Fuel:        ");
    print_simple_progress_bar(results->fuel_emissions, max_emission, 20, "");
    printf(" %s\n", co2_unit);
    
    printf("  Irrigation:  ");
    print_simple_progress_bar(results->irrigation_emissions, max_emission, 20, "");
    printf(" %s\n", co2_unit);
    
    printf("  Livestock:   ");
    print_simple_progress_bar(results->livestock_emissions, max_emission, 20, "");
    printf(" %s\n", co2_unit);
    
    printf("----------------------------------------------------------------\n\n");
    
    // Sustainability recommendations
    printf("--- Sustainability Recommendations -------------------------\n");
    
    if (results->per_hectare_emissions < 2.0) {
        printf("  %s LOW emissions (< 2 %s/ha) - Excellent sustainability!\n", check_mark, co2_unit);
        printf("    - Continue current sustainable practices\n");
        printf("    - Consider carbon sequestration opportunities\n");
        printf("    - Share your methods with other farmers\n");
    } else if (results->per_hectare_emissions < 5.0) {
        printf("  %s MODERATE emissions (2-5 %s/ha)\n", warning, co2_unit);
        printf("    - Focus on highest emission categories above\n");
        printf("    - Consider precision agriculture techniques\n");
        printf("    - Gradual improvements can make a big difference\n");
    } else {
        printf("  %s HIGH emissions (> 5 %s/ha) - Action needed!\n", warning, co2_unit);
        printf("    - Reduce fertilizer use through soil testing\n");
        printf("    - Optimize fuel consumption and machinery efficiency\n");
        printf("    - Consider organic farming practices\n");
    }
    
    printf("----------------------------------------------------------------\n\n");
    
    printf("Options:\n");
    printf("  S - Save report to file\n");
    printf("  C - Continue with another farm\n");
    printf("  Enter - Return to main menu\n");
    printf("\nYour choice: ");
    
    char choice = getchar();
    if (choice == 's' || choice == 'S') {
        save_legacy_report_to_file(farm, results, "report.txt");
        printf("\n%s Report saved to report.txt\n", check_mark);
        pause_for_input();
    } else if (choice == 'c' || choice == 'C') {
        // Clear input buffer
        while (getchar() != '\n' && !feof(stdin));
        printf("\nStarting new farm analysis...\n");
        pause_for_input();
        return; // This will cause the simple UI to restart its loop
    }
    
    // Clear input buffer
    while (getchar() != '\n' && !feof(stdin));
}

void display_simple_help(void) {
    clear_screen();
    printf("================================================================\n");
    printf("                    Help & Information                          \n");
    printf("================================================================\n\n");
    
    printf("ABOUT:\n");
    printf("The Farm Carbon Footprint Estimator calculates CO2-equivalent\n");
    printf("emissions from various agricultural activities using scientifically\n");
    printf("established emission factors based on IPCC guidelines.\n\n");
    
    const char* bullet = "-";
    const char* co2_unit = "CO2e";
    
    printf("INPUT FIELDS:\n");
    printf("%s Farm Size: Total cultivated area in hectares\n", bullet);
    printf("%s Crop Type: wheat, maize, soybean, sunflower, potato, rice, barley, rapeseed, sugar beet, vegetables\n", bullet);
    printf("%s Fertilizers: Amount of N, P2O5, K2O applied per hectare\n", bullet);
    printf("%s Manure: Organic matter applied per hectare (kg)\n", bullet);
    printf("%s Diesel: Fuel consumption per hectare (liters)\n", bullet);
    printf("%s Irrigation: Water used per hectare (mm)\n", bullet);
    printf("%s Livestock: Number of animals (cows, pigs, chickens)\n\n", bullet);
    
    printf("EMISSION FACTORS:\n");
    printf("%s Nitrogen fertilizer: 6.3 kg %s per kg N\n", bullet, co2_unit);
    printf("%s Phosphorus: 1.5 kg %s per kg P2O5\n", bullet, co2_unit);
    printf("%s Potassium: 1.0 kg %s per kg K2O\n", bullet, co2_unit);
    printf("%s Manure: 0.6 kg %s per kg\n", bullet, co2_unit);
    printf("%s Diesel fuel: 2.68 kg %s per liter\n", bullet, co2_unit);
    printf("%s Irrigation: 5.0 kg %s per mm\n", bullet, co2_unit);
    printf("%s Livestock (annual): Cow 1000, Pig 200, Chicken 5 kg %s\n\n", bullet, co2_unit);
    
    printf("CSV FORMAT:\n");
    printf("farm_size,crop_type,nitrogen,phosphorus,potassium,manure,\n");
    printf("diesel,irrigation,cows,pigs,chickens\n\n");
    
    printf("EXAMPLE:\n");
    printf("50.0,wheat,120.0,40.0,60.0,2000.0,80.0,500.0,10,5,100\n\n");
    
    pause_for_input();
}

int run_simple_ui(void) {
    char mode_choice;
    int choice;
    char filename[256];
    
    // Ask user which mode they prefer
    clear_screen();
    printf("=== Farm Carbon Footprint Estimator - Simple UI ===\n\n");
    printf("Choose input mode:\n");
    printf("1. Multi-crop farm (NEW) - Support for multiple crops and pesticides\n");
    printf("2. Single-crop farm (Legacy) - Traditional single crop analysis\n");
    printf("\nEnter your choice (1 or 2): ");
    
    if (scanf(" %c", &mode_choice) != 1) {
        printf("Invalid input. Using legacy mode for Simple UI compatibility.\n");
        mode_choice = '2';
    }
    // Clear input buffer after mode selection
    while (getchar() != '\n' && !feof(stdin));
    
    if (mode_choice == '1') {
        // Multi-crop mode
        FarmData farm = {0};
        EmissionResults results;
        
        while (1) {
            display_menu();
            choice = get_menu_choice();
            
            switch (choice) {
                case 1: // Interactive input
                    do {
                        if (read_interactive_input(&farm)) {
                            if (validate_input(&farm)) {
                                results = calculate_emissions(&farm);
                                print_report(&farm, &results);
                                
                                printf("\nSave report to file? (y/n): ");
                                char save_choice;
                                if (scanf(" %c", &save_choice) == 1 && (save_choice == 'y' || save_choice == 'Y')) {
                                    save_report_to_file(&farm, &results, "report.txt");
                                    printf("Report saved to report.txt\n");
                                }
                                
                                // Ask if user wants to continue with another farm
                                printf("\nWould you like to analyze another farm? (y/n): ");
                                char continue_choice;
                                if (scanf(" %c", &continue_choice) == 1 && (continue_choice == 'y' || continue_choice == 'Y')) {
                                    // Clear input buffer and reset farm data
                                    while (getchar() != '\n' && !feof(stdin));
                                    memset(&farm, 0, sizeof(farm));
                                    printf("\nStarting new farm analysis...\n");
                                    pause_for_input();
                                    continue; // Continue the do-while loop for another analysis
                                } else {
                                    // Clear input buffer
                                    while (getchar() != '\n' && !feof(stdin));
                                    break; // Exit the do-while loop
                                }
                            } else {
                                printf("\nInput validation failed. Please check your values.\n");
                                pause_for_input();
                                break;
                            }
                        } else {
                            printf("\nInput failed. Please try again.\n");
                            pause_for_input();
                            break;
                        }
                    } while (1);
                    break;
                    
                case 2: // CSV file - not supported in multi-crop mode yet
                    printf("CSV input not yet supported for multi-crop mode.\n");
                    printf("Please use interactive input or legacy mode.\n");
                    pause_for_input();
                    break;
                    
                case 3: // Help
                    display_simple_help();
                    break;
                    
                case 4: // Exit
                    clear_screen();
                    printf("Thank you for using Farm Carbon Footprint Estimator!\n\n");
                    return 0;
                    
                default:
                    printf("Invalid choice. Please enter 1-4.\n");
                    pause_for_input();
                    break;
            }
        }
    } else {
        // Legacy single-crop mode
        LegacyFarmData farm = {0};
        EmissionResults results;
        int continue_analysis = 1;
        
        while (continue_analysis) {
            display_menu();
            choice = get_menu_choice();
            
            switch (choice) {
                case 1: // Interactive input
                    do {
                        if (run_interactive_form(&farm)) {
                            if (validate_legacy_input(&farm)) {
                                results = calculate_legacy_emissions(&farm);
                                display_simple_results(&farm, &results);
                                
                                // Ask if user wants to continue with another farm
                                printf("\n\033[96mWould you like to analyze another farm? (y/n): \033[0m");
                                char continue_choice;
                                if (scanf(" %c", &continue_choice) == 1 && (continue_choice == 'y' || continue_choice == 'Y')) {
                                    // Clear input buffer and reset farm data
                                    while (getchar() != '\n' && !feof(stdin));
                                    memset(&farm, 0, sizeof(farm));
                                    printf("\n\033[92m🔄 Starting new farm analysis...\033[0m\n");
                                    pause_for_input();
                                    continue; // Continue the do-while loop for another analysis
                                } else {
                                    // Clear input buffer
                                    while (getchar() != '\n' && !feof(stdin));
                                    break; // Exit the do-while loop
                                }
                            } else {
                                printf("\nInput validation failed. Please check your values.\n");
                                pause_for_input();
                                break;
                            }
                        } else {
                            printf("\nInput failed. Please try again.\n");
                            pause_for_input();
                            break;
                        }
                    } while (1);
                    break;
                    
                case 2: // CSV file
                    do {
                        clear_screen();
                        printf("Enter CSV filename: ");
                        if (fgets(filename, sizeof(filename), stdin)) {
                            // Remove newline
                            filename[strcspn(filename, "\n")] = 0;
                            
                            if (read_csv_input(filename, &farm) && validate_legacy_input(&farm)) {
                                results = calculate_legacy_emissions(&farm);
                                display_simple_results(&farm, &results);
                                
                                // Ask if user wants to continue with another farm
                                printf("\n\033[96mWould you like to analyze another farm? (y/n): \033[0m");
                                char continue_choice;
                                if (scanf(" %c", &continue_choice) == 1 && (continue_choice == 'y' || continue_choice == 'Y')) {
                                    // Clear input buffer and reset farm data
                                    while (getchar() != '\n' && !feof(stdin));
                                    memset(&farm, 0, sizeof(farm));
                                    printf("\n\033[92m🔄 Starting new farm analysis...\033[0m\n");
                                    pause_for_input();
                                    continue; // Continue the do-while loop for another analysis
                                } else {
                                    // Clear input buffer
                                    while (getchar() != '\n' && !feof(stdin));
                                    break; // Exit the do-while loop
                                }
                            } else {
                                printf("Failed to load CSV file or invalid data.\n");
                                pause_for_input();
                                break;
                            }
                        } else {
                            printf("Invalid filename.\n");
                            pause_for_input();
                            break;
                        }
                    } while (1);
                    break;
                
                case 3: // Help
                    display_simple_help();
                    break;
                    
                case 4: // Exit
                    clear_screen();
                    printf("Thank you for using Farm Carbon Footprint Estimator!\n\n");
                    return 0;
                    
                default:
                    printf("Invalid choice. Please enter 1-4.\n");
                    pause_for_input();
                    break;
            }
        }
    }
}
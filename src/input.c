#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "input.h"

#ifdef _WIN32
#define strcasecmp _stricmp
#endif

// Predefined crop data with default agronomic parameters
Crop crops[] = {
    {"Wheat",      120, 60, 30, 450, 5.5},
    {"Maize",      150, 70, 40, 500, 7.0},
    {"Soybean",     20, 50, 30, 400, 2.5},
    {"Sunflower",   60, 40, 40, 380, 2.2},
    {"Potato",     180, 90,100, 600, 30.0},
    {"Rice",       160, 70, 60,1200, 6.5},
    {"Barley",     100, 50, 30, 400, 4.5},
    {"Rapeseed",   160, 70, 50, 450, 3.0},
    {"Sugar beet", 180, 80, 90, 550, 60.0},
    {"Vegetables", 120, 80, 70, 500, 25.0}
};
int num_crops = sizeof(crops) / sizeof(crops[0]);

// Predefined pesticide data with emission factors
Pesticide pesticides[] = {
    {"Roundup",     "Glyphosate",          "Herbicide",   29.0},
    {"Atrazine",    "Atrazine",            "Herbicide",   25.0},
    {"Harness",     "Acetochlor",          "Herbicide",   27.0},
    {"Karate",      "Lambda-cyhalothrin",  "Insecticide", 55.0},
    {"Decis",       "Deltamethrin",        "Insecticide", 50.0},
    {"Ridomil Gold","Metalaxyl",           "Fungicide",   45.0},
    {"Bravo",       "Chlorothalonil",      "Fungicide",   30.0},
    {"Tilt",        "Propiconazole",       "Fungicide",   35.0}
};
int num_pesticides = sizeof(pesticides) / sizeof(pesticides[0]);

void display_available_crops(void) {
    printf("\nAvailable crops:\n");
    printf("ID  Crop Name     N(kg/ha) P(kg/ha) K(kg/ha) Irrig(mm) Yield(t/ha)\n");
    printf("----------------------------------------------------------------\n");
    for (int i = 0; i < num_crops; i++) {
        printf("%-2d  %-12s %8.0f %8.0f %8.0f %9.0f %9.1f\n", 
               i + 1, crops[i].name, crops[i].n_rate, crops[i].p_rate, 
               crops[i].k_rate, crops[i].irrigation, crops[i].yield);
    }
    printf("\n");
}

void display_available_pesticides(void) {
    printf("\nAvailable pesticides:\n");
    printf("ID  Trade Name      Active Ingredient    Type         EF(kgCO2e/kg)\n");
    printf("--------------------------------------------------------------------\n");
    for (int i = 0; i < num_pesticides; i++) {
        printf("%-2d  %-15s %-20s %-12s %8.1f\n", 
               i + 1, pesticides[i].trade_name, pesticides[i].substance, 
               pesticides[i].type, pesticides[i].ef);
    }
    printf("0   No pesticide\n\n");
}

int find_crop_by_name(const char *name) {
    for (int i = 0; i < num_crops; i++) {
        if (strcasecmp(name, crops[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

int find_pesticide_by_name(const char *name) {
    for (int i = 0; i < num_pesticides; i++) {
        if (strcasecmp(name, pesticides[i].trade_name) == 0) {
            return i;
        }
    }
    return -1;
}

void convert_legacy_to_multi_crop(const LegacyFarmData *legacy, FarmData *multi) {
    multi->total_farm_size = legacy->farm_size;
    multi->num_crops = 1;
    multi->dairy_cows = legacy->dairy_cows;
    multi->pigs = legacy->pigs;
    multi->chickens = legacy->chickens;
    
    // Convert single crop to multi-crop format
    multi->crops[0].crop_id = find_crop_by_name(legacy->crop_type);
    if (multi->crops[0].crop_id == -1) multi->crops[0].crop_id = 0; // Default to wheat
    multi->crops[0].area = legacy->farm_size;
    multi->crops[0].nitrogen_kg_ha = legacy->nitrogen_kg_ha;
    multi->crops[0].phosphorus_kg_ha = legacy->phosphorus_kg_ha;
    multi->crops[0].potassium_kg_ha = legacy->potassium_kg_ha;
    multi->crops[0].manure_kg_ha = legacy->manure_kg_ha;
    multi->crops[0].diesel_l_ha = legacy->diesel_l_ha;
    multi->crops[0].irrigation_mm = legacy->irrigation_mm;
    multi->crops[0].pesticide_rate = 0.0;
    multi->crops[0].pesticide_id = -1;
}

int read_interactive_input(FarmData *farm) {
    printf("=== Farm Carbon Footprint Estimator - Multi-Crop Mode ===\n\n");
    printf("Note: All values must be non-negative (>= 0)\n\n");

    // Total farm size
    do {
        printf("Enter total farm size (hectares, > 0): ");
        if (scanf("%lf", &farm->total_farm_size) != 1) {
            printf("Error: Invalid input. Please enter a positive number.\n");
            while (getchar() != '\n');
            continue;
        }
        if (farm->total_farm_size <= 0) {
            printf("Error: Farm size must be greater than 0.\n");
        }
    } while (farm->total_farm_size <= 0);

    // Display available crops
    display_available_crops();

    // Get number of different crops
    do {
        printf("How many different crops do you grow? (1-10): ");
        if (scanf("%d", &farm->num_crops) != 1) {
            printf("Error: Invalid input. Please enter a number between 1 and 10.\n");
            while (getchar() != '\n');
            continue;
        }
        if (farm->num_crops < 1 || farm->num_crops > 10) {
            printf("Error: Number of crops must be between 1 and 10.\n");
        }
    } while (farm->num_crops < 1 || farm->num_crops > 10);

    double total_area_entered = 0.0;

    // Get data for each crop
    for (int i = 0; i < farm->num_crops; i++) {
        printf("\n--- Crop %d ---\n", i + 1);
        
        // Crop selection
        do {
            printf("Select crop ID (1-%d): ", num_crops);
            int crop_choice;
            if (scanf("%d", &crop_choice) != 1) {
                printf("Error: Invalid input. Please enter a number between 1 and %d.\n", num_crops);
                while (getchar() != '\n');
                continue;
            }
            if (crop_choice < 1 || crop_choice > num_crops) {
                printf("Error: Crop ID must be between 1 and %d.\n", num_crops);
                farm->crops[i].crop_id = -1;
            } else {
                farm->crops[i].crop_id = crop_choice - 1;
            }
        } while (farm->crops[i].crop_id < 0);

        printf("Selected: %s\n", crops[farm->crops[i].crop_id].name);

        // Area for this crop
        do {
            printf("Area for %s (hectares, >= 0): ", crops[farm->crops[i].crop_id].name);
            if (scanf("%lf", &farm->crops[i].area) != 1) {
                printf("Error: Invalid input. Please enter a non-negative number.\n");
                while (getchar() != '\n');
                continue;
            }
            if (farm->crops[i].area < 0) {
                printf("Error: Area cannot be negative.\n");
            } else if (total_area_entered + farm->crops[i].area > farm->total_farm_size) {
                printf("Error: Total crop area (%.1f ha) would exceed farm size (%.1f ha).\n", 
                       total_area_entered + farm->crops[i].area, farm->total_farm_size);
                farm->crops[i].area = -1;
            }
        } while (farm->crops[i].area < 0);

        if (farm->crops[i].area == 0) {
            printf("Skipping %s (zero area).\n", crops[farm->crops[i].crop_id].name);
            i--; // Skip this crop
            farm->num_crops--;
            continue;
        }

        total_area_entered += farm->crops[i].area;

        // Use default values or custom input
        char use_defaults;
        printf("Use default agronomic parameters for %s? (y/n): ", crops[farm->crops[i].crop_id].name);
        scanf(" %c", &use_defaults);

        if (use_defaults == 'y' || use_defaults == 'Y') {
            farm->crops[i].nitrogen_kg_ha = crops[farm->crops[i].crop_id].n_rate;
            farm->crops[i].phosphorus_kg_ha = crops[farm->crops[i].crop_id].p_rate;
            farm->crops[i].potassium_kg_ha = crops[farm->crops[i].crop_id].k_rate;
            farm->crops[i].irrigation_mm = crops[farm->crops[i].crop_id].irrigation;
            printf("Using defaults: N=%.0f, P=%.0f, K=%.0f, Irrigation=%.0fmm\n",
                   farm->crops[i].nitrogen_kg_ha, farm->crops[i].phosphorus_kg_ha,
                   farm->crops[i].potassium_kg_ha, farm->crops[i].irrigation_mm);
        } else {
            // Custom input for fertilizers
            do {
                printf("Nitrogen fertilizer (kg N/ha, >= 0): ");
                if (scanf("%lf", &farm->crops[i].nitrogen_kg_ha) != 1) {
                    printf("Error: Invalid input.\n");
                    while (getchar() != '\n');
                    continue;
                }
                if (farm->crops[i].nitrogen_kg_ha < 0) {
                    printf("Error: Cannot be negative.\n");
                }
            } while (farm->crops[i].nitrogen_kg_ha < 0);

            do {
                printf("Phosphorus fertilizer (kg P2O5/ha, >= 0): ");
                if (scanf("%lf", &farm->crops[i].phosphorus_kg_ha) != 1) {
                    printf("Error: Invalid input.\n");
                    while (getchar() != '\n');
                    continue;
                }
                if (farm->crops[i].phosphorus_kg_ha < 0) {
                    printf("Error: Cannot be negative.\n");
                }
            } while (farm->crops[i].phosphorus_kg_ha < 0);

            do {
                printf("Potassium fertilizer (kg K2O/ha, >= 0): ");
                if (scanf("%lf", &farm->crops[i].potassium_kg_ha) != 1) {
                    printf("Error: Invalid input.\n");
                    while (getchar() != '\n');
                    continue;
                }
                if (farm->crops[i].potassium_kg_ha < 0) {
                    printf("Error: Cannot be negative.\n");
                }
            } while (farm->crops[i].potassium_kg_ha < 0);

            do {
                printf("Irrigation water (mm, >= 0): ");
                if (scanf("%lf", &farm->crops[i].irrigation_mm) != 1) {
                    printf("Error: Invalid input.\n");
                    while (getchar() != '\n');
                    continue;
                }
                if (farm->crops[i].irrigation_mm < 0) {
                    printf("Error: Cannot be negative.\n");
                }
            } while (farm->crops[i].irrigation_mm < 0);
        }

        // Manure and diesel
        do {
            printf("Manure applied (kg/ha, >= 0): ");
            if (scanf("%lf", &farm->crops[i].manure_kg_ha) != 1) {
                printf("Error: Invalid input.\n");
                while (getchar() != '\n');
                continue;
            }
            if (farm->crops[i].manure_kg_ha < 0) {
                printf("Error: Cannot be negative.\n");
            }
        } while (farm->crops[i].manure_kg_ha < 0);

        do {
            printf("Diesel consumption (L/ha, >= 0): ");
            if (scanf("%lf", &farm->crops[i].diesel_l_ha) != 1) {
                printf("Error: Invalid input.\n");
                while (getchar() != '\n');
                continue;
            }
            if (farm->crops[i].diesel_l_ha < 0) {
                printf("Error: Cannot be negative.\n");
            }
        } while (farm->crops[i].diesel_l_ha < 0);

        // Pesticides
        display_available_pesticides();
        do {
            printf("Select pesticide ID (0 for none, 1-%d): ", num_pesticides);
            int pest_choice;
            if (scanf("%d", &pest_choice) != 1) {
                printf("Error: Invalid input.\n");
                while (getchar() != '\n');
                continue;
            }
            if (pest_choice < 0 || pest_choice > num_pesticides) {
                printf("Error: Invalid pesticide ID.\n");
                farm->crops[i].pesticide_id = -2; // Invalid marker
            } else {
                farm->crops[i].pesticide_id = pest_choice - 1; // -1 means no pesticide
            }
        } while (farm->crops[i].pesticide_id < -1);

        if (farm->crops[i].pesticide_id >= 0) {
            do {
                printf("Pesticide application rate (kg active ingredient/ha, >= 0): ");
                if (scanf("%lf", &farm->crops[i].pesticide_rate) != 1) {
                    printf("Error: Invalid input.\n");
                    while (getchar() != '\n');
                    continue;
                }
                if (farm->crops[i].pesticide_rate < 0) {
                    printf("Error: Cannot be negative.\n");
                }
            } while (farm->crops[i].pesticide_rate < 0);
        } else {
            farm->crops[i].pesticide_rate = 0.0;
        }
    }

    // Livestock
    do {
        printf("\nNumber of dairy cows (>= 0): ");
        if (scanf("%d", &farm->dairy_cows) != 1) {
            printf("Error: Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }
        if (farm->dairy_cows < 0) {
            printf("Error: Cannot be negative.\n");
        }
    } while (farm->dairy_cows < 0);

    do {
        printf("Number of pigs (>= 0): ");
        if (scanf("%d", &farm->pigs) != 1) {
            printf("Error: Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }
        if (farm->pigs < 0) {
            printf("Error: Cannot be negative.\n");
        }
    } while (farm->pigs < 0);

    do {
        printf("Number of chickens (>= 0): ");
        if (scanf("%d", &farm->chickens) != 1) {
            printf("Error: Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }
        if (farm->chickens < 0) {
            printf("Error: Cannot be negative.\n");
        }
    } while (farm->chickens < 0);

    return 1;
}

int read_legacy_interactive_input(LegacyFarmData *farm)
{
    printf("=== Farm Carbon Footprint Estimator ===\n\n");
    printf("Note: All values must be non-negative (>= 0)\n\n");

    // Farm size
    do
    {
        printf("Enter farm size (hectares, > 0): ");
        if (scanf("%lf", &farm->farm_size) != 1)
        {
            printf("Error: Invalid input. Please enter a positive number.\n");
            // Clear input buffer
            while (getchar() != '\n')
                ;
            continue;
        }
        if (farm->farm_size <= 0)
        {
            printf("Error: Farm size must be greater than 0.\n");
        }
    } while (farm->farm_size <= 0);

    // Crop type
    do
    {
        printf("Enter crop type (wheat/maize/sunflower/soybean/potato/rice/barley/rapeseed/sugar beet/vegetables): ");
        if (scanf("%49s", farm->crop_type) != 1)
        {
            printf("Error: Invalid crop type input.\n");
            return 0;
        }
        if (!is_valid_crop_type(farm->crop_type))
        {
            printf("Error: Invalid crop type. Valid options: wheat, maize, sunflower, soybean, potato, rice, barley, rapeseed, sugar beet, vegetables\n");
        }
    } while (!is_valid_crop_type(farm->crop_type));

    // Nitrogen fertilizer
    do
    {
        printf("Enter nitrogen fertilizer applied (kg N per hectare, >= 0): ");
        if (scanf("%lf", &farm->nitrogen_kg_ha) != 1)
        {
            printf("Error: Invalid input. Please enter a non-negative number.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        if (farm->nitrogen_kg_ha < 0)
        {
            printf("Error: Nitrogen fertilizer amount cannot be negative.\n");
        }
    } while (farm->nitrogen_kg_ha < 0);

    // Phosphorus fertilizer
    do
    {
        printf("Enter phosphorus fertilizer applied (kg P2O5 per hectare, >= 0): ");
        if (scanf("%lf", &farm->phosphorus_kg_ha) != 1)
        {
            printf("Error: Invalid input. Please enter a non-negative number.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        if (farm->phosphorus_kg_ha < 0)
        {
            printf("Error: Phosphorus fertilizer amount cannot be negative.\n");
        }
    } while (farm->phosphorus_kg_ha < 0);

    // Potassium fertilizer
    do
    {
        printf("Enter potassium fertilizer applied (kg K2O per hectare, >= 0): ");
        if (scanf("%lf", &farm->potassium_kg_ha) != 1)
        {
            printf("Error: Invalid input. Please enter a non-negative number.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        if (farm->potassium_kg_ha < 0)
        {
            printf("Error: Potassium fertilizer amount cannot be negative.\n");
        }
    } while (farm->potassium_kg_ha < 0);

    // Manure
    do
    {
        printf("Enter manure applied (kg per hectare, >= 0): ");
        if (scanf("%lf", &farm->manure_kg_ha) != 1)
        {
            printf("Error: Invalid input. Please enter a non-negative number.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        if (farm->manure_kg_ha < 0)
        {
            printf("Error: Manure amount cannot be negative.\n");
        }
    } while (farm->manure_kg_ha < 0);

    // Diesel consumption
    do
    {
        printf("Enter diesel consumption (liters per hectare, >= 0): ");
        if (scanf("%lf", &farm->diesel_l_ha) != 1)
        {
            printf("Error: Invalid input. Please enter a non-negative number.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        if (farm->diesel_l_ha < 0)
        {
            printf("Error: Diesel consumption cannot be negative.\n");
        }
    } while (farm->diesel_l_ha < 0);

    // Irrigation water
    do
    {
        printf("Enter irrigation water used (mm, >= 0): ");
        if (scanf("%lf", &farm->irrigation_mm) != 1)
        {
            printf("Error: Invalid input. Please enter a non-negative number.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        if (farm->irrigation_mm < 0)
        {
            printf("Error: Irrigation water amount cannot be negative.\n");
        }
    } while (farm->irrigation_mm < 0);

    // Dairy cows
    do
    {
        printf("Enter number of dairy cows (>= 0): ");
        if (scanf("%d", &farm->dairy_cows) != 1)
        {
            printf("Error: Invalid input. Please enter a non-negative integer.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        if (farm->dairy_cows < 0)
        {
            printf("Error: Number of dairy cows cannot be negative.\n");
        }
    } while (farm->dairy_cows < 0);

    // Pigs
    do
    {
        printf("Enter number of pigs (>= 0): ");
        if (scanf("%d", &farm->pigs) != 1)
        {
            printf("Error: Invalid input. Please enter a non-negative integer.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        if (farm->pigs < 0)
        {
            printf("Error: Number of pigs cannot be negative.\n");
        }
    } while (farm->pigs < 0);

    // Chickens
    do
    {
        printf("Enter number of chickens (>= 0): ");
        if (scanf("%d", &farm->chickens) != 1)
        {
            printf("Error: Invalid input. Please enter a non-negative integer.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        if (farm->chickens < 0)
        {
            printf("Error: Number of chickens cannot be negative.\n");
        }
    } while (farm->chickens < 0);

    return 1;
}

int read_csv_input(const char *filename, LegacyFarmData *farm)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error: Cannot open file \"%s\"\n", filename);
        printf("Failed to read input data from file: \"%s\"\n", filename);
        return 0;
    }

    char line[1024];
    int line_count = 0;

    // Skip header line
    if (fgets(line, sizeof(line), file))
    {
        line_count++;
    }

    // Read data line
    if (fgets(line, sizeof(line), file))
    {
        line_count++;

        char *token = strtok(line, ",");
        if (!token || sscanf(token, "%lf", &farm->farm_size) != 1)
        {
            printf("Error: Invalid farm size in CSV line %d\n", line_count);
            fclose(file);
            return 0;
        }

        token = strtok(NULL, ",");
        if (!token)
        {
            printf("Error: Missing crop type in CSV line %d\n", line_count);
            fclose(file);
            return 0;
        }
        strncpy(farm->crop_type, token, sizeof(farm->crop_type) - 1);
        farm->crop_type[sizeof(farm->crop_type) - 1] = '\0';

        // Remove any trailing whitespace/newlines from crop type
        char *end = farm->crop_type + strlen(farm->crop_type) - 1;
        while (end > farm->crop_type && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r'))
        {
            *end = '\0';
            end--;
        }

        token = strtok(NULL, ",");
        if (!token || sscanf(token, "%lf", &farm->nitrogen_kg_ha) != 1)
        {
            printf("Error: Invalid nitrogen in CSV line %d\n", line_count);
            fclose(file);
            return 0;
        }

        token = strtok(NULL, ",");
        if (!token || sscanf(token, "%lf", &farm->phosphorus_kg_ha) != 1)
        {
            printf("Error: Invalid phosphorus in CSV line %d\n", line_count);
            fclose(file);
            return 0;
        }

        token = strtok(NULL, ",");
        if (!token || sscanf(token, "%lf", &farm->potassium_kg_ha) != 1)
        {
            printf("Error: Invalid potassium in CSV line %d\n", line_count);
            fclose(file);
            return 0;
        }

        token = strtok(NULL, ",");
        if (!token || sscanf(token, "%lf", &farm->manure_kg_ha) != 1)
        {
            printf("Error: Invalid manure in CSV line %d\n", line_count);
            fclose(file);
            return 0;
        }

        token = strtok(NULL, ",");
        if (!token || sscanf(token, "%lf", &farm->diesel_l_ha) != 1)
        {
            printf("Error: Invalid diesel in CSV line %d\n", line_count);
            fclose(file);
            return 0;
        }

        token = strtok(NULL, ",");
        if (!token || sscanf(token, "%lf", &farm->irrigation_mm) != 1)
        {
            printf("Error: Invalid irrigation in CSV line %d\n", line_count);
            fclose(file);
            return 0;
        }

        token = strtok(NULL, ",");
        if (!token || sscanf(token, "%d", &farm->dairy_cows) != 1)
        {
            printf("Error: Invalid dairy cows in CSV line %d\n", line_count);
            fclose(file);
            return 0;
        }

        token = strtok(NULL, ",");
        if (!token || sscanf(token, "%d", &farm->pigs) != 1)
        {
            printf("Error: Invalid pigs in CSV line %d\n", line_count);
            fclose(file);
            return 0;
        }

        token = strtok(NULL, ",");
        if (!token || sscanf(token, "%d", &farm->chickens) != 1)
        {
            printf("Error: Invalid chickens in CSV line %d\n", line_count);
            fclose(file);
            return 0;
        }
    }
    else
    {
        printf("Error: No data found in CSV file\n");
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}

int is_valid_crop_type(const char *crop_type)
{
    // Check against the predefined crops array
    for (int i = 0; i < num_crops; i++)
    {
        if (strcasecmp(crop_type, crops[i].name) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int validate_input(const FarmData *farm) {
    // Total farm size must be positive
    if (farm->total_farm_size <= 0 || farm->total_farm_size > 100000) {
        printf("Error: Total farm size must be greater than 0 and less than 100,000 hectares\n");
        return 0;
    }

    // Must have at least one crop
    if (farm->num_crops <= 0 || farm->num_crops > 10) {
        printf("Error: Number of crops must be between 1 and 10\n");
        return 0;
    }

    double total_crop_area = 0.0;
    for (int i = 0; i < farm->num_crops; i++) {
        // Validate crop ID
        if (farm->crops[i].crop_id < 0 || farm->crops[i].crop_id >= num_crops) {
            printf("Error: Invalid crop ID %d for crop %d\n", farm->crops[i].crop_id, i + 1);
            return 0;
        }

        // Validate area
        if (farm->crops[i].area < 0 || farm->crops[i].area > 50000) {
            printf("Error: Crop %d area must be between 0 and 50,000 hectares\n", i + 1);
            return 0;
        }
        total_crop_area += farm->crops[i].area;

        // Validate fertilizer values
        if (farm->crops[i].nitrogen_kg_ha < 0 || farm->crops[i].nitrogen_kg_ha > 1000) {
            printf("Error: Crop %d nitrogen must be between 0 and 1,000 kg/ha\n", i + 1);
            return 0;
        }
        if (farm->crops[i].phosphorus_kg_ha < 0 || farm->crops[i].phosphorus_kg_ha > 500) {
            printf("Error: Crop %d phosphorus must be between 0 and 500 kg/ha\n", i + 1);
            return 0;
        }
        if (farm->crops[i].potassium_kg_ha < 0 || farm->crops[i].potassium_kg_ha > 500) {
            printf("Error: Crop %d potassium must be between 0 and 500 kg/ha\n", i + 1);
            return 0;
        }

        // Validate other inputs
        if (farm->crops[i].manure_kg_ha < 0 || farm->crops[i].manure_kg_ha > 50000) {
            printf("Error: Crop %d manure must be between 0 and 50,000 kg/ha\n", i + 1);
            return 0;
        }
        if (farm->crops[i].diesel_l_ha < 0 || farm->crops[i].diesel_l_ha > 1000) {
            printf("Error: Crop %d diesel must be between 0 and 1,000 L/ha\n", i + 1);
            return 0;
        }
        if (farm->crops[i].irrigation_mm < 0 || farm->crops[i].irrigation_mm > 2000) {
            printf("Error: Crop %d irrigation must be between 0 and 2,000 mm\n", i + 1);
            return 0;
        }

        // Validate pesticide
        if (farm->crops[i].pesticide_id >= num_pesticides) {
            printf("Error: Invalid pesticide ID %d for crop %d\n", farm->crops[i].pesticide_id, i + 1);
            return 0;
        }
        if (farm->crops[i].pesticide_rate < 0 || farm->crops[i].pesticide_rate > 100) {
            printf("Error: Crop %d pesticide rate must be between 0 and 100 kg/ha\n", i + 1);
            return 0;
        }
    }

    // Check total crop area doesn't exceed farm size
    if (total_crop_area > farm->total_farm_size * 1.01) { // Allow 1% tolerance
        printf("Error: Total crop area (%.1f ha) exceeds farm size (%.1f ha)\n", 
               total_crop_area, farm->total_farm_size);
        return 0;
    }

    // Validate livestock
    if (farm->dairy_cows < 0 || farm->dairy_cows > 10000) {
        printf("Error: Number of dairy cows must be between 0 and 10,000\n");
        return 0;
    }
    if (farm->pigs < 0 || farm->pigs > 50000) {
        printf("Error: Number of pigs must be between 0 and 50,000\n");
        return 0;
    }
    if (farm->chickens < 0 || farm->chickens > 1000000) {
        printf("Error: Number of chickens must be between 0 and 1,000,000\n");
        return 0;
    }

    return 1;
}

int validate_legacy_input(const LegacyFarmData *farm)
{
    // Farm size must be positive
    if (farm->farm_size <= 0 || farm->farm_size > 100000)
    {
        printf("Error: Farm size must be greater than 0 and less than 100,000 hectares\n");
        return 0;
    }

    // Validate crop type
    if (!is_valid_crop_type(farm->crop_type))
    {
        printf("Error: Invalid crop type '%s'. Valid options: wheat, maize, sunflower, soybean, potato, vegetables\n", farm->crop_type);
        return 0;
    }

    // All fertilizer values must be non-negative
    if (farm->nitrogen_kg_ha < 0 || farm->nitrogen_kg_ha > 1000)
    {
        printf("Error: Nitrogen fertilizer must be between 0 and 1,000 kg/ha\n");
        return 0;
    }

    if (farm->phosphorus_kg_ha < 0 || farm->phosphorus_kg_ha > 500)
    {
        printf("Error: Phosphorus fertilizer must be between 0 and 500 kg/ha\n");
        return 0;
    }

    if (farm->potassium_kg_ha < 0 || farm->potassium_kg_ha > 500)
    {
        printf("Error: Potassium fertilizer must be between 0 and 500 kg/ha\n");
        return 0;
    }

    // Manure must be non-negative
    if (farm->manure_kg_ha < 0 || farm->manure_kg_ha > 50000)
    {
        printf("Error: Manure application must be between 0 and 50,000 kg/ha\n");
        return 0;
    }

    // Diesel consumption must be non-negative
    if (farm->diesel_l_ha < 0 || farm->diesel_l_ha > 1000)
    {
        printf("Error: Diesel consumption must be between 0 and 1,000 L/ha\n");
        return 0;
    }

    // Irrigation must be non-negative
    if (farm->irrigation_mm < 0 || farm->irrigation_mm > 2000)
    {
        printf("Error: Irrigation water must be between 0 and 2,000 mm\n");
        return 0;
    }

    // Livestock numbers must be non-negative
    if (farm->dairy_cows < 0 || farm->dairy_cows > 10000)
    {
        printf("Error: Number of dairy cows must be between 0 and 10,000\n");
        return 0;
    }

    if (farm->pigs < 0 || farm->pigs > 50000)
    {
        printf("Error: Number of pigs must be between 0 and 50,000\n");
        return 0;
    }

    if (farm->chickens < 0 || farm->chickens > 1000000)
    {
        printf("Error: Number of chickens must be between 0 and 1,000,000\n");
        return 0;
    }

    return 1;
}
#ifndef INPUT_H
#define INPUT_H

// Crop structure with default agronomic parameters
typedef struct {
    char name[30];
    double n_rate;       // Nitrogen (kg N/ha)
    double p_rate;       // Phosphorus (kg P2O5/ha)
    double k_rate;       // Potassium (kg K2O/ha)
    double irrigation;   // Irrigation (mm)
    double yield;        // Average yield (t/ha)
} Crop;

// Pesticide structure with emission factors
typedef struct {
    char trade_name[50];
    char substance[50];
    char type[20];
    double ef;   // Emission factor (kg CO2e per kg a.i.)
} Pesticide;

// Individual crop data for multi-crop farms
typedef struct {
    int crop_id;                // Index in crops array
    double area;                // hectares for this crop
    double nitrogen_kg_ha;      // kg N per hectare (can override default)
    double phosphorus_kg_ha;    // kg P2O5 per hectare
    double potassium_kg_ha;     // kg K2O per hectare
    double manure_kg_ha;        // kg manure per hectare
    double diesel_l_ha;         // liters diesel per hectare
    double irrigation_mm;       // mm water (1 mm = 10 m³/ha)
    double pesticide_rate;      // kg active ingredient per hectare
    int pesticide_id;           // Index in pesticides array (-1 if none)
} CropData;

typedef struct {
    double total_farm_size;     // total hectares
    int num_crops;              // number of different crops
    CropData crops[10];         // up to 10 different crops
    int dairy_cows;             // number of dairy cows
    int pigs;                   // number of pigs
    int chickens;               // number of chickens
} FarmData;

// Legacy single-crop structure for backward compatibility
typedef struct {
    double farm_size;           // hectares
    char crop_type[50];         // crop name
    double nitrogen_kg_ha;      // kg N per hectare
    double phosphorus_kg_ha;    // kg P2O5 per hectare
    double potassium_kg_ha;     // kg K2O per hectare
    double manure_kg_ha;        // kg manure per hectare
    double diesel_l_ha;         // liters diesel per hectare
    double irrigation_mm;       // mm water (1 mm = 10 m³/ha)
    int dairy_cows;             // number of dairy cows
    int pigs;                   // number of pigs
    int chickens;               // number of chickens
} LegacyFarmData;

// Global crop and pesticide data
extern Crop crops[];
extern int num_crops;
extern Pesticide pesticides[];
extern int num_pesticides;

// Function declarations
int read_interactive_input(FarmData *farm);
int read_legacy_interactive_input(LegacyFarmData *farm);
int read_csv_input(const char *filename, LegacyFarmData *farm);
int validate_input(const FarmData *farm);
int validate_legacy_input(const LegacyFarmData *farm);
int is_valid_crop_type(const char *crop_type);
void display_available_crops(void);
void display_available_pesticides(void);
int find_crop_by_name(const char *name);
int find_pesticide_by_name(const char *name);
void convert_legacy_to_multi_crop(const LegacyFarmData *legacy, FarmData *multi);

#endif
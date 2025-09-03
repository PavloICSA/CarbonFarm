#ifndef COMPUTE_H
#define COMPUTE_H

#include "input.h"

// Emission factors (kg CO2e)
#define NITROGEN_FACTOR 6.3     // kg CO2e per kg N
#define PHOSPHORUS_FACTOR 1.5   // kg CO2e per kg P2O5
#define POTASSIUM_FACTOR 1.0    // kg CO2e per kg K2O
#define MANURE_FACTOR 0.6       // kg CO2e per kg manure
#define DIESEL_FACTOR 2.68      // kg CO2e per liter
#define IRRIGATION_FACTOR 0.5   // kg CO2e per m³ (5.0 kg CO2e per mm)
#define COW_FACTOR 1000.0       // kg CO2e per cow per year
#define PIG_FACTOR 200.0        // kg CO2e per pig per year
#define CHICKEN_FACTOR 5.0      // kg CO2e per chicken per year

// Per-crop emission results
typedef struct {
    int crop_id;
    double area;
    double fertilizer_emissions;   // tonnes CO2e
    double manure_emissions;       // tonnes CO2e
    double fuel_emissions;         // tonnes CO2e
    double irrigation_emissions;   // tonnes CO2e
    double pesticide_emissions;    // tonnes CO2e
    double livestock_emissions;    // tonnes CO2e (allocated proportionally)
    double total_emissions;        // tonnes CO2e
} CropEmissionResults;

typedef struct {
    double fertilizer_emissions;   // tonnes CO2e
    double manure_emissions;       // tonnes CO2e
    double fuel_emissions;         // tonnes CO2e
    double irrigation_emissions;   // tonnes CO2e
    double pesticide_emissions;    // tonnes CO2e
    double livestock_emissions;    // tonnes CO2e
    double total_emissions;        // tonnes CO2e
    double per_hectare_emissions;  // tonnes CO2e per hectare
    int num_crops;
    CropEmissionResults crop_results[10];  // per-crop breakdown
} EmissionResults;

// Function declarations
EmissionResults calculate_emissions(const FarmData *farm);
EmissionResults calculate_legacy_emissions(const LegacyFarmData *farm);

#endif
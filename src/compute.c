#include <stdio.h>
#include <math.h>
#include "compute.h"

EmissionResults calculate_emissions(const FarmData *farm) {
    EmissionResults results = {0};
    
    // Calculate livestock emissions first (to be allocated proportionally)
    double cow_emissions = farm->dairy_cows * COW_FACTOR / 1000.0;
    double pig_emissions = farm->pigs * PIG_FACTOR / 1000.0;
    double chicken_emissions = farm->chickens * CHICKEN_FACTOR / 1000.0;
    results.livestock_emissions = cow_emissions + pig_emissions + chicken_emissions;
    
    // Calculate per-crop emissions
    results.num_crops = farm->num_crops;
    double total_crop_area = 0.0;
    
    // First pass: calculate total crop area
    for (int i = 0; i < farm->num_crops; i++) {
        total_crop_area += farm->crops[i].area;
    }
    
    // Second pass: calculate emissions per crop
    for (int i = 0; i < farm->num_crops; i++) {
        CropEmissionResults *crop_result = &results.crop_results[i];
        const CropData *crop = &farm->crops[i];
        
        crop_result->crop_id = crop->crop_id;
        crop_result->area = crop->area;
        
        // Fertilizer emissions (convert kg to tonnes)
        double nitrogen_emissions = crop->nitrogen_kg_ha * crop->area * NITROGEN_FACTOR / 1000.0;
        double phosphorus_emissions = crop->phosphorus_kg_ha * crop->area * PHOSPHORUS_FACTOR / 1000.0;
        double potassium_emissions = crop->potassium_kg_ha * crop->area * POTASSIUM_FACTOR / 1000.0;
        crop_result->fertilizer_emissions = nitrogen_emissions + phosphorus_emissions + potassium_emissions;
        
        // Manure emissions (convert kg to tonnes)
        crop_result->manure_emissions = crop->manure_kg_ha * crop->area * MANURE_FACTOR / 1000.0;
        
        // Fuel emissions (convert kg to tonnes)
        crop_result->fuel_emissions = crop->diesel_l_ha * crop->area * DIESEL_FACTOR / 1000.0;
        
        // Irrigation emissions (convert mm to m³/ha, then kg to tonnes)
        // 1 mm = 10 m³/ha
        double irrigation_m3_ha = crop->irrigation_mm * 10.0;
        crop_result->irrigation_emissions = irrigation_m3_ha * crop->area * IRRIGATION_FACTOR / 1000.0;
        
        // Pesticide emissions (convert kg to tonnes)
        if (crop->pesticide_id >= 0 && crop->pesticide_rate > 0) {
            crop_result->pesticide_emissions = crop->pesticide_rate * crop->area * 
                                             pesticides[crop->pesticide_id].ef / 1000.0;
        } else {
            crop_result->pesticide_emissions = 0.0;
        }
        
        // Allocate livestock emissions proportionally by area
        if (total_crop_area > 0) {
            crop_result->livestock_emissions = results.livestock_emissions * (crop->area / total_crop_area);
        } else {
            crop_result->livestock_emissions = 0.0;
        }
        
        // Calculate crop total
        crop_result->total_emissions = crop_result->fertilizer_emissions + 
                                     crop_result->manure_emissions + 
                                     crop_result->fuel_emissions + 
                                     crop_result->irrigation_emissions + 
                                     crop_result->pesticide_emissions + 
                                     crop_result->livestock_emissions;
        
        // Add to farm totals
        results.fertilizer_emissions += crop_result->fertilizer_emissions;
        results.manure_emissions += crop_result->manure_emissions;
        results.fuel_emissions += crop_result->fuel_emissions;
        results.irrigation_emissions += crop_result->irrigation_emissions;
        results.pesticide_emissions += crop_result->pesticide_emissions;
    }
    
    // Calculate farm totals
    results.total_emissions = results.fertilizer_emissions + 
                             results.manure_emissions + 
                             results.fuel_emissions + 
                             results.irrigation_emissions + 
                             results.pesticide_emissions + 
                             results.livestock_emissions;
    
    if (farm->total_farm_size > 0) {
        results.per_hectare_emissions = results.total_emissions / farm->total_farm_size;
    } else {
        results.per_hectare_emissions = 0.0;
    }
    
    return results;
}

EmissionResults calculate_legacy_emissions(const LegacyFarmData *farm) {
    EmissionResults results = {0};
    
    // Calculate fertilizer emissions (convert kg to tonnes)
    double nitrogen_emissions = farm->nitrogen_kg_ha * farm->farm_size * NITROGEN_FACTOR / 1000.0;
    double phosphorus_emissions = farm->phosphorus_kg_ha * farm->farm_size * PHOSPHORUS_FACTOR / 1000.0;
    double potassium_emissions = farm->potassium_kg_ha * farm->farm_size * POTASSIUM_FACTOR / 1000.0;
    
    results.fertilizer_emissions = nitrogen_emissions + phosphorus_emissions + potassium_emissions;
    
    // Calculate manure emissions (convert kg to tonnes)
    results.manure_emissions = farm->manure_kg_ha * farm->farm_size * MANURE_FACTOR / 1000.0;
    
    // Calculate fuel emissions (convert kg to tonnes)
    results.fuel_emissions = farm->diesel_l_ha * farm->farm_size * DIESEL_FACTOR / 1000.0;
    
    // Calculate irrigation emissions (convert mm to m³/ha, then kg to tonnes)
    // 1 mm = 10 m³/ha
    double irrigation_m3_ha = farm->irrigation_mm * 10.0;
    results.irrigation_emissions = irrigation_m3_ha * farm->farm_size * IRRIGATION_FACTOR / 1000.0;
    
    // No pesticide emissions in legacy mode
    results.pesticide_emissions = 0.0;
    
    // Calculate livestock emissions (convert kg to tonnes)
    double cow_emissions = farm->dairy_cows * COW_FACTOR / 1000.0;
    double pig_emissions = farm->pigs * PIG_FACTOR / 1000.0;
    double chicken_emissions = farm->chickens * CHICKEN_FACTOR / 1000.0;
    
    results.livestock_emissions = cow_emissions + pig_emissions + chicken_emissions;
    
    // Calculate totals
    results.total_emissions = results.fertilizer_emissions + 
                             results.manure_emissions + 
                             results.fuel_emissions + 
                             results.irrigation_emissions + 
                             results.pesticide_emissions + 
                             results.livestock_emissions;
    
    results.per_hectare_emissions = results.total_emissions / farm->farm_size;
    results.num_crops = 0; // Legacy mode doesn't track individual crops
    
    return results;
}
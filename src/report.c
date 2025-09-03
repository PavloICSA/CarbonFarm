#include <stdio.h>
#include <string.h>
#include <locale.h>
#include "report.h"

// Set this to 1 to use UTF-8 symbols, 0 for ASCII
// Windows users: run 'chcp 65001' before execution for UTF-8 support
#define USE_UTF8_SYMBOLS 0

void print_report(const FarmData *farm, const EmissionResults *results) {
    // Set locale for UTF-8 support (Windows users should run 'chcp 65001' first)
    setlocale(LC_ALL, "");
    
    const char* co2_unit = USE_UTF8_SYMBOLS ? "tCO₂e" : "tCO2e";
    
    printf("\n");
    printf("===================================================================\n");
    printf("                   Farm Carbon Footprint Report\n");
    printf("===================================================================\n");
    
    if (results->num_crops > 0) {
        // Multi-crop report with table format
        printf("Crop          Area(ha)   Fert(%s)   Fuel   Irrig   Pestic   Live.   Total\n", co2_unit);
        printf("-------------------------------------------------------------------\n");
        
        for (int i = 0; i < results->num_crops; i++) {
            const CropEmissionResults *crop_result = &results->crop_results[i];
            printf("%-12s %8.1f %10.2f %8.2f %7.2f %8.2f %7.2f %7.2f\n",
                   crops[crop_result->crop_id].name,
                   crop_result->area,
                   crop_result->fertilizer_emissions,
                   crop_result->fuel_emissions,
                   crop_result->irrigation_emissions,
                   crop_result->pesticide_emissions,
                   crop_result->livestock_emissions,
                   crop_result->total_emissions);
        }
        
        printf("-------------------------------------------------------------------\n");
        printf("FARM TOTAL   %8.1f %10.2f %8.2f %7.2f %8.2f %7.2f %7.2f\n",
               farm->total_farm_size,
               results->fertilizer_emissions,
               results->fuel_emissions,
               results->irrigation_emissions,
               results->pesticide_emissions,
               results->livestock_emissions,
               results->total_emissions);
        
        printf("Per Hectare            %10.2f %8.2f %7.2f %8.2f %7.2f %7.2f\n",
               results->fertilizer_emissions / farm->total_farm_size,
               results->fuel_emissions / farm->total_farm_size,
               results->irrigation_emissions / farm->total_farm_size,
               results->pesticide_emissions / farm->total_farm_size,
               results->livestock_emissions / farm->total_farm_size,
               results->per_hectare_emissions);
    } else {
        // Legacy single-crop report
        printf("Farm Size: %.1f ha\n", farm->total_farm_size);
        printf("----------------------------------------\n");
        printf("EMISSION BREAKDOWN:\n");
        printf("Fertilizer Emissions: %.2f %s\n", results->fertilizer_emissions, co2_unit);
        printf("Manure Emissions: %.2f %s\n", results->manure_emissions, co2_unit);
        printf("Fuel Emissions: %.2f %s\n", results->fuel_emissions, co2_unit);
        printf("Irrigation Emissions: %.2f %s\n", results->irrigation_emissions, co2_unit);
        printf("Pesticide Emissions: %.2f %s\n", results->pesticide_emissions, co2_unit);
        printf("Livestock Emissions: %.2f %s\n", results->livestock_emissions, co2_unit);
        printf("----------------------------------------\n");
        printf("TOTAL EMISSIONS: %.2f %s\n", results->total_emissions, co2_unit);
        printf("Per Hectare: %.2f %s/ha\n", results->per_hectare_emissions, co2_unit);
    }
    
    printf("===================================================================\n");
    printf("Units: all emissions in %s (tons of CO2 equivalent).\n", co2_unit);
    printf("===================================================================\n");
    
    print_recommendations(results);
}

void print_legacy_report(const LegacyFarmData *farm, const EmissionResults *results) {
    // Set locale for UTF-8 support (Windows users should run 'chcp 65001' first)
    setlocale(LC_ALL, "");
    
    const char* co2_unit = USE_UTF8_SYMBOLS ? "tCO₂e" : "tCO2e";
    
    printf("\n");
    printf("========================================\n");
    printf("    Farm Carbon Footprint Report\n");
    printf("========================================\n");
    printf("Farm Size: %.1f ha\n", farm->farm_size);
    printf("Crop Type: %s\n", farm->crop_type);
    printf("----------------------------------------\n");
    printf("EMISSION BREAKDOWN:\n");
    printf("Fertilizer Emissions: %.2f %s\n", results->fertilizer_emissions, co2_unit);
    printf("Manure Emissions: %.2f %s\n", results->manure_emissions, co2_unit);
    printf("Fuel Emissions: %.2f %s\n", results->fuel_emissions, co2_unit);
    printf("Irrigation Emissions: %.2f %s\n", results->irrigation_emissions, co2_unit);
    printf("Livestock Emissions: %.2f %s\n", results->livestock_emissions, co2_unit);
    printf("----------------------------------------\n");
    printf("TOTAL EMISSIONS: %.2f %s\n", results->total_emissions, co2_unit);
    printf("Per Hectare: %.2f %s/ha\n", results->per_hectare_emissions, co2_unit);
    printf("========================================\n");
    
    print_recommendations(results);
}

void save_report_to_file(const FarmData *farm, const EmissionResults *results, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Warning: Could not save report to %s\n", filename);
        return;
    }
    
    const char* co2_unit = USE_UTF8_SYMBOLS ? "tCO₂e" : "tCO2e";
    
    fprintf(file, "Farm Carbon Footprint Report\n");
    fprintf(file, "===================================================================\n");
    
    if (results->num_crops > 0) {
        // Multi-crop report
        fprintf(file, "Crop          Area(ha)   Fert(%s)   Fuel   Irrig   Pestic   Live.   Total\n", co2_unit);
        fprintf(file, "-------------------------------------------------------------------\n");
        
        for (int i = 0; i < results->num_crops; i++) {
            const CropEmissionResults *crop_result = &results->crop_results[i];
            fprintf(file, "%-12s %8.1f %10.2f %8.2f %7.2f %8.2f %7.2f %7.2f\n",
                   crops[crop_result->crop_id].name,
                   crop_result->area,
                   crop_result->fertilizer_emissions,
                   crop_result->fuel_emissions,
                   crop_result->irrigation_emissions,
                   crop_result->pesticide_emissions,
                   crop_result->livestock_emissions,
                   crop_result->total_emissions);
        }
        
        fprintf(file, "-------------------------------------------------------------------\n");
        fprintf(file, "FARM TOTAL   %8.1f %10.2f %8.2f %7.2f %8.2f %7.2f %7.2f\n",
               farm->total_farm_size,
               results->fertilizer_emissions,
               results->fuel_emissions,
               results->irrigation_emissions,
               results->pesticide_emissions,
               results->livestock_emissions,
               results->total_emissions);
        
        fprintf(file, "Per Hectare            %10.2f %8.2f %7.2f %8.2f %7.2f %7.2f\n",
               results->fertilizer_emissions / farm->total_farm_size,
               results->fuel_emissions / farm->total_farm_size,
               results->irrigation_emissions / farm->total_farm_size,
               results->pesticide_emissions / farm->total_farm_size,
               results->livestock_emissions / farm->total_farm_size,
               results->per_hectare_emissions);
    } else {
        // Legacy format
        fprintf(file, "Farm Size: %.1f ha\n", farm->total_farm_size);
        fprintf(file, "\nEmission Breakdown:\n");
        fprintf(file, "Fertilizer Emissions: %.2f %s\n", results->fertilizer_emissions, co2_unit);
        fprintf(file, "Manure Emissions: %.2f %s\n", results->manure_emissions, co2_unit);
        fprintf(file, "Fuel Emissions: %.2f %s\n", results->fuel_emissions, co2_unit);
        fprintf(file, "Irrigation Emissions: %.2f %s\n", results->irrigation_emissions, co2_unit);
        fprintf(file, "Pesticide Emissions: %.2f %s\n", results->pesticide_emissions, co2_unit);
        fprintf(file, "Livestock Emissions: %.2f %s\n", results->livestock_emissions, co2_unit);
        fprintf(file, "\nTotal Emissions: %.2f %s\n", results->total_emissions, co2_unit);
        fprintf(file, "Per Hectare: %.2f %s/ha\n", results->per_hectare_emissions, co2_unit);
    }
    
    fprintf(file, "===================================================================\n");
    fprintf(file, "Units: all emissions in %s (tons of CO2 equivalent).\n", co2_unit);
    
    fclose(file);
    printf("Report saved to %s\n", filename);
}

void save_legacy_report_to_file(const LegacyFarmData *farm, const EmissionResults *results, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Warning: Could not save report to %s\n", filename);
        return;
    }
    
    const char* co2_unit = USE_UTF8_SYMBOLS ? "tCO₂e" : "tCO2e";
    
    fprintf(file, "Farm Carbon Footprint Report\n");
    fprintf(file, "============================\n");
    fprintf(file, "Farm Size: %.1f ha\n", farm->farm_size);
    fprintf(file, "Crop Type: %s\n", farm->crop_type);
    fprintf(file, "\nEmission Breakdown:\n");
    fprintf(file, "Fertilizer Emissions: %.2f %s\n", results->fertilizer_emissions, co2_unit);
    fprintf(file, "Manure Emissions: %.2f %s\n", results->manure_emissions, co2_unit);
    fprintf(file, "Fuel Emissions: %.2f %s\n", results->fuel_emissions, co2_unit);
    fprintf(file, "Irrigation Emissions: %.2f %s\n", results->irrigation_emissions, co2_unit);
    fprintf(file, "Livestock Emissions: %.2f %s\n", results->livestock_emissions, co2_unit);
    fprintf(file, "\nTotal Emissions: %.2f %s\n", results->total_emissions, co2_unit);
    fprintf(file, "Per Hectare: %.2f %s/ha\n", results->per_hectare_emissions, co2_unit);
    
    fclose(file);
    printf("Report saved to %s\n", filename);
}

void print_recommendations(const EmissionResults *results) {
    const char* plant_symbol = USE_UTF8_SYMBOLS ? "🌱" : "*";
    const char* bullet = USE_UTF8_SYMBOLS ? "•" : "-";
    const char* arrow = USE_UTF8_SYMBOLS ? "→" : ">";
    const char* co2_unit = USE_UTF8_SYMBOLS ? "tCO₂e" : "tCO2e";
    
    printf("\n%s RECOMMENDATIONS:\n", plant_symbol);
    printf("===================================================================\n");
    
    // Check fertilizer emissions (typically highest contributor)
    if (results->fertilizer_emissions > results->total_emissions * 0.4) {
        printf("%s HIGH FERTILIZER EMISSIONS detected!\n", bullet);
        printf("  %s Consider reducing nitrogen fertilizer use through soil testing\n", arrow);
        printf("  %s Plant nitrogen-fixing legumes (beans, peas, clover)\n", arrow);
        printf("  %s Use precision agriculture techniques and variable rate application\n", arrow);
        printf("  %s Apply fertilizer in split doses to reduce losses\n", arrow);
        printf("  %s Consider slow-release or organic fertilizers\n\n", arrow);
    }
    
    // Check fuel emissions
    if (results->fuel_emissions > results->total_emissions * 0.3) {
        printf("%s HIGH FUEL EMISSIONS detected!\n", bullet);
        printf("  %s Optimize machinery efficiency and maintenance schedules\n", arrow);
        printf("  %s Reduce tillage operations (no-till or minimum tillage)\n", arrow);
        printf("  %s Consider electric or hybrid equipment for smaller operations\n", arrow);
        printf("  %s Plan field operations to minimize trips and optimize routes\n", arrow);
        printf("  %s Use GPS guidance systems to reduce overlap\n\n", arrow);
    }
    
    // Check pesticide emissions
    if (results->pesticide_emissions > results->total_emissions * 0.1) {
        printf("%s HIGH PESTICIDE EMISSIONS detected!\n", bullet);
        printf("  %s Implement Integrated Pest Management (IPM) strategies\n", arrow);
        printf("  %s Use biological control agents and beneficial insects\n", arrow);
        printf("  %s Consider organic or lower-impact pesticide alternatives\n", arrow);
        printf("  %s Improve application timing and precision spraying\n", arrow);
        printf("  %s Plant pest-resistant crop varieties\n\n", arrow);
    }
    
    // Check livestock emissions
    if (results->livestock_emissions > results->total_emissions * 0.5) {
        printf("%s HIGH LIVESTOCK EMISSIONS detected!\n", bullet);
        printf("  %s Improve manure management systems (composting, anaerobic digestion)\n", arrow);
        printf("  %s Optimize feed quality and digestibility to reduce methane\n", arrow);
        printf("  %s Consider methane capture systems for biogas production\n", arrow);
        printf("  %s Implement rotational grazing to improve pasture health\n", arrow);
        printf("  %s Add feed additives that reduce methane production\n\n", arrow);
    }
    
    // Check irrigation emissions
    if (results->irrigation_emissions > results->total_emissions * 0.2) {
        printf("%s HIGH IRRIGATION EMISSIONS detected!\n", bullet);
        printf("  %s Install drip irrigation or micro-sprinkler systems\n", arrow);
        printf("  %s Use soil moisture sensors and weather-based scheduling\n", arrow);
        printf("  %s Consider renewable energy (solar) for irrigation pumping\n", arrow);
        printf("  %s Implement water-efficient crop varieties and mulching\n", arrow);
        printf("  %s Improve irrigation system maintenance to reduce losses\n\n", arrow);
    }
    
    // General recommendations based on total emissions per hectare
    if (results->per_hectare_emissions > 5.0) {
        printf("%s OVERALL HIGH EMISSIONS (>5 %s/ha) - URGENT ACTION NEEDED:\n", bullet, co2_unit);
        printf("  %s Consider transitioning to organic or regenerative practices\n", arrow);
        printf("  %s Implement carbon sequestration methods (cover crops, agroforestry)\n", arrow);
        printf("  %s Plant perennial crops or establish carbon-storing grasslands\n", arrow);
        printf("  %s Explore carbon credit opportunities and certification programs\n", arrow);
        printf("  %s Conduct energy audit and implement renewable energy solutions\n\n", arrow);
    } else if (results->per_hectare_emissions < 2.0) {
        printf("%s EXCELLENT! Low emissions (<2 %s/ha) - SUSTAINABILITY LEADER:\n", bullet, co2_unit);
        printf("  %s You're already following excellent sustainable practices\n", arrow);
        printf("  %s Consider carbon credit certification to monetize your efforts\n", arrow);
        printf("  %s Share your methods with other farmers and agricultural networks\n", arrow);
        printf("  %s Document and publish your sustainable farming case study\n", arrow);
        printf("  %s Explore opportunities for carbon-negative farming practices\n\n", arrow);
    } else {
        printf("%s MODERATE emissions (2-5 %s/ha) - GOOD PROGRESS POTENTIAL:\n", bullet, co2_unit);
        printf("  %s Focus on the highest emission categories identified above\n", arrow);
        printf("  %s Gradual improvements can make a significant difference\n", arrow);
        printf("  %s Monitor progress with regular carbon footprint assessments\n", arrow);
        printf("  %s Consider joining sustainable agriculture programs\n", arrow);
        printf("  %s Implement one major improvement per growing season\n\n", arrow);
    }
    
    printf("REFERENCES:\n");
    printf("%s IPCC 2006 + 2019 Refinement Guidelines for National GHG Inventories\n", bullet);
    printf("%s FAO EX-ACT Tool - Ex-Ante Carbon-balance Tool\n", bullet);
    printf("%s Ecoinvent LCI Database - Life Cycle Inventory Data\n", bullet);
    printf("===================================================================\n");
}
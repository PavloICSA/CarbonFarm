#ifndef REPORT_H
#define REPORT_H

#include "input.h"
#include "compute.h"

// Function declarations
void print_report(const FarmData *farm, const EmissionResults *results);
void print_legacy_report(const LegacyFarmData *farm, const EmissionResults *results);
void save_report_to_file(const FarmData *farm, const EmissionResults *results, const char *filename);
void save_legacy_report_to_file(const LegacyFarmData *farm, const EmissionResults *results, const char *filename);
void print_recommendations(const EmissionResults *results);

#endif
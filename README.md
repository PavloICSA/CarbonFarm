# Farm Carbon Footprint Estimator

## 🌱 Overview

The **Farm Carbon Footprint Estimator** is a lightweight C-based tool that calculates the **carbon footprint (CO₂-equivalent emissions)** of agricultural activities. It works offline, runs on any PC, and provides actionable recommendations to reduce emissions.

## ✨ Features

- **Multi-Crop Farm Support:** Analyze farms with up to 10 different crops
- **Predefined Crop Database:** 10 major crops with default agronomic parameters
- **Pesticide Emissions:** 8 common pesticides with realistic emission factors
- **Comprehensive Emission Calculations:**
  - Fertilizer (N, P, K) with crop-specific defaults
  - Manure application
  - Diesel fuel consumption
  - Irrigation water usage
  - Pesticide applications
  - Livestock methane (cows, pigs, chickens)
- **Multiple Interface Options:**
  - **Interactive Startup Menu:** Choose your preferred mode
  - **CLI Mode:** Multi-crop or legacy single-crop modes
  - **Simple UI:** Menu-driven interface
  - **Advanced UI:** Full interactive interface
  - **Batch Mode:** CSV file processing
- **Professional Reporting:** Clean table format with per-crop breakdown and farm totals
- **Zero External Dependencies:** Works on any system with a C compiler
- **Cross-platform:** Linux, macOS, Windows

---

## ⚖️ Emission Factors Used

The **greenhouse gas (GHG) emission factors** applied in this tool **are based on internationally recognized scientific sources**:

- _IPCC Guidelines for National Greenhouse Gas Inventories (2006, 2019 Refinement)_ – methodologies for calculating emissions from agriculture, soils, fuel combustion, and livestock.
- _FAO Ex-Ante Carbon-balance Tool (EX-ACT)_ – emission factors and accounting framework for agricultural practices.
- _Ecoinvent Database (v3.x)_ – life cycle inventory data for fertilizers, fuels, and energy use.

---

## ⚙️ Installation

### Clone the repository:
```bash
git clone [https://github.com/your-username/farm-carbon-footprint.git](https://github.com/PavloICSA/CarbonFarm)
cd farm-carbon-footprint
```

### Build on Linux/macOS:
```bash
make                 # Builds unified version with all interfaces
```

Or manually:
```bash
# Unified version with all interfaces (no dependencies)
gcc src/main.c src/input.c src/compute.c src/report.c src/ui.c src/simple_ui.c -o carbon -lm
```

### Build on Windows:
```cmd
build.bat           # Automated build script
```

Or manually:
```cmd
# Unified version with all interfaces (no dependencies)
gcc src\main.c src\input.c src\compute.c src\report.c src\ui.c src\simple_ui.c -o carbon.exe -lm
```

**Note for Windows users:** For proper UTF-8 symbol display, run `chcp 65001` before executing the program. If you see corrupted characters, the program will still work but symbols will be replaced with ASCII equivalents.

---

## 🚀 Usage

### Interactive Startup Menu (Recommended):
```bash
./carbon             # Linux/macOS
carbon.exe           # Windows
```

**Menu Options:**
```
========================================
    Farm Carbon Footprint Estimator
========================================
Select Mode of Operation:
  1. CLI Mode (Multi-Crop Support)
  2. Simple UI Mode (Menu-driven interface)
  3. Advanced UI Mode (Interactive interface)
  4. Batch Mode (CSV file processing)
  5. Help
  6. Exit
========================================
```

### Direct Usage Options:
```bash
# Direct CSV processing
./carbon data/sample_input.csv      # Process CSV file directly
carbon.exe data\sample_input.csv    # Windows

# Command-line flags (legacy support)
./carbon --simple    # Simple UI mode
./carbon --ui        # Advanced UI mode
./carbon --help      # Show help
```

---

## 📊 Example Output

### Multi-Crop Farm Report
```
===================================================================
                   Farm Carbon Footprint Report
===================================================================
Crop          Area(ha)   Fert(tCO2e)   Fuel   Irrig   Pestic   Live.   Total
-------------------------------------------------------------------
Wheat           10.0        1.20        0.45    0.30    0.25     0.00    2.20
Maize            5.0        0.90        0.20    0.35    0.15     0.00    1.60
Potato           2.0        0.75        0.10    0.25    0.05     0.00    1.15
-------------------------------------------------------------------
FARM TOTAL      17.0        2.85        0.75    0.90    0.45     0.00    5.95
Per Hectare                 0.17        0.04    0.05    0.03     0.00    0.35
===================================================================
Units: all emissions in tCO2e (tons of CO₂ equivalent).
===================================================================

* RECOMMENDATIONS:
===================================================================
• EXCELLENT! Low emissions (<2 tCO2e/ha) - SUSTAINABILITY LEADER:
  → You're already following excellent sustainable practices
  → Consider carbon credit certification to monetize your efforts
  → Share your methods with other farmers and agricultural networks
  → Document and publish your sustainable farming case study

REFERENCES:
• IPCC 2006 + 2019 Refinement Guidelines for National GHG Inventories
• FAO EX-ACT Tool - Ex-Ante Carbon-balance Tool
• Ecoinvent LCI Database - Life Cycle Inventory Data
===================================================================
```

### Supported Crops
| ID | Crop Name   | N(kg/ha) | P(kg/ha) | K(kg/ha) | Irrigation(mm) | Yield(t/ha) |
|----|-------------|----------|----------|----------|----------------|-------------|
| 1  | Wheat       | 120      | 60       | 30       | 450            | 5.5         |
| 2  | Maize       | 150      | 70       | 40       | 500            | 7.0         |
| 3  | Soybean     | 20       | 50       | 30       | 400            | 2.5         |
| 4  | Sunflower   | 60       | 40       | 40       | 380            | 2.2         |
| 5  | Potato      | 180      | 90       | 100      | 600            | 30.0        |
| 6  | Rice        | 160      | 70       | 60       | 1200           | 6.5         |
| 7  | Barley      | 100      | 50       | 30       | 400            | 4.5         |
| 8  | Rapeseed    | 160      | 70       | 50       | 450            | 3.0         |
| 9  | Sugar beet  | 180      | 80       | 90       | 550            | 60.0        |
| 10 | Vegetables  | 120      | 80       | 70       | 500            | 25.0        |

### Supported Pesticides
| ID | Trade Name   | Active Ingredient   | Type       | EF(kgCO2e/kg) |
|----|--------------|---------------------|------------|---------------|
| 1  | Roundup      | Glyphosate          | Herbicide  | 29.0          |
| 2  | Atrazine     | Atrazine            | Herbicide  | 25.0          |
| 3  | Harness      | Acetochlor          | Herbicide  | 27.0          |
| 4  | Karate       | Lambda-cyhalothrin  | Insecticide| 55.0          |
| 5  | Decis        | Deltamethrin        | Insecticide| 50.0          |
| 6  | Ridomil Gold | Metalaxyl           | Fungicide  | 45.0          |
| 7  | Bravo        | Chlorothalonil      | Fungicide  | 30.0          |
| 8  | Tilt         | Propiconazole       | Fungicide  | 35.0          |

---

## 📁 Project Structure

```
farm-carbon-footprint/
├── src/                    # Source code
│   ├── main.c              # Program entry point with startup menu
│   ├── input.c & input.h   # Input handling (interactive/CSV)
│   ├── compute.c & compute.h # Emission calculations
│   ├── report.c & report.h # Report generation
│   ├── ui.c & ui.h         # Advanced interactive UI
│   └── simple_ui.c & simple_ui.h # Simple console UI
├── data/                   # Sample data files
│   ├── sample_input.csv    # Legacy single-crop sample
│   └── multi_crop_sample.csv # Multi-crop sample
├── build.bat               # Windows build script
├── demo.bat                # Windows demo script
├── demo.sh                 # Linux/macOS demo script
├── Makefile                # Linux/macOS build configuration
├── carbon.exe              # Windows executable (after build)
└── README.md               # Documentation
```

---

## 🖥️ Interface Options

### CLI Mode
- Multi-crop farm support with up to 10 different crops
- Pesticide emissions calculation with 8 predefined pesticides
- Professional table output with per-crop breakdown
- Enhanced recommendations with sustainability advice

### Simple UI Mode
- Menu-driven interface with text-based prompts
- Choose between multi-crop or legacy modes
- Maximum compatibility across all systems

### Advanced UI Mode
- Full interactive interface with visual feedback
- Professional forms with box-drawing characters
- Color-coded results and recommendations

### Batch Mode
- Direct CSV file processing
- Support for both legacy single-crop and multi-crop formats
- Automated report generation

> **💡 All interfaces produce identical results - choose based on your preference!**

---

## 📄 CSV Input Formats

### Legacy Single-Crop Format
```csv
farm_size,crop_type,nitrogen,phosphorus,potassium,manure,diesel,irrigation,cows,pigs,chickens
50.0,wheat,120.0,40.0,60.0,2000.0,80.0,500.0,10,5,100
```

### Multi-Crop Format
```csv
crop_id,area,nitrogen,phosphorus,potassium,manure,diesel,irrigation,pesticide_id,pesticide_rate
1,10.0,120.0,60.0,30.0,2000.0,80.0,450.0,1,2.5
2,5.0,150.0,70.0,40.0,1500.0,90.0,500.0,4,1.0
```

**Key Fields:**
- **Area:** Farm/crop area in hectares
- **Fertilizers:** N, P₂O₅, K₂O in kg per hectare
- **Manure:** Application in kg per hectare
- **Diesel:** Consumption in liters per hectare
- **Irrigation:** Water in mm (1 mm = 10 m³/ha)
- **Livestock:** Number of animals (single-crop format)
- **Pesticides:** ID and application rate (multi-crop format)

**Usage:**
```bash
./carbon data/sample_input.csv         # Process CSV directly
./carbon data/multi_crop_sample.csv    # Multi-crop CSV
```

---

## 🧪 Testing & Demo

### Quick Start:
```bash
# Windows
demo.bat            # Comprehensive demo
build.bat           # Build program

# Linux/macOS
./demo.sh           # Interactive demo
make                # Build program
```

### Test Commands:
```bash
# Interactive mode (recommended)
./carbon            # Shows startup menu

# Direct CSV processing
./carbon data/sample_input.csv         # Single-crop data
./carbon data/multi_crop_sample.csv    # Multi-crop data

# Specific interface modes
./carbon --simple   # Simple UI
./carbon --ui       # Advanced UI
```

---

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

---

## 📄 License

This project is open source and available under the MIT License.

---

## 🎯 Project Summary

**Farm Carbon Footprint Estimator** - A lightweight, offline C tool to measure and reduce agricultural greenhouse gas emissions.

**Key Benefits:**
- **Fast & Offline:** Works without internet connection
- **Universal:** Runs on any system with a C compiler
- **Accurate:** Based on IPCC and FAO emission factors
- **User-Friendly:** Multiple interface options
- **Actionable:** Provides specific sustainability recommendations

**Technologies:**
- C (C99 standard)
- Standard C libraries only
- Cross-platform build system

---

## 📞 Support

For questions or issues, please open an issue on GitHub or contact the development team.

Happy farming! 🚜🌾

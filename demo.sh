#!/bin/bash

# Farm Carbon Footprint Estimator Demo Script
# For Linux/macOS systems

echo "╔══════════════════════════════════════════════════════════════╗"
echo "║          Farm Carbon Footprint Estimator Demo               ║"
echo "╚══════════════════════════════════════════════════════════════╝"
echo

# Check if the program exists
if [ ! -f "./carbon" ]; then
    echo "Building the program..."
    make
    if [ $? -ne 0 ]; then
        echo "Build failed. Please check the error messages above."
        exit 1
    fi
    echo "✓ Build successful!"
    echo
fi

echo "Choose demo mode:"
echo "1. NEW: Startup Menu Demo (shows the new interactive menu system)"
echo "2. CLI Demo (batch processing with sample data)"
echo "3. CLI Interactive Demo (command-line prompts)"
echo "4. Simple UI Demo (basic interface)"
echo "5. Advanced UI Demo (interactive interface)"
echo "6. Compare All Outputs"
echo "7. Exit"
echo

read -p "Enter your choice (1-7): " choice

case $choice in
    1)
        echo
        echo "🎯 NEW FEATURE: Startup Menu System"
        echo "=========================================="
        echo "This demonstrates the new startup menu that appears when you run:"
        echo "  ./carbon"
        echo
        echo "The menu provides 5 options:"
        echo "  1. CLI Mode (Multi-Crop Support)"
        echo "  2. Simple UI Mode (Multi-Crop or Legacy choice)"
        echo "  3. Advanced UI Mode (Legacy Single-Crop)"
        echo "  4. Batch Mode (load from CSV file)"
        echo "  5. Help"
        echo "  6. Exit"
        echo
        echo "Let's test the Help option (choice 5):"
        echo "5" | ./carbon
        echo
        echo "✓ This is the new default behavior - no command-line flags needed!"
        ;;
    2)
        echo
        echo "Running CLI demo with sample data..."
        echo "Input file: data/sample_input.csv"
        echo
        ./carbon data/sample_input.csv
        echo
        echo "✓ Demo completed! Report saved to report.txt"
        ;;
    3)
        echo
        echo "Starting CLI interactive mode with startup menu..."
        echo "You'll see the startup menu first, then choose CLI mode."
        echo
        ./carbon
        ;;
    4)
        echo
        echo "Starting Simple UI mode..."
        echo "Use number keys to navigate menus."
        echo
        ./carbon --simple
        ;;
    5)
        echo
        echo "Starting Advanced UI mode..."
        echo "Use arrow keys to navigate, Enter to select/edit."
        echo
        ./carbon --ui
        ;;
    6)
        echo
        echo "Running sample data through all interfaces..."
        echo
        echo "1. CLI Batch Processing:"
        echo "----------------------------------------"
        ./carbon data/sample_input.csv
        echo
        echo "2. Simple UI Batch Processing:"
        echo "----------------------------------------"
        ./carbon --simple data/sample_input.csv
        echo
        echo "3. Advanced UI Batch Processing:"
        echo "----------------------------------------"
        ./carbon --ui data/sample_input.csv
        echo
        echo "✓ All outputs should be identical - only the interface differs!"
        ;;
    7)
        echo "Goodbye!"
        exit 0
        ;;
    *)
        echo "Invalid choice. Please run the script again."
        exit 1
        ;;
esac

echo
echo "Demo completed!"
echo
echo "🎯 NEW Usage (Startup Menu System):"
echo "- ./carbon                    (shows startup menu with 5 options)"
echo
echo "Legacy Usage (Still Supported):"
echo "- ./carbon data/sample_input.csv (direct batch processing)"
echo "- ./carbon --simple           (simple UI)"
echo "- ./carbon --ui               (advanced UI)"
echo "- ./carbon --help             (show help)"
echo
echo "✨ The startup menu makes the program more user-friendly!"
echo "   No need to remember command-line flags anymore."
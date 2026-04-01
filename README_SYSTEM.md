# Water Refinement Automation System

## Overview
This is a comprehensive C++ project that implements an advanced **Water Refinement Automation System** for controlling and monitoring industrial water treatment plants. The system automates the process of water filtration, chemical treatment, and quality monitoring with real-time sensor integration.

## Project Architecture

### Core Components

1. **SensorManager** - Manages all water quality sensors
   - pH Level, Turbidity, Temperature, Dissolved Oxygen, TDS, Pressure, Flow Rate
   - Sensor reading validation and calibration
   - Historical data tracking
   - Calibration offset management

2. **FiltrationSystem** - Manages multi-stage water filtration
   - Pre-Filtration
   - Sand Filtration  
   - Activated Carbon Filtration
   - Reverse Osmosis
   - UV Treatment
   - Automatic efficiency monitoring and maintenance tracking

3. **ChemicalTreatment** - Manages chemical dosing and treatment
   - Coagulants (Aluminum Sulfate)
   - Disinfectants (Chlorine)
   - pH Adjusters (Sodium Carbonate)
   - Softening Agents (Lime)
   - Oxidizing Agents (Potassium Permanganate)
   - Auto-replenishment alerts when chemical levels drop below 20%
   - Automatic dose adjustment based on water quality parameters

4. **QualityMonitor** - Real-time water quality assessment
   - Continuous monitoring against WHO/EPA standards
   - Automated alert generation (INFO, WARNING, CRITICAL)
   - Alert history tracking (up to 1000 alerts)
   - Quality scoring system (0-100)
   - Parameter compliance checking

5. **AutomationController** - Central orchestration engine
   - System mode management (IDLE, NORMAL_OPERATION, MAINTENANCE, EMERGENCY_SHUTDOWN)
   - Coordinates all subsystems
   - Automated control cycles with configurable intervals
   - Emergency shutdown procedures
   - Comprehensive system health checks
   - Thread-safe operations with mutex locking

## Features

### Automation Capabilities
- **Automated Cycles**: Run multiple treatment cycles with configurable parameters
- **Self-Monitoring**: Continuous water quality compliance checking
- **Predictive Maintenance**: Filter life tracking and replacement scheduling
- **Chemical Management**: Auto-adjustment of chemical doses based on water quality
- **Emergency Modes**: Immediate shutdown with emergency protocols
- **Health Diagnostics**: Complete system status reports

### Monitoring & Reporting
- Real-time sensor data display
- Filtration efficiency tracking
- Chemical tank level monitoring
- Water quality scoring and compliance status
- Comprehensive system reports
- Historical data export capabilities
- Alert logging and history

### Quality Standards
The system enforces the following water quality standards:

| Parameter | Min | Max | Unit |
|-----------|-----|-----|------|
| pH Level | 6.5 | 8.5 | pH |
| Turbidity | 0 | 1.0 | NTU |
| Temperature | 5 | 30 | °C |
| Dissolved Oxygen | 6 | 15 | mg/L |
| Total Dissolved Solids | 0 | 1000 | mg/L |

## Project Structure

```
├── CMakeLists.txt                 # Build configuration
├── include/                       # Header files
│   ├── AutomationController.h
│   ├── ChemicalTreatment.h
│   ├── FiltrationSystem.h
│   ├── QualityMonitor.h
│   └── SensorManager.h
├── src/                          # Implementation files
│   ├── AutomationController.cpp
│   ├── ChemicalTreatment.cpp
│   ├── FiltrationSystem.cpp
│   ├── QualityMonitor.cpp
│   ├── SensorManager.cpp
│   └── main.cpp
└── build/                        # Build output directory
    └── water_refinement          # Compiled executable
```

## Building the Project

### Requirements
- C++17 or later
- CMake 3.10+
- GCC, Clang, or MSVC compiler

### Build Instructions

```bash
# Navigate to project directory
cd /workspaces/test

# Create build directory
mkdir -p build
cd build

# Configure with CMake
cmake ..

# Build the project
make

# Run the executable
./water_refinement
```

## Usage

The system provides an interactive menu interface:

```
=== MAIN MENU ===
1. Start System               - Begin water treatment operations
2. Run Automation Cycles      - Execute multiple treatment cycles
3. System Health Check        - Display complete system diagnostics
4. View Water Quality Report  - Show current water quality metrics
5. View Filtration Status     - Display filter efficiency and status
6. View Chemical Treatment    - Show chemical tank levels and dosing
7. Refill Chemical Tank       - Replenish chemical supplies
8. Replace Filter             - Perform filter maintenance
9. Emergency Shutdown         - Initiate emergency stop procedure
0. Exit                       - Terminate the program
```

## System Workflow

1. **Initialization**: System registers all sensors and loads default configuration
2. **Continuous Monitoring**: Real-time sensor data collection and validation
3. **Quality Assessment**: Water quality parameters checked against standards
4. **Automated Adjustment**: Chemical doses auto-adjusted if parameters out-of-range
5. **Filter Monitoring**: Efficiency tracked and maintenance alerts generated
6. **Alert Generation**: Severity-based alerts (INFO, WARNING, CRITICAL) issued
7. **Reporting**: Data logged and reports generated for analysis

## Key Classes

### SensorManager
- Manages 7 sensor types with real-time readings
- Validation based on sensor type and acceptable ranges
- Calibration tracking with error tolerance (0.05 default)
- Historical readingsretained for trend analysis

### FiltrationSystem
- 5-stage filtration pipeline
- Automatic efficiency tracking
- Clogging detection (efficiency falls below 70%)
- Replacement scheduling (10,000 operating hours)
- Overall system efficiency calculation

### ChemicalTreatment
- 6 chemical types with independent management
- Tank capacity and level tracking
- Dose adjustment limits (±50%)
- Auto-replenishment alerts at 20% tank level
- Treatment action logging

### QualityMonitor
- 7 water quality parameters monitored
- Compliance checking against standards
- Quality score calculation (0-100 scale)
- 3-level alert severity system
- Up to 1000 alert history entries

### AutomationController
- Central system orchestrator
- 4 system modes for different operational states
- Cycle counter for automation tracking
- Thread-safe design with mutex protection
- Configurable monitoring intervals

## Sample Output

When running the health check, you'll see:

```
=== SYSTEM HEALTH CHECK ===
System Mode: IDLE
Cycle Count: 0

=== SENSOR STATUS ===
All sensor readings with validation status

=== FILTRATION SYSTEM STATUS ===
Overall Efficiency: 77.38%
Individual filter stages and their status

=== CHEMICAL TREATMENT STATUS ===
Chemical tanks, levels, and dosing information

=== WATER QUALITY REPORT ===
All parameters with compliance status
Quality Score and Alert Summary
```

## Extension Points

The system is designed to be extensible:

- Add new sensor types in the `SensorType` enum
- Implement new chemical treatment protocols
- Add custom quality standards per application
- Extend alert system with email/SMS notifications
- Integrate with external monitoring systems
- Add machine learning for predictive maintenance

## Technical Highlights

- **C++17 Features**: Smart pointers, lambda expressions, auto type deduction
- **Memory Safety**: RAII patterns, smart pointer management
- **Thread Safety**: Mutex-protected critical sections
- **Scalability**: Modular design for easy component addition
- **Error Handling**: Validation at multiple levels
- **Standards Compliance**: Follows WHO/EPA water quality guidelines

## Performance Characteristics

- Sensor reading update: O(1) average case
- Quality compliance check: O(n) where n = number of parameters (~7)
- Filter efficiency calculation: O(m) where m = number of filters (~5)
- Memory footprint: ~50KB base + dynamic allocation for history

## Future Enhancements

- [ ] Database integration for persistent storage
- [ ] Network communication for remote monitoring
- [ ] Machine learning for predictive maintenance
- [ ] Mobile app integration via REST API
- [ ] Advanced analytics and trend prediction
- [ ] Multi-plant management and comparison
- [ ] IoT sensor integration
- [ ] Web dashboard interface

## Security Considerations

- Input validation on all user inputs
- Bounds checking on array/vector access
- Safe string handling with std::string
- No buffer overflow vulnerabilities
- Graceful error handling

## License

This project is provided as-is for educational and industrial automation purposes.

## Support

For issues or feature requests, review the system logs and ensure:
1. All sensors are properly calibrated
2. Chemical tanks are adequately filled
3. Filter maintenance is current
4. System has not entered EMERGENCY_SHUTDOWN mode

---

**Version**: 1.0  
**Last Updated**: April 2026  
**Language**: C++17  
**Platform**: Linux/Unix/Windows

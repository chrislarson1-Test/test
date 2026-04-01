# Water Refinement Automation System - Quick Reference

## System Components Quick Overview

```
┌─────────────────────────────────────────────────────────┐
│        AutomationController (System Orchestrator)        │
├────────────────┬─────────────────┬─────────────┬────────┤
│ SensorManager  │  Filtration     │  Chemical   │Quality │
│                │  System         │  Treatment  │Monitor │
│ • pH Level     │ • Pre-Filt      │ • Coagulant │• Comp. │
│ • Turbidity    │ • Sand Filt     │ • Disinfect │• Score │
│ • Temp         │ • Carbon        │ • pH Adjust │• Alert │
│ • DO           │ • RO            │ • Softening │• Report│
│ • TDS          │ • UV            │ • Oxidizer  │        │
│ • Pressure     │ • Monitor Eff.  │ • Chelating │        │
│ • Flow Rate    │ • Schedule Maint │ • Tank Mgmt │        │
└────────────────┴─────────────────┴─────────────┴────────┘
```

## Enumes & Constants Reference

### System Modes
```cpp
enum class SystemMode {
    IDLE,                    // System at rest
    NORMAL_OPERATION,        // Running treatment
    MAINTENANCE,             // Service mode
    EMERGENCY_SHUTDOWN       // Critical stop
};
```

### Sensor Types (7 Total)
```cpp
PH_LEVEL, TURBIDITY, TEMPERATURE, DISSOLVED_OXYGEN,
TOTAL_DISSOLVED_SOLIDS, PRESSURE, FLOW_RATE
```

### Filter Stages (5 Total)
```cpp
PRE_FILTRATION, SAND_FILTRATION, ACTIVATED_CARBON,
REVERSE_OSMOSIS, UV_TREATMENT
```

### Filter Status
```cpp
OPERATIONAL, CLOGGED, NEEDS_REPLACEMENT,
MAINTENANCE_REQUIRED, OFFLINE
```

### Chemical Types (6 Total)
```cpp
COAGULANT, DISINFECTANT, PH_ADJUSTER,
SOFTENING_AGENT, OXIDIZING_AGENT, CHELATING_AGENT
```

### Alert Severity
```cpp
INFO, WARNING, CRITICAL
```

## Key Constants

| Component | Constant | Value |
|-----------|----------|-------|
| SensorManager | CALIBRATION_ERROR_TOLERANCE | 0.05 |
| FiltrationSystem | MAX_OPERATING_HOURS | 10000 |
| FiltrationSystem | CLOGGING_THRESHOLD | 0.7 |
| ChemicalTreatment | REORDER_THRESHOLD | 0.2 (20%) |
| ChemicalTreatment | MAX_DOSE_ADJUSTMENT | 1.5 (50%) |
| QualityMonitor | MAX_ALERT_HISTORY | 1000 |

## Default Quality Standards

| Parameter | Min | Max | Unit |
|-----------|-----|-----|------|
| pH Level | 6.5 | 8.5 | pH |
| Turbidity | 0 | 1.0 | NTU |
| Temperature | 5 | 30 | °C |
| Dissolved Oxygen | 6 | 15 | mg/L |
| TDS | 0 | 1000 | mg/L |

## Code Examples

### Initialize System
```cpp
AutomationController controller;
controller.initialize();
controller.startSystem();
```

### Run Automation Cycle
```cpp
for (int i = 0; i < cycles; ++i) {
    controller.runAutomationCycle();
    // Cycle includes: sensor update, quality check, 
    // chemical adjustment, filter monitoring
}
```

### Check Sensor Reading
```cpp
SensorReading reading = sensor_manager->getSensorReading("SENSOR_PH_1");
if (reading.is_valid) {
    std::cout << "pH: " << reading.value << " " << reading.unit << std::endl;
}
```

### Get Water Quality Status
```cpp
WaterQualityParameters params = quality_monitor->getCurrentParameters();
double score = quality_monitor->getOverallQualityScore(); // 0-100
bool compliant = quality_monitor->isQualityWithinStandards();
```

### View Active Alerts
```cpp
std::vector<QualityAlert> alerts = quality_monitor->getActiveAlerts();
for (const auto& alert : alerts) {
    std::cout << alert.message << " (Value: " 
              << alert.current_value << ")" << std::endl;
}
```

### Get Filter Efficiency
```cpp
double efficiency = filtration_system->getFilterEfficiency(
    FilterStage::SAND_FILTRATION
);
double overall = filtration_system->getOverallFiltrationEfficiency();
```

### Check Chemical Levels
```cpp
double level = chemical_treatment->getChemicalLevel(
    ChemicalType::COAGULANT
);
if (chemical_treatment->isChemicalLow(ChemicalType::DISINFECTANT)) {
    std::cout << "Refill needed!" << std::endl;
}
```

## System Workflow Sequence

```
Initialize System
    ├── Register all sensors
    ├── Configure filter stages
    ├── Setup chemical tanks
    └── Apply quality standards

Main Control Loop
    ├── Update Sensor Readings
    │   └── Validate readings
    ├── Check Water Quality
    │   ├── Compare to standards
    │   └── Generate alerts
    ├── Adjust Chemicals (if auto-enabled)
    │   ├── pH adjustment
    │   └── Turbidity adjustment
    └── Monitor Filters
        ├── Track efficiency
        └── Generate maintenance alerts

Maintenance Mode
    ├── Replace filters
    ├── Refill chemicals
    └── Calibrate sensors

Emergency Shutdown
    ├── Stop all systems
    ├── Close valves
    ├── Halt chemical injection
    └── Log incident
```

## File I/O Operations

### Export System Data
```cpp
controller.exportSystemData("water_treatment_data_20260401.txt");
```

### Export Quality Data
```cpp
quality_monitor->exportQualityData("quality_report_20260401.csv");
```

## Menu Options at Runtime

| Option | Function | Trigger |
|--------|----------|---------|
| 1 | Start System | Initiate NORMAL_OPERATION mode |
| 2 | Run Cycles | Execute N automated treatment cycles |
| 3 | Health Check | Full system diagnostics report |
| 4 | Quality Report | Display water quality metrics |
| 5 | Filtration Info | Show filter stages and efficiency |
| 6 | Chemical Info | Display tank levels and dosing |
| 7 | Refill Chemical | Add to specific chemical tank |
| 8 | Replace Filter | Perform filter maintenance |
| 9 | Emergency Stop | Immediate system shutdown |
| 0 | Exit | Terminate program |

## Thread Safety

Critical sections protected by `std::mutex system_mutex`:
- Sensor reading updates
- Water quality assessment
- Filter efficiency changes
- Chemical adjustments
- Mode transitions

## Performance Tips

1. **Sensor Updates**: O(1) via hash map
2. **Quality Checks**: Runs in ~O(7) for 7 parameters
3. **Filter Analysis**: ~O(5) for 5 filter stages
4. **Chemical Mgmt**: ~O(6) for 6 chemical types
5. **Alert History**: Capped at 1000 entries to prevent memory bloat

## Common Operations

### Daily Check
```
Menu → Option 3 (Health Check)
Review: All sensors valid, No critical alerts, Filters operational
```

### Weekly Maintenance
```
Menu → View status of each system
Menu → Replace any clogged filters
Menu → Refill low chemical tanks
```

### Monthly Report
```
Menu → Export system data
Review trends in quality scores
Check if any alerts were repeated
Adjust standards if needed
```

## Default Configuration (at Startup)

- System Mode: IDLE
- Cycle Count: 0
- Auto Chemical Adjustment: Enabled
- Auto Filter Monitoring: Enabled
- Monitoring Interval: 5 seconds
- All Filters: OPERATIONAL
- All Chemicals: Tank full, dose default, inactive
- Quality Standards: WHO/EPA default values
- Alert History: Empty

## System Thresholds

| Threshold | Value | Action |
|-----------|-------|--------|
| Filter Clogging | < 70% efficiency | Generate WARNING |
| Filter Life | 10,000 hours | Schedule CRITICAL replacement |
| Chemical Low | < 20% tank | Generate reorder alert |
| Dose Adjustment | ±50% max | Prevents over/under treatment |
| Quality Out-of-Range | Any parameter | Generate alert + auto-adjust |
| Pressure Drop | > 20% variance | Indicates clogging |

---

## Build & Run Command Reference

```bash
# Full build
cd /workspaces/test && mkdir -p build && cd build && cmake .. && make

# Quick rebuild
cd /workspaces/test/build && make

# Run program
./water_refinement

# Clean build
rm -rf /workspaces/test/build && mkdir build && cd build && cmake .. && make
```

---

**Quick Start**: Build → Run → Select "1" to start system → "3" for health check → "0" to exit

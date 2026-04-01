#include "AutomationController.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <thread>
#include <chrono>

AutomationController::AutomationController()
    : sensor_manager(std::make_unique<SensorManager>()),
      filtration_system(std::make_unique<FiltrationSystem>()),
      chemical_treatment(std::make_unique<ChemicalTreatment>()),
      quality_monitor(std::make_unique<QualityMonitor>()),
      current_mode(SystemMode::IDLE),
      is_running(false),
      cycle_count(0),
      auto_chemical_adjustment(true),
      auto_filter_monitoring(true),
      monitoring_interval_seconds(5) {}

AutomationController::~AutomationController() {
    stopSystem();
}

void AutomationController::initialize() {
    std::cout << "Initializing Water Refinement Automation System..." << std::endl;
    setupDefaultConfiguration();
    std::cout << "System initialized successfully." << std::endl;
}

void AutomationController::setupDefaultConfiguration() {
    // Setup sensors
    sensor_manager->registerSensor("SENSOR_PH_1", SensorType::PH_LEVEL);
    sensor_manager->registerSensor("SENSOR_TURBIDITY_1", SensorType::TURBIDITY);
    sensor_manager->registerSensor("SENSOR_TEMP_1", SensorType::TEMPERATURE);
    sensor_manager->registerSensor("SENSOR_DO_1", SensorType::DISSOLVED_OXYGEN);
    sensor_manager->registerSensor("SENSOR_TDS_1", SensorType::TOTAL_DISSOLVED_SOLIDS);
    sensor_manager->registerSensor("SENSOR_PRESSURE_1", SensorType::PRESSURE);
    sensor_manager->registerSensor("SENSOR_FLOW_1", SensorType::FLOW_RATE);
    
    // Setup filtration stages
    filtration_system->addFilterStage(FilterStage::PRE_FILTRATION);
    filtration_system->addFilterStage(FilterStage::SAND_FILTRATION);
    filtration_system->addFilterStage(FilterStage::ACTIVATED_CARBON);
    filtration_system->addFilterStage(FilterStage::REVERSE_OSMOSIS);
    filtration_system->addFilterStage(FilterStage::UV_TREATMENT);
    
    // Setup chemicals
    chemical_treatment->addChemical(ChemicalType::COAGULANT, "Aluminum Sulfate", 
                                    1000.0, 50.0);
    chemical_treatment->addChemical(ChemicalType::DISINFECTANT, "Chlorine", 
                                    500.0, 1.0);
    chemical_treatment->addChemical(ChemicalType::PH_ADJUSTER, "Sodium Carbonate", 
                                    800.0, 20.0);
    chemical_treatment->addChemical(ChemicalType::SOFTENING_AGENT, "Lime", 
                                    600.0, 30.0);
    chemical_treatment->addChemical(ChemicalType::OXIDIZING_AGENT, "Potassium Permanganate",
                                    400.0, 2.0);
    
    // Setup quality standards
    QualityStandards standards;
    standards.min_ph = 6.5;
    standards.max_ph = 8.5;
    standards.max_turbidity = 1.0;
    standards.min_temperature = 5.0;
    standards.max_temperature = 30.0;
    standards.min_dissolved_oxygen = 6.0;
    standards.max_total_dissolved_solids = 1000.0;
    standards.target_pressure = 60.0;
    quality_monitor->setQualityStandards(standards);
}

void AutomationController::startSystem() {
    std::lock_guard<std::mutex> lock(system_mutex);
    is_running = true;
    current_mode = SystemMode::NORMAL_OPERATION;
    std::cout << "System started. Running in NORMAL_OPERATION mode." << std::endl;
}

void AutomationController::stopSystem() {
    std::lock_guard<std::mutex> lock(system_mutex);
    is_running = false;
    current_mode = SystemMode::IDLE;
    std::cout << "System stopped." << std::endl;
}

void AutomationController::setSystemMode(SystemMode mode) {
    std::lock_guard<std::mutex> lock(system_mutex);
    current_mode = mode;
    
    std::string mode_str;
    switch(mode) {
        case SystemMode::IDLE: mode_str = "IDLE"; break;
        case SystemMode::NORMAL_OPERATION: mode_str = "NORMAL_OPERATION"; break;
        case SystemMode::MAINTENANCE: mode_str = "MAINTENANCE"; break;
        case SystemMode::EMERGENCY_SHUTDOWN: mode_str = "EMERGENCY_SHUTDOWN"; break;
    }
    std::cout << "System mode changed to: " << mode_str << std::endl;
}

SystemMode AutomationController::getCurrentMode() const {
    return current_mode;
}

void AutomationController::enableAutoChemicalAdjustment(bool enable) {
    auto_chemical_adjustment = enable;
    std::cout << "Auto chemical adjustment: " << (enable ? "ENABLED" : "DISABLED") << std::endl;
}

void AutomationController::enableAutoFilterMonitoring(bool enable) {
    auto_filter_monitoring = enable;
    std::cout << "Auto filter monitoring: " << (enable ? "ENABLED" : "DISABLED") << std::endl;
}

void AutomationController::setMonitoringInterval(int seconds) {
    monitoring_interval_seconds = seconds;
    std::cout << "Monitoring interval set to " << seconds << " seconds." << std::endl;
}

void AutomationController::runAutomationCycle() {
    std::lock_guard<std::mutex> lock(system_mutex);
    
    if (!is_running || current_mode != SystemMode::NORMAL_OPERATION) {
        return;
    }
    
    cycle_count++;
    
    // Simulate sensor readings update
    updateSensorReadings();
    
    // Check water quality
    checkWaterQuality();
    
    // Adjust chemicals if auto mode is enabled
    if (auto_chemical_adjustment) {
        adjustChemicals();
    }
    
    // Monitor filters if auto mode is enabled
    if (auto_filter_monitoring) {
        monitorFilters();
    }
}

void AutomationController::updateSensorReadings() {
    // Simulate reading updates with slight variations
    double ph = 7.0 + (rand() % 10 - 5) * 0.1;
    double turbidity = 0.5 + (rand() % 8) * 0.1;
    double temperature = 20.0 + (rand() % 10 - 5);
    double dissolved_oxygen = 8.0 + (rand() % 6 - 3) * 0.1;
    double tds = 500.0 + (rand() % 100 - 50);
    double pressure = 60.0 + (rand() % 10 - 5);
    double flow_rate = 100.0 + (rand() % 20 - 10);
    
    sensor_manager->updateSensorReading("SENSOR_PH_1", ph);
    sensor_manager->updateSensorReading("SENSOR_TURBIDITY_1", turbidity);
    sensor_manager->updateSensorReading("SENSOR_TEMP_1", temperature);
    sensor_manager->updateSensorReading("SENSOR_DO_1", dissolved_oxygen);
    sensor_manager->updateSensorReading("SENSOR_TDS_1", tds);
    sensor_manager->updateSensorReading("SENSOR_PRESSURE_1", pressure);
    sensor_manager->updateSensorReading("SENSOR_FLOW_1", flow_rate);
}

void AutomationController::checkWaterQuality() {
    WaterQualityParameters params;
    params.ph_level = sensor_manager->getSensorReading("SENSOR_PH_1").value;
    params.turbidity = sensor_manager->getSensorReading("SENSOR_TURBIDITY_1").value;
    params.temperature = sensor_manager->getSensorReading("SENSOR_TEMP_1").value;
    params.dissolved_oxygen = sensor_manager->getSensorReading("SENSOR_DO_1").value;
    params.total_dissolved_solids = sensor_manager->getSensorReading("SENSOR_TDS_1").value;
    params.pressure = sensor_manager->getSensorReading("SENSOR_PRESSURE_1").value;
    params.flow_rate = sensor_manager->getSensorReading("SENSOR_FLOW_1").value;
    
    quality_monitor->updateParameters(params);
    quality_monitor->checkQualityCompliance();
}

void AutomationController::adjustChemicals() {
    double ph = quality_monitor->getCurrentParameters().ph_level;
    double turbidity = quality_monitor->getCurrentParameters().turbidity;
    
    QualityStandards standards = quality_monitor->getQualityStandards();
    
    // Adjust pH
    if (ph < standards.min_ph || ph > standards.max_ph) {
        chemical_treatment->adjustDoseForPH(ph, (standards.min_ph + standards.max_ph) / 2.0);
        chemical_treatment->activateTreatment(ChemicalType::PH_ADJUSTER);
    } else {
        chemical_treatment->deactivateTreatment(ChemicalType::PH_ADJUSTER);
    }
    
    // Adjust turbidity
    if (turbidity > standards.max_turbidity) {
        chemical_treatment->adjustDoseForTurbidity(turbidity, standards.max_turbidity);
        chemical_treatment->activateTreatment(ChemicalType::COAGULANT);
    } else {
        chemical_treatment->deactivateTreatment(ChemicalType::COAGULANT);
    }
    
    // Always activate disinfectant
    chemical_treatment->activateTreatment(ChemicalType::DISINFECTANT);
    
    // Check chemical levels and warn if low
    if (chemical_treatment->isChemicalLow(ChemicalType::DISINFECTANT)) {
        std::cout << "WARNING: Disinfectant level is low. Refill recommended." << std::endl;
    }
}

void AutomationController::monitorFilters() {
    filtration_system->checkFilterCondition();
    
    // Simulate filter operating hour increment
    filtration_system->incrementOperatingHours(FilterStage::PRE_FILTRATION, 1);
    filtration_system->incrementOperatingHours(FilterStage::SAND_FILTRATION, 1);
    filtration_system->incrementOperatingHours(FilterStage::ACTIVATED_CARBON, 1);
    filtration_system->incrementOperatingHours(FilterStage::REVERSE_OSMOSIS, 1);
    filtration_system->incrementOperatingHours(FilterStage::UV_TREATMENT, 1);
}

void AutomationController::performHealthCheck() {
    std::cout << "\n=== SYSTEM HEALTH CHECK ===" << std::endl;
    std::cout << "Cycle Count: " << cycle_count << std::endl;
    std::cout << "System Mode: ";
    switch(current_mode) {
        case SystemMode::IDLE: std::cout << "IDLE"; break;
        case SystemMode::NORMAL_OPERATION: std::cout << "NORMAL_OPERATION"; break;
        case SystemMode::MAINTENANCE: std::cout << "MAINTENANCE"; break;
        case SystemMode::EMERGENCY_SHUTDOWN: std::cout << "EMERGENCY_SHUTDOWN"; break;
    }
    std::cout << std::endl;
    
    sensor_manager->printSensorStatus();
    std::cout << filtration_system->getSystemStatusReport();
    chemical_treatment->printChemicalStatus();
    quality_monitor->printQualityStatus();
    
    std::cout << "=== END HEALTH CHECK ===" << std::endl;
}

void AutomationController::emergencyShutdown() {
    std::lock_guard<std::mutex> lock(system_mutex);
    current_mode = SystemMode::EMERGENCY_SHUTDOWN;
    is_running = false;
    std::cout << "EMERGENCY SHUTDOWN ACTIVATED!" << std::endl;
    std::cout << "All systems halted immediately." << std::endl;
}

void AutomationController::resetSystem() {
    std::lock_guard<std::mutex> lock(system_mutex);
    is_running = false;
    cycle_count = 0;
    current_mode = SystemMode::IDLE;
    quality_monitor->clearAlerts();
    std::cout << "System reset completed." << std::endl;
}

void AutomationController::printSystemStatus() const {
    std::cout << "\n=== SYSTEM STATUS ===" << std::endl;
    std::cout << "Is Running: " << (is_running ? "YES" : "NO") << std::endl;
    std::cout << "Cycle Count: " << cycle_count << std::endl;
    std::cout << "Quality Score: " << std::fixed << std::setprecision(2) 
              << quality_monitor->getOverallQualityScore() << "/100" << std::endl;
    std::cout << std::string(22, '=') << std::endl;
}

std::string AutomationController::generateSystemReport() const {
    std::ostringstream report;
    report << "\n=== COMPREHENSIVE SYSTEM REPORT ===" << std::endl;
    report << quality_monitor->getQualityReport();
    report << filtration_system->getSystemStatusReport();
    report << chemical_treatment->getTreatmentReport();
    return report.str();
}

void AutomationController::exportSystemData(const std::string& filename) const {
    quality_monitor->exportQualityData(filename);
}

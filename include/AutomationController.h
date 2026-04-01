#ifndef AUTOMATION_CONTROLLER_H
#define AUTOMATION_CONTROLLER_H

#include "SensorManager.h"
#include "FiltrationSystem.h"
#include "ChemicalTreatment.h"
#include "QualityMonitor.h"
#include <memory>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

// System modes
enum class SystemMode {
    IDLE,
    NORMAL_OPERATION,
    MAINTENANCE,
    EMERGENCY_SHUTDOWN
};

class AutomationController {
public:
    std::unique_ptr<SensorManager> sensor_manager;
    std::unique_ptr<FiltrationSystem> filtration_system;
    std::unique_ptr<ChemicalTreatment> chemical_treatment;
    std::unique_ptr<QualityMonitor> quality_monitor;

private:
    
    SystemMode current_mode;
    bool is_running;
    int cycle_count;
    std::mutex system_mutex;
    
    // Control parameters
    bool auto_chemical_adjustment;
    bool auto_filter_monitoring;
    int monitoring_interval_seconds;

public:
    AutomationController();
    ~AutomationController();
    
    // System initialization
    void initialize();
    void setupDefaultConfiguration();
    
    // System control
    void startSystem();
    void stopSystem();
    void setSystemMode(SystemMode mode);
    SystemMode getCurrentMode() const;
    
    // Automation control
    void enableAutoChemicalAdjustment(bool enable);
    void enableAutoFilterMonitoring(bool enable);
    void setMonitoringInterval(int seconds);
    
    // Main control loop
    void runAutomationCycle();
    void performHealthCheck();
    
    // Parameter updates
    void updateSensorReadings();
    void checkWaterQuality();
    void adjustChemicals();
    void monitorFilters();
    
    // Emergency operations
    void emergencyShutdown();
    void resetSystem();
    
    // Reporting and status
    void printSystemStatus() const;
    std::string generateSystemReport() const;
    void exportSystemData(const std::string& filename) const;
};

#endif // AUTOMATION_CONTROLLER_H

#include "QualityMonitor.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>

QualityMonitor::QualityMonitor() {
    // Default standards for drinking water
    standards.min_ph = 6.5;
    standards.max_ph = 8.5;
    standards.max_turbidity = 1.0;
    standards.min_temperature = 5.0;
    standards.max_temperature = 30.0;
    standards.min_dissolved_oxygen = 6.0;
    standards.max_total_dissolved_solids = 1000.0;
    standards.target_pressure = 60.0;
    
    // Initialize current parameters
    current_parameters.ph_level = 7.0;
    current_parameters.turbidity = 0.5;
    current_parameters.temperature = 20.0;
    current_parameters.dissolved_oxygen = 8.0;
    current_parameters.total_dissolved_solids = 500.0;
    current_parameters.pressure = 60.0;
    current_parameters.flow_rate = 100.0;
}

void QualityMonitor::setQualityStandards(const QualityStandards& stds) {
    standards = stds;
}

QualityStandards QualityMonitor::getQualityStandards() const {
    return standards;
}

void QualityMonitor::updateParameters(const WaterQualityParameters& params) {
    current_parameters = params;
}

WaterQualityParameters QualityMonitor::getCurrentParameters() const {
    return current_parameters;
}

bool QualityMonitor::isQualityWithinStandards() const {
    return (current_parameters.ph_level >= standards.min_ph &&
            current_parameters.ph_level <= standards.max_ph &&
            current_parameters.turbidity <= standards.max_turbidity &&
            current_parameters.temperature >= standards.min_temperature &&
            current_parameters.temperature <= standards.max_temperature &&
            current_parameters.dissolved_oxygen >= standards.min_dissolved_oxygen &&
            current_parameters.total_dissolved_solids <= standards.max_total_dissolved_solids);
}

double QualityMonitor::getOverallQualityScore() const {
    double score = 100.0;
    
    // pH score
    if (current_parameters.ph_level < standards.min_ph || 
        current_parameters.ph_level > standards.max_ph) {
        score -= 20.0;
    }
    
    // Turbidity score
    if (current_parameters.turbidity > standards.max_turbidity) {
        double excess = current_parameters.turbidity - standards.max_turbidity;
        score -= std::min(20.0, excess * 10.0);
    }
    
    // Dissolved oxygen score
    if (current_parameters.dissolved_oxygen < standards.min_dissolved_oxygen) {
        score -= 15.0;
    }
    
    // TDS score
    if (current_parameters.total_dissolved_solids > standards.max_total_dissolved_solids) {
        double excess = current_parameters.total_dissolved_solids - standards.max_total_dissolved_solids;
        score -= std::min(25.0, excess * 0.01);
    }
    
    return std::max(0.0, score);
}

void QualityMonitor::checkQualityCompliance() {
    // Check pH
    if (current_parameters.ph_level < standards.min_ph) {
        generateAlert(AlertSeverity::WARNING, "pH Level",
                     current_parameters.ph_level, standards.min_ph);
    } else if (current_parameters.ph_level > standards.max_ph) {
        generateAlert(AlertSeverity::WARNING, "pH Level",
                     current_parameters.ph_level, standards.max_ph);
    }
    
    // Check turbidity
    if (current_parameters.turbidity > standards.max_turbidity) {
        generateAlert(AlertSeverity::WARNING, "Turbidity",
                     current_parameters.turbidity, standards.max_turbidity);
    }
    
    // Check dissolved oxygen
    if (current_parameters.dissolved_oxygen < standards.min_dissolved_oxygen) {
        generateAlert(AlertSeverity::CRITICAL, "Dissolved Oxygen",
                     current_parameters.dissolved_oxygen, standards.min_dissolved_oxygen);
    }
    
    // Check TDS
    if (current_parameters.total_dissolved_solids > standards.max_total_dissolved_solids) {
        generateAlert(AlertSeverity::CRITICAL, "Total Dissolved Solids",
                     current_parameters.total_dissolved_solids, 
                     standards.max_total_dissolved_solids);
    }
    
    // Check temperature
    if (current_parameters.temperature < standards.min_temperature ||
        current_parameters.temperature > standards.max_temperature) {
        generateAlert(AlertSeverity::INFO, "Temperature",
                     current_parameters.temperature, standards.target_pressure);
    }
}

void QualityMonitor::generateAlert(AlertSeverity severity, const std::string& parameter,
                                  double current, double threshold) {
    QualityAlert alert;
    alert.severity = severity;
    alert.parameter = parameter;
    alert.current_value = current;
    alert.threshold_value = threshold;
    
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    alert.timestamp = oss.str();
    
    // Generate message
    std::ostringstream msg;
    msg << "ALERT: " << parameter << " is " << std::fixed << std::setprecision(2)
        << current << " (threshold: " << threshold << ")";
    alert.message = msg.str();
    
    active_alerts.push_back(alert);
    alert_history.push_back(alert);
    
    // Limit history size
    if (alert_history.size() > MAX_ALERT_HISTORY) {
        alert_history.erase(alert_history.begin());
    }
}

std::vector<QualityAlert> QualityMonitor::getActiveAlerts() const {
    return active_alerts;
}

void QualityMonitor::clearAlerts() {
    active_alerts.clear();
}

std::string QualityMonitor::getQualityReport() const {
    std::ostringstream report;
    report << "\n=== WATER QUALITY REPORT ===" << std::endl;
    report << std::fixed << std::setprecision(2);
    report << "pH Level: " << current_parameters.ph_level 
           << " (Target: " << standards.min_ph << "-" << standards.max_ph << ")" << std::endl;
    report << "Turbidity: " << current_parameters.turbidity 
           << " NTU (Max: " << standards.max_turbidity << ")" << std::endl;
    report << "Temperature: " << current_parameters.temperature 
           << "°C" << std::endl;
    report << "Dissolved Oxygen: " << current_parameters.dissolved_oxygen 
           << " mg/L (Min: " << standards.min_dissolved_oxygen << ")" << std::endl;
    report << "Total Dissolved Solids: " << current_parameters.total_dissolved_solids 
           << " mg/L (Max: " << standards.max_total_dissolved_solids << ")" << std::endl;
    report << "Pressure: " << current_parameters.pressure << " PSI" << std::endl;
    report << "Flow Rate: " << current_parameters.flow_rate << " GPM" << std::endl;
    report << "\nOverall Quality Score: " << getOverallQualityScore() << "/100" << std::endl;
    report << "Status: " << (isQualityWithinStandards() ? "COMPLIANT" : "NON-COMPLIANT") << std::endl;
    report << std::string(28, '=') << std::endl;
    return report.str();
}

std::string QualityMonitor::getAlertReport() const {
    std::ostringstream report;
    report << "\n=== ACTIVE ALERTS ===" << std::endl;
    
    if (active_alerts.empty()) {
        report << "No active alerts." << std::endl;
    } else {
        for (const auto& alert : active_alerts) {
            std::string severity_str;
            switch(alert.severity) {
                case AlertSeverity::INFO: severity_str = "INFO"; break;
                case AlertSeverity::WARNING: severity_str = "WARNING"; break;
                case AlertSeverity::CRITICAL: severity_str = "CRITICAL"; break;
            }
            report << "[" << severity_str << "] " << alert.message << std::endl;
        }
    }
    report << std::string(20, '=') << std::endl;
    return report.str();
}

void QualityMonitor::printQualityStatus() const {
    std::cout << getQualityReport();
    std::cout << getAlertReport();
}

void QualityMonitor::exportQualityData(const std::string& filename) const {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        
        file << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << ","
             << std::fixed << std::setprecision(2)
             << current_parameters.ph_level << ","
             << current_parameters.turbidity << ","
             << current_parameters.temperature << ","
             << current_parameters.dissolved_oxygen << ","
             << current_parameters.total_dissolved_solids << ","
             << getOverallQualityScore() << std::endl;
        file.close();
        std::cout << "Quality data exported to " << filename << std::endl;
    }
}

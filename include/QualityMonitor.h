#ifndef QUALITY_MONITOR_H
#define QUALITY_MONITOR_H

#include <string>
#include <vector>
#include <unordered_map>

// Water quality parameters
struct WaterQualityParameters {
    double ph_level;
    double turbidity;
    double temperature;
    double dissolved_oxygen;
    double total_dissolved_solids;
    double pressure;
    double flow_rate;
};

// Water quality standards
struct QualityStandards {
    double min_ph;
    double max_ph;
    double max_turbidity;
    double min_temperature;
    double max_temperature;
    double min_dissolved_oxygen;
    double max_total_dissolved_solids;
    double target_pressure;
};

// Alert severity levels
enum class AlertSeverity {
    INFO,
    WARNING,
    CRITICAL
};

// Quality alert
struct QualityAlert {
    AlertSeverity severity;
    std::string parameter;
    std::string message;
    double current_value;
    double threshold_value;
    std::string timestamp;
};

class QualityMonitor {
private:
    WaterQualityParameters current_parameters;
    QualityStandards standards;
    std::vector<QualityAlert> active_alerts;
    std::vector<QualityAlert> alert_history;
    
    static const int MAX_ALERT_HISTORY = 1000;

public:
    QualityMonitor();
    
    // Standard setting
    void setQualityStandards(const QualityStandards& stds);
    QualityStandards getQualityStandards() const;
    
    // Parameters update
    void updateParameters(const WaterQualityParameters& params);
    WaterQualityParameters getCurrentParameters() const;
    
    // Quality assessment
    bool isQualityWithinStandards() const;
    double getOverallQualityScore() const;
    
    // Alert management
    void checkQualityCompliance();
    void generateAlert(AlertSeverity severity, const std::string& parameter, 
                      double current, double threshold);
    std::vector<QualityAlert> getActiveAlerts() const;
    void clearAlerts();
    
    // Reporting
    std::string getQualityReport() const;
    std::string getAlertReport() const;
    void printQualityStatus() const;
    void exportQualityData(const std::string& filename) const;
};

#endif // QUALITY_MONITOR_H

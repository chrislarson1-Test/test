#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <string>
#include <unordered_map>
#include <chrono>
#include <vector>

// Sensor data structure
struct SensorReading {
    std::string sensor_id;
    std::string sensor_type;
    double value;
    std::string unit;
    std::chrono::system_clock::time_point timestamp;
    bool is_valid;
};

// Sensor types enumeration
enum class SensorType {
    PH_LEVEL,
    TURBIDITY,
    TEMPERATURE,
    DISSOLVED_OXYGEN,
    TOTAL_DISSOLVED_SOLIDS,
    PRESSURE,
    FLOW_RATE
};

class SensorManager {
private:
    std::unordered_map<std::string, SensorReading> sensor_readings;
    std::vector<SensorReading> reading_history;
    static constexpr double CALIBRATION_ERROR_TOLERANCE = 0.05;

public:
    SensorManager();
    
    // Sensor reading operations
    void registerSensor(const std::string& sensor_id, SensorType type);
    void updateSensorReading(const std::string& sensor_id, double value);
    SensorReading getSensorReading(const std::string& sensor_id) const;
    
    // Validation and calibration
    bool validateReading(const SensorReading& reading);
    void calibrateSensor(const std::string& sensor_id, double calibration_offset);
    
    // Data retrieval
    std::vector<SensorReading> getAllReadings() const;
    std::vector<SensorReading> getReadingsHistory(int last_n_readings = 100) const;
    
    // Status
    void printSensorStatus() const;
};

#endif // SENSOR_MANAGER_H

#include "SensorManager.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>

SensorManager::SensorManager() {}

void SensorManager::registerSensor(const std::string& sensor_id, SensorType type) {
    SensorReading reading;
    reading.sensor_id = sensor_id;
    reading.is_valid = true;
    reading.timestamp = std::chrono::system_clock::now();
    
    switch(type) {
        case SensorType::PH_LEVEL:
            reading.sensor_type = "pH Level";
            reading.unit = "pH";
            reading.value = 7.0;
            break;
        case SensorType::TURBIDITY:
            reading.sensor_type = "Turbidity";
            reading.unit = "NTU";
            reading.value = 0.0;
            break;
        case SensorType::TEMPERATURE:
            reading.sensor_type = "Temperature";
            reading.unit = "°C";
            reading.value = 25.0;
            break;
        case SensorType::DISSOLVED_OXYGEN:
            reading.sensor_type = "Dissolved Oxygen";
            reading.unit = "mg/L";
            reading.value = 8.0;
            break;
        case SensorType::TOTAL_DISSOLVED_SOLIDS:
            reading.sensor_type = "Total Dissolved Solids";
            reading.unit = "mg/L";
            reading.value = 500.0;
            break;
        case SensorType::PRESSURE:
            reading.sensor_type = "Pressure";
            reading.unit = "PSI";
            reading.value = 60.0;
            break;
        case SensorType::FLOW_RATE:
            reading.sensor_type = "Flow Rate";
            reading.unit = "GPM";
            reading.value = 100.0;
            break;
    }
    
    sensor_readings[sensor_id] = reading;
}

void SensorManager::updateSensorReading(const std::string& sensor_id, double value) {
    auto it = sensor_readings.find(sensor_id);
    if (it != sensor_readings.end()) {
        it->second.value = value;
        it->second.timestamp = std::chrono::system_clock::now();
        it->second.is_valid = validateReading(it->second);
        reading_history.push_back(it->second);
    }
}

SensorReading SensorManager::getSensorReading(const std::string& sensor_id) const {
    auto it = sensor_readings.find(sensor_id);
    if (it != sensor_readings.end()) {
        return it->second;
    }
    SensorReading empty_reading;
    empty_reading.is_valid = false;
    return empty_reading;
}

bool SensorManager::validateReading(const SensorReading& reading) {
    // Validate based on sensor type
    if (reading.sensor_type == "pH Level") {
        return reading.value >= 0.0 && reading.value <= 14.0;
    } else if (reading.sensor_type == "Turbidity") {
        return reading.value >= 0.0 && reading.value <= 500.0;
    } else if (reading.sensor_type == "Temperature") {
        return reading.value >= -10.0 && reading.value <= 60.0;
    } else if (reading.sensor_type == "Dissolved Oxygen") {
        return reading.value >= 0.0 && reading.value <= 20.0;
    } else if (reading.sensor_type == "Total Dissolved Solids") {
        return reading.value >= 0.0 && reading.value <= 2000.0;
    } else if (reading.sensor_type == "Pressure") {
        return reading.value >= 0.0 && reading.value <= 200.0;
    } else if (reading.sensor_type == "Flow Rate") {
        return reading.value >= 0.0 && reading.value <= 500.0;
    }
    return false;
}

void SensorManager::calibrateSensor(const std::string& sensor_id, double calibration_offset) {
    auto it = sensor_readings.find(sensor_id);
    if (it != sensor_readings.end()) {
        it->second.value += calibration_offset;
        std::cout << "Sensor " << sensor_id << " calibrated with offset: " 
                  << calibration_offset << std::endl;
    }
}

std::vector<SensorReading> SensorManager::getAllReadings() const {
    std::vector<SensorReading> readings;
    for (const auto& pair : sensor_readings) {
        readings.push_back(pair.second);
    }
    return readings;
}

std::vector<SensorReading> SensorManager::getReadingsHistory(int last_n_readings) const {
    std::vector<SensorReading> history;
    int start_index = std::max(0, static_cast<int>(reading_history.size()) - last_n_readings);
    history.insert(history.end(), 
                  reading_history.begin() + start_index, 
                  reading_history.end());
    return history;
}

void SensorManager::printSensorStatus() const {
    std::cout << "\n=== SENSOR STATUS ===" << std::endl;
    std::cout << std::left << std::setw(15) << "Sensor ID" 
              << std::setw(25) << "Type" 
              << std::setw(15) << "Value" 
              << std::setw(10) << "Unit" 
              << "Valid" << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    
    for (const auto& pair : sensor_readings) {
        const SensorReading& reading = pair.second;
        std::cout << std::left << std::setw(15) << reading.sensor_id
                  << std::setw(25) << reading.sensor_type
                  << std::setw(15) << std::fixed << std::setprecision(2) << reading.value
                  << std::setw(10) << reading.unit
                  << (reading.is_valid ? "YES" : "NO") << std::endl;
    }
    std::cout << std::string(70, '-') << std::endl;
}

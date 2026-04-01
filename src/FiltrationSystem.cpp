#include "FiltrationSystem.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>

FiltrationSystem::FiltrationSystem() {}

void FiltrationSystem::addFilterStage(FilterStage stage) {
    auto it = std::find(filter_stages.begin(), filter_stages.end(), stage);
    if (it == filter_stages.end()) {
        filter_stages.push_back(stage);
        filter_status[stage] = FilterStatus::OPERATIONAL;
        filter_efficiency[stage] = 0.95;
        operating_hours[stage] = 0;
        std::cout << "Filter stage added successfully." << std::endl;
    }
}

void FiltrationSystem::removeFilterStage(FilterStage stage) {
    auto it = std::find(filter_stages.begin(), filter_stages.end(), stage);
    if (it != filter_stages.end()) {
        filter_stages.erase(it);
        filter_status.erase(stage);
        filter_efficiency.erase(stage);
        operating_hours.erase(stage);
        std::cout << "Filter stage removed successfully." << std::endl;
    }
}

void FiltrationSystem::updateFilterEfficiency(FilterStage stage, double efficiency) {
    if (efficiency >= 0.0 && efficiency <= 1.0) {
        filter_efficiency[stage] = efficiency;
    }
}

FilterStatus FiltrationSystem::getFilterStatus(FilterStage stage) const {
    auto it = filter_status.find(stage);
    if (it != filter_status.end()) {
        return it->second;
    }
    return FilterStatus::OFFLINE;
}

void FiltrationSystem::checkFilterCondition() {
    for (auto& pair : filter_efficiency) {
        FilterStage stage = pair.first;
        double efficiency = pair.second;
        int hours = operating_hours[stage];
        
        // Check for clogging
        if (efficiency < CLOGGING_EFFICIENCY_THRESHOLD) {
            filter_status[stage] = FilterStatus::CLOGGED;
        }
        // Check for old age
        else if (hours >= MAX_OPERATING_HOURS_BEFORE_REPLACEMENT) {
            filter_status[stage] = FilterStatus::NEEDS_REPLACEMENT;
        }
        else {
            filter_status[stage] = FilterStatus::OPERATIONAL;
        }
    }
}

double FiltrationSystem::getFilterEfficiency(FilterStage stage) const {
    auto it = filter_efficiency.find(stage);
    if (it != filter_efficiency.end()) {
        return it->second;
    }
    return 0.0;
}

void FiltrationSystem::replaceFilter(FilterStage stage) {
    filter_status[stage] = FilterStatus::OPERATIONAL;
    filter_efficiency[stage] = 0.95;
    operating_hours[stage] = 0;
    std::cout << "Filter replaced successfully." << std::endl;
}

void FiltrationSystem::cleanFilter(FilterStage stage) {
    double current_efficiency = filter_efficiency[stage];
    if (current_efficiency < 0.95) {
        filter_efficiency[stage] = std::min(0.95, current_efficiency + 0.15);
        filter_status[stage] = FilterStatus::OPERATIONAL;
        std::cout << "Filter cleaned successfully. Efficiency improved." << std::endl;
    }
}

void FiltrationSystem::incrementOperatingHours(FilterStage stage, int hours) {
    operating_hours[stage] += hours;
}

double FiltrationSystem::getOverallFiltrationEfficiency() const {
    if (filter_efficiency.empty()) return 0.0;
    
    double total_efficiency = 1.0;
    for (const auto& pair : filter_efficiency) {
        total_efficiency *= pair.second;
    }
    return total_efficiency;
}

std::string FiltrationSystem::getSystemStatusReport() const {
    std::ostringstream report;
    report << "\n=== FILTRATION SYSTEM STATUS ===" << std::endl;
    report << "Overall Efficiency: " << std::fixed << std::setprecision(2) 
           << (getOverallFiltrationEfficiency() * 100.0) << "%" << std::endl;
    report << "\nFilter Stages:" << std::endl;
    
    for (const auto& stage : filter_stages) {
        report << "  Stage: ";
        switch(stage) {
            case FilterStage::PRE_FILTRATION: report << "Pre-Filtration"; break;
            case FilterStage::SAND_FILTRATION: report << "Sand Filtration"; break;
            case FilterStage::ACTIVATED_CARBON: report << "Activated Carbon"; break;
            case FilterStage::REVERSE_OSMOSIS: report << "Reverse Osmosis"; break;
            case FilterStage::UV_TREATMENT: report << "UV Treatment"; break;
        }
        
        report << " - Status: ";
        FilterStatus status = filter_status.at(stage);
        switch(status) {
            case FilterStatus::OPERATIONAL: report << "OPERATIONAL"; break;
            case FilterStatus::CLOGGED: report << "CLOGGED"; break;
            case FilterStatus::NEEDS_REPLACEMENT: report << "NEEDS REPLACEMENT"; break;
            case FilterStatus::MAINTENANCE_REQUIRED: report << "MAINTENANCE REQUIRED"; break;
            case FilterStatus::OFFLINE: report << "OFFLINE"; break;
        }
        
        report << " (Efficiency: " << std::fixed << std::setprecision(1) 
               << (filter_efficiency.at(stage) * 100.0) << "%, "
               << "Hours: " << operating_hours.at(stage) << ")" << std::endl;
    }
    
    report << std::string(35, '=') << std::endl;
    return report.str();
}

void FiltrationSystem::printFilterChain() const {
    std::cout << "\n=== FILTER CHAIN ===" << std::endl;
    for (size_t i = 0; i < filter_stages.size(); ++i) {
        if (i > 0) std::cout << "  ↓" << std::endl;
        
        FilterStage stage = filter_stages[i];
        std::cout << "  Stage " << (i + 1) << ": ";
        
        switch(stage) {
            case FilterStage::PRE_FILTRATION: std::cout << "Pre-Filtration"; break;
            case FilterStage::SAND_FILTRATION: std::cout << "Sand Filtration"; break;
            case FilterStage::ACTIVATED_CARBON: std::cout << "Activated Carbon"; break;
            case FilterStage::REVERSE_OSMOSIS: std::cout << "Reverse Osmosis"; break;
            case FilterStage::UV_TREATMENT: std::cout << "UV Treatment"; break;
        }
        std::cout << std::endl;
    }
    std::cout << std::string(20, '=') << std::endl;
}

#ifndef FILTRATION_SYSTEM_H
#define FILTRATION_SYSTEM_H

#include <string>
#include <vector>
#include <unordered_map>

// Filter stage enumeration
enum class FilterStage {
    PRE_FILTRATION,
    SAND_FILTRATION,
    ACTIVATED_CARBON,
    REVERSE_OSMOSIS,
    UV_TREATMENT
};

// Filter status enumeration
enum class FilterStatus {
    OPERATIONAL,
    CLOGGED,
    NEEDS_REPLACEMENT,
    MAINTENANCE_REQUIRED,
    OFFLINE
};

class FiltrationSystem {
private:
    std::vector<FilterStage> filter_stages;
    std::unordered_map<FilterStage, FilterStatus> filter_status;
    std::unordered_map<FilterStage, double> filter_efficiency;
    std::unordered_map<FilterStage, int> operating_hours;
    
    static constexpr int MAX_OPERATING_HOURS_BEFORE_REPLACEMENT = 10000;
    static constexpr double CLOGGING_EFFICIENCY_THRESHOLD = 0.7;

public:
    FiltrationSystem();
    
    // Filter operations
    void addFilterStage(FilterStage stage);
    void removeFilterStage(FilterStage stage);
    void updateFilterEfficiency(FilterStage stage, double efficiency);
    
    // Status monitoring
    FilterStatus getFilterStatus(FilterStage stage) const;
    void checkFilterCondition();
    double getFilterEfficiency(FilterStage stage) const;
    
    // Maintenance operations
    void replaceFilter(FilterStage stage);
    void cleanFilter(FilterStage stage);
    void incrementOperatingHours(FilterStage stage, int hours);
    
    // System operations
    double getOverallFiltrationEfficiency() const;
    std::string getSystemStatusReport() const;
    void printFilterChain() const;
};

#endif // FILTRATION_SYSTEM_H

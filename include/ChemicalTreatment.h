#ifndef CHEMICAL_TREATMENT_H
#define CHEMICAL_TREATMENT_H

#include <string>
#include <unordered_map>
#include <vector>

// Chemical type enumeration
enum class ChemicalType {
    COAGULANT,
    DISINFECTANT,
    PH_ADJUSTER,
    SOFTENING_AGENT,
    OXIDIZING_AGENT,
    CHELATING_AGENT
};

// Chemical dosing information
struct ChemicalDose {
    ChemicalType type;
    std::string chemical_name;
    double dose_amount;        // in mg/L
    double tank_capacity;       // in liters
    double current_level;       // in liters
    bool is_active;
    std::string last_refill_date;
};

class ChemicalTreatment {
private:
    std::unordered_map<ChemicalType, ChemicalDose> chemicals;
    std::vector<std::pair<std::string, std::string>> treatment_log;
    
    static constexpr double REORDER_THRESHOLD = 0.2; // 20% tank level
    static constexpr double MAX_DOSE_ADJUSTMENT = 1.5; // 50% adjustment

public:
    ChemicalTreatment();
    
    // Chemical management
    void addChemical(ChemicalType type, const std::string& name, 
                    double tank_capacity, double dose_amount);
    void updateChemicalDose(ChemicalType type, double dose_amount);
    
    // Level monitoring
    double getChemicalLevel(ChemicalType type) const;
    void refillChemical(ChemicalType type, double amount);
    bool isChemicalLow(ChemicalType type) const;
    
    // Treatment operations
    void activateTreatment(ChemicalType type);
    void deactivateTreatment(ChemicalType type);
    bool isTreatmentActive(ChemicalType type) const;
    
    // Adjustment
    void adjustDoseForPH(double current_ph, double target_ph);
    void adjustDoseForTurbidity(double current_turbidity, double target_turbidity);
    
    // Reporting
    void logTreatmentAction(const std::string& action, const std::string& details);
    std::string getTreatmentReport() const;
    void printChemicalStatus() const;
};

#endif // CHEMICAL_TREATMENT_H

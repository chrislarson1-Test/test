#include "ChemicalTreatment.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>

ChemicalTreatment::ChemicalTreatment() {}

void ChemicalTreatment::addChemical(ChemicalType type, const std::string& name,
                                    double tank_capacity, double dose_amount) {
    ChemicalDose dose;
    dose.type = type;
    dose.chemical_name = name;
    dose.tank_capacity = tank_capacity;
    dose.current_level = tank_capacity;
    dose.dose_amount = dose_amount;
    dose.is_active = false;
    dose.last_refill_date = "2026-04-01";
    
    chemicals[type] = dose;
    std::cout << "Chemical " << name << " added to the system." << std::endl;
}

void ChemicalTreatment::updateChemicalDose(ChemicalType type, double dose_amount) {
    auto it = chemicals.find(type);
    if (it != chemicals.end()) {
        double adjustment = dose_amount / it->second.dose_amount;
        if (adjustment >= 1.0 / MAX_DOSE_ADJUSTMENT && adjustment <= MAX_DOSE_ADJUSTMENT) {
            it->second.dose_amount = dose_amount;
            logTreatmentAction("DOSE_UPDATE", 
                             "Updated dose to " + std::to_string(dose_amount) + " mg/L");
        }
    }
}

double ChemicalTreatment::getChemicalLevel(ChemicalType type) const {
    auto it = chemicals.find(type);
    if (it != chemicals.end()) {
        return it->second.current_level;
    }
    return 0.0;
}

void ChemicalTreatment::refillChemical(ChemicalType type, double amount) {
    auto it = chemicals.find(type);
    if (it != chemicals.end()) {
        it->second.current_level = std::min(it->second.tank_capacity,
                                           it->second.current_level + amount);
        logTreatmentAction("REFILL", 
                         "Refilled " + it->second.chemical_name + " with " + 
                         std::to_string(amount) + " liters");
        std::cout << "Chemical refilled successfully." << std::endl;
    }
}

bool ChemicalTreatment::isChemicalLow(ChemicalType type) const {
    auto it = chemicals.find(type);
    if (it != chemicals.end()) {
        double threshold = it->second.tank_capacity * REORDER_THRESHOLD;
        return it->second.current_level < threshold;
    }
    return false;
}

void ChemicalTreatment::activateTreatment(ChemicalType type) {
    auto it = chemicals.find(type);
    if (it != chemicals.end()) {
        it->second.is_active = true;
        logTreatmentAction("ACTIVATED", it->second.chemical_name + " treatment activated");
        std::cout << it->second.chemical_name << " treatment activated." << std::endl;
    }
}

void ChemicalTreatment::deactivateTreatment(ChemicalType type) {
    auto it = chemicals.find(type);
    if (it != chemicals.end()) {
        it->second.is_active = false;
        logTreatmentAction("DEACTIVATED", it->second.chemical_name + " treatment deactivated");
        std::cout << it->second.chemical_name << " treatment deactivated." << std::endl;
    }
}

bool ChemicalTreatment::isTreatmentActive(ChemicalType type) const {
    auto it = chemicals.find(type);
    if (it != chemicals.end()) {
        return it->second.is_active;
    }
    return false;
}

void ChemicalTreatment::adjustDoseForPH(double current_ph, double target_ph) {
    double ph_difference = target_ph - current_ph;
    double adjustment_factor = 1.0 + (ph_difference * 0.1);
    
    adjustment_factor = std::max(1.0 / MAX_DOSE_ADJUSTMENT, 
                                std::min(MAX_DOSE_ADJUSTMENT, adjustment_factor));
    
    auto it = chemicals.find(ChemicalType::PH_ADJUSTER);
    if (it != chemicals.end()) {
        double old_dose = it->second.dose_amount;
        it->second.dose_amount *= adjustment_factor;
        logTreatmentAction("PH_ADJUSTMENT", 
                         "pH adjustment dose changed from " + std::to_string(old_dose) +
                         " to " + std::to_string(it->second.dose_amount) + " mg/L");
    }
}

void ChemicalTreatment::adjustDoseForTurbidity(double current_turbidity, double target_turbidity) {
    double turbidity_difference = current_turbidity - target_turbidity;
    double adjustment_factor = 1.0 + (turbidity_difference * 0.05);
    
    adjustment_factor = std::max(1.0 / MAX_DOSE_ADJUSTMENT,
                                std::min(MAX_DOSE_ADJUSTMENT, adjustment_factor));
    
    auto it = chemicals.find(ChemicalType::COAGULANT);
    if (it != chemicals.end()) {
        double old_dose = it->second.dose_amount;
        it->second.dose_amount *= adjustment_factor;
        logTreatmentAction("TURBIDITY_ADJUSTMENT",
                         "Coagulant dose changed from " + std::to_string(old_dose) +
                         " to " + std::to_string(it->second.dose_amount) + " mg/L");
    }
}

void ChemicalTreatment::logTreatmentAction(const std::string& action, const std::string& details) {
    treatment_log.push_back({action, details});
}

std::string ChemicalTreatment::getTreatmentReport() const {
    std::ostringstream report;
    report << "\n=== CHEMICAL TREATMENT REPORT ===" << std::endl;
    report << "Recent Actions:" << std::endl;
    
    int start = std::max(0, static_cast<int>(treatment_log.size()) - 10);
    for (size_t i = start; i < treatment_log.size(); ++i) {
        report << "  [" << treatment_log[i].first << "] " 
               << treatment_log[i].second << std::endl;
    }
    
    report << std::string(35, '=') << std::endl;
    return report.str();
}

void ChemicalTreatment::printChemicalStatus() const {
    std::cout << "\n=== CHEMICAL TREATMENT STATUS ===" << std::endl;
    std::cout << std::left << std::setw(20) << "Chemical Name"
              << std::setw(15) << "Current Level"
              << std::setw(15) << "Tank Capacity"
              << std::setw(12) << "Dose (mg/L)"
              << "Active" << std::endl;
    std::cout << std::string(75, '-') << std::endl;
    
    for (const auto& pair : chemicals) {
        const ChemicalDose& dose = pair.second;
        std::cout << std::left << std::setw(20) << dose.chemical_name
                  << std::setw(15) << std::fixed << std::setprecision(2) << dose.current_level
                  << std::setw(15) << dose.tank_capacity
                  << std::setw(12) << dose.dose_amount
                  << (dose.is_active ? "YES" : "NO");
        
        if (isChemicalLow(pair.first)) {
            std::cout << " [LOW LEVEL]";
        }
        std::cout << std::endl;
    }
    std::cout << std::string(75, '-') << std::endl;
}

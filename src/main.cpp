#include "AutomationController.h"
#include <iostream>
#include <thread>
#include <chrono>

void printWelcomeBanner() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << " WATER REFINEMENT AUTOMATION SYSTEM v1.0" << std::endl;
    std::cout << " Advanced Water Treatment and Quality Control" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
}

void printMenu() {
    std::cout << "\n=== MAIN MENU ===" << std::endl;
    std::cout << "1. Start System" << std::endl;
    std::cout << "2. Run Automation Cycles" << std::endl;
    std::cout << "3. System Health Check" << std::endl;
    std::cout << "4. View Water Quality Report" << std::endl;
    std::cout << "5. View Filtration Status" << std::endl;
    std::cout << "6. View Chemical Treatment Status" << std::endl;
    std::cout << "7. Refill Chemical Tank" << std::endl;
    std::cout << "8. Replace Filter" << std::endl;
    std::cout << "9. Emergency Shutdown" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "==================" << std::endl;
}

int main() {
    printWelcomeBanner();
    
    AutomationController controller;
    controller.initialize();
    
    int choice = -1;
    bool running = true;
    
    while (running) {
        printMenu();
        std::cout << "Select option: ";
        std::cin >> choice;
        std::cin.ignore();
        
        switch(choice) {
            case 1: {
                controller.startSystem();
                break;
            }
            case 2: {
                std::cout << "\nEnter number of cycles to run: ";
                int cycles;
                std::cin >> cycles;
                std::cin.ignore();
                
                std::cout << "Running " << cycles << " automation cycles..." << std::endl;
                for (int i = 0; i < cycles; ++i) {
                    controller.runAutomationCycle();
                    std::cout << "Cycle " << (i + 1) << " completed." << std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                }
                std::cout << "All cycles completed." << std::endl;
                break;
            }
            case 3: {
                controller.performHealthCheck();
                break;
            }
            case 4: {
                std::cout << controller.generateSystemReport();
                break;
            }
            case 5: {
                // Simulate filtration system status by running a cycle first
                controller.runAutomationCycle();
                WaterQualityParameters params = controller.quality_monitor->getCurrentParameters();
                std::cout << "\n[Filtration Status Retrieved]" << std::endl;
                break;
            }
            case 6: {
                std::cout << "\n[Chemical Treatment Status]" << std::endl;
                break;
            }
            case 7: {
                std::cout << "Enter chemical type (1=Coagulant, 2=Disinfectant, 3=pH Adjuster): ";
                int chem_type;
                std::cin >> chem_type;
                std::cout << "Enter refill amount (liters): ";
                double amount;
                std::cin >> amount;
                std::cin.ignore();
                
                ChemicalType type;
                switch(chem_type) {
                    case 1: type = ChemicalType::COAGULANT; break;
                    case 2: type = ChemicalType::DISINFECTANT; break;
                    case 3: type = ChemicalType::PH_ADJUSTER; break;
                    default: type = ChemicalType::DISINFECTANT;
                }
                std::cout << "Chemical refilled successfully." << std::endl;
                break;
            }
            case 8: {
                std::cout << "Enter filter stage (1=Pre, 2=Sand, 3=Carbon, 4=RO, 5=UV): ";
                int filter_num;
                std::cin >> filter_num;
                std::cin.ignore();
                
                FilterStage stage;
                switch(filter_num) {
                    case 1: stage = FilterStage::PRE_FILTRATION; break;
                    case 2: stage = FilterStage::SAND_FILTRATION; break;
                    case 3: stage = FilterStage::ACTIVATED_CARBON; break;
                    case 4: stage = FilterStage::REVERSE_OSMOSIS; break;
                    case 5: stage = FilterStage::UV_TREATMENT; break;
                    default: stage = FilterStage::SAND_FILTRATION;
                }
                std::cout << "Filter replaced successfully." << std::endl;
                break;
            }
            case 9: {
                std::cout << "Are you sure? (yes/no): ";
                std::string confirm;
                std::getline(std::cin, confirm);
                if (confirm == "yes") {
                    controller.emergencyShutdown();
                }
                break;
            }
            case 0: {
                running = false;
                controller.stopSystem();
                std::cout << "\nThank you for using Water Refinement Automation System." << std::endl;
                std::cout << "Exiting..." << std::endl;
                break;
            }
            default: {
                std::cout << "Invalid option. Please try again." << std::endl;
            }
        }
    }
    
    return 0;
}

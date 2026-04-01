cmake_minimum_required(VERSION 3.10)
project(WaterRefinementAutomation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main executable
add_executable(water_refinement
    src/main.cpp
    src/SensorManager.cpp
    src/FiltrationSystem.cpp
    src/ChemicalTreatment.cpp
    src/QualityMonitor.cpp
    src/AutomationController.cpp
)

# Include directories
target_include_directories(water_refinement PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Compiler flags
if(MSVC)
    target_compile_options(water_refinement PRIVATE /W4)
else()
    target_compile_options(water_refinement PRIVATE -Wall -Wextra -Wpedantic)
endif()

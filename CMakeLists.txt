cmake_minimum_required(VERSION 3.16)
project(fitness-tracker VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts)

# Set up Qt MOC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Source files
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/datenavigationbar.cpp
    src/journalcontentarea.cpp
    src/datemanager.cpp
    src/bodycompositionform.cpp
    src/bodycompositionview.cpp
    src/workoutbuilder.cpp
    src/workoutview.cpp
    src/exerciselibrary.cpp
    src/analyticstab.cpp
    src/models/bodycomposition.cpp
    src/models/datamanager.cpp
    src/models/exercise.cpp
    src/models/setdata.cpp
    src/models/workout.cpp
    src/utils/calculations.cpp
)

set(HEADERS
    src/mainwindow.h
    src/datenavigationbar.h
    src/journalcontentarea.h
    src/datemanager.h
    src/bodycompositionform.h
    src/bodycompositionview.h
    src/workoutbuilder.h
    src/workoutview.h
    src/exerciselibrary.h
    src/analyticstab.h
    src/models/bodycomposition.h
    src/models/datamanager.h
    src/models/exercise.h
    src/models/setdata.h
    src/models/workout.h
    src/utils/calculations.h
)

# Create executable
add_executable(fitness-tracker ${SOURCES} ${HEADERS})

# Link Qt libraries
target_link_libraries(fitness-tracker Qt6::Core Qt6::Widgets Qt6::Charts)

# Set output directory
set_target_properties(fitness-tracker PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Release-specific optimizations
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Enable link-time optimization
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
    
    # Strip debug symbols for smaller executable
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
    
    # Additional release flags
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")
endif()

# Set application properties
set_target_properties(fitness-tracker PROPERTIES
    OUTPUT_NAME "fitness-tracker"
    VERSION ${PROJECT_VERSION}
) 
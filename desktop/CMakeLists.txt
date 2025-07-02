cmake_minimum_required(VERSION 3.16)
project(fitness-tracker-desktop VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

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
    src/models/bodycomposition.cpp
)

set(HEADERS
    src/mainwindow.h
    src/datenavigationbar.h
    src/journalcontentarea.h
    src/datemanager.h
    src/bodycompositionform.h
    src/models/bodycomposition.h
)

# Create executable
add_executable(fitness-tracker-desktop ${SOURCES} ${HEADERS})

# Link Qt libraries
target_link_libraries(fitness-tracker-desktop Qt6::Core Qt6::Widgets)

# Set output directory
set_target_properties(fitness-tracker-desktop PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
) 
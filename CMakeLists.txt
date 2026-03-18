cmake_minimum_required(VERSION 3.16)

# FIXED: Added "C" to languages so it knows how to handle .c files
project(StudentManager VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts)

# The list of all your files
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        etudiant.c
        etudiant.h
        file.c
        file.h
        utilities.c
        utilities.h
)

# Tell Qt to create the app using all your sources
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(StudentManager
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(StudentManager ${PROJECT_SOURCES})
endif()

# Link the Qt graphics libraries
target_link_libraries(StudentManager PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Charts)

# Set properties for Windows/Mac
set_target_properties(StudentManager PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(StudentManager)
endif()
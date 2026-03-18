cmake_minimum_required(VERSION 3.16)

project(StudentManager VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ADDED: PrintSupport to the components list
find_package(Qt6 REQUIRED COMPONENTS Widgets Charts PrintSupport)

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

qt_add_executable(StudentManager
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

# ADDED: Qt6::PrintSupport to the link libraries
target_link_libraries(StudentManager PRIVATE
    Qt6::Widgets
    Qt6::Charts
    Qt6::PrintSupport
)

set_target_properties(StudentManager PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

qt_finalize_executable(StudentManager)
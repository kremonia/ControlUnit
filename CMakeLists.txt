cmake_minimum_required(VERSION 3.26)
project(ControlUnit)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

set(SOURCES
        src/CaCRC.cpp
        src/Camera.cpp
        src/Control.cpp
        src/CTest.cpp
        src/DLPMega.cpp
        src/Focus.cpp
        src/Global.cpp
        src/InputSwitch.cpp
        src/Joy.cpp
        src/Key.cpp
        src/Keybord.cpp
        src/Keypad.cpp
        src/Marker.cpp
        src/Motor.cpp
        src/Objective.cpp
        src/Scan.cpp
        src/TransferRs485.cpp
        src/TransferUsb.cpp
        src/Trinamic_TMC5130.cpp
        src/Vacuum.cpp
        main.cpp)

add_executable(ControlUnit ${SOURCES})

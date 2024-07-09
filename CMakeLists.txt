cmake_minimum_required(VERSION 3.28)
project(WLEDControllerCPP)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 17)
# Source files
set(SOURCES
        main.cpp
        # Add all ALGLIB source files here
        D:/outilsJetBrain/CLion/libraries/alglib-cpp/src/alglibinternal.cpp
        D:/outilsJetBrain/CLion/libraries/alglib-cpp/src/alglibmisc.cpp
        D:/outilsJetBrain/CLion/libraries/alglib-cpp/src/ap.cpp
        D:/outilsJetBrain/CLion/libraries/alglib-cpp/src/dataanalysis.cpp
        D:/outilsJetBrain/CLion/libraries/alglib-cpp/src/diffequations.cpp
        D:/outilsJetBrain/CLion/libraries/alglib-cpp/src/fasttransforms.cpp
        D:/outilsJetBrain/CLion/libraries/alglib-cpp/src/integration.cpp
        D:/outilsJetBrain/CLion/libraries/alglib-cpp/src/interpolation.cpp
        D:/outilsJetBrain/CLion/libraries/alglib-cpp/src/linalg.cpp
        D:/outilsJetBrain/CLion/libraries/alglib-cpp/src/optimization.cpp
        D:/outilsJetBrain/CLion/libraries/alglib-cpp/src/solvers.cpp
        D:/outilsJetBrain/CLion/libraries/alglib-cpp/src/specialfunctions.cpp
        D:/outilsJetBrain/CLion/libraries/alglib-cpp/src/statistics.cpp
)

add_executable(WLEDControllerCPP ${SOURCES})

target_link_libraries(WLEDControllerCPP
        ws2_32
        kernel32
        user32
        gdi32
        winspool
        shell32
        ole32
        oleaut32
        uuid
        comdlg32
        advapi32)

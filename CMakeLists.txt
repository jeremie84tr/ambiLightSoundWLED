cmake_minimum_required(VERSION 3.28)
project(WLEDControllerCPP)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 17)
# Source files
set(SOURCES
        main.cpp
        # Add all ALGLIB source files here
        arglib/src/alglibinternal.cpp
        arglib/src/alglibmisc.cpp
        arglib/src/ap.cpp
        arglib/src/dataanalysis.cpp
        arglib/src/diffequations.cpp
        arglib/src/fasttransforms.cpp
        arglib/src/integration.cpp
        arglib/src/interpolation.cpp
        arglib/src/linalg.cpp
        arglib/src/specialfunctions.cpp
        arglib/src/optimization.cpp
        arglib/src/solvers.cpp
        arglib/src/statistics.cpp
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

cmake_minimum_required(VERSION 3.28)
project(WLEDControllerCPP)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Activer la liaison statique pour libstdc++ et libgcc
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc -static")

# Spécifiez l'architecture cible
set(CMAKE_GENERATOR_PLATFORM x64)

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

# Lier les bibliothèques nécessaires
target_link_libraries(WLEDControllerCPP
        -static
        -static-libgcc
        -static-libstdc++
        -static-libgcc
        -lpthread
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

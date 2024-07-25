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
        alglib/src/alglibinternal.cpp
        alglib/src/alglibmisc.cpp
        alglib/src/ap.cpp
        alglib/src/dataanalysis.cpp
        alglib/src/diffequations.cpp
        alglib/src/fasttransforms.cpp
        alglib/src/integration.cpp
        alglib/src/interpolation.cpp
        alglib/src/linalg.cpp
        alglib/src/specialfunctions.cpp
        alglib/src/optimization.cpp
        alglib/src/solvers.cpp
        alglib/src/statistics.cpp
        ambiLight/Ambilight.cpp
        ambiLight/Ambilight.h
        soundLight/SoundLight.h
        soundLight/SoundLight.cpp
        soundLight/SoundLightColor.h
        soundLight/SoundLightColor.cpp
        parseur/ParserOption.h
        parseur/ParserOption.cpp
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

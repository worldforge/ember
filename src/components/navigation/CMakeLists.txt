add_library(navigation OBJECT
        Awareness.cpp fastlz.c Steering.cpp Loitering.cpp AwarenessUtils.h)

target_include_directories(navigation PUBLIC
        ${CMAKE_SOURCE_DIR}/external/RecastDetour/DetourTileCache/Include
        ${CMAKE_SOURCE_DIR}/external/RecastDetour/Detour/Include)

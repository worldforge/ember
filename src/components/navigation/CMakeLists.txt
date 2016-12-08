add_library(navigation
        Awareness.cpp fastlz.c Steering.cpp Loitering.cpp)
add_subdirectory(external/RecastDetour/Detour)
add_subdirectory(external/RecastDetour/DetourTileCache)
add_subdirectory(external/RecastDetour/Recast)

target_link_libraries(navigation DetourTileCache Detour Recast)
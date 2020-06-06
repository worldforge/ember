add_library(navigation OBJECT
        Awareness.cpp fastlz.c Steering.cpp Loitering.cpp AwarenessUtils.h)

target_link_libraries(navigation PUBLIC DetourTileCache Detour Recast)
add_library(navigation
        Awareness.cpp fastlz.c Steering.cpp Loitering.cpp AwarenessUtils.h)

target_link_libraries(navigation DetourTileCache Detour Recast)
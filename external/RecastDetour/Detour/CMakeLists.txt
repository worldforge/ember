add_library(Detour
        Source/DetourAlloc.cpp
        Source/DetourCommon.cpp
        Source/DetourNavMesh.cpp
        Source/DetourNavMeshBuilder.cpp
        Source/DetourNavMeshQuery.cpp
        Source/DetourNode.cpp
        Source/DetourObstacleAvoidance.cpp)

target_include_directories(Detour PUBLIC Include)

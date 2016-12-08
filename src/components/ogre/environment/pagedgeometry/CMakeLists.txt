add_library(pagedgeometry
        source/BatchPage.cpp source/BatchedGeometry.cpp
        source/GrassLoader.cpp source/ImpostorPage.cpp
        source/PagedGeometry.cpp source/PropertyMaps.cpp source/StaticBillboardSet.cpp
        source/TreeLoader2D.cpp source/TreeLoader3D.cpp source/DummyPage.cpp source/PassiveEntityPage.cpp source/WindBatchedGeometry.cpp source/WindBatchPage.cpp
        source/ShaderHelper.cpp)

target_include_directories(pagedgeometry PUBLIC include)


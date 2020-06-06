add_library(pagedgeometry OBJECT
        source/BatchPage.cpp 
        source/BatchedGeometry.cpp
        source/GrassLoader.cpp 
        source/ImpostorPage.cpp
        source/PagedGeometry.cpp
        source/PropertyMaps.cpp
        source/StaticBillboardSet.cpp
        source/TreeLoader2D.cpp
        source/TreeLoader3D.cpp 
        source/DummyPage.cpp 
        source/PassiveEntityPage.cpp
        source/WindBatchedGeometry.cpp
        source/WindBatchPage.cpp
        source/ShaderHelper.cpp
        
        include/BatchPage.h
        include/BatchedGeometry.h
        include/GrassLoader.h
        include/ImpostorPage.h
        include/PagedGeometry.h
        include/PropertyMaps.h
        include/StaticBillboardSet.h
        include/TreeLoader2D.h
        include/TreeLoader3D.h
        include/DummyPage.h
        include/PassiveEntityPage.h
        include/WindBatchedGeometry.h
        include/WindBatchPage.h
        include/ShaderHelper.h
        include/RandomTable.h)

target_include_directories(pagedgeometry PUBLIC include)


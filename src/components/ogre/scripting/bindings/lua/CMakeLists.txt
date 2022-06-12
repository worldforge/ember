add_subdirectory(ogre)
add_library(BindingsEmberOgre OBJECT
        BindingsEmberOgre.cpp
        helpers/OgreUtils.cpp)

target_precompile_headers(BindingsEmberOgre
        REUSE_FROM
        BindingsLua)
target_precompile_headers(BindingsEmberOgre
        REUSE_FROM
        emberogre)
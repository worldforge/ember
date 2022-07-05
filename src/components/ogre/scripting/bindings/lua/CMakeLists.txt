add_subdirectory(ogre)

file(GLOB BINDINGS_FILES *.cpp)

add_library(BindingsEmberOgre OBJECT
        ${BINDINGS_FILES}
        helpers/OgreUtils.cpp)

target_precompile_headers(BindingsEmberOgre
        REUSE_FROM
        BindingsLua)
target_precompile_headers(BindingsEmberOgre
        REUSE_FROM
        emberogre)
add_subdirectory(ogre)

file(GLOB BINDINGS_FILES *.cpp)

add_library(BindingsEmberOgre OBJECT
        ${BINDINGS_FILES}
        helpers/OgreUtils.cpp)

if (MSVC)
    #Need to allow for a lot of symbols when compiling the bindings on MSVC.
    target_compile_options(BindingsEmberOgre PRIVATE "/bigobj")
endif ()

target_precompile_headers(BindingsEmberOgre
        REUSE_FROM
        BindingsLua)
target_precompile_headers(BindingsEmberOgre
        REUSE_FROM
        emberogre)
add_library(BindingsOgre OBJECT
        BindingsOgre.cpp)
target_precompile_headers(BindingsOgre
        REUSE_FROM
        BindingsLua)
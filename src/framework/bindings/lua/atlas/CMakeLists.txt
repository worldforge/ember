add_library(BindingsAtlas OBJECT
        BindingsAtlas.cpp)
target_precompile_headers(BindingsAtlas
        REUSE_FROM
        BindingsLua)
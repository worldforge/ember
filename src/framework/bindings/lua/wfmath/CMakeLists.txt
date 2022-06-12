add_library(BindingsWFMath OBJECT
        BindingsWFMath.cpp)
target_precompile_headers(BindingsWFMath
        REUSE_FROM
        BindingsLua)
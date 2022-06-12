add_library(BindingsVarconf OBJECT
        BindingsVarconf.cpp)
target_precompile_headers(BindingsVarconf
        REUSE_FROM
        BindingsLua)
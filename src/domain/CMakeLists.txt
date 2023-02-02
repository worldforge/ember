add_library(domain
        EmberEntity.cpp
        EntityTalk.cpp
        EmberEntityRef.cpp
        )


add_library(BindingsDomain OBJECT
        bindings/lua/BindingsDomain.cpp)
if (MSVC)
    #Need to allow for a lot of symbols when compiling the bindings on MSVC.
    target_compile_options(BindingsDomain PRIVATE "/bigobj")
endif ()

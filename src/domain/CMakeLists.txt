add_library(domain OBJECT
        EmberEntity.cpp
        EntityTalk.cpp
        IHeightProvider.h
        IEntityControlDelegate.h
        IEntityAttachment.h
        IEntityVisitor.h
        IVisualizable.h
        Types.h
        IGraphicalRepresentation.h EmberEntityRef.cpp EmberEntityRef.h)


add_library(BindingsDomain OBJECT
        bindings/lua/BindingsDomain.cpp)
if (MSVC)
    #Need to allow for a lot of symbols when compiling the bindings on MSVC.
    target_compile_options(BindingsDomain "/bigobj")
endif ()

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


wf_generate_lua_bindings(bindings/lua/Domain)



add_library(domain
        EmberEntity.cpp
        EntityTalk.cpp
        IHeightProvider.h
        IEntityControlDelegate.h
        IEntityAttachment.h
        IEntityVisitor.h
        IVisualizable.h
        Types.h
        IGraphicalRepresentation.h)


wf_generate_lua_bindings(bindings/lua/Domain)



add_library(cegui
        CEGUILogger.cpp
        ColouredRenderedStringParser.cpp
        RenderedColourStringTextComponent.cpp
        SDLNativeClipboardProvider.cpp)


wf_generate_lua_bindings(bindings/lua/Cegui)

install(DIRECTORY datafiles DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/ember/gui/cegui)


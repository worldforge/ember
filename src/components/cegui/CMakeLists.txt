add_library(cegui OBJECT
        CEGUILogger.cpp
        ColouredRenderedStringParser.cpp
        RenderedColourStringTextComponent.cpp
        SDLNativeClipboardProvider.cpp CEGUISetup.cpp CEGUISetup.h)


wf_generate_lua_bindings(bindings/lua/Cegui)

install(DIRECTORY datafiles DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/ember/gui/cegui)
install(DIRECTORY schemas/ DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/ember/gui/)


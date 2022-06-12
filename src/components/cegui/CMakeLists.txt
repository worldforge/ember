add_library(cegui OBJECT
        CEGUILogger.cpp
        ColouredRenderedStringParser.cpp
        RenderedColourStringTextComponent.cpp
        SDLNativeClipboardProvider.cpp
        CEGUISetup.cpp)


add_library(BindingsCEGUI OBJECT
        bindings/lua/BindingsCEGUI.cpp)
target_precompile_headers(BindingsCEGUI
        REUSE_FROM
        BindingsLua)

install(DIRECTORY datafiles DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/ember/gui/cegui)
install(DIRECTORY schemas DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/ember/gui)


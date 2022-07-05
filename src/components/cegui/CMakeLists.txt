add_library(cegui OBJECT
        CEGUILogger.cpp
        ColouredRenderedStringParser.cpp
        RenderedColourStringTextComponent.cpp
        SDLNativeClipboardProvider.cpp
        CEGUISetup.cpp)



file(GLOB CEGUI_FILES bindings/lua/*.cpp)

add_library(BindingsCEGUI OBJECT
        ${CEGUI_FILES})

target_precompile_headers(BindingsCEGUI
        PUBLIC
        <CEGUI/CEGUI.h>
        <sol2/sol.hpp>
        )

install(DIRECTORY datafiles DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/ember/gui/cegui)
install(DIRECTORY schemas DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/ember/gui)


add_library(services OBJECT
        config/ConfigListener.cpp
        config/ConfigListenerContainer.cpp
        config/ConfigService.cpp
        config/ConfigConsoleCommands.cpp
        input/Input.cpp
        input/InputCommandMapper.cpp
        input/EmberIcon.h
        input/IInputAdapter.h
        logging/ErisLogReciever.cpp
        metaserver/MetaserverService.cpp
        scripting/ScriptingService.cpp
        server/AccountAvailableState.cpp
        server/AvatarTransferInfo.cpp
        server/ConnectedState.cpp
        server/ConnectingState.cpp
        server/Connection.cpp
        server/EnteredWorldState.cpp
        server/LocalServerAdminCreator.cpp
        server/LoggedInState.cpp
        server/NonConnectedState.cpp
        server/OOGChat.cpp
        server/ServerService.cpp
        server/ServerServiceConnectionListener.cpp
        server/TransferInfoStringSerializer.cpp
        server/IConnectionListener.h
        server/IState.h
        server/ServerServiceSignals.h
        server/StateBase.h
        server/AssetsSyncState.cpp
        serversettings/ServerSettings.cpp
        serversettings/ServerSettingsCredentials.cpp
        sound/SoundBinding.cpp
        sound/SoundGeneral.cpp
        sound/SoundInstance.cpp
        sound/SoundSample.cpp
        sound/SoundService.cpp
        sound/SoundSource.cpp
        EmberServices.cpp)

target_link_libraries(services PUBLIC mojoal)


file(GLOB BINDINGS_FILES bindings/lua/*.cpp)

add_library(BindingsServices OBJECT
        ${BINDINGS_FILES})

target_precompile_headers(BindingsServices
        REUSE_FROM
        BindingsLua)

if (MSVC)
    #Need to allow for a lot of symbols when compiling the bindings on MSVC.
    target_compile_options(BindingsServices PRIVATE "/bigobj")
endif ()

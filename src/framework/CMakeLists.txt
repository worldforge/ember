add_library(framework
        AttributeObserver.cpp ConsoleBackend.cpp ConsoleCommandWrapper.cpp
        DeepAttributeObserver.cpp DirectAttributeObserver.cpp Exception.cpp Log.cpp LoggingInstance.cpp StreamLogObserver.cpp
        Tokeniser.cpp ../../external/binreloc/binreloc.cpp TimedLog.cpp TimeHelper.cpp Service.cpp TimeFrame.cpp
        CommandHistory.cpp MainLoopController.cpp FileResourceProvider.cpp EntityExporterBase.cpp EntityExporter.cpp EntityImporterBase.cpp EntityImporter.cpp AtlasMessageLoader.cpp TinyXmlCodec.cpp
        AtlasObjectDecoder.cpp
        tasks/TaskExecutor.cpp
        tasks/TaskExecutionContext.cpp
        tasks/TaskQueue.cpp
        tasks/TaskUnit.cpp
        tasks/SerialTask.cpp
        FileSystemObserver.cpp
        StackChecker.cpp
        )

target_link_libraries(framework ember_tinyxml binreloc)

wf_generate_lua_bindings(bindings/lua/Framework)
wf_generate_lua_bindings(bindings/lua/atlas/Atlas)
wf_generate_lua_bindings(bindings/lua/eris/Eris)
wf_generate_lua_bindings(bindings/lua/varconf/Varconf)



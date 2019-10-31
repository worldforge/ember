add_library(framework
        AttributeObserver.cpp ConsoleBackend.cpp ConsoleCommandWrapper.cpp
        DeepAttributeObserver.cpp DirectAttributeObserver.cpp Exception.cpp Log.cpp LoggingInstance.cpp StreamLogObserver.cpp
        Tokeniser.cpp binreloc.cpp TimedLog.cpp TimeHelper.cpp Service.cpp TimeFrame.cpp
        CommandHistory.cpp MainLoopController.cpp FileResourceProvider.cpp EntityExporterBase.cpp EntityExporter.cpp EntityImporterBase.cpp EntityImporter.cpp AtlasMessageLoader.cpp TinyXmlCodec.cpp
        AtlasObjectDecoder.cpp
        tasks/TaskExecutor.cpp
        tasks/TaskExecutionContext.cpp
        tasks/TaskQueue.cpp
        tasks/TaskUnit.cpp
        tasks/SerialTask.cpp
        tasks/ITask.h
        tasks/ITaskExecutionListener.h
        tasks/TemplateNamedTask.h
        FileSystemObserver.cpp FileSystemObserver.h

        ConsoleObject.h IResourceProvider.h IScriptingProvider.h LogObserver.h osdir.h ShutdownException.h Singleton.h utf8.h
        AtlasQuery.h

        StackChecker.cpp)

target_link_libraries(framework ember_tinyxml)

wf_generate_lua_bindings(bindings/lua/Framework)
wf_generate_lua_bindings(bindings/lua/atlas/Atlas)
wf_generate_lua_bindings(bindings/lua/eris/Eris)
wf_generate_lua_bindings(bindings/lua/varconf/Varconf)



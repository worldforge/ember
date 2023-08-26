add_library(framework OBJECT
        AttributeObserver.cpp ConsoleBackend.cpp ConsoleCommandWrapper.cpp
        DeepAttributeObserver.cpp DirectAttributeObserver.cpp Log.cpp LoggingInstance.cpp StreamLogObserver.cpp
        Tokeniser.cpp TimedLog.cpp TimeHelper.cpp TimeFrame.cpp
        CommandHistory.cpp MainLoopController.cpp FileResourceProvider.cpp EntityExporterBase.cpp EntityExporter.cpp EntityImporterBase.cpp EntityImporter.cpp AtlasMessageLoader.cpp TinyXmlCodec.cpp
        AtlasObjectDecoder.cpp
        tasks/TaskExecutor.cpp
        tasks/TaskExecutionContext.cpp
        tasks/TaskQueue.cpp
        tasks/TaskUnit.cpp
        tasks/SerialTask.cpp
        tasks/FunctionalTask.cpp
        FileSystemObserver.cpp
        StackChecker.cpp
        )
add_subdirectory(bindings)

#wf_generate_lua_bindings(bindings/lua/wfmath/WFMath)



add_library(framework
        AttributeObserver.cpp ConsoleBackend.cpp ConsoleCommandWrapper.cpp
        DeepAttributeObserver.cpp DirectAttributeObserver.cpp Exception.cpp Log.cpp LoggingInstance.cpp StreamLogObserver.cpp
        Tokeniser.cpp XMLCodec.cpp binreloc.cpp TimedLog.cpp Time.cpp Service.cpp TimeFrame.cpp
        CommandHistory.cpp MainLoopController.cpp FileResourceProvider.cpp EntityExporterBase.cpp EntityExporter.cpp EntityImporterBase.cpp EntityImporter.cpp AtlasMessageLoader.cpp TinyXmlCodec.cpp
        AtlasObjectDecoder.cpp
        tasks/TaskExecutor.cpp tasks/TaskExecutionContext.cpp tasks/TaskQueue.cpp tasks/TaskUnit.cpp tasks/SerialTask.cpp
        tinyxml/tinystr.cpp tinyxml/tinyxml.cpp tinyxml/tinyxmlerror.cpp tinyxml/tinyxmlparser.cpp)


wf_generate_lua_bindings(bindings/lua/Framework)
wf_generate_lua_bindings(bindings/lua/atlas/Atlas)
wf_generate_lua_bindings(bindings/lua/eris/Eris)
wf_generate_lua_bindings(bindings/lua/varconf/Varconf)



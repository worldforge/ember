add_library(framework
        AttributeObserver.cpp ConsoleBackend.cpp ConsoleCommandWrapper.cpp
        DeepAttributeObserver.cpp DirectAttributeObserver.cpp Exception.cpp Log.cpp LoggingInstance.cpp StreamLogObserver.cpp
        Tokeniser.cpp XMLCodec.cpp binreloc.cpp TimedLog.cpp TimeHelper.cpp Service.cpp TimeFrame.cpp
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
        tinyxml/tinystr.cpp tinyxml/tinyxml.cpp tinyxml/tinyxmlerror.cpp tinyxml/tinyxmlparser.cpp FileSystemObserver.cpp FileSystemObserver.h
        dir_monitor/include/dir_monitor/basic_dir_monitor.hpp dir_monitor/include/dir_monitor/dir_monitor.hpp
        dir_monitor/include/dir_monitor/inotify/basic_dir_monitor_service.hpp dir_monitor/include/dir_monitor/inotify/dir_monitor_impl.hpp
        dir_monitor/include/dir_monitor/fsevents/basic_dir_monitor_service.hpp dir_monitor/include/dir_monitor/fsevents/dir_monitor_impl.hpp
        dir_monitor/include/dir_monitor/kqueue/basic_dir_monitor_service.hpp dir_monitor/include/dir_monitor/kqueue/dir_monitor_impl.hpp
        dir_monitor/include/dir_monitor/windows/basic_dir_monitor_service.hpp dir_monitor/include/dir_monitor/windows/dir_monitor_impl.hpp

        ConsoleObject.h IGameView.h IResourceProvider.h IScriptingProvider.h LogObserver.h osdir.h ShutdownException.h Singleton.h utf8.h
        )


wf_generate_lua_bindings(bindings/lua/Framework)
wf_generate_lua_bindings(bindings/lua/atlas/Atlas)
wf_generate_lua_bindings(bindings/lua/eris/Eris)
wf_generate_lua_bindings(bindings/lua/varconf/Varconf)



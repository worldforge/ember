if (CPPUNIT_FOUND)

    MESSAGE(STATUS "Building tests.")

    add_executable(TestOgreView TestOgreView.cpp ConvertTestCase.cpp ModelMountTestCase.cpp)
    target_compile_definitions(TestOgreView PUBLIC -DLOG_TASKS)
    target_link_libraries(TestOgreView ${CPPUNIT_LIBRARIES} emberogre entitymapping framework)
    target_include_directories(TestOgreView PUBLIC ${CPPUNIT_INCLUDE_DIRS})
    add_test(NAME TestOgreView COMMAND TestOgreView)
    add_dependencies(check TestOgreView)

    add_executable(TestTasks TestTasks.cpp)
    target_compile_definitions(TestTasks PUBLIC -DLOG_TASKS)
    target_link_libraries(TestTasks ${CPPUNIT_LIBRARIES} framework)
    target_include_directories(TestTasks PUBLIC ${CPPUNIT_INCLUDE_DIRS})
    add_test(NAME TestTasks COMMAND TestTasks)
    add_dependencies(check TestTasks)

    add_executable(TestTimeFrame TestTimeFrame.cpp)
    target_compile_definitions(TestTimeFrame PUBLIC -DLOG_TASKS)
    target_link_libraries(TestTimeFrame ${CPPUNIT_LIBRARIES} framework)
    target_include_directories(TestTimeFrame PUBLIC ${CPPUNIT_INCLUDE_DIRS})
    add_test(NAME TestTimeFrame COMMAND TestTimeFrame)
    add_dependencies(check TestTimeFrame)

    add_executable(TestFramework TestFramework.cpp)
    target_compile_definitions(TestFramework PUBLIC -DLOG_TASKS -DSRCDIR="${CMAKE_CURRENT_SOURCE_DIR}")
    target_link_libraries(TestFramework ${CPPUNIT_LIBRARIES} framework)
    target_include_directories(TestFramework PUBLIC ${CPPUNIT_INCLUDE_DIRS})
    add_test(NAME TestFramework COMMAND TestFramework)
    add_dependencies(check TestFramework)

#    add_executable(TestTerrain TestTerrain.cpp)
#    target_compile_definitions(TestTerrain PUBLIC -DLOG_TASKS)
#    target_link_libraries(TestTerrain ${CPPUNIT_LIBRARIES} ${WF_LIBRARIES} emberogre terrain caelum pagedgeometry entitymapping lua services framework)
#    target_include_directories(TestTerrain PUBLIC ${CPPUNIT_INCLUDE_DIRS})
#    add_test(NAME TestTerrain COMMAND TestTerrain)

endif (CPPUNIT_FOUND)
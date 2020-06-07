if (CPPUNIT_FOUND)

    MESSAGE(STATUS "Building tests.")

    add_executable(TestOgreView
            TestOgreView.cpp
            ConvertTestCase.cpp
            ModelMountTestCase.cpp
            $<TARGET_OBJECTS:emberogre>
            $<TARGET_OBJECTS:caelum>
            $<TARGET_OBJECTS:meshtree>
            $<TARGET_OBJECTS:pagedgeometry>
            $<TARGET_OBJECTS:widgets>
            $<TARGET_OBJECTS:emberogre_terrain>
            $<TARGET_OBJECTS:terrain>
            $<TARGET_OBJECTS:entitymapping>
            $<TARGET_OBJECTS:lua>
            $<TARGET_OBJECTS:cegui>
            $<TARGET_OBJECTS:navigation>
            $<TARGET_OBJECTS:services>
            $<TARGET_OBJECTS:domain>
            $<TARGET_OBJECTS:framework>
            $<TARGET_OBJECTS:ember_tinyxml>
            $<TARGET_OBJECTS:binreloc>
            $<TARGET_OBJECTS:CEGUIOgreRenderer>
            $<TARGET_OBJECTS:DetourTileCache>
            $<TARGET_OBJECTS:Detour>
            $<TARGET_OBJECTS:Recast>)
    target_compile_definitions(TestOgreView PUBLIC -DLOG_TASKS)
    target_link_libraries(TestOgreView ${CPPUNIT_LIBRARIES}
            ${WF_LIBRARIES}
            ${BULLET_LIBRARIES})
    target_include_directories(TestOgreView PUBLIC ${CPPUNIT_INCLUDE_DIRS})
    add_test(NAME TestOgreView COMMAND TestOgreView)
    add_dependencies(check TestOgreView)

    add_executable(TestTasks
            TestTasks.cpp
            $<TARGET_OBJECTS:framework>
            $<TARGET_OBJECTS:ember_tinyxml>)
    target_compile_definitions(TestTasks PUBLIC -DLOG_TASKS)
    target_link_libraries(TestTasks
            ${WF_LIBRARIES}
            ${CPPUNIT_LIBRARIES})
    target_include_directories(TestTasks PUBLIC ${CPPUNIT_INCLUDE_DIRS})
    add_test(NAME TestTasks COMMAND TestTasks)
    add_dependencies(check TestTasks)

    add_executable(TestTimeFrame
            TestTimeFrame.cpp
            $<TARGET_OBJECTS:framework>
            $<TARGET_OBJECTS:ember_tinyxml>)
    target_compile_definitions(TestTimeFrame PUBLIC -DLOG_TASKS)
    target_link_libraries(TestTimeFrame
            ${WF_LIBRARIES}
            ${CPPUNIT_LIBRARIES})
    target_include_directories(TestTimeFrame PUBLIC ${CPPUNIT_INCLUDE_DIRS})
    add_test(NAME TestTimeFrame COMMAND TestTimeFrame)
    add_dependencies(check TestTimeFrame)

    add_executable(TestFramework
            TestFramework.cpp
            $<TARGET_OBJECTS:framework>
            $<TARGET_OBJECTS:ember_tinyxml>)
    target_compile_definitions(TestFramework PUBLIC -DLOG_TASKS -DSRCDIR="${CMAKE_CURRENT_SOURCE_DIR}")
    target_link_libraries(TestFramework
            ${WF_LIBRARIES}
            ${CPPUNIT_LIBRARIES})
    target_include_directories(TestFramework PUBLIC ${CPPUNIT_INCLUDE_DIRS})
    add_test(NAME TestFramework COMMAND TestFramework)
    add_dependencies(check TestFramework)

    #    add_executable(TestTerrain TestTerrain.cpp)
    #    target_compile_definitions(TestTerrain PUBLIC -DLOG_TASKS)
    #    target_link_libraries(TestTerrain ${CPPUNIT_LIBRARIES} ${WF_LIBRARIES} emberogre terrain caelum pagedgeometry entitymapping lua services framework)
    #    target_include_directories(TestTerrain PUBLIC ${CPPUNIT_INCLUDE_DIRS})
    #    add_test(NAME TestTerrain COMMAND TestTerrain)

endif (CPPUNIT_FOUND)
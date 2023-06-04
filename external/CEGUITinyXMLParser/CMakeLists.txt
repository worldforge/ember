add_library(CEGUITinyXMLParser OBJECT
        XMLParser.cpp
        XMLParserModule.cpp)

target_link_libraries(CEGUITinyXMLParser PUBLIC
        ember_tinyxml)


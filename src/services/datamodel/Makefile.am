INCLUDES = -I$(top_srcdir)/src

noinst_LIBRARIES = libDataModelService.a

check_PROGRAMS = DataModelTest

libDataModelService_a_SOURCES = DataModelService.cpp DataObject.cpp DataProvider.cpp FolderProvider.cpp LinkProvider.cpp

DataModelTest_SOURCES = DataModelTest.cpp
DataModelTest_LDFLAGS = libDataModelService.a

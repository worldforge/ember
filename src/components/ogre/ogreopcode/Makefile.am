INCLUDES = -I$(top_srcdir)/src -I$(srcdir)/opcode -I$(srcdir)/include -I$(srcdir)  -I$(top_builddir)/src
METASOURCES = AUTO
noinst_HEADERS = include/OgreCollisionContext.h include/OgreCollisionManager.h \
	include/OgreCollisionObject.h include/OgreCollisionReporter.h include/OgreCollisionShape.h include/OgreCollisionTypes.h \
	include/OgreHash.h include/OgreKeyArray.h include/OgreNodes.h include/OgreOpcodeDebugObject.h include/OgreOpcodeExports.h \
	include/OgreOpcode.h include/OgreOpcodeMath.h
SUBDIRS = opcode
noinst_LIBRARIES = libOgreOpcode.a
libOgreOpcode_a_SOURCES = src/OgreCollisionContext.cpp src/OgreCollisionManager.cpp \
	src/OgreCollisionShape.cpp src/OgreOpcodeDebugObject.cpp

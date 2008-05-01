INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src

noinst_LTLIBRARIES = libModel.la

libModel_la_SOURCES = Action.cpp AnimationSet.cpp Model.cpp ModelDefinition.cpp \
	ModelDefinitionAtlasComposer.cpp ModelDefinitionManager.cpp ParticleSystem.cpp ParticleSystemBinding.cpp \
	SubModel.cpp SubModelPart.cpp XMLModelDefinitionSerializer.cpp
	
#	SimpleModel.cpp IModel.cpp ModelBase.cpp
#noinst_HEADERS = SimpleModel.h IModel.h ModelBase.h
noinst_HEADERS = Action.h AnimationSet.h Model.h ModelDefinition.h \
	ModelDefinitionAtlasComposer.h ModelDefinitionManager.h ParticleSystem.h ParticleSystemBinding.h \
	SubModel.h SubModelPart.h XMLModelDefinitionSerializer.h
SUBDIRS = mapping

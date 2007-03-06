INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src

noinst_LTLIBRARIES = libModel.la

libModel_la_SOURCES = Model.cpp SubModel.cpp SubModelPart.cpp  \
		ModelDefinition.cpp  ModelDefinitionManager.cpp \
		XMLModelDefinitionSerializer.cpp  \
		Action.cpp AnimationSet.cpp ParticleSystemBinding.cpp ParticleSystem.cpp
	
#	SimpleModel.cpp IModel.cpp ModelBase.cpp
#noinst_HEADERS = SimpleModel.h IModel.h ModelBase.h
noinst_HEADERS = Action.h AnimationSet.h ParticleSystemBinding.h \
		ParticleSystem.h Model.h SubModel.h SubModelPart.h ModelDefinition.h \
		XMLModelDefinitionSerializer.h ModelDefinitionManager.h
SUBDIRS = mapping

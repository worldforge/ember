INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src

noinst_LIBRARIES = libModel.a

libModel_a_SOURCES = Model.cpp SubModel.cpp SubModelPart.cpp Model.h SubModel.h \
		SubModelPart.h ModelDefinition.cpp ModelDefinition.h ModelDefinitionManager.cpp \
		ModelDefinitionManager.h XMLModelDefinitionSerializer.cpp XMLModelDefinitionSerializer.h \
	Action.cpp AnimationSet.cpp ParticleSystemBinding.cpp ParticleSystem.cpp
#	SimpleModel.cpp IModel.cpp ModelBase.cpp
#noinst_HEADERS = SimpleModel.h IModel.h ModelBase.h
noinst_HEADERS = Action.h AnimationSet.h ParticleSystemBinding.h \
	ParticleSystem.h

INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src

noinst_LIBRARIES = libModel.a

libModel_a_SOURCES = Action.cpp AnimationSet.cpp Model.cpp \
	ModelBackgroundLoader.cpp ModelDefinition.cpp ModelDefinitionAtlasComposer.cpp \
	ModelDefinitionManager.cpp ModelPart.cpp ParticleSystem.cpp ParticleSystemBinding.cpp SubModel.cpp \
	SubModelPart.cpp XMLModelDefinitionSerializer.cpp ModelRepresentation.cpp \
	ModelRepresentationManager.cpp ModelMount.cpp ModelAttachment.cpp ModelAttachedAttachment.cpp ModelBoneProvider.cpp
	
#	SimpleModel.cpp IModel.cpp ModelBase.cpp
#noinst_HEADERS = SimpleModel.h IModel.h ModelBase.h
noinst_HEADERS = Action.h AnimationSet.h Model.h ModelBackgroundLoader.h \
	ModelDefinition.h ModelDefinitionAtlasComposer.h ModelDefinitionManager.h ModelPart.h \
	ParticleSystem.h ParticleSystemBinding.h SubModel.h SubModelPart.h \
	XMLModelDefinitionSerializer.h  ModelRepresentation.h ModelRepresentationManager.h \
	ModelMount.h ModelAttachment.h ModelAttachedAttachment.h ModelBoneProvider.h

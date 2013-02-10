INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/src -DPREFIX=\"@prefix@\"
METASOURCES = AUTO

noinst_LIBRARIES = libCEGUI.a
libCEGUI_a_SOURCES = CEGUILogger.cpp RenderedColourStringTextComponent.cpp ColouredRenderedStringParser.cpp
noinst_HEADERS = CEGUILogger.h RenderedColourStringTextComponent.h ColouredRenderedStringParser.h
	
ceguifontsdir = $(datadir)/ember/media/shared/gui/cegui/datafiles/fonts
ceguiimagesetsdir = $(datadir)/ember/media/shared/gui/cegui/datafiles/imagesets
ceguilayoutsdir = $(datadir)/ember/media/shared/gui/cegui/datafiles/layouts
ceguischemesdir = $(datadir)/ember/media/shared/gui/cegui/datafiles/schemes
ceguiconfigdir = $(datadir)/ember/media/shared/gui/cegui/datafiles/configs
ceguilooknfeeldir = $(datadir)/ember/media/shared/gui/cegui/datafiles/looknfeel
ceguischemadir = $(datadir)/ember/media/shared/gui

dist_ceguifonts_DATA = datafiles/fonts/DejaVuSans-Bold-8.xml datafiles/fonts/DejaVuSans-Bold-10.xml datafiles/fonts/DejaVuSans-14.xml datafiles/fonts/DejaVuSans-10.xml datafiles/fonts/DejaVuSans-8.xml datafiles/fonts/DejaVuSans-6.xml datafiles/fonts/DejaVuSans-7.xml
dist_ceguiimagesets_DATA = datafiles/imagesets/Imageset.xsd datafiles/imagesets/iconset_standard.imageset datafiles/imagesets/iconset_mason.imageset datafiles/imagesets/EmberLook.imageset datafiles/imagesets/status_main.imageset datafiles/imagesets/compass.imageset datafiles/imagesets/splash.imageset
dist_ceguilayouts_DATA = datafiles/layouts/GUILayout.xsd
dist_ceguischemes_DATA = datafiles/schemes/GUIScheme.xsd datafiles/schemes/EmberLookSkin.scheme datafiles/schemes/EmberLookSkinMinimal.scheme
dist_ceguiconfig_DATA = datafiles/configs/cegui.config
dist_ceguilooknfeel_DATA = datafiles/looknfeel/EmberLook.looknfeel
dist_ceguischema_DATA = datafiles/layouts/GUILayout.xsd datafiles/imagesets/Imageset.xsd datafiles/fonts/Font.xsd datafiles/schemes/GUIScheme.xsd datafiles/configs/CEGUIConfig.xsd datafiles/looknfeel/Falagard.xsd

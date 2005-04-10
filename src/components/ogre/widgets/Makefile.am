current = $(PWD)

dist-hook:
	mkdir -p $(distdir)
	(cd $(top_srcdir)/src/components/ogre/widgets; tar cf - `find . -name \*.xml` | ( cd $(current); cd $(distdir); tar xf -))
	(cd $(top_srcdir)/src/components/ogre/widgets; tar cf - `find . -name \*.widget` | ( cd $(current); cd $(distdir); tar xf -))

install-data-local:
	mkdir -p $(DESTDIR)$(datadir)/ember/data/media/gui/cegui/widgets/ 
	(cd $(top_srcdir)/src/components/ogre/widgets; tar cf - `find . -name \*.xml`) | (cd $(DESTDIR)$(datadir)/ember/data/media/gui/cegui/widgets; tar xf -)
	(cd $(top_srcdir)/src/components/ogre/widgets; tar cf - `find . -name \*.widget`) | (cd $(DESTDIR)$(datadir)/ember/data/media/gui/cegui/widgets; tar xf -)

uninstall-local:
	find $(DESTDIR)$(datadir)/ember/data/media/gui/cegui/widgets/ -name  \*.xml | xargs rm
	find $(DESTDIR)$(datadir)/ember/data/media/gui/cegui/widgets/ -name  \*.widget | xargs rm


INCLUDES = -I$(top_srcdir)/src 
METASOURCES = AUTO
noinst_LIBRARIES = libWidgets.a
libWidgets_a_SOURCES = Widget.cpp Widget.h ChatWidget.cpp ChatWidget.h \
	ConsoleWidget.cpp 		ConsoleWidget.h DebugWidget.cpp DebugWidget.h EntityPickerWidget.cpp \
			EntityPickerWidget.h GiveWidget.cpp GiveWidget.h IngameChatWidget.cpp IngameChatWidget.h \
			IngameEventsWidget.cpp IngameEventsWidget.h InspectWidget.cpp InspectWidget.h \
			InventoryWidget.cpp InventoryWidget.h MakeEntityWidget.cpp MakeEntityWidget.h \
			ServerBrowserWidget.cpp ServerBrowserWidget.h ServerWidget.cpp ServerWidget.h 		 \
	Performance.cpp Performance.h WidgetDefinitions.cpp JesusEdit.cpp
noinst_HEADERS = WidgetDefinitions.h JesusEdit.h

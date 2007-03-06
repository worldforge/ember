current = $(PWD)

dist-hook:
	mkdir -p $(distdir)
	(cd $(top_srcdir)/src/components/ogre/jesus; tar cf - `find . -name \*.xml` | ( cd $(current); cd $(distdir); tar xf -))

install-data-local:
	mkdir -p $(DESTDIR)$(datadir)/ember/jesus 
	(cd $(top_srcdir)/src/components/ogre/jesus; tar cf - `find . -name \*.xml`) | (cd $(DESTDIR)$(datadir)/ember/jesus; tar xf -)

uninstall-local:
	find $(DESTDIR)$(datadir)/ember/jesus/ -name  \*.xml | xargs rm -f



INCLUDES = -I$(top_srcdir)/src  -I$(top_builddir)/src
METASOURCES = AUTO
noinst_LTLIBRARIES = libJesus.la
noinst_HEADERS = Jesus.h JesusMousePicker.h JesusPickerObject.h \
	XMLJesusSerializer.h
libJesus_la_SOURCES = Jesus.cpp JesusMousePicker.cpp JesusPickerObject.cpp \
	XMLJesusSerializer.cpp

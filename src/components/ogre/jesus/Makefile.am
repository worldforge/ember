current = $(PWD)

dist-hook:
	mkdir -p $(distdir)
	(cd $(top_srcdir)/src/components/ogre/jesus/modelmappings; tar cf - `find . -name \*.xml` | ( cd $(current); cd $(distdir); tar xf -))

install-data-local:
	mkdir -p $(DESTDIR)$(datadir)/ember/data/jesus/modelmappings 
	(cd $(top_srcdir)/src/components/ogre/jesus/modelmappings; tar cf - `find . -name \*.xml`) | (cd $(DESTDIR)$(datadir)/ember/data/jesus/modelmappings; tar xf -)

uninstall-local:
	find $(DESTDIR)$(datadir)/ember/data/jesus/modelmappings/ -name  \*.xml | xargs rm



INCLUDES = -I$(top_srcdir)/src -DETCDIR=\"$(sysconfdir)\" -DEMBERDATADIR=\"@datadir@/ember/data/\"
METASOURCES = AUTO
noinst_LIBRARIES = libJesus.a
noinst_HEADERS = Jesus.h JesusMousePicker.h JesusPickerObject.h
libJesus_a_SOURCES = Jesus.cpp JesusMousePicker.cpp JesusPickerObject.cpp

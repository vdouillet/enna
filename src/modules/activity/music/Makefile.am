include ./config.mak

MAINTAINERCLEANFILES = Makefile.in

MODULE = music

# data files for the module
filesdir = $(libdir)/enna/modules/

# the module .so file
AM_CPPFLAGS = \
-I. \
-I$(top_srcdir) \
-I$(top_srcdir)/src/modules/$(MODULE) \
-I$(top_srcdir)/src/bin \
-I$(top_srcdir)/src/modules \
@ENNA_CFLAGS@

if BUILD_ACTIVITY_MUSIC

pkgdir = $(libdir)/enna/modules

pkg_LTLIBRARIES = activity_music.la

activity_music_la_SOURCES = $(SRCS_ACTIVITY_MUSIC)

activity_music_la_LIBADD = @ENNA_LIBS@
activity_music_la_LDFLAGS = -module -avoid-version
activity_music_la_DEPENDENCIES = $(top_builddir)/config.h

uninstall:
	rm -rf $(DESTDIR)$(libdir)/enna/modules/activity_$(MODULE).*

endif

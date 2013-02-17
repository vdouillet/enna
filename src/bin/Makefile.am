## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
-DPACKAGE_LIB_DIR=\"$(libdir)\" \
-DPACKAGE_BIN_DIR=\"$(bindir)\" \
-DPACKAGE_DATA_DIR=\"$(datadir)\" \
-DLOCALE_DIR=\"$(datadir)/locale\"

AM_CFLAGS = @ENNA_CFLAGS@

bin_PROGRAMS = enna

enna_SOURCES = \
enna.c\
enna_config.c\
utils.c\
buffer.c\
metadata.c\
mainmenu.c\
module.c\
activity.c\
vfs.c\
volumes.c\
file.c\
browser.c\
browser_obj.c\
infos.c\
infos_video_flags.c\
content.c\
view_list.c\
view_list2.c\
view_wall.c\
mediaplayer_obj.c\
url_utils.c\
logs.c\
parser_cdda.c\
box.c\
exit.c\
volume_notification.c\
ini_parser.c\
input.c\
geoip.c\
search.c\
gadgets.c\
kbdnav.c\
videoplayer_obj.c \
mediaplayer_emotion.c

enna_LDADD = @ENNA_LIBS@
enna_LDFLAGS = -rdynamic

##########################################################################
# For Modules Static linking : BEGIN
##########################################################################

if USE_STATIC_MODULES

include ../modules/activity/configuration/config.mak
include ../modules/activity/music/config.mak
include ../modules/activity/photo/config.mak
include ../modules/activity/video/config.mak
include ../modules/browser/localfiles/config.mak
include ../modules/gadgets/date/config.mak
include ../modules/gadgets/dummy/config.mak
include ../modules/input/kbd/config.mak
include ../modules/input/lirc/config.mak
include ../modules/volume/mtab/config.mak

if BUILD_ACTIVITY_CONFIGURATION
enna_SOURCES += $(SRCS_ACTIVITY_CONFIGURATION)
endif

if BUILD_ACTIVITY_MUSIC
enna_SOURCES += $(SRCS_ACTIVITY_MUSIC)
endif

if BUILD_ACTIVITY_PHOTO
enna_SOURCES += $(SRCS_ACTIVITY_PHOTO)
endif

if BUILD_ACTIVITY_VIDEO
enna_SOURCES += $(SRCS_ACTIVITY_VIDEO)
endif

if BUILD_BROWSER_LOCALFILES
enna_SOURCES += $(SRCS_BROWSER_LOCALFILES)
endif

if BUILD_GADGET_DATE
enna_SOURCES += $(SRCS_GADGET_DATE)
endif

if BUILD_GADGET_DUMMY
enna_SOURCES += $(SRCS_GADGET_DUMMY)
endif

if BUILD_INPUT_KEYBOARD
enna_SOURCES += $(SRCS_INPUT_KBD)
endif

if BUILD_INPUT_LIRC
enna_SOURCES += $(SRCS_INPUT_LIRC)
endif

if BUILD_VOLUME_MTAB
enna_SOURCES += $(SRCS_VOLUME_MTAB)
endif

endif

##########################################################################
# For Modules Static linking : END
##########################################################################

EXTRA_DIST= \
enna.h\
enna_config.h\
utils.h\
buffer.h\
metadata.h\
mainmenu.h\
module.h\
vfs.h\
volumes.h\
activity.h\
file.h\
browser.h\
browser_obj.h\
infos.h\
infos_video_flags.h\
content.h\
view_list.h\
view_list2.h\
view_wall.h\
mediaplayer.h\
mediaplayer_obj.h\
url_utils.h\
logs.h\
parser_cdda.h\
box.h\
exit.h\
gettext.h\
volume_notification.h\
ini_parser.h\
input.h\
geoip.h\
gadgets.h\
kbdnav.h\
videoplayer_obj.h

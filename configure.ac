# get rid of that stupid cache mechanism
rm -f config.cache

AC_INIT([enna],[v5],[http://enna.geexbox.org])
AC_PREREQ(2.61)
AC_CONFIG_SRCDIR(configure.ac)
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_ISC_POSIX

AM_INIT_AUTOMAKE(1.6 dist-bzip2)
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_MACRO_DIR([m4])

AC_C_CONST
AC_PROG_CC_STDC

AC_DIAGNOSE([obsolete],[AM_PROG_CC_STDC:
	your code should no longer depend upon `am_cv_prog_cc_stdc', but upon
	`ac_cv_prog_cc_stdc'.  Remove this warning and the assignment when
	you adjust the code.  You can also remove the above call to
	AC_PROG_CC if you already called it elsewhere.])
am_cv_prog_cc_stdc=$ac_cv_prog_cc_stdc

AC_HEADER_STDC
AC_C_CONST
AM_PROG_CC_C_O
AC_PROG_GCC_TRADITIONAL

define([AC_LIBTOOL_LANG_CXX_CONFIG], [:])dnl
define([AC_LIBTOOL_LANG_F77_CONFIG], [:])dnl

AC_PROG_LIBTOOL

AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.17])
CFLAGS="$CFLAGS -DLOCALEDIR=\"\\\"$datadir/locale\\\"\""

PKG_CHECK_MODULES([ENNA],
  [
   eina >= 1.2.0
   eet >= 1.6.0
   evas >= 1.2.0
   ecore >= 1.2.0
   ecore-input > 1.2.0
   ecore-evas >= 1.2.0
   ecore-file >= 1.2.0
   ecore-ipc >= 1.2.0
   ecore-con >= 1.2.0
   efreet >= 1.2.0
   edje >= 1.2.0
   edbus >= 1.2.0
   ethumb >= 1.0.0
   elementary >= 1.0.0
   libvalhalla >= 2.0.0 libvalhalla < 3.0.0
   emotion >= 1.2.0
   libxml-2.0
   libcurl
  ]
)

# Evil library for compilation on Windows

case "$host_os" in
   mingw*)
      PKG_CHECK_MODULES([EVIL], [evil])
      AC_DEFINE(HAVE_EVIL, 1, [Set to 1 if evil package is installed])
   ;;
esac
AC_SUBST(EFL_EINA_BUILD)

##################
# Static Modules #
##################

AC_ARG_ENABLE(static_modules,
  [AC_HELP_STRING([--disable-static-modules], [disable static modules.
  [[default=enabled]]])],
  [use_static_modules=$enableval],
  [use_static_modules="yes"]
)
AM_CONDITIONAL([USE_STATIC_MODULES], test "x${use_static_modules}" = "xyes")
if test "x$use_static_modules" = "xyes"; then
  AC_DEFINE(USE_STATIC_MODULES, 1, [Static Modules])
fi

#########
# Theme #
#########

AC_ARG_ENABLE(theme,
	[AC_HELP_STRING([--disable-theme], [disable theme.
        [[default=enabled]]])],
	[build_theme=$enableval],
	[build_theme="yes"]
)
AM_CONDITIONAL([ENNA_BUILD_THEME], test "x${build_theme}" = "xyes")


####################
# Activity Modules #
####################

# Configuration
want_activity_configuration="yes"
AC_MSG_CHECKING(whether configuration activity module is to be built)
AC_ARG_ENABLE(activity-configuration,
  AS_HELP_STRING([--enable-activity-configuration],[Enable configuration Activity Module]),
  [ want_activity_configuration=$enableval ]
)
AC_MSG_RESULT($want_activity_configuration)
AM_CONDITIONAL(BUILD_ACTIVITY_CONFIGURATION, test "x$want_activity_configuration" = "xyes")
if test "x$want_activity_configuration" = "xyes"; then
  AC_DEFINE(BUILD_ACTIVITY_CONFIGURATION, 1, [configuration Activity])
fi

# Music
want_activity_music="yes"
AC_MSG_CHECKING(whether music activity module is to be built)
AC_ARG_ENABLE(activity-music,
  AS_HELP_STRING([--enable-activity-music],[Enable Music Activity Module]),
  [ want_activity_music=$enableval ]
)
AC_MSG_RESULT($want_activity_music)
AM_CONDITIONAL(BUILD_ACTIVITY_MUSIC, test "x$want_activity_music" = "xyes")
if test "x$want_activity_music" = "xyes"; then
  AC_DEFINE(BUILD_ACTIVITY_MUSIC, 1, [Music Activity])
fi

# Photo
want_activity_photo="yes"
AC_MSG_CHECKING(whether photo activity module is to be built)
AC_ARG_ENABLE(activity-photo,
  AS_HELP_STRING([--enable-activity-photo],[Enable Photo Activity Module]),
  [ want_activity_photo=$enableval ]
)
AC_MSG_RESULT($want_activity_photo)
AM_CONDITIONAL(BUILD_ACTIVITY_PHOTO, test "x$want_activity_photo" = "xyes")
if test "x$want_activity_photo" = "xyes"; then
  AC_DEFINE(BUILD_ACTIVITY_PHOTO, 1, [Photo Activity])
fi

# Video
want_activity_video="yes"
AC_MSG_CHECKING(whether video activity module is to be built)
AC_ARG_ENABLE(activity-video,
  AS_HELP_STRING([--enable-activity-video],[Enable Video Activity Module]),
  [ want_activity_video=$enableval ]
)
AC_MSG_RESULT($want_activity_video)
AM_CONDITIONAL(BUILD_ACTIVITY_VIDEO, test "x$want_activity_video" = "xyes")
if test "x$want_activity_video" = "xyes"; then
  AC_DEFINE(BUILD_ACTIVITY_VIDEO, 1, [Video Activity])
fi

###################
# Browser Modules #
###################

# Local Files
want_browser_localfiles="yes"
AC_MSG_CHECKING(whether localfiles browser module is to be built)
AC_ARG_ENABLE(browser-localfiles,
  AS_HELP_STRING([--enable-browser-localfiles],[Enable Local Files Browser Module]),
  [ want_browser_localfiles=$enableval ]
)
AC_MSG_RESULT($want_browser_localfiles)
AM_CONDITIONAL(BUILD_BROWSER_LOCALFILES, test "x$want_browser_localfiles" = "xyes")
if test "x$want_browser_localfiles" = "xyes"; then
  AC_DEFINE(BUILD_BROWSER_LOCALFILES, 1, [Localfiles Browser])
fi

###################
# Gadgets Modules #
###################

# Date gadget
want_gadget_date="yes"
AC_MSG_CHECKING(whether Date gadget is to be built)
AC_ARG_ENABLE(gadget_date,
  AS_HELP_STRING([--enable-gadget-date],[Enable Date Gadget Module]),
  [ want_gadget_date=$enableval ]
)
AC_MSG_RESULT($want_gadget_date)
AM_CONDITIONAL(BUILD_GADGET_DATE, test "x$want_gadget_date" = "xyes")
if test "x$want_gadget_date" = "xyes"; then
  AC_DEFINE(BUILD_GADGET_DATE, 1, [Gadget date Module])
fi

# Dummy gadget
want_gadget_dummy="no"
AC_MSG_CHECKING(whether Dummy gadget is to be built)
AC_ARG_ENABLE(gadget_dummy,
  AS_HELP_STRING([--enable-gadget-dummy],[Enable Dummy Gadget Module]),
  [ want_gadget_dummy=$enableval ]
)
AC_MSG_RESULT($want_gadget_dummy)
AM_CONDITIONAL(BUILD_GADGET_DUMMY, test "x$want_gadget_dummy" = "xyes")
if test "x$want_gadget_dummy" = "xyes"; then
  AC_DEFINE(BUILD_GADGET_DUMMY, 1, [Gadget dummy Module])
fi

#################
# Input Modules #
#################

# Keyboard
want_input_keyboard="yes"
AC_MSG_CHECKING(whether keyboard input module is to be built)
AC_ARG_ENABLE(input-keyboard,
  AS_HELP_STRING([--enable-input-keyboard],[Enable Keyboard Input Module]),
  [ want_input_keyboard=$enableval ]
)
AC_MSG_RESULT($want_input_keyboard)
AM_CONDITIONAL(BUILD_INPUT_KBD, test "x$want_input_keyboard" = "xyes")
if test "x$want_input_keyboard" = "xyes"; then
  AC_DEFINE(BUILD_INPUT_KBD, 1, [Keyboard Input])
fi

# LIRC
have_input_lirc=no
AC_CHECK_HEADER(lirc/lirc_client.h, have_input_lirc=yes)
if test "x$have_input_lirc" = "xyes"; then
  AC_DEFINE([BUILD_INPUT_LIRC], [1], [Define if LIRC client lib is present])
  have_input_lirc="yes"
  LIRC_LIBS=-llirc_client
  AC_SUBST(LIRC_LIBS)
fi
AM_CONDITIONAL(BUILD_INPUT_LIRC, test "x$have_input_lirc" = "xyes")

##################
# Volume Modules #
##################

# mtab
want_volume_mtab="yes"
AC_MSG_CHECKING(whether mtab volume module is to be built)
AC_ARG_ENABLE(volume-mtab,
  AS_HELP_STRING([--enable-volume-mtab],[Enable mtab Volume Module]),
  [ want_volume_mtab=$enableval ]
)
AC_MSG_RESULT($want_volume_mtab)
AM_CONDITIONAL(BUILD_VOLUME_MTAB, test "x$want_volume_mtab" = "xyes")
if test "x$want_volume_mtab" = "xyes"; then
  AC_DEFINE(BUILD_VOLUME_MTAB, 1, [mtab Volume])
fi

##############
# Librairies #
##############

# libxrandr
want_libxrandr="auto"
have_libxrandr="no"
AC_MSG_CHECKING(whether libxrandr support is to be built)
AC_ARG_ENABLE(libxrandr,
    AS_HELP_STRING([--enable-libxrandr],[Enable libxrandr support]),
    [ want_libxrandr=$enableval ]
)
AC_MSG_RESULT($want_libxrandr)
if test "x$want_libxrandr" = "xyes" -o "x$want_libxrandr" = "xauto"; then
  PKG_CHECK_MODULES(
    [LIBXRANDR],
    [xrandr],
    [
      AC_DEFINE(BUILD_LIBXRANDR, 1, [libxrandr support])
      have_libxrandr="yes"
    ],
    [
      if test "x$want_libxrandr" = "xyes" -a "x$use_strict" = "xyes" ; then
        AC_MSG_ERROR([libxrandr not found (strict dependencies checking)])
      fi
    ]
  )
fi
AM_CONDITIONAL(BUILD_LIBXRANDR, test "x$have_libxrandr" = "xyes")

##########
# THEMES #
##########

# Default theme
want_theme_default="yes"
AC_MSG_CHECKING(whether Default Theme is to be built)
AC_ARG_ENABLE(theme_default,
  AS_HELP_STRING([--enable-theme-default],[Enable Default Theme]),
  [ want_theme_default=$enableval ]
)
AC_MSG_RESULT($want_theme_default)
AM_CONDITIONAL(BUILD_THEME_DEFAULT, test "x$want_theme_default" = "xyes")
if test "x$want_theme_default" = "xyes"; then
  AC_DEFINE(BUILD_THEME_DEFAULT, 1, [Default Theme])
fi

# Set Top Box theme
want_theme_stb="yes"
AC_MSG_CHECKING(whether Stb Theme is to be built)
AC_ARG_ENABLE(theme_stb,
  AS_HELP_STRING([--enable-theme-stb],[Enable Stb Theme]),
  [ want_theme_stb=$enableval ]
)
AC_MSG_RESULT($want_theme_stb)
AM_CONDITIONAL(BUILD_THEME_STB, test "x$want_theme_stb" = "xyes")
if test "x$want_theme_stb" = "xyes"; then
  AC_DEFINE(BUILD_THEME_STB, 1, [Stb Theme])
fi

# silent rules require automake 1.11
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])],[AC_SUBST([AM_DEFAULT_VERBOSITY],[1])])

AC_CONFIG_FILES([
Makefile
po/Makefile.in
src/Makefile
src/bin/Makefile
src/modules/Makefile
src/modules/activity/Makefile
src/modules/activity/configuration/Makefile
src/modules/activity/music/Makefile
src/modules/activity/photo/Makefile
src/modules/activity/video/Makefile
src/modules/browser/Makefile
src/modules/browser/localfiles/Makefile
src/modules/gadgets/Makefile
src/modules/gadgets/date/Makefile
src/modules/gadgets/dummy/Makefile
src/modules/input/Makefile
src/modules/input/kbd/Makefile
src/modules/input/lirc/Makefile
src/modules/volume/Makefile
src/modules/volume/mtab/Makefile
data/Makefile
data/other/Makefile
data/theme/Makefile
data/theme/default/Makefile
data/theme/stb/Makefile
])

AC_OUTPUT

echo
echo "$PACKAGE $VERSION"
echo
eval echo "Installation Path.................. : $exec_prefix"
eval echo "\  Binaries......................... : $exec_prefix/bin"

if test "x$use_static_modules" = "xno"; then
eval echo "\  Modules.......................... : $exec_prefix/lib/enna/modules"
fi

eval echo "\  Theme............................ : $exec_prefix/share/enna/theme"
echo
echo "Build theme........................ : $build_theme"
echo "Static Modules..................... : $use_static_modules"
echo "NLS................................ : $USE_NLS"
echo
echo "Supported Activity Modules:"
echo
echo "  Configuration.................... : $want_activity_configuration"
echo "  Music............................ : $want_activity_music"
echo "  Photo............................ : $want_activity_photo"
echo "  Video............................ : $want_activity_video"
echo
echo
echo "Supported Browser Modules:"
echo
echo "  Local files...................... : $want_browser_localfiles"
echo
echo "Supported Gadget Modules:"
echo
echo "  Date............................. : $want_gadget_date"
echo "  Dummy............................ : $want_gadget_dummy"
echo
echo "Supported Input Modules:"
echo
echo "  Keyboard client.................. : $want_input_keyboard"
echo "  LIRC client...................... : $have_input_lirc"
echo
echo "Supported Volume Modules:"
echo
echo "  mtab............................. : $want_volume_mtab"
echo
echo "Optional linked-in librairies:"
echo
echo "  libxrandr........................ : $have_libxrandr"
echo
echo "Suppported Themes:"
echo
echo "  default.......................... : $want_theme_default"
echo "  stb.............................. : $want_theme_stb"
echo
echo "Now type 'make' ('gmake' on some systems) to compile $PACKAGE."
echo


# get rid of that stupid cache mechanism
rm -f config.cache

AC_INIT([enna],[v5],[http://enna.geexbox.org])
AC_PREREQ([2.61])
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.6 dist-bzip2])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

LT_INIT([win32-dll])

AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.17])

### Additional options to configure

## Static Modules
AC_ARG_ENABLE([static_modules],
   [AC_HELP_STRING(
       [--disable-static-modules],
       [disable static modules. @<:@default=enabled@:>@])],
   [
    if test "x${enableval}" = "xyes"; then
       use_static_modules="yes"
    else
       use_static_modules="no"
    fi
   ],
   [use_static_modules="yes"])

if test "x${use_static_modules}" = "xyes"; then
   AC_DEFINE([USE_STATIC_MODULES], [1], [Static Modules])
fi

AM_CONDITIONAL([USE_STATIC_MODULES], [test "x${use_static_modules}" = "xyes"])

## Theme
AC_ARG_ENABLE([theme],
   [AC_HELP_STRING(
       [--disable-theme],
       [disable theme. @<:@default=enabled@:>@])],
   [
    if test "x${enableval}" = "xyes"; then
       build_theme="yes"
    else
       build_theme="no"
    fi
   ],
   [build_theme="yes"])

AC_MSG_CHECKING([whether Theme is to be built])
AC_MSG_RESULT([${build_theme}])

AM_CONDITIONAL([ENNA_BUILD_THEME], [test "x${build_theme}" = "xyes"])

# Default theme
AC_ARG_ENABLE([theme-default],
   [AC_HELP_STRING(
       [--disable-theme-default],
       [disable default theme. @<:@default=enabled@:>@])],
   [
    if test "x${enableval}" = "xyes"; then
       want_theme_default="yes"
    else
       want_theme_default="no"
    fi
   ],
   [want_theme_default="yes"])

AC_MSG_CHECKING([whether default theme is to be built])
AC_MSG_RESULT([${want_theme_default}])

if test "x${want_theme_default}" = "xyes"; then
  AC_DEFINE([BUILD_THEME_DEFAULT], [1], [Default Theme])
fi

AM_CONDITIONAL([BUILD_THEME_DEFAULT], [test "x${want_theme_default}" = "xyes"])

# Set Top Box theme
AC_ARG_ENABLE([theme-stb],
   [AC_HELP_STRING(
       [--disable-theme-stb],
       [disable Set Top Box theme. @<:@default=enabled@:>@])],
   [
    if test "x${enableval}" = "xyes"; then
       want_theme_stb="yes"
    else
       want_theme_stb="no"
    fi
   ],
   [want_theme_stb="yes"])

AC_MSG_CHECKING([whether Set Top Box theme is to be built])
AC_MSG_RESULT([${want_theme_stb}])

if test "x${want_theme_stb}" = "xyes"; then
  AC_DEFINE([BUILD_THEME_STB], [1], [Set Top Box Theme])
fi

AM_CONDITIONAL([BUILD_THEME_STB], [test "x${want_theme_stb}" = "xyes"])

# X RandR
AC_ARG_ENABLE([libxrandr],
   [AS_HELP_STRING(
       [--enable-libxrandr],
       [Enable libxrandr support])],
   [
    if test "x${enableval}" = "xyes"; then
       want_libxrandr="yes"
    else
       want_libxrandr="no"
    fi
   ],
   [want_libxrandr="auto"])


### Default values or options with respect to host

AC_CANONICAL_HOST

efl_version="1.7.0"

### Checks for programs

AC_PROG_CC

PKG_PROG_PKG_CONFIG

### Checks for libraries

requirements=""
requirements_libs=""

## Evil

case "$host_os" in
   mingw*)
      PKG_CHECK_EXISTS([evil >= ${efl_version}],
         [
          requirements="evil >= ${efl_version}"
          AC_DEFINE(HAVE_EVIL, 1, [Set to 1 if evil package is installed])
         ])
   ;;
esac

## hard dependencies
requirements_hard="eina >= ${efl_version} eet >= ${efl_version} evas >= ${efl_version} ecore >= ${efl_version} ecore-input >= ${efl_version} ecore-evas >= ${efl_version} ecore-file >= ${efl_version} ecore-ipc >= ${efl_version} ecore-con >= ${efl_version} efreet >= ${efl_version} edje >= ${efl_version} edbus >= ${efl_version} emotion >= ${efl_version} ethumb >= ${efl_version} ethumb_client >= ${efl_version} elementary >= ${efl_version} libvalhalla >= 2.0.0 libvalhalla < 3.0.0 libcurl"

PKG_CHECK_EXISTS(
   [${requirements_hard}],
   [
    requirements="${requirements_hard} ${requirements}"
   ],
   [PKG_CHECK_MODULES([dummy], [${requirements_hard}])])

## Activity Modules

# Configuration
ENNA_CHECK_MODULE([Activity], [Configuration], [yes])

# Music
ENNA_CHECK_MODULE([Activity], [Music], [yes])

# Photo
ENNA_CHECK_MODULE([Activity], [Photo], [yes])

# Video
ENNA_CHECK_MODULE([Activity], [Video], [yes])

## Gadgets Modules

# Date gadget
ENNA_CHECK_MODULE([Gadget], [Date], [yes])

# Dummy gadget
ENNA_CHECK_MODULE([Gadget], [Dummy], [no])

## Browser Modules

# Local Files
ENNA_CHECK_MODULE([Browser], [LocalFiles], [yes])

## Input Modules

# Keyboard
ENNA_CHECK_MODULE([Input], [Keyboard], [yes])

# LIRC
ENNA_CHECK_MODULE([Input], [LIRC], [yes])

## Volume Modules

# mtab
ENNA_CHECK_MODULE([Volume], [mtab], [yes])

PKG_CHECK_MODULES([ENNA], [${requirements}])

ENNA_LIBS="${ENNA_LIBS} ${requirements_libs}"

### Checks for header files

### Checks for types

### Checks for structures

### Checks for compiler characteristics

AM_PROG_CC_C_O

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
echo "  LIRC client...................... : $want_input_lirc"
echo
echo "Supported Volume Modules:"
echo
echo "  mtab............................. : $want_volume_mtab"
echo
echo "Optional linked-in librairies:"
echo
echo "  libxrandr........................ : $want_libxrandr"
echo
echo "Suppported Themes:"
echo
echo "  default.......................... : $want_theme_default"
echo "  stb.............................. : $want_theme_stb"
echo
echo "Now type 'make' ('gmake' on some systems) to compile $PACKAGE."
echo


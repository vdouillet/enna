
dnl Activity

AC_DEFUN([ENNA_CHECK_MODULE_ACTIVITY_CONFIGURATION],
[
have_dep="yes"

if test "x${want_libxrandr}" = "xyes" || test "x${want_libxrandr}" = "xauto" ; then
   PKG_CHECK_EXISTS([xrandr],
      [
       want_libxrandr="yes"
       requirements="${requirements} xrandr"
       requirements_libs="${requirements_libs} -lX11"
       AC_DEFINE(BUILD_LIBXRANDR, 1, [libxrandr support])
      ],
      [want_libxrandr="no"])
fi

AM_CONDITIONAL([BUILD_LIBXRANDR], [test "x${have_libxrandr}" = "xyes"])

AS_IF([test "x${have_dep}" = "xyes"], [$1], [$2])
])

AC_DEFUN([ENNA_CHECK_MODULE_ACTIVITY_MUSIC],
[
have_dep="yes"
AS_IF([test "x${have_dep}" = "xyes"], [$1], [$2])
])

AC_DEFUN([ENNA_CHECK_MODULE_ACTIVITY_PHOTO],
[
have_dep="yes"
AS_IF([test "x${have_dep}" = "xyes"], [$1], [$2])
])

AC_DEFUN([ENNA_CHECK_MODULE_ACTIVITY_VIDEO],
[
have_dep="yes"
AS_IF([test "x${have_dep}" = "xyes"], [$1], [$2])
])

dnl Gadget

AC_DEFUN([ENNA_CHECK_MODULE_GADGET_DATE],
[
have_dep="yes"
AS_IF([test "x${have_dep}" = "xyes"], [$1], [$2])
])

AC_DEFUN([ENNA_CHECK_MODULE_GADGET_DUMMY],
[
have_dep="yes"
AS_IF([test "x${have_dep}" = "xyes"], [$1], [$2])
])

dnl Browser

AC_DEFUN([ENNA_CHECK_MODULE_BROWSER_LOCALFILES],
[
have_dep="yes"
AS_IF([test "x${have_dep}" = "xyes"], [$1], [$2])
])

dnl Input

AC_DEFUN([ENNA_CHECK_MODULE_INPUT_KEYBOARD],
[
have_dep="yes"
AS_IF([test "x${have_dep}" = "xyes"], [$1], [$2])
])

AC_DEFUN([ENNA_CHECK_MODULE_INPUT_LIRC],
[
AC_CHECK_HEADER([lirc/lirc_client.h], [have_dep="yes"], [have_dep="no"])

if test "x${have_dep}" = "xyes" ; then
   AC_CHECK_LIB([lirc_client], [lirc_init],
      [
       have_dep="yes"
       requirements_libs="-llirc_client"
      ],
      [have_dep="no"])
fi

AS_IF([test "x${have_dep}" = "xyes"], [$1], [$2])
])

dnl Volume

AC_DEFUN([ENNA_CHECK_MODULE_VOLUME_MTAB],
[
have_dep="yes"
AS_IF([test "x${have_dep}" = "xyes"], [$1], [$2])
])

dnl use: ENNA_CHECK(system, name, want_module)

AC_DEFUN([ENNA_CHECK_MODULE],
[
m4_pushdef([UP], m4_translit([[$2]], [-a-z], [_A-Z]))dnl
m4_pushdef([DOWN], m4_translit([[$2]], [-A-Z], [_a-z]))dnl

m4_pushdef([UPSYS], m4_translit([[$1]], [-a-z], [_A-Z]))dnl
m4_pushdef([DOWNSYS], m4_translit([[$1]], [-A-Z], [_a-z]))dnl

AC_ARG_ENABLE(m4_defn([DOWNSYS])[-]m4_defn([DOWN]),
   [AS_HELP_STRING(
      [--enable-]m4_defn([DOWNSYS])[-]m4_defn([DOWN]),
      [Enable $2 $1 Module @<:@default=$3@:>@])],
   [
    if test "x${enableval}" = "xyes"; then
       want_[]m4_defn([DOWNSYS])_[]m4_defn([DOWN])="yes"
    else
       want_[]m4_defn([DOWNSYS])_[]m4_defn([DOWN])="no"
    fi
   ],
   [want_[]m4_defn([DOWNSYS])_[]m4_defn([DOWN])=$3])

AC_MSG_CHECKING([whether $2 $1 module is to be built])
AC_MSG_RESULT([$want_[]m4_defn([DOWNSYS])_[]m4_defn([DOWN])])

if test "x${want_[]m4_defn([DOWNSYS])_[]m4_defn([DOWN])}" = "xyes" ; then
   m4_default([ENNA_CHECK_MODULE_]m4_defn([UPSYS])[_]m4_defn([UP]))([want_[]m4_defn([DOWNSYS])_[]m4_defn([DOWN])="yes"], [want_[]m4_defn([DOWNSYS])_[]m4_defn([DOWN])="no"])
fi

if test "x${want_[]m4_defn([DOWNSYS])_[]m4_defn([DOWN])}" = "xyes"; then
   AC_DEFINE([BUILD_]m4_defn([UPSYS])[_]m4_defn([UP]), [1], [$2])
fi

AM_CONDITIONAL([BUILD_]m4_defn([UPSYS])[_]m4_defn([UP]), [test "x${want_[]m4_defn([DOWNSYS])_[]m4_defn([DOWN])}" = "xyes"])

m4_popdef([DOWNSYS])
m4_popdef([UPSYS])
m4_popdef([DOWN])
m4_popdef([UP])
])

dnl End of enna_modules.m4

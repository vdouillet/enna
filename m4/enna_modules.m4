
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

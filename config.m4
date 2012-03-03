dnl $Id$
dnl config.m4 for extension notifyerror

PHP_ARG_WITH(notifyerror, for notifyerror support,
[  --with-notifyerror             Include notifyerror support])

if test "$PHP_NOTIFYERROR" != "no"; then
  PHP_SUBST(NOTIFYERROR_SHARED_LIBADD)
  AC_DEFINE(HAVE_NOTIFYERROR, 1, [ ])
  PHP_NEW_EXTENSION(notifyerror, notifyerror.c, $ext_shared)
fi

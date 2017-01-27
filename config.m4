dnl $Id$
dnl config.m4 for extension CModel

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

 PHP_ARG_WITH(CModel, for CModel support,
 Make sure that the comment is aligned:
 [  --with-CModel             Include CModel support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(CModel, whether to enable CModel support,
dnl Make sure that the comment is aligned:
dnl [  --enable-CModel           Enable CModel support])

if test "$PHP_CMODEL" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-CModel -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/CModel.h"  # you most likely want to change this
  dnl if test -r $PHP_CMODEL/$SEARCH_FOR; then # path given as parameter
  dnl   CMODEL_DIR=$PHP_CMODEL
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for CModel files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       CMODEL_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$CMODEL_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the CModel distribution])
  dnl fi

  dnl # --with-CModel -> add include path
  dnl PHP_ADD_INCLUDE($CMODEL_DIR/include)

  dnl # --with-CModel -> check for lib and symbol presence
  dnl LIBNAME=CModel # you may want to change this
  dnl LIBSYMBOL=CModel # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $CMODEL_DIR/$PHP_LIBDIR, CMODEL_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_CMODELLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong CModel lib version or lib not found])
  dnl ],[
  dnl   -L$CMODEL_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(CMODEL_SHARED_LIBADD)

  PHP_NEW_EXTENSION(CModel, CModel.c, $ext_shared)
fi

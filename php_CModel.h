/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2012 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_CMODEL_H
#define PHP_CMODEL_H

extern zend_module_entry CModel_module_entry;
#define phpext_CModel_ptr &CModel_module_entry

#ifdef PHP_WIN32
#	define PHP_CMODEL_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_CMODEL_API __attribute__ ((visibility("default")))
#else
#	define PHP_CMODEL_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(CModel);
PHP_MSHUTDOWN_FUNCTION(CModel);
PHP_RINIT_FUNCTION(CModel);
PHP_RSHUTDOWN_FUNCTION(CModel);
PHP_MINFO_FUNCTION(CModel);

PHPAPI extern unsigned char *php_update_where_str(zval **value);
PHPAPI extern unsigned char *php_table_str(char *table);
PHPAPI extern unsigned char *php_and_where_str(zval **value,zval *this,zend_class_entry *model);
PHP_FUNCTION(confirm_CModel_compiled);	/* For testing, remove later. */
PHP_FUNCTION(CModel);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(CModel)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(CModel)
*/

/* In every utility function you add that needs to use variables 
   in php_CModel_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as CMODEL_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define CMODEL_G(v) TSRMG(CModel_globals_id, zend_CModel_globals *, v)
#else
#define CMODEL_G(v) (CModel_globals.v)
#endif

#endif	/* PHP_CMODEL_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

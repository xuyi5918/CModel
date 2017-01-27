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
  | Author: Y-agile <xuyi5918@live.cn>                                                             |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_CModel.h"
#include "ext/standard/php_string.h"  
/* If you declare any globals in php_CModel.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(CModel)
*/

/* True global resources - no need for thread safety here */
static int le_CModel;

/* {{{ CModel_functions[]
 *
 * Every user visible function must have an entry in CModel_functions[].
 */
const zend_function_entry CModel_functions[] = {
	PHP_FE(confirm_CModel_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(CModel,	NULL)
	PHP_FE_END	/* Must be the last line in CModel_functions[] */
};
/* }}} */

/* {{{ CModel_module_entry
 */
zend_module_entry CModel_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"CModel",
	CModel_functions,
	PHP_MINIT(CModel),
	PHP_MSHUTDOWN(CModel),
	PHP_RINIT(CModel),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(CModel),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(CModel),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_CMODEL
ZEND_GET_MODULE(CModel)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("CModel.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_CModel_globals, CModel_globals)
    STD_PHP_INI_ENTRY("CModel.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_CModel_globals, CModel_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_CModel_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_CModel_init_globals(zend_CModel_globals *CModel_globals)
{
	CModel_globals->global_value = 0;
	CModel_globals->global_string = NULL;
}
*/
/* }}} */
int checckType(zval *data){
	if(Z_TYPE_P(data)== IS_NULL){
		return 0;
	}else{
		return data->type;
	}
}
/* {{{ PHP_MINIT_FUNCTION
 */

zend_class_entry *model_class;
// create sql string
ZEND_METHOD(model_class,getSql){
	zend_class_entry *model;
	int len;
	model=Z_OBJCE_P(getThis());
	zval *selectVal;
	zval *whereVal;
	zval *Type=zend_read_property(model,getThis(),"sqlType",sizeof("sqlType")-1,0 TSRMLS_CC);
	zval *setVal;
	if(Z_LVAL_PP(&Type) ==1){
		selectVal=zend_read_property(model,getThis(),"select",sizeof("select")-1,0 TSRMLS_CC);
	}else if(Z_LVAL_PP(&Type)==2){
		selectVal=zend_read_property(model,getThis(),"delete",sizeof("delete")-1,0 TSRMLS_CC);
	}else if(Z_LVAL_PP(&Type)==3){
		selectVal=zend_read_property(model,getThis(),"update",sizeof("update")-1,0 TSRMLS_CC);
		setVal=zend_read_property(model,getThis(),"set",sizeof("set")-1,0 TSRMLS_CC);
	}else if(Z_LVAL_PP(&Type)==4){
		selectVal=zend_read_property(model,getThis(),"insert",sizeof("insert")-1,0 TSRMLS_CC);
	}
	
	//read table 
	zval *tableVal=zend_read_property(model,getThis(),"table",sizeof("table")-1,0 TSRMLS_CC);

	if(Z_LVAL_PP(&Type) < 4){
		//read where string
		whereVal=zend_read_property(model,getThis(),"where",sizeof("where")-1,0 TSRMLS_CC);
		unsigned char *whereChar=Z_STRVAL_PP(&whereVal);
		
		int wLen=strlen(whereChar);
		int slen=wLen-strlen(" AND ");

		for (wLen; wLen > slen; wLen--)
		{
			whereChar[wLen]='\0';
		}
		// TOTAL STRING LEN
		len=strlen(Z_STRVAL_PP(&tableVal))+strlen(Z_STRVAL_PP(&selectVal))+strlen(Z_STRVAL_PP(&whereVal));
	}else{
		len=strlen(Z_STRVAL_PP(&selectVal));
	}
	
	unsigned char *bindSql;

	if(Z_LVAL_PP(&Type) == 4){
		bindSql=(unsigned char *)malloc(strlen(Z_STRVAL_PP(&selectVal)));

		sprintf(bindSql,"%s",Z_STRVAL_PP(&selectVal));
		
	}else if(Z_LVAL_PP(&Type) == 3){
		unsigned char *val;
		int setLen=strlen(Z_STRVAL_PP(&setVal));
		 val=(unsigned char *)malloc(setLen);
		 val=Z_STRVAL_PP(&setVal);
		if(val[setLen-1]==','){
			val[setLen-1]=' ';
		}	
		bindSql=(unsigned char *)malloc(len+6+setLen);
		sprintf(bindSql,"%s%s %s%s",Z_STRVAL_PP(&selectVal),Z_STRVAL_PP(&tableVal),Z_STRVAL_PP(&setVal),Z_STRVAL_PP(&whereVal));
		
	}else{
		int wheLen=strlen(Z_STRVAL_PP(&whereVal));
		bindSql=(unsigned char *)malloc(len+wheLen+6);
		sprintf(bindSql,"%s FROM %s %s",Z_STRVAL_PP(&selectVal),Z_STRVAL_PP(&tableVal),Z_STRVAL_PP(&whereVal));
	}
	zend_update_property_string(model_class,getThis(),"sql",sizeof("sql")-1,bindSql TSRMLS_CC);
	char *return_sql;
	len = spprintf(&return_sql, 0, "%s", bindSql);
	RETURN_STRINGL(return_sql, len, 0);
}


PHPAPI  unsigned char *php_update_where_str(zval **value){
	char *data=Z_STRVAL_PP(value);
	int length=strlen(data)+1;
	unsigned char *result;
	result = (unsigned char *)malloc(length);
	sprintf(result,"%s",data);
	return result;
}

PHPAPI unsigned char *php_table_str(char *table){
	unsigned char *result;
	result = (unsigned char *)malloc(strlen(table)+2);
	sprintf(result,"`%s`",table);
	return result;
}


ZEND_METHOD(model_class,where){
	zend_class_entry *model;
	model=Z_OBJCE_P(getThis());
	zval *value;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"z",&value)){
		return ;
	}
	
	int status=checckType(value);
	if(status==IS_STRING){
		unsigned char *buff=php_update_where_str(&value);
		zval *old=zend_read_property(model,getThis(),"where",sizeof("where")-1,0 TSRMLS_CC);

 		unsigned char *where;
		 where=(unsigned char *)malloc(strlen(Z_STRVAL_PP(&old))+strlen(buff)+5);
		sprintf(where,"%s %s  AND ",Z_STRVAL_PP(&old),buff);

		zend_update_property_string(model,getThis(),"where",sizeof("where")-1,where TSRMLS_CC);
	}

	RETURN_ZVAL(getThis(),1,0); //return object => return $this;
}

ZEND_METHOD(model_class,set){
	zend_class_entry *model;
	model=Z_OBJCE_P(getThis());
	zval *set;
	zval *value;
	int valueLen;
	zval *bind;
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"z|z",&set,&value,&valueLen)==FAILURE){
		return ;
	}
	int status=checckType(value);
	bind=zend_read_property(model,getThis(),"set",sizeof("set")-1,0 TSRMLS_CC);
	int oldLen=strlen(Z_STRVAL_PP(&bind));
	if(status==IS_STRING){
		unsigned char *buff;
		
		int newLen=strlen(Z_STRVAL_PP(&set));
		 valueLen=strlen(Z_STRVAL_PP(&value));
		 buff=(unsigned char *)malloc(oldLen+newLen+valueLen+4);

		sprintf(buff,"%s%s = '%s',",Z_STRVAL_PP(&bind),Z_STRVAL_PP(&set),Z_STRVAL_PP(&value)) ;
		
		zend_update_property_string(model,getThis(),"set",sizeof("set")-1,buff TSRMLS_CC);
	}else if(status == IS_LONG){
		unsigned char *buff;
		int num=Z_LVAL_PP(&value);
		buff=(unsigned char *)malloc(num+oldLen);
		sprintf(buff,"%s%s='%d',",Z_STRVAL_PP(&bind),Z_STRVAL_PP(&set),num);
		 zend_update_property_string(model,getThis(),"set",sizeof("set")-1,buff TSRMLS_CC);
	}

	RETURN_ZVAL(getThis(),1,0);
}

ZEND_METHOD(model_class,insert){
	zend_class_entry *model;
	model =Z_OBJCE_P(getThis());
	zval  *array;
	HashTable *myht;
	zval **entry;
	char *string_key;
	uint string_length;
	ulong num_key;
	int num;
	char *table="";
	int tableLen;
	unsigned char *buf;
	char *SqlIns;
	char *SqlKeys;
	zval * key;
	zval *value;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"z|s",&array,&table,&tableLen)==FAILURE){
		return ;
	}

	int Type=checckType(array);
	if(Type==IS_ARRAY){
		myht=Z_ARRVAL_PP(&array);
		int count=zend_hash_num_elements(myht);
		int i=0;
		zend_hash_internal_pointer_reset(myht);
		int keyLen=0;
		int valLen=0;
		char *temp;
		for(i;i<count;i++){
			switch (zend_hash_get_current_key_ex(myht, &string_key, &string_length, &num_key, 0, NULL)) 
			{
				case HASH_KEY_IS_STRING:
					key = zend_read_property(model,getThis(),"insertkey",sizeof("insertkey") -1,0 TSRMLS_CC);
					temp=(unsigned char *)malloc(strlen(string_key)+strlen(Z_STRVAL_PP(&key)));
					sprintf(temp,"`%s`,%s",string_key,Z_STRVAL_PP(&key));
					zend_update_property_string(model,getThis(),"insertkey",sizeof("insertkey")-1,temp TSRMLS_CC);
					
					zend_hash_get_current_data(myht, (void **) &entry);
					int a=Z_TYPE_PP(entry);
					if(a==IS_STRING){
						
						value=zend_read_property(model,getThis(),"insertvalue",sizeof("insertvalue")-1,0 TSRMLS_CC);
						temp=(unsigned char *)malloc(strlen(Z_STRVAL_PP(&value))+strlen(Z_STRVAL_PP(entry)));
						sprintf(temp," '%s',%s",Z_STRVAL_PP(entry),Z_STRVAL_PP(&value));
						zend_update_property_string(model,getThis(),"insertvalue",sizeof("insertvalue")-1,temp TSRMLS_CC);
						
					}else if(a==IS_LONG){
						value=zend_read_property(model,getThis(),"insertvalue",sizeof("insertvalue")-1,0 TSRMLS_CC);
						buf=(unsigned char *)malloc(100);
						num=Z_LVAL_PP(entry);
						sprintf(buf,"%d",num);
						temp=(unsigned char *)malloc(strlen(buf)+strlen(Z_STRVAL_PP(&value)));
						sprintf(temp,"'%s',%s",buf,Z_STRVAL_PP(&value));
						zend_update_property_string(model,getThis(),"insertvalue",sizeof("insertvalue")-1,temp TSRMLS_CC);
					}					
					zend_hash_move_forward(myht);
					break;

			}
		}

		key = zend_read_property(model,getThis(),"insertkey",sizeof("insertkey")-1,0 TSRMLS_CC);
		int klen=strlen(Z_STRVAL_PP(&key));
		SqlKeys=(unsigned char *)malloc(klen);
		char *attr;
		attr=(unsigned char *)malloc(klen);
		sprintf(attr,"%s",Z_STRVAL_PP(&key));
		attr[klen-1]=' ';
		sprintf(SqlKeys,"%s",attr);

		value=zend_read_property(model,getThis(),"insertvalue",sizeof("insertvalue")-1,0 TSRMLS_CC);
		 klen=strlen(Z_STRVAL_PP(&value));
		 SqlIns=(unsigned char *)malloc(klen);
		
		attr=(unsigned char *)malloc(klen);
		sprintf(attr,"%s",Z_STRVAL_PP(&value));
		attr[klen-1]=' ';
		sprintf(SqlIns,"%s",attr);

	}


	zval *insertStr=zend_read_property(model,getThis(),"insert",sizeof("insert")-1,0 TSRMLS_CC);
	unsigned char *ins;

	if(strlen(table) > 0){
	 	unsigned char *buff=php_table_str(table);
		zend_update_property_string(model,getThis(),"table",sizeof("table")-1,buff TSRMLS_CC);
		ins=(unsigned char *)malloc(strlen(buff)+strlen(Z_STRVAL_PP(&insertStr)));
		sprintf(ins,"%s%s",Z_STRVAL_PP(&insertStr),buff);

	}else{
		zval *tables=zend_read_property(model,getThis(),"table",sizeof("table")-1,0 TSRMLS_CC);
		ins=(unsigned char *)malloc(strlen(Z_STRVAL_PP(&tables))+strlen(Z_STRVAL_PP(&insertStr)));
		sprintf(ins,"%s%s",Z_STRVAL_PP(&insertStr),Z_STRVAL_PP(&tables));
	}

	unsigned char *temp;
	temp=(unsigned char *)malloc(strlen(ins)+strlen(SqlIns)+strlen(SqlKeys)+12);

	sprintf(temp,"%s(%s) VALUES (%s)",ins,SqlKeys,SqlIns);
	zend_update_property_string(model,getThis(),"insert",sizeof("insert")-1,temp TSRMLS_CC);
	zend_update_property_long(model,getThis(),"sqlType",sizeof("sqlType")-1,4 TSRMLS_CC);
	RETURN_ZVAL(getThis(),1,0);
}

ZEND_METHOD(model_class,update){
	zend_class_entry *model;
	model=Z_OBJCE_P(getThis());
	zval *updateWhere;
	char *table="";
	int tableLen;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"|sz",&table,&tableLen,&updateWhere)==FAILURE){
		return;
	}
	
	if(strlen(table) > 0){
	 	unsigned char *buff=php_table_str(table);
		zend_update_property_string(model,getThis(),"table",sizeof("table")-1,buff TSRMLS_CC);
	}

	int status =checckType(updateWhere);

	if(status==IS_STRING){
	 	unsigned char *param=php_update_where_str(&updateWhere);
		 zval *old=zend_read_property(model,getThis(),"where",sizeof("where")-1,0 TSRMLS_CC);
		 unsigned char *where;
		 where=(unsigned char *)malloc(strlen(Z_STRVAL_PP(&old))+strlen(param)+5);
		sprintf(where,"%s %s  AND ",Z_STRVAL_PP(&old),param);
		zend_update_property_string(model,getThis(),"where",sizeof("where")-1,where TSRMLS_CC);
	}

	int t=3;
	zend_update_property_long(model,getThis(),"sqlType",sizeof("sqlType")-1,t TSRMLS_CC);

	RETURN_ZVAL(getThis(),1,0); //return object => return $this;
}



ZEND_METHOD(model_class,delete){
	zend_class_entry *model;
	model=Z_OBJCE_P(getThis());
	int t=2;
	zend_update_property_long(model,getThis(),"sqlType",sizeof("sqlType")-1,t TSRMLS_CC);
	RETURN_ZVAL(getThis(),1,0);
}

ZEND_METHOD(model_class,table){
	zend_class_entry *model;
	model=Z_OBJCE_P(getThis());
	char *table;
	int tableLen;
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s",&table,&tableLen)==FAILURE){
		return ;
	}

 	unsigned char *buff=php_table_str(table);
 	zend_update_property_string(model,getThis(),"table",sizeof("table")-1,buff TSRMLS_CC);
 	RETURN_ZVAL(getThis(),1,0);
}

ZEND_METHOD(model_class,select){
	zend_class_entry *model;
	model=Z_OBJCE_P(getThis());

	zval *value="";
	zval *sele;
	int fieldLen;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"|z",&value)==FAILURE){
		return ;
	}
	int status=checckType(value);
	sele=zend_read_property(model,getThis(),"select",sizeof("select")-1,select TSRMLS_CC);
	
	if(status==IS_STRING || status > 9){
		char *data;
		if(status > 9){
			data="*";
		}else{
			data=Z_STRVAL_PP(&value);
		}
		char buff[strlen(data)+strlen(Z_STRVAL_PP(&sele))];
		snprintf(buff,sizeof(buff)+1,"%s%s",Z_STRVAL_PP(&sele),data);
		zend_update_property_string(model,getThis(),"select",sizeof("select")-1,buff TSRMLS_CC);
	}else if(status==IS_ARRAY){

	}
	int t=1;
	zend_update_property_long(model,getThis(),"sqlType",sizeof("sqlType")-1,t TSRMLS_CC);

	RETURN_ZVAL(getThis(),1,0); //return object => return $this;
}

/* set table name*/
ZEND_METHOD(model_class,__construct){
	char *table;
	int tableLen;
	zend_class_entry *model;
	model=Z_OBJCE_P(getThis());
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",&table,&tableLen))
	{
		return ;
	}
	unsigned char *buff=php_table_str(table);
	zend_update_property_string(model,getThis(),"table",sizeof("table")-1,buff TSRMLS_CC);
}


static zend_function_entry cmodel_method[]={
	PHP_ME(model_class,__construct,NULL,ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(model_class,select,NULL,ZEND_ACC_PUBLIC)
	PHP_ME(model_class,where,NULL,ZEND_ACC_PUBLIC)
	PHP_ME(model_class,getSql,NULL,ZEND_ACC_PUBLIC)
	PHP_ME(model_class,delete,NULL,ZEND_ACC_PUBLIC)
	PHP_ME(model_class,update,NULL,ZEND_ACC_PUBLIC)
	PHP_ME(model_class,table,NULL,ZEND_ACC_PUBLIC)
	PHP_ME(model_class,set,NULL,ZEND_ACC_PUBLIC)

	PHP_ME(model_class,insert,NULL,ZEND_ACC_PUBLIC)
	// PHP_ME(model_class,or_where,NULL,ZEND_ACC_PUBLIC)
	// PHP_ME(model_class,group_by,NULL,ZEND_ACC_PUBLIC)
	{NULL,NULL,NULL}
};


PHP_MINIT_FUNCTION(CModel)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	zend_class_entry  class;
	INIT_CLASS_ENTRY(class,"CModel",cmodel_method);
	model_class=zend_register_internal_class(&class TSRMLS_CC);
	
	/** create class attr **/

	zend_declare_property_null(model_class,"table",strlen("table"),ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_string(model_class,"select",strlen("select"),"SELECT ",ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_string(model_class,"delete",strlen("delete"),"DELETE *",ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_string(model_class,"update",strlen("update"),"UPDATE ",ZEND_ACC_PUBLIC TSRMLS_CC);

	zend_declare_property_null(model_class,"sql",strlen("sql"),ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_string(model_class,"where",strlen("where"),"WHERE ",ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_string(model_class,"set",strlen("set"),"SET ",ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_string(model_class,"insert",strlen("insert"),"INSERT INTO ",ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_string(model_class,"insertkey",strlen("insertkey"),"",ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_string(model_class,"insertvalue",strlen("insertvalue"),"",ZEND_ACC_PUBLIC TSRMLS_CC);
	/**  SQL  Type  in  3:UPDATE  \2: DELETE \ 1:SELECT **/
	/** default value is SELECT **/
	zend_declare_property_long(model_class,"sqlType",strlen("sqlType"),1,ZEND_ACC_PUBLIC TSRMLS_CC);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(CModel)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(CModel)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(CModel)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(CModel)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "CModel support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_CModel_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_CModel_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "CModel", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/

/* {{{ proto string CModel(string tables)
   ; */
PHP_FUNCTION(CModel)
{
	char *tables = NULL;
	int argc = ZEND_NUM_ARGS();
	int tables_len;

	if (zend_parse_parameters(argc TSRMLS_CC, "s", &tables, &tables_len) == FAILURE) 
		return;

	php_error(E_WARNING, "CModel: not yet implemented");
}
/* }}} */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

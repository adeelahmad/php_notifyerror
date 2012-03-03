#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef P_tmpdir
#define TMPDIR P_tmpdir
#else
#define TMPDIR "/tmp"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_notifyerror.h"
#include "ext/standard/php_mail.h"
#include "ext/standard/php_dns.h"
#include <unistd.h>


static void (*orig_error_cb)(int type, const char *error_filename, const uint error_lineno, const char *format, va_list args);

static void notifyerror_error_cb(int type, const char *error_filename, const uint error_lineno, const char *format, va_list args) /* {{{ */
{
	va_list args_cp;
	char *buffer, *error_message, *header;
	int log_level = 0;
	va_copy(args_cp, args);
	vspprintf(&buffer, PG(log_errors_max_len), format, args);
	spprintf(&error_message, 0, "%s\nin %s on line %d", buffer, error_filename, error_lineno);
	efree(buffer);

	switch (type) {
		case E_CORE_ERROR:
		case E_ERROR:
		case E_COMPILE_ERROR:
		case E_USER_ERROR:
			header = "PHP Fatal Error";
			log_level = 1;
			break;
		case E_RECOVERABLE_ERROR:
			header = "PHP Recoverable Fatal Error";
			log_level = 1;
			break;
		case E_WARNING:
		case E_CORE_WARNING:
		case E_COMPILE_WARNING:
			header = "PHP Warning";
			log_level = 2;
			break;
		case E_PARSE:
			header = "PHP Parse Error";
			log_level = 1;
			break;
		case E_NOTICE:
		case E_USER_NOTICE:
			header = "PHP Notice";
			log_level = 2;
			break;
		case E_STRICT:
			header = "PHP Strict Error";
			log_level = 1;
			break;
#ifdef E_DEPRECATED
		case E_DEPRECATED:
		case E_USER_DEPRECATED:
			header = "PHP Deprecation";
			log_level = 3;
			break;
#endif
		default:
			header = "Unknown PHP Error";
			log_level = 3;
			break;
	}

	zend_bool enabled = INI_BOOL("errornotify.enable");
	char * email = INI_STR("errornotify.email");
	int currentLogLevel = INI_INT("errornotify.loglevel");
	
	

	
	if(enabled && email != "") {

		
		if(currentLogLevel >= log_level ) {
			
			char * subject;
			char * hostname[128];
			gethostname(hostname, sizeof hostname);
			php_sprintf(subject,"%s [%s]",hostname,header);
			php_mail(email,subject,error_message,NULL,NULL);
		}
		
		efree(error_message);
		
	}
	orig_error_cb(type, error_filename, error_lineno, format, args);
}
/* }}} */

PHP_INI_BEGIN()
PHP_INI_ENTRY("errornotify.enable", "0",         PHP_INI_ALL, NULL)
PHP_INI_ENTRY("errornotify.email", "", PHP_INI_ALL, NULL)
PHP_INI_ENTRY("errornotify.loglevel", "1", PHP_INI_ALL, NULL)
PHP_INI_END()


/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(notifyerror)
{
	//notify_init("php " PHP_VERSION);
	//REGISTER_INI_ENTRIES();
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(notifyerror)
{
	//notify_uninit();
	UNREGISTER_INI_ENTRIES( );
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(notifyerror)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "Author", "Adeel Ahmad(adeelahmad99[at]gmail.com)");
	php_info_print_table_row(2, "Version",  NOTIFYERROR_VERSION);
	DISPLAY_INI_ENTRIES();
	php_info_print_table_end();
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(notifyerror)
{
	REGISTER_INI_ENTRIES();
	orig_error_cb = zend_error_cb;
	zend_error_cb = notifyerror_error_cb;
    return SUCCESS;
}

/* {{{ notifyerror_module_entry
 */
zend_module_entry notifyerror_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"notifyerror",
	NULL,
	PHP_MINIT(notifyerror),
	PHP_MSHUTDOWN(notifyerror),
	PHP_RINIT(notifyerror),
	NULL,
	PHP_MINFO(notifyerror),
#if ZEND_MODULE_API_NO >= 20010901
	NOTIFYERROR_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */


#ifdef COMPILE_DL_NOTIFYERROR
ZEND_GET_MODULE(notifyerror)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

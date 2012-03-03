#ifndef PHP_NOTIFYERROR_H
#define PHP_NOTIFYERROR_H

extern zend_module_entry notifyerror_module_entry;
#define phpext_notifyerror_ptr &notifyerror_module_entry

#define NOTIFYERROR_VERSION "0.1"

static char * notifyerror_mk_filename(char * identifier, char * template);

#endif	/* PHP_NOTIFYERROR_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

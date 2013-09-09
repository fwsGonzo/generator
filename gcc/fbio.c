#include "fbio.h"
#include <stdarg.h>
#include <stdio.h>

int fbprintf(char *format, ...) {
	
	char buf[8192];
	int i;
	va_list args;
	
	va_start(args, format);
	
	i = vsnprintf((char *)buf, 8192, format, args);
	
	if (i > 0)
		logText((char *) buf);
	va_end(args);
	
	return(i);
	
}

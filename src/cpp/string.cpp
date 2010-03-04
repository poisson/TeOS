#include "string.h"

size_t strlen(const char * string)
{
	if (string == NULL) return 0;
	const char * p = string;
	while (*p != '\0') p++;
	return(p-string);
}

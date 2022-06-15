#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>
#include "aglobal.h"

int syncPrintf(const char *format, ...)
{
	int res;
	va_list args;
	asm {pushf;
	 	 cli}
	va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	asm popf;
		return res;
}

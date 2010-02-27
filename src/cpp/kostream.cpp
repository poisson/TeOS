#include "kostream.h"

fmtflags kostream::flags() const
{
	return flags;
}

fmtflags kostream::flags( fmtflags fmtfl )
{
	fmtflags temp = flags;
	flags = fmtfl;
	return temp;
}

fmtflags setf ( fmtflags fmtfl ) 
{
	fmtflags temp = flags;
	flags |= fmtfl;
	return temp;
}

fmtflags setf ( fmtflags fmtfl, fmtflags mask )
{
	fmtflags temp = flags;
	flags = (fmtfl & mask) | (flags & ~mask);
	return temp;
}

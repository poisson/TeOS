#include "kostream.h"

kostream::fmtflags kostream::flags() const
{
	return mflags;
}

kostream::fmtflags kostream::flags( fmtflags fmtfl )
{
	fmtflags temp = mflags;
	mflags = fmtfl;
	return temp;
}

kostream::fmtflags kostream::setf ( fmtflags fmtfl ) 
{
	fmtflags temp = mflags;
	mflags |= fmtfl;
	return temp;
}

kostream::fmtflags kostream::setf ( fmtflags fmtfl, fmtflags mask )
{
	fmtflags temp = mflags;
	mflags = (fmtfl & mask) | (mflags & ~mask);
	return temp;
}

void kostream::unsetf ( kostream::fmtflags mask )
{
	mflags &= (~mask);
}

streamsize kostream::precision () const
{
	return mprec; 
}

streamsize kostream::precision ( streamsize prec )
{
	streamsize temp = mprec;
	mprec = prec;
	return temp;
}

streamsize kostream::width () const
{
	return mfieldwidth;
}

streamsize kostream::width ( streamsize wide )
{
	streamsize temp = mfieldwidth;
	mfieldwidth = wide;
	return temp;
}

bool kostream::good () const
{
	return (mstate == goodbit);
}

bool kostream::eof () const
{
	return (mstate & eofbit) != 0;
}

bool kostream::fail () const
{
	return (mstate & (failbit | badbit)) != 0;
}

bool kostream::bad () const
{
	return (mstate & badbit) != 0;
}

bool kostream::operator! () const
{
	return fail();
}

kostream::operator void* () const
{
	if (fail()) return (void*)0;
	return (void*)this;
}

kostream::iostate kostream::rdstate () const
{
	return mstate;
}

void kostream::setstate ( iostate state )
{
	mstate |= state;
}

void kostream::clear ( iostate state )
{
	mstate = state;
}

char kostream::fill () const
{
	return mfill;
}
char kostream::fill ( char fillch )
{
	char temp = mfill;
	mfill = fillch;
	return temp;
}

kostream& kostream::copyfmt ( const kostream& rhs )
{
	mflags = rhs.mflags;
	mprec = rhs.mprec;
	mfieldwidth = mfieldwidth;
	mfill = mfill;
	return *this;
}

kostream& kostream::put ( char c )
{
	mbuff[mput] = c;
	mput++;
	if (mput >= buffsize)
		this->flush(); // not sure if necessary
	return *this;
}

kostream& kostream::write ( const char* s, streamsize n )
{
	while (buffsize - mput < n)
	{
		memcpy((void*)(mbuff+mput),(void*)s,buffsize);
		n -= buffsize;
		s += buffsize;
		mput = 0;
		this->flush(); // buffer is full now
	}
	if (n != 0)
	{
		memcpy((void*)(mbuff+mput),(void*)s, n);
		mput = n;
	}
	return *this;
}
// TODO: formatted output operators
kostream& operator<< (bool val)
{

}

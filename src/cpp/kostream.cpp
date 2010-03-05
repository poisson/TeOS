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
	if ((mflags & uppercase) && (c > 96 && c < 123)) c -= 32;
	mbuff[mput] = c;
	mput++;
	if (mput >= buffsize)
	{
		this->flush(); // not sure if necessary
		mput = 0;
	}
	return *this;
}

kostream& kostream::write ( const char* s, streamsize n )
{
//	while (buffsize - mput < n)
//	{
//		memcpy((void*)(mbuff+mput),(void*)s,buffsize-mput);
//		this->flush(); // buffer is full now
//		n -= buffsize;
//		s += buffsize;
//		mput = 0;
//	}
//	if (n != 0)
//	{
//		memcpy((void*)(mbuff+mput),(void*)s, n);
//		mput += n;
//		if (mflags & unitbuf) { this->flush(); mput = 0; }
//	}
	for (int i = 0; i < n; i++)
		put(s[i]);
	return *this;
}

kostream& kostream::operator<< (bool val)
{
	if (mflags & boolalpha)
	{
		if (val) write("true", 4);
		else write("false", 5);
	}
	else
	{
		if (val) put('1');
		else put('0');
	}
	return *this;
}

kostream& kostream::operator<< (unsigned long val)
{
	int digit = 1;
	int base = 10;
	const char * cbase = "";
	bool sb = (bool)(mflags & showbase); 
	if (mflags & dec) 
	{ 
		base = 10;
	}
	else if (mflags & hex) 
	{
		base = 16;
		cbase = "0x";
		digit = 2;
	}
	else if (mflags & oct) 
	{
		base = 8;
		cbase = "0";
	}
	else if (mflags & bin)
	{
		base = 2;
		cbase = "b";
	}

	if (sb) write(cbase, digit);

	char string[sizeof(unsigned long)*8];
		// could want to print in binary

	// FIXME: memset is a better way... once it works =/
	for (int i = sizeof(unsigned long)*8 - 1; i >= 0; i--)
		string[i] = '\0';

	if (val == 0)
		put('0');
	int i = sizeof(unsigned long)*8 - 1;
	while (val > 0)
	{
		digit = val % base;
		if (digit <= 9)
			string[i] = ('0' + digit);
		else string[i] = ('A' + digit - 10);
		val /= base;
		i--;
	}
	for (uint_fast16_t j = 0; j < sizeof(unsigned long)*8; j++)
	{
		if (string[j] != '\0') 
		{
			write(string+j, sizeof(unsigned long)*8-j);
			break;
		}
	}
	return (*this);	
}

kostream& kostream::operator<< (long val)
{
	if (val < 0)
	{
		val = -val;
		put('-');
	}
	else if (mflags & showpos) put('+');
	return (*this) << (unsigned long)val;
}

kostream& kostream::operator<< (short val)
{
	return (*this) << (long)val;
}

kostream& kostream::operator<< (unsigned short val)
{
	return (*this) << (unsigned long) val;
}

kostream& kostream::operator<< (int val)
{
	return (*this) << (long)val;
}

kostream& kostream::operator<< (unsigned int val)
{
	return (*this) << (unsigned long)val;
}

kostream& kostream::operator<< (long double val)
{
	val = 0;
	return (*this) << "\nLong double output not supported!\n";
}

kostream& kostream::operator<< (double val)
{
	val = 0;
	return (*this) << "Double output not implemented!";
}

kostream& kostream::operator<< (float val)
{
	val = 0;
	return (*this) << "Float output not implemented!";
}

kostream& operator<< (kostream& out, const char* s)
{
	out.write(s, strlen(s));
	return out;
}

// FIXME: Floating point.

kostream& operator<< (kostream& out, const void* val) // ENHANCE: make sure what do
{
	return out << (uintptr_t)val;
}

kostream& operator<< (kostream& out, char c)
{
	return out.put(c);
}

kostream& operator<< (kostream& out, signed char c)
{
	return out << (char)c;
}

kostream& operator<< (kostream& out, unsigned char c)
{
	return out << (char)c;
}

kostream& operator<< (kostream& out, const signed char * s)
{
	return out << (const char*)s;
}

kostream& operator<< (kostream& out, const unsigned char * s)
{
	return out << (const char*)s;
}

kostream::kostream() : mput(0), mstate(goodbit), mfill(' ') {};

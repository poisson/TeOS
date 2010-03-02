#ifndef __TEOS_KOSTREAM_H__
#define __TEOS_KOSTREAM_H__

extern "C"
{
#include <stdint.h>
}

typedef int_fast32_t  streamsize;

class kostream
{
public:
	// Typedefs go here
	// {
	typedef uint_fast32_t fmtflags;
	typedef uint_fast8_t  iostate;
	typedef uint_fast8_t  openmode;
	typedef uint_fast8_t  seekdir;
	//}
	
	// Constants go here
	// {
	// fmtflags constants
	// {
	static const fmtflags boolalpha		= 0x0001;
	static const fmtflags showbase		= 0x0002;
	static const fmtflags showpoint		= 0x0004;
	static const fmtflags showpos 		= 0x0008;
	static const fmtflags skipws		= 0x0010;
	static const fmtflags unitbut		= 0x0020;
	static const fmtflags uppercase		= 0x0040;
	static const fmtflags dec			= 0x0080;
	static const fmtflags hex			= 0x0100;
	static const fmtflags oct			= 0x0200;
	static const fmtflags fixed			= 0x0400;
	static const fmtflags scientific	= 0x0800;
	static const fmtflags internal		= 0x1000;
	static const fmtflags left 			= 0x2000;
	static const fmtflags right			= 0x4000;
	static const fmtflags adjustfield	= left | right | internal;
	static const fmtflags basefield		= dec | oct | hex;
	static const fmtflags floatfield	= scientific | fixed;
	// }
	// iostate constants
	// {
	static const iostate eofbit		= 0x01;
	static const iostate failbit 	= 0x02;
	static const iostate badbit		= 0x04;
	static const iostate goodbit	= 0x00;
	// }
	// openmode constants
	// {
	static const openmode app 		= 0x01;
	static const openmode ate		= 0x02;
	static const openmode binary	= 0x04;
	static const openmode in		= 0x08;
	static const openmode out		= 0x10;
	static const openmode trunc		= 0x20;
	// }
	// seekdir constants
	// {
	static const seekdir beg	= 0x01;
	static const seekdir cur	= 0x02;
	static const seekdir end	= 0x04;
	// }
	// }
	
	// Functions go here
	// {
	fmtflags flags () const; // get format flags
	fmtflags flags ( fmtflags fmtfl ); // set format flags
	fmtflags setf ( fmtflags fmtfl ); // set some format flags
	fmtflags setf ( fmtflags fmtfl, fmtflags mask );
	void unsetf ( fmtflags mask ); // clear the bits that are set in mask
	streamsize precision () const; // get precision
	streamsize precision ( streamsize prec ); // set precision
	streamsize width () const; // get feildwidth
	streamsize width ( streamsize wide ); // set feildwidth
	bool good () const;
	bool eof () const;
	bool fail () const;
	bool bad () const;
	bool operator! () const;
	operator void* () const;
	iostate rdstate () const;
	void setstate ( iostate state);
	void clear ( iostate state = goodbit );
	char fill () const;
	char fill ( char fillch );
	kostream& copyfmt ( const kostream& rhs );
	kostream& put ( char c );
	kostream& write ( const char* s, streamsize n );
	virtual kostream& flush () = 0;
	// Formatted output functions go here
	// {
	kostream& operator<< (bool val);
	kostream& operator<< (short val);
	kostream& operator<< (unsigned short val);
	kostream& operator<< (int val);
	kostream& operator<< (unsigned int val);
	kostream& operator<< (long val);
	kostream& operator<< (unsigned long val);
	kostream& operator<< (float val);
	kostream& operator<< (double val);
	kostream& operator<< (long double val);
	kostream& operator<< (const void* val);
	friend kostream& operator<< (kostream& out, char c);
	friend kostream& operator<< (kostream& out, signed char c);
	friend kostream& operator<< (kostream& out, unsigned char c);
	friend kostream& operator<< (kostream& out, const char* s);
	friend kostream& operator<< (kostream& out, const signed char* s);
	friend kostream& operator<< (kostream& out, const unsigned char* s);
	// }
	// }
protected:

private:
	fmtflags mflags;
	streamsize mprec;
	streamsize mfieldwidth;
	iostate mstate;
	char mfill;
	static const int_fast8_t buffsize = 32;
	char[buffsize] mbuff;
	int_fast8_t mput;
};

#endif

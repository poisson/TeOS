#ifndef __TEOS_TEXTMODE_H__
#define __TEOS_TEXTMODE_H__

extern "C"
{
	#include <stdint.h>
}

// flags go here

class vterm
{
public:
	vterm(unsigned char* const address, unsigned char modifier, unsigned int base = 10, unsigned char flags = 0, unsigned int width = 80, unsigned int height = 25);
	void setflags(unsigned char flags);
	void setintbase(unsigned int base);
	unsigned int getintbase();
	void setmodifier(unsigned char modifier);
	void scroll(int lines);

	friend vterm& operator<<(vterm& out, const char* in); // string
	vterm& operator<< (const int& val ); // int
	//friend vterm& operator<<(vterm& out, double in ); // double
protected:

private:
	unsigned char flags;
	unsigned char modifier;
	unsigned char* const address;
	int currentx;
	int currenty;
	void printchar(char c);
	void makecursorsane();
	const int width;
	const int height;
	unsigned int base;
};

vterm& operator<< (vterm& out, const char* in); // string
//vterm& operator<<(vterm& out, double in ); // double

#endif

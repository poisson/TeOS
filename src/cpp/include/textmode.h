#ifndef __TEOS_TEXTMODE_H__
#define __TEOS_TEXTMODE_H__

extern "C"
{
	#include <stdint.h>
}
#include "kostream.h"
// flags go here

class vterm : public kostream
{
public:
	vterm(unsigned char* const address, unsigned char modifier, unsigned int base = 10, fmtflags flags = 0, unsigned int width = 80, unsigned int height = 25);
	void scroll(int lines);
	~vterm();
protected:

private:
	int currentx;
	int currenty;
	void printchar(char c);
	void makecursorsane();
	virtual kostream& flush();
	unsigned char * address;
	unsigned char modifier;
	const int width;
	const int height;
};

//vterm& operator<<(vterm& out, double in ); // double

#endif

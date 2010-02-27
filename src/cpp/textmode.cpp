#include "textmode.h"
#include "memory.h"

vterm::vterm(unsigned char * const address, unsigned char modifier, unsigned int base,
	unsigned char flags, unsigned int width, unsigned int height) : flags(flags), modifier(modifier), address(address), width(width), height(height), base(base)
{

}

void vterm::setflags(unsigned char flags)
{
	this->flags = flags;
}

void vterm::setintbase(unsigned int base)
{
	this->base = base;
}

unsigned int vterm::getintbase()
{
	return base;
}

void vterm::setmodifier(unsigned char modifier)
{
	this->modifier = modifier;
}

void vterm::scroll(int lines)
{
	unsigned char * dest = address;
	unsigned char zero [width*2];
	for (int i = 0; i < width*2; i++)
	{
		zero[i] = 0;
	}
	for (int i = 0; i < height - lines; i++)
	{
		memcpy(dest, dest + (lines*width*2), width*2);
		dest += width*2;
	}
	for (int i = 0; i < lines; i++)
	{
		memcpy(dest, zero, width*2);
		dest += width*2;
	}
	currenty -= lines;
}

void vterm::makecursorsane()
{
	if (currentx >= width)
	{
		currentx = 0;
		currenty++;
	}
	while (currenty >= height)
	{
		scroll(3);
	}
	if (currentx < 0)
	{
		currentx = 0;
	}
}

void vterm::printchar(char c)
{
	int pos = ((currenty*width) + currentx) * 2;
	address[pos+1] = modifier;
	switch ( c )
	{
	case 7: // backspace
		currentx--;
		break;
	case 9: // tab
		// TODO: Make tab work
		break;
	case '\n':
		currenty++;
	case '\r': // falls through from above when \n
		currentx = 0;
		break;
	case 127: // delete
		address[pos] = ' ';
		break;
	default:
		address[pos] = c;
		currentx++;
	}
	makecursorsane();
}

vterm& operator<<(vterm& out, const char* in)
{
	int i = 0;
	while (in[i] != '\0')
	{
		out.printchar(in[i]);
		out.makecursorsane();
		i++;
	}
	return out;
} // string

vterm& vterm::operator<< (const int& val ) // int
{
	int in = val;
	int digit = 0;
	if (in < 0)
	{
		printchar('-');
		in = -in;
	}

	char string[sizeof(int)*8] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
		'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };
		// could want to print in binary

	if (in == 0)
		printchar('0');
	int i = 9;
	while (in > 0)
	{
		digit = in % base;
		if (digit <= 9)
			string[i] = ('0' + digit);
		else string[i] = ('A' + digit - 10);
		in /= base;
		i--;
	}
	for (int j = 0; j < 10; j++)
	{
		if (string[j] != '\0') printchar(string[j]);
	}
	return (*this);
} // int

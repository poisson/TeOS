#include "textmode.h"
#include "memory.h"

vterm::vterm(unsigned char * const address, unsigned char modifier, unsigned int base,
	fmtflags flags, unsigned int width, unsigned int height) : kostream(), address(address), modifier(modifier), width(width), height(height) 
{
	if (base == 10) flags |= dec;
	else if (base == 16) flags |= hex;
	else if (base == 2) flags |= bin;
	else if (base == 8) flags |= oct;
	else flags |= dec;
	this->flags(flags);
}

void vterm::scroll(int lines)
{
	unsigned char * dest = address;
	unsigned char zero [width*2]; // FIXME: Memset... =/
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

kostream& vterm::flush()
{
	for (int i = 0; i < mput; i++)  
		printchar(mbuff[i]);
	mput = 0;
	return (*this);
}

vterm::~vterm()
{
	this->flush();
}

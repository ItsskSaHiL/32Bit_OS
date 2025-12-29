#ifndef IO_H
#define IO_H

// Implement the IO Function Parameter
unsigned char insb(unsigned short port);
unsigned short insw(unsigned short port);

unsigned char outb(unsigned short port,unsigned char val);
unsigned short outw(unsigned short port,unsigned short val);

#endif
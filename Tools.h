// Tools.h - collection of tools
// Used by: Rpi project
//          Ubuntu projects
//          RpiIotClient
//          ESP projects
//
// Created:  04/03/2019, Ward Dewerchin
// Modified:

#ifndef TOOLS_H_
#define TOOLS_H_

#include <string>
#include <ctime>

using namespace std;

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <memory.h>
#include <math.h>

void AsciiHexByte2Bin(unsigned char * spPtr);
size_t PreProcessHtmlString (unsigned char * spVal);
unsigned char * Bin2AsciiHex(unsigned char * pIn, unsigned char * pOut, unsigned int iInBufLength);
unsigned char * AsciiHex2Bin(unsigned char * sIn, unsigned char * pOut, unsigned int iBufferLength);
void   HexDump   (unsigned char * sBuffer, int iBufferLength);
void   DumpBuffer(unsigned char * buffer, int iLength);
char * HexBuffer (unsigned char * buffer, int iLength);
string ErrnoVerbose();
char * itos   (int iInt);
char * itos_0x(int iInt);
string DateTime();
string SecOfDay();
string MinOfDay();
string MinOfMonth();
#endif // TOOLS_H_

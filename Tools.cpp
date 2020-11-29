// Tools.h - collection of tools
// Used by: Rpi project
//          Ubuntu projects
//          RpiIotClient
//          ESP projects
//
// Created:  04/03/2019, Ward Dewerchin
// Modified:

#include "Tools.h"

// Convert Ascii byte Hex to Binary: e.g. "Ab" -> 0xab
void AsciiHexByte2Bin(unsigned char * spPtr)
{
   char c;
   c = (char) toupper(*(spPtr  )); c < 0x3A ? *spPtr  = (c - 0x30) << 4 : *spPtr  = (c - 0x37) << 4;
   c = (char) toupper(*(spPtr+1)); c < 0x3A ? *spPtr += (c - 0x30)      : *spPtr += (c - 0x37)     ;
}

size_t PreProcessHtmlString (unsigned char * spVal)
{
   unsigned char * spPtr;
   // Standard content type implementation in ANSI C (for re-use in c on embedded Linux)
   // Replace all '%xx' values with corresponding ASCII value
   // Input is ASCII Hex encoded for special characters with:
   // - 0x25 ('%') flags the following character as special
   // - 0x2B ('+') for spaces

   // Hex dump example - spVal contains in this case: 61 2B 25 32 42 2B 62 2B 25 43 33  25 41 39 2B 25 32 36 26 00 -> "a + b é &"

   // 2D 26 65 73 74 61 62 6C  6E 61 6D 65 3D 61 2B 25    -&establname=a+%
   // 32 42 2B 62 2B 25 43 33  25 41 39 2B 25 32 36 26    2B+b+%C3%A9+%26&

   // replace all '+' with ' '
   while ((spPtr = (unsigned char *) strchr((char *) spVal, '+')) != nullptr)
   {
      * spPtr = ' ';
   }
   spPtr = spVal;
   while ((spPtr = (unsigned char *) strchr((char *) spPtr, '%')) != nullptr)
   {
      memcpy (spPtr, (spPtr + 1), strlen((char *) (spPtr + 1)) + 1);         // Shift left one byte (include 0x00 termination) - remove '%'
      // Convert from AsciiHex to Binary
      AsciiHexByte2Bin(spPtr);
      if (*spPtr == (char) 0xC2)
      {
         memcpy (spPtr, (spPtr + 3), strlen((char *) (spPtr + 3)) + 1);      // Shift left three bytes - remove 43 32 25 ("C2%")
         AsciiHexByte2Bin(spPtr);
      }
      if (*spPtr == (char) 0xC3)
      {
         memcpy (spPtr, (spPtr + 3), strlen((char *) (spPtr + 3)) + 1);      // Shift left three bytes - remove 43 33 25 ("C3%")
         AsciiHexByte2Bin(spPtr);
         * spPtr = * spPtr + 0x40;
      }
      memcpy (spPtr + 1, (spPtr + 2), strlen((char *) (spPtr + 2)) + 1);     // Shift left on byte - remove second Ascii byte (e.g 32 42 ("2B") , remove 42)
      ++spPtr;
   }
   return (strlen((char *) spVal));
}


unsigned char * Bin2AsciiHex(unsigned char * pIn, unsigned char * pOut, unsigned int iInBufLength)
{
   for (unsigned int i=0; i < iInBufLength * 2; i+=2)
   {
      *(pOut+i  ) = (*(pIn+i/2) & 0xf0) >> 4;
      *(pOut+i+1) = (*(pIn+i/2) & 0x0f);

      if (*(pOut+i  ) < 0x0A) *(pOut+i  ) += 0x30; else *(pOut+i  ) += 0x37;
      if (*(pOut+i+1) < 0x0A) *(pOut+i+1) += 0x30; else *(pOut+i+1) += 0x37;

   }
   * (pOut + iInBufLength * 2) = 0x00;
   return(pOut);
}

// Convert Ascii Hex to Binary: e.g. "AbC1234" -> 0xab,0xc1,0x23,0x40
unsigned char * AsciiHex2Bin(unsigned char * sIn, unsigned char * pOut, unsigned int iBufferLength)
{
   unsigned char * pOriOut = pOut;
   unsigned char c;
   unsigned char * i;

   memset(pOut, 0, iBufferLength/2);
   * pOut = ('A' - 0x30);

   for (i = sIn; i < sIn + iBufferLength; i+=2)
   {
      if ( (c = toupper(*(i  ))) != 0) { if (c < 0x3A) *pOut =  (c - 0x30) << 4; else *pOut  = (c - 0x37) << 4; }
      if ( (c = toupper(*(i+1))) != 0) { if (c < 0x3A) *pOut += (c - 0x30)     ; else *pOut += (c - 0x37)     ; }
      ++pOut;
   }
   return (pOriOut);
}

void HexDump(unsigned char * sBuffer, int iBufferLength)
{
int i,j;
char c,c1,c2;

   for (i = 0; i < (iBufferLength/16+1); ++i)
   {
      for (j = 0; j < 16; ++j)
      {
         if (j == 8) printf(" ");
         if (i * 16 + j < iBufferLength)
         {
            c = * (sBuffer + (i * 16 + j));
            c1 = ((c >> 4) & 0x0f) + 0x30;
            if (c1 > 0x39) c1 += 0x07;
            c2 = (c & 0x0f) + 0x30;
            if (c2 > 0x39) c2 += 0x07;
            printf("%c%c ", c1,c2);
         }
         else
         {
            printf("   ");
         }
      }
      printf ("   ");
      for (j= 0; j < 16; ++j)
      {
         if (i * 16 + j < iBufferLength)
         {
            c = * (sBuffer + (i * 16 + j));
            if (! (c >= 0x20 && c <= 0x7F)) c = 0x2e;
            printf("%c", c);
         }
         else
         {
            printf (" ");
         }
      }
      printf("\n");
   }
}

void DumpBuffer(unsigned char * buffer, int iLength)
{
for (int i = 0; i < iLength; ++i) printf("%02X ", buffer[i]);
} 

char * HexBuffer(unsigned char * buffer, int iLength)
{
static char sBuffer[512];
memset(sBuffer, 0x00, 512);
for (int i = 0; i < iLength; ++i) sprintf(sBuffer + 2*i, "%02X ", buffer[i]);
return sBuffer;
}

string ErrnoVerbose()
{
   string sErrnoVerbose;
   switch (errno)
   {
      case EAGAIN:
         sErrnoVerbose = "The socket is marked nonblocking and the receive operation would block, or a receive timeout had been set and\nthe timeout expired before data was received. POSIX.1-2001 allows either error to be returned for this case,\nand does not require these constants to have the same value, so a portable application should check for both\npossibilities.";
         break;

      case EBADF:
         sErrnoVerbose = "The argument sockfd is an invalid descriptor.";
         break;

      case ECONNREFUSED:
         sErrnoVerbose = "A remote host refused to allow the network connection (typically because it is not running the requested service).";
         break;

      case EFAULT:
         sErrnoVerbose = "The receive buffer pointer(s) point outside the process's address space.";
         break;

      case EINTR:
         sErrnoVerbose = "The receive was interrupted by delivery of a signal before any data were available; see signal(7).";
         break;

      case EINVAL:
         sErrnoVerbose = "Invalid argument passed.";
         break;

      case ENOMEM:
         sErrnoVerbose = "Could not allocate memory for recvmsg().";
         break;

      case ENOTCONN:
         sErrnoVerbose = "The socket is associated with a connection-oriented protocol and has not been connected (see connect(2) and accept(2)).";
         break;

      case ENOTSOCK:
         sErrnoVerbose = "The argument sockfd does not refer to a socket.";
         break;

      default:
         sErrnoVerbose = "Undefined";
         break;

   }
   return (sErrnoVerbose);
}

char * itos(int iInt)
{
   static char spOut[16];
   sprintf(spOut, "%1i", iInt);
   return (spOut);
}

char * itos(int iInt, const char * sFormat)
{
   static char spOut[16];
   int iLength;
   iLength = strlen(sFormat) - (iInt == 0 ? 0 : (int) log10(iInt)) - 1; // calculate the length of the integer (allow for alignment/leading zeros)
   if (iLength < 0) iLength = 0;                                        // protect against underruns (e.g. format string to short)
   memcpy(spOut, sFormat, strlen(sFormat));                             // copy the format
   sprintf(spOut + iLength, "%i", iInt);                                // convert int to string using iLength as start position

   return spOut;
}
char * itos_0x(int iInt)
{
   static char spOut[16];
   sprintf(spOut, "0x%x", iInt);
   return (spOut);
}

// Returns current date and time in YYYYMMDDhhmmss format
string DateTime()
{
   string sDT;
   time_t tCT = time(0);               // Current Time
   tm*    tLT = localtime(&tCT);       // Local Time
   sDT  = itos(tLT->tm_year + 1900, "0000");
   sDT += itos(tLT->tm_mon,         "00");
   sDT += itos(tLT->tm_mday,        "00");
   sDT += itos(tLT->tm_hour,        "00");
   sDT += itos(tLT->tm_min,         "00");
   sDT += itos(tLT->tm_sec,         "00");
   return (sDT);
}

// Returs the number of minutes since 00:00
string SecOfDay()
{
   string sM;
   time_t tCT = time(0);               // Current Time
   tm*    tLT = localtime(&tCT);       // Local Time
   sM = itos(tLT->tm_hour * 60 * 60 + tLT->tm_min * 60 + tLT->tm_sec);
   return (sM);
}

// Returs the number of minutes since 00:00
string MinOfDay()
{
   string sM;
   time_t tCT = time(0);               // Current Time
   tm*    tLT = localtime(&tCT);       // Local Time
   sM = itos(tLT->tm_hour * 60 + tLT->tm_min);
   return (sM);
}
// Returs the number of minutes since 00:00
string MinOfMonth()
{
   string sM;
   time_t tCT = time(0);               // Current Time
   tm*    tLT = localtime(&tCT);       // Local Time
   sM = itos((tLT->tm_mday - 1) * 1440 + tLT->tm_hour * 60 + tLT->tm_min);
   return (sM);
}



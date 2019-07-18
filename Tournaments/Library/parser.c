#include <stdio.h>
#include <ctype.h>

int getnextc (FILE* in)
{
  char c;
  while ((c = getc(in)) != EOF) {
    if (!isspace (c)) {
      return c;
    }
  }
  return EOF;
}


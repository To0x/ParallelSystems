#include "common.h"
#include <ctype.h>

int isnumeric(char *str)
{
  while(*str)
  {
    if(!isdigit(*str))
      return 0;
    str++;
  }

  return 1;
}
#pragma once

#include "includes.h"

char *atos(char *);

char *atos(char *source)
{
  char *dest = malloc(strlen(source) + 1);
  strcpy(dest, source);
  return dest;
}

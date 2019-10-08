/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */
#ifndef BASIC_H
#define BASIC_H
#include "basic_IO.h"

#define MAX_LINE		80 /* 80 chars per line, per command */
#define SHOULD_RUN 1

int main(void)
{
  lsh_loop();
  
	return EXIT_SUCCESS;

}
#endif
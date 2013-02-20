#ifndef __MISC_H__
#define __MISC_H__

#include <stdio.h>
#include <stdlib.h>
// errors:
#include <errno.h>
#include <string.h>

#define IF_ERROR(_message)  do {\
	if (err != 0) {\
		fprintf (stderr, "ERROR : %d with %s: %s\n", err, _message, strerror (errno));\
		exit (EXIT_FAILURE); }} while (0);

#endif /* __MISC_H__ */

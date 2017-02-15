/**
 * @file utils.c
 * Implementation of utility functions
 *
 * @author Thomas M. Galla <galla@technikum-wien.at>
 * @date 2014/12/14
 *
 * @version $Revision$
 *
 * @todo Nothing to do. - Everything perfect! ;-)
 *
 * URL: $HeadURL$
 *
 * Last Modified: $Author$
 */

/*
 * -------------------------------------------------------------- includes --
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "utils.h"

/*
 * ------------------------------------------------------------- functions --
 */

/**
 *
 * \brief Print error message to stderr.
 *
 * This function prints the given error message to stderr. - If errno
 * is different from 0, the corresponding error string is printed as
 * well.
 *
 * \param msg Message to print. (IN)
 *
 * \return None
 *
 * \note This functions uses the global variables \a errno and \a cmd
 *
 */
void PrintError(
    const char *msg
    )
{
    if (errno != 0)
    {
	/*
	 * Error handling does not make sense here, since where are we going
	 * to write the error message to if output to stderr does not work ...
	 */
        (void) fprintf(
	    stderr,
	    "%s: %s - %s\n",
	    cmd,
	    msg,
	    strerror(errno)
	    );
    }
    else
    {
	/*
	 * Error handling does not make sense here, since where are we going
	 * to write the error message to if output to stderr does not work ...
	 */
        (void) fprintf(
	    stderr,
	    "%s: %s\n",
	    cmd,
	    msg
	    );
    }
}


/**
 *
 * \brief Print error message and bail out.
 *
 * This function prints the given error message to stderr, frees all
 * allocated resources by calling \a FreeResources() and terminates the
 * program.
 *
 * \param msg Message to print. (IN)
 *
 * \return None
 *
 */
void BailOut(
    const char *msg
    )
{
    PrintError(msg);
    FreeResources();
    exit(EXIT_FAILURE);
}

/*
 * =================================================================== eof ==
 */
  

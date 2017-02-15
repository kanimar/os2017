/**
 * @file utils.h
 * Interface of utility functions
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

#ifndef _UTILS_H_
#define _UTILS_H_

/*
 * -------------------------------------------------------------- includes --
 */

/*
 * ---------------------------------------------------- external variables --
 */

/**
 * Pointer to a string containing the command name
 */
extern const char *cmd;

/*
 * ---------------------------------------------------- external functions --
 */

/**
 *
 * \brief Free all resources.
 *
 * This function frees all resources that have been allocated by the program.
 *
 * \return None
 *
 * \note This function has to be provided by the module using utils.[ch]
 *
 */
extern void FreeResources(
    void
    );

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
extern void PrintError(
    const char *msg
    );


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
extern void BailOut(
    const char *msg
    );

#endif /* _UTILS_H_ */

/*
 * =================================================================== eof ==
 */

/**
 * @file myfgrep.c
 * Main module of myfgrep
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
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "utils.h"

/*
 * --------------------------------------------------------------- defines --
 */

/**
 * Maximum length of an input line
 */
#define MAX_LINE 256

/*
 * --------------------------------------------------------------- globals --
 */

/**
 * Pointer to a string containing the command name
 */
const char *cmd = "<not yet set>";

/**
 * Pointer to currently opened file
 */
static FILE *fpOpenFile = NULL;

/*
 * ------------------------------------------------------------- functions --
 */

/**
 *
 * \brief Print usage info to stderr and exit.
 *
 * This function prints the usage information and exits the program
 *
 * \return None
 *
 * \note This functions uses the global variables \a szCommand
 *
 */
static void PrintUsageAndExit(
    void
    )
{
    (void) fprintf(stderr, "USAGE: %s [-h] [-v] <textstring> ...\n", cmd);

    exit(EXIT_FAILURE);
}


/**
 *
 * \brief Free all resources.
 *
 * This function frees all resources that have been allocated by the program.
 *
 * \return None
 *
 */
void FreeResources(
    void
    )
{
    /*
     * only close file if has previously been opened
     */
    if (fpOpenFile != NULL)
    {
	/*
	 * no need to handle return value - fp only opened for reading
	 */
        (void) fclose(fpOpenFile);
        fpOpenFile = NULL;
    }

    /*
     * force flushing of remainder of buffer for stdout and check
     * whether this is successful
     */
    if (fflush(stdout) == EOF)
    {
        BailOut("Cannot flush stdout");
    }
}


/**
 *
 * \brief Print a given line.
 *
 * This function prints the given line to stdout. If \a bShowFilename is
 * different from 0, the line given by \a szLine is preceeded by the
 * filename given by \a szFileName and a colon.
 *
 * \param fileName Filename to print. (IN)
 * \param line Line to print. (IN)
 * \param showFilename Boolean, whether filename is to be printed. (IN)
 *
 * \return None
 *
 */
static void PrintLine(
    const char *fileName,
    const char *line,
    int showFilename
    )
{
    if (showFilename)
    {
        if (printf("%s: %s", fileName, line) < 0)
	{
            BailOut("Cannot print to stdout");
        }
    }
    else
    {
        if (printf("%s", line) < 0)
	{
            BailOut("Cannot print to stdout");
        }
    }
}
   

/**
 *
 * \brief Handle the given file.
 *
 * This function processes the supplied file line by line.
 *
 * \param fileName Filename to process. (IN)
 * \param fp Pointer to opened file to process. (IN)
 * \param searchString String to search for in each line of file. (IN)
 * \param reverse Boolean, indicating whether to reverse the search. (IN)
 * \param showFilename Boolean, indicating whether filename is to be
 *                     printed. (IN)
 *
 * \return None
 *
 */
static void HandleFile(
    const char * const fileName,
    FILE *fp,
    const char * const searchString,
    int reverse,
    int showFilename
    )
{
    char buffer[MAX_LINE];

    /*
     * process all lines of file
     */
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        /*
         * line too long (i.e., not '\n' terminated)?
         */
        if (buffer[strlen(buffer)- 1] != '\n')
	{
            PrintError("Line too long - skipping rest of file");
            return;
        }

        /*
         * string found in current line?
         */
        if (strstr(buffer, searchString) == NULL)
	{
            if (reverse)
	    {
                PrintLine(fileName, buffer, showFilename);
            }
        }
	else
	{
            if (!reverse)
	    {
                PrintLine(fileName, buffer, showFilename);
            }
        }
    }

    /*
     * read error?
     */
    if (ferror(fp))
    {
        PrintError("Read error - skipping rest of file");
        return;
    }
}



/**
 *
 * \brief Main entry point of program.
 *
 * This function is the main entry point of the program.
 *
 * \param argc Number of command line arguments. (IN)
 * \param argv Array of command line arguments. (IN)
 *
 * \return The function returns whether or not the program executed
 *         successfully.
 * \retval 0 Successful program termination
 * \retval 1 Failed program termination
 *
 */
int main(
    int argc,
    char *argv[]
    )
{
    const char *searchString;
    int c;
    int optionH = 0;
    int optionV = 0;

    cmd = argv[0];

    /*
     * process options
     */
    while ((c = getopt(argc, argv, "hv")) != EOF)
    {
        switch(c)
	{
        case 'h':
            /* fgrep accepts more than one -h */
            optionH = 1;
            break;

        case 'v':
            /* fgrep accepts more than one -v */
            optionV = 1;
            break;

        case '?':
            PrintUsageAndExit();
            break;

        default:
            assert(0);
            break;
        }
    }

    /*
     * check for number of remaining args
     */
    if (argc < optind + 1)
    {
        PrintUsageAndExit();
    }
   
    searchString = argv[optind];

    /*
     * filename(s) given? - if not use stdin
     */
    if (argc == optind + 1)
    {
        HandleFile("stdin", stdin, searchString, optionV, 0);
    }
    /*
     * otherwise process neach file one by one
     */
    else
    {
        int i;
        const int printFilename = ((optind + 2) == argc) ? 0 : !optionH;

        /*
         * iterate over all files
         */
        for (i = optind + 1; i < argc; i++)
	{
            const char * const fileName = argv[i];
           
            if ((fpOpenFile = fopen(fileName, "r")) == NULL)
	    {
                PrintError("Cannot open file");
            }
	    else
	    {
                HandleFile(
		    fileName,
		    fpOpenFile,
		    searchString,
		    optionV,
		    printFilename
		    );
		/*
		 * no need to handle return value - fp only opened for reading
		 */
                (void) fclose(fpOpenFile);
		fpOpenFile = NULL;
            }
        }
    }

    FreeResources();

    exit(EXIT_SUCCESS);
}

/*
 * =================================================================== eof ==
 */
  

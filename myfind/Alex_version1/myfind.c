/**
 * @file myfind.c
 * Main module of myfind
 *
 * @author Alexander Pirka <alexander.pirka@technikum-wien.at>
 * @date 2017/02/26
 *
 * @version $Draft$
 *
 * @todo 
 * - Solve Path Problems for lstat
 * - Stop Recursion
 * - Find solution for passing arguments
 * - Concept for Syscalls/library functions for actions
 *
 * URL: $HeadURL$
 *
 * Last Modified: $Author$
 */

/*
 * -------------------------------------------------------------- includes --
 */
#include <stdio.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

/*
 * --------------------------------------------------------------- defines --
 */
 
 /*
 * --------------------------------------------------------------- globals --
 */
 /**
 * Integer number of parameters
 */
int parms_count;
 
 /*
 * ------------------------------------------------------------- functions --
 */
 void do_file(const char* file_name, const char* const* parms);
 void do_dir(const char* dir_name, const char* const* parms);


/**
 *
 * \brief iterate over all files starting at current working directory
 *
 * This function iterates over all files. Printf filename and passing each file of the
 * the directory to do_find
 *
 * \param dir_name Name of directory. It should be called first with the current directory "."
 * \param parms Pointer to the passed arguments
 *
 * \return None
 *
 * \note $Author$ do not know how to handle recursion and pass correct path for do_find
 *
 */
void do_dir(const char* dir_name, const char* const* parms)
{
DIR* directory;
struct dirent* dirent_str;
static int fct_call_counter;
char* cwd = getenv("PWD");
char* slash = "/";
strcat(cwd,slash);
strcat(cwd, dir_name);

(fct_call_counter == 0) ? (directory = opendir(".")) : (directory = opendir(cwd)); /*At the first function call, start at current working directory*/
fct_call_counter++;


        if (directory == NULL)
        {
         printf("Error Code: %d \n", errno);
			exit(EXIT_FAILURE);
        }
        else
        {
                do
                {
                        if ((dirent_str = readdir(directory)) != NULL)
                        {
                        	printf("%s ",dirent_str->d_name);
									do_file(dirent_str->d_name, parms);
                        }
                } while (dirent_str != NULL);
                exit(EXIT_SUCCESS);
        }
}
/**
 *
 * \brief Check the given file
 *
 * This function checks the given file with lstat
 *
 * \param file_name Name of the file (could be directory as well)
 * \param parms Pointer to the passed arguments
 *
 * \return None
 *
 * \note None
 *
 */
void do_file(const char* file_name, const char* const* parms)
{
	errno = 0;
	struct stat buf;

	if (lstat(file_name, &buf) < 0)
	{
		printf("Error Code: %d \n", errno);
		exit(EXIT_FAILURE);
	}
	
	switch (buf.st_mode & S_IFMT) 
	{
		case S_IFDIR: printf("....d\n"); do_dir(file_name, parms); break;
		case S_IFREG: printf("....f\n"); break;	
		default: printf("....unkown\n"); break;
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
int main(int argc, const char* const argv[])
{
const char* const* actions;
parms_count = argc;
const char* search_string;

        if (argc == 1)
        { printf("Usage %s [name]", argv[0]);
        return 1;
        }
       
actions = argv;
search_string = argv[1];

do_dir(search_string, actions);

return 0;
}
/*
 * =================================================================== eof ==
 */

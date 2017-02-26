/**
 * @file myfind.c
 * Main module of myfind
 *
 * @author Alexander Pirka <alexander.pirka@technikum-wien.at>
 * @date 2017/02/26
 *
 * @version $Draft$
 *
 * @todo Solve Path Problem
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
/**
 * Pointer to the actual search file/directory passed as argv[1]
 */
 
 /*
 * ------------------------------------------------------------- functions --
 */

const char* search_string;

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
char* path = malloc(sizeof(char)*1024);

        directory = opendir(dir_name);

        if (directory == NULL)
        {
                printf("consult errno\n");
                closedir(directory);
        }
        else
        {
                do
                {
                        if ((dirent_str = readdir(directory)) != NULL)
                        {
                        	printf("%s ",dirent_str->d_name);
                        	get_pwd(dirent_str->d_name, dir_name, path);
									printf("%s", path);
									do_file(path, parms);
                        }
                } while (dirent_str != NULL);
                closedir(directory);
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
 * \brief Creating absolute path to given filename
 *
 * This function creates an absolute path to given filename and store path to \a bpp
 *
 * \param name Name of file or directory
 * \param dir Name of dir
 * \param pp Buffer to store path after creation is finished
 *
 * \return None
 *
 * \note $Author$ is wondering if this is really necessary
 *
 */
void get_pwd(char* name, const char* dir, char* pp)
{
	char* slash = "/";
	char path[1024];
	char* r;
	
		r = getcwd(path, sizeof(path)); 
		if (r == NULL) exit(EXIT_FAILURE);
		
		strcat(path, slash);
		strcat(path, dir);
      strcat(path, slash);
		strcat(path, name); 
		strcpy(pp, path);
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

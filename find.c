/**
 * @file myfind.c
 * Betriebssysteme myfind File.
 * Beispiel 1
 *
 * @author Alexander Pirka <ic16b031@technikum-wien.at>
 * @author Maria Karnikova <ic16b002@technikum-wien.at>
 * @author Christian Fuhry <ic16b055@technikum-wien.at>
 * @author Sebastian Boehm <ic16b032@technikum-wien.at>
 * @date 2017/03/04
 *
 * @version 0.2 
 *
 * @todo God help us all
 * @Christian error handling!!
 *
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
#include <grp.h>
#include <time.h>
#include <fnmatch.h>

/*
 * --------------------------------------------------------------- defines --
 */

/*
 * -------------------------------------------------------------- typedefs --
 */

/*
 * --------------------------------------------------------------- globals --
 */

/*
 * ------------------------------------------------------------- functions --
 */
 static void usage_print(const char* const* params);
 static void do_file(const char* file_name, const char* const* parms);
 static void do_dir(const char* dir_name, const char* const* parms);
 /*static void comp_name(const char* file_name, const char* const* parms, const int* fnm);*/
 static void comp_print(const char* file_name);
 static int parameter_check(const char* const* parms);
 static void do_error(const char* file_name, const char* const* parameters);

/**
 *
 * \brief The start of myfind
 *
 * This is the main entry point for this myfind C program.
 * Checks if sufficent ammount of parameters have been entered.
 *
 * \param argc the number of arguments
 * \param argv the arguments itselves (including the program name in argv[0])
 *
 * \return 0 if successful, 1 if erroneous
 * \retval EXIT_SUCCESS if successful
 * \retval EXIT_FAILURE if erroneous
 *
 */
int main(int argc, const char *argv[])
{
    if (argc < 2)
	{
		usage_print(argv);
		return EXIT_FAILURE;
	}
	if (parameter_check(argv) == 0)
	{
		do_file(argv[1], argv);
	}
	else 
	{
		usage_print(argv);
	}
	return EXIT_SUCCESS;
}
/**
 * \brief do_file compares parameters with set parameters and prints if corresponding
 *
 * Compares the arguments/parameters with the parameters defined in the function.
 * Prints the file name, directory as set in the parameter functions. 
 * If file_name is a directory the function calls do_dir.
 *
 * \param file_name name of directory or file
 * \param parms pointer to start of parameters
 *
 * \return nada
 *
 */
 static void do_file(const char* file_name, const char* const* parms)
 {
	 struct stat buffer; 
	 int parm_cnt = 2; //helper variable to choose array element
	 
	 /*checks if lstat completes*/
	 if (lstat(file_name, &buffer) == -1)
	 {
		  do_error(file_name, parms);                          
		 return;	 
	 }
	 while (parms[parm_cnt] != NULL) 
	 {
/*		 if (strcmp(parms[parm_cnt], '-name') == 0) //original find: find . -name test -> need argument after -name
		 {
			 parm_cnt++;
			 if (parms[parm_cnt] != NULL)
			 {
				 	 comp_name(file_name, parms[parm_cnt], FNM_NOESCAPE);
			 }
		 } */
		 if (strcmp(parms[parm_cnt], "-print") == 0)
		 {
			 comp_print(file_name);
			 parm_cnt++;
		 }
	 }
	 if(S_ISDIR(buffer.st_mode)) //checks if file is a directory
	 {
		 do_dir(file_name, parms);
	 }
 }
 /**
 * \brief do_dir works through directory and opens do_file for files
 *
 * Reads the entries (via dirent) of the given directory and calls do_file for every entry.
 * Adds / at the end of directory name if missing to get full path name.
 *
 * \param file_name name of directory
 * \param parms pointer to start of parameters
 *
 * \return nothing babe
 *
 */
 static void do_dir(const char* dir_name, const char* const* parms)
 {
	 DIR *tempDir = NULL;
	 struct dirent *dirpt;
	 tempDir = opendir(dir_name);
	 if (tempDir == NULL)
	 {
		 do_error(dir_name, parms);
		 return;
	 }
	errno = 0;
	 while ((dirpt = readdir(tempDir)) != NULL) 


	 {
		 if(strcmp(dirpt->d_name, ".") != 0 && strcmp(dirpt->d_name, "..") != 0)
		 {
			 char tempPath[strlen(dir_name) + strlen(dirpt->d_name)];
			 if(dir_name[strlen(dir_name) - 1] != '/')
			 {
				 strcpy(tempPath, dir_name);
				 strcat(tempPath, "/");
			 }
			 strcat(tempPath, dirpt->d_name);
			 do_file(tempPath, parms);
		 }
	 }
	errno = 0;
	 closedir(tempDir);

	 
 }
 
 /**
 *
 * \brief comp_print prints found files with same name
 *
 * Prints files or directory with the same name as argv[2].
 *
 * \param file_name Name of the file as defined by the calling function.
 *
 * \return njet
 *
 */
 
 static void comp_print(const char* file_name)
 {
	 printf("%s\n", file_name);	 //error handling???
 }
 
 /**
 *
 * \brief comp_name compares file name with find name.
 *
 * xxx.
 *
 * \param file_name
 * \param parms
 * \param fnm
 *
 * \return njet
 *
 */
 /*static void comp_name(const char* file_name, const char* const* parms, const int* fnm)
 {
	 
 }*/
 
/**
 *
 * \brief usage_print explains usage of myfind
 *
 * This function lists the parameters needed for successful myfind execution.
 *
 * \param params is list of parameters typed as parameters of function
 *
 * \return njet
 *
 */
static void usage_print(const char* const* params) /* how does error handling in printf work?? */
{
	fprintf(stderr, "Usage: %s\t<directory> <test-aktion> ...\n"
		"-type <xyz>\n"
		"-user <name/uid>\n"
		"-nouser\n"
		"-name <glob-pattern>\n"
		"-path <glob-pattern>\n"
		"-print\n"
		"-ls\n",
		*params);
}

/**
 *
 * \brief parameter_check compares entered parameters with set parameters
 *
 * This function compares the arguments entered with the set parameters.
 * If returned unsuccessful, usage_print shall be called.
 *
 * \param params is list of parameters typed as parameters of function
 *
 * \return 0 if successful 1 if unsuccessful
 *
 */
static int parameter_check(const char* const* parms)
{
	int counter = 2;
	while (parms[counter] != NULL)
	{
		if(strcmp(parms[counter], "-user") == 0 ||
		   strcmp(parms[counter], "-name") == 0 ||
		   strcmp(parms[counter], "-type") == 0 ||
		   strcmp(parms[counter], "-path") == 0 ||
		   strcmp(parms[counter], "-group") == 0)
		{
			  if (parms[counter + 1] == NULL)
		   {
			   usage_print(parms);
			   return EXIT_FAILURE;
		   } 
		   counter += 2;
		}
		else if(strcmp(parms[counter], "-print") == 0 ||
		        strcmp(parms[counter], "-ls") == 0 ||
		        strcmp(parms[counter], "-nouser") == 0 ||
		        strcmp(parms[counter], "-nogroup") == 0)
		{
			counter++;   
	    }
		else 
		{
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}
/**
*
* \This funktion prints error messages
*
*/
static void do_error(const char* file_name, const char* const* parameters)
{
	fprintf(stderr,"%s: %s %s\n",*parameters,file_name,strerror(errno));
}
/*
 * =================================================================== eof ==
 */



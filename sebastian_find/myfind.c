/**
 * @file myfind.c
 * Betriebssysteme myfind File.
 * Beispiel 1
 *
 * @author Alexander Pirka <ic16b031@technikum-wien.at>
 * @author Maria Karnikova <ic16b002@technikum-wien.at>
 * @author Christian Fuhry <ic16b055@technikum-wien.at>
 * @author Sebastian Boehm <ic16b032@technikum-wien.at>
 * @date 2017/02/26
 *
 * @version 0.1 
 *
 * @todo God help us all
 * @todo error handling in usage function
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

/**
 *
 * \brief The start of myfind.
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
	}
	do_file(argv[1], argv);
	return 0;
}
/**
 * \brief do_file
 *
 * xxx
 *
 * \param file_name name of directory or file
 * \param parms pointer to start of parameters
 *
 * \return xxx
 * \retval xxx
 * \retval xxx
 *
 */
 static void do_file(const char* file_name, const char* const* parms)
 {
	 struct stat buffer; 
	 int errno_temp = 0;
	 int parm_cnt = 2; //helper variable to choose array element
	 
	 /*checks if lstat completes*/
	 if (lstat(file_name, &buffer) == -1)
	 {
		 printf("lstat() failed");
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
	 if(S_ISDIR(buffer.st_mode))
	 {
		 do_dir(file_name, parms);
	 }
 }
 /**
 * \brief do_dir
 *
 * xxx
 *
 * \param file_name name of directory or file
 * \param parms pointer to start of parameters
 *
 * \return xxx
 * \retval xxx
 * \retval xxx
 *
 */
 static void do_dir(const char* dir_name, const char* const* parms)
 {
	 DIR *tempDir = NULL;
	 struct dirent *dirpt;
	 tempDir = opendir(dir_name);
	 if (tempDir == NULL)
	 {
		 /*error handling*/
	 }
	 while ((dirpt = readdir(tempDir)) != NULL) //error handling
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
	 closedir(tempDir); //error handling
 }
 
 /**
 *
 * \brief comp_print prints found files with same name.
 *
 * xxx.
 *
 * \param file_name
 * \param parms
 *
 * \return njet
 *
 */
 
 static void comp_print(const char* file_name)
 {
	 printf("%s\n", file_name); //error handling???
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
 * \brief usage_print explains usage of myfind.
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
	printf("Usage of myfind:\n "
	"type:            myfind <file or directory> [ <action> ]\n"
	"actions can be: -user <name>|<uid>\n"
	"                -name <pattern>\n"
	"                -type [bcdpfls]\n"
	"                -print\n"
	"                -ls\n"
	"                -nouser\n"
	"                -path <pattern>\n"
	"                -group <name>|<gid>\n"
	"                -nogroup\n");
}

/*
 * =================================================================== eof ==
 */

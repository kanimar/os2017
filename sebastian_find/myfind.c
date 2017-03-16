/**
* @file myfind.c
* Betriebssysteme myfind File.
* Beispiel 1
*
* @author Alexander Pirka <ic16b031@technikum-wien.at>
* @author Maria Kanikova <ic16b002@technikum-wien.at>
* @author Christian Fuhry <ic16b055@technikum-wien.at>
* @author Sebastian Boehm <ic16b032@technikum-wien.at>
* @date 2017/03/16
*
* @version 1.0
*
* @todo God help us all!
*												Brauchen wir printed variabel ?
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
#include <libgen.h>

/*
* --------------------------------------------------------------- defines --
*/

/*
* -------------------------------------------------------------- typedefs --
*/
# define BUFFER 80
/*
* --------------------------------------------------------------- globals --
*/

/*
* ------------------------------------------------------------- functions --
*/
static void do_usage_print(const char* const* parms);
static void do_file(const char* file_name, const char* const* parms);
static void do_dir(const char* dir_name, const char* const* parms);
static int do_comp_name(const char* file_name, const char* parms);
static int do_type(const char* file_name, const char* parms, const struct stat buf);
static int do_path(const char* file_name, const char *parms);
static int do_check(const char* const* parms);
static void do_error(const char* file_name, const char* const* parms);
static void do_comp_print(const char* file_name);
static int do_ls_print(const char* file_name, const char* const* parms, const struct stat buf);
static int do_comp_userOrGroup(const char * userparms, const struct stat buf, char *userOrGroup);
static int do_comp_no_userOrGroup(const char* file_name, const char* const* parms, const struct stat buf, char *userOrGroup);

/**
*
* \brief The start of myfind
*
* This is the main entry point for this myfind C program.
* Checks if sufficent ammount of parms have been entered.
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


//		int i = argc;
//		for (i; i >= 0; i--)
	
//	printf("argc %d argv %s\n", i, argv[i]);
	


	if (argc < 2)
	{
		do_usage_print(argv);
		return EXIT_FAILURE;
	}								
	if (do_check(argv) == 0)
		{								printf("check OK\n");
		do_file(argv[1], argv);
		}
	
	return EXIT_SUCCESS;
}
/**
* \brief do_file compares parms with set parms and prints if corresponding
*
* Compares the arguments/parms with the parms defined in the function.
* Prints the file name, directory as set in the parameter functions.
* If file_name is a directory the function calls do_dir.
*
* \param file_name name of directory or file
* \param parms pointer to start of parms
*
* \return nada
*
*/
static void do_file(const char* file_name, const char* const* parms)
{
	struct stat buf; //metadata (attribute)
	int offset = 1; //helper variable to choose array element
	int print_needed = 0;
	int printed = 0;

	if (lstat(file_name, &buf) == -1)
	{
		fprintf(stderr, "%s: unable to read lstat `%s'\n", parms[0], file_name);
		return;
	}
	while (parms[offset] != NULL)
	{
		if (strcmp(parms[offset], "-type") == 0)
		{
			if (parms[offset + 1] != NULL)
			{
				print_needed = do_type(file_name, parms[offset + 1], buf);
			}
			else
			{
				do_error(file_name, parms);
				exit(EXIT_FAILURE);
			}

		}
		else if (strcmp(parms[offset], "-path") == 0)
		{

			if (parms[offset + 1] != NULL)
			{
				print_needed = do_path(file_name, parms[offset + 1]);

			}
			else
			{
				do_error(file_name, parms);
				exit(EXIT_FAILURE);
			}
		}
		else if (strcmp(parms[offset], "-user") == 0)
		{

			if (parms[offset + 1] != NULL)
			{
				print_needed = do_comp_userOrGroup(parms[offset + 1], buf, "user");
			}
			else
			{
				do_error(file_name, parms);
				exit(EXIT_FAILURE);
			}
		}
		else if (strcmp(parms[offset], "-group") == 0)
		{

			if (parms[offset + 1] != NULL)
			{
				print_needed = do_comp_userOrGroup(parms[offset + 1], buf, "group");
			}
			else
			{
				do_error(file_name, parms);
				exit(EXIT_FAILURE);
			}
		}
		else if (strcmp(parms[offset], "-nouser") == 0)
		{

			if (parms[offset + 1] == NULL)
			{
				print_needed = do_comp_no_userOrGroup(file_name, parms, buf, "user");
			}
			else
			{
				do_error(file_name, parms);
				exit(EXIT_FAILURE);
			}
		}
		else if (strcmp(parms[offset], "-nogroup") == 0)
		{

			if (parms[offset + 1] == NULL)
			{
				print_needed = do_comp_no_userOrGroup(file_name, parms, buf, "group");
			}
			else
			{
				do_error(file_name, parms);
				exit(EXIT_FAILURE);
			}
		}
		else if (strcmp(parms[offset], "-name") == 0)
		{
			
			if (parms[offset + 1] != NULL)
			{
				print_needed = do_comp_name(file_name, parms[offset + 1]);
			}
		}
		else if (strcmp(parms[offset], "-print") == 0)
		{
			do_comp_print(file_name);
			
		}
		else if (strcmp(parms[offset], "-ls") == 0)
		{
			do_ls_print(file_name, parms, buf);
		}
		else if ( ((parms[1]) != NULL) && ((parms[2]) == NULL) && (printed == 0) )
		{
			print_needed = 1;			
		}
		offset++;
	}
	if (print_needed > 0 )
	{
		do_comp_print(file_name);
		printed = 1;
	}

	if (S_ISDIR(buf.st_mode)) //checks if file is a directory
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
* \param parms pointer to start of parms
*
* \return nothing babe
*
*/
static void do_dir(const char* dir_name, const char* const* parms)
{
	DIR *dirp = NULL;
	const struct dirent *dirent;
	int offset = 1; //helper variable to choose array element

	dirp = opendir(dir_name);
	if (dirp == NULL)
	{
		fprintf(stderr, "%s: can't open directory `%s'\n", parms[0], dir_name);
		return;
	}

	errno = 0;					//reset errno

	while ((dirent = readdir(dirp)) != NULL)
	{
		if (errno != 0)
		{
			do_error(dir_name, parms);
			errno = 0;			//reset errno
			continue;
		}

		if (strcmp(dirent->d_name, ".") != 0 && strcmp(dirent->d_name, "..") != 0)
		{
			char tempPath[strlen(dir_name) + strlen(dirent->d_name)];
			if (dir_name[strlen(dir_name) - 1] != '/')
			{
				strcpy(tempPath, dir_name);
				strcat(tempPath, "/");
			}
			strcat(tempPath, dirent->d_name);
			do_file(tempPath, parms);
		}
		offset++;
	}
	int closedir(DIR *dirp);

	if (closedir(dirp) != 0)
	{
		do_error(dir_name, parms);
		return;
	}

}

/**
* if match 1 return sucessful
* \brief do_type
*/
static int do_type(const char* file_name, const char* parms, const struct stat buf)
{
	int match = -1;

	if		(strcmp(parms, "b") == 0) match = S_ISBLK(buf.st_mode);
	else if (strcmp(parms, "c") == 0) match = S_ISCHR(buf.st_mode);
	else if (strcmp(parms, "d") == 0) match = S_ISDIR(buf.st_mode);
	else if (strcmp(parms, "p") == 0) match = S_ISFIFO(buf.st_mode);
	else if (strcmp(parms, "f") == 0) match = S_ISREG(buf.st_mode);
	else if (strcmp(parms, "l") == 0) match = S_ISLNK(buf.st_mode);
	else if (strcmp(parms, "s") == 0) match = S_ISSOCK(buf.st_mode);
	else
	{	
		fprintf(stderr, "%s: unknown argument to %s\n", parms, file_name);
		exit(EXIT_FAILURE);
	}
	

	if (match == 0)
	{
		return 0;
	}

	return 1;
}

/**
*
* \brief do_path
* int fnmatch(const char *pattern, const char *string, int flags);
*Flag FNM_NOESCAPE > treat backslash as an ordinary character
*
*	match > Returns TRUE if there is a match, FALSE otherwise.
*/
static int do_path(const char* file_name, const char *parms)
{
	int match = -1;

	match = fnmatch(parms, file_name, FNM_NOESCAPE);

	if (match != 0) 
	{
		return 0;
	}

	return 1;
}

/**
*
* \brief do_comp_userOrGroup compares parameter with uid/gid and uname/grpname
*
* This function compares the argument after "-user" od "-group" with the uid/gid of the file.
* It returns 1 if the comparation is successful and 0 if unsuccessful.
*
* \param userparms is the argument typed by the user after -user or -group.
* \param stat buf is the buffer with the metadata created by lstat.
* \param userOrGroup hardcoded to distinguish betweed -user and -group search action.
*
* \return 1 if successful 0 if unsuccessful
*
*/
static int do_comp_userOrGroup(const char * userparms, const struct stat buf, char *userOrGroup)
{
	struct passwd *pwd = NULL;
	char *endptr = NULL;
	long int id = 0;
	pwd = getpwnam(userparms);

	if (pwd == NULL) //A null pointer is returned if the requested entry is not found, or an error occurs.
	{
		fprintf(stderr, "-%s: %s is not the name of a known %s\n", userOrGroup, userparms, userOrGroup);
		exit(EXIT_FAILURE);
	}
	else if (pwd != NULL)
	{
		if (strcmp(userOrGroup, "user") == 0)
		{
			if (pwd->pw_uid == buf.st_uid)
			{
				return 1;
			}
			id = strtol(userparms, &endptr, 10);
			if (strcmp(endptr, "/0") != 0)
			{
				exit(EXIT_FAILURE); //strtol couldnt finish converting
			}
			if (buf.st_uid == id)
			{
				return 1;
			}
		}
		else if (strcmp(userOrGroup, "group") == 0)
		{
			if (pwd->pw_gid == id)
			{
				return 1;
			}
			id = strtol(userparms, &endptr, 10);
			if (strcmp(endptr, "/0") != 0)
			{
				exit(EXIT_FAILURE); //strtol couldnt finish converting
			}
			if (buf.st_gid == id)
			{
				return 1;
			}
		}
	}
	else
	{
		fprintf(stderr, "-%s: %s is not the name of a known %s\n", userOrGroup, userparms, userOrGroup);
		exit(EXIT_FAILURE);
	}
	return 0;
}

/**
*
* \brief do_comp_no_userOrGroup checks if a file is user- or groupless
*
* This function checks if a file or directory currently has a user or group assigned.
* It returns 1 if the user or group parameter is not set in file.
*
* \param file_name is the name of the file to be checked.
* \param parameter needed for error handling.
* \param stat buf is the buffer with the metadata created by lstat.
* \param userOrGroup hardcoded to distinguish betweed -nouser and -nogroup  action.
*
* \return 1 if user or groupless 0 if user or group present
*
*/
static int do_comp_no_userOrGroup(const char* file_name, const char* const* parms, const struct stat buf, char *userOrGroup)
{
	const struct passwd *pwd = NULL;
	const struct group *gid = NULL;
	errno = 0;			//reset errno
	if (strcmp(userOrGroup, "user") == 0)
	{
		pwd = getpwuid(buf.st_uid);
		if ((pwd == NULL) && (errno == 0))
		{
			return 1;
		}
		else if (errno != 0)
		{
			do_error(file_name, parms);
		}
	}
	else if (strcmp(userOrGroup, "group") == 0)
	{
		gid = getgrgid(buf.st_gid);
		if ((gid == NULL) && (errno == 0))
		{
			return 1;
		}
		else if (errno != 0)
		{
			do_error(file_name, parms);
		}
	}
	return 0;
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

static void do_comp_print(const char* file_name)
{
	if (printf("%s\n", file_name) < 0)
	{
		printf("do_comp_print error");
	}
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
static int do_comp_name(const char* file_name, const char* parms)
{
	char temp_name[strlen(file_name) + 1]; // basename() may modify the contents of path, so it may be desirable to pass a copy when calling one of these functions.
	strcpy(temp_name, file_name);
	char* base = NULL;
	base = basename(temp_name);

	const int match_name = fnmatch(base, parms, FNM_NOESCAPE);

	if (match_name == 0)
	{
		return 1;
	}
	else if (match_name != FNM_NOMATCH) //FNM_NOMATCH if there is no match or another nonzero value if there is an error
		exit(EXIT_FAILURE);

	return 0;
}

/**
*
* \brief usage_print explains usage of myfind
*
* This function lists the parms needed for successful myfind execution.
*
* \param parms is list of parms typed as parms of function
*
* 
*					
*
*/
static void do_usage_print(const char* const* parms) /* how does error handling in printf work?? */
{
	fprintf(stderr, "Usage: %s <file or directory> <aktion> \n"
		"-type <bcdpfls>\n"
		"-path <pattern>\n"
		"-name <pattern>\n"
		"-user <name/uid>\n"
		"-nouser\n"
		"-group <name/gid>\n"
		"-nogroup\n"
		"-print\n"
		"-ls\n",
		*parms);
	exit(EXIT_FAILURE);
}
/**
*
* \This funktion prints -ls
*  char *getenv(const char *name) searches for the environment string pointed to by name and returns the associated value to the string.
*  Acronym for P ortable O perating S ystem I nterface UniX.
*/
static int do_ls_print(const char* file_name, const char* const* parms, const struct stat buf)
{
	const struct passwd * user = NULL;
	const struct group* group = NULL;
	const struct tm* time = NULL;

	unsigned int blocks = 0;
	char mode[] = { "?---------" };

	char do_name[strlen(file_name)];
	int symb_link_length = 0;
	char symb_link_string[buf.st_size];
	const char arrow[] = { " -> " };

	errno = 0;			//reset errno

	char* do_user = "";
	char* do_group = "";

	char uid[BUFFER] = { 0 };
	char gid[BUFFER] = { 0 };

	char month[BUFFER] = { 0 };
	char do_time[BUFFER] = { 0 };


	if		(S_ISREG(buf.st_mode))		mode[0] = '-';		//regular file
	else if (S_ISDIR(buf.st_mode))		mode[0] = 'd';		//directory
	else if (S_ISCHR(buf.st_mode))		mode[0] = 'c';		//char special file
	else if (S_ISBLK(buf.st_mode))		mode[0] = 'b';		//block special file			
	else if (S_ISFIFO(buf.st_mode))		mode[0] = 'f';		//FIFO(named pipe)
	else if (S_ISLNK(buf.st_mode))		mode[0] = 'l';		//symbolic link
	else if (S_ISSOCK(buf.st_mode))		mode[0] = 's';		//socket
	else								mode[0] = '?';		//unknown 


	if		(buf.st_mode & S_IRUSR)									mode[1] = 'r'; //user readable	
	if		(buf.st_mode & S_IWUSR)									mode[2] = 'w'; //user writeable
	if		((buf.st_mode & S_IXUSR) && !(buf.st_mode & S_ISUID))	mode[3] = 'x'; //user executable without sticky
	else if (buf.st_mode & S_IXUSR)									mode[3] = 's'; //user executable
	else if (buf.st_mode & S_ISUID)									mode[3] = 'S'; //user not executable with sticky

	if		(buf.st_mode & S_IRGRP)									mode[4] = 'r'; //group readable	
	if		(buf.st_mode & S_IWGRP)									mode[5] = 'w'; //group writeable
	if		((buf.st_mode & S_IXGRP) && !(buf.st_mode & S_ISGID))	mode[6] = 'x'; //group executable without sticky
	else if (buf.st_mode & S_IXGRP)									mode[6] = 's'; //group executable
	else if (buf.st_mode & S_ISGID)									mode[6] = 'S'; //group not executable with sticky

	if		(buf.st_mode & S_IROTH)									mode[7] = 'r'; //others readable	
	if		(buf.st_mode & S_IWOTH)									mode[8] = 'w'; //others writeable
	if		((buf.st_mode & S_IXOTH) && !(buf.st_mode & S_ISVTX))	mode[9] = 'x'; //others executable without sticky
	else if (buf.st_mode & S_IXOTH)									mode[9] = 't'; //others executable
	else if (buf.st_mode & S_ISVTX)									mode[9] = 'T'; //others save swapped test after use (sticky)

	strcpy(do_name, file_name);

	if (mode[0] != 'l')
	{
		blocks = (unsigned int)buf.st_blocks;

		if (getenv("POSIXLY_CORRECT") == NULL)
		{
			blocks = ((unsigned int)buf.st_blocks);
			blocks = blocks / 2 + buf.st_blocks % 2;
		}
	}

	if (mode[0] == 'l')
	{
		symb_link_length = readlink(file_name, symb_link_string, buf.st_size);
		symb_link_string[symb_link_length] = '\0';   //ending for readlink '/0'

		if (symb_link_length)
		{
			strcat(do_name, arrow);
			strcat(do_name, symb_link_string);
		}
		else
		{
			fprintf(stderr, "%s: Symbolic link error: %s", *parms, strerror(errno));
		}
	}

	errno = 0;  //reset errno
	if ((user = getpwuid(buf.st_uid)) == NULL || user->pw_name == NULL)
	{
		if (errno != 0)
		{

			do_error(file_name, parms);
		}
		else
		{
			snprintf(uid, sizeof(uid), "%d", buf.st_uid);
			do_user = uid;
		}
	}
	else
	{
		do_user = user->pw_name;
	}

	errno = 0;  //reset errno
	if ((group = getgrgid(buf.st_gid)) == NULL || (group->gr_name == NULL))
	{
		if (errno != 0)
		{
			do_error(file_name, parms);
		}
		else
		{
			snprintf(gid, sizeof(gid), "%d", buf.st_gid);
			do_group = gid;
		}
	}
	else
	{
		do_group = group->gr_name;
	}
	errno = 0;  //reset errno

	time = localtime(&(buf.st_mtime));

	strftime(month, sizeof(month), "%b", time);  //formatted month 

	if (sprintf(do_time, "%s %2d %02d:%02d", month, time->tm_mday, time->tm_hour, time->tm_min) < 0)
	{
		fprintf(stderr, "%s: do_time error", *parms);
	}

	if (printf("%ld %4u %s%4.0d %s %s %8lu %s %s\n", buf.st_ino, blocks, mode, buf.st_nlink, do_user, do_group, buf.st_size, do_time, do_name) < 0)
	{
		fprintf(stderr, "%s: -ls error %s", *parms, file_name);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
/**
*
* \brief parameter_check compares entered parms with set parms
*
* This function compares the arguments entered with the set parms.
* If returned unsuccessful, usage_print shall be called.
*
* \param parms is list of parms typed as parms of function
*
* \return 0 if successful 1 if unsuccessful
*
*/
static int do_check(const char* const* parms)
{
	int offset = 2;

	while (parms[offset] != NULL)
	{
		if (strcmp(parms[offset], "-user") == 0 ||
			strcmp(parms[offset], "-name") == 0 ||
			strcmp(parms[offset], "-type") == 0 ||
			strcmp(parms[offset], "-path") == 0 ||
			strcmp(parms[offset], "-group") == 0)
		{
			if (parms[offset + 1] == NULL)
			{
				printf("%s: missing argument to `%s'\n",parms[0],parms[offset]);	//CHECK		
				return EXIT_FAILURE;
			}
			offset += 2;
		}
		else if	(strcmp(parms[offset], "-print" ) == 0 ||
				 strcmp(parms[offset], "-ls"	) == 0 ||
				 strcmp(parms[offset], "-nouser") == 0 ||
				 strcmp(parms[offset], "-nogroup") == 0)
		{
			offset += 1;
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
static void do_error(const char* file_name, const char* const* parms)
{
	fprintf(stderr, "%s: %s %s\n", parms[0], file_name, strerror(errno));
}
/*
* =================================================================== eof ==
*/

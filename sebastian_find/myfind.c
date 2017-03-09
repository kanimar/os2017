/**
* @file myfind.c
* Betriebssysteme myfind File.
* Beispiel 1
*
* @author Alexander Pirka <ic16b031@technikum-wien.at>
* @author Maria Kanikova <ic16b002@technikum-wien.at>
* @author Christian Fuhry <ic16b055@technikum-wien.at>
* @author Sebastian Boehm <ic16b032@technikum-wien.at>
* @date 2017/03/07 23:30
*
* @version 0.3
*
* @todo God help us all!
* @Christian > "Error handling in work"
* @Christian > "-ls in work"
* @Christian > anpassen der Variablen-Bezeichnungen > Unterrichtsfolien
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
static void do_usage_print(const char* const* parms);
static void do_file(const char* file_name, const char* const* parms);
static void do_dir(const char* dir_name, const char* const* parms);
/*static void comp_name(const char* file_name, const char* const* parms, const int* fnm);*/
static int do_check(const char* const* parms);
static int do_comp_user(const uid_t userid, const char * userparameter);
static void do_error(const char* file_name, const char* const* parms);
static void do_comp_print(const char* file_name);
static int do_ls_print(const char* file_name, const char* const* parms, const struct stat sb);
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
		do_usage_print(argv);
		return EXIT_FAILURE;
	}
	if (do_check(argv) == 0)
	{
		do_file(argv[1], argv);
	}
	else
	{
		do_usage_print(argv);
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
	struct stat buf; //metadata (atribute)
	int offset = 2; //helper variable to choose array element
	int print_needed = 0;

	if (lstat(file_name, &buf) == -1)
	{
		do_error(file_name, parms);      //checks if lstat completes
		return;
	}
	while (parms[offset] != NULL)
	{
		if (strcmp(parms[offset], "-user") == 0)
		{
			offset++; // <action> -user needs another argument user_id or user_name
			if (parms[offset] != NULL)
			{
				print_needed = do_comp_user(buf.st_uid, parms[offset]);
				do_comp_print(file_name);
				offset++;
				if(parms[offset] == NULL) break;
			}
			else
			{
				do_error(file_name, parms);
				exit(EXIT_FAILURE);
			}
		}
		/*		 if (strcmp(parms[parm_cnt], '-name') == 0) //original find: find . -name test -> need argument after -name
		{
		parm_cnt++;
		if (parms[parm_cnt] != NULL)
		{
		comp_name(file_name, parms[parm_cnt], FNM_NOESCAPE);
		}
		} */
		if (strcmp(parms[offset], "-print") == 0)
		{
			do_comp_print(file_name);

		}
		if (strcmp(parms[offset], "-ls") == 0)
		{
			do_ls_print(file_name, parms, buf);
		}
		offset++;
	}
	if (print_needed > 0)
	{
		do_comp_print(file_name);
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
* \param parms pointer to start of parameters
*
* \return nothing babe
*
*/
static void do_dir(const char* dir_name, const char* const* parms)
{
	DIR *dirp = NULL;
	const struct dirent *dirent;

	dirp = opendir(dir_name);
	if (dirp == NULL)
	{
		do_error(dir_name, parms);
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
	}
	int closedir(DIR *dirp);

	if (closedir(dirp) != 0)
	{
		do_error(dir_name, parms);
		return;
	}

}

/**
*
* \brief do_comp_user compares parameter with uid and uname
*
* This function compares the argument after "-user" with the uid and username of the file.
* It returns 1 if the comparation is successful and 0 if unsuccessful.
*
* \param userid is the user id of the file.
* \param userparameter is the argument after "-user"
*
* \return 1 if successful 0 if unsuccessful
*
*/
static int do_comp_user(const uid_t userid, const char * userparameter)
{
	struct passwd *pwd = NULL;
	char *endptr = NULL;
	long int uid = 0;
	
	pwd = getpwnam(userparameter);
	if(pwd == NULL) //A null pointer is returned if the requested entry is not found, or an error occurs.
	{
		printf("Fehler in Zeile 242");
		exit(EXIT_FAILURE);
	}
	else if(pwd != NULL)
	{
		if(pwd->pw_uid == userid)
		{
			return 1;
		}
	}
	else
	{
		uid = strtol(userparameter, &endptr, 10);
		if(strcmp(endptr, "/0") != 0)
		{
			printf("Fehler in Zeile 257");
			exit(EXIT_FAILURE); //strtol couldnt finish converting
		}
		if(userid == uid)
		{
			return 1; 
		}
		else
		{
			fprintf(stderr, "-user: %s is not the name of a known user\n", userparameter);
			exit(EXIT_FAILURE);
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
* \param parms is list of parameters typed as parameters of function
*
* \ don't work		"-type <xyz>\n"
"-user <name/uid>\n"
"-nouser\n"
"-name <pattern>\n"
"-path <pattern>\n"
*
*/
static void do_usage_print(const char* const* parms) /* how does error handling in printf work?? */
{
	fprintf(stderr, "Usage: %s <file or directory> <aktion> \n"

		"-print\n"
		"-ls\n",
		*parms);
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
	errno = 0;			//reset errno

	char* do_user = "";
	char* do_group = "";


	char uid[14];
	char gid[14];

	char month[5] = { 0 };
	char do_time[14] = { 0 };


	if		(buf.st_mode  & S_IFREG)		mode[0] = '-';		//regular file
	else if (buf.st_mode  & S_IFDIR)		mode[0] = 'd';		//directory
	else if (buf.st_mode  & S_IFCHR)		mode[0] = 'c';		//char special file
	else if (buf.st_mode  & S_IFBLK)		mode[0] = 'b';		//block special file			
	else if (buf.st_mode  & S_IFIFO)		mode[0] = 'f';		//FIFO(named pipe)
	else if (buf.st_mode  & S_IFLNK)		mode[0] = 'l';		//symbolic link
	else if (buf.st_mode  & S_IFSOCK)		mode[0] = 's';		//socket
	else									mode[0] = '?';		//unknown 


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


	if (mode[0] != 'l')
	{
		blocks = (unsigned int)buf.st_blocks;

		if (getenv("POSIXLY_CORRECT") == NULL)
		{
			blocks = ((unsigned int)buf.st_blocks);
			blocks = blocks / 2 + buf.st_blocks % 2;
		}
	}

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
	errno = 0;

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
	errno = 0;

	time = localtime(&(buf.st_mtime));

	strftime(month, sizeof(month), "%b", time);

	if (sprintf(do_time, "%s %2d %02d:%02d", month, time->tm_mday, time->tm_hour, time->tm_min) < 0)
	{

		fprintf(stderr, "%s: do_time error", *parms);
	}



	printf("%ld %4u %s%4.0d %s %s %8lu %s %s\n", buf.st_ino, blocks, mode, buf.st_nlink, do_user, do_group, buf.st_size, do_time, file_name);

	return EXIT_SUCCESS;
}
/**
*
* \brief parameter_check compares entered parameters with set parameters
*
* This function compares the arguments entered with the set parameters.
* If returned unsuccessful, usage_print shall be called.
*
* \param parms is list of parameters typed as parameters of function
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
				do_usage_print(parms);
				return EXIT_FAILURE;
			}
			offset += 2;
		}
		else if (strcmp(parms[offset], "-print") == 0 ||
			strcmp(parms[offset], "-ls") == 0 ||
			strcmp(parms[offset], "-nouser") == 0 ||
			strcmp(parms[offset], "-nogroup") == 0)
		{
			offset++;
		}
		else
		{
			do_usage_print(parms);
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
	fprintf(stderr, "%s: %s %s\n", *parms, file_name, strerror(errno));
}
/*
* =================================================================== eof ==
*/

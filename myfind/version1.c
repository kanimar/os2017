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

int parms_count;

void do_dir(const char* dir_name, const char* const* parms);
void do_file(const char* file_name, const char* const* parms);
int main(int argc, const char* const argv[])
{
const char* search_string;
const char* const* actions;
parms_count = argc;

        if (argc == 1)
        { printf("Usage %s [name]", argv[0]);
        return 1;
        }
       
search_string = argv[1];
actions = argv;

do_dir(search_string, actions);

return 0;
}

void do_dir(const char* dir_name, const char* const* parms)
{
DIR* directory;
struct dirent* dirent_str;
char* slash = "/";
char path[1024];

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
                        	/**absolute path with filename is created => in eine eigene Funktion auslagern?**/
										getcwd(path, sizeof(path)); /*Get current working directory*/
										strcat(path, slash); /*add slash after current working directory*/
										strcat(path, dir_name);
                        		strcat(path, slash);
										strcat(path, dirent_str->d_name); /*concat current working directory with file_name*/
                        	do_file(path, parms);
                        }
                } while (dirent_str != NULL);
                printf("fine!\n");
        }
}

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
		case S_IFDIR: printf("....d\n"); /*do_dir(file_name, parms) soll nochnmal aufgerufen werden*/; break;
		case S_IFREG: printf("....f\n"); break;	
		default: printf("....unkown\n"); break;
	} 
	
}
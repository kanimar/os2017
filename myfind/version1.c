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
const char* search_string;

void do_dir(const char* dir_name, const char* const* parms);
void do_file(const char* file_name, const char* const* parms);
void get_pwd(char* name, const char* dir, char* pp);
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


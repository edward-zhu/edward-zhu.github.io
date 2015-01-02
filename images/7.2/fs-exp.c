#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

int func(char * path, int * AllFileNumber, int * AllDepthNumber, int * AllFilenameNumber, int * AllFolderNumber)
{
	struct stat statbuf;
	struct dirent * dirp;
	DIR * dp;
	char str1[1000];
	char * str2;
	int judge1, judge2, ret;

	judge1 = 0;
	judge2 = 0;

	stat(path, &statbuf);

	switch(statbuf.st_mode & S_IFMT) {
		case S_IFREG: {
			*AllFilenameNumber += strlen(path);
			*AllFileNumber = * AllFileNumber + 1;
			*AllDepthNumber = *AllDepthNumber + 1;
			break;
		}

		case S_IFDIR: {
			*AllDepthNumber = *AllDepthNumber + 1;
			*AllFolderNumber = *AllFolderNumber + 1;
			dp = opendir(path);
			while((dirp = readdir(dp)) != NULL) {
				if ((strcmp(dirp->d_name, ".") == 0) 
					|| strcmp(dirp->d_name, "..") == 0)
				{
					continue;
				}
				if (judge1 == 0) {
					strcpy(str1, path);
					judge1 = 1;
				}
				if (judge2 == 0) {
					str2 = str1 + strlen(str1);
					judge2 = 1;
				}
				*str2++ = '/';
				*str2 = '\0';
				strcpy(str2, dirp->d_name);
				printf("%s\n", str1);
				ret = func(str1, AllFileNumber, AllDepthNumber, AllFilenameNumber, AllFolderNumber);
				*str2--;
				*str2 = '\0';
				
			}
			closedir(dp);
			break;
		}
	}
	return 1;
}

int main(int argc, char * argv[])
{
	int ret;
	int AllFileNumber = 0;
	int AllDepthNumber = 0;
	int AllFilenameNumber = 0;
	int AllFolderNumber = 0;
	printf("%s\n", argv[1]);
	ret = func(argv[1], &AllFileNumber, &AllDepthNumber, &AllFilenameNumber, &AllFolderNumber);
	printf("The average file # is %d\n", AllFileNumber / AllFolderNumber);
	printf("The average file depth # is %d\n", AllDepthNumber / AllFolderNumber);
	printf("The average filename number is %d\n", AllFilenameNumber / AllFileNumber);

	return 0;
}
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char *argv[]){
	
	if(argc!=1 && argc!=2){
		perror("Wrong number of arguments");
		exit(EXIT_FAILURE);
	}

	DIR *dh = opendir(".");
	struct dirent * dirent;
	if(!dh){
		if(errno==ENOENT){
			perror("This directory does not exist");
		}
		else{
			perror("Cannot read directory");
		}
		exit(EXIT_FAILURE);
	}
	while((dirent=readdir(dh))!=NULL){
		if(argc==1){
			if(dirent->d_name[0]=='.' || strcmp(dirent->d_name,"..")==0){
				continue;
			}
			printf("%s ", dirent->d_name);
		}
		else if(argc==2 && strcmp(argv[1],"-1")==0){
			if(dirent->d_name[0]=='.' || strcmp(dirent->d_name,"..")==0){
				continue;
			}
			printf("%s\n", dirent->d_name);
		}
		else if(argc==2 && strcmp(argv[1],"-a")==0){
			printf("%s ", dirent->d_name);
		}	
		else{
			perror("Invalid option");
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}

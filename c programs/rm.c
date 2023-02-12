#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char *argv[]){
    if(argc<=1){
        perror("Wrong number of arguments");
        exit(EXIT_FAILURE);
    }
    
    if(strcmp(argv[1],"-i")==0){
         for(int i=2; i<argc; i++){
            int option;
            printf("rm: remove regular file '%s' ?", argv[i]);
            scanf("%d", &option);
            if(option==1){
                if(remove(argv[i])==-1){
			printf("File not found %s\n",argv[i]);
			exit(1);
		}
            }
	    printf("\n");
        }
    }

    else if(strcmp(argv[1],"-d")==0){
        int n = 0;
        struct dirent *d;
        DIR *dir = opendir(argv[2]);
        if (dir == NULL){
            perror("Directory does not exist");
            exit(EXIT_FAILURE);
        }
        while ((d = readdir(dir)) != NULL) {
            if(++n > 2)
            break;
        }
        closedir(dir);
        if (n <= 2){
            int create= rmdir(argv[2]);
            if(create==-1){
                perror("Unable to remove directory");
                exit(EXIT_FAILURE);
            }
        }
        else{
            perror("Directory is not empty");
            exit(EXIT_FAILURE);
        }
    }

    else{
        for(int i=1; i<argc; i++){
            FILE *filepointer;
            filepointer=fopen(argv[i],"r");
            if(filepointer==NULL){
                perror("File does not exist");
                exit(EXIT_FAILURE);
            }
            fclose(filepointer);
            remove(argv[i]);
        }
    }
}

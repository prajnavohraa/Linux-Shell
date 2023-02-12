#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char * argv[]){

    if(argc==1){
        perror("Wrong number of arguments");
        exit(EXIT_FAILURE);
    }
    if(argc==2){
        int create=mkdir(argv[1],0777);
        if(create==-1){
            perror("Directory could not be created");
            exit(EXIT_FAILURE);
        }
    }

    else if(strcmp(argv[1],"-v")==0){
        for(int i=2; i<argc; i++){
            int create=mkdir(argv[i],0777);
            if(create==-1){
                perror("Directory could not be created");
                exit(EXIT_FAILURE);
            }
            else{
                printf("mkdir: created directory '%s'\n", argv[i]);   
            }
        }
    }

    else if(strcmp(argv[1],"-p")==0){
        char * directory=strtok(argv[2],"/");
        while(directory!=NULL){
            char *new_dir=directory;
            int create=mkdir(new_dir,0777);
            if(create==-1){
                perror("Directory could not be created");
                exit(EXIT_FAILURE);
            }
            chdir(new_dir);
            directory = strtok(NULL, "/");
        }
    }
    else{
        perror("Wrong command");
        exit(EXIT_FAILURE);
    }
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <string.h>

int main(int argc, char* argv[]){
    
    if(argc==1){
        time_t t;
        t=time(NULL);
        for(int i=0; i<20; i++){
            printf("%c",(ctime(&t))[i]);
        }
        printf("%s","IST ");
        for(int i=20; i<strlen(ctime(&t)); i++){
            printf("%c",(ctime(&t))[i]);
        }
    }

    else if(argc==2){
        if(strcmp(argv[1],"-u")==0){
            time_t mytime;
            struct tm * ptm;
            time(&mytime);
            ptm=gmtime(&mytime);
            time_t utctime=mktime(ptm);
            for(int i=0; i<20; i++){
                printf("%c",(ctime(&utctime))[i]);
            }
            printf("%s","UTC ");
            for(int i=20; i<strlen(ctime(&utctime)); i++){
                printf("%c",(ctime(&utctime))[i]);
            }
        }
        else{
            perror("Wrong command");
            exit(EXIT_FAILURE);
        }
    }

    else if(argc==3 && strcmp(argv[1],"-r")==0){
        FILE *filepointer;
        filepointer=fopen(argv[2],"r");
        if(filepointer==NULL){
            perror("File does not exist");
            exit(EXIT_FAILURE);
        }
        struct stat file;
        stat(argv[2],&file);

        for(int i=0; i<20; i++){
            printf("%c",(ctime(&file.st_mtime))[i]);
        }
        printf("%s","IST ");
        for(int i=20; i<strlen(ctime(&file.st_mtime)); i++){
            printf("%c",(ctime(&file.st_mtime))[i]);
        }
    }
}

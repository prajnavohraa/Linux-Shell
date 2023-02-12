#include <stdio.h>
#include <string.h>
#include <errno.h>
#include<stdlib.h>

int main(int argc, char *argv[]){
    if(argc<2){
        perror("Wrong number of arguments");
        exit(EXIT_FAILURE);
    }

    if(argc==2){
        FILE *filepointer;
        filepointer=fopen(argv[1],"r");
        if(filepointer==NULL){
            perror("File does not exist");
            exit(EXIT_FAILURE);
        }
        char ch;
        while((ch=fgetc(filepointer))!=EOF){
            printf("%c", ch);
        }
        fclose(filepointer);
    }

    else if(argc==3 && strcmp(argv[1],"-n")==0){
        FILE *filepointer;
        filepointer=fopen(argv[2],"r");
        if(filepointer==NULL){
            perror("File does not exist");
            exit(EXIT_FAILURE);
        }
        
        int line_number=1;
        printf("%d ", line_number);
        char ch;

        while ((ch=getc(filepointer))!=EOF){
            if(ch=='\n'){
                printf("%c", ch);
                line_number++;
                printf("%d ", line_number);
            }
            else{
                printf("%c", ch);
            }
        }
        fclose(filepointer);
    }

    else if(argc==3 && strcmp(argv[1],"-e")==0){
        FILE *filepointer;
        filepointer=fopen(argv[2],"r");
        if(filepointer==NULL){
            perror("File does not exist");
            exit(EXIT_FAILURE);
        }
        char ch;
        while((ch=fgetc(filepointer))!=EOF){
            if(ch=='\n'){
                printf("%c",'$');
            }
            printf("%c", ch);
        }
        fclose(filepointer);
        printf("%c",'$');
    }
    else{
        perror("Wrong command");
        exit(EXIT_FAILURE);
    }
}

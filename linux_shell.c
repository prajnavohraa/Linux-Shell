#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#define delimiter " \t\r\n\a"

char *array;

int argument_count(char **args){
    int count=0;
    for(int i=0; args[i]!=NULL; i++){
        count++;
    }
    return count;
}


char * convert_to_string(char **args){
    char *a=malloc(1000*sizeof(char));
    strcat(a,array);
    strcat(a,"/");
    if(strcmp(args[0],"ls&t")==0){
        strcat(a,"ls");
    }
    else if(strcmp(args[0],"cat&t")==0){
        strcat(a,"cat");
    }
    else if(strcmp(args[0],"rm&t")==0){
        strcat(a,"rm");
    }
    else if(strcmp(args[0],"date&t")==0){
        strcat(a,"date");
    }
    else if(strcmp(args[0],"mkdir&t")==0){
        strcat(a,"mkdir");
    }

    for(int i=1; i<argument_count(args); i++){
        strcat(a," ");
        strcat(a, args[i]);
    }
    return a;
}


//internal commands


int myshell_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "Wrong number of arguments");
    }
    else{
        if (chdir(args[1])!=0) {
            perror("Error");
        }
    }
    return 1;
}


int myshell_echo(char **args){

    if(sizeof(args)<=1){
        perror("Wrong number of arguments");
        exit(1);
    }

    if(sizeof(args)>=2){
        if(strcmp(args[1],"-e")==0){
            for(int i=2; i<sizeof(args); i++){
                if((args[i])==NULL){
                        return 1;
                }
                for(int j=0; j<strlen(args[i]); j++){
                    if((args[i][j])=='\\' && j!=strlen(args[i])-1 && args[i][j+1]=='n'){
                        printf("\n");
                        j++;
                    }
                    else if((args[i][j])=='\\' && j!=strlen(args[i])-1 && args[i][j+1]=='c'){
                        return 1;
                    }
                    else{
                        printf("%c", args[i][j]);
                    }
                }
                printf(" ");
            }
        }

        else{
            for(int i=1; i<sizeof(args); i++){
                if(args[i]!=NULL){
                    printf("%s ", args[i]);

                }
            }
        }
    }

    return 1;
}

int myshell_pwd(char **args){

    if(args[1]==NULL){
        char dir[1000];
        printf("%s\n",getcwd(dir,sizeof(dir)));
    }

    else if(strcmp(args[1],"--help")==0){
        printf("pwd: pwd [-LP]\n\tPrint the name of the current working directory.\n\n\tOptions:\n\t-L        print the value of $PWD if it names the current working directory\n\t-P        print the physical directory, without any symbolic links\n\n\tBy default, `pwd' behaves as if `-L' were specified.\n\n\tExit Status:\n\tReturns 0 unless an invalid option is given or the current directory cannot be read.");
    }

    else if(strcmp(args[1],"--version")==0){
        printf("pwd (GNU coreutils) 9.1\nCopyright (C) 2022 Free Software Foundation, Inc.\nLicense GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\nThis is free software: you are free to change and redistribute it.\nThere is NO WARRANTY, to the extent permitted by law.\n\nWritten by Jim Meyering."); 
    }

    else{
        perror("Wrong");
        exit(1);
    }
    
    return 1;
}


typedef struct commands{
    char *command;
    int (*func)(char **args);
}commands;

struct commands internal[]={{"cd", myshell_cd},{"pwd", myshell_pwd},{"echo", myshell_echo}};

void *thread_call(void * arg){
    char *command=(char *)arg;
    system(command);
    pthread_cancel(pthread_self());
    return NULL;
}

int execute_process(char **args){
    int x=sizeof(internal)/sizeof(struct commands);
    for(int i=0; i<x; i++){
        if(strcmp(internal[i].command,args[0])==0){
            return (internal[i].func(args));
        }
    }

    int size=strlen(args[0]);
    if(args[0][size-1]=='t' && args[0][size-2]=='&'){
        char *c=convert_to_string(args);
        pthread_t newthread;
        pthread_create(&newthread, NULL, thread_call, c);
        pthread_join(newthread, NULL);
        return 1;
    }

    else{
        pid_t cpid=fork();
        if(cpid==-1){
            perror("Error");
        }
        if(cpid==0){
            char *x=malloc(1024*sizeof(char));
            strcat(x,array);
            strcat(x,"/");
            strcat(x,args[0]);
            if(execvp(x,args)==-1){
                    perror("Wrong Command");
                    exit(1);
            }
        }
        else{
            wait(NULL);
        }
    }
    return 1;
}

char * read_command(void){
    char  *cmd=NULL;
    size_t length=0;
    getline(&cmd, &length, stdin);
    return cmd;
}


char ** split_command(char * command){
    int size=64;
    char *tok;
    char **arguments=malloc(size * sizeof(char*));

    if(!arguments){
        perror("Error");
        exit(1);
    }

    int i=0;
    //splitting the command on basis of delimiter
    tok=strtok(command,delimiter);
    while(tok!=NULL){
        arguments[i]=tok;
        i++;

        if(i>=size){
            size+=64;
            arguments=realloc(arguments,size*sizeof(char*));
            if(!arguments){
                printf("Error");
                exit(1);
            }
        }
        tok=strtok(NULL,delimiter);
    }
    arguments[i]=NULL;
    return arguments;
}

int main(int argc, char *argv[]){
    printf("\n\nWelcome to your shell! To exit from the shell, press ^C\n\n");
    char * command;
    char **args;
    int flag;
    array=malloc(1000*sizeof(char));
    getcwd(array,1000);

    do{
        printf("\nmyshell>> ");
        command=read_command();
        args=split_command(command);
        flag=execute_process(args);
        free(command);
        free(args);
    }
    while(flag);
    return 0;
}

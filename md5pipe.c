#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <signal.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include "md5.hpp"
#include <cstdio>
#include <iostream>
char result [40];
pid_t pidChild;
char userString[20];
void validResult(int signal_number){
    if(strlen(result)==32){
        printf("encrypted by process %d : %s\n",getpid(), result);
        exit(0);
    }
    else{
        printf("no");
    }

}
int main()
{
    //using 2 pipes ,one for sending the input to the child and the second one for the child to sent the result to the parent.
    int fd1[2];  // Used to store two ends of first pipe
    int fd2[2];  // Used to store two ends of second pipe
    if (pipe(fd1)==-1)
    {
        fprintf(stderr, "Pipe Failed" );
        return 1;
    }
    if (pipe(fd2)==-1)
    {
        fprintf(stderr, "Pipe Failed" );
        return 1;
    }
    //define the signal to check validation.
    signal(SIGHUP, validResult);
    //reading input from user ,maximum 20 chars.

    printf("plain text:");
    fgets(userString, 20, stdin);
    pidChild = fork();
    if (pidChild < 0)
    {
        fprintf(stderr, "fork Failed" );
        return 1;
    }

    // Parent process
    else if (pidChild > 0)
    {
        char md5Reading[32];

        close(fd1[0]);// Close reading end of first pipe ,no need to read just passing the string to child.
        close(fd2[1]); // Close writing of the second pipe ,only need to read!
        //writing the input string into the pipe.
        write(fd1[1], userString, strlen(userString)+1);
       // close(fd1[1]);
        // Read string from child, print it and kill the son.
        read(fd2[0], result, 32);
        wait(NULL);
    }

        // child process
    else
    {
        close(fd1[1]);  // Close writing end of first pipe
        close(fd2[0]);
        // reading the string coming from the father.
        char origin[20];
        pid_t parentId=getppid();
        read(fd1[0], origin, 20);
        //close(fd1[0]);

        // Writing the result into the pipe and sending signal for checking the result.
        write(fd2[1], md5(origin).c_str(), 32);
        kill(parentId,SIGHUP);
        //kill(validResult,SIGTERM);

        //exit(0);
    }
    return 0;
}

#include <string.h>

#include <stdio.h>

#include <stdlib.h>

#include <fcntl.h>

#include <sys/uio.h>

#include <sys/types.h>

#include <unistd.h>

#include <signal.h>

#include <sys/fcntl.h>
char result [40];
void validResult(){
    if(strlen(result)!=32){
        printf("not valid");
        exit(0);
    }

}
int main()
{
    //using 2 pipes ,one for sending the input to the child and the second one for the child to sent the result to the parent.
    int fd1[2];  // Used to store two ends of first pipe
    int fd2[2];  // Used to store two ends of second pipe
    //reading input from user ,maximum 20 chars.
    char userString[20];
    printf("plain text:");
    scanf("%19s", userString);
    pid_t p;
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
    signal(SIGTERM, validResult);
    p = fork();

    if (p < 0)
    {
        fprintf(stderr, "fork Failed" );
        return 1;
    }

        // Parent process
    else if (p > 0)
    {
        char md5Reading[32];

        close(fd1[0]);  // Close reading end of first pipe ,no need to read just passing the string to child.

        //writing the input string into the pipe.
        write(fd1[1], userString, strlen(userString)+1);
        close(fd1[1]);

        // Waiting for the child to send the string
        wait(NULL);

        close(fd2[1]); // Close writing of the second pipe ,only need to read!


        //waiting for signal handler
        wait(NULL);
        // Read string from child, print it and kill the son.
        read(fd2[0], md5Reading, 32);
        printf("encrypted by process %d : %s\n",getpid(), md5Reading);
        close(fd2[0]);
        //killing the child.
        kill(p,SIGKILL);
    }

        // child process
    else
    {
        close(fd1[1]);  // Close writing end of first pipe

        // reading the string coming from the father.
        char origin[20];
        read(fd1[0], origin, 20);
      //  unsigned char result[MD5_DIGEST_LENGTH];
        MD5(origin, strlen(origin), result);
        // Close both reading ends
        close(fd1[0]);
        close(fd2[0]);
        // Writing the result into the pipe and sending signal for checking the result.
        write(fd2[1], result, strlen(result)+1);
        close(fd2[1]);
        kill(p,SIGTERM);

        //exit(0);
    }
}

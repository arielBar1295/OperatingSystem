#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/fcntl.h>
//for holding the children.
int children[5];
int j;

// to know when a signal is caught.
int catch() {
    signal(SIGINT, catch);
    printf("PID %d caught one\n", getpid());
    if (j > -1)
        kill(children[j], SIGINT);  
}

int main(){

    int zombie;
    int status;
    int pid;
    // a hendler
    signal(SIGINT, catch);
    int i;
    //creating the children
    for(i=0; i<5; i++){
        if((pid=fork()) ==  0){
            printf("PID %d ready\n", getpid());
            j = i-1;
            pause();
            exit(0);
        }
        else
            children[i] = pid;    //update the array
    }
    sleep(2);
    //start sending signals
    kill(children[4], SIGINT);
    sleep(2);
    for(i=0; i<5; i++){
        zombie = wait(&status);
        printf("%d is dead\n", zombie);
    }
    exit(0);

}

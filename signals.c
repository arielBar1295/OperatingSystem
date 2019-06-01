#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
//global
//for holding the children.
int children[5];
//for tracking
int ind;

// to know when a signal is caught.
int catch() {
    signal(SIGINT, catch);
    printf("PID %d caught one\n", getpid());
    if (ind > -1) {
        kill(children[ind], SIGINT);
    }
}

int main(){

    int zombie;
    int status;
    int pid;
    // a hendler
    signal(SIGINT, catch);
    int j;
    //creating the children
    for(j=0; j<5; j++){
        if((pid=fork()) ==  0){
            printf("PID %d ready\n", getpid());
            ind = j-1;
            pause();
            exit(0);
        }
        else {
            children[j] = pid;    //update the array
        }
    }
    sleep(2);
    //start sending signals
    kill(children[4], SIGINT);
    sleep(2);
    for(j=0; j<5; j++){
        zombie = wait(&status);
        printf("%d is dead\n", zombie);
    }
    exit(0);

}

#include <signal.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/fcntl.h>
//for the children
int children[5];  
//for tracking the chikdren
int ind;                    

// to know when a signal is caught.
int catch() {
    signal(SIGINT, catch); 
    printf("PID %d caught one\n", getpid());
    if (ind >= 0) {
        //sends ignal to the brother
        kill(children[ind], SIGINT);
    }
}
int main() {
    int zombie;
    int pid;
    int j;
    int status;
    //a hendler
    signal(SIGINT, catch);
    //creating the children
    for(j=0; j<5; j++){
        if((pid=fork()) ==  0){      		
            printf("PID %d ready\n", getpid());
            ind = j-1;
            pause(); 			
            exit(0);  			
        }
        else {  
            //update the children array
            children[j] = pid;
        }
    }
    sleep(2); 
    //send a signal to the child.
    kill(children[4], SIGINT);     		
    sleep(2);                 			
    for(j=0; j<5; j++){
        zombie = wait(&status); 		
        printf("Process %d is dead\n", zombie);
    }
    exit(0);
}

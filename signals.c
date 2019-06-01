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

// function to activate when a signal is caught
int catch() {
    signal(SIGINT, catch);  // re-assign the signal catcher
    printf("PID %d caught one\n", getpid());
    if (j > -1) {
        kill(children[ind], SIGINT);
    }// send signal to next child in cpid
}
int main() {
    int i;
    int zombie;
    int status;
    int pid;
    signal(SIGINT, catch);
    for(i=0; i<5; i++){
        if((pid=fork()) ==  0){      		// create new child
            printf("PID %d ready\n", getpid());
            ind = i-1;
            pause(); 			// wait for signal
            exit(0);  			// end process (become a zombie)
        }
        else {                // Only father updates the cpid array.
            children[i] = pid;
        }
    }
    sleep(2);     			// allow children time to enter pause
    kill(children[4], SIGINT);     		// send signal to first child
    sleep(2);                 			// wait for children to become zombies
    for(i=0; i<5; i++){
        zombie = wait(&status); 		// collect zombies
        printf("Process %d is dead\n", zombie);
    }
    exit(0);
}

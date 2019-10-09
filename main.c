#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

#define ERROR -1
#define PROCS 4

static int idata = 11;            /* Allocated in data segment */

void runChildLogic(int *initialNumber, int childId, pid_t id) {
    for(int i = 0; i < PROCS; ++i){
        *initialNumber += i;
        sleep(2);
        printf("(child(%d)[%d]): idata: %d| ppid: %d, childID: %d, PROCESS%d", childId, i, idata, getppid(), id, getpid());
    }
}

void runParentLogic(int *initialNumber, int *factor, pid_t childProccess) {
    for(int i = 0; i < 1; ++i){
        *initialNumber *= (*factor + i);
        *factor = *factor - 1;
        sleep(1);
        printf("(parent[%d]): idata: %d | istack: %d ", i, idata, *factor);
        printf("My childs is: %d", childProccess);
    }
}

int main(int argc, char *argv[]){
    int istack = 100;              /* Allocated in stack segment */
    pid_t childPid;
    pid_t id_process;
    id_process = getpid();

    for(int i = 0; i < 1; ++i){
        switch (childPid = fork()) {
            case -1:
                return ERROR;

            case 0:
                runChildLogic(&idata, i, id_process);
                break;

            default:
                runParentLogic(&idata, &istack, id_process);
                waitpid(childPid, NULL, 0);
                break;
        }
        printf("PID=%ld %s idata=%d istack=%d\n", (long) getpid(),
               (childPid == 0) ? "(child) " : "(parent)", idata, istack);
    }
    /* Both parent and child come here */
    exit(EXIT_SUCCESS);
}

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

#define ERROR -1
#define PROCS 4

static int idata = 11;            /* Allocated in data segment */

void collectCpuData(int data, pid_t id_cpu){
    printf("CPU | data: %d, ID: %d\n", data, id_cpu);
}

void collectMemData(int data, pid_t id_ram){
    printf("Memory | data: %d, ID: %d\n", data, id_ram);
}

void collectNetData(int data, pid_t id_net) {
    printf("Network | data: %d, ID: %d\n", data, id_net);
}

void collectDiskData(int data, pid_t id_disk){
    printf("Disk | data: %d, ID: %d\n", data, id_disk);
}

void runChildLogic(int *initialNumber, int childId, pid_t id_cpu, pid_t id_ram, pid_t id_net, pid_t id_disk) {
    for(int i = 0; i < PROCS; ++i){
        *initialNumber += i;
        sleep(2);
        if(i == 0) {
            collectCpuData(childId, id_cpu);
        } else if (i == 1){
            collectMemData(childId, id_ram);
        } else if (i == 2){
            collectNetData(childId, id_net);
        } else if (i == 3){
            collectDiskData(childId, id_disk);
        } else {
            printf("ERROR TO CREATE SOME CHILD PROCESS");
        }
    }
}

void runParentLogic(int *initialNumber, int *factor, pid_t id_cpu, pid_t id_ram, pid_t id_net, pid_t id_disk) {
    for(int i = 0; i < 1; ++i){
        *initialNumber *= (*factor + i);
        *factor = *factor - 1;
        sleep(1);
        printf("(parent[%d]): idata: %d | istack: %d ", i, idata, *factor);
        printf("My child's ID's: cpu: %d | ram: %d | network: %d | disk: %d ", id_cpu, id_ram, id_net, id_disk);
    }
}

int main(int argc, char *argv[]){
    int istack = 100;              /* Allocated in stack segment */
    pid_t childPid;
    pid_t id_process_cpu;
    pid_t id_process_ram;
    pid_t id_process_net;
    pid_t id_process_disk;

    id_process_cpu = getpid();
    id_process_ram = getpid();
    id_process_net = getpid();
    id_process_disk = getpid();

    for(int i = 0; i < 1; ++i){
        switch (childPid = fork()) {
            case -1:
                return ERROR;

            case 0:
                runChildLogic(&idata, i, id_process_cpu, id_process_ram, id_process_net, id_process_disk);
                break;

            default:
                runParentLogic(&idata, &istack, id_process_cpu, id_process_ram, id_process_net, id_process_disk);
                waitpid(childPid, NULL, 0);
                break;
        }
        printf("PID=%ld %s idata=%d istack=%d\n", (long) getpid(),
               (childPid == 0) ? "(child) " : "(parent)", idata, istack);
    }
    /* Both parent and child come here */
    exit(EXIT_SUCCESS);
}

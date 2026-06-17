#ifndef PCB_H
#define PCB_H

#include <stdint.h>
#include <stddef.h>

#define MAX_PROCESSES 10

typedef uint32_t pid_t;
typedef uint32_t clock_t;
typedef uint32_t time_t;

enum State
{
    Free,
    Sleeping,
    Swapper,
    Ready,
    Running,
    Zombie,
};

struct pcb
{
    pid_t pid;
    enum State state;
    uint32_t uid, eiud, gid, egid;
    uint32_t exit_code;
    uint32_t eax, ebx, ecx, edx, edi, esi, ebp, esp, eip;
    pid_t ppid;
    char* process_name;
};

void fillProcessTable();
void initMultitasking();
struct pcb* initNewProcess(pid_t pid);
int fork();
int exec(struct pcb* pcb);
void execCurrentProcess();
void showAllProcesses();
void yield();

#endif
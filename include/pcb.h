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

struct tss {
	uint32_t link;
	uint32_t esp0;
	uint32_t ss0;
	uint32_t esp1;
	uint32_t esp2;
	uint32_t ss2;
	uint32_t sr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t edi;
	uint32_t es;
	uint32_t cs;
	uint32_t ss;
	uint32_t ds;
	uint32_t fs;
	uint32_t gs;
	uint32_t ldtr;
	uint32_t iopb;

}__attribute__((packed));

/* WAIT A MINUTE, IN THIS OPERATING SYSTEM, ALL PROCESSES ARE RING 0, NOT IN OTHER RINGS */

struct pcb
{
    pid_t pid;
    enum State state;
    uint32_t uid, eiud, gid, egid;
    uint32_t exit_code;
    struct tss tss; /* HAHA, I DO NOT USE SOFTWARE MULTITASKING HAHAHAHAHA */
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
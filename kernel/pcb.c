#include "include/console.h"
#include "include/pcb.h"
#include "include/string.h"

#include <stdint.h>
#include <stddef.h>

struct pcb processes_table[MAX_PROCESSES];
struct pcb* current_process = &processes_table[0];
struct pcb* next_process = &processes_table[1];

pid_t searchForFreePID()
{
    for (pid_t i = 0; i < MAX_PROCESSES; ++i)
    {
        if (processes_table[i].state == Free)
        {
            return processes_table[i].pid;
        }
    }

    return 0;
}

void func()
{
    /*
    int res = fork();
    if (res == -1)
    {
        printk("ERROR WHILE EXECUTING FORK()\n");
        // yield();

    } else if (res == 0)
    {
        printk("Hello from child process\n");
        // yield();
        // while (1) {}

    } else
    {
        printk("Hello from the father process\n");
        // yield();
    }
    // yield();
    */
    printk("process1");
    yield();
}

void func2()
{
    printk("process2");
    yield();
}

void func3()
{
    printk("process3");
    __asm__ volatile ("sti");
    while(1) {__asm__ volatile ("hlt");}
}

void initMultitasking()
{
    fillProcessTable();
    struct pcb* newProcess = initNewProcess(1);
    newProcess->state = Ready;
    newProcess->ppid = 0;
    newProcess->eip = (uint32_t)func;
    struct pcb* newProcess2 = initNewProcess(2);
    newProcess2->state = Ready;
    newProcess2->ppid = 1;
    newProcess2->eip = (uint32_t)func2;
    struct pcb* newProcess3 = initNewProcess(3);
    newProcess3->state = Ready;
    newProcess3->ppid = 2;
    newProcess3->eip = (uint32_t)func3;
}

struct pcb* initNewProcess(pid_t pid) /* ONLY FOR KERNEL USE */
{
    struct pcb* newProcess = &processes_table[pid];
    newProcess->state = Ready;
    newProcess->pid = pid;
    newProcess->process_name = "empty process";

    newProcess->tss.esp0 = 0x600000 + pid * 4096; /* 4 KB OF STACK FOR EACH INTERVENTION OF A PROCESS */
    newProcess->tss.ss0 = 0x10; /* DATA SELECTOR (KERNEL) */

    newProcess->tss.es = 0x10;
    newProcess->tss.ds = 0x10;
    newProcess->tss.fs = 0x10;
    newProcess->tss.gs = 0x10;
    newProcess->tss.ss = 0x10;

    newProcess->tss.eflags = 0;

    newProcess->tss.eax = 0;
    newProcess->tss.ebx = 0;
    newProcess->tss.ecx = 0;
    newProcess->tss.edx = 0;
    newProcess->tss.edi = 0;
    newProcess->tss.esi = 0;
    newProcess->tss.ebp = 0;
    newProcess->tss.esp = 0x3FF000 + pid * 8192; /* Each process has 8 KB of stack */
    newProcess->tss.eip = 0;
    newProcess->tss.ebp = 0;

    return newProcess;
}

int fork()
{
    pid_t childPID = searchForFreePID();
    struct pcb* childProcess = initNewProcess(childPID);
    if (childPID != 0)
    {
        childProcess->ppid = current_process->pid;
        childProcess->eax = current_process->eax;
        childProcess->ebx = current_process->ebx;
        childProcess->ecx = current_process->ecx;
        childProcess->edx = current_process->edx;
        childProcess->edi = current_process->edi;
        childProcess->esi = current_process->esi;
        childProcess->ebp = current_process->ebp;
        childProcess->esp = current_process->esp;
        childProcess->eip = current_process->eip;

        if (current_process->pid != current_process->ppid)
        {
            return 0;

        } else
        {
            return childProcess->pid;
        }

    } else
    {
        return -1;
    }
}

int exec(struct pcb* pcb)
{
    if (current_process != NULL && pcb != NULL)
    {
        current_process = pcb;
        __asm__ volatile (
            "movl %0, %%esp\n\t"
            "jmp *%1\n\t"
            :
            : "m"(current_process->esp), "m"(current_process->eip)
            : "memory"
        );
        return 0;
    }

    return -1;
}

void saveCurrentProcessContext()
{
    __asm__ volatile ("movl %%eax, %0" : "=m"(processes_table[current_process->pid].eax) ::);
    __asm__ volatile ("movl %%ebx, %0" : "=m"(processes_table[current_process->pid].ebx) ::);
    __asm__ volatile ("movl %%edx, %0" : "=m"(processes_table[current_process->pid].edx) ::);
    __asm__ volatile ("movl %%ecx, %0" : "=m"(processes_table[current_process->pid].ecx) ::);
    __asm__ volatile ("movl %%edi, %0" : "=m"(processes_table[current_process->pid].edi) ::);
    __asm__ volatile ("movl %%esi, %0" : "=m"(processes_table[current_process->pid].esi) ::);
    __asm__ volatile ("movl %%esp, %0" : "=m"(processes_table[current_process->pid].esp) ::);
}

void execCurrentProcess()
{
    exec(current_process);
}

void fillProcessTable()
{
    for (pid_t i = 0; i < MAX_PROCESSES; ++i)
    {
        processes_table[i].pid = i;
        processes_table[i].ppid = -1;
        processes_table[i].state = Free;
        processes_table[i].process_name = "free pid";
    }
}

pid_t searchForRunning()
{
    int found = 0;
    for (pid_t i = 0; i < MAX_PROCESSES; ++i)
    {
        if (processes_table[i].state == Running)
        {
            found++;
        }
    }
    
    return found;
}

void yield()
{
    saveCurrentProcessContext();
    current_process = next_process;
    current_process->state = Running;
    if (current_process->eip != 0 && current_process->state != Free)
    {
        pid_t next_pid = (next_process->pid+1) % MAX_PROCESSES;
        next_process = &processes_table[next_pid];
        next_process->state = Ready;

    } else
    {
        if (searchForRunning() < 1)
        {
            current_process = &processes_table[0];
            next_process = &processes_table[1];
            current_process = next_process;
            next_process->state = Ready;
            current_process->state = Running;
        
        } else
        {
            current_process = &processes_table[0];
            next_process = &processes_table[1];
        }
    }

    exec(current_process);
}

void showAllProcesses()
{
    char buf[1024];
    for (pid_t i = 0; i < MAX_PROCESSES; ++i)
    {
        if (processes_table[i].state != Free && processes_table[i].state != Zombie && processes_table[i].state != Sleeping)
        {
            itoa(processes_table[i].pid, buf, 10);
            printk("PID: ");
            printk(buf);
            printk(" | ");
            if (processes_table[i].ppid == -1)
            {
                printk("FATHER PID: NONE | ");
            
            } else
            {
                itoa(processes_table[i].ppid, buf, 10);
                printk("FATHER PID: ");
                printk(buf);
                printk(" | ");
            }

            switch (processes_table[i].state)
            {
                case Swapper:
                    printk("STATE: SWAPPER | ");
                    break;
                
                case Running:
                    printk("STATE: RUNNING | ");
                    break;

                case Ready:
                    printk("STATE: READY | ");
                    break;

                default:
                    printk("STATE: UNKNOWN | ");
                    break;
            }

            printk("PROCESS NAME: ");
            printk(processes_table[i].process_name);
            printk("\n");
        }
    }
}
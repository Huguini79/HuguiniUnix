#include "include/console.h"
#include "include/pcb.h"
#include "include/string.h"
#include "include/gdt.h"

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

void addTSS(struct pcb* pcb)
{
    uint32_t tss_address = (uint32_t)&(pcb->tss);
    setSegmentDescriptor(3+pcb->pid, sizeof(pcb->tss) - 1, (tss_address & 0xFFFF), (tss_address >> 16) & 0xFF, 0x89, 0x40, (tss_address >> 24) & 0xFF);
}

void initMultitasking()
{
    fillProcessTable();
    struct pcb* newProcess = initNewProcess(1, (uint32_t)func);
    newProcess->state = Ready;
    newProcess->ppid = 0;
    struct pcb* newProcess2 = initNewProcess(2, (uint32_t)func2);
    newProcess2->state = Ready;
    newProcess2->ppid = 1;
    struct pcb* newProcess3 = initNewProcess(3, (uint32_t)func3);
    newProcess3->state = Ready;
    newProcess3->ppid = 2;

}

struct pcb* initNewProcess(pid_t pid, uint32_t func) /* ONLY FOR KERNEL USE */
{
    struct pcb* newProcess = &processes_table[pid];
    newProcess->state = Ready;
    newProcess->pid = pid;
    newProcess->process_name = "empty process";

    newProcess->tss.esp0 = 0x600000 + pid * 4096; /* 4 KB OF STACK FOR EACH INTERVENTION OF A PROCESS */
    newProcess->tss.ss0 = 0x10; /* DATA SELECTOR (KERNEL) */

    newProcess->tss.cs = 0x08; /* CODE SELECTOR (KERNEL) */

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
    newProcess->tss.eip = func;
    newProcess->tss.ebp = 0;

    newProcess->tss.eflags = 0x202;

    newProcess->tss.iopb = 0x80000000;

    addTSS(newProcess);

    return newProcess;
}

int fork()
{
    pid_t childPID = searchForFreePID();
    struct pcb* childProcess = initNewProcess(childPID, current_process->tss.eip);
    if (childPID != 0)
    {
        childProcess->ppid = current_process->pid;
        childProcess->tss.eax = current_process->tss.eax;
        childProcess->tss.ebx = current_process->tss.ebx;
        childProcess->tss.ecx = current_process->tss.ecx;
        childProcess->tss.edx = current_process->tss.edx;
        childProcess->tss.edi = current_process->tss.edi;
        childProcess->tss.esi = current_process->tss.esi;
        childProcess->tss.ebp = current_process->tss.ebp;
        childProcess->tss.esp = current_process->tss.esp;
        childProcess->tss.eip = current_process->tss.eip;

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

        struct
        {
            uint32_t offset;
            uint16_t selector;

        } __attribute__((packed)) far_target;

        current_process = pcb;

        far_target.offset = 0;
        far_target.selector = (3 + current_process->pid) * 8;

        __asm__ volatile ("ljmp *%0":: "m"(far_target));
        return 0;
    }

    return -1;
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
    current_process = next_process;
    current_process->state = Running;
    if (current_process->tss.eip != 0 && current_process->state != Free)
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

    // exec(current_process);
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
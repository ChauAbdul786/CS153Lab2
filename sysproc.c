#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  int status;
  if(argint(0, &status) < 0) {
    return -1;
  }

  exit(status);
  return 0;  // not reached
}

int
sys_wait(void)
{
  int* status;
  argptr(0, (void*)&status, sizeof(status));
  return wait(status);
}

int
sys_waitpid(void)
{
  int pid; 
  int* status;
  argint(0, &pid); //From exit
  argptr(1, (void*)&status, sizeof(status)); //From wait

  return waitpid(pid, status, 0); //Options field 0 for now
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_getpriority(void)
{
  return getpriority();
}

int
sys_setpriority(void)
{
  int p;
  argint(0, &p);
  
  //Priority ranges from 0 - 31 (test program ranges)
  if(p < 0){
    p = 0;
  }else if(p > 16){
    p = 16;
  }

  return setpriority(p);
}

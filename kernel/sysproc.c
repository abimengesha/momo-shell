#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "fs.h"
#include "file.h"  

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//t sys_shutdown
uint64
sys_shutdown(void) {
//create a pointer to mem location where qemu looks for shutdown and reboot commands
  volatile uint32 *test_dev = (uint32 *) VIRT_TEST; // VIRT_TEST is referring to the physical memory 
  // where QEMU listens for shutdown and reboot commands.
  *test_dev = 0x5555;  // Shutdown command this hex num is is hardcoded in qemu
  return 0; // if works return 0
}

// t sys_reboot
uint64
sys_reboot(void) {
//create a pointer to mem location where qemu looks for shutdown and reboot commands
  volatile uint32 *test_dev= (uint32 *) VIRT_TEST;
  *test_dev = 0x7777;  // Reboot command this hex number is hardcoded in qemu
  return 0; //if it works return zero
}

// turns on system call for curr process
uint64
sys_strace_on(void)
{
  struct proc *p = myproc();  //  current process
  p->strace_on = 1;      // syscall tracing on
  return 0;
}

// Get the current working directory
uint64
sys_getcwd(void) {
    uint64 buf; //user space buff address
    int size; //size of buffer
    
    struct proc *p = myproc(); //get current process
    struct inode *cwd_inode = p->cwd; // get curr working dir inode
    struct inode *parent_inode; 
    
    char temp_path[MAXPATH] = {0}; // temp buff to store path
    int path_len = 0; 

    // Retrieve user-space arguments
    argaddr(0, &buf); // get buff address from user input
    argint(1, &size); // get size of the buffer
    
	// validate buff size
    if (size <= 0 || size > MAXPATH) {
        return -1; //rerturn bc invalid size
    }

    // make sure the process has a valid working directory
    if (cwd_inode == 0) {
        return -1; // return bc no working dir
    }

    ilock(cwd_inode); //lock prevent modifications

   
    // If we are at the root directory  return "/"
    if (cwd_inode->inum == ROOTINO) {
        safestrcpy(temp_path, "/", MAXPATH);
        iunlock(cwd_inode);
        goto copy_to_user; // skip and copy result
    }

    // Go  up to create the full path
    while (cwd_inode->inum != ROOTINO) {
        char dirname[DIRSIZ];
        struct dirent de;
        uint off;
        int found = 0; // flag to indicate if dir found

        // Unlock cwd_inode before looking up its parent
        parent_inode = dirlookup(cwd_inode, "..", &off);
        iunlock(cwd_inode);  // Release cwd_inode before proceeding

        if (parent_inode == 0) {
            return -1;  // If we can't find parent fail
        }

        ilock(parent_inode); // lock parent dir inode

        // find the name of cwd_inode in the parent directory
        for (off = 0; off < parent_inode->size; off += sizeof(de)) {
            if (readi(parent_inode, 0, (uint64)&de, off, sizeof(de)) != sizeof(de)) {
                iunlock(parent_inode);
                return -1; // error reading
            }

			// check if this entry matches curr dirr inode
            if (de.inum == cwd_inode->inum) {
                safestrcpy(dirname, de.name, DIRSIZ);
                found = 1;
                break; // stop searching after found
            }
        }

        if (!found) {
            iunlock(parent_inode);
            return -1; // directory entry not found
        }

        // make directory name to temp_path
        int dir_len = strlen(dirname);
        if (path_len + dir_len + 2 >= MAXPATH) {  // +2 accounts for '/' and '\0'
            iunlock(parent_inode);
            return -1;
        }

		// move exsisiting path forward for new dir name
        memmove(temp_path + dir_len + 1, temp_path, path_len);
        memmove(temp_path + 1, dirname, dir_len);
        temp_path[0] = '/'; //add '/' before dir name
        path_len += dir_len + 1;

        // Move to parent directory
        cwd_inode = parent_inode;
    }

    iunlock(cwd_inode); //unlock last inode

    // null-terminate the path
    temp_path[path_len] = '\0';

copy_to_user:
    // Ensure buffer size is enough
    if (path_len + 1 > size) {
        return -1; //buff too small
    }

    // copy  path to user buffer
    if (copyout(p->pagetable, buf, temp_path, path_len + 1) < 0) {
        return -1; //copying failed
    }

    return 0; //was successful
    
}

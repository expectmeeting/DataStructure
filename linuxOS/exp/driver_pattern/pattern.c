#include<stdio.h>
#include<linux/module.h>
#include<linux/sched.h>
#include<linux/slab.h>
#include<asm/mmu.h>
#include<asm/page.h>
#include<asm/pgtable.h>

static int __init start(void)
{
    
    return 0;
}
module_init(start);

static void __exit end(void)
{   

}
module_exit(end);

MODULE_LICENSE("GPL");

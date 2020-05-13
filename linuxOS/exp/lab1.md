[TOC]

# 实验1实验报告

## 练习1

- make 执行命令分析

~~~shell
[root@CentOS7 lab1]# make V=
+ cc kern/init/init.c
gcc -Ikern/init/ -march=i686 -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -nostdinc  -fno-stack-protector -Ilibs/ -Ikern/debug/ -Ikern/driver/ -Ikern/trap/ -Ikern/mm/ -c kern/init/init.c -o obj/kern/init/init.o
+ cc kern/libs/stdio.c
gcc -Ikern/libs/ -march=i686 -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -nostdinc  -fno-stack-protector -Ilibs/ -Ikern/debug/ -Ikern/driver/ -Ikern/trap/ -Ikern/mm/ -c kern/libs/stdio.c -o obj/kern/libs/stdio.o
+ cc kern/libs/readline.c
gcc -Ikern/libs/ -march=i686 -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -nostdinc  -fno-stack-protector -Ilibs/ -Ikern/debug/ -Ikern/driver/ -Ikern/trap/ -Ikern/mm/ -c kern/libs/readline.c -o obj/kern/libs/readline.o
+ cc kern/debug/panic.c
gcc -Ikern/debug/ -march=i686 -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -nostdinc  -fno-stack-protector -Ilibs/ -Ikern/debug/ -Ikern/driver/ -Ikern/trap/ -Ikern/mm/ -c kern/debug/panic.c -o obj/kern/debug/panic.o
+ cc kern/debug/kdebug.c
gcc -Ikern/debug/ -march=i686 -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -nostdinc  -fno-stack-protector -Ilibs/ -Ikern/debug/ -Ikern/driver/ -Ikern/trap/ -Ikern/mm/ -c kern/debug/kdebug.c -o obj/kern/debug/kdebug.o
gcc -Ikern/debug/ -march=i686 -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -nostdinc  -fno-stack-protector -Ilibs/ -Ikern/debug/ -Ikern/driver/ -Ikern/trap/ -Ikern/mm/ -c kern/debug/kmonitor.c -o obj/kern/debug/kmonitor.o
+ cc kern/driver/clock.c
gcc -Ikern/driver/ -march=i686 -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -nostdinc  -fno-stack-protector -Ilibs/ -Ikern/debug/ -Ikern/driver/ -Ikern/trap/ -Ikern/mm/ -c kern/driver/clock.c -o obj/kern/driver/clock.o
+ cc kern/driver/console.c
gcc -Ikern/driver/ -march=i686 -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -nostdinc  -fno-stack-protector -Ilibs/ -Ikern/debug/ -Ikern/driver/ -Ikern/trap/ -Ikern/mm/ -c kern/driver/console.c -o obj/kern/driver/console.o
+ cc kern/driver/picirq.c
gcc -Ikern/driver/ -march=i686 -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -nostdinc  -fno-stack-protector -Ilibs/ -Ikern/debug/ -Ikern/driver/ -Ikern/trap/ -Ikern/mm/ -c kern/driver/picirq.c -o obj/kern/driver/picirq.o
+ cc kern/driver/intr.c
gcc -Ikern/driver/ -march=i686 -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -nostdinc  -fno-stack-protector -Ilibs/ -Ikern/debug/ -Ikern/driver/ -Ikern/trap/ -Ikern/mm/ -c kern/driver/intr.c -o obj/kern/driver/intr.o
+ cc kern/trap/trap.c
gcc -Ikern/trap/ -march=i686 -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -nostdinc  -fno-stack-protector -Ilibs/ -Ikern/debug/ -Ikern/driver/ -Ikern/trap/ -Ikern/mm/ -c kern/trap/trap.c -o obj/kern/trap/trap.o
+ cc kern/trap/vectors.S
gcc -Ikern/trap/ -march=i686 -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -nostdinc  -fno-stack-protector -Ilibs/ -Ikern/debug/ -Ikern/driver/ -Ikern/trap/ -Ikern/mm/ -c kern/trap/vectors.S -o obj/kern/trap/vectors.o
+ cc kern/trap/trapentry.S
gcc -Ikern/trap/ -march=i686 -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -nostdinc  -fno-stack-protector -Ilibs/ -Ikern/debug/ -Ikern/driver/ -Ikern/trap/ -Ikern/mm/ -c kern/trap/trapentry.S -o obj/kern/trap/trapentry.o
+ cc kern/mm/pmm.c
gcc -Ikern/mm/ -march=i686 -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -nostdinc  -fno-stack-protector -Ilibs/ -Ikern/debug/ -Ikern/driver/ -Ikern/trap/ -Ikern/mm/ -c kern/mm/pmm.c -o obj/kern/mm/pmm.o
+ cc libs/string.c
gcc -Ilibs/ -march=i686 -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -nostdinc  -fno-stack-protector -Ilibs/  -c libs/string.c -o obj/libs/string.o
+ cc libs/printfmt.c
gcc -Ilibs/ -march=i686 -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -nostdinc  -fno-stack-protector -Ilibs/  -c libs/printfmt.c -o obj/libs/printfmt.o
+ ld bin/kernel
ld -m    elf_i386 -nostdlib -T tools/kernel.ld -o bin/kernel  obj/kern/init/init.o obj/kern/libs/stdio.o obj/kern/libs/readline.o obj/kern/debug/panic.o obj/kern/debug/kdebug.o obj/kern/debug/kmonitor.o obj/kern/driver/clock.o obj/kern/driver/console.o obj/kern/driver/picirq.o obj/kern/driver/intr.o obj/kern/trap/trap.o obj/kern/trap/vectors.o obj/kern/trap/trapentry.o obj/kern/mm/pmm.o  obj/libs/string.o obj/libs/printfmt.o
+ cc boot/bootasm.S
gcc -Iboot/ -march=i686 -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -nostdinc  -fno-stack-protector -Ilibs/ -Os -nostdinc -c boot/bootasm.S -o obj/boot/bootasm.o
+ cc boot/bootmain.c
gcc -Iboot/ -march=i686 -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -nostdinc  -fno-stack-protector -Ilibs/ -Os -nostdinc -c boot/bootmain.c -o obj/boot/bootmain.o
+ cc tools/sign.c
gcc -Itools/ -g -Wall -O2 -c tools/sign.c -o obj/sign/tools/sign.o
gcc -g -Wall -O2 obj/sign/tools/sign.o -o bin/sign
+ ld bin/bootblock
ld -m    elf_i386 -nostdlib -N -e start -Ttext 0x7C00 obj/boot/bootasm.o obj/boot/bootmain.o -o obj/bootblock.o
'obj/bootblock.out' size: 472 bytes
build 512 bytes boot sector: 'bin/bootblock' success!
dd if=/dev/zero of=bin/ucore.img count=10000
10000+0 records in
10000+0 records out
5120000 bytes (5.1 MB) copied, 0.0154086 s, 332 MB/s
dd if=bin/bootblock of=bin/ucore.img conv=notrunc
1+0 records in
1+0 records out
512 bytes (512 B) copied, 3.9468e-05 s, 13.0 MB/s
dd if=bin/kernel of=bin/ucore.img seek=1 conv=notrunc
146+1 records in
146+1 records out
74820 bytes (75 kB) copied, 0.000226537 s, 330 MB/s
~~~

通过Makefile确认第一目标：

~~~makefile
TARGETS: $(TARGETS)
.DEFAULT_GOAL := TARGETS       # 通过.DEFAULT_GOAL确认了第一目标TARGETS
# 通过增加打印查看第一目标的依赖：
$(warning "goal : $(TARGETS)")
# Makefile:204: "goal :  bin/kernel bin/bootblock bin/sign bin/ucore.img"
~~~

将依赖有关的Makefile提出处理单独分析

~~~makefile
# create kernel target
kernel = $(call totarget,kernel) # 该函数扩展为$(addprefix $(BINDIR)$(SLASH),$(1))
# 即对kernel增加了一个前缀bin/，即$kernel = bin/kernel
$(kernel): tools/kernel.ld # kernel依赖于kernel.ld文件，其为ld链接脚本

$(kernel): $(KOBJS)	# kernel依赖于$(KOBJS)，其值为lib目录及附属目录下所有.c文件对应的.o文件
        @echo + ld $@
        $(V)$(LD) $(LDFLAGS) -T tools/kernel.ld -o $@ $(KOBJS) #输出二进制文件 bin/kernel
        @$(OBJDUMP) -S $@ > $(call asmfile,kernel) #反汇编生成汇编文件 obj/kernel.asm
        @$(OBJDUMP) -t $@ | $(SED) '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > $(call symfile,kernel) # 输出精简的符号表

$(call create_target,kernel) 
# 该函数扩展为$(eval $(call do_create_target,$(1),$(2),$(3),$(4),$(5)))
#
# -------------------------------------------------------------------
# create bootblock
bootfiles = $(call listf_cc,boot)
$(foreach f,$(bootfiles),$(call cc_compile,$(f),$(CC),$(CFLAGS) -Os -nostdinc))

bootblock = $(call totarget,bootblock)

$(bootblock): $(call toobj,$(bootfiles)) | $(call totarget,sign)
        @echo + ld $@
        $(V)$(LD) $(LDFLAGS) -N -e start -Ttext 0x7C00 $^ -o $(call toobj,bootblock)
        @$(OBJDUMP) -S $(call objfile,bootblock) > $(call asmfile,bootblock)
        @$(OBJCOPY) -S -O binary $(call objfile,bootblock) $(call outfile,bootblock)
        @$(call totarget,sign) $(call outfile,bootblock) $(bootblock) #输出符号标准的引导程序

$(call create_target,bootblock)
# create_target = $(eval $(call do_create_target,$(1),$(2),$(3),$(4),$(5)))
# -------------------------------------------------------------------
# create 'sign' tools
$(call add_files_host,tools/sign.c,sign,sign)
$(call create_target_host,sign,sign)

# -------------------------------------------------------------------
# create ucore.img
UCOREIMG        := $(call totarget,ucore.img)

$(UCOREIMG): $(kernel) $(bootblock)
        $(V)dd if=/dev/zero of=$@ count=10000 #生成10000*512字节长全为0的文件ucore.img
        $(V)dd if=$(bootblock) of=$@ conv=notrunc #将bootblock填入ucore.img的前512字节
        $(V)dd if=$(kernel) of=$@ seek=1 conv=notrunc #将kernel填入ucore.img,偏移为512字节

$(call create_target,ucore.img)

$(call finish_all)
~~~

- 符合规范的硬盘主引导扇区的特征是最后两字节为0x55aa

## 练习2 使用qemu执行并调试lab1中的软件

- 1、从CPU加电后执行的第一条指令开始，单步跟踪BIOS的执行

  CPU 8086在加电后重置寄存器，其中CS(代码段寄存器) 重置为0xffff，IP(指令指针寄存器)重置为0x0000；

  CPU 80386在加电后重置寄存器，其中CS(代码段寄存器) 重置为0xf000，EIP(指令指针寄存器)重置为0x0000fff0，在实模式阶段，仍采用CS:IP指向将要执行的指令；

  因此无论8086还是80386架构，加电后执行的第一条指令存放在内存地址0xffff0所对应的内存空间

  ~~~shell
  (gdb) x	/4x 0xffff0 # 以16进制打印内存地址0xffff0对应的连续4*4字节的数据
  0xffff0:        0x00e05bea	0x2f3630f0	0x392f3332	0x00fc0039 #汇编代码所对应的二进制机器指令
  (gdb) x	/i 0xffff0  # 打印内存地址0xffff0对应的一条汇编代码
  0xffff0:	ljmp   $0x3630,$0xf000e05b
  ~~~

- 2、在初始化位置0x7c00设置实地址断点,测试断点正常

  bios在完成一系列的初始化操作后，会将启动盘的第一个扇区即主引导扇区读取到内存的0x7c00中，因此将断点设置为0x7c00，即可调试引导程序。

  ~~~c
  (gdb) b *(0x7c00)
  (gdb) c    
  Continuing.          //对应的QEMU窗口打印“Booting from Hard Disk”
  => 0x7c00:	cli      //禁止中断
  ~~~

- 3、从0x7c00开始跟踪代码运行,将单步跟踪反汇编得到的代码与bootasm.S和 bootblock.asm进行比较。

  ~~~c
  (gdb) si			 //汇编歩进调试，遇到函数会进入
  => 0x7c01:	cld		 //清除方向标志
  (gdb) si
  => 0x7c02:	xor    %eax,%eax    //通过异或清空寄存器eax
  (gdb) si
  => 0x7c04:	mov    %eax,%ds     //清空数据段寄存器
  //通过前几条汇编代码能看出与bootasm.S中代码一致
  ~~~

- 4、自己找一个bootloader或内核中的代码位置，设置断点并进行测试

  ~~~c
  //在0x7c10处设置断点
  (gdb) b	*(0x7c10)
  Breakpoint 1 at	0x7c10
  (gdb) c
  Continuing.
  => 0x7c10:	mov    $0xd1,%al
  ~~~

## 练习3：分析bootloader进入保护模式的过程。（要求在报告中写出分析）

- 为何开启A20，以及如何开启A20

  A20的**缘由**是因为8086芯片的总线位宽为20bit，而数据位宽仅为16位，因此采用CS<<4+IP的方式来进行寻址，但这又出现了新的问题，采用这种寻址方式，CS:IP的最大值为0x10ffef超出了20位总线位宽的物理寻址能力，因此当寻址超过1M内存时，就会发生回卷，即0x1000e1和0x0000e1寻址到的内存是一样的。而后面推出的24位总线位宽80286能寻址16M的内存空间，而在其工作在实模式下时，为了向下兼容8086，就出现了A20 Gate。其原理是通过键盘控制器某一位输出与A20地址线相与，以此来控制A20地址线的打开和关闭。**至于为何要开启A20**，是因此在实模式下要访问高端内存，这个开关必须打开；而在保护模式下，不打开此开关将无法访问一半的可用内存。

  如何开启：

  ~~~c
  //分析bootasm.S汇编源码
  seta20.1: 
  # Wait for not busy(8042 input buffer empty).
      inb $0x64, %al       //读64h端口的值到寄存器al中，即读取键盘控制器8042的状态寄存器值
      testb $0x2, %al      //其第2位表明8042输入buffer是否有数据，1->有，0->无
      jnz seta20.1         //有数据则继续等待
  
      movb $0xd1, %al      # 0xd1 -> port 0x64               
      outb %al, $0x64      //向64h端口写0x64，表明将从60h写数据到8042的P2端口，A20的开关就在P2端口中的第2位(即1<2^1)                   
                           
  seta20.2:
  # Wait for not busy(8042 input buffer empty).   
      inb $0x64, %al                    
      testb $0x2, %al              
      jnz seta20.2                                                                       
   
      movb $0xdf, %al      # 0xdf -> port 0x60               
      outb %al, $0x60      //向60h端口写0xdf，该值表明将P2端口中的第2位置1，即打开A20
  ~~~

- 如何初始化GDT表

  关于x86架构的分段机制(寄存器和描述段的数据结构(段描述符))可以查看intel官方文档，BootLoader中的初始化即通过对寄存器进行初始化将段表建立起来

  ~~~assembly
   # Bootstrap GDT                                                                  
   .p2align 2                  # 4字节对齐，提升CPU处理数据性能(32位CPU一次能处理32bit的数据)
   gdt:                        # 定义全局描述符表，其位置在定义时就决定了
       SEG_NULLASM                            # intel规定第一段为空段，占8字节               
       SEG_ASM(STA_X|STA_R, 0x0, 0xffffffff)  # 表示可读可执行的代码段，占8字节      
       SEG_ASM(STA_W, 0x0, 0xffffffff)        # 表示可写的数据段，占8字节                                                                                         
   gdtdesc:                                                                         
       .word 0x17                  # 全局描述符表的大小，以字节为单位sizeof(gdt) - 1
       .long gdt                   # 全局描述符表的地址               
  ~~~
  
- 如何使能和进入保护模式

  ~~~shell
  lgdt gdtdesc            # 加载全局描述符表的地址和长度到全局描述符表寄存器(GDTR)中
  movl %cr0, %eax         # 对控制寄存器0(CR0)的位0置1，即保护使能位(protect enable,PE)，其使能分段机制
  orl $CR0_PE_ON, %eax    # 进行位与运算
  movl %eax, %cr0         
  
  # Jump to next instruction, but in 32-bit code segment. 
  # Switches processor into 32-bit mode.                  
  ljmp $PROT_MODE_CSEG, $protcseg   # 跳转执行代码段中的指令
  ~~~

## 练习4：分析bootloader加载ELF格式的OS的过程。（要求在报告中写出分析）

- bootloader如何读取硬盘扇区的

  ~~~c
  // bootmain.c
  // @readseg 从内核代码存放的物理地址偏移offset处取count字节存入虚拟地址va处
  static void readseg(uintptr_t va, uint32_t count, uint32_t offset) {             
      uintptr_t end_va = va + count; 
      // round down to sector boundary
      va -= offset % SECTSIZE; //由于磁盘IO是以扇区为单位，并保证va对应保存offset处的数据，因此将offset所在的扇区的数据一并读取，无用的数据存在 va - (offset % SECTSIZE)处
                                                                       
      // translate from bytes to sectors; kernel starts at sector 1    
      uint32_t secno = (offset / SECTSIZE) + 1;
                                                            
      // If this is too slow, we could read lots of sectors at a time. 
      // We'd write more to memory than asked, but it doesn't matter --
      // we load in increasing order.                                  
      for (; va < end_va; va += SECTSIZE, secno ++) {                  
          readsect((void *)va, secno); // 执行IO指令，直接从硬盘数据寄存器处读取1个扇区的数据
      }                                                                
  }
  
  static void readsect(void *dst, uint32_t secno) {                                 
      // wait for disk to be ready                                      
      waitdisk();
      
      outb(0x1F2, 1); // count = 1，读取一个扇区的数据
      outb(0x1F3, secno & 0xFF); //输入LBA参数的0...7位；      
      outb(0x1F4, (secno >> 8) & 0xFF); //输入LBA参数的15...8位
      outb(0x1F5, (secno >> 16) & 0xFF); //输入LBA参数的23...16位
      outb(0x1F6, ((secno >> 24) & 0xF) | 0xE0); // 输入LBA参数的24-27位（对应到0-3位），第四位为0表示从主盘读取，其余位被强制置为1,最初LBA的地址空间大小为2^28-1,对应137GB
      outb(0x1F7, 0x20); // cmd 0x20 - read sectors
                                                                        
      // wait for disk to be ready                                      
      waitdisk();                                                       
                                                                        
      // read a sector                                                  
      insl(0x1F0, dst, SECTSIZE / 4); // 从数据端口0x1F0读取数据，除以4是因为此处是以4个字节为单位的，这个从指令是以l(long)结尾这点可以推测出来
  }                                                                     
              
  static void waitdisk(void) {                       
      while ((inb(0x1F7) & 0xC0) != 0x40); //从物理地址0x1F7处读取磁盘状态寄存器，等待磁盘空闲
  }
  ~~~

- bootloader是如何加载ELF格式的OS？

  ~~~c
  // bootmain.c
  void bootmain(void) {                                
      // read the 1st page off disk               
      readseg((uintptr_t)ELFHDR, SECTSIZE * 8, 0);
      
      if (ELFHDR->e_magic != ELF_MAGIC) { // 判断内核程序是不是ELF格式               
           goto bad;                                                
      }                                                            
                                                                    
       struct proghdr *ph, *eph;                                    
                                                                    
       // load each program segment (ignores ph flags)              
       ph = (struct proghdr *)((uintptr_t)ELFHDR + ELFHDR->e_phoff);// 得到程序头的起始地址
       eph = ph + ELFHDR->e_phnum; // 得到程序头的结束地址
       for (; ph < eph; ph ++) { // 程序头的元素为描述段的数据结构，该数据结构包含一系列描述段的字段
           readseg(ph->p_va & 0xFFFFFF, ph->p_memsz, ph->p_offset); //将各段读入内存中
       }                                                            
                                                                    
       // call the entry point from the ELF header                  
       // note: does not return                                     
       ((void (*)(void))(ELFHDR->e_entry & 0xFFFFFF))(); //查询内核代码的入口地址，并通过函数调用的方式跳转到该地址上
                                                                    
   bad:                                                             
       outw(0x8A00, 0x8A00);                                        
       outw(0x8A00, 0x8E00);                                        
                                                                    
       /* do nothing */                                             
       while (1);
  }
  ~~~

## FAQ

- Q：Makefile的第一目标(goal)怎么确定？

​       A：通常make后不接Makefile中的目标，此时当变量.DEFAULT_GOAL为空,则第一个目标就是第一目标(goal)，反之则由该变量的值确定第一目标；若make后接了一个目标(如clean)，则该目标为Makefile的第一目标；

- Q：make debug后PC值为0xfff0而不是0xffff0
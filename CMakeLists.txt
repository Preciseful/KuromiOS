cmake_minimum_required(VERSION 3.28)
project(KuromiOS C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_COMPILER /home/developful/opt/cross/bin/i686-elf-gcc)

add_compile_definitions(__is_libk)

include_directories(/home/developful/opt/cross/lib/gcc/i686-elf/11.4.0/include
        kernel/include
        libc/include)

add_executable(KuromiOS
        kernel/arch/i386/tty.c
        kernel/include/kernel/vga.h
        kernel/include/kernel/tty.h
        kernel/kernel/kernel.c
        libc/include/sys/cdefs.h
        libc/include/stdio.h
        libc/include/stdlib.h
        libc/include/string.h
        libc/stdio/printf.c
        libc/stdio/putchar.c
        libc/stdio/puts.c
        libc/stdlib/abort.c
        libc/string/memcmp.c
        libc/string/memcpy.c
        libc/string/memmove.c
        libc/string/memset.c
        libc/string/strlen.c
        libc/include/sys/io.h
        kernel/include/kernel/debug.h
        libc/include/stdint.h
        kernel/arch/i386/mm/vmm.c
        kernel/include/kernel/vmm.h
        libc/stdlib/malloc.c
        kernel/arch/i386/mm/pmm.c
        kernel/include/kernel/pmm.h
        libc/stdlib/free.c
        kernel/arch/i386/mm/heap.c
        kernel/include/kernel/heap.h
        kernel/arch/i386/gdt.c
        kernel/include/kernel/gdt.h
        kernel/arch/i386/idt.c
        kernel/include/kernel/idt.h
)
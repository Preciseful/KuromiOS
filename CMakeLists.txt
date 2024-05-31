cmake_minimum_required(VERSION 3.28)
project(KuromiOS C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_COMPILER "/home/developful/opt/cross/bin/i686-elf-gcc-11.4.0")

include_directories(sysroot/usr/include)

add_executable(KuromiOS
        kernel/arch/i386/tty.c
        kernel/arch/i386/vga.h
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
        libc/string/strlen.c)
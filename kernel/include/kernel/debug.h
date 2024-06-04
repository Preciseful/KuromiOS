//
// Created by developful on 01.06.2024.
//

#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/vga.h>

#define ERROR(x) terminal_setcolor(vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK)); \
                 printf("[!] " x); \
                 terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK)); \
                 return

#define AND_RETURN(x) x;

#define SUCCESS(x) terminal_setcolor(vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK)); \
                   printf("[*] " x); \
                   terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK))

#define WARN(x) terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLACK)); \
                printf("[?] " x); \
                terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK))

#endif //DEBUG_H

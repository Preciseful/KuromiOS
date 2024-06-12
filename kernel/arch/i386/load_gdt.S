.section .text

.global     setGdt
.type       setGdt, @function
setGdt:
  mov 4(%esp), %ax
  mov %ax, gdtr
  mov 8(%esp), %eax
  mov %eax, gdtr+2
  lgdt gdtr
  ret

.global reloadSegments
.type reloadSegments, @function
reloadSegments:
    ljmpl $0x08, $reload_CS
reload_CS:
    mov $0x10, %ax          # 0x10 is a stand-in for your data segment
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ret
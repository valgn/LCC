.global _start
.text
_start:
    mov r7, #1      @ sys_exit
    mov r0, #0
    svc #0
    
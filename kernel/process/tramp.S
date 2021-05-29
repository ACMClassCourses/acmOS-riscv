# 这个文件中你可以设置一些处理用户态过来的中断处理二进制程序
# 如果你遵循两阶段处理
# 那么第一个处理流程是保存上下文，调转到中断处理函数
# 第二个人处理流程是还原上下文
# 但是你也可以有其他设计
# 在kernel.ld中指定了相关的位置

.section trampsec
.globl trampoline
trampoline:
.align 4
.globl usertrap1
usertrap1:

.globl usertrap2
usertrap2:
    sret
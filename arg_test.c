#include <stdio.h>

long args[6];

void arg_test(int a, int b, int c, int d, int e, int f, /* using stack */ int g, int h)
{
    
    asm volatile("movq %%rdi, %0\n": "=r"(args[0]));
    asm volatile("movq %%rsi, %0\n": "=r"(args[1]));
    asm volatile("movq %%rdx, %0\n": "=r"(args[2]));
    asm volatile("movq %%rcx, %0\n": "=r"(args[3]));
    asm volatile("movq %%r8, %0\n": "=r"(args[4]));
    asm volatile("movq %%r9, %0\n": "=r"(args[5]));
    // 获取栈上的参数
    // 8(%%rbp) 保存了栈帧
    asm volatile("movq 16(%%rbp), %0\n": "=r"(args[6]));
    asm volatile("movq 24(%%rbp), %0\n": "=r"(args[7]));

    // 打印得到的参数
    for (int i = 0; i < 8; i++) {
        printf("arg%d: %ld\n", i, args[i]);
    }
}

int main()
{
    arg_test(1, 2, 3, 4, 5, 6, 7, 8);
    return 0;
}
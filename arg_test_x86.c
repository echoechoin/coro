#include <stdio.h>

int args[6];

void arg_test(int a, int b, int c, int d, int e, int f, int g, int h)
{
    // 获取栈上的参数
    // 4(%%ebp) 保存了栈帧
    asm volatile("movl 8(%%ebp), %0\n": "=r"(args[0]));
    asm volatile("movl 12(%%ebp), %0\n": "=r"(args[1]));
    asm volatile("movl 16(%%ebp), %0\n": "=r"(args[2]));
    asm volatile("movl 20(%%ebp), %0\n": "=r"(args[3]));
    asm volatile("movl 24(%%ebp), %0\n": "=r"(args[4]));
    asm volatile("movl 28(%%ebp), %0\n": "=r"(args[5]));
    asm volatile("movl 32(%%ebp), %0\n": "=r"(args[6]));
    asm volatile("movl 36(%%ebp), %0\n": "=r"(args[7]));

    // 打印得到的参数
    for (int i = 0; i < 8; i++) {
        printf("arg%d: %d\n", i, args[i]);
    }
}
__attribute__((fastcall)) void arg_test_fast_call(int a, int b, int c, int d, int e, int f, int g, int h)
{
    asm volatile("movl %%edx, %0\n": "=r"(args[1]));
    asm volatile("movl %%ecx, %0\n": "=r"(args[0])); 

    // 获取栈上的参数
    // 4(%%ebp) 保存了栈帧
    asm volatile("movl 8(%%ebp), %0\n": "=r"(args[2]));
    asm volatile("movl 12(%%ebp), %0\n": "=r"(args[3]));
    asm volatile("movl 16(%%ebp), %0\n": "=r"(args[4]));
    asm volatile("movl 20(%%ebp), %0\n": "=r"(args[5]));
    asm volatile("movl 24(%%ebp), %0\n": "=r"(args[6]));
    asm volatile("movl 28(%%ebp), %0\n": "=r"(args[7]));

    // 打印得到的参数
    for (int i = 0; i < 8; i++) {
        printf("arg%d: %d\n", i, args[i]);
    }
}


int main()
{
    arg_test_fast_call(1, 2, 3, 4, 5, 6, 7, 8);
    return 0;
}
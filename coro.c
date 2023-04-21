// none symmetric coroutines example.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "list.h"

#define PAGE_ALIGNEMENT ~0xfffL
#define PAGE_SIZE        0x1000
#define MAGIC            0xdeadbeefdeadbeefL
#define LOG(fmt, ...)    printf(fmt, ##__VA_ARGS__)

typedef void *(*task_func_t)(void *);

typedef struct task_t {
    void *sp;
    list_node_t node;
    unsigned long magic;
} task_t;

typedef struct task_frame_t {
    void *rbx;
    void *rsi;
    void *rdi;
    void *rbp;        // rbp for stack frame
    task_func_t func; // rip for ret instruction
} task_frame_t;

list_node_t task_list = {&task_list, &task_list};
unsigned int task_count;

int coro_new(task_func_t func, void *stack)
{
    if (!stack) posix_memalign(&stack, PAGE_SIZE, PAGE_SIZE);
    if (!stack) {
        LOG("Failed to allocate stack.\n");
        return -1;
    }
    if ((unsigned long)stack & PAGE_ALIGNEMENT != (unsigned long)stack) {
        LOG("Stack is not page aligned.\n");
        return -1;
    }
    task_t *task = (task_t *)stack;
    task_frame_t *frame = (task_frame_t *)((char *)stack + PAGE_SIZE - sizeof(task_frame_t));
    frame->func = func;
    task->sp = frame;
    task->magic = MAGIC;
    list_add_tail(&task->node, &task_list);
    task_count++;
    return 0;
}

void coro_switch(task_t *next)
{
    // save current task's context
    asm volatile(
        "pushq %rdi\n"
        "pushq %rsi\n"
        "pushq %rbx\n"
    );
    // get current task's base stack address
    asm volatile(
        "movq %rsp, %rax\n"
        "andq $0xfffffffffffff000, %rax\n"
    );
    
    // save current task's stack pointer to base stack address
    asm volatile(
        "movq %rsp, (%rax)\n"
    );

    // load next task's stack pointer from %rdi
    asm volatile(
        "movq (%rdi), %rsp\n"
    );

    // restore next task's context
    asm volatile(
        "popq %rbx\n"
        "popq %rsi\n"
        "popq %rdi\n"
    );

    // return to next task's function
    return;
}

void coro_yield()
{
    if (list_empty(&task_list)) return;
    task_t *next = list_entry(task_list.next, task_t, node);
    assert(next->magic == MAGIC);
    list_del(&next->node);
    list_add_tail(&next->node, &task_list);
    coro_switch(next);
}

void coro_exit()
{
    char dummy = 0;
    assert(!list_empty(&task_list));
    task_t *curr = (task_t *)((unsigned long)&dummy & PAGE_ALIGNEMENT);
    list_del(&curr->node);
    free(curr);
    task_count--;
    if (task_count == 0) {
        LOG("All tasks are done.\n");
        exit(0);
    }
    coro_yield();
}

void *task_A(void *arg)
{
    int count = 2;
    while(count--)
    {
        LOG("task_A: 1\n");
        coro_yield();
        LOG("task_A: 2\n");
        coro_yield();
    }
    coro_exit();
}

void *task_B(void *arg)
{
    int count = 1;
    while(count--)
    {
        LOG("task_B: 1\n");
        coro_yield();
        LOG("task_B: 2\n");
        coro_yield();
    }
    coro_exit();
}

int main(int argc, char *argv[])
{
    coro_new(task_A, NULL);
    coro_new(task_B, NULL);
    coro_yield();
    return 0;
}

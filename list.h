#ifndef _LIST_H_
#define _LIST_H_

typedef struct list_node_t
{
	struct list_node_t *next;
	struct list_node_t *prev;
} list_node_t;

#define container_of(ptr, type, member) \
	((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

#define list_init(head) \
	do { \
		(head)->next = (head); \
		(head)->prev = (head); \
	} while (0)

#define list_add(node, head) \
	do { \
		(node)->next = (head)->next; \
		(node)->prev = (head); \
		(head)->next->prev = (node); \
		(head)->next = (node); \
	} while (0)

#define list_add_tail(node, head) \
	do { \
		(node)->next = (head); \
		(node)->prev = (head)->prev; \
		(head)->prev->next = (node); \
		(head)->prev = (node); \
	} while (0)

#define list_del(node) \
	do { \
		(node)->next->prev = (node)->prev; \
		(node)->prev->next = (node)->next; \
	} while (0)

#define list_empty(head) \
	((head)->next == (head))

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

#endif


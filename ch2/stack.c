#include <stddef.h>
#include <stdint.h>
#include "assert.h"
#include "mem.h"
#include "stack.h"

#define T Stack_T
#define MAGIC_VALUE 0x00008123
#define isBadPtr(p) ((p) == NULL || ((uintptr_t)((void *)(p))) % 8 != 0)

struct T {
	unsigned count;
	struct elem {
		void *x;
		struct elem *link;
	} *head;
	unsigned magic;
};

T Stack_new(void) {
	T stk;

	NEW(stk);
	stk->count = 0;
	stk->head = NULL;
	stk->magic = MAGIC_VALUE;
	return stk;
}

int Stack_empty(T stk) {
	assert(!isBadPtr(stk));
	return stk->count == 0;
}

void Stack_push(T stk, void *x) {
	struct elem *t;

	assert(!isBadPtr(stk));
	NEW(t);
	t->x = x;
	t->link = stk->head;
	stk->head = t;
	stk->count++;
}

void *Stack_pop(T stk) {
	void *x;
	struct elem *t;

	assert(!isBadPtr(stk));
	assert(stk->count > 0);
	t = stk->head;
	stk->head = t->link;
	stk->count--;
	x = t->x;
	FREE(t);
	return x;
}

void Stack_free(T *stk) {
	struct elem *t, *u;

	assert(!isBadPtr(stk));
	assert(!isBadPtr(*stk));
	for (t = (*stk)->head; t; t = u) {
		u = t->link;
		FREE(t);
	}
	FREE(*stk);
}

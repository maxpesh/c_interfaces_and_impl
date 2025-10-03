#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <stdint.h>
#include "stack.h"
#include "assert.h"
#include "mem.h"

#define T Stack_T
#define isPtrAligned(ptr, byte_count) (((uintptr_t)(ptr)) % (byte_count))
#define isBadPtr(ptr) ((ptr == NULL) || (isPtrAligned(ptr, 8)))

struct T {
	int count;
	struct elem {
		void *x;
		struct elem *link;
	} *head;
	long canary;
};

static long saved_canary;

T Stack_new(void) {
	T stk;

	NEW(stk);
	stk->count = 0;
	stk->head = NULL;
	if (saved_canary == 0) {
		srandom((unsigned)time(NULL));
		saved_canary = random();
	}
	stk->canary = saved_canary;
	return stk;
}

int Stack_empty(T stk) {
	assert(!isBadPtr(stk));
	assert(stk->canary == saved_canary);
	return stk->count == 0;
}

void Stack_push(T stk, void *x) {
	struct elem *t;

	assert(!isBadPtr(stk));
	assert(stk->canary == saved_canary);
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
	assert(stk->canary == saved_canary);
	t = stk->head;
	stk->head = t->link;
	stk->count--;
	x = t->x;
	FREE(t);
	return x;
}

void Stack_free(T *stk) {
	struct elem *t, *u;

	assert(stk && *stk);
	assert((*stk)->canary == saved_canary);
	for (t = (*stk)->head; t; t = u) {
		u = t->link;
		FREE(t);
	}
	FREE(*stk);
}

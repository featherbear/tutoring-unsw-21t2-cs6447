#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

void* randomfunction(size_t s, void* c) {
	return NULL;
}

int main() {
	__malloc_hook = randomfunction;


	malloc(32);
}

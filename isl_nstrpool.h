#ifndef ISL_NAIVE_STRING_POOL_H_
#define ISL_NAIVE_STRING_POOL_H_

/*
 isl_nstrpool.h - v0.0.2 - public domain naive string interning library
                           no warranty implied; use at your own risk

 author: Ilya Kolbin (iskolbin@gmail.com)
 url: github.com/iskolbin/isl_nstrpool

 Supports only adding, which linear scans the whole list. While unuseful under
 common load, very simple in its core and appropriate for some easy scenarios.

 LICENSE

 See end of file for license information.
*/

#include <stdlib.h>
#include <string.h>

typedef struct {
	char **strings;
	int allocated;
	int used;
} islnsp_strpool;

int islnsp_add(islnsp_strpool *pool, const char *str);
const char *islnsp_get(islnsp_strpool *pool, int id);
void islnsp_release(islnsp_strpool *pool);

#endif

#ifdef ISL_NAIVE_STRING_POOL_IMPLEMENTATION

#ifndef ISL_NAIVE_STRING_POOL_ALREADY_IMPLEMENTED
#define ISL_NAIVE_STRING_POOL_ALREADY_IMPLEMENTED
#else
#error "ISL_NAIVE_STRING_POOL Already implemented"
#endif

int islnsp_add(islnsp_strpool *pool, const char *str) {
	char *copied_str = NULL;
	int next_string_id = pool->used;
	for (int i = 0; i < pool->used; i++) {
		if (strcmp(pool->strings[i], str) == 0) {
			return i;
		}
	}
	if (pool->allocated <= pool->used) {
		int new_allocated = pool->allocated ? 2*pool->allocated : 1;
		char **old_strings = pool->strings;
		char **new_strings = realloc(pool->strings, new_allocated * sizeof *pool->strings);
		if (new_strings == NULL) {
			pool->strings = old_strings;
			return -1;
		} else {
			pool->strings = new_strings;
			pool->allocated = new_allocated;
		}
	}
	copied_str = malloc(sizeof(char) * (strlen(str)+1));
	if (copied_str == NULL) {
		return -2;
	}
	strcpy(copied_str, str);
	pool->strings[next_string_id] = copied_str;
	pool->used++;
	return next_string_id;
}

const char *islnsp_get(islnsp_strpool *pool, int id) {
	return (id >= 0 && id < pool->used) ? (const char *) pool->strings[id] : NULL;
}

void islnsp_release(islnsp_strpool *pool) {
	if (pool->strings != NULL) {
		for (int i = 0; i < pool->used; i++) {
			free(pool->strings[i]);
		}
		free(pool->strings);
		pool->strings = NULL;
		pool->allocated = 0;
		pool->used = 0;
	}
}

#endif

/*
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2019 Ilya Kolbin
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/

/*
   Very Stupid Vectors
    Copyright (C) 2026  Anonymous

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

   NOTES:
     * VSV is an opaque pointer type and
       should not have its members accessed.
       This is done because member names are
       subject to change. Use functions to
       query it.
     * Any function prefixed with two
       underscores (__) is intended for
       private use within context of the
       library only.
     * Should support C89/90.
     * VSV->elem can't be zero for the
       majority of functions to succeed.
     * You can find other notes scattered
       around using commands like grep
       to find instances of the word "NOTE".
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "vsv.h"

/**
 * @brief Check if a VSV strucg is valid and expand its slots.
 * @param[in]  count Count of elements to allocate space for (optional, set 0 otherwise)
 * @param[out] self Pointer to VSV struct.
 */
int __vsv_check(size_t count, VSV* self)
{
	if (self != NULL)
	{
		if (count > 0 && count > self->allocd && self->elem > 0)
		{
			/* NOTE: Don't rely on ptr == NULL then malloc */
			void* newptr = NULL;
			if (self->ptr == NULL)
			{
				newptr = malloc(self->elem * count);
			}
			else
			{
				newptr = realloc(self->ptr, self->elem * count);
			}

			if (newptr == NULL)
			{
				fprintf(stderr, "Alloc failed: %s\n", strerror(errno));
				return 1;
			}

			if (self->ptr == NULL)
			{
				/* down here to only run if the malloc rpute succèeds */
				self->count = 0;
			}

			self->ptr = newptr;
			self->allocd = count;
		}
		/*else if (self->ptr == NULL)
		{
			   NOTE: safe then sorry: don't play pretwnd
                                 that there is more then there is
			self->count = 0;
			self->allocd = 0;

			fprintf(stderr, "self->ptr == NULL failure\n");
			return 1;
		}*/
	}
	else
	{
		return 1;
	}
	return 0;
}

int vsv_init(size_t elem, VSV* self)
{
	if (__vsv_check(0, self) != 0)
	{
		return 1;
	}

	self->count = 0;
	self->elem = elem;
	self->ptr = NULL;
	self->allocd = 0;
	return 0;
}
int vsv_free(VSV* self)
{
	if (__vsv_check(0, self) != 0)
	{
		return 1;
	}

	self->count = 0;
	self->elem = 0;
	if (self->ptr != NULL)
	{
		free(self->ptr);
	}
	self->ptr = NULL;
	self->allocd = 0;
	return 0;
}


/* push/pop/insert/erase - modifiers */
int vsv_assign(size_t n, void* x, VSV* self)
{
	size_t i;

	if (__vsv_check(n, self) != 0)
	{
		return 1;
	}
	if (n == 0)
	{
		self->count = 0;
		return 0;
	}
	if (self->ptr == NULL || self->elem <= 0 || x == NULL)
	{
		return 1;
	}

	for(i = 0; i < n; i++)
	{
		memcpy((void*)((unsigned char*)self->ptr + (i * self->elem)), x, self->elem);
	}

	self->count = n;
	return 0;
}

int vsv_clear(VSV* self)
{
	if (__vsv_check(0, self) != 0)
	{
		return 1;
	}

	self->count = 0;

	return 0;
}

int vsv_insert_n(size_t i, size_t n, const void* x, VSV* self)
{
	size_t j, origcount = vsv_size(self);

	if (n == 0)
	{
		return 0;
	}
	if (self == NULL)
	{
		return 1;
	}
	if (i > origcount || self->elem <= 0 || x == NULL)
	{
		return 1;
	}
	if (__vsv_check(origcount+n, self) != 0)
	{
		return 1;
	}
	if (self->ptr == NULL)
	{
		/* diarrhea */
		return 1;
	}

	if(origcount > 0)
	{
		for(j = origcount; j > i; j--)
		{
			memcpy((void*)((unsigned char*)self->ptr + ((j-1 + n) * self->elem)), (void*)((unsigned char*)self->ptr + ((j-1) * self->elem)), self->elem);
		}
	}

	for(j = 0; j < n; j++)
	{
		memcpy((void*)((unsigned char*)self->ptr + ((i + j) * self->elem)), x, self->elem);
	}

	self->count += n;

	return 0;
}
int vsv_insert(size_t i, const void* x, VSV* self)
{
	int insret;

	insret = vsv_insert_n(i, 1, x, self);

	return insret;
}
int vsv_erase_n(size_t i, size_t n, VSV* self)
{
	size_t j, origcount = vsv_size(self);

	if (__vsv_check(0, self) != 0)
	{
		return 1;
	}
	if (n == 0)
	{
		return 0;
	}
	if (self->ptr == NULL || i >= origcount || self->elem <= 0)
	{
		return 1;
	}

	if(origcount > 0 && n < (origcount - i))
	{
		for(j = i + n; j < origcount; j++)
		{
			memcpy((void*)((unsigned char*)self->ptr + ((j-n) * self->elem)), (void*)((unsigned char*)self->ptr + ((j) * self->elem)), self->elem);
		}
	}

	if(n > origcount - i)
	{
		self->count -= origcount - i;
	}
	else
	{
		self->count -= n;
	}

	return 0;
}
int vsv_erase(size_t i, VSV* self)
{
	int ersret;

	ersret = vsv_erase_n(i, 1, self);

	return ersret;
}

int vsv_push_back(const void* x, VSV* self)
{
	if (self == NULL)
	{
		return 1;
	}
	if (self->elem <= 0 || x == NULL)
	{
		return 1;
	}
	if (__vsv_check(self->count+1, self) != 0)
	{
		return 1;
	}
	if (self->ptr == NULL)
	{
		/* poo */
		return 1;
	}

	memcpy((void*)((unsigned char*)self->ptr + (self->count * self->elem)), x, self->elem);
	self->count++;

	return 0;
}
int vsv_pop_back(VSV* self)
{
	if (__vsv_check(0, self) != 0)
	{
		return 1;
	}

	/* NOTE: No clearing is required,
	         unless you want to be polite.
	*/
	/* memset((void*)((unsigned char*)self->ptr + (self->count * self->elem)), 0, self->elem); */
	if(self->count > 0)
	{
		self->count--;
	}

	return 0;
}
int vsv_resize_from(size_t n, void* x, VSV* self)
{
	size_t size, i;

	if (self == NULL)
	{
		return 1;
	}
	if (n == 0)
	{
		self->count = 0;
		return 0;
	}
	if (self->elem <= 0)
	{
		return 1;
	}

	size = self->count;
	if (__vsv_check(n, self) != 0)
	{
		return 1;
	}
	if (self->ptr == NULL)
	{
		/* poo */
		return 1;
	}


	self->count = n;
	for(i = size; i < n; i++)
	{
		if (x == NULL)
		{
			memset((void*)((unsigned char*)self->ptr + (i * self->elem)), 0, self->elem);
		}
		else
		{
			memcpy((void*)((unsigned char*)self->ptr + (i * self->elem)), x, self->elem);
		}
	}


	return 0;
}
int vsv_resize(size_t n, VSV* self)
{
	int resret;

	resret = vsv_resize_from(n, NULL, self);

	return resret;
}

/* access functions */
void* vsv_at(size_t i, VSV* self)
{
	if (__vsv_check(0, self) != 0)
	{
		return NULL;
	}
	if (self->ptr == NULL || self->elem <= 0 || i >= self->count)
	{
		return NULL;
	}

	return (void*)((unsigned char*)self->ptr + (i * self->elem));
}
void* vsv_front(VSV* self)
{
	void* atret;

	atret = vsv_at(0, self);

	return atret;
}
void* vsv_back(VSV* self)
{
	void* atret;
	size_t size;

	if (__vsv_check(0, self) != 0)
	{
		return NULL;
	}

	atret = NULL;
	size = self->count;
	if (size > 0)
	{
		atret = vsv_at(size-1, self);
	}

	return atret;
}
void* vsv_data(const VSV* self)
{
	if (self == NULL)
	{
		return NULL;
	}

	/* NOTE: Not much protection on this function...
	         but users of it would know what they're doing, right?
	         RIGHT?
	 */

	return self->ptr;
}

/* capacity functions */
size_t vsv_size(const VSV* self)
{
	if (self == NULL)
	{
		return 0;
	}

	return self->count;
}
size_t vsv_capacity(const VSV* self)
{
	if (self == NULL)
	{
		return 0;
	}

	return self->allocd;
}
int vsv_empty(const VSV* self)
{
	if (self == NULL)
	{
		return 1;
	}

	return (int)(self->count == 0);
}
int vsv_reserve(size_t n, VSV* self)
{
	if(n <= 0) {
		/* nothing to allocate, user is just playing with us */
		return 0;
	}
	/* __vsv_check allocs for us */
	if (__vsv_check(n, self) != 0)
	{
		return 1;
	}

	return 0;
}
int vsv_shrink_to_fit(VSV* self)
{
	void* newptr = NULL;

	if (__vsv_check(0, self) != 0)
	{
		return 1;
	}
	if (self->elem <= 0)
	{
		return 1;
	}

	if (self->count > 0)
	{
		if (self->ptr == NULL)
		{
			newptr = malloc(self->elem * self->count);
		}
		else
		{
			newptr = realloc(self->ptr, self->elem * self->count);
		}
		if (newptr == NULL)
		{
			fprintf(stderr, "Realloc error: %s\n", strerror(errno));
			return 1;
		}
	}
	else if (self->ptr != NULL)
	{
		free(self->ptr);
	}

	self->ptr = newptr;
	self->allocd = self->count;

	return 0;
}

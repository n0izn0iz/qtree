#include "array.h"

#include <assert.h>
#include <stdlib.h>

t_array*				array_create(unsigned int cap)
{
	t_array* array;

	array = (t_array*)malloc(sizeof(t_array));
	if (array)
	{
		array->size = 0;
		array->cap = cap;
		array->data = malloc(sizeof(void*) * cap);
		if (!array->data)
		{
			free(array);
			array = NULL;
		}
	}
	return (array);
}

void					array_destroy(t_array* array)
{
	if (array != NULL)
	{
		free(array->data);
		free(array);
	}
}

void					array_append(t_array* array, void* value)
{
	assert(array != NULL);
	if (array->size >= array->cap)
	{
		array->cap *= 2;
		array->data = realloc(array->data, sizeof(void*) * array->cap);
		assert(array->data);
	}
	array->data[array->size] = value;
	array->size++;
}

void		array_merge(t_array* dest, t_array* src)
{
	unsigned int	i;

	assert(src != NULL);
	assert(dest != NULL);
	i = 0;
	while (i < src->size)
	{
		array_append(dest, src->data[i]);
		i++;
	}
	array_destroy(src);
}

void*					array_get(t_array* array, unsigned int index)
{
	assert(index < array->size);
	return (array->data[index]);
}

void					array_set(t_array* array, unsigned int index, void* value)
{
	assert(index < array->size);
	array->data[index] = value;
}

void					array_swap(t_array* array, unsigned int i1, unsigned int i2)
{
	void*	tmptr;

	assert(i1 < array->size);
	assert(i2 < array->size);
	tmptr = array->data[i1];
	array->data[i1] = array->data[i2];
	array->data[i2] = tmptr;
}

void*					array_remove(t_array* array, unsigned int index)
{
	void* tmp;

	assert(array->size > 0);
	tmp = array_get(array, index);
	array->size -= 1;
	array_set(array, index, array_get(array, array->size));
	return (tmp);
}

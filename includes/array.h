#ifndef ARRAY_H
# define ARRAY_H

typedef struct	s_array
{
	unsigned int	size;
	unsigned int	cap;
	void**			data;
}				t_array;

t_array*	array_create(unsigned int cap);
void		array_destroy(t_array* array);
void		array_append(t_array* array, void* value);
void		array_merge(t_array* dest, t_array* src);
void*		array_get(t_array* array, unsigned int index);
void		array_set(t_array* array, unsigned int index, void* value);
void		array_swap(t_array* array, unsigned int i1, unsigned int i2);
void*		array_remove(t_array* array, unsigned int index);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   vec_lib2.c                                          :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/29 10:57:42 by adecheri       #+#    #+#                */
/*   Updated: 2025/06/29 10:57:44 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	ft_safefree(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
	return ;
}

void	ft_free_arr(void **array)
{
	int	i;

	if (array)
	{
		i = -1;
		while (array[++i])
			ft_safefree(array[i]);
		ft_safefree(array);
	}
}


int	element_delete(element *e, int index)
{
	int	i;
	int	status;

	i = 0;
	status = -1;
	if (e)
	{
		if ((index < 0) || ((size_t)index >= e->element_list.total))
			return (-1);
		e->element_list.tokens[index] = NULL;
		i = index;
		while (i < e->element_list.total - 1)
		{
			e->element_list.tokens[i] = e->element_list.tokens[i + 1];
			e->element_list.tokens[i + 1] = NULL;
			i++;
		}
		e->element_list.total--;
		if ((e->element_list.total > 0)
			&& ((e->element_list.total) == (e->element_list.size / 4)))
			element_resize(e, e->element_list.size, e->element_list.size / 2);
		status = 0;
	}
	return (status);
}

int	element_free(element *e)
{
	if (e)
	{
		ft_free_arr(e->element_list.tokens);
		e->element_list.total = 0;
		e->element_list.tokens = NULL;
		return (0);
	}
	return (-1);
}

void	element_init(element *e, int size)
{
	e->pf_element_total = element_total;
	e->pf_element_resize = element_resize;
	e->pf_element_add = element_push_back;
	e->pf_element_set = element_set;
	e->pf_element_get = element_get;
	e->pf_element_free = element_free;
	e->pf_element_delete = element_delete;
	e->element_list.size = size;
	e->element_list.total = 0;
	e->element_list.tokens = malloc(sizeof(void *) * e->element_list.size);
	if (!e->element_list.tokens)
	{
		fprintf(stderr, "Memory alloc failed in init tokenlist\n");
		exit(EXIT_FAILURE);
	}
}

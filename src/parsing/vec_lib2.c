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


int	elementDelete(element *e, int index)
{
	int	i;
	int	status;

	i = 0;
	status = -1;
	if (e)
	{
		if ((index < 0) || ((size_t)index >= e->elementList.total))
			return (-1);
		e->elementList.tokens[index] = NULL;
		i = index;
		while (i < e->elementList.total - 1)
		{
			e->elementList.tokens[i] = e->elementList.tokens[i + 1];
			e->elementList.tokens[i + 1] = NULL;
			i++;
		}
		e->elementList.total--;
		if ((e->elementList.total > 0)
			&& ((e->elementList.total) == (e->elementList.size / 4)))
			elementResize(e, e->elementList.size, e->elementList.size / 2);
		status = 0;
	}
	return (status);
}

int	elementFree(element *e)
{
	if (e)
	{
		ft_free_arr(e->elementList.tokens);
		e->elementList.total = 0;
		e->elementList.tokens = NULL;
		return (0);
	}
	return (-1);
}

void	element_init(element *e, int size)
{
	e->pfElementTotal = elementTotal;
	e->pfElementResize = elementResize;
	e->pfElementAdd = elementPushBack;
	e->pfElementSet = elementSet;
	e->pfElementGet = elementGet;
	e->pfElementFree = elementFree;
	e->pfElementDelete = elementDelete;
	e->elementList.size = size;
	e->elementList.total = 0;
	e->elementList.tokens = malloc(sizeof(void *) * e->elementList.size);
	if (!e->elementList.tokens)
	{
		fprintf(stderr, "Memory alloc failed in init tokenlist\n");
		exit(EXIT_FAILURE);
	}
}

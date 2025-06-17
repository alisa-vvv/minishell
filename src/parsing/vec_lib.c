/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   vec_lib.c                                           :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/07 16:07:40 by adecheri       #+#    #+#                */
/*   Updated: 2025/06/07 16:07:44 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// not tested yet, also have to check if
void	**ft_realloc(void *ptr, unsigned int oldsize, unsigned int newsize)
{
	int		i;
	void	*new_ptr;

	if (!ptr)
		return (write(2, "Memory Allocation Failed\n", 25));
	new_ptr = ptr + oldsize;
	i = newsize - oldsize;
	while (i <= newsize)
	{
		if (new_ptr + i != NULL)
		{
			new_ptr += i + 1;
			i = -1;
		}
		i++;
	}
	if (new_ptr == (ptr + oldsize))
	{
		new_ptr = malloc(newsize - oldsize);
		return (free(new_ptr), &ptr);
	}
	new_ptr = malloc(newsize);
	ft_memmove(new_ptr, ptr, oldsize);
	return (free(ptr), &new_ptr);
}

int	elementTotal(element *e)
{
	if (e)
		return (e->elementList.total);
	return (-1);
}

int	elementResize(element *e, int oldsize, int newsize)
{
	void	**tokens;

	if (e)
	{
		tokens = ft_realloc(e->elementList.tokens, sizeof(void *) * oldsize, sizeof(void *) * newsize);
		if (tokens)
		{
			e->elementList.tokens = tokens;
			e->elementList.size = newsize;
			return (0);
		}
	}
	return (-1);
}

int	elementPushBack(element *e, void *token)
{
	int	status;

	status = -1;
	if (e)
	{
		if (e->elementList.size == e->elementList.total)
		{
			status = elementResize(e, e->elementList.size, e->elementList.size * 2);
			if (status == -1)
				e->elementList.tokens[e->elementList.total++] = token;
		}
		else
		{
			e->elementList.tokens[e->elementList.total++] = token;
			status = 0;
		}
	}
	return (status);
}

int	elementSet(element *e, int index, void *token)
{
	if (e)
	{
		if ((index >= 0) && (index < e->elementList.total))
		{
			e->elementList.tokens[index] = token;
			return (0);
		}
	}
	return (-1);
}

void	*elementGet(element *e, int index)
{
	void	*token_data;

	token_data = NULL;
	if (e)
	{
		if ((index >= 0) && ((size_t)index < e->elementList.total))
			token_data = e->elementList.tokens[index];
	}
	return (token_data);
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
			elementResize(e, e->elementList.size / 2);
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
		fprintf(stderr, "Memory allocation failed in vector_init\n");
		exit(EXIT_FAILURE);
	}
}

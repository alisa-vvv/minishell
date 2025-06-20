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

// realloc for shrink and expand size 
void	*ft_realloc(void *ptr, unsigned int oldsize, unsigned int newsize)
{
	int		i;
	unsigned int copy_size;
	void *n_ptr;

	if (newsize == 0)
		return(ft_safe_free(n_ptr), NULL);
	if (oldsize > newsize)
		copy_size = oldsize;
	else 
		copy_size = newsize;
	while (ptr && i < copy_size)
	{
		((char *)n_ptr)[i] = ((char *)ptr)[i];
		i++;
	}
	while (i < newsize)
	{
		((char *)ptr)[i] = '\0';
		i++;
	}
	return (free(ptr), n_ptr);
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
		fprintf(stderr, "Memory allocation failed in vector_init\n");
		exit(EXIT_FAILURE);
	}
}

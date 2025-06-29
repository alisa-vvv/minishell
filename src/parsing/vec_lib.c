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


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
#include "vec_lib.h"
#include <string.h>

// realloc for shrink and expand size
void	*ft_realloc(void *ptr, unsigned int oldsize, unsigned int newsize)
{
	unsigned int    copy_size;
	void            *n_ptr;

	if (newsize == 0)
	{
		ft_safe_free(ptr);
		return (NULL);
	}
	n_ptr = malloc(newsize);
	if (!n_ptr)
		return (NULL);
	if (oldsize < newsize)
		copy_size = oldsize;
	else 
		copy_size = newsize;
	if (ptr && copy_size)
		memcpy(n_ptr, ptr, copy_size);
	if (newsize > copy_size)
		memset((char *)n_ptr + copy_size, 0, newsize - copy_size);
	ft_safe_free(ptr);
	return (n_ptr);
}

int	element_resize(element *e, int oldsize, int newsize)
{
	void	**tokens;

	if (e)
	{
		tokens = ft_realloc(e->element_list.tokens, sizeof(void *) * oldsize,
				sizeof(void *) * newsize);
		if (tokens)
		{
			e->element_list.tokens = tokens;
			e->element_list.size = newsize;
			return (0);
		}
	}
	return (-1);
}

int	element_push_back(element *e, void *token)
{
	int	status;

	status = -1;
	if (e)
	{
		if (e->element_list.size == e->element_list.total)
		{
			status = element_resize(e, e->element_list.size, e->element_list.size
					* 2);
			if (status != 0)
				return (-1);
			e->element_list.tokens[e->element_list.total++] = token;
			status = 0;
		}
		else
		{
			e->element_list.tokens[e->element_list.total++] = token;
			status = 0;
		}
	}
	return (status);
}

int	element_set(element *e, size_t index, void *token)
{
	if (e)
	{
		if ((index >= 0) && (index < e->element_list.total))
		{
			e->element_list.tokens[index] = token;
			return (0);
		}
	}
	return (-1);
}

void	*element_get(element *e, size_t index)
{
	void	*token_data;

	token_data = NULL;
	if (e)
	{
		if ((index >= 0) && (index < e->element_list.total))
			token_data = e->element_list.tokens[index];
	}
	return (token_data);
}

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

// realloc for shrink and expand size
void	*ft_realloc(void *ptr, unsigned int oldsize, unsigned int newsize)
{
	int				i;
	unsigned int	copy_size;
	void			*n_ptr;

	if (newsize == 0)
		return (ft_safefree(n_ptr), NULL);
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
			if (status == -1)
				e->element_list.tokens[e->element_list.total++] = token;
		}
		else
		{
			e->element_list.tokens[e->element_list.total++] = token;
			status = 0;
		}
	}
	return (status);
}

int	element_set(element *e, int index, void *token)
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

void	*element_get(element *e, int index)
{
	void	*token_data;

	token_data = NULL;
	if (e)
	{
		if ((index >= 0) && ((size_t)index < e->element_list.total))
			token_data = e->element_list.tokens[index];
	}
	return (token_data);
}

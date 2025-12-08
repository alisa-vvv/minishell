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
	unsigned int	copy_size;
	void			*n_ptr;

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

int	tokenlist_resize(t_tokenlist *e, int oldsize, int newsize)
{
	void	**tokens;

	if (e)
	{
		tokens = ft_realloc(e->tokens, sizeof(void *) * oldsize, sizeof(void *)
				* newsize);
		if (tokens)
		{
			e->tokens = tokens;
			e->size = newsize;
			return (0);
		}
	}
	return (-1);
}

// p_printf("RESIZE?\n");
int	tokenlist_add(t_tokenlist *e, void *token)
{
	int	status;

	status = -1;
	if (e)
	{
		if (e->size == e->total)
		{
			status = tokenlist_resize(e, e->size, e->size * 2);
			if (status != 0)
				return (-1);
			e->tokens[e->total++] = token;
			status = 0;
		}
		else
		{
			e->tokens[e->total++] = token;
			status = 0;
		}
	}
	return (status);
}

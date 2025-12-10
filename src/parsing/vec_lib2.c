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

void	ft_free_arr(void **array)
{
	int	i;

	if (array)
	{
		i = -1;
		while (array[++i])
			ft_safe_free((unsigned char **)&array[i]);
		ft_safe_free((unsigned char **)array);
	}
}

//p_printf("STRING = $%s$\n", (*token)->value);
int	ft_free_s_token(void **check_token)
{
	t_token	**token;

	token = (t_token **)check_token;
	if (!token || !(*token))
		return (1);
	if ((*token))
	{
		if ((*token)->value)
			ft_safe_free((unsigned char **)&(*token)->value);
		(*token)->command = 0;
		(*token)->type = 0;
		(*token)->pos = 0;
	}
	ft_safe_free((unsigned char **)check_token);
	return (0);
}


//better implementation
int	tokenlist_delete(t_tokenlist *e, size_t index)
{
	t_token	*check_token;
	int		err;

	err = 0;
	if (e)
	{
		if ((index < 0) || ((size_t)index >= e->total))
			return (1);
		if (e->tokens[index] != NULL)
			ft_free_s_token((void **)&e->tokens[index]);
		while (index < e->total - 1)
		{
			e->tokens[index] = e->tokens[index + 1];
			check_token = e->tokens[index];
			check_token->pos--;
			index++;
		}
		e->tokens[index] = NULL;
		e->total--;
		// if ((e->total > 0) && ((e->total) == (e->size / 4)))
		// 	err = tokenlist_resize(e, e->size, e->size / 2);
	}
	return (err);
}

// p_printf("FREE THIS: %p %p\n", e->tokens[i],
//((t_token*)e->tokens[i])->value);
void	tokenlist_free(t_tokenlist *e)
{
	size_t	i;

	i = 0;
	if (e)
	{
		while (e->tokens[i])
		{
			ft_free_s_token((void **)&e->tokens[i]);
			i++;
		}
		e->total = 0;
		ft_safe_free((unsigned char **)&e->tokens);
		free(e);
	}
}

//fprintf(stderr, "Memory alloc failed in init tokenlist\n");
int	tokenlist_init(t_tokenlist **e, int size)
{
	*e = ft_calloc(1, sizeof(t_tokenlist));
	if (!(*e))
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	(*e)->size = size;
	if ((*e)->size <= 0) // i dunno what this is
	{
		(*e)->tokens = NULL;
		return (1);
	}
	(*e)->tokens = ft_calloc((size_t)(*e)->size + 1, sizeof(void *));
	if (!(*e)->tokens)
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	(*e)->tokens[size] = NULL;
	return (success);
}

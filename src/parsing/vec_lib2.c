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
		ft_safe_free((unsigned char**)array);
	}
}

int ft_free_s_token(void **check_token)
{
	t_token **token = (t_token**)check_token;

	if (!token || !(*token))
		return (write (2, "No Token Found\n", 15));
	if ((*token))
	{
		if ((*token)->value)
			ft_safe_free((unsigned char **)&(*token)->value);
		(*token)->command = 0;
		(*token)->pos = 0;
	}
	ft_safe_free((unsigned char **)token);
	
	// ft_safe_free((unsigned char **)&token[i]->value);
	// 		token[i]->command = 0;
	// 		token[i]->pos = 0;
	// 		token[i]->type = 0;
	// 	ft_safe_free((unsigned char **)&token[i]);
	return (0);
}


// if (index < e->total -1)
// 	ft_memmove(&e->tokens[index], &e->tokens[index+1], (e->total - index -1)(sizeof)t_token);
// check_token = e->tokens[index +1];
// if (check_token)
// 	p_printf("CURR TOKEN = %s\n", check_token->value); 
//better implementation
int	tokenlist_delete(t_tokenlist *e, size_t index)
{
	unsigned long	i;
	t_token *check_token;

	i = 0;
	if (e)
	{
		if ((index < 0) || ((size_t)index >= e->total))
			return (1);
		if (e->tokens[index] != NULL)
			ft_free_s_token((void **)&e->tokens[index]);
		while (index < e->total-1)
		{
			e->tokens[index] = e->tokens[index+1];
			check_token = e->tokens[index]; 
			check_token->pos--;
			index++;
		}
		e->total--;
		if ((e->total > 0)
			&& ((e->total) == (e->size / 4)))
			tokenlist_resize(e, e->size, e->size / 2);
	}
//	index_lexer(&e);
	return (0);
}

int	tokenlist_free(t_tokenlist *e)
{
	size_t i;
	i = 0;
	if (e)
	{
		//ft_free_tokens(e->tokens);
		while (e->tokens[i])
		{
			ft_free_s_token((void **)&e->tokens[i]);
			i++;
		}
		e->total = 0;
		ft_safe_free((unsigned char **)&e->tokens);
	//	e->tokens = NULL;
		return (0);
	}
	return (-1);
}

int	tokenlist_init(t_tokenlist **e, int size)
{
	*e = calloc(1, sizeof(t_tokenlist));
	if (!(*e))
		return (1);
	(*e)->size = size;
	if ((*e)->size <= 0)
	{
		(*e)->tokens = NULL;
		return (1);
	}
	(*e)->tokens = calloc((size_t)(*e)->size +1, sizeof(void *));
	if (!(*e)->tokens)
	{
		fprintf(stderr, "Memory alloc failed in init tokenlist\n");
		exit(EXIT_FAILURE);
	}
	(*e)->tokens[size] = NULL;
	return (0);
}

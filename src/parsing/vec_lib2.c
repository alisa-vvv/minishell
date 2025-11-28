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
	
	ft_safe_free((unsigned char **)&(*token)->value);
	(*token)->command = 0;
	(*token)->pos = 0;
	ft_safe_free((unsigned char **)token);

	// ft_safe_free((unsigned char **)&token[i]->value);
	// 		token[i]->command = 0;
	// 		token[i]->pos = 0;
	// 		token[i]->type = 0;
	// 	ft_safe_free((unsigned char **)&token[i]);
	return (0);
}

/*i = index;
		while (i < e->element_list.total - 1)
		{
			e->element_list.tokens[i] = e->element_list.tokens[i + 1];
			i++;
		}
		ft_free_s_token(e->element_list.tokens[e->element_list.total]);
		e->element_list.tokens[e->element_list.total - 1] = NULL;*/
//reverted back to old 
int	tokenlist_delete(t_tokenlist *e, size_t index)
{
	unsigned long	i;
	int	status;
	//t_token *check_token;

	i = 0;
	status = -1;
	if (e)
	{
		if ((index < 0) || ((size_t)index >= e->total))
			return (-1);
		if (e->tokens[index] != NULL)
			ft_free_s_token(&e->tokens[index]);
		// memmove();
		// check_token = e->tokenlist_list.tokens[index];
		// if (check_token)
		// {
		// 	ft_free_s_token(check_token);
		// 	ft_safe_free((unsigned char **)&check_token);
		// }
		i = index;
		while (i < e->total - 1)
		{
			e->tokens[i] = e->tokens[i + 1];
			e->tokens[i + 1] = NULL;
			i++;
		}
		// ft_free_s_token(e->tokens[e->total]);
		// e->tokens[e->total - 1] = NULL;
		e->total--;
		if ((e->total > 0)
			&& ((e->total) == (e->size / 4)))
			tokenlist_resize(e, e->size, e->size / 2);
		status = 0;
	}
	return (status);
}


int ft_free_tokens(void **tokens)
{
	int i;

	t_token **token;
	token = (t_token **)tokens; 
	i = 0;
	if (token)
	{
		while(token[i])
		{
			ft_free_s_token((void **)&token[i]);
			// ft_safe_free((unsigned char **)&token[i]->value);
			// token[i]->command = 0;
			// token[i]->pos = 0;
			// token[i]->type = 0;
			// ft_safe_free((unsigned char **)&token[i]);
			i++;
		}
		ft_safe_free((unsigned char **)&token);
	}
	return (0);
}

int	tokenlist_free(t_tokenlist *e)
{
	if (e)
	{
		ft_free_tokens(e->tokens);
		e->total = 0;
		e->tokens = NULL;
		return (0);
	}
	return (-1);
}

int	tokenlist_init(t_tokenlist **e, int size)
{
	// e->total = tokenlist_total;
	// e->pf_tokenlist_resize = tokenlist_resize;
	// e->pf_tokenlist_add = tokenlist_push_back;
	// e->pf_tokenlist_swap = tokenlist_swap;
	// e->pf_tokenlist_insert = tokenlist_insert;
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

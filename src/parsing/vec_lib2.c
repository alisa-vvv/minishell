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

int	element_delete(element *e, size_t index)
{
	unsigned long	i;
	int	status;

	i = 0;
	status = -1;
	if (e)
	{
		if ((index < 0) || ((size_t)index >= e->element_list.total))
			return (-1);
		e->element_list.tokens[index] = NULL;
		i = index;
		while (i < e->element_list.total - 1)
		{
			e->element_list.tokens[i] = e->element_list.tokens[i + 1];
			e->element_list.tokens[i + 1] = NULL;
			i++;
		}
		e->element_list.total--;
		if ((e->element_list.total > 0)
			&& ((e->element_list.total) == (e->element_list.size / 4)))
			element_resize(e, e->element_list.size, e->element_list.size / 2);
		status = 0;
	}
	return (status);
}

int ft_free_tokens(void **token, int count)
{
	int i;
	t_token **tokens;
	i = 0;

	tokens = (t_token **)token; 
	if (tokens)
	{
		while(i < count)
		{
			ft_safe_free((unsigned char **)&tokens[i]->value);
			i++;
		}
		//ft_safe_free((unsigned char **)&tokens);
	}
	return (0);
}

int	element_free(element *e, int count)
{
	if (e)
	{
		ft_free_tokens(e->element_list.tokens, count);
		ft_free_arr(e->element_list.tokens);
		e->element_list.total = 0;
		e->element_list.tokens = NULL;
		return (0);
	}
	return (-1);
}

void	element_init(element *e, int size)
{
	e->pf_element_total = element_total;
	e->pf_element_resize = element_resize;
	e->pf_element_add = element_push_back;
	// e->pf_element_swap = element_swap;
	// e->pf_element_insert = element_insert;
	e->pf_element_set = element_set;
	e->pf_element_get = element_get;
	e->pf_element_free = element_free;
	e->pf_element_delete = element_delete;
	e->element_list.size = size;
	e->element_list.total = 0;
	if (e->element_list.size <= 0)
	{
		e->element_list.tokens = NULL;
		return ;
	}
	e->element_list.tokens = calloc((size_t)e->element_list.size, sizeof(void *));
	if (!e->element_list.tokens)
	{
		fprintf(stderr, "Memory alloc failed in init tokenlist\n");
		exit(EXIT_FAILURE);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   parser_utils.c                                      :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/08/19 10:44:43 by adecheri       #+#    #+#                */
/*   Updated: 2025/08/19 10:44:45 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// func returning next token type in the array
t_token	*lookahead(
	element *tokenlist, 
	size_t index)
{
	t_token	*check_token;

	check_token = NULL;
	if (index + 1 < tokenlist->element_list.total)
		check_token = (t_token *)tokenlist->pf_element_get((tokenlist), index + 1);
	if (!check_token)
		return (NULL);
	return (check_token);
}

t_token	*lookbehind(
	element *tokenlist, 
	size_t index)
{
	t_token	*check_token;

	check_token = NULL;
	if (index > 0)
		check_token = (t_token *)tokenlist->pf_element_get((tokenlist), index -1);
	if (!check_token)
		return (NULL);
	return (check_token);
}


// func to traverse list and find a spec symbol
int	find_symbol(element *tokenlist, int pos, char symbol)
{
	t_token	*check_token;

	while ((size_t)pos < tokenlist->element_list.total)
	{
		check_token = (t_token *)tokenlist->element_list.tokens[pos];
		if (ft_strchr(check_token->value, symbol) != NULL)
			return (pos);
		pos++;
	}
	return (-1);
}
int	find_token_type(element *tokenlist, size_t pos, int pos_red, t_token_type type)
{
	t_token	*check_token;
	size_t total;
	total = 0;

	if (pos_red == -1)
		total = (size_t)tokenlist->element_list.total;
	else 
		total = pos_red;
	while (pos < total)
	{
		//p_printf("total = %d\n, pos = %d\n", tokenlist->element_list.total, pos);
		check_token = (t_token *)tokenlist->pf_element_get(tokenlist, pos);
		if (check_token->type == type)
			return (pos);
		pos++;
	}
	return (-1);
}

// count how many exec data structs need to be made
int	count_lists(element *tokenlist)
{
	size_t	i;
	size_t	count;
	t_token *check_token;

	i = 0;
	count = 1;
	while (i < tokenlist->element_list.total)
	{
		check_token = (t_token *)tokenlist->element_list.tokens[i];
		if (check_token->type == PIPE)
			count++;
		if (i == tokenlist->element_list.total -1 && check_token->type == PIPE)
			return(-1);
		i++;
	}
	return (count);
}


int count_args(
	element *tokenlist, 
	int pos, 
	int total)
{
	int redir;
	int i;
	t_token *check_token;
	i = pos;
	redir = 0;
	while (i < total)
	{
		check_token = (t_token *)tokenlist->pf_element_get(tokenlist, i);
		if (token_is_redirect(check_token))
		{
			if (i > 0 && lookbehind(tokenlist, i)->type != PIPE)
				redir++;
			if (lookahead(tokenlist, i))
				redir++;
			redir++;
		}
		else if (check_token->type == PIPE)
			redir += 2;
		i++;
	}
	if (redir)
		total = total - (redir + pos);
	else 
		total -= (pos + 1);
	p_printf("total = %d\n", total +1);
	return(total + 1);
}


// count until the next pos that is a command
int	count_next_cm(element *tokenlist, int pos)
{
	int redir;
	size_t i;
	t_token *check_token;

	i = pos +1;
	redir = 0;
	while (i < tokenlist->element_list.total)
	{
		check_token = (t_token *)tokenlist->element_list.tokens[i];
		if (check_token->type == PIPE)
			return (check_token->pos + 1);
		if (check_token->command)
			return (check_token->pos);
		i++;
	}
	return (-1);
}

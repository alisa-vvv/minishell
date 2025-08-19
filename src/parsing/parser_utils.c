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
t_token *lookahead(
    element *tokenlist, 
    size_t index)
{
	t_token	*check_token;

	check_token = (t_token *)tokenlist->pf_element_get((tokenlist), index
			+ 1);
	if (!check_token)
		return (NULL);
	return (check_token);
}

// func to traverse list and find a spec symbol
int	find_symbol(
	element *tokenlist, 
	int pos, 
	char symbol)
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

//count how many exec data structs need to be made
int count_lists(
	element *tokenlist)
{
	size_t		i;
	size_t		count;
	count = 0;
	while (i < tokenlist->element_list.total)
	{
		t_token * check_token;
		check_token = (t_token *)tokenlist->element_list.tokens[i];
		if (check_token->command)
			count++;
		i++;
	}
	return (count);
}

//count until the next pos that is a command
int count_next_cm(
	element *tokenlist, 
	int pos)
{
	size_t		i;
	i = pos;
	while (i < tokenlist->element_list.total)
	{
		t_token * check_token;
		check_token = (t_token *)tokenlist->element_list.tokens[i];
		if (check_token->command)
			return (check_token->pos);
		i++;
	}
	return (pos);
}

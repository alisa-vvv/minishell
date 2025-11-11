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

//searches tokenlist for specific type token
int	find_token_type(
	element *tokenlist, 
	size_t pos, 
	int pos_red, 
	t_token_type type)
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


int count_redirs(
	element *tokenlist,
	int total, 
	int i)
{
	int redir;
	t_token *check_token;
	redir = 0;

	while (i < total)
	{
		check_token = (t_token *)tokenlist->element_list.tokens[i];
		if (token_is_redirect(check_token))
		{
			if (i == 0)
				redir++;
			else if (i > 0 && lookbehind(tokenlist, i)->type != PIPE)
			{
				if (lookbehind(tokenlist, i)->type == NUMBER && (check_token->type == REDIRECT_OUT || check_token->type == REDIRECT_OUT_APP))
					redir++;
				else if (lookahead(tokenlist, i) && lookahead(tokenlist, i)->type == REDIRECT_IN)
					redir++;
			}
			if (lookahead(tokenlist, i) && (lookahead(tokenlist, i)->type == REDIRECT_OUT_APP || lookahead(tokenlist, i)->type == REDIRECT_OUT))
				redir++;
			if (check_token->type == PIPE)
				redir++;
		}
		i++;
	}
	return (redir);
}


//counts no args for the execdata arv
int count_args(
	element *tokenlist, 
	int pos, 
	int total)
{
	int redir;
	int i;
	// t_token *check_token;
	i = pos;
	redir = 0;
	redir = count_redirs(tokenlist, total, i);
	if (redir)
		total = total - (redir + pos);
	else 
		total -= (pos + 1);
	p_printf("total = %d\n", total);
	return(total + 1);
}

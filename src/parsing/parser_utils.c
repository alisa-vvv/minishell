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
	if (index > 0)
		check_token = (t_token *)tokenlist->pf_element_get((tokenlist), index -1);
	if (!check_token)
		return (NULL);
	return (check_token);
}

bool str_is_red(char c)
{
	if (c == '<' || c == '>' || c == '|')
		return (1);
	return (0);
}

//check if redirect
bool	token_is_redirect(
	t_token *check_token)
{
	if (check_token->type == HEREDOC || check_token->type == REDIRECT_IN
		|| check_token->type == REDIRECT_OUT
		|| check_token->type == REDIRECT_OUT_APP)
		return (true);
	return (false);
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
int	find_token_type(element *tokenlist, size_t pos, t_token_type type)
{
	t_token	*check_token;

	while (pos < (size_t)tokenlist->element_list.total)
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
	int flag;

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
	t_token *check_token;
	redir = 0;
	while (pos < total)
	{
		check_token = (t_token *)tokenlist->element_list.tokens[pos];
		if (token_is_redirect(check_token))
		{
			// if (check_token->type != PIPE)
			// {
			if (pos > 0 && lookbehind(tokenlist, pos)->type != PIPE)
				redir++;
			if (lookahead(tokenlist, pos))
				redir++;
			redir++;
		}
		pos++;
	}
//	p_printf("TOTAL = %d\nRedir = %d\nPOS = %d\n", total, redir, pos);
	if (redir)
		total -= redir;
	else 
		total -= pos;
	return(total + 1);
}


// count until the next pos that is a command
int	count_next_cm(element *tokenlist, int pos)
{
	int redir;
	int i;
	t_token *check_token;

	i = pos +1;
	redir = 0;
	while (i < tokenlist->element_list.total)
	{
		check_token = (t_token *)tokenlist->element_list.tokens[i];
		
		if (check_token->command)
			return (check_token->pos);
		i++;
	}
	return (-1);
}

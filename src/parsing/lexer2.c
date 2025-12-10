/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   lexer2.c                                            :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/18 18:22:33 by adecheri       #+#    #+#                */
/*   Updated: 2025/06/18 18:22:35 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

//counts how many expansions need to be done
int	count_exp(t_tokenlist *tokenlist, char symbol)
{
	int		count;
	size_t	i;
	t_token	*check_token;

	i = 0;
	count = 0;
	check_token = NULL;
	while (i < tokenlist->total)
	{
		check_token = tokenlist->tokens[i];
		if (!check_token)
			return (count);
		if (symbol == '$' && ft_strchr(check_token->value, '$'))
		{
			if (i == 0 || (i > 0 && lookbehind(tokenlist, i)
					&& lookbehind(tokenlist, i)->type != HEREDOC))
				count++;
		}
		else if (symbol == '"' && check_token->type == DOUBLE_Q)
			count++;
		else if (symbol == '\'' && check_token->type == SINGLE_Q)
			count++;
		i++;
	}
	return (count);
}

//expands single and double quotes
int	expand_quotes(t_tokenlist *tokenlist,
					t_msh_data *msh_data)
{
	int	count;
	int	count_single;
	int	err;

	err = 0;
	count = count_exp(tokenlist, '"');
	count_single = count_exp(tokenlist, '\'');
	while (count_single > 0)
	{
		err = exp_lexer(tokenlist, msh_data, SINGLE_Q, 0);
		if (err != success)
			return (err);
		count_single--;
	}
	while (count > 0)
	{
		err = exp_lexer(tokenlist, msh_data, DOUBLE_Q, 0);
		if (err != success)
			return (err);
		count--;
	}
	return (err);
}

//expand vars and merge operators for export
int	expand_param(t_tokenlist *tokenlist,
					t_msh_data *msh_data)
{
	int	count;
	int	err;

	count = count_exp(tokenlist, '$');
	while (count > 0)
	{
		err = exp_lexer(tokenlist, msh_data, PARAMETER, 0);
		if (err != success)
			return (err);
		count--;
	}
	count = count_exp(tokenlist, '=');
	while (count > 0)
	{
		err = exp_lexer(tokenlist, msh_data, OPERATOR, 0);
		if (err != success)
			return (err);
		count--;
	}
	return (success);
}

// go through lexer and clean up data
int	check_lexer(t_tokenlist *tokenlist,
				t_msh_data *msh_data)
{
	int	err;

	err = expand_param(tokenlist, msh_data);
	if (err != success)
		return (err);
	err = expand_quotes(tokenlist, msh_data);
	if (err != success)
		return (err);
	err = clean_lexer(tokenlist, 0);
	if (err != success)
		return (err);
	if (tokenlist->total == 0)
		return (success);
	err = contract_list(tokenlist, tokenlist->total - 1);
	if (err != success)
		return (err);
	err = check_spcases(tokenlist);
	if (err != success)
		return (err);
	err = check_pipes(tokenlist);
	if (err != success)
		return (err);
	err = prep_execdata(tokenlist, msh_data);
	return (err);
}

// check if left and right are args for pipe
int	check_pipe_redirect(char *str, char symbol)
{
	int	command;
	int	i;

	i = 0;
	command = 0;
	if (ft_strchr(str, symbol) != (void *)0)
	{
		while (&str[i] <= ft_strchr(str, symbol))
		{
			if (ft_isalpha(str[i]))
				command++;
			i++;
		}
		if (!command && symbol == '|')
			return (1);
		if (str[i] == symbol && (str + i + 1 == (void *)0 || *str + i
				+ 1 == '"'))
			return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
	}
	return (success);
}

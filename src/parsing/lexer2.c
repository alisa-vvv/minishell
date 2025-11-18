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
int count_exp(element *tokenlist, char symbol)
{
	int count;
	size_t i;
	t_token *check_token;
	
	i = 0;
	count = 0;
	check_token = NULL;
	while (i < tokenlist->element_list.total)
	{
		check_token = tokenlist->element_list.tokens[i];
		if (!check_token)
			return (count);
		if (symbol == '"' && check_token->type == DOUBLE_Q)
			count++;
		if (symbol == '\'' && check_token->type == SINGLE_Q)
			count++;
		if (symbol == '$' && check_token->type == PARAMETER)
			count++;
		i++;
	}
	return (count);
}

//merge tokens with pos1 and pos2 into pos 1
int merge_tokens(element *tokenlist, int pos1, int pos2)
{
	t_token *check_token;
	t_token *extra_token;

	check_token = tokenlist->element_list.tokens[pos1];
	extra_token = tokenlist->element_list.tokens[pos2];
	if (check_token->type == OPERATOR)
	{
		check_token->value = ft_strjoin(check_token->value, extra_token->value);
		tokenlist->pf_element_delete(tokenlist, pos2);
		index_lexer(&tokenlist);
	}
	return (0);
}


//expands single and double quotes 
int expand_quotes(element *tokenlist, 
	t_minishell_data *minishell_data)
{
	int count;
	int count_single;
	count = count_exp(tokenlist, '"');
	count_single = count_exp(tokenlist, '\'');
	
	while (count > 0)
	{
		if (exp_lexer(tokenlist, minishell_data, DOUBLE_Q, 0))
			return (write(1, "Wrong quotes\n", 14));
		count--;
	}
	while (count_single > 0)
	{
		if (exp_lexer(tokenlist, minishell_data, SINGLE_Q, 0))
			return (write(1, "Wrong single quotes\n", 20));
		count_single--;
	}
	return (0);
}

int expand_param(element *tokenlist,
	t_minishell_data *minishell_data)
{
	int count;
	count = count_exp(tokenlist, '$');
	while (count > 0)
	{
		if (exp_lexer(tokenlist, minishell_data, PARAMETER, 0))
			return (write(1, "Wrong variable\n", 15));
		count--;
	}
	count = count_exp(tokenlist, '=');
	while (count > 0)
	{
		if (exp_lexer(tokenlist, minishell_data, OPERATOR, 0))
			return (write(1, "Wrong operator\n", 15));
		count--;
	}
	return (0);
}

// go through lexer and clean up data
int	check_lexer(element *tokenlist, 
	t_minishell_data *minishell_data)
{

	expand_quotes(tokenlist, minishell_data);
	expand_param(tokenlist, minishell_data);
	//clean_lexer(tokenlist, 0);
	if (tokenlist->element_list.total < 2)
	{
		if (single_token(tokenlist))
			return (write(1, "Wrong redirect\n", 15));
	}
	else if (val_redir(tokenlist, 0))
		return (write(1, "Wrong redirect\n", 15));
	set_pipe_cm(tokenlist, 0);
	// t_printf("\nAfter expansion, rm quotes and set commands:\n");
	// test_tokens(*tokenlist);
	if (tokenlist)
	{
		if (pass_comm(tokenlist, minishell_data, 0, 0))
			return (write(1, "Failed passing exec data\n", 25));
	}
	return (0);
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
			return (1);
	}
	return (0);
}

// validate input on quotes, pipes
int	val_inputline(char *str)
{
	if (check_in_quote(str, ft_strlen(str) - 1))
		return (write(1, "command not found\n", 18));
	if (check_pipe_redirect(str, '|'))
		return (write(1, "command not found\n", 18));
	if (check_pipe_redirect(str, '>'))
		return (write(1, "command not found\n", 18));
	if (check_pipe_redirect(str, '<'))
		return (write(1, "command not found\n", 18));
	return (0);
}

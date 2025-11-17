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


int count_exp(element *tokenlist)
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
		if (check_token->type == DOUBLE_Q)
			count++;
		if (check_token->type == PARAMETER)
			count++;
		if (check_token->type == SINGLE_Q)
			count++;
		i++;
	}
	return (count);
}



// go through lexer and clean up data
int	check_lexer(element *tokenlist, 
	t_minishell_data *minishell_data)
{
	int count;
	count = 0;

	count = count_exp(tokenlist);
	p_printf("COUNT = %d\n", count);
	while (count > 0)
	{
		if (exp_lexer(tokenlist, minishell_data, SINGLE_Q, 0) || exp_lexer(tokenlist, minishell_data, DOUBLE_Q, 0))
			return (write(1, "Wrong quotes\n", 14));
		if (exp_lexer(tokenlist, minishell_data, PARAMETER, 0))
			return(write(1, "Wrong expanion param\n", 21));
		count = count_exp(tokenlist);
	}
	index_lexer(&tokenlist);
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
	if (pass_comm(tokenlist, minishell_data, 0, 0))
		return (write(1, "Failed passing exec data\n", 25));
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

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

//go through lexer and clean up data 
int check_lexer(
	element *tokenlist, 
	t_minishell_data *minishell_data)
{
	int i;
	i = 0;
	exp_lexer(tokenlist, &minishell_data, PARAMETER);
	exp_lexer(tokenlist, &minishell_data, SINGLE_Q);
	exp_lexer(tokenlist, &minishell_data, DOUBLE_Q);
	index_lexer(&tokenlist);
	if (tokenlist->element_list.total < 2)
	{
		if (single_token(tokenlist))
			return(write(1, "Wrong redirect\n", 15));
	}
	else if (val_redir(tokenlist))
		return (write(1, "Wrong redirect\n", 15));
	set_pipe_cm(tokenlist);
	// t_printf("\nAfter expansion, rm quotes and set commands:\n");
	// test_tokens(*tokenlist);
	pass_comm(tokenlist, minishell_data, 0, 0);
	while (i < count_lists(tokenlist))
	{
		test_execdata(*(minishell_data->exec_data + i));
		i++;
	}
	return (0); 
}

// check if left and right are args for pipe
int	check_pipe_redirect(
	char *str, 
	char symbol)
{
	int	command;
    int i; 
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
		// check for single pipe or redirect
		if (str[i] == symbol && (str + i + 1 == (void *)0 || *str + i + 1 == '"'))
			return (1);
	}
    return (0);
}

// validate input on quotes, pipes
int	val_inputline(
	char *str)
{
	if (check_in_quote(str, ft_strlen(str) -1))
	    return (write(1, "command not found\n", 18));
	if (check_pipe_redirect(str, '|'))
		return (write(1, "command not found\n", 18));
	if (check_pipe_redirect(str, '>'))
		return (write(1, "command not found\n", 18));
	if (check_pipe_redirect(str, '<'))
		return (write(1, "command not found\n", 18));
    return(0);
}


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

// count occurrence of spec symbol in array
int	count_symbol(element *tokenlist, int pos, char symbol)
{
	int		count;
	int		i;
	t_token	*check_token;

	check_token = tokenlist->pf_element_get(tokenlist, pos);
	count = 0;
	i = 0;
	while ((size_t)pos < tokenlist->element_list.total)
	{
		if (find_symbol(tokenlist, pos, symbol) > -1)
		{
			while (check_token->value[i])
			{
				if (check_token->value[i] == symbol)
					count++;
				i++;
			}
		}
		pos++;
	}
	return (count);
}
//test token
void test_tokens(element tokenlist)
{
	size_t i;
	t_token	*token_test;

	i = 0;
	while (i < tokenlist.element_list.total)
	{
		token_test = tokenlist.element_list.tokens[i];
		t_printf("Token value = %s$\n", token_test->value);
		t_printf("Token type = %d$\n", token_test->type);
		i++;
	}
	t_printf("\n");
}
//index lexer by traversing - not needed anymore
int index_lexer(element **tokenlist)
{
    size_t i;
    i = 0;
	e_printf("TOTAL= %zu \n", (size_t)(*tokenlist)->element_list.total);

    while (i < (size_t)(*tokenlist)->element_list.total)
    {
        t_token *check_token;
		check_token = (t_token *)(*tokenlist)->element_list.tokens[i];
		check_token->pos = i;
		//e_printf("POS= %d \n", check_token->pos);
        i++;
    }
    return (0);
}

int check_lexer(element *tokenlist, t_minishell_data *minishell_data)
{
	test_tokens(*tokenlist);
	exp_lexer(tokenlist, &minishell_data, PARAMETER);
	exp_lexer(tokenlist, &minishell_data, SINGLE_Q);
	exp_lexer(tokenlist, &minishell_data, DOUBLE_Q);
	if (single_token(tokenlist) || val_redir(tokenlist))
		return (write(1, "Wrong redirect\n", 15));
	set_pipe_cm(tokenlist);
	t_printf("\nAfter expansion and rm quotes:\n");
	test_tokens(*tokenlist);
	return (0);
}


// check if left and right are args for pipe
int	check_pipe_redirect(char *str, char symbol)
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
		if (str[i] == symbol && (str + i + 1 == (void *)0 || *str + i + 1 == '"' || !ft_isspace(str[i + 1])))
			return (1);
		//else if (str[i] == symbol && (str + i + 2 == (void *)0 || *str + i + 3 == '"' || !ft_isspace(str[i + 2])))
		// 	return (1);
		//t_printf("\nstring is:%c\n", str[i+1]);
	}
    return (0);
}

// validate input on quotes, pipes
int	val_inputline(char *str)
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



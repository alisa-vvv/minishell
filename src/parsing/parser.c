/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   parser.c                                        :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/03 16:55:05 by adecheri       #+#    #+#                */
/*   Updated: 2025/06/03 16:55:07 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// construc parsing table
// first() and follow() funcs
// lookahead func for check next token
// match function to check and expand on aliases

// Your shell must implement the following built-in commands:
//     - echo with option -n
//     - cd with only a relative or absolute path
//     - pwd with no options
//     - export with no options
//     - unset with no options
//     - env with no options or arguments
//     - exit with no options


// func returning next token type in the array
t_token *lookahead(element **tokenlist, size_t index)
{
	t_token	*check_token;

	check_token = (t_token *)(*tokenlist)->pf_element_get((*tokenlist), index
			+ 1);
	if (!check_token)
		return (NULL);
	return (check_token);
}

// check lexer on expansion and quotes
int	exp_lexer(element *tokenlist, t_minishell_data **minishell_data, int type)
{
	size_t	i;
	t_token	*check_token;

	i = 0;
	while (i < (size_t)tokenlist->element_list.total)
	{
		check_token = (t_token *)tokenlist->element_list.tokens[i];
		// e_printf("TYPE = %d \n", (int)check_token->type);
		if ((int)check_token->type == PARAMETER && type == PARAMETER
			|| ((int)check_token->type == DOUBLE_Q && type == DOUBLE_Q))
		{
			rm_quotes(tokenlist, i, '"');
			if (type == DOUBLE_Q && ft_strchr(check_token->value, '$') != NULL)
			{
				if (expand_var(&tokenlist, i, minishell_data, true))
					i--;
			}
			else if (type == PARAMETER)
			{
				if (expand_var(&tokenlist, i, minishell_data, false))
					i--;
			}
		}
		else if ((int)check_token->type == SINGLE_Q && type == SINGLE_Q)
			rm_quotes(tokenlist, i, '\'');
		i++;
	}
	return (0);
}

// push appropiate token to exec_data argv
int	add_arg_to_list(t_exec_data **comm_list, element *tokenlist, int pos)
{
	t_token		*check_token;

	check_token = (t_token *)tokenlist->element_list.tokens[pos];
	(*comm_list)->argv[pos] = ft_strdup(check_token->value);
	if (!(*comm_list)->argv[pos])
		return (write(1, MALLOC_ERR, 15));
	if (check_token->type >= CAT && check_token->type <= UNSET)
		(*comm_list)->is_builtin = check_token->type;
	if (lookahead(&tokenlist, pos)->type == PIPE)
		(*comm_list)->input_is_pipe = true;
	if (lookahead(&tokenlist, pos)->type == HEREDOC)
	{
		(*comm_list)->redirections = malloc(sizeof(t_redir_list));
		if (!(*comm_list)->redirections)
			return (write(1, MALLOC_ERR, 15));
	}
	if (check_token->type == HEREDOC_DEL)
		(*comm_list)->redirections->heredoc_delim = ft_strdup(check_token->value);
	
	return (0);
}

t_exec_data	*convert_data(element *tokenlist)
{
	size_t		i;
	t_exec_data	*comm_list;

	i = 0;
	comm_list = malloc(sizeof(t_exec_data));
	if (!comm_list)
		return (NULL);
	comm_list->argv = malloc(sizeof(char *) * tokenlist->element_list.total);
	if (!comm_list->argv)
		return (NULL);
	comm_list->argv[tokenlist->pf_element_total(tokenlist)] = NULL;
	while (i < (size_t)tokenlist->element_list.total)
	{
		if (!add_arg_to_list(&comm_list, tokenlist, i))
		{
			free_2d_arr((void *)comm_list->argv);
			return (NULL);
		}
		i++;
	}
	return (comm_list);
}

// typedef struct	s_exec_data
// {
// 	char			**argv;
// 	int				is_builtin;
// 	int				input_is_pipe;
// 	int				output_is_pipe;
// 	t_redir_list	*redirections;
// }	t_exec_data;


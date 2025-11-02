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

// make an empty execdata
int	make_cm_list(
	element *tokenlist, 
	t_exec_data *comm_list, 
	size_t pos,
	int pos_red)
{
	int	total;

	//	p_printf("POS = %d and POS_RED = %d\n", pos, pos_red);
	if (pos_red > 0)
		total = count_args(tokenlist, pos, pos_red);
	else
		total = count_args(tokenlist, pos, tokenlist->element_list.total);
	if (total == 0)
	{
		comm_list->argv = NULL;
		return (0);
	}
	comm_list->argv = malloc(sizeof(char *) * (total + 1));
	if (!comm_list->argv)
		return (write(1, MALLOC_ERR, 15));
	comm_list->argv[total] = NULL;
	return (0);
}

// start conversion by making lists of commands
int	pass_comm(
	element *tokenlist, 
	t_minishell_data *minishell_data, 
	int i,
	int pos)
{
	int	pos_red;

	if (count_lists(tokenlist) == -1)
		return (write(1, "No lists counted\n", 17));
	minishell_data->command_count = count_lists(tokenlist);
	minishell_data->exec_data = ft_calloc(minishell_data->command_count, sizeof(t_exec_data));
	if (!minishell_data->exec_data)
		return (write(1, MALLOC_ERR, 15));
	while (i < minishell_data->command_count)
	{
		pos_red = count_next_cm(tokenlist, pos);
		if (pos_red > 0 && lookahead(tokenlist, pos)->type == HEREDOC)
			pos_red = count_next_cm(tokenlist, pos + 1);
		convert_data(tokenlist, minishell_data, i, pos, pos_red);
		if (pos_red > 0 && find_token_type(tokenlist, pos,
				find_token_type(tokenlist, pos, pos_red, PIPE), HEREDOC) == -1)
			pos = pos_red;
		else if (pos_red > 0 && find_token_type(tokenlist, pos, pos_red,
				PIPE) != -1)
			pos = find_token_type(tokenlist, pos, pos_red, PIPE) + 1;
		else
			pos = count_next_cm(tokenlist, pos_red);
		i++;
	}
	return (0);
}

// convert the tokenlist to executable data
int	convert_data(
	element *tokenlist, 
	t_minishell_data *minishell_data, 
	int i,
	size_t pos, 
	int pos_red)
{
	t_exec_data	*comm_list;

	comm_list = minishell_data->exec_data + i;
	//	p_printf("\nCONVERT DATA:\n Pos = %d\n Pos_red = %d\n", pos, pos_red);
	if (make_cm_list(tokenlist, comm_list, pos, pos_red))
		return (write(1, "Command list failed\n", 20));
	comm_list->redirections = NULL;
	if (find_token_type(tokenlist, pos, pos_red, HEREDOC) != -1)
		set_heredoc(comm_list, tokenlist, pos, pos_red);
	else if (fill_comm_list(comm_list, tokenlist, pos, pos_red))
		return (write(1, "Fill list failed\n", 17));
	//	p_printf("Next position = %d\n", count_next_cm(tokenlist, pos));
	return (0);
}

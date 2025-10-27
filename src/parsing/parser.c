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

// typedef struct	s_redir_list
// {
// 	t_redirect_type		type;
// 	int					src_fd; // equal to -1 if fd not provided/not default
// 	int					dest_fd; // equal to -1 if fd not provided/not default
// 	char				*dest_filename; // equal to NULL if filename wasn't provided
// 	char				*src_filename; // equal to NULL if filename wasn't provided
// 	char				*heredoc_delim; // null or delim, will be used to check if input is heredoc
// 	struct s_redir_list	*next;
// }	t_redir_list;

int set_exec_def(
	t_exec_data *execdata, 
	element *tokenlist,
	size_t pos)
{
	t_token *check_token;
	check_token = tokenlist->pf_element_get(tokenlist, pos);
	execdata->builtin_name = set_builtins(check_token->type);
	execdata->input_is_pipe = false;
	execdata->output_is_pipe = false;
	execdata->redirections = NULL;
	return (0);
}

// push appropiate token to argv skipping redirects and heredoc delim 
int	add_arg_to_list(
	t_exec_data *comm_list, 
	element *tokenlist,
	int *i,
	size_t pos,
	int pos_red)
{
	t_token		*check_token;
	check_token = (t_token *)tokenlist->element_list.tokens[pos];
	if ((pos + 1 < tokenlist->element_list.total        
		&& token_is_redirect(lookahead(tokenlist, pos)))
		|| (pos > 0 && token_is_redirect(lookbehind(tokenlist, pos))))
		*i -= 1;
	else if (check_token->command)
	{
		if (pos > 0 && lookbehind(tokenlist, pos)->type == PIPE)
		{
			comm_list->argv[*i] = ft_strdup(check_token->value);
			comm_list->input_is_pipe = true;
		}
		else
			comm_list->argv[*i] = ft_strdup(check_token->value);
		comm_list->builtin_name = set_builtins(check_token->type);
	}
	else if (token_is_redirect(check_token))
	{
		*i -= 1;
		add_redirect(comm_list, tokenlist, pos, pos_red);
		return (0);
	}
	else if (check_token->type == PIPE)
	{
		comm_list->output_is_pipe = true;
		return (0);
	}
	else
		comm_list->argv[*i] = ft_strdup(check_token->value);
//	p_printf("arg[%d]: %s\n", *i, comm_list->argv[*i]);
	return (0);
}


int fill_comm_list(
	t_exec_data *exec_data,
	element *tokenlist,
	size_t pos,
	int pos_red)
{
	size_t total;
	int i;
	i = 0;

	set_exec_def(exec_data, tokenlist, pos);
	if (pos_red < 0)
		total = tokenlist->element_list.total;
	else
		total = pos_red;
	while (pos < total)
	{
		if (add_arg_to_list(exec_data, tokenlist, &i, pos, pos_red))
		{
			free_2d_arr((void *)exec_data->argv);
			return (write(1, MALLOC_ERR, 15));
		}
		i++;
		pos++;
	}
	p_printf("Token list total = %d\n Token list i = %d\n", total, i);
	// if (i > 1)
	// 	exec_data->argv[i] = NULL;
	return (0);
}

//make an empty execdata
int make_cm_list(
	element *tokenlist,
	t_exec_data *comm_list,
	size_t pos,
	int pos_red)
{
	int total;
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


//start conversion by making lists of commands
int pass_comm(
	element *tokenlist, 
	t_minishell_data *minishell_data,
	int i,
	int pos)
{
	int n_list;
	int pos_red;
	if (count_lists(tokenlist) == -1)
		return (write(1, "Wrong pipe command\n", 19));
	n_list = count_lists(tokenlist);
	minishell_data->exec_data = ft_calloc(n_list, sizeof(t_exec_data));
	minishell_data->command_count = n_list;
	if (!minishell_data->exec_data)
		return(write(1, MALLOC_ERR, 15));
	while (n_list > 0)
	{
		pos_red = count_next_cm(tokenlist, pos);
		if (pos_red > 0 && lookahead(tokenlist, pos)->type == HEREDOC)
			pos_red = count_next_cm(tokenlist, pos + 1);
		convert_data(tokenlist, minishell_data, i, pos, pos_red);
		if (pos_red > 0 && find_token_type(tokenlist, pos, find_token_type(tokenlist, pos, pos_red, PIPE), HEREDOC) == -1)
			pos = pos_red;
		else if (pos_red > 0 && find_token_type(tokenlist, pos, pos_red, PIPE) != -1)
			pos = find_token_type(tokenlist, pos, pos_red, PIPE) + 1;
		else 
			pos = count_next_cm(tokenlist, pos_red);
		i++;
		n_list--;
	}
	return (0);
}

//convert the tokenlist to executable data 
int convert_data(
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


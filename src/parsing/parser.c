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

// set heredoc 
int set_heredoc(
	t_exec_data** execdata, 
	element *tokenlist, 
	int pos)
{
	t_token *check_token;	
	size_t i;
	i = 0;
	
	(*execdata)->redirections->type = heredoc;
	(*execdata)->redirections->dest_fd = -1;
	(*execdata)->redirections->dest_filename = NULL;
	(*execdata)->redirections->src_fd = -1;
	(*execdata)->redirections->src_filename = NULL;
	(*execdata)->redirections->next = NULL;
	while (pos < tokenlist->element_list.total)
	{	
		check_token = (t_token *)tokenlist->element_list.tokens[pos];
		if (check_token->type == HEREDOC_DEL)
		{
			(*execdata)->redirections->heredoc_delim = ft_strdup(check_token->value);
			pos++;
		}
		else 
			(*execdata)->redirections->heredoc_delim = NULL;
		(*execdata)->argv[i] = ft_strdup(check_token->value);
		pos++;
		i++;
	}
	return (0);
}

t_builtin_name set_builtins(t_token_type tokentype)
{
	if (tokentype == CD)
		return (builtin_cd);
	else if (tokentype == ECHO)
		return (builtin_echo);
	else if (tokentype == EXPORT)
		return (builtin_export);
	else if (tokentype == ENV)
		return (builtin_env);
	else if (tokentype == EXIT)
		return (builtin_exit);
	else if (tokentype == PWD)
		return (builtin_pwd);
	else if (tokentype == UNSET)
		return (builtin_unset);
	else 
		return(not_builtin);
}


// push appropiate token to exec_data argv
int	add_arg_to_list(
	t_exec_data **comm_list, 
	element *tokenlist, 
	int pos)
{
	t_token		*check_token;
	check_token = (t_token *)tokenlist->element_list.tokens[pos];
	if (check_token->command)
	{
		if (check_token->type == HEREDOC)
			return (set_heredoc(comm_list, tokenlist, pos));
		(*comm_list)->builtin_name = set_builtins(check_token->type);
	}
	if (pos + 1 < tokenlist->element_list.total && lookahead(tokenlist, pos)->type == PIPE)
		(*comm_list)->output_is_pipe = true;
	
	(*comm_list)->argv[pos] = ft_strdup(check_token->value);
	return (0);
}


int fill_comm_list(
	t_exec_data **execdata,
	element *tokenlist,
	size_t pos,
	int pos_red
)
{
	int total;
	t_token *check = (t_token *)tokenlist->element_list.tokens[pos];
	if (pos_red <= pos)
		total = tokenlist->element_list.total;
	else if (check->type == HEREDOC)
		return (add_redirect(execdata, tokenlist, 0));
	else
		total = pos_red - 1;
	while (pos < total)
	{
		t_token* check_token;
		check_token = tokenlist->pf_element_get(tokenlist, pos);
		if (add_arg_to_list(execdata, tokenlist, pos))
		{
			free_2d_arr((void *)(*execdata)->argv);
			return (write(1, MALLOC_ERR, 15));
		}
		pos++;
	}
	return (0);
}

//make an empty execdata
t_exec_data *make_cm_list(
	element *tokenlist, 
	size_t pos,
	int pos_red)
{
	t_exec_data	*comm_list;
	comm_list = malloc(sizeof(t_exec_data));
	if (!comm_list)
		return (NULL);
	// p_printf("POS = %d and POS_RED = %d\n", pos, pos_red);
	if (pos_red > 0)
	{
		pos_red = count_next_cm(tokenlist, pos);
		comm_list->argv = malloc(sizeof(char *) * pos_red + 1);
	}
	else 
		comm_list->argv = malloc(sizeof(char *) * (tokenlist->element_list.total + 1));
	if (!comm_list->argv)
		return (NULL);
	if (pos_red > pos)
		comm_list->argv[count_next_cm(tokenlist, pos)-1] = NULL;
	else 
		comm_list->argv[tokenlist->element_list.total] = NULL;
	return (comm_list);
}


//add re
int add_redirect(
	t_exec_data** execdata, 
	element *tokenlist, 
	int pos)
{
	t_token *check_token;
	check_token = tokenlist->pf_element_get(tokenlist, pos);
	if (check_token->type == HEREDOC)
		return (set_heredoc(execdata, tokenlist, pos));
	else 
	{
		(*execdata)->redirections->src_fd = -1;
		(*execdata)->redirections->dest_fd = -1;
		check_token = tokenlist->pf_element_get(tokenlist, pos - 1);
		if (check_token->value)
			(*execdata)->redirections->src_filename = ft_strdup(check_token->value);
		else 
			(*execdata)->redirections->src_filename = NULL;
	}
	return (0);
}
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


//convert the tokenlist to executable data 
t_exec_data	*convert_data(
	element *tokenlist, 
	size_t pos)
{
	t_exec_data	*comm_list;
	t_token * check_token;
	int pos_red;

	pos_red = count_next_cm(tokenlist, pos);
	check_token = (t_token *)tokenlist->pf_element_get(tokenlist, pos);
	comm_list = make_cm_list(tokenlist, pos, pos_red);
	if (!comm_list)
		return (NULL);
	if (count_lists(tokenlist) > 0)
	{
		comm_list->redirections = malloc(sizeof(t_redir_list));
		if (!comm_list->redirections)
			return (NULL);
		p_printf("IN HERE\n");
		if (pos > 0)
			add_redirect(&comm_list, tokenlist, count_next_cm(tokenlist, pos) -1);
		else if (add_redirect(&comm_list, tokenlist, pos))
		{
			write(1, "redirect error\n", 15);
			return (NULL);
		}
	}
	else 
		comm_list->redirections = NULL;
	if (fill_comm_list(&comm_list, tokenlist, pos, pos_red))
		return NULL;
	p_printf("Next position = %d\n", count_next_cm(tokenlist, pos));
	return (comm_list);
}


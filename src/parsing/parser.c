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

int set_heredoc_red(
	t_exec_data** execdata, 
	element *tokenlist, 
	int pos,
	int pos_red)
{
	
	(*execdata)->redirections->type = heredoc;
	(*execdata)->redirections->dest_fd = -1;
	(*execdata)->redirections->dest_filename = NULL;
	(*execdata)->redirections->src_fd = -1;
	(*execdata)->redirections->src_filename = NULL;
	if ((*execdata)->output_is_pipe)
		add_redirect(execdata, tokenlist, pos_red -1, pos_red);
	else 
		(*execdata)->redirections->next = NULL;
	return (0);
}

// set heredoc 
int set_heredoc(
	t_exec_data** execdata, 
	element *tokenlist, 
	int pos,
	int pos_red)
{
	t_token *check_token;	
	size_t i;

	i = 0;
	(*execdata)->redirections->heredoc_delim = NULL;
	while (pos < tokenlist->element_list.total)
	{	
		check_token = (t_token *)tokenlist->element_list.tokens[pos];
		if (check_token->type == HEREDOC_DEL)
			(*execdata)->redirections->heredoc_delim = ft_strdup(check_token->value);
		if (!token_is_redirect(check_token) && check_token->type != HEREDOC_DEL)
		{
			(*execdata)->argv[i] = ft_strdup(check_token->value);
			i++;
		}
		pos++;
	}
	if (i == 0)
		ft_free_arr((void*)(*execdata)->argv);
	(*execdata)->argv[i] = NULL;
	if (pos_red > 0)
		(*execdata)->output_is_pipe = true;
	else 
		(*execdata)->output_is_pipe = false;
	(*execdata)->builtin_name = not_builtin;
	(*execdata)->input_is_pipe = false;
	set_heredoc_red(execdata, tokenlist, pos, pos_red);
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

// int set_redirect(
// 	t_exec_data **comm_list, 
// 	element *tokenlist, 
// 	size_t pos,
// 	int pos_red)
// {
// 	t_token		*check_token;
// 	check_token = (t_token *)tokenlist->element_list.tokens[pos];
// 	while (pos_red > 0 && pos < pos_red)
// 	{
// 		if (check_token->command)
// 		{
// 			if (lookahead(tokenlist, pos -2) == PIPE)
// 				set_pipe_ls(comm_list, tokenlist, pos);
// 			else if (lookahead(tokenlist, pos -2) == REDIRECT_IN)
// 				set_redin_ls(comm_list, tokenlist, pos);
// 			else if (lookahead(tokenlist, pos -2) == REDIRECT_OUT_APP)
// 				set_redapp_ls(comm_list, tokenlist, pos);
// 			else if (lookahead(tokenlist, pos -2) == REDIRECT_OUT)
// 				set_redout_ls(comm_list, tokenlist, pos);
// 			else 	
// 				return (write(1, "redirect error", 14));
// 		}
// 	}
// 	return (0);
// }

// push appropiate token to exec_data argv
int	add_arg_to_list(
	t_exec_data **comm_list, 
	element *tokenlist, 
	size_t pos,
	int pos_red)
{
	t_token		*check_token;
	check_token = (t_token *)tokenlist->element_list.tokens[pos];
	while (pos < tokenlist->element_list.total)
	{
		if (check_token->command)
		{
			if (check_token->type == HEREDOC)
				return (set_heredoc(comm_list, tokenlist, pos, pos_red));
			if (pos > 0 && lookahead(tokenlist, pos -2)->type == PIPE)
				(*comm_list)->input_is_pipe = true;
			(*comm_list)->builtin_name = set_builtins(check_token->type);
		}
		if (pos_red > 0 && pos < pos_red && !token_is_redirect(check_token))
			(*comm_list)->argv[pos] = ft_strdup(check_token->value);
		if (pos + 1 < tokenlist->element_list.total && lookahead(tokenlist, pos)->type == PIPE)
		{
			(*comm_list)->output_is_pipe = true;
			add_redirect(comm_list, tokenlist, pos, pos_red);
			return (0);
		}
		pos++;
	}
	return (0);
}


int fill_comm_list(
	t_exec_data **execdata,
	element *tokenlist,
	size_t pos,
	int pos_red)
{
	int total;
	t_token *check = (t_token *)tokenlist->element_list.tokens[pos];
	if (pos_red <= pos)
		total = tokenlist->element_list.total;
	else if (check->type == HEREDOC)
		return (add_redirect(execdata, tokenlist, pos, pos_red));
	else
		total = pos_red - 1;
	while (pos < total)
	{
		if (add_arg_to_list(execdata, tokenlist, pos, pos_red))
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
	comm_list->argv = malloc(sizeof(char *) * (tokenlist->element_list.total + 1));
	// if (pos_red > 0)
	// {
	// 	pos_red = count_next_cm(tokenlist, pos);
	// 	comm_list->argv = malloc(sizeof(char *) * pos_red + 1);
	// }
	// else 
	// 	comm_list->argv = malloc(sizeof(char *) * (tokenlist->element_list.total + 1));
	// if (!comm_list->argv)
	// 	return (NULL);
	// if (pos_red > pos)
	// 	comm_list->argv[count_next_cm(tokenlist, pos)-1] = NULL;
	// else 
		comm_list->argv[tokenlist->element_list.total] = NULL;
	return (comm_list);
}

int set_pipe(
	t_exec_data **execdata, 
	element *tokenlist, 
	int pos,
	int pos_red)
{
	t_token *check_token;
	t_redir_list *redirlist;

	if (pos > 1)
	{
		redirlist = (*execdata)->redirections->next;
		while (redirlist->next)
		{
			redirlist = redirlist->next;
		}
		redirlist->type = input;
		redirlist->src_fd = -1;
		redirlist->dest_fd = -1;
		check_token = tokenlist->pf_element_get(tokenlist, pos - 1);
	}
	redirlist = (*execdata)->redirections;
	if (check_token->type == STRING)
		redirlist->src_filename = ft_strdup(check_token->value);
	else 
		redirlist->src_filename = NULL;
	if (pos + 1 < tokenlist->element_list.total && lookahead(tokenlist, pos)->type == PIPE)
	{
		add_redirect(execdata, tokenlist, pos, pos_red);
	}
	return (0);
}

//add re
int add_redirect(
	t_exec_data **execdata,
	element *tokenlist, 
	int pos,
	int pos_red)
{
	t_token *check_token;
	t_redir_list *redirlist;
	
	redirlist = malloc(sizeof(t_redir_list));
	if (!redirlist)
		return (write(1, MALLOC_ERR, 15));
	redirlist->next = NULL;
	if ((*execdata)->redirections == NULL)
		(*execdata)->redirections = redirlist;
	else
	{
		t_redir_list *next;
		next = (*execdata)->redirections->next;
		while (next)
		{
			next = next->next;
		}
		next = redirlist;
	}
	check_token = tokenlist->pf_element_get(tokenlist, pos);
	if (check_token->type == HEREDOC)
		return (set_heredoc(execdata, tokenlist, pos, pos_red));
	else if (check_token->type == PIPE)
		set_pipe(execdata, tokenlist, pos, pos_red);
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
	int b;
	b = count_lists(tokenlist);
	p_printf("Count list = %d\n", b);
	comm_list->redirections = NULL;
	if (count_lists(tokenlist) > 0)
		add_redirect(&comm_list, tokenlist, pos, pos_red);
	if (fill_comm_list(&comm_list, tokenlist, pos, pos_red))
		return NULL;
	p_printf("Next position = %d\n", count_next_cm(tokenlist, pos));
	return (comm_list);
}


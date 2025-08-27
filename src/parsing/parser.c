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


// push appropiate token to exec_data argv
int	add_arg_to_list(
	t_exec_data **comm_list, 
	element *tokenlist,
	int *i,
	size_t pos,
	int pos_red)
{
	t_token		*check_token;
	check_token = (t_token *)tokenlist->element_list.tokens[pos];
	(*comm_list)->output_is_pipe = false;
	if (check_token->command)
	{
		if (check_token->type == HEREDOC)
			return (set_heredoc(comm_list, tokenlist, pos, count_next_cm(tokenlist, pos)));
		if (pos > 2 && lookahead(tokenlist, pos -2)->type == PIPE)
			(*comm_list)->input_is_pipe = true;
		(*comm_list)->builtin_name = set_builtins(check_token->type);
	}
	if (token_is_redirect(check_token))
	{
		*i -= 1;
		add_redirect(comm_list, tokenlist, pos, pos_red);
	}
	else
		(*comm_list)->argv[*i] = ft_strdup(check_token->value);
	if (pos + 1 < tokenlist->element_list.total && lookahead(tokenlist, pos)->type == PIPE)
	{
		(*comm_list)->output_is_pipe = true;
		return (0);
	}
//	p_printf("arg[%d]: %s\n", *i, (*comm_list)->argv[*i]);
	return (0);
}


int fill_comm_list(
	t_exec_data **exec_data,
	element *tokenlist,
	size_t pos,
	int pos_red)
{
	int total;
	int i;
	i = 0;
	t_token *check = (t_token *)tokenlist->element_list.tokens[pos];
	if (pos_red < 0)
		total = tokenlist->element_list.total;
	else if (check->type == HEREDOC)
		return (0);
	else
		total = pos_red;
	while (pos < total)
	{
		if (add_arg_to_list(exec_data, tokenlist, &i, pos, pos_red))
		{
			free_2d_arr((void *)(*exec_data)->argv);
			return (write(1, MALLOC_ERR, 15));
		}
		i++;
		pos++;
	}
	(*exec_data)->argv[i] = NULL;
//	p_printf("Token list total = %d\n", tokenlist->element_list.total);
	return (0);
}

//make an empty execdata
int make_cm_list(
	element *tokenlist,
	t_exec_data **comm_list,
	size_t pos,
	int pos_red)
{

//	p_printf("POS = %d and POS_RED = %d\n", pos, pos_red);
//	(*comm_list)->argv = ft_calloc(tokenlist->element_list.total + 1, sizeof(char *));
	if (pos_red > 0)
	{
		pos_red = count_next_cm(tokenlist, pos);
		(*comm_list)->argv = malloc(sizeof(char *) * pos_red + 1);
	}
	else
		(*comm_list)->argv = malloc(sizeof(char *)* tokenlist->element_list.total - pos + 1);
	if (!(*comm_list)->argv)
		return (write(1, MALLOC_ERR, 15));
	if (pos_red > pos)
		(*comm_list)->argv[count_next_cm(tokenlist, pos) - 1] = NULL;
	else
		(*comm_list)->argv[tokenlist->element_list.total - pos] = NULL;
	return (0);
}




int set_redirect(
	t_exec_data **execdata,
	element *tokenlist,
	int pos, 
	int pos_red
)
{
	t_token *check_token;
	check_token = tokenlist->pf_element_get(tokenlist, pos);
	if (check_token->type == REDIRECT_IN)
	{
		(*execdata)->redirections->type = input;
		(*execdata)->redirections->dest_filename = ft_strdup(lookahead(tokenlist, pos -2)->value);
		(*execdata)->redirections->src_filename = ft_strdup(lookahead(tokenlist, pos)->value);
	}
	else if (check_token->type == REDIRECT_OUT_APP)
	{
		(*execdata)->redirections->type = append;
		(*execdata)->redirections->dest_filename = ft_strdup(lookahead(tokenlist, pos)->value);
		(*execdata)->redirections->src_filename = ft_strdup(lookahead(tokenlist, pos -2)->value);
	}
	else if (check_token->type == REDIRECT_OUT)
	{
		
		(*execdata)->redirections->type = trunc;
		(*execdata)->redirections->dest_filename = ft_strdup(lookahead(tokenlist, pos)->value);
		(*execdata)->redirections->src_filename = ft_strdup(lookahead(tokenlist, pos -2)->value);
	}
	(*execdata)->redirections->heredoc_delim = NULL;
	(*execdata)->redirections->src_fd = -1;
	(*execdata)->redirections->dest_fd = -1;
	return (0);
}

//add redirect to list of execdata
int add_redirect(
	t_exec_data **execdata,
	element *tokenlist, 
	int pos,
	int pos_red)
{
	t_token *check_token;
	t_redir_list *redirlist;
	
	check_token = tokenlist->pf_element_get(tokenlist, pos);
	if (check_token->type == PIPE)
		return (0);
	redirlist = ft_calloc(1, sizeof(t_redir_list));
	if (!redirlist)
		return (write(1, MALLOC_ERR, 15));
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
	if (token_is_redirect(check_token))
		set_redirect(execdata,tokenlist, pos, pos_red);		
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


int pass_comm(
	element *tokenlist, 
	t_minishell_data *minishell_data)
{
	int n_list;
	int i;
	size_t pos;
	int pos_red;

	i = 0;
	pos = 0;
	n_list = count_lists(tokenlist);
	minishell_data->exec_data = ft_calloc(n_list, sizeof(t_exec_data));
	p_printf("List count = %d\n", n_list);
	if (!minishell_data->exec_data)
		return(write(1, MALLOC_ERR, 15));
	while (n_list > 0)
	{
		pos_red = count_next_cm(tokenlist, pos);
		convert_data(tokenlist, minishell_data, i, pos, pos_red);
		if (pos_red > 0)
		{
			pos = pos_red;
		}
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
	if (make_cm_list(tokenlist, &comm_list, pos, pos_red))
		return (write(1, "Command list failed\n", 20));
	comm_list->redirections = NULL;
	
	if (fill_comm_list(&comm_list, tokenlist, pos, pos_red))
		return (write(1, "Fill list failed\n", 17));
//	p_printf("Next position = %d\n", count_next_cm(tokenlist, pos));
	
	return (0);
}


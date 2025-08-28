/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   parser_heredoc.c                                    :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/08/28 13:12:42 by adecheri       #+#    #+#                */
/*   Updated: 2025/08/28 13:12:43 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"


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

// int set_heredoc_arg(t_exec_data** execdata, 
// 	element *tokenlist, 
// 	int pos,
// 	int pos_red)
// {

// }

int set_heredoc_red(
	t_exec_data** execdata, 
	element *tokenlist, 
	int pos,
	int pos_red)
{
    t_token *check_token;
    int i;
    i = 0; 	
    
    (*execdata)->redirections->type = heredoc;
    (*execdata)->builtin_name = not_builtin;
    (*execdata)->output_is_pipe = false;
    (*execdata)->redirections->dest_filename = NULL;
	while (pos < tokenlist->element_list.total)
	{	
        check_token = (t_token *)tokenlist->element_list.tokens[pos];
        if (check_token->type != HEREDOC && lookahead(tokenlist, pos)->type == HEREDOC)
            (*execdata)->redirections->dest_filename = ft_strdup(check_token->value);
		if (check_token->type == HEREDOC_DEL)
			(*execdata)->redirections->heredoc_delim = ft_strdup(check_token->value);
		if (!token_is_redirect(check_token) && check_token->type != HEREDOC_DEL && lookahead(tokenlist, pos)->type != HEREDOC)
		{
			(*execdata)->argv[i] = ft_strdup(check_token->value);
            (*execdata)->builtin_name = set_builtins(check_token->type);
			i++;
		}
        if (lookahead(tokenlist, pos)->type == PIPE)
        {
            (*execdata)->output_is_pipe = true;
            break;
        }
		pos++;
	}
    if (i == 0)
		ft_free_arr((void*)(*execdata)->argv);
	(*execdata)->argv[i] = NULL;
	(*execdata)->redirections->src_fd = -1;
	(*execdata)->redirections->src_filename = NULL;
	if ((*execdata)->output_is_pipe)
		add_redirect(execdata, tokenlist, pos_red -1, pos + 1);
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
    t_redir_list *redirlist;

	i = 0;
    redirlist = ft_calloc(1, sizeof(t_redir_list));
	if (!redirlist)
		return (write(1, MALLOC_ERR, 15));
	if ((*execdata)->redirections == NULL)
		(*execdata)->redirections = redirlist;
    (*execdata)->redirections->heredoc_delim = NULL;
    set_heredoc_red(execdata, tokenlist, pos, pos_red);
	// while (pos < tokenlist->element_list.total)
	// {	
    //     check_token = (t_token *)tokenlist->element_list.tokens[pos];
    //     if (pos == 0 && check_token->type != HEREDOC)
    //         (*execdata)->redirections->dest_filename = ft_strdup(check_token->value);
		
	// 	if (check_token->type == HEREDOC_DEL)
	// 		(*execdata)->redirections->heredoc_delim = ft_strdup(check_token->value);
	// 	if (!token_is_redirect(check_token) && check_token->type != HEREDOC_DEL)
	// 	{
	// 		(*execdata)->argv[i] = ft_strdup(check_token->value);
    //         (*execdata)->builtin_name = set_builtins(check_token->type);
	// 		i++;
	// 	}
    //     else 
    //         (*execdata)->builtin_name = not_builtin;
    //     if (lookahead(tokenlist, pos)->type == PIPE)
    //     {
    //         (*execdata)->output_is_pipe = true;
    //         add_redirect(execdata, tokenlist, pos, pos +1);
    //     }
	// 	else 
    //         (*execdata)->output_is_pipe = false;
	// 	pos++;
	// }
    p_printf("POS_RED = %d\n", pos_red);
	(*execdata)->input_is_pipe = false;
	
	return (0);
}


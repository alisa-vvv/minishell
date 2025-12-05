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



//  if (ft_strncmp(check_token->value, "cd", 3) == 0)
//     return (builtin_cd);
// else if (ft_strncmp(check_token->value, "pwd", 4) == 0)
//     return (builtin_pwd);
// else if (ft_strncmp(check_token->value, "env", 4) == 0)
// 	return (builtin_env);
// else if (ft_strncmp(check_token->value, "echo", 5) == 0)
//     return (builtin_echo);
// else if (ft_strncmp(check_token->value, "export", 7) == 0)
//     return (builtin_export);
// else if (ft_strncmp(check_token->value, "unset", 6) == 0)
//     return (builtin_unset);
// else if (ft_strncmp(check_token->value, "exit", 5) == 0)
// 	return (builtin_exit);
// else 
// 	return(not_builtin);

//return the correct builtin for execdata from the token-type 
t_builtin_name set_builtins(t_token *check_token)
{
	if (check_token->type == CD)
		return (builtin_cd);
	else if (check_token->type == ECHO)
		return (builtin_echo);
	else if (check_token->type == EXPORT)
		return (builtin_export);
	else if (check_token->type == ENV)
		return (builtin_env);
	else if (check_token->type == EXIT)
		return (builtin_exit);
	else if (check_token->type == PWD)
		return (builtin_pwd);
	else if (check_token->type == UNSET)
		return (builtin_unset);
	else 
		return(not_builtin);
}

//set defaults of heredoc
void set_heredoc_def(t_exec_data* execdata)
{
	execdata->builtin_name = not_builtin;
    execdata->output_is_pipe = false;
	execdata->input_is_pipe = false;
    execdata->redirections->type = heredoc;
	execdata->redirections->heredoc_delim = NULL;
    execdata->redirections->dest_filename = NULL;
	execdata->redirections->dest_fd = -1;
	execdata->redirections->src_fd = -1;
}

int set_heredoc_red(
	t_exec_data* execdata, 
	t_tokenlist *tokenlist, 
	size_t pos,
	int i)
{
    t_token *check_token;
	while (pos++ < tokenlist->total)
	{	
        check_token = (t_token *)tokenlist->tokens[pos];
		if (pos > 0 && lookbehind(tokenlist, pos)->type == PIPE)
			execdata->input_is_pipe = true;
		p_printf("check_token->value: %s\n and POS is: %d\n", check_token->value, pos);
        if (check_token->type != HEREDOC && pos + 1 < tokenlist->total && lookahead(tokenlist, pos)->type == HEREDOC)
            execdata->redirections->dest_filename = ft_strdup(check_token->value);
		if (lookahead(tokenlist, i) && lookahead(tokenlist, i)->type == (size_t) HEREDOC_DEL)
			execdata->redirections->heredoc_delim = ft_strdup(lookahead(tokenlist, i)->value);
		if (!token_is_redirect(check_token) && check_token->type != HEREDOC_DEL && lookahead(tokenlist, pos) && lookahead(tokenlist, pos)->type != HEREDOC)
		{
			execdata->argv[i] = ft_strdup(check_token->value);
            execdata->builtin_name = set_builtins(check_token);
			i++;
		}
        if (pos + 1 < tokenlist->total && lookahead(tokenlist, pos)->type == PIPE)
        {
            execdata->output_is_pipe = true;
            break;
        }
	}
	if (execdata->argv)
		execdata->argv[i] = NULL;
	return (0);
}

// set heredoc 
int set_heredoc(
	t_exec_data* execdata, 
	t_tokenlist *tokenlist, 
	int pos,
	int pos_red)
{
    t_redir_list *redirlist;

    redirlist = ft_calloc(1, sizeof(t_redir_list));
	if (!redirlist)
		return (write(1, MALLOC_ERR, 15));
	if (execdata->redirections == NULL)
		execdata->redirections = redirlist;
	set_heredoc_def(execdata);
    set_heredoc_red(execdata, tokenlist, pos, pos_red);
   // p_printf("POS_RED = %d\n", pos_red);
	return (0);
}


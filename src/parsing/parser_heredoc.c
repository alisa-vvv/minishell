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

//return the correct builtin for execdata from the token-type
t_builtin_name	set_builtins(t_token *check_token)
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
		return (not_builtin);
}

//set defaults of heredoc
void	set_heredoc_def(t_exec_data *execdata)
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

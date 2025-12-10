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

int	set_hered_pipe(t_exec_data *execdata,
	t_tokenlist *tlist,
	t_pos *ind,
	t_token *c_token)
{
	int	err;

	err = success;
	if (ind->pos > 0 && lookbehind(tlist, ind->pos)->type == PIPE)
		execdata->input_is_pipe = true;
	if (c_token->type == HEREDOC)
	{
		execdata->redirections->heredoc_delim = ft_strdup(looknxt(tlist,
					ind->pos)->value);
		if (!execdata->redirections->heredoc_delim)
			return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
		ind->pos++;
	}
	if ((size_t)ind->pos < tlist->total && looknxt(tlist, ind->pos)
		&& looknxt(tlist, ind->pos)->type == PIPE)
	{
		execdata->output_is_pipe = true;
		return (err);
	}
	if (looknxt(tlist, ind->pos) &&c_token->type != HEREDOC)
		add_redirect(execdata, tlist, ind);
	return (err);
}

// if (execdata->argv)
// 	execdata->argv[i] = NULL;
// sets data for heredocs
int	set_heredoc_red(
	t_exec_data *execdata,
	t_tokenlist *tlist,
	t_pos *ind,
	int total)
{
	t_token	*c_token;
	int		i;
	int		err;

	i = 0;
	err = success;
	while (ind->pos < total)
	{
		c_token = (t_token *)tlist->tokens[ind->pos];
		err = set_hered_pipe(execdata, tlist, ind, c_token);
		if (err != success)
			return (err);
		if (tok_is_red(c_token))
			break; 
		if (!tok_is_red(c_token) && c_token->type != HEREDOC_DEL)
		{
			execdata->argv[i] = ft_strdup(c_token->value);
			if (!execdata->argv[i])
				return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
			if (!execdata->builtin_name)
				execdata->builtin_name = set_builtins(c_token);
			i++;
		}
		ind->pos++;
	}
	return (err);
}

// p_printf("POS_RED = %d\n", pos_red);
// set heredoc default values and red values
int	set_heredoc(
	t_exec_data *execdata,
	t_tokenlist *tokenlist,
	t_pos *ind)
{
	t_redir_list	*redirlist;
	int				total;
	int				err;

	err = success;
	total = find_type(tokenlist, ind->pos, tokenlist->total - 1, PIPE);
	if (total < 0)
		// total = find_type(tokenlist, ind->pos, tokenlist->total -1, RED_IN);
		total = tokenlist->total;
	redirlist = ft_calloc(1, sizeof(t_redir_list));
	if (!redirlist)
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	if (execdata->redirections == NULL)
		execdata->redirections = redirlist;
	set_heredoc_def(execdata);
	ind->pos = 0;
	err = set_heredoc_red(execdata, tokenlist, ind, total);
	return (err);

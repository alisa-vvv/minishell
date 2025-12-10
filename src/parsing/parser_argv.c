/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   parser_argv.c                                       :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/11/02 12:35:05 by adecheri       #+#    #+#                */
/*   Updated: 2025/11/02 12:35:07 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// sets default values for execdata
int	set_exec_def(t_exec_data *execdata, t_tokenlist *tokenlist, size_t pos)
{
	t_token	*check_token;

	check_token = tokenlist_get(tokenlist, pos);
	execdata->builtin_name = set_builtins(check_token);
	execdata->input_is_pipe = false;
	execdata->output_is_pipe = false;
	execdata->redirections = NULL;
	return (0);
}

//sets command details in the argv for execdata
void	set_command(
	t_exec_data *comm_list,
	t_tokenlist *tokenlist,
	t_pos *ind,
	int *i)
{
	t_token	*check_token;

	check_token = tokenlist->tokens[ind->pos];
	if (ind->pos == 0 && tok_is_red(check_token))
		return ;
	else if (ind->pos == 0 && (looknxt(tokenlist, ind->pos)
			&& tok_is_red(looknxt(tokenlist, ind->pos))))
	{
		return ;
	}
	if (ind->pos > 0 && lookbehind(tokenlist, ind->pos)->type == PIPE)
	{
		comm_list->argv[*i] = ft_strdup(check_token->value);
		comm_list->input_is_pipe = true;
	}
	else if (!tok_is_red(check_token))
		comm_list->argv[*i] = ft_strdup(check_token->value);
	comm_list->builtin_name = set_builtins(check_token);
}

void	add_arg(t_exec_data *execdata, t_token *check_token, int *i)
{
	if (!tok_is_red(check_token))
	{
		execdata->argv[*i] = ft_strdup(check_token->value);
		if (execdata->builtin_name == not_builtin)
			execdata->builtin_name = set_builtins(check_token);
	}
}

// push appropriate token to argv skipping redirects, heredoc delim and pipes
// returns: 1 if an argv entry was added, 0 if nothing was added,
//	-1 on malloc/error
int	add_arg_to_list(
	t_exec_data *comm_list,
	t_tokenlist *tokenlist,
	t_pos *ind,
	int *i)
{
	t_token	*check_token;

	check_token = (t_token *)tokenlist->tokens[ind->pos];
	if (ind->pos > 0 && tok_is_red(lookbehind(tokenlist, ind->pos)))
		return (0);
	if (check_token->command && !tok_is_red(check_token))
	{
		set_command(comm_list, tokenlist, ind, i);
		if (!comm_list->argv[*i])
			return (0);
		return (1);
	}
	if (tok_is_red(check_token) || check_token->type == PIPE)
	{
		if (check_token->type == PIPE)
			comm_list->output_is_pipe = true;
		else if (add_redirect(comm_list, tokenlist, ind))
			return (-1);
		return (0);
	}
	add_arg(comm_list, check_token, i);
	return (1);
}

// p_printf("Token list total = %d\n Token list i = %d\n Token list pos = %d\n",
//		total, i, pos);
//fill the execdata with specified argv from tokenlist
int	fill_comm_list(
	t_exec_data *exec_data,
	t_tokenlist *tokenlist,
	t_pos *ind)
{
	size_t	total;
	int		i;
	int		added;

	i = 0;
	set_exec_def(exec_data, tokenlist, ind->pos);
	if (ind->red < 0)
		total = tokenlist->total;
	else
		total = ind->red;
	while ((size_t)ind->pos < total)
	{
		added = add_arg_to_list(exec_data, tokenlist, ind, &i);
		if (added == -1)
		{
			free_2d_arr((void *)exec_data->argv);
			return (msh_perror(NULL, MALLOC_ERR, parse_err), malloc_err);
		}
		if (added == 1)
			i++;
		ind->pos++;
	}
	exec_data->argv[i] = NULL;
	return (0);
}

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

// count how many exec data structs need to be made
int	count_lists(t_tokenlist *tokenlist)
{
	size_t	i;
	size_t	count;
	t_token	*check_token;

	i = 0;
	count = 1;
	while (i < tokenlist->total)
	{
		check_token = (t_token *)tokenlist->tokens[i];
		if (check_token->type == PIPE)
			count++;
		if (i == tokenlist->total - 1 && check_token->type == PIPE)
			return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
		i++;
	}
	return (count);
}

// count until the next pos that is a command
int	count_next_cm(t_tokenlist *tokenlist, int pos)
{
	size_t	i;
	t_token	*check_token;

	i = pos + 1;
	while (i < tokenlist->total)
	{
		check_token = (t_token *)tokenlist->tokens[i];
		if (check_token->type == PIPE && looknxt(tokenlist, pos))
			return (check_token->pos + 1);
		if (check_token->command)
			return (check_token->pos);
		i++;
	}
	return (-1);
}

// make an empty execdata
int	make_cm_list(
	t_tokenlist *tokenlist,
	t_exec_data *comm_list,
	t_pos *ind)
{
	int	total;
	int	i;

	i = -1;
	total = 0;
	comm_list->argv = NULL;
	if (ind->red > 0)
		total = count_args(tokenlist, ind->pos, ind->red);
	else
		total = count_args(tokenlist, ind->pos, tokenlist->total);
	if (total == 0)
		return (0);
	comm_list->argv = ft_calloc(sizeof(char *), (total + 1));
	if (!comm_list->argv)
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	while (++i < total)
		comm_list->argv[i] = NULL;
	comm_list->argv[total] = NULL;
	return (success);
}

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
		return (msh_perror(NULL, MALLOC_ERR, parse_err), malloc_err);
	while (++i < total)
		comm_list->argv[i] = NULL;
	comm_list->argv[total] = NULL;
	return (success);
}

int	pass_list_pos(t_tokenlist *tokenlist, t_msh_data *msh_data, t_pos *ind)
{
	int	err;

	err = success;
	while ((int)ind->i < msh_data->command_count)
	{
		ind->red = count_next_cm(tokenlist, ind->pos);
		if (ind->red > 0 && looknxt(tokenlist, ind->pos)->type == HEREDOC)
			ind->red = count_next_cm(tokenlist, ind->pos + 1);
		err = convert_data(tokenlist, msh_data, ind);
		if (err != success)
			return (err);
		if (ind->red > 0 && find_type(tokenlist, ind->pos, find_type(tokenlist,
					ind->pos, ind->red, PIPE), HEREDOC) == -1)
			ind->pos = ind->red;
		else if (ind->red > 0 && find_type(tokenlist, ind->pos, ind->red,
				PIPE) != -1)
			ind->pos = find_type(tokenlist, ind->pos, ind->red, PIPE) + 1;
		else
			ind->pos = count_next_cm(tokenlist, ind->red);
		ind->i++;
	}
	return (err);
}

// start conversion by making lists of commands
int	pass_comm(t_tokenlist *tokenlist, t_msh_data *msh_data, t_pos *ind)
{
	int	err;

	err = success;
	if (count_lists(tokenlist) == -1)
		return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
	msh_data->command_count = count_lists(tokenlist);
	if (msh_data->command_count == syntax_err)
		return (syntax_err);
	msh_data->exec_data = ft_calloc(msh_data->command_count,
			sizeof(t_exec_data));
	if (!msh_data->exec_data)
		return (msh_perror(NULL, MALLOC_ERR, parse_err), malloc_err);
	err = pass_list_pos(tokenlist, msh_data, ind);
	return (err);
}


// convert the tokenlist to executable data
int	convert_data(t_tokenlist *tokenlist, t_msh_data *msh_data, t_pos *ind)
{
	t_exec_data	*comm_list;
	int			err;

	err = success;
	comm_list = NULL;
	comm_list = msh_data->exec_data + ind->i;
	err = make_cm_list(tokenlist, comm_list, ind);
	if (err != success)
		return (err);
	comm_list->redirections = NULL;
	if (find_type(tokenlist, ind->pos, ind->red, HEREDOC) != -1)
		err = set_heredoc(comm_list, tokenlist, ind);
	else
		err = fill_comm_list(comm_list, tokenlist, ind);
	return (err);
}

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
	t_pos *xpos
)
{
	int	total;
	int	i;

	i = -1;
	total = 0;
	comm_list->argv = NULL;
	if (xpos->red > 0)
		total = count_args(tokenlist, xpos->pos, xpos->red);
	else
		total = count_args(tokenlist, xpos->pos, tokenlist->total);
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

int pass_list_pos(t_tokenlist *tokenlist, t_msh_data *msh_data, t_pos *xp)
{
	int err; 

	err = success; 
	while ((int)xp->i < msh_data->command_count)
	{
		xp->red = count_next_cm(tokenlist, xp->pos);
		if (xp->red > 0 && looknxt(tokenlist, xp->pos)->type == HEREDOC)
			xp->red = count_next_cm(tokenlist, xp->pos + 1);
		convert_data(tokenlist, msh_data, xp);
		if (xp->red > 0 && find_type(tokenlist, xp->pos,
				find_type(tokenlist, xp->pos, xp->red, PIPE), HEREDOC) == -1)
			xp->pos = xp->red;
		else if (xp->red > 0 && find_type(tokenlist, xp->pos, xp->red,
				PIPE) != -1)
			xp->pos = find_type(tokenlist, xp->pos, xp->red, PIPE) + 1;
		else
			xp->pos = count_next_cm(tokenlist, xp->red);
		xp->i++;
	}
	return (err);
}


// start conversion by making lists of commands
int	pass_comm(t_tokenlist *tokenlist, t_msh_data *msh_data, t_pos *xp)
{
	if (count_lists(tokenlist) == -1)
		return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
	msh_data->command_count = count_lists(tokenlist);
	if (msh_data->command_count == syntax_err) // check if we even need error check here for this
		return (syntax_err);
	msh_data->exec_data = ft_calloc(msh_data->command_count,
			sizeof(t_exec_data));
	if (!msh_data->exec_data)
		return (msh_perror(NULL, MALLOC_ERR, parse_err), malloc_err);
	pass_list_pos(tokenlist, msh_data, xp);
	return (success);
}

//	p_printf("\nCONVERT DATA:\n Pos = %d\n Pos_red = %d\n", pos, pos_red);
//	p_printf("Next position = %d\n", count_next_cm(tokenlist, pos));
// convert the tokenlist to executable data
int	convert_data(t_tokenlist *tokenlist, t_msh_data *msh_data, t_pos *xpos)
{
	t_exec_data	*comm_list;
	int err;

	err = success;
	comm_list = NULL;
	comm_list = msh_data->exec_data + xpos->i;
	err = make_cm_list(tokenlist, comm_list, xpos);
	if (err != success)
		return (err);
	comm_list->redirections = NULL;
	if (find_type(tokenlist, xpos->pos, xpos->red, HEREDOC) != -1)
		err = set_heredoc(comm_list, tokenlist, xpos);
	else 
		err = fill_comm_list(comm_list, tokenlist, xpos);
	return (err);
}

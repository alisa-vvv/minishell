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
			return (-1);
		i++;
	}
	return (count);
}

// count until the next pos that is a command
int	count_next_cm(t_tokenlist *tokenlist, int pos)
{
	int		redir;
	size_t	i;
	t_token	*check_token;

	i = pos + 1;
	redir = 0;
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

// p_printf("TOTAL = %d\n", total);
//	p_printf("POS = %d and POS_RED = %d\n", pos, pos_red);
// make an empty execdata
int	make_cm_list(t_tokenlist *tokenlist,
					t_exec_data *comm_list,
					t_pos *xpos)
{
	int	total;
	int	i;

	i = -1;
	total = 0;
	if (!comm_list)
		return (write(1, "No command list\n", 16));
	comm_list->argv = NULL;
	if (xpos->red > 0)
		total = count_args(tokenlist, xpos->pos, xpos->red);
	else
		total = count_args(tokenlist, xpos->pos, tokenlist->total);
	if (total == 0)
		return (0);
	comm_list->argv = malloc(sizeof(char *) * (total + 1));
	if (!comm_list->argv)
		return (write(1, MALLOC_ERR, 15));
	while (++i < total)
		comm_list->argv[i] = NULL;
	comm_list->argv[total] = NULL;
	return (0);
}

// start conversion by making lists of commands
int	pass_comm(t_tokenlist *tokenlist, t_msh_data *msh_data, t_pos *xp)
{
	if (count_lists(tokenlist) == -1)
		return (write(1, "No lists counted\n", 17));
	msh_data->command_count = count_lists(tokenlist);
	msh_data->exec_data = ft_calloc(msh_data->command_count,
			sizeof(t_exec_data));
	if (!msh_data->exec_data)
		return (write(2, MALLOC_ERR, 15));
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
	return (0);
}

//	p_printf("\nCONVERT DATA:\n Pos = %d\n Pos_red = %d\n", pos, pos_red);
//	p_printf("Next position = %d\n", count_next_cm(tokenlist, pos));
// convert the tokenlist to executable data
int	convert_data(t_tokenlist *tokenlist, t_msh_data *msh_data, t_pos *xpos)
{
	t_exec_data	*comm_list;

	comm_list = NULL;
	comm_list = msh_data->exec_data + xpos->i;
	if (make_cm_list(tokenlist, comm_list, xpos))
		return (write(1, "Command list failed\n", 20));
	comm_list->redirections = NULL;
	if (find_type(tokenlist, xpos->pos, xpos->red, HEREDOC) != -1)
		set_heredoc(comm_list, tokenlist, xpos->pos, xpos->red);
	else if (fill_comm_list(comm_list, tokenlist, xpos))
		return (write(1, "Fill list failed\n", 17));
	return (0);
}

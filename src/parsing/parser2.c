/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   parser2.c                                           :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/12/09 12:51:47 by adecheri       #+#    #+#                */
/*   Updated: 2025/12/09 12:51:49 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	check_spcases(t_tokenlist *tokenlist)
{
	t_token	*token;
	int		err;

	err = success;
	token = NULL;
	if (tokenlist->total < 2)
	{
		err = single_token(tokenlist);
		if (err != success)
			return (err);
	}
	else
		err = val_redir(tokenlist, 0, token);
	return (err);
}

int	prep_execdata(t_tokenlist *tlist, t_msh_data *msh_data)
{
	t_pos	*ind;
	int		err;

	err = success;
	ind = NULL;
	if (tlist)
	{
		//set_pipe_cm(tlist, 0);
		ind = ft_calloc(1, sizeof(t_pos));
		if (!ind)
			return (msh_perror(NULL, MALLOC_ERR, parse_err), malloc_err);
		msh_data->command_count = count_lists(tlist);
		if (msh_data->command_count == syntax_err)
			return (syntax_err);
		msh_data->exec_data = ft_calloc(msh_data->command_count,
				sizeof(t_exec_data));
		if (!msh_data->exec_data)
			return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
		err = pass_comm(tlist, msh_data);
	}
	free(ind);
	return (err);
}

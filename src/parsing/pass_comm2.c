/* ************************************************************************** */
/*                                                                            */
/*                                                            ::::::::        */
/*   pass_comm2.c                                            :+:    :+:       */
/*                                                          +:+               */
/*   By: avaliull <avaliull@student.codam.nl>              +#+                */
/*                                                        +#+                 */
/*   Created: 2025/12/11 19:07:39 by avaliull            #+#    #+#           */
/*   Updated: 2025/12/11 19:12:24 by avaliull            ########   odam.nl   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

int	reallocate_argv(
	t_msh_data *msh_data,
	size_t argv_mem,
	size_t argv_i,
	size_t exdata_i
)
{
	char	**new_argv;
	int		i;

	if (argv_i == argv_mem - 1)
	{
		argv_mem += argv_mem / 2;
		new_argv = ft_calloc(argv_mem, sizeof (char *));
		i = -1;
		while (msh_data->exec_data[exdata_i].argv[++i])
		{
			new_argv[i] = ft_strdup(msh_data->exec_data[exdata_i].argv[i]);
			if (!new_argv[i])
			{
				free_2d_arr((void **) new_argv);
				return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
			}
		}
		free(msh_data->exec_data[exdata_i].argv);
		msh_data->exec_data[exdata_i].argv = new_argv;
	}
	return (success);
}

void	new_redir_elem(
	t_redir_list **first,
	t_redir_list *redir_node
)
{
	t_redir_list	*cur_node;

	redir_node->next = NULL;
	if (*first != NULL)
	{
		cur_node = *first;
		while (cur_node->next != NULL)
			cur_node = cur_node->next;
		cur_node->next = redir_node;
	}
	else
		*first = redir_node;
}

bool legit_token(t_token *cur_token)
{
	if (cur_token->type == COMMAND || cur_token->type == STRING
		|| cur_token->type == PARAMETER || cur_token->type == DOUBLE_Q
		|| cur_token->type == SINGLE_Q )
		return (true);
	return (false);
}

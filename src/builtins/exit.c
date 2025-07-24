/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   exit.c                                            :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/07/18 19:25:05 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/18 20:18:03 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

void	minishell_exit(
	t_exec_data *command,
	t_minishell_data *const minishell_data
)
{
	free_and_close_exec_data(command);
	free_2d_arr((void **) minishell_data->env);
	exit (0);
}

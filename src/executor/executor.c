/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   executor.c                                        :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/03 15:24:57 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/03 18:05:31 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "executor.h"
#include <stdio.h>
#include <errno.h>

t_exec_data	*test_get_dummy_exec_data()
{
	t_exec_data	*exec_data;

	exec_data = ft_calloc(3, sizeof(t_exec_data *));
	exec_data[0].command = ft_calloc(3, sizeof(char *));
	exec_data[0].command[0] = ft_strdup("cat");
	exec_data[0].command[1] = ft_strdup("-b");
	exec_data[0].input_type = custom_fd;
	exec_data[0].output_type = pipe_write;
	exec_data[0].in_filename = ft_strdup("infile");
	exec_data[1].command = ft_calloc(3, sizeof(char *));
	exec_data[1].command[0] = ft_strdup("cat");
	exec_data[1].command[1] = ft_strdup("-e");
	exec_data[1].input_type = pipe_read;
	exec_data[1].output_type = custom_fd;
	exec_data[1].in_filename = ft_strdup("outfile");
	return (exec_data);
}

int	executor(t_exec_data *exec_data, int dummy_minishell_struct)
{
	return (1);
}

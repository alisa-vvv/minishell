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
#include <fcntl.h>

t_exec_data	*test_get_dummy_exec_data()
{
	t_exec_data	*exec_data;

	exec_data = ft_calloc(3, sizeof(t_exec_data));
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

void	setup_in(t_exec_data *element, int *in_fd)
{
	if (element->input_type == custom_fd)
		*in_fd = open(element->in_filename, O_RDONLY);
	else if (element->input_type == std_in)
		*in_fd = STDIN_FILENO;
}

void	setup_out(t_exec_data *element, int *out_fd)
{
	if (element->output_type == custom_fd
		&& element->redirect_type == trunc)
		*out_fd = open(element->out_filename, O_WRONLY | O_CREAT | O_TRUNC);
	else if (element->output_type == custom_fd
		&& element->redirect_type == append)
		*out_fd = open(element->out_filename, O_WRONLY | O_CREAT | O_APPEND);
	else if (element->output_type == std_out)
		*out_fd = STDOUT_FILENO;
	else if (element->output_type == std_err)
		*out_fd = STDERR_FILENO;
}

int	setup_io_chain(t_exec_data *element, int (*io)[2], int i)
{
	int	err_check;

	err_check = 0;
	if (element->input_type == pipe_read)
		io[i][0] = io[i - 1][0];
	else
		setup_in(element, io[0]);
	if (io[i][0] < 0)
	{
		// ADD ERROR MANAGEMENT!!
		printf("PLACEHOLDER, THIS SHOULD ERROR\n");
	}

	if (element->output_type == pipe_write)
		err_check = pipe(io[i]);
	else
		setup_out(element, io[1]);
	if (io[1] < 0)
	{
		// ADD ERROR MANAGEMENT!!
		printf("PLACEHOLDER, THIS SHOULD ERROR\n");
	}
	return (err_check);
}

// I think I need to just iteratively setup pipes in the parent process
// then the next iteration would actually split the process into children since now we have pipe pairs
// and execution can start from 0 to N
// and if there's no pipes then it jsut uses whatever the in/out fd value is
// doest that regardless I think?
// probably special casse for heredoc but would need to research
// setup a union for nicer pipe management!
//
int	executor(t_exec_data *exec_data, int dummy_minishell_struct)
{
	int	i;
	int	(*io_chain)[2];

	i = -1;
	io_chain = ft_calloc(exec_data->len, sizeof (int[2]));
	if (!io_chain)
	{
		// ADD ERROR MANAGEMENT!!
		printf("PLACEHOLDER, THIS SHOULD ERROR\n");
	}
	while (++i < exec_data->len)
		setup_io_chain(&exec_data[i], io_chain, i);
	return (1);
}

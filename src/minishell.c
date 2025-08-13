/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   minishell.c                                       :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/05/31 15:31:56 by avaliull     #+#    #+#                  */
/*   Updated: 2025/08/06 15:42:16 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_testfuncs.h"
#include <assert.h>

#include <stdlib.h>
#include <errno.h>
#include "minishell.h"
#include "executor.h"
#include "minishell_env.h"
#include <unistd.h>

static void	reset_minishell_data(
	t_minishell_data *minishell_data
)
{
	minishell_data->exec_data = NULL;
	minishell_data->last_pipeline_return = 0;
	minishell_data->command_count = 0;
}

static void	setup_minishell_data(
	t_minishell_data *minishell_data
)
{
	reset_minishell_data(minishell_data);
	minishell_data->env = clone_env(&minishell_data->env_var_count, &minishell_data->env_mem);
	if (!minishell_data->env)
		exit(errno);
}

int	main(void)
{
	char				*read_line;
	int					is_open = true;
	int					err_check;
	t_minishell_data	minishell_data;
	t_exec_data			*exec_data;

	// NOTE: THE TEST_len VALUE BEING WRONG CAN CAUSE LEAKS. THIS IS NOT AN ISSUE CAUSE IT'S A TEST.
	// MAKE SURE THAT IT'S EQUAL TO THE AMOUNT OF COMMANDS ACTUALLY BEING TESTED.
	// OTHERWISE OUT OF BOUNDS ERRORS HAPPEN.
	setup_minishell_data(&minishell_data);
	int	TEST_len = 3;
	while (is_open != 0)
	{
		rl_catch_signals = false;
		handle_signals_interactive();
		read_line = readline("minishell$ ");
		if (!read_line)
			clean_exit(&minishell_data, NULL, EXIT_SUCCESS);
		// on ctrl-D, an EOT is sent to the tty.
		// readline returns a null pointer in this case.
		// this check is all thats needed to exit on ctrl-d in interactive mode
		if (g_msh_signal == SIGINT)
		{
			if (read_line)
				free(read_line);
			g_msh_signal = 0;
			continue ;
		}
		if (read_line)
			add_history(read_line);
		if (strcmp(read_line, "exit") == 0)
			is_open = false;
		else if (strcmp(read_line, "executor") == 0)
		{
			handle_signals_non_interactive();
			exec_data = test_get_dummy_exec_data(&minishell_data, TEST_len);
			err_check = executor(exec_data, TEST_len, &minishell_data);
		}
		else
			printf("We entered: %s\n", read_line);
		if (read_line)
			free(read_line);
		reset_minishell_data(&minishell_data); // this should ALAWAYS happen if we parse something.
	}
	clean_exit(&minishell_data, NULL, EXIT_SUCCESS);
	return (0);
}

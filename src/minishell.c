/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   msh.c                                       :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/05/31 15:31:56 by avaliull     #+#    #+#                  */
/*   Updated: 2025/08/06 15:42:16 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_testfuncs.h"
#include <stdlib.h>
#include "minishell.h"
#include "parser.h"
#include "executor.h"
#include <signal.h>
#include <string.h>
#include <stdio.h> /*	readline	*/
#include <readline/readline.h> // Needed for rl_catch_signals
#include <readline/history.h>

static int	setup_msh_data(
	t_msh_data *msh_data,
	char *envp[]
)
{
	int	err_check;

	err_check = success;
	msh_data->env_var_count = 0;
	msh_data->env_mem = 0;
	msh_data->is_parent = true;
	msh_data->last_pipeline_return = 0;
	msh_data->exec_data = NULL;
	msh_data->command_count = 0;
	msh_data->do_exit = false;
	msh_data->exit_code = 0;
	err_check = clone_env(msh_data, envp);
	if (err_check != success)
		return (err_check);
	msh_data->cur_dir = ft_calloc(sizeof(char), PATH_MAX + 1);
	if (!msh_data->cur_dir)
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	getcwd(msh_data->cur_dir, PATH_MAX);
	if (!getcwd(msh_data->cur_dir, PATH_MAX) || msh_data->cur_dir[0] == '\0')
		msh_data->cur_dir[0] = '/';
	return (success);
}

int	TEST_len = 0;


// NOTE: THE TEST_len VALUE BEING WRONG CAN CAUSE LEAKS. THIS IS NOT AN ISSUE
// CAUSE IT'S A TEST.
// MAKE SURE THAT IT'S EQUAL TO THE AMOUNT OF COMMANDS ACTUALLY BEING TESTED.
// OTHERWISE OUT OF BOUNDS ERRORS HAPPEN.
//
int	TEST_run_executor(
	t_msh_data *const msh_data
)
{
	msh_data->exec_data = test_get_dummy_exec_data(msh_data);
	return (executor(msh_data, TEST_len));
}

static void	post_execution_cleanup(
	t_msh_data *const msh_data,
	char *read_line,
	int err_check
)
{
	int	i;
	if (read_line)
		free(read_line);
	i = -1;
	if (msh_data->exec_data)
	{
		while (++i < msh_data->command_count && &msh_data->exec_data[i])
			free_and_close_exec_data(&msh_data->exec_data[i]);
		free(msh_data->exec_data);
		if (msh_data->is_parent == false)
		{
			if (err_check == child_heredoc || err_check == child_success
				|| err_check == success)
				clean_exit(msh_data, NULL, EXIT_SUCCESS, true);
			else if (err_check < 0)
				clean_exit(msh_data, NULL, EXIT_FAILURE, true);
		}
		msh_data->exec_data = NULL;
		msh_data->command_count = 0;
	}
}

volatile sig_atomic_t g_msh_signal = 0; 
int	main(int argc, char **argv, char *envp[])
{
	char		*read_line;
	int			err_check;
	t_msh_data	msh_data;

	(void) argc;
	(void) argv;
	rl_catch_signals = false;
	if (setup_msh_data(&msh_data, envp) != success)
		clean_exit(&msh_data, NULL, EXIT_SUCCESS, false);
	while (msh_data.do_exit == false && msh_data.is_parent == true)
	{
		handle_signals_interactive();
		read_line = readline("msh$ ");
		if (!read_line)
			clean_exit(&msh_data, NULL, EXIT_SUCCESS, false);
		add_history(read_line);
		if (strcmp(read_line, "executor") == 0) // for debug only! change maybe!
			err_check = TEST_run_executor(&msh_data);
		else
		{
			err_check = default_lexer(read_line, &msh_data);
			TEST_MINISHELLDATA(msh_data); // debug only!
			if (err_check == success)
				err_check = executor(&msh_data, msh_data.command_count);
		}
		post_execution_cleanup(&msh_data, read_line, err_check);
	}
	clean_exit(&msh_data, NULL, err_check, false);
}

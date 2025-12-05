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
#include <errno.h>
#include "minishell.h"
#include "executor.h"
#include "parser.h"
#include <string.h>
#include <stdio.h> /*	readline	*/
#include <readline/readline.h> // Needed for rl_catch_signals
#include <readline/history.h>

static void	reset_msh_data(
	t_msh_data *msh_data
)
{
	msh_data->exec_data = NULL;
	msh_data->command_count = 0;
}

static void	setup_msh_data(
	t_msh_data *msh_data,
	char *envp[]
)
{
	msh_data->env_var_count = 0;
	msh_data->env_mem = 0;
	msh_data->is_parent = true;
	msh_data->last_pipeline_return = 0;
	msh_data->exec_data = NULL;
	msh_data->command_count = 0;
	if (clone_env(msh_data, envp) != success)
		clean_exit(msh_data, NULL, EXIT_FAILURE, false); // check for errors here
	msh_data->cur_dir = ft_calloc(sizeof(char), PATH_MAX + 1);
	if (!msh_data->cur_dir)
		clean_exit(msh_data, NULL, errno, true); // add error message
	getcwd(msh_data->cur_dir, PATH_MAX); // ADD ERROR CHECKING!
	if (msh_data->cur_dir[0] == '\0')
		msh_data->cur_dir[0] = '/';
}

int	TEST_len = 0;

int	main(int argc, char **argv, char *envp[])
{
	char				*read_line;
	int					is_open = true;
	int					err_check;
	t_msh_data	msh_data;
//	t_exec_data			*exec_data;

	(void) argc;
	(void) argv;
	// NOTE: THE TEST_len VALUE BEING WRONG CAN CAUSE LEAKS. THIS IS NOT AN ISSUE CAUSE IT'S A TEST.
	// MAKE SURE THAT IT'S EQUAL TO THE AMOUNT OF COMMANDS ACTUALLY BEING TESTED.
	// OTHERWISE OUT OF BOUNDS ERRORS HAPPEN.
	//
	rl_catch_signals = false;
	setup_msh_data(&msh_data, envp);
	while (is_open != 0)
	{
		handle_signals_interactive();
		read_line = readline("msh$ ");
		if (!read_line)
			clean_exit(&msh_data, NULL, EXIT_SUCCESS, false); // check exit code interaction here
		int	TEST_lexer_return;
		if (read_line)
			add_history(read_line);
		if (strcmp(read_line, "exit") == 0)
			clean_exit(&msh_data, read_line, EXIT_SUCCESS, false);
		/*	Test Executor */
		else if (strcmp(read_line, "executor") == 0)
		{
			handle_signals_non_interactive();
			msh_data.exec_data = test_get_dummy_exec_data(&msh_data);
			err_check = executor(&msh_data, TEST_len);
		}
		/*	endof Test Executor*/
		/*	Test Lexer */
		else
		{
			TEST_lexer_return = default_lexer(read_line, &msh_data);
			TEST_MINISHELLDATA(msh_data);
			if (TEST_lexer_return != 0)
				write(2, "syntax error\n", 13);
			err_check = executor(&msh_data, msh_data.command_count);
		}
		/*	endof Test Lexer*/
		if (read_line)
			free(read_line);
		if (msh_data.is_parent == false) // make this better
		{
			if (err_check == child_heredoc || err_check == child_success
				|| err_check == success)
				clean_exit(&msh_data, NULL, EXIT_SUCCESS, true);
			else if (err_check < 0)
				clean_exit(&msh_data, NULL, EXIT_FAILURE, true);
		}
		// add error cases where bash exits (never questionmark?)
		reset_msh_data(&msh_data); // this should ALAWAYS happen if we parse something.
	}
	clean_exit(&msh_data, NULL, EXIT_SUCCESS, false);
}

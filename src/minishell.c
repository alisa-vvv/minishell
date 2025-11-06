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
#include "parser.h"
#include <string.h>
#include <stdio.h> /*	readline	*/
#include <readline/readline.h> // Needed for rl_catch_signals
#include <readline/history.h>
#include <unistd.h> /* isatty */

/* for waits	*/
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
/*	signals		*/
#include <signal.h>

static void	reset_minishell_data(
	t_minishell_data *minishell_data
)
{
	minishell_data->exec_data = NULL;
	minishell_data->command_count = 0;
}

static void	setup_minishell_data(
	t_minishell_data *minishell_data
)
{
	minishell_data->env = NULL;
	minishell_data->env_var_count = 0;
	minishell_data->env_mem = 0;
	minishell_data->last_pipeline_return = 0;
	minishell_data->exec_data = NULL;
	minishell_data->command_count = 0;
	minishell_data->env = clone_env(&minishell_data->env_var_count,
								 &minishell_data->env_mem);
	if (!minishell_data->env)
		exit(errno); // add error message
	minishell_data->cur_dir = ft_calloc(sizeof(char), PATH_MAX + 1);
	if (!minishell_data->cur_dir)
		clean_exit(minishell_data, NULL, errno, true); // add error message
	getcwd(minishell_data->cur_dir, PATH_MAX); // ADD ERROR CHECKING!
	printf("did we get it? %s\n", minishell_data->cur_dir);
	if (minishell_data->cur_dir[0] == '\0')
		minishell_data->cur_dir[0] = '/';
}

int	main(void)
{
	char				*read_line;
	int					is_open = true;
	int					err_check;
	t_minishell_data	minishell_data;
//	t_exec_data			*exec_data;

	// NOTE: THE TEST_len VALUE BEING WRONG CAN CAUSE LEAKS. THIS IS NOT AN ISSUE CAUSE IT'S A TEST.
	// MAKE SURE THAT IT'S EQUAL TO THE AMOUNT OF COMMANDS ACTUALLY BEING TESTED.
	// OTHERWISE OUT OF BOUNDS ERRORS HAPPEN.
	//
	rl_catch_signals = false;
	int	TEST_len = 2;
	printf("pid of parrent: %d\n", getpid());
	setup_minishell_data(&minishell_data);
	while (is_open != 0)
	{
		handle_signals_interactive();
		read_line = readline("minishell$ ");
		if (!read_line)
		{
			printf("pid of of null return: %d\n", getpid());
			printf("read_line return NULL!\n");
			clean_exit(&minishell_data, NULL, EXIT_SUCCESS, false);
		}
	//	if (g_msh_signal == SIGINT)
	//	{
	//		if (read_line)
	//			free(read_line);
	//		g_msh_signal = 0;
	//		continue ;
	//	}
		int	TEST_lexer_return;
		if (read_line)
			add_history(read_line);
		if (strcmp(read_line, "exit") == 0)
			clean_exit(&minishell_data, read_line, EXIT_SUCCESS, false);
		/*	Test Executor */
		else if (strcmp(read_line, "executor") == 0)
		{
			handle_signals_non_interactive();
			minishell_data.exec_data = test_get_dummy_exec_data(&minishell_data, TEST_len);
			err_check = executor(&minishell_data, TEST_len);
		}
		/*	endof Test Executor*/
		/*	Test Lexer */
		else
		{
			TEST_lexer_return = default_lexer(read_line, &minishell_data);
			printf("Parsing COMPLETE\n");
			TEST_MINISHELLDATA(minishell_data);
			if (TEST_lexer_return != 0)
				printf("PLACEHOLDER ERROR\n");
			err_check = executor(&minishell_data, minishell_data.command_count);
		}
		/*	endof Test Lexer*/
		if (read_line)
			free(read_line);
		if (err_check == child_heredoc || err_check == child_success)
			clean_exit(&minishell_data, NULL, EXIT_SUCCESS, true);
		else if (err_check < 0)
			clean_exit(&minishell_data, NULL, EXIT_FAILURE, true);
		reset_minishell_data(&minishell_data); // this should ALAWAYS happen if we parse something.
	}
	clean_exit(&minishell_data, NULL, EXIT_SUCCESS, false);
}

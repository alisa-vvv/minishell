/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   minishell.c                                       :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/05/31 15:31:56 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/18 20:02:01 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_testfuncs.h"

#include "libft.h"
#include "minishell.h"
#include "executor.h"
#include "minishell_env.h"
#include <unistd.h>

int	main(void)
{
	char				*read_line;
	int					is_open = true;
	t_minishell_data	minishell_data;
	t_exec_data			*exec_data;

	// NOTE: THE TEST_len VALUE BEING WRONG CAN CAUSE LEAKS. THIS IS NOT AN ISSUE CAUSE IT'S A TEST.
	// MAKE SURE THAT IT'S EQUAL TO THE AMOUNT OF COMMANDS ACTUALLY BEING TESTED.
	// OTHERWISE OUT OF BOUNDS ERRORS HAPPEN.
	int	TEST_len = 3;
	minishell_data.env = clone_env(&minishell_data.env_var_count, &minishell_data.env_mem);
	if (!minishell_data.env)
	{
		printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
		return (EXIT_FAILURE);
	}
	while (is_open != 0)
	{
		read_line = readline("minishell ");
		if (!read_line)
			printf("what happens?\n");
		else if (strcmp(read_line, "clear") == 0)
			rl_clear_history();
		else if (strcmp(read_line, "exit") == 0)
			is_open = false;
		else if (strcmp(read_line, "executor") == 0)
		{
			exec_data = test_get_dummy_exec_data(TEST_len);
			executor(exec_data, TEST_len, &minishell_data);
		}
		else
			printf("We entered: %s\n", read_line);
		add_history(read_line);
		if (read_line)
			free(read_line);
	}
	clean_exit(NULL, &minishell_data, NULL, EXIT_SUCCESS);
	return (0);
}

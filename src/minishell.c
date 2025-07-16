/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   minishell.c                                       :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/05/31 15:31:56 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/15 20:14:36 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "executor.h"
#include "parser.h"
#include <string.h>
#include <stdio.h> /*	readline	*/
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h> /* isatty */
/* for waits	*/
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
/*	signals		*/
#include <signal.h>

int	main(void)
{
	char				*read_line;
	int					is_open = true;
	t_minishell_data	minishell_data;

	int	TEST_len = 3;
	minishell_data.environment = clone_environment(&minishell_data.env_var_count, &minishell_data.env_mem);
	if (!minishell_data.environment)
	{
		printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
		return (EXIT_FAILURE);
	}
	while (is_open != 0)
	{
		read_line = readline("minishell ");
		if (strcmp(read_line, "clear") == 0)
			clear_history();
		else if (strcmp(read_line, "clear") == 0)
			rl_clear_history();
		else if (strcmp(read_line, "exit") == 0)
			is_open = false;
		else if (strcmp(read_line, "executor") == 0)
			executor(test_get_dummy_exec_data(TEST_len), TEST_len, &minishell_data);
		else if (!read_line)
			printf("what happens?\n");
		//else
		//{
		//	default_lexer(read_line);
		//	printf("We entered: %s\n", read_line);
		//}
		add_history(read_line);
	}
	free_2d_arr((void **) minishell_data.environment);
	return (0);
}

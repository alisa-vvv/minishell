/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   minishell.c                                       :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/05/31 15:31:56 by avaliull     #+#    #+#                  */
/*   Updated: 2025/05/31 16:28:00 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

//#include "minishell.h"
#include <stdio.h> /*	readline	*/
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h> /* isatty */

int	main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	char	*read_line;
	//printf("We entered: %s\n", read_line);
	//rl_on_new_line();
	int	is_open = 1;
	while (is_open != 0)
	{
		read_line = readline("minishell ");
		if (strcmp(read_line, "clear") == 0)
			rl_clear_history();
		else if (strcmp(read_line, "exit") == 0)
			is_open = 0;
		else if (strcmp(read_line, "edit") == 0)
		{
			rl_replace_line("edit", 0);
			read_line = "edit";
		}
		else if (strcmp(read_line, "replace") == 0)
		{
			rl_replace_line("edit", 0);
			read_line = "edit";
		}
		else
			printf("We entered: %s\n", read_line);
		add_history(read_line);
	}
	return (1);
}

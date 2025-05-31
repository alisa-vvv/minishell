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

#include "minishell.h"
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
	char	*read_line;
	int	is_open = 1;

	while (is_open != 0)
	{
		read_line = readline("minishell ");
		if (strcmp(read_line, "clear") == 0)
			rl_clear_history();
		else if (strcmp(read_line, "exit") == 0)
			is_open = 0;
		else
			printf("We entered: %s\n", read_line);
		add_history(read_line);
	}
	return (0);
}

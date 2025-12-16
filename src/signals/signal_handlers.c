/* ************************************************************************** */
/*                                                                            */
/*                                                            ::::::::        */
/*   signal_handlers.c                                       :+:    :+:       */
/*                                                          +:+               */
/*   By: avaliull <avaliull@student.codam.nl>              +#+                */
/*                                                        +#+                 */
/*   Created: 2025/12/16 19:17:46 by avaliull            #+#    #+#           */
/*   Updated: 2025/12/16 19:17:48 by avaliull            ########   odam.nl   */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <unistd.h>
#include "minishell.h"

void	sigint_handler_interactive(
	int sig
)
{
	(void) sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sigquit_handler_non_interactive(
	int sig
)
{
	(void) sig;
	if (g_msh_signal != SIGQUIT)
	{
		g_msh_signal = SIGQUIT;
		kill(0, SIGQUIT);
	}
	else
	{
		g_msh_signal = 0;
		write(STDOUT_FILENO, "\n", 1);
	}
}

void	sigint_handler_non_interactive(
	int sig
)
{
	(void) sig;
	if (g_msh_signal != SIGINT)
	{
		g_msh_signal = SIGINT;
		kill(0, SIGINT);
	}
	else
	{
		g_msh_signal = 0;
		write(STDOUT_FILENO, "\n", 1);
	}
}

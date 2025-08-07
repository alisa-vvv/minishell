/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   signal_handlers.c                                 :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/08/06 15:16:52 by avaliull     #+#    #+#                  */
/*   Updated: 2025/08/06 15:36:55 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <stddef.h>
#include <signal.h>
#include <unistd.h>
#include <readline/readline.h>

void	sigquit_handler(
	int sig
)
{
	g_msh_signal = SIGQUIT;
}
void	sigign_handler(
	int sig
)
{
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_msh_signal = SIGINT;
	// add function that sends signals to hcildren?
	// or it should be separate
}

void	handle_signals_interactive(
	void
)
{
	struct sigaction	handle_sigign;
	struct sigaction	handle_sigquit;

	sigemptyset(&handle_sigign.sa_mask);
	handle_sigign.sa_handler = sigign_handler;
	handle_sigign.sa_flags = 0;
	sigaction(SIGINT, &handle_sigign, NULL);
	sigemptyset(&handle_sigquit.sa_mask);
	handle_sigquit.sa_handler = SIG_IGN;
	handle_sigquit.sa_flags = 0;
	sigaction(SIGQUIT, &handle_sigquit, NULL);
}

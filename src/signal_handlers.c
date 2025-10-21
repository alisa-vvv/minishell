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

#define _GNU_SOURCE
#include <minishell.h>
#include <stddef.h>
#include <signal.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

void	sigint_handler_interactive(
)
{
	write(STDOUT_FILENO, "\n", 1);
	// rl_replace_line("", 0); // Commented out - not available on all systems
	rl_on_new_line();
	rl_redisplay();
	g_msh_signal = SIGINT;
}

void	sigint_handler_non_interactive(
)
{
	kill(0, SIGINT);
	sigint_handler_interactive();
}

void	handle_signals_child_process(
	void
)
{
	struct sigaction	handle_sigint;

	sigemptyset(&handle_sigint.sa_mask);
	handle_sigint.sa_handler = SIG_DFL;
	handle_sigint.sa_flags = 0;
	sigaction(SIGINT, &handle_sigint, NULL);
}

void	handle_signals_non_interactive(
	void
)
{
	struct sigaction	handle_sigquit;

	sigemptyset(&handle_sigquit.sa_mask);
	handle_sigquit.sa_handler = SIG_DFL;
	handle_sigquit.sa_flags = 0;
	sigaction(SIGQUIT, &handle_sigquit, NULL);
}

void	handle_signals_interactive(
	void
)
{
	struct sigaction	handle_sigint;
	struct sigaction	handle_sigquit;

	sigemptyset(&handle_sigint.sa_mask);
	sigaddset(&handle_sigint.sa_mask, SIGINT);
	sigaddset(&handle_sigint.sa_mask, SIGQUIT);
	handle_sigint.sa_handler = sigint_handler_interactive;
	handle_sigint.sa_flags = 0;
	sigaction(SIGINT, &handle_sigint, NULL);

	sigemptyset(&handle_sigquit.sa_mask);
	sigaddset(&handle_sigquit.sa_mask, SIGINT);
	sigaddset(&handle_sigquit.sa_mask, SIGQUIT);
	handle_sigquit.sa_handler = SIG_IGN;
	handle_sigquit.sa_flags = 0;
	sigaction(SIGQUIT, &handle_sigquit, NULL);
}

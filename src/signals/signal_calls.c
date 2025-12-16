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

#include "minishell.h"
#include <stddef.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

//volatile sig_atomic_t g_msh_signal = 0; 

void	sigint_handler_interactive(
)
{
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sigquit_handler_non_interactive(
)
{
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
)
{
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

void	handle_signals_child_process(
	void
)
{
	struct sigaction	handle_sigint;
	struct sigaction	handle_sigquit;

	sigemptyset(&handle_sigint.sa_mask);
	handle_sigint.sa_handler = SIG_DFL;
	handle_sigint.sa_flags = 0;
	sigaction(SIGINT, &handle_sigint, NULL);
	sigemptyset(&handle_sigquit.sa_mask);
	handle_sigquit.sa_handler = SIG_DFL;
	handle_sigquit.sa_flags = 0;
	sigaction(SIGQUIT, &handle_sigquit, NULL);
}

//could make this sighandler to handle all signals when in child process
//-->global variable to sig what handler to use
void	handle_signals_non_interactive(
	void
)
{
	struct sigaction	handle_sigquit;
	struct sigaction	handle_sigint;

	sigemptyset(&handle_sigquit.sa_mask);
	handle_sigquit.sa_handler = sigquit_handler_non_interactive;
	handle_sigquit.sa_flags = 0;
	sigaction(SIGQUIT, &handle_sigquit, NULL);
	sigemptyset(&handle_sigint.sa_mask);
	handle_sigint.sa_handler = sigint_handler_non_interactive;
	handle_sigint.sa_flags = 0;
	sigaction(SIGINT, &handle_sigint, NULL);
}

void	ignore_signals(
	void
)
{
	struct sigaction	handle_sigint;
	struct sigaction	handle_sigquit;

	sigemptyset(&handle_sigint.sa_mask);
	sigaddset(&handle_sigint.sa_mask, SIGINT);
	handle_sigint.sa_handler = SIG_IGN;
	handle_sigint.sa_flags = 0;
	sigaction(SIGINT, &handle_sigint, NULL);
	sigemptyset(&handle_sigquit.sa_mask);
	sigaddset(&handle_sigquit.sa_mask, SIGQUIT);
	handle_sigquit.sa_handler = SIG_IGN;
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
	handle_sigint.sa_handler = sigint_handler_interactive;
	handle_sigint.sa_flags = 0;
	sigaction(SIGINT, &handle_sigint, NULL);
	sigemptyset(&handle_sigquit.sa_mask);
	sigaddset(&handle_sigquit.sa_mask, SIGQUIT);
	handle_sigquit.sa_handler = SIG_IGN;
	handle_sigquit.sa_flags = 0;
	sigaction(SIGQUIT, &handle_sigquit, NULL);
}

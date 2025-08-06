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

#include <stddef.h>
#include <signal.h>

void	handle_signals_interactive(
	void
)
{
	struct sigaction	ignore_signal;

	ignore_signal.sa_handler = SIG_IGN;
	sigemptyset(&ignore_signal.sa_mask);
	ignore_signal.sa_flags = 0;
	sigaction(SIGQUIT, &ignore_signal, NULL);
	sigaction(SIGINT, &ignore_signal, NULL);
}

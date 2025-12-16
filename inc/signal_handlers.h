/* ************************************************************************** */
/*                                                                            */
/*                                                            ::::::::        */
/*   msh_signals.h                                           :+:    :+:       */
/*                                                          +:+               */
/*   By: avaliull <avaliull@student.codam.nl>              +#+                */
/*                                                        +#+                 */
/*   Created: 2025/12/16 19:24:36 by avaliull            #+#    #+#           */
/*   Updated: 2025/12/16 19:24:45 by avaliull            ########   odam.nl   */
/*                                                                            */
/* ************************************************************************** */

void	sigint_handler_interactive(int sig);
void	sigquit_handler_non_interactive(int sig);
void	sigint_handler_non_interactive(int sig);

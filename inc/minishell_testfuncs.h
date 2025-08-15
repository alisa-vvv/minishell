/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   minishell_testfuncs.h                             :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/07/18 19:58:23 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/18 20:00:56 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_TEST_FUNCS_H
# define MINISHELL_TEST_FUNCS_H

#include "minishell.h"

static int	TEST_counter_01;

t_redir_list	*test_add_redirection(
	const t_redir_list *first,
	const t_redirect_type type,
	const int src,
	const char *dest,
	const char *heredoc_delim
);

t_exec_data	*test_get_dummy_exec_data(
	t_minishell_data	*minishell_data,
	int len
);

#endif

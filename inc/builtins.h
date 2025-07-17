/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   builtins.h                                        :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/07/15 19:19:54 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/15 20:19:07 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

#include "minishell.h"

int	exec_builtin(
	char *builtin_name,
	char **arguments,
	t_minishell_data *const minishell_data
);

int	minishell_echo(
	char **arguments
);
int	minishell_cd(
	const char *path,
	t_minishell_data *const minishell_data
);
int	minishell_pwd(
	void
);
int	minishell_env(
	t_minishell_data *const minishell_data
);
int	minishell_export(
	char *const *argv,
	t_minishell_data *minishell_data
);
int	minishell_unset(
	char **argv,
	t_minishell_data *minishell_data
);

#endif


/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   builtins.h                                        :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/07/15 19:19:54 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/18 20:17:54 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

#include "minishell.h"

int	exec_builtin(
	t_exec_data *const command,
	t_msh_data *const msh_data
);

int	msh_echo(
	char **arguments
);
int	msh_cd(
	char *const arg,
	t_msh_data *const msh_data
);
int	msh_pwd(
	t_msh_data *const msh_data
);
int	msh_env(
	t_msh_data *const msh_data
);
int	msh_export(
	char *const *argv,
	t_msh_data *const msh_data
);
int	msh_unset(
	char **argv,
	t_msh_data *msh_data
);
int	msh_exit(
	t_exec_data *command,
	t_msh_data *const msh_data
);

#endif


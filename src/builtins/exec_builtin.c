/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   exec_builtin.c                                    :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/19 15:47:26 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/18 20:18:05 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	exec_builtin(
	t_exec_data *const command,
	t_minishell_data *const minishell_data
)
{
	int			err_check;
	char		**builtin_args;

	builtin_args = &command->argv[1]; // weird, check this for being normal
	err_check = 0;
	if (command->builtin_name == builtin_echo)
		err_check = minishell_echo(builtin_args);
	else if (command->builtin_name == builtin_cd)
		err_check = minishell_cd(builtin_args[0], minishell_data);
	else if (command->builtin_name == builtin_pwd)
		err_check = minishell_pwd();
	else if (command->builtin_name == builtin_env)
		err_check = minishell_env(minishell_data);
	else if (command->builtin_name == builtin_export)
		err_check = minishell_export(builtin_args, minishell_data);
	else if (command->builtin_name == builtin_unset)
		err_check = minishell_unset(builtin_args, minishell_data);
	else if (command->builtin_name == builtin_exit)
		minishell_exit(command, minishell_data);
	return (err_check);
}

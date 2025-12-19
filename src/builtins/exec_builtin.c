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
	t_msh_data *const msh_data
)
{
	int			err_check;
	char		**builtin_args;

	builtin_args = &command->argv[1]; // weird, check this for being normal
	err_check = 0;
	if (command->builtin_name == builtin_echo)
		err_check = msh_echo(builtin_args);
	else if (command->builtin_name == builtin_cd)
		err_check = msh_cd(builtin_args[0], msh_data);
	else if (command->builtin_name == builtin_pwd)
		err_check = msh_pwd(msh_data);
	else if (command->builtin_name == builtin_env)
		err_check = msh_env(msh_data);
	else if (command->builtin_name == builtin_export)
		err_check = msh_export(builtin_args, msh_data);
	else if (command->builtin_name == builtin_unset)
		err_check = msh_unset(builtin_args, msh_data);
	else if (command->builtin_name == builtin_exit)
		err_check = msh_exit(command, msh_data);
	return (err_check);
}

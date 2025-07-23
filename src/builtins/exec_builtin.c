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
#include "libft.h"

// replace strncmps enum later based on what parsing does
//
// required:
// 1 - DONE, NEEDS ADJ. echo with option -n;
// 2. cd - with relative or absolute path
// 3 - DONE. pwd with no options
// 4. export with no options
// 5. unset with no options
// 6. env with no options or arguments
// 7. exit with no options
//
int	exec_builtin(
	const t_exec_data *const command,
	t_minishell_data *const minishell_data
)
{
	int			err_check;
	char		**builtin_args;

	builtin_args = &command->argv[1];
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

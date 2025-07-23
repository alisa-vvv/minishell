/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   exec_builtin.c                                    :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/19 15:47:26 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/15 19:29:59 by avaliull     ########   odam.nl          */
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
	char *builtin_name,
	char **arguments,
	t_minishell_data *const minishell_data
)
{
	int	err_check;

	err_check = 0;
	printf("arguments[0]: %s\n", arguments[0]);
	if (ft_strncmp(builtin_name, "echo", 5) == 0)
		err_check = minishell_echo(arguments);
	else if (ft_strncmp(builtin_name, "cd", 3) == 0)
		err_check = minishell_cd(arguments[0], minishell_data);
	else if (ft_strncmp(builtin_name, "pwd", 4) == 0)
		err_check = minishell_pwd();
	else if (ft_strncmp(builtin_name, "env", 4) == 0)
		err_check = minishell_env(minishell_data);
	else if (ft_strncmp(builtin_name, "export", 7) == 0)
		err_check = minishell_export(arguments, minishell_data);
	return (err_check);
}

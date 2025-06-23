/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   exec_builtin.c                                    :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/19 15:47:26 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/19 16:53:18 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
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
	char **arguments
)
{
	int	err_check;

	err_check = 0;
	printf("arguments[0]: %s\n", arguments[0]);
	if (ft_strncmp(builtin_name, "echo", 5) == 0)
		err_check = minishell_echo(arguments);
	else if (ft_strncmp(builtin_name, "cd", 3) == 0)
		err_check = minishell_cd(arguments[0]);
	else if (ft_strncmp(builtin_name, "pwd", 4) == 0)
		err_check = minishell_pwd();
	return (err_check);
}

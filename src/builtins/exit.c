/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   exit.c                                            :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/07/18 19:25:05 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/18 20:18:03 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

int	msh_exit(
	t_exec_data *command,
	t_msh_data *const msh_data
)
{
	char *const *const	args = &command->argv[1];
	int					exit_code;

	exit_code = 0;
	if (args[0] == NULL)
		clean_exit(msh_data, NULL, 0, false);
	if (args[1] != NULL)
	{
		msh_perror("exit: ", "too many arguments", msh_err);
		return (builtin_err);
	}
	exit_code = ft_atoi(args[0]);
	if (exit_code == 0 && args[0][0] != '0')
	{
		if ((args[0][0] == '-' || args[0][0] == '+') &&
				args[0][1] == '0')
			;
		else
		{
			msh_perror("exit: ", "numeric argument required", msh_err);
			clean_exit(msh_data, NULL, 2, false);
		}
	}
	clean_exit(msh_data, NULL, exit_code, false);
	return (-1);
}

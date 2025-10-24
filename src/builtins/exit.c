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

int	minishell_exit(
	t_exec_data *command,
	t_minishell_data *const minishell_data
)
{
	char *const *const	args = &command->argv[1];
	int					exit_code;

	exit_code = 0;
	if (args[0] == NULL)
		clean_exit(minishell_data, NULL, 0, false);
	else if (args[1] != NULL)
	{
		//should return and not exit
		// "bash: exit: too many arguments"
		printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
		return (1);
	}
	else
	{
		exit_code = ft_atoi(args[0]);
		printf("[n]: %d\n", exit_code);
		if (exit_code == 0 && args[0][0] != '0')
		{
			if ((args[0][0] == '-' || args[0][0] == '+') &&
					args[0][1] == '0')
				;
			else
			{
				//should return and exit, throw appropriate error
				// "bash: exit: numeric argument required"
				// also for whatever reason i think bash exits with 2 here (dumb?)
				printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
				clean_exit(minishell_data, NULL, exit_code, false);
			}
		}
	}
	clean_exit(minishell_data, NULL, exit_code, false);
	// should NEVER get here, something is COMPLETELY fucked if we are here
	return (-1);
}

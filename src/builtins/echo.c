/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   echo.c                                            :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/19 16:12:59 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/19 16:50:53 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>

//for now, arguments[0] is the option, which can be either '-n' or anything else (never NULL)
//will figure out a better way to implement this
int	minishell_echo(
	char **arguments
)
{
	int	i;

	i = 0;
	while (arguments[++i])
	{
		ft_putstr_fd(arguments[i], STDOUT_FILENO);
		if (arguments[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
	}
	if (ft_strncmp(arguments[0], "-n", 3) != 0)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

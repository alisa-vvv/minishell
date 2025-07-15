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
#include <stdbool.h>

int	minishell_echo(
	char **arguments
)
{
	int		i;
	bool	put_nl;

	i = 0;
	put_nl = true;
	while (arguments[++i])
	{
		if (ft_strncmp(arguments[i], "-n", 3) != 0)
			put_nl = false;
		else
		{
			ft_putstr_fd(arguments[i], STDOUT_FILENO);
			if (arguments[i + 1])
				ft_putchar_fd(' ', STDOUT_FILENO);
		}
	}
	if (put_nl == true)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

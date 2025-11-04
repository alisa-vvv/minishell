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

#include <stdio.h>

static bool	check_for_minus_n(
	const char *const first_argument
)
{
	int	i;

	if (ft_strncmp(first_argument, "-n", 2) == 0)
	{
		i = 2;
		while (first_argument[i] == 'n')
			i++;
		if (first_argument[i] == '\0')
			return (false);
		else
			i = 0;
	}
	return (true);
}

int	minishell_echo(
	char *const *const arguments
)
{
	int		i;
	bool	put_nl;

	put_nl = check_for_minus_n(arguments[0]);
	i = !put_nl;
	while (arguments[i])
	{
		ft_putstr_fd(arguments[i], STDOUT_FILENO);
		if (arguments[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;	
	}
	if (put_nl == true)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

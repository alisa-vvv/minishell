/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   cd.c                                              :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/21 16:29:42 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/21 16:43:58 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <unistd.h>

int	minishell_cd(
const char *path
)
{
	int	err_check;

	printf("path: %s\n", path);
	err_check = chdir(path);
	if (err_check < 0)
		perror_and_return("cd", MINISHELL_ERR, 1);
	return (0);
}

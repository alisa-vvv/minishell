/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   cd.c                                              :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/21 16:29:42 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/15 19:40:28 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "builtins.h"
#include <unistd.h>

#include <stdio.h>

//figure out if you actually want to record oldpwd
int	minishell_cd(
	const char *path,
	t_minishell_data *const minishell_data
)
{
	int			err_check;
	const char	*new_pwd = ft_strjoin("PWD=", path);

	if (!new_pwd)
		return (1);
	err_check = chdir(path);
	free((char *) new_pwd);
	if (err_check < 0)
		perror_and_return("cd", MINISHELL_ERR, 1);
	else
		minishell_export((char **) &new_pwd, minishell_data);
	return (0);
}

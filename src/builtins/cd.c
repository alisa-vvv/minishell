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
#include "minishell_env.h"
#include <unistd.h>

#include <stdio.h>

//figure out if you actually want to record oldpwd
int	minishell_cd(
	const char *path,
	t_minishell_data *const minishell_data
)
{
	int		err_check;
	int		old_pwd_index;
	char	*new_pwd;
//	char	*new_old_pwd;

	// look for OLD_PWD and PWD variables // probably don't actually need to look for OLD_PWD cause we always export it
	// if they exist, change values
	// otherwise export them
	//
	new_pwd = ft_strjoin("PWD=", path);
	if (!new_pwd)
		return (-1);
	if (err_check < 0)
	{
		free(new_pwd);
		perror_and_return("cd", MINISHELL_ERR, -1);
	}
	else
	{
		minishell_export(&new_pwd, minishell_data);
	}
	free(new_pwd);
	return (0);
}

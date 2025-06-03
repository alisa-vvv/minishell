/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   try_execve.c                                      :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/03 17:14:59 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/03 17:16:30 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "executor.h"
#include <stdio.h>
#include <errno.h>

char	**find_env_path(void)
{
	int		i;
	char	**path_arr;

	i = -1;
	path_arr = NULL;
	while (__environ[++i])
	{
		if (ft_strncmp(__environ[i], "PATH", 4) == 0)
			path_arr = ft_split(&__environ[i][5], ':');
	}
	if (!path_arr)
		return (NULL);
	return (path_arr);
}

static int	perror_return(char *err_msg)
{
	perror(err_msg);
	return (errno);
}

int	try_execve(const char **path, char *const argv[])
{
	char	*tmp_slash;
	char	*command_path;

	if (execve(argv[0], argv, __environ) != -1)
		return (0);
	tmp_slash = ft_strjoin("/", argv[0]);
	if (!tmp_slash)
		return (perror_return(MALLOC_ERR));
	while (path[0])
	{
		command_path = ft_strjoin(path[0], tmp_slash);
		if (!command_path)
			return (perror_return(MALLOC_ERR));
		if (execve(command_path, argv, __environ) == -1)
			free(command_path);
		else
		{
			free(command_path);
			free(tmp_slash);
			return (0);
		}
		path++;
	}
	free(tmp_slash);
	return (perror_return(argv[0]));
}

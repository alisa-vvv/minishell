/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   try_execve.c                                      :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/03 17:14:59 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/18 20:25:48 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "executor.h"
#include <stdio.h>

//aedit this to find p
const char	*const *find_env_path(
	char **env
)
{
	int		i;
	char	**path_arr;

	i = -1;
	path_arr = NULL;
	while (env[++i])
	{
		if (ft_strncmp(env[i], "PATH", 4) ==0)
		{
			path_arr = ft_split(&env[i][5], ':');
			if (!path_arr)
			{
				perror(MALLOC_ERR);
				return (NULL);
			}
			break ;
		}
	}
	if (!path_arr)
		return (NULL);
	return ((const char *const *) path_arr);
}

static int	command_not_found(
	char *command_name
)
{
	ft_putstr_fd(command_name, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	return (EXIT_FAILURE);
}

int	try_execve(
	char **env,
	char *const argv[]
)
{
	char				*tmp_slash;
	char				*command_path;
	const char *const 	*path = find_env_path(env);

	if (execve(argv[0], argv, env) != -1)
		return (0);
	tmp_slash = ft_strjoin("/", argv[0]);
	if (!tmp_slash)
		return (perror_and_return(MALLOC_ERR, LIBFUNC_ERR, EXIT_FAILURE));
	while (path)
	{
		command_path = ft_strjoin(*path, tmp_slash);
		if (!command_path)
			return (perror_and_return(MALLOC_ERR, LIBFUNC_ERR, EXIT_FAILURE));
		if (execve(command_path, argv, env) == -1)
			free(command_path);
		path++;
	}
	free(tmp_slash);
	return (command_not_found(argv[0]));
}

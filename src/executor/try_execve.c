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

const char	**find_env_path(void)
{
	int		i;
	char	**path_arr;
	char	*path_var;

	i = -1;
	path_arr = NULL;
	path_var = getenv("PATH");
	if (!path_var)
	{
		ft_putstr_fd(PATH_ERR, STDERR_FILENO);
		return (NULL);
	}
	path_arr = ft_split(path_var, ':');
	if (!path_arr)
	{
		perror(MALLOC_ERR);
		return (NULL);
	}
	return ((const char **) path_arr);
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
	const char **path,
	char *const argv[]
)
{
	char	*tmp_slash;
	char	*command_path;

	if (execve(argv[0], argv, __environ) != -1)
		return (0);
	tmp_slash = ft_strjoin("/", argv[0]);
	if (!tmp_slash)
		return (perror_and_return(MALLOC_ERR, LIBFUNC_ERR, EXIT_FAILURE));
	while (path[0])
	{
		command_path = ft_strjoin(path[0], tmp_slash);
		if (!command_path)
			return (perror_and_return(MALLOC_ERR, LIBFUNC_ERR, EXIT_FAILURE));
		if (execve(command_path, argv, __environ) == -1)
			free(command_path);
		path++;
	}
	free(tmp_slash);
	return (command_not_found(argv[0]));
}

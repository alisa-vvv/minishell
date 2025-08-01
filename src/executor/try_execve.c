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
#include "minishell_env.h"
#include "executor.h"
#include <stdio.h>

static const char	*const *split_path_var(
	char **env
)
{
	int		i;
	char	*path_value;
	char	**path_arr;

	i = -1;
	path_arr = NULL;
	path_value = env_var_get_value(env, "PATH");
	if (!path_value)
		return (NULL);
	path_arr = ft_split(path_value, ':');
	if (!path_arr)
	{
		perror(MALLOC_ERR);
		return (NULL);
	}
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
	const char *const 	*path = split_path_var(env);
	int					i;

	if (execve(argv[0], argv, env) != -1)
		return (0);
	tmp_slash = ft_strjoin("/", argv[0]);
	if (!tmp_slash)
		return (perror_and_return(NULL, MALLOC_ERR, extern_err, -1));
	i = -1;
	while (path[++i])
	{
		command_path = ft_strjoin(path[i], tmp_slash);
		if (!command_path)
			return (perror_and_return(NULL, MALLOC_ERR, extern_err, -1));
		if (execve(command_path, argv, env) == -1)
			free(command_path);
	}
	free(tmp_slash);
	free_2d_arr((void **) path);
	return (command_not_found(argv[0]));
}

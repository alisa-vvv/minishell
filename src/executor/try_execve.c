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
#include <errno.h>


static const char *const	*handle_no_path(
	void
)
{
	char **path_arr;

	path_arr = ft_calloc(2, sizeof(char *));
	path_arr[0] = ft_calloc(1, sizeof(char));
	path_arr[1] = NULL;
	return ((const char *const *) path_arr);
}

static const char *const	*split_path_var(
	char **env
)
{
	char	*path_value;
	char	**path_arr;
	int		err_check;
	int		existing;

	existing = 0;
	path_arr = NULL;
	path_value = NULL;
	err_check = env_var_get_value(env, "PATH", &path_value, &existing);
	if (err_check != success)
	{
		msh_perror(NULL, MALLOC_ERR, extern_err);
		return (NULL);
	}
	if (path_value == NULL)
		return (handle_no_path());
	path_arr = ft_split(path_value, ':');
	if (!path_arr)
	{
		free(path_value);
		msh_perror(NULL, MALLOC_ERR, extern_err);
		return (NULL);
	}
	free(path_value);
	return ((const char *const *) path_arr);
}

static int	error_loop(
	char *slash_arg,
	char *error_msg,
	int	error_code

)
{
	free(slash_arg);
	msh_perror(NULL, error_msg, extern_err);
	return (error_code);
}

static int	look_for_command(
	char **env,
	char *const argv[],
	const char *const *path
)
{
	int		i;
	char	*command_path;
	char	*slash_arg;

	i = -1;
	slash_arg = ft_strjoin("/", argv[0]);
	if (!slash_arg)
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	while (path[++i])
	{
		command_path = ft_strjoin(path[i], slash_arg);
		if (!command_path)
		{
			return (error_loop(slash_arg, MALLOC_ERR, malloc_err));
		}
		execve(command_path, argv, env);
		free(command_path);
		if (errno != ENOENT)
			return (error_loop(slash_arg, EXECVE_ERR, malloc_err));
	}
	free(slash_arg);
	return (no_command);
}

int	try_execve(
	char **env,
	char *const argv[]
)
{
	int					err_check;
	const char *const 	*path;

	if (argv == NULL || argv[0] == NULL)
		return (child_success);
	path = split_path_var(env);
	if (!path)
		return (malloc_err);
	execve(argv[0], argv, env);
	if (errno != ENOENT)
	{
		free_2d_arr((void **) path);
		return (msh_perror(NULL, EXECVE_ERR, extern_err), execve_err);
	}
	err_check = look_for_command(env, argv, path);
	free_2d_arr((void **) path);
	if (err_check == no_command)
	{
		ft_putstr_fd(argv[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	}
	return (err_check);
}

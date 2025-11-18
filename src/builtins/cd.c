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
#include "executor.h"
#include "builtins.h"
#include "minishell_env.h"
#include <unistd.h>

#include <stdio.h>

static void	free_variables(
	char **variables
)
{
	if (variables)
	{
		if (variables[0])
			free(variables[0]);
		if (variables[1])
			free(variables[1]);
		free(variables);
	}
}

static int	set_env_vars(
	t_minishell_data *const minishell_data,
	char *cwd,
	char *path
)
{
	int		err_check;
	char	**variables;

	variables = ft_calloc(3, sizeof (char *));
	if (!variables)
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err); // check return
	variables[0] = ft_strjoin("OLDPWD=", cwd);
	variables[1] = ft_strjoin("PWD=", path);
	if (!variables[0] || !variables[1])
	{
		free_variables(variables);
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err); // check return
	}
	err_check = minishell_export(variables, minishell_data);
	if (err_check < 0)
		return (err_check); // check return
	free_variables(variables);
	return (0);
}

static int find_target_path(
	char *const arg,
	t_minishell_data *const minishell_data,
	char *const cwd,
	char **path_pointer
)
{
	char	*arg_with_slash;
	char	*path;

	if (!arg || arg[0] == '\0')
	{
		path = env_var_get_value(minishell_data->env, "HOME");
		if (!path)
			return(msh_perror("cd", "HOME not set", exec_err), builtin_err);
	}
	else
	{
		arg_with_slash = ft_strjoin("/", arg);
		path = ft_strjoin(cwd, arg_with_slash);
		free(arg_with_slash);
		if (!path)
			return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	}
	if (access(path, F_OK))
	{
		free(path);
		return (msh_perror(NULL, "cd", extern_err), builtin_err);
	}
	*path_pointer = path;
	return (0);
}

// add case for when directory was removed
int	minishell_cd(
	char *const arg,
	t_minishell_data *const minishell_data
)
{
	int			err_check;
	char		*path;
	char *const	cwd = ft_calloc(PATH_MAX, sizeof(char));

	if (cwd == NULL)
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	path = NULL;
	getcwd(cwd, PATH_MAX);
	err_check = find_target_path(arg, minishell_data, cwd, &path);
	if (err_check != 0)
	{
		free(cwd);
		return (err_check);
	}
	err_check = chdir(path); // this SHOULD check for path length i think
	if (err_check != 0)
		return (msh_perror("cd: ", NULL, extern_err), -1); // check return
	else
		err_check = set_env_vars(minishell_data, cwd, path);
	free(minishell_data->cur_dir);
	minishell_data->cur_dir = path;
	free(cwd);
	return (err_check);
}

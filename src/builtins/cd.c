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
		perror_and_return(NULL, MALLOC_ERR, extern_err, -1);
	variables[0] = ft_strjoin("OLDPWD=", cwd);
	variables[1] = ft_strjoin("PWD=", path);
	if (!variables[0] || !variables[1])
	{
		free_variables(variables);
		perror_and_return(NULL, MALLOC_ERR, extern_err, -1);
	}
	err_check = minishell_export(variables, minishell_data);
	if (err_check < 0)
	{
		printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
		return (err_check);
	}
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
		{
			printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
			printf("what do we return in cases like this?\n");
			return(perror_and_return("cd: ", "HOME not set", exec_err, 1));
		}
	}
	else
	{
		// ADD A FUNCTION THAT ADDS SLASH AND JOINS. AAAAAAAA
		arg_with_slash = ft_strjoin("/", arg);
		path = ft_strjoin(cwd, arg_with_slash);
		free(arg_with_slash);
		if (!path)
			return (perror_and_return(NULL, MALLOC_ERR, extern_err, -1));
	}
	if (access(path, F_OK))
	{
		free(path);
		printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
		return (perror_and_return(NULL, "cd: ", extern_err, -1));
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
//
//	printf("\n\n\nbefore:\n\n\n");
//	minishell_pwd(minishell_data);
//
	path = NULL;
	//add check for if path does not exist
	getcwd(cwd, PATH_MAX);
	err_check = find_target_path(arg, minishell_data, cwd, &path);
	if (err_check != 0)
	{
		free(cwd);
		return (err_check);
	}
	err_check = chdir(path);
	if (err_check != 0)
	{
		printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
		perror_and_return("cd: ", NULL, extern_err, -1);
	}
	else
		err_check = set_env_vars(minishell_data, cwd, path);
	ft_bzero(minishell_data->cur_dir, PATH_MAX);
	getcwd(minishell_data->cur_dir, PATH_MAX);
	free(path);
	free(cwd);
//
//	printf("\n\n\nafter:\n\n\n");
//	minishell_pwd(minishell_data);
//	printf("\n\n\nvars:\n\n\n");
//	minishell_env(minishell_data);
//
	return (err_check);
}

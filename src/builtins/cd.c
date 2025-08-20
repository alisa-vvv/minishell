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

static void cd_free(
	char *path,
	char *new_wd,
	char *new_old_wd,
	char *cur_wd_value
)
{
	if (path)
		free(path);
	if (new_wd)
		free(new_wd);
	if (new_old_wd)
		free(new_old_wd);
	if (cur_wd_value)
		free(cur_wd_value);
}

static int	record_dirs(
	char *path,
	char **new_wd,
	char **new_old_wd,
	char *cur_wd_value
)
{
	*new_old_wd = ft_strjoin("OLDPWD=", cur_wd_value);
	*new_wd = ft_strjoin("PWD=", path);
	if (!new_wd || !new_old_wd)
	{
		cd_free(path, *new_wd, *new_old_wd, cur_wd_value);
		printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
		return (-1);
	}
	return (0);
}

static int	set_env_vars(
	t_minishell_data *const minishell_data,
	char *const new_wd,
	char *const new_old_wd,
	char *const cur_wd
)
{
	int	err_check;

	printf("new_wd: %s\n", new_wd);
	printf("new_old_wd: %s\n", new_old_wd);
	err_check = minishell_export(&new_wd, minishell_data);
	if (err_check < 0)
	{
		printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
		return (err_check);
	}
	err_check = minishell_export(&new_old_wd, minishell_data);
	if (err_check < 0)
	{
		printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
		return (err_check);
	}
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
			perror_and_return("cd: ", "HOME not set", exec_err, 1);
		}
	}
	else
	{
		// ADD A FUNCTION THAT ADDS SLASH AND JOINS. AAAAAAAA
		arg_with_slash = ft_strjoin("/", arg);
		path = ft_strjoin(cwd, arg_with_slash);
		free(arg_with_slash);
		if (!path)
			perror_and_return(NULL, MALLOC_ERR, extern_err, -1);
	}
	if (access(path, F_OK))
	{
		free(path);
		printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
		perror_and_return(NULL, FD_ERR, extern_err, -1);
	}
	*path_pointer = path;
	return (0);
}

int	minishell_cd(
	char *const arg,
	t_minishell_data *const minishell_data
)
{
	int		err_check;
	char	*path;
	char	*new_wd;
	char	*new_old_wd;
	char	*const cwd = ft_calloc(PATH_MAX, sizeof(char));

	// look for OLD_PWD and PWD variables // probably don't actually need to look for OLD_PWD cause we always export it
	// if they exist, change values
	// otherwise export them
	//
	new_wd = NULL;
	new_old_wd = NULL;
	path = NULL;
	getcwd(cwd, PATH_MAX);

	printf("\n\n\nbefore:\n\n\n");
	minishell_pwd();

	err_check = find_target_path(arg, minishell_data, cwd, &path);
	if (err_check != 0)
	{
		free(cwd);
		return (err_check);
	}
	err_check = record_dirs(path, &new_wd, &new_old_wd, cwd);
	if (err_check < 0)
	{
		printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
		return (err_check);
	}
	err_check = chdir(path);
	if (err_check != 0)
	{
		printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
		perror_and_return("cd: ", NULL, extern_err, -1);
	}
	else
		err_check = set_env_vars(minishell_data, new_wd, new_old_wd, cwd);
	cd_free(path, new_wd, new_old_wd, cwd);

	printf("\n\n\nafter:\n\n\n");
	minishell_pwd();
	//printf("\n\n\nvars:\n\n\n");
	//minishell_env(minishell_data);

	return (err_check);
}

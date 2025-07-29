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
#include <unistd.h>

#include <stdio.h>

//figure out if you actually want to record oldpwd
static void cd_free(
	char *path,
	char *new_pwd,
	char *new_old_pwd,
	char *cur_pwd_value
)
{
	if (path)
		free(path);
	if (new_pwd)
		free(new_pwd);
	if (new_old_pwd)
		free(new_old_pwd);
	if (cur_pwd_value)
		free(cur_pwd_value);
}
static int	record_dirs(
	char *path,
	char **new_pwd,
	char **new_old_pwd,
	char *cur_pwd_value
)
{
	*new_old_pwd = ft_strjoin("OLDPWD=", cur_pwd_value);
	*new_pwd = ft_strjoin("PWD=", path);
	if (!new_pwd || !new_old_pwd)
	{
		cd_free(path, *new_pwd, *new_old_pwd, cur_pwd_value);
		printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
		return (-1);
	}
	return (0);
}

static int	set_env_vars(
	t_minishell_data *const minishell_data,
	char *const new_pwd,
	char *const new_old_pwd,
	char *const cur_pwd
)
{
	int	err_check;

	printf("new_pwd: %s\n", new_pwd);
	printf("new_old_pwd: %s\n", new_old_pwd);
	err_check = minishell_export(&new_pwd, minishell_data);
	if (err_check < 0)
	{
		printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
		return (err_check);
	}
	err_check = minishell_export(&new_old_pwd, minishell_data);
	if (err_check < 0)
	{
		printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
		return (err_check);
	}
	return (0);
}

int	minishell_cd(
	char *const arg,
	t_minishell_data *const minishell_data
)
{
	int		err_check;
	char	*path;
	char	*new_pwd;
	char	*new_old_pwd;
	char	*const cur_pwd = ft_calloc(PATH_MAX, sizeof(char));

	// look for OLD_PWD and PWD variables // probably don't actually need to look for OLD_PWD cause we always export it
	// if they exist, change values
	// otherwise export them
	//
	new_pwd = NULL;
	new_old_pwd = NULL;

	printf("\n\n\nbefore:\n\n\n");
	minishell_pwd();

	// GOD I HATE THIS
	char *arg_with_slash = ft_strjoin("/", arg);

	getcwd(cur_pwd, PATH_MAX);
	path = ft_strjoin(cur_pwd, arg_with_slash);
	free(arg_with_slash);
	printf("path before thing: %s\n", path);
	if (!path)
	{
		printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
		perror_and_return(MALLOC_ERR, LIBFUNC_ERR, -1);
	}
	int check = access(path, F_OK);
	if (check != 0)
	{
		perror("which access error");
		free(path);
		path = ft_strdup(arg);
	}
	err_check = record_dirs(path, &new_pwd, &new_old_pwd, cur_pwd);
	if (err_check < 0)
	{
		printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
		return (err_check);
	}
	err_check = chdir(path);
	if (err_check != 0)
	{
		printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
		perror_and_return("cd", LIBFUNC_ERR, -1);
	}
	else
		err_check = set_env_vars(minishell_data, new_pwd, new_old_pwd, cur_pwd);
	cd_free(path, new_pwd, new_old_pwd, cur_pwd);

	printf("\n\n\nafter:\n\n\n");
	minishell_pwd();
	printf("\n\n\nvars:\n\n\n");
	minishell_env(minishell_data);

	return (err_check);
}

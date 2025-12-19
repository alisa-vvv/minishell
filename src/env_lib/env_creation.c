/* ************************************************************************** */
/*                                                                            */
/*                                                            ::::::::        */
/*   env_creation.c                                          :+:    :+:       */
/*                                                          +:+               */
/*   By: avaliull <avaliull@student.codam.nl>              +#+                */
/*                                                        +#+                 */
/*   Created: 2025/11/14 19:30:33 by avaliull            #+#    #+#           */
/*   Updated: 2025/12/19 17:07:00 by avaliull            ########   odam.nl   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "builtins.h"
#include "minishell_env.h"
#include <errno.h>

static int	set_default_env(
	char **msh_env,
	int *env_var_count
)
{
	char	*cwd;

	cwd = ft_calloc(PATH_MAX, sizeof(char));
	msh_env[0] = ft_strdup(STD_PATH);
	msh_env[1] = ft_strdup("SHLVL=1");
	if (!cwd || !msh_env[0] || !msh_env[1])
	{
		ft_safe_free((unsigned char **) &cwd);
		ft_safe_free((unsigned char **) &msh_env[0]);
		ft_safe_free((unsigned char **) &msh_env[1]);
		return (msh_perror(NULL, MALLOC_ERR, extern_err), extern_err);
	}
	*env_var_count = 2;
	if (env_util_get_cwd(cwd) != success)
		return (extern_err);
	msh_env[2] = ft_strjoin("PWD=\0", cwd);
	if (!msh_env[2])
		return (msh_perror(NULL, MALLOC_ERR, extern_err), extern_err);
	*env_var_count = 3;
	return (success);
}

static int	clone_loop(
	char *envp[],
	int *alloc_size,
	char **msh_env,
	int *env_var_count
)
{
	int	i;

	i = -1;
	while (envp[++i])
	{
		if (i == *alloc_size)
		{
			*alloc_size += *alloc_size / 2;
			free_2d_arr((void **) msh_env);
			msh_env = ft_calloc(*alloc_size, sizeof(char *));
			if (!msh_env)
				return (msh_perror(NULL, MALLOC_ERR, extern_err), errno);
			i = 0;
		}
		msh_env[i] = ft_strdup(envp[i]);
		if (!msh_env[i])
		{
			free_2d_arr((void **) msh_env);
			return (msh_perror(NULL, MALLOC_ERR, extern_err), errno);
		}
	}
	*env_var_count = i;
	return (success);
}

int	clone_env(
	t_msh_data *const msh_data,
	char *envp[]
)
{
	int	alloc_size;
	int	err;

	alloc_size = 128;
	msh_data->env = ft_calloc(alloc_size, sizeof(char *));
	if (!msh_data->env)
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	if (envp[0] == NULL)
		set_default_env(msh_data->env, &msh_data->env_var_count);
	else
	{
		err = clone_loop(envp, &alloc_size,
				msh_data->env, &msh_data->env_var_count);
		if (err != success)
			return (err);
	}
	err = validate_default_vars(msh_data);
	msh_data->env_mem = alloc_size;
	return (err);
}

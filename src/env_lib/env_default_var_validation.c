/* ************************************************************************** */
/*                                                                            */
/*                                                            ::::::::        */
/*   env_default_var_validation.c                            :+:    :+:       */
/*                                                          +:+               */
/*   By: avaliull <avaliull@student.codam.nl>              +#+                */
/*                                                        +#+                 */
/*   Created: 2025/11/21 18:05:26 by avaliull            #+#    #+#           */
/*   Updated: 2025/12/19 17:07:22 by avaliull            ########   odam.nl   */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "libft.h"
#include "minishell.h"
#include "minishell_env.h"
#include "builtins.h"

int	export_default_var(
	t_msh_data *const msh_data,
	char **var_value,
	char *var_string
)
{
	int	err_check;

	err_check = success;
	var_value[0] = ft_strdup(var_string);
	if (!var_value[0])
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	err_check = msh_export(var_value, msh_data);
	ft_safe_free((unsigned char **) &var_value[0]);
	return (err_check);
}

static int	export_cwd(
	t_msh_data *const msh_data,
	char **var_value
)
{
	char	*cwd;
	int		err_check;

	cwd = ft_calloc(PATH_MAX, sizeof(char));
	if (!cwd)
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	err_check = env_util_get_cwd(cwd);
	if (err_check != success)
	{
		free(cwd);
		return (err_check);
	}
	var_value[0] = ft_strjoin("PWD=\0", cwd);
	free(cwd);
	if (!var_value[0])
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	err_check = msh_export(var_value, msh_data);
	return (err_check);
}

int	validate_default_vars(
	t_msh_data *const msh_data
)
{
	char	**var_value;
	int		err_check;

	err_check = success;
	var_value = ft_calloc(2, sizeof(char *));
	if (env_var_if_exists(msh_data->env, "PATH") == false)
		err_check = export_default_var(msh_data, var_value, STD_PATH);
	if (err_check != success)
	{
		free_2d_arr((void **) var_value);
		return (err_check);
	}
	if (env_var_if_exists(msh_data->env, "SHLVL") == false)
		err_check = export_default_var(msh_data, var_value, "SHLVL=1");
	else
		err_check = update_shlvl(msh_data, var_value);
	if (err_check != success)
	{
		free_2d_arr((void **) var_value);
		return (err_check);
	}
	if (env_var_if_exists(msh_data->env, "PWD") == false)
		err_check = export_cwd(msh_data, var_value);
	free_2d_arr((void **) var_value);
	return (err_check);
}

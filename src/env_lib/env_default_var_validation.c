/* ************************************************************************** */
/*                                                                            */
/*                                                            ::::::::        */
/*   env_default_var_validation.c                            :+:    :+:       */
/*                                                          +:+               */
/*   By: avaliull <avaliull@student.codam.nl>              +#+                */
/*                                                        +#+                 */
/*   Created: 2025/11/21 18:05:26 by avaliull            #+#    #+#           */
/*   Updated: 2025/11/21 19:08:16 by avaliull            ########   odam.nl   */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "libft.h"
#include "minishell.h"
#include "minishell_env.h"
#include "builtins.h"

static int	export_default_var(
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

static int	iterate_shlvl(
	t_msh_data *const msh_data,
	char **var_value,
	char *shlvl_string
)
{
	int				err_check;
	unsigned long	shlvl_int;
	int				i;

	err_check = 0;
	shlvl_int = 0;
	i = -1;
	while (shlvl_string[++i])
		shlvl_int = shlvl_int * 10 + shlvl_string[i] - '0';
	if (shlvl_int >= INT_MAX)
		shlvl_int = 0;
	shlvl_int += 1;
	var_value[0] = ft_itoa(shlvl_int);
	if (!var_value[0])
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	err_check = msh_export(var_value, msh_data);
	ft_safe_free((unsigned char **) &var_value[0]);
	return (err_check);
}

static int	update_shlvl(
	t_msh_data *const msh_data,
	char **var_value
)
{
	int		err_check;
	char	*shlvl_value;
	int		i;
	bool	broken_shlvl;

	err_check = success;
	shlvl_value = NULL;
	err_check = env_var_get_value(msh_data->env, "SHLVL", &shlvl_value);
	if (err_check != success)
		return (err_check);
	i = -1;
	broken_shlvl = false;
	while (shlvl_value[++i])
	{
		if (!ft_isdigit(shlvl_value[i]) || i > 10)
		{
			err_check = export_default_var(msh_data, var_value, "SHLVL=1");
			broken_shlvl = true;
	  		break ;
		}
	}
	if (broken_shlvl == false)
		iterate_shlvl(msh_data, var_value, shlvl_value);
	ft_safe_free((unsigned char **) &shlvl_value);
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

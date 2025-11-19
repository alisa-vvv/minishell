/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   export.c                                          :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/07/01 15:41:12 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/18 16:51:00 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

//REMOVE
#include <assert.h>

#include "libft.h"
#include "minishell.h"
#include "builtins.h"
#include "minishell_env.h"

static int truncate_var( // move to env lib?
	t_msh_data *const msh_data,
	int var_index,
	char *arg
)
{
	if (var_index < msh_data->env_var_count &&
			msh_data->env[var_index])
		free(msh_data->env[var_index]);
	msh_data->env[var_index] = ft_strdup(arg);
	if (!msh_data->env[var_index])
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err); // check return
	return (0);
}

static int append_var( // move to env lib?
	t_msh_data *const msh_data,
	int var_index,
	char *arg,
	char *identifier
)
{
	char	*appended_var;

	if (var_index == msh_data->env_var_count)
	{
		*identifier = '\0';
		appended_var = ft_strjoin(arg, identifier + 1);
		*identifier = '+';
		if (!appended_var)
			return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err); // check return
	}
	else
		appended_var = ft_strjoin(msh_data->env[var_index], identifier + 2);
	if (!appended_var)
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err); // check return
	if (msh_data->env[var_index])
		free(msh_data->env[var_index]);
	msh_data->env[var_index] = appended_var;
	return (0);
}

int	msh_export(
	char *const *argv,
	t_msh_data *const msh_data
)
{
	int		var_i;
	int		arg_i;
	char	*identifier;
	int		err_check;

//	printf("\n\n\nPRE EXPORT\n\n\n");
//	msh_env(msh_data);
//	printf("\n\n\nENDPRE\n\n\n");
//
	if (!*argv)
		return (msh_env(msh_data));
	arg_i = -1;
	while (argv[++arg_i])
	{
		
		identifier = env_var_find_identifier(argv[arg_i]);
		if (!identifier)
			continue ;
		var_i = env_var_find_index(msh_data->env, argv[arg_i], identifier);

		assert(var_i <= msh_data->env_var_count); // REMOVE ASSERT
    	
		// CHANGE ENV_VAR_REALLOC
		// checks if env mem needs to be expanded, and if we='re adding or modifyig a var
		// ? confusion
		if (var_i == msh_data->env_mem - 1)
			env_var_realloc(msh_data);
		// ^ this should probably not be xposed to functions outsied env_lib
		if (*identifier == '=')
			err_check = truncate_var(msh_data, var_i, argv[arg_i]);
		if (*identifier == '+')
			err_check = append_var(msh_data, var_i, argv[arg_i], identifier);
		if (err_check < 0)
		{
			printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
			return (-1);
		}
		if (var_i == msh_data->env_var_count)
			msh_data->env_var_count += 1; 
	}

	
//	printf("\n\n\nPOST EXPORT\n\n\n");
//	msh_env(msh_data);

	//char **test_unset = ft_calloc(2, sizeof(char *));
	//test_unset[0] = "var";
	//test_unset[1] = "var2";
	//msh_unset(test_unset, msh_data);

	//printf("\n\n\nPOST UNSET\n\n\n");
	//msh_env(msh_data);
	return (0);
}

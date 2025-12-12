/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   env_operations.c                                  :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/07/15 19:44:31 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/18 16:41:07 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "minishell_env.h"

char	*env_var_find_identifier( // also change to return error?
	char *arg
)
{
	while (*arg)
	{
		if (*arg == '=' || (*arg == '+' && *(arg + 1) == '='))
			return (arg);
		else if (!ft_isalnum(*arg) && *arg != '_')
		{
			ft_putstr_fd(INVALID_IDENTIFIER, STDERR_FILENO); // should never happen???
			return (NULL);
		}
		arg++;
	}
	return (arg);
}

// compares the string name vs the variable in env.
// if finds match, checks if next symbol in env is '='
int	env_var_find_index( // change to return error if not found var?
	char **env,
	char *name,
	char *identifier
)
{
	int			i;
	const int	name_len = identifier - name;

	i = -1;

	while (env[++i] != NULL)
	{
		if (ft_strncmp(env[i], name, name_len) == 0 &&
				env[i][name_len] == '=') // fuck, this is wrong i think
			return (i);
	}
	return (i);
}

bool	env_var_if_exists(
	char **env,
	char *name
)
{
	int	name_len;
	int	i;

	name_len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0 &&
				env[i][name_len] == '=')
			return (true);
		i++;
	}
	return (false);
}

// this function takes the adress of a pointer to NULL. it will allocate and
// write to it.
int	env_var_get_value(// change this to be int for errors. and write to buffer.
	char **env,
	char *name,
	char **buf_ptr
)
{
	char	*value;
	int		name_len;
	int		i;

	name_len = ft_strlen(name);
	value = NULL;
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0 &&
				env[i][name_len] == '=')
		{
			value = ft_strdup(&env[i][name_len + 1]);
			if (!value)
				return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
			break ;
		}
		else
			i++;
	}
	*buf_ptr = value;
	return (success);
}

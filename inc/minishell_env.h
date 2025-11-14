/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   minishell_env.h                                   :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/07/15 19:45:44 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/15 20:14:55 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_ENV_H
# define MINISHELL_ENV_H

#include "minishell.h"

/*		Environment errors		*/
# define INVALID_IDENTIFIER "not a valid identifier: "
/**/

char	**clone_env(
	t_minishell_data *const minishell_data,
	int *env_var_count,
	int *env_mem
);

int env_var_realloc(
	t_minishell_data *minishell_data
);

int	env_var_find_index(
	char **env,
	char *name,
	char *identifier
);

char	*env_var_find_identifier(
	char *arg
);

char	*env_var_get_value(
	char **env,
	char *name
);

#endif 

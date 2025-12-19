/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   msh_env.h                                   :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/07/15 19:45:44 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/15 20:14:55 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_ENV_H
# define MINISHELL_ENV_H

# include "minishell.h"
# include "parser.h"

/*		Environment errors		*/
# define INVALID_IDENTIFIER "not a valid identifier: "
# define TOO_MANY_SH_ERR "Maximum number of sessions reached!"
# define NO_VAR -1
# define TOO_MANY_ENV_VAR "could not export, environment variable limit reached"
/**/

/*	Initialization	*/
int		clone_env(
			t_msh_data *const msh_data,
			char *envp[]);
int		validate_default_vars(
			t_msh_data *const msh_data);
/**/

/*	Variable usage	*/
bool	env_var_if_exists(
			char **env,
			char *name);
int		env_var_find_index(
			char **env,
			char *name,
			char *identifier);
char	*env_var_find_identifier(
			char *arg);
int		env_var_get_value(
			char **env,
			char *name,
			char **buf_ptr,
			int *existing);
/**/

/*	Utils	*/
int		env_util_get_cwd(
			char *cwd);
/**/

#endif

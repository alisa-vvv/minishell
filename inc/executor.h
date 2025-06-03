/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   executor.h                                        :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/03 15:25:20 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/03 18:05:13 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"

/*		Executor - temporary functions!		*/
t_exec_data	*test_get_dummy_exec_data();
int	executor(t_exec_data *exec_data, int dummy_minishell_struct);

#endif

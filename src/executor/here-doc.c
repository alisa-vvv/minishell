/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   here-doc.c                                        :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/05 19:39:01 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/05 20:21:32 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "executor.h"
#include <stdio.h>
#include <fcntl.h>

int	create_here_doc(t_exec_data *command, int here_doc[2])
{
	int			err_check;
	size_t		delim_len;
	char		*input_str;

	err_check = 0;
	delim_len = ft_strlen(command->heredoc_delim);
	err_check = pipe(here_doc);
	if (err_check < 0)
	{
		// ADD ERROR MANAGEMENT
		printf("PLACEHOLDER - THIS SHOULD ERROR\n");
	}

	input_str = readline("heredoc> ");
	if (input_str == NULL)
	{
		// ADD ERROR MANAGEMENT
		printf("PLACEHOLDER - THIS SHOULD ERROR\n");
	}
	while (ft_strncmp(input_str, command->heredoc_delim, delim_len) != 0)
	{
		ft_putstr_fd(input_str, here_doc[1]);
		input_str = readline("heredoc> ");
		if (input_str == NULL)
		{
			// ADD ERROR MANAGEMENT
			printf("PLACEHOLDER - THIS SHOULD ERROR\n");
		}
	}
	close(here_doc[1]);
	return (err_check);
}

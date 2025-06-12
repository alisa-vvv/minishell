/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   here-doc.c                                        :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/05 19:39:01 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/09 19:16:19 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "executor.h"
#include <stdio.h>
#include <fcntl.h>

int	create_here_doc(
	const char *heredoc_delim,
	int here_doc[2]
)
{
	const size_t	delim_len = ft_strlen(heredoc_delim);
	int				err_check;
	char			*input_str;

	err_check = 0;
	err_check = pipe(here_doc);
	if (err_check < 0)
		perror_and_return(PIPE_ERR, LIBFUNC_ERR);
	input_str = readline("heredoc> ");
	if (input_str == NULL)
		perror_and_return(READLINE_ERR, LIBFUNC_ERR);
	while (ft_strncmp(input_str, heredoc_delim, delim_len) != 0)
	{
		ft_putstr_fd(input_str, here_doc[WRITE_END]);
		input_str = readline("heredoc> ");
		if (input_str == NULL)
			perror_and_return(READLINE_ERR, LIBFUNC_ERR);
	}
	test_close(here_doc[WRITE_END]);
	return (err_check);
}

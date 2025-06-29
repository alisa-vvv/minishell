/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   here-doc.c                                        :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/05 19:39:01 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/12 19:05:47 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "executor.h"
#include <stdio.h>
#include <fcntl.h>
#include <curses.h>
#include <term.h>

int	create_here_doc(
	const char *heredoc_delim
)
{
	const size_t	delim_len = ft_strlen(heredoc_delim);
	int				err_check;
	char			*input_str;
	int				here_doc[2];

	err_check = 0;
	err_check = pipe(here_doc);
	if (err_check < 0)
		perror_and_return(PIPE_ERR, LIBFUNC_ERR, -1);
	input_str = readline("heredoc> ");
	if (input_str == NULL)
		perror_and_return(READLINE_ERR, LIBFUNC_ERR, -1);
	while (ft_strncmp(input_str, heredoc_delim, delim_len) != 0)
	{
		ft_putstr_fd(input_str, here_doc[WRITE_END]);
		free(input_str);
		input_str = readline("heredoc> ");
		if (input_str == NULL)
			perror_and_return(READLINE_ERR, LIBFUNC_ERR, -1);
	}
	test_close(here_doc[WRITE_END]);
	return (here_doc[READ_END]);
}

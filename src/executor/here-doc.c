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

#include "executor.h"
#include "libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>

//why do we have a handler ignoring the SIGINT and SIGQUIT
//-> can we just set the global flag to change when we get in the heredoc to
//have main ignore signals
//and have the non-interactive sighandler handle the sigs from there
static int	heredoc_readline_loop(
	const char *const heredoc_delim,
	int here_doc[2]

)
{
	const size_t	delim_len;
	char			*input_str;

	delim_len = ft_strlen(heredoc_delim);
	handle_signals_interactive();
	while (1)
	{
		input_str = readline("heredoc> ");
		if (!input_str)
		{
			ft_putstr_fd("msh: warning: terminated here-doc\n", STDERR_FILENO);
			break ;
		}
		if (ft_strncmp(input_str, heredoc_delim, delim_len) == 0
			&& input_str[delim_len] == '\0')
			break ;
		ft_putstr_fd(input_str, here_doc[WRITE_END]);
		ft_putchar_fd('\n', here_doc[WRITE_END]);
		free(input_str);
	}
	if (input_str)
		free(input_str);
	return (success);
}

static int	handle_heredoc_child(
	t_msh_data *const msh_data,
	const char *heredoc_delim,
	int here_doc[2],
	int pid)
{
	int	err_check;

	err_check = success;
	if (pid < 0)
	{
		safe_close(&here_doc[READ_END]);
		safe_close(&here_doc[WRITE_END]);
		return (msh_perror(NULL, FORK_ERR, extern_err), fork_err);
	}
	else if (pid == 0)
	{
		safe_close(&here_doc[READ_END]);
		msh_data->is_parent = false;
		err_check = heredoc_readline_loop(heredoc_delim, here_doc);
		safe_close(&here_doc[WRITE_END]);
		return (child_heredoc);
	}
	else if (pid > 0)
	{
		safe_close(&here_doc[WRITE_END]);
		ignore_signals();
		wait(NULL);
		handle_signals_non_interactive();
	}
	return (err_check);
}

int			create_here_doc(
	t_msh_data *const msh_data,
	const char *heredoc_delim)
{
	int	err_check;
	int	here_doc[2];
	int	pid;

	err_check = 0;
	err_check = pipe(here_doc);
	if (err_check < 0)
		return (msh_perror(NULL, PIPE_ERR, extern_err), pipe_err);
	pid = fork();
	err_check = handle_heredoc_child(msh_data, heredoc_delim, here_doc, pid);
	if (err_check != success)
	{
		safe_close(&here_doc[READ_END]);
		return (err_check);
	}
	return (here_doc[READ_END]);
}

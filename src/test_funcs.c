/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   test_funcs.c                                      :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/01 15:42:19 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/18 20:28:45 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdio.h>

t_redir_list	*test_add_redirection(
	const t_redir_list *first,
	const t_redirect_type type,
	const int src,
	const char *dest,
	const char *heredoc_delim
)
{
	t_redir_list	*cur_node = (t_redir_list *) first;
	t_redir_list	*redir_list = ft_calloc(1, sizeof(*redir_list));
	
	redir_list->type = type;
	redir_list->src_fd = src;
	if (dest)
		redir_list->dest_filename = ft_strdup(dest);
	if (heredoc_delim)
		redir_list->heredoc_delim = ft_strdup(heredoc_delim);
	redir_list->next = NULL;
	if (first != NULL)
	{
		while (cur_node->next != NULL)
			cur_node = cur_node->next;
		cur_node->next = redir_list;
	}
	else
		first = redir_list;
	return ((t_redir_list *) first);
}

t_exec_data	*test_get_dummy_exec_data(
	int len
)
{
	t_exec_data	*exec_data;

	int i = 0;
	exec_data = ft_calloc(len + 1, sizeof(t_exec_data));
	exec_data[i].argv = ft_calloc(10, sizeof(char *));
	exec_data[i].argv[0] = ft_strdup("ls");
	exec_data[i].argv[1] = ft_strdup("-l");
//	exec_data[i].argv[0] = ft_strdup("env");
//	exec_data[i].argv[1] = ft_strdup("src");
	//exec_data[i].argv[2] = ft_strdup("one");
	//exec_data[i].argv[3] = ft_strdup("two");
	exec_data[i].is_builtin = false;
	exec_data[i].builtin_name = not_builtin;
	exec_data[i].input_is_pipe = false;
	exec_data[i].output_is_pipe = false;
	exec_data[i].redirections = NULL;
	//exec_data[i].redirections = test_add_redirection(exec_data[i].redirections, trunc, STDOUT_FILENO, "", NULL);
	//exec_data[i].redirections = test_add_redirection(exec_data[i].redirections, input, STDIN_FILENO, "infile", NULL);
	//exec_data[i].redirections = test_add_redirection(exec_data[i].redirections, heredoc, STDIN_FILENO, NULL, "EOF2");
	i++;

	//exec_data = ft_calloc(len + 1, sizeof(t_exec_data));
	//exec_data[i].argv = ft_calloc(10, sizeof(char *));
	//exec_data[i].argv[0] = ft_strdup("export");
	//exec_data[i].is_builtin = true;
	//exec_data[i].input_is_pipe = false;
	//exec_data[i].output_is_pipe = false;
	//exec_data[i].redirections = NULL;
	//i++;

//	exec_data = ft_calloc(len + 1, sizeof(t_exec_data));
//	exec_data[i].argv = ft_calloc(10, sizeof(char *));
//	exec_data[i].argv[0] = ft_strdup("env");
//	exec_data[i].is_builtin = true;
//	exec_data[i].input_is_pipe = false;
//	exec_data[i].output_is_pipe = false;
//	exec_data[i].redirections = NULL;
//	i++;

//	exec_data[i].argv = ft_calloc(10, sizeof(char *));
//	exec_data[i].argv[0] = ft_strdup("pwd");
//	//exec_data[i].argv[1] = ft_strdup("-e");
//	exec_data[i].is_builtin = true;
//	exec_data[i].input_is_pipe = false;
//	exec_data[i].output_is_pipe = false;
//	i++;

//	exec_data[i].argv = ft_calloc(10, sizeof(char *));
//	exec_data[i].argv[0] = ft_strdup("cat");
//	exec_data[i].argv[1] = ft_strdup("-T");
//	exec_data[i].is_builtin = false;
//	exec_data[i].input_is_pipe = true;
//	exec_data[i].output_is_pipe = true;
//	i++;
//
//	exec_data[i].argv = ft_calloc(10, sizeof(char *));
//	exec_data[i].argv[0] = ft_strdup("cat");
//	exec_data[i].argv[1] = ft_strdup("-b");
//	exec_data[i].is_builtin = false;
//	exec_data[i].input_is_pipe = true;
//	exec_data[i].output_is_pipe = false;
//	exec_data[i].redirections = test_add_redirection(exec_data[i].redirections, append, STDOUT_FILENO, "outfile", NULL);
//	i++;

	return (exec_data);
}

void	test_func(void){
	printf("test\n");
}

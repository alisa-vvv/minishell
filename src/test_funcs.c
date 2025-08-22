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
	t_redir_list *first,
	t_redirect_type type,
	int src,
	char *dest,
	char *heredoc_delim
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
	t_minishell_data	*minishell_data,
	int len
)
{
	t_exec_data	*exec_data;

	int i = 0;
	printf("len? %d\n", len);
	exec_data = ft_calloc(len + 1, sizeof(t_exec_data));
	minishell_data->exec_data = exec_data;
	minishell_data->command_count = len;


	// {
	// REDIRECTIONS TEST
	//
//	//exec_data[i].redirections = test_add_redirection(exec_data[i].redirections, trunc, STDOUT_FILENO, "", NULL);
//	//exec_data[i].redirections = test_add_redirection(exec_data[i].redirections, input, STDIN_FILENO, "infile", NULL);
//	//exec_data[i].redirections = test_add_redirection(exec_data[i].redirections, heredoc, STDIN_FILENO, NULL, "EOF");
//	//exec_data[i].redirections = test_add_redirection(exec_data[i].redirections, heredoc, STDIN_FILENO, NULL, "EOF2");
//	i++;
	//	}

	// {
	// BASIC_TEST_01: ls sleep cat
	//
//	exec_data[i].argv = ft_calloc(10, sizeof(char *));
//	exec_data[i].argv[0] = ft_strdup("ls");
//	exec_data[i].argv[1] = ft_strdup("-l");
//	exec_data[i].is_builtin = false;
//	exec_data[i].builtin_name = not_builtin;
//	exec_data[i].input_is_pipe = false;
//	exec_data[i].output_is_pipe = true;
//	exec_data[i].redirections = NULL;
//	i++;
//
//	exec_data[i].argv = ft_calloc(10, sizeof(char *));
//	exec_data[i].argv[0] = ft_strdup("sleep");
//	exec_data[i].argv[1] = ft_strdup("2");
//	exec_data[i].is_builtin = false;
//	exec_data[i].builtin_name = not_builtin;
//	exec_data[i].input_is_pipe = true;
//	exec_data[i].output_is_pipe = true;
//	exec_data[i].redirections = NULL;
//	i++;
	//	}

//	// {
//	// BASIC_TEST_02: ls cat
//	//
//	exec_data[i].argv = ft_calloc(10, sizeof(char *));
//	exec_data[i].argv[0] = ft_strdup("ls");
//	exec_data[i].argv[1] = ft_strdup("-l");
//	exec_data[i].builtin_name = not_builtin;
//	exec_data[i].input_is_pipe = false;
//	exec_data[i].output_is_pipe = true;
//	exec_data[i].redirections = NULL;
//	i++;
//
//	exec_data[i].argv = ft_calloc(10, sizeof(char *));
//	exec_data[i].argv[0] = ft_strdup("cat");
//	exec_data[i].argv[1] = ft_strdup("-b");
//	exec_data[i].builtin_name = not_builtin;
//	exec_data[i].input_is_pipe = true;
//	exec_data[i].output_is_pipe = false;
//	exec_data[i].redirections = NULL;
//	i++;
//	//	}

	// {
	// BASIC_TEST_03: heredoc cat
	//
//	exec_data[i].argv = ft_calloc(10, sizeof(char *));
//	exec_data[i].argv[0] = ft_strdup("cat");
//	exec_data[i].argv[1] = ft_strdup("-b");
//	exec_data[i].builtin_name = not_builtin;
//	exec_data[i].input_is_pipe = false;
//	exec_data[i].output_is_pipe = true;
//	exec_data[i].redirections = test_add_redirection(exec_data[i].redirections, heredoc, STDIN_FILENO, NULL, "EOF");
//	i++;
//
//	exec_data[i].argv = ft_calloc(10, sizeof(char *));
//	exec_data[i].argv[0] = ft_strdup("cat");
//	exec_data[i].argv[1] = ft_strdup("-e");
//	exec_data[i].builtin_name = not_builtin;
//	exec_data[i].input_is_pipe = true;
//	exec_data[i].output_is_pipe = false;
//	exec_data[i].redirections = NULL;
//	i++;

//	// BASIC_TEST_04: just heredhoc
//	//
//	exec_data[i].argv = ft_calloc(10, sizeof(char *));
//	exec_data[i].builtin_name = not_builtin;
//	exec_data[i].input_is_pipe = false;
//	exec_data[i].output_is_pipe = false;
//	exec_data[i].redirections = test_add_redirection(exec_data[i].redirections, heredoc, STDIN_FILENO, NULL, "EOF");
//	i++;
//
//	// BASIC_TEST_05: a builtin
//	//
//	exec_data[i].argv = ft_calloc(10, sizeof(char *));
//	exec_data[i].argv[0] = ft_strdup("cd");
//	exec_data[i].argv[1] = ft_strdup("");
//	exec_data[i].builtin_name = builtin_cd;
//	exec_data[i].input_is_pipe = false;
//	exec_data[i].output_is_pipe = false;
//	exec_data[i].redirections = NULL;
//	i++;

	// BASIC_TEST_03: ls (irr.) | heredoc cat
	//
	exec_data[i].argv = ft_calloc(10, sizeof(char *));
	exec_data[i].argv[0] = ft_strdup("ls");
	exec_data[i].argv[1] = ft_strdup("-l");
	exec_data[i].builtin_name = not_builtin;
	exec_data[i].input_is_pipe = false;
	exec_data[i].output_is_pipe = true;
	exec_data[i].redirections = NULL;
	i++;

	exec_data[i].argv = ft_calloc(10, sizeof(char *));
	exec_data[i].argv[0] = ft_strdup("cat");
	exec_data[i].argv[1] = ft_strdup("-b");
	exec_data[i].builtin_name = not_builtin;
	exec_data[i].input_is_pipe = true;
	exec_data[i].output_is_pipe = false;
	exec_data[i].redirections = test_add_redirection(exec_data[i].redirections, heredoc, STDIN_FILENO, NULL, "EOF");
	i++;

	return (exec_data);
}

void	test_func(void){
	printf("test\n");
}

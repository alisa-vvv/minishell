/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   parser2.c                                           :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/12/09 12:51:47 by adecheri       #+#    #+#                */
/*   Updated: 2025/12/09 12:51:49 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	check_spcases(t_tokenlist *tokenlist)
{
	t_token	*token;
	int		err;

	err = success;
	token = NULL;
	if (tokenlist->total < 2)
	{
		err = single_token(tokenlist);
		if (err != success)
			return (err);
	}
	else
		err = val_redir(tokenlist, 0, token);
	return (err);
}

int	experimental_add_arg_to_execdata(
	t_msh_data *msh_data,
	t_token *cur_token,
	const size_t exdata_i,
	size_t *argv_i
)
{
	msh_data->exec_data[exdata_i].argv[*argv_i] = ft_strdup(cur_token->value);
	if (!msh_data->exec_data[exdata_i].argv[*argv_i])
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	*argv_i += 1;
	return (success);
}

void	experimental_new_redir_elem(
	t_redir_list **first,
	t_redir_list *redir_node
)
{
	t_redir_list	*cur_node;

	redir_node->next = NULL;
	if (*first != NULL)
	{
		cur_node = *first;
		while (cur_node->next != NULL)
			cur_node = cur_node->next;
		cur_node->next = redir_node;
	}
	else
		*first = redir_node;
}

int	experimental_add_redirection(
	t_redir_list **first,
	t_token_type type,
	char *next_token_val
)
{
	t_redir_list	*redir_node;
	
	redir_node = ft_calloc(1, sizeof(t_redir_list));
	if (!redir_node)
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	if (type == RED_IN)
	{
		redir_node->type = input;
		redir_node->src_fd = STDIN_FILENO; // change these to defaults isntead?
	}
	if (type == RED_OUT || type == RED_APP)
		redir_node->src_fd = STDOUT_FILENO;
	if (type == RED_OUT)
		redir_node->type = trunc;
	if (type == RED_APP)
		redir_node->type = append;
	if (type == HEREDOC)
		redir_node->type = heredoc;
	if (next_token_val && type != HEREDOC)//  cnan next_token_val be null?
	{
		redir_node->dest_filename = ft_strdup(next_token_val);
		if (!redir_node->dest_filename)
			return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	}
	else if (next_token_val && type == HEREDOC)
	{
		redir_node->heredoc_delim = ft_strdup(next_token_val);
		if (!redir_node->heredoc_delim)
			return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	}
	experimental_new_redir_elem(first, redir_node);
	return (success);
}

int experimental_pass_comm(
	t_tokenlist *tokenlist,
	t_msh_data *msh_data
)
{
	size_t	token_i;
	size_t	argv_i;
	size_t	exdata_i;
	int		err;
	t_token	*cur_token;
	t_token	*next_token;

	size_t	argv_mem;
	int		i;

	token_i = 0;
	argv_i = 0;
	exdata_i = 0;
	err = success;

	argv_mem = 128;
	msh_data->exec_data[0].argv = ft_calloc(argv_mem, sizeof (char *));
	while (token_i < tokenlist->total)
	{
		if (argv_i == argv_mem - 1)
		{
			argv_mem += argv_mem / 2;
			char **new_argv;
			new_argv = ft_calloc(argv_mem, sizeof (char *));
			i = -1;
			while (msh_data->exec_data[exdata_i].argv[++i])
			{
				new_argv[i] = ft_strdup(msh_data->exec_data[exdata_i].argv[i]);
				if (!new_argv[i])
				{
					free_2d_arr((void **) new_argv);
					return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
				}
			}
			free(msh_data->exec_data[exdata_i].argv);
			msh_data->exec_data[exdata_i].argv = new_argv;
		}
		cur_token = tokenlist_get(tokenlist, token_i);
		if (cur_token->type >= CD && cur_token->type <= UNSET)
		{
			if (msh_data->exec_data[exdata_i].builtin_name == not_builtin)
				msh_data->exec_data[exdata_i].builtin_name = set_builtins(cur_token);
			err = experimental_add_arg_to_execdata(msh_data, cur_token,
										  exdata_i, &argv_i);
		}
		if (cur_token->type == COMMAND || cur_token->type == STRING
			|| cur_token->type == PARAMETER || cur_token->type == DOUBLE_Q
			|| cur_token->type == SINGLE_Q )
			err = experimental_add_arg_to_execdata(msh_data, cur_token,
										  exdata_i, &argv_i);
		else if (cur_token->type == RED_IN || cur_token->type == RED_OUT
			|| cur_token->type == RED_APP)
		{
			next_token = tokenlist_get(tokenlist, ++token_i);
			if (!next_token || next_token->type != STRING)
				return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
			experimental_add_redirection(&msh_data->exec_data[exdata_i].redirections,
								cur_token->type, next_token->value);
		}
		else if (cur_token->type == HEREDOC)
		{
			next_token = tokenlist_get(tokenlist, ++token_i);
			if (!next_token || next_token->type != HEREDOC_DEL)
				return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
			experimental_add_redirection(&msh_data->exec_data[exdata_i].redirections,
								cur_token->type, next_token->value);
		}
		else if (cur_token->type == PIPE)
		{
			next_token = tokenlist_get(tokenlist, token_i + 1);
			if (!next_token || next_token->type == PIPE)
				return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
			msh_data->exec_data[exdata_i].output_is_pipe = true;
			exdata_i++;
			argv_i = 0;
			argv_mem = 128;
			msh_data->exec_data[exdata_i].argv = ft_calloc(argv_mem, sizeof (char *)); // REPEATED CODE ALRET
			msh_data->exec_data[exdata_i].input_is_pipe = true;
		}
		token_i++;
	}
	return (err);
}

int	prep_execdata(t_tokenlist *tlist, t_msh_data *msh_data)
{
	t_pos	*ind;
	int		err;

	err = success;
	ind = NULL;
	test_tokens(tlist);
	if (tlist)
	{
		set_pipe_cm(tlist, 0);
		ind = ft_calloc(1, sizeof(t_pos));
		if (!ind)
			return (msh_perror(NULL, MALLOC_ERR, parse_err), malloc_err);
		//err = pass_comm(tlist, msh_data, ind);

		msh_data->command_count = count_lists(tlist);
		if (msh_data->command_count == syntax_err)
			return (syntax_err);
		dprintf(STDERR_FILENO, "coomand count correct? %d\n", msh_data->command_count);
		msh_data->exec_data = ft_calloc(msh_data->command_count,
				sizeof(t_exec_data));
		if (!msh_data->exec_data)
			return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
		err = experimental_pass_comm(tlist, msh_data);

	}
	free(ind);
	return (err);
}

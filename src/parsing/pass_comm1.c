/* ************************************************************************** */
/*                                                                            */
/*                                                            ::::::::        */
/*   pass_comm1.c                                            :+:    :+:       */
/*                                                          +:+               */
/*   By: avaliull <avaliull@student.codam.nl>              +#+                */
/*                                                        +#+                 */
/*   Created: 2025/12/10 20:13:17 by avaliull            #+#    #+#           */
/*   Updated: 2025/12/10 21:02:28 by avaliull            ########   odam.nl   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

int	add_arg_to_execdata(
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

void	new_redir_elem(
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

int	add_redirection(
	t_redir_list **first,
	t_token_type type,
	char *next_token_val
)
{
	t_redir_list	*redir_node;
	
	redir_node = ft_calloc(1, sizeof(t_redir_list));
	if (!redir_node)
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	if (type == RED_OUT)
		redir_node->type = trunc;
	if (type == RED_APP)
		redir_node->type = append;
	if (type == HEREDOC)
		redir_node->type = heredoc;
	if (next_token_val && type != HEREDOC)//  yes if we expand on empty, next token_val can be '\0' || I believe we do check if reds are not the last token 
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
	new_redir_elem(first, redir_node);
	return (success);
}

int	reallocate_argv(
	t_msh_data *msh_data,
	size_t argv_mem,
	size_t argv_i,
	size_t exdata_i
)
{
	int	i;

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
	return (success);
}

bool legit_token(t_token *cur_token)
{
	if (cur_token->type == COMMAND || cur_token->type == STRING
		|| cur_token->type == PARAMETER || cur_token->type == DOUBLE_Q
		|| cur_token->type == SINGLE_Q )
		return (true);
	return (false);
}

int set_tok_built(t_msh_data *msh_data,
	t_token *cur_token,
	size_t exdata_i,
	size_t argv_i)
{
	int err;

	err = success;
	if (cur_token->type >= CD && cur_token->type <= UNSET)
	{
		if (msh_data->exec_data[exdata_i].builtin_name == not_builtin)
			msh_data->exec_data[exdata_i].builtin_name = set_builtins(cur_token);
		err = add_arg_to_execdata(msh_data, cur_token, exdata_i, &argv_i);
	}
	return (err);
}


int set_redir_comm(t_tokenlist *tokenlist,
	t_msh_data *msh_data,
	size_t index[4],
	t_token_type type)
{
	int err;
	t_token *cur_token; 
	t_token *next_token;

	err = success;
	cur_token = tokenlist_get(tokenlist, index[0]);
	next_token = tokenlist_get(tokenlist, ++index[0]);
	if (!next_token || next_token->type != type)
		return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
	add_redirection(&msh_data->exec_data[index[2]].redirections,
								cur_token->type, next_token->value);
	return (err);
}
//when encountered pipe, set data 
int set_pipe_data(t_msh_data *msh_data,
	t_tokenlist *tokenlist, 
	size_t index[4]
	)
{
	int err;
	t_token *next_token;

	err = success;

	next_token = looknxt(tokenlist, index[0]);
	if (!next_token || next_token->type == PIPE)
		return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
	msh_data->exec_data[index[2]].output_is_pipe = true;
	index[2]++;
	index[1] = 0;
	index[3] = 128;
	msh_data->exec_data[index[2]].argv = ft_calloc(index[3], sizeof (char *)); // REPEATED CODE ALRET
	msh_data->exec_data[index[2]].input_is_pipe = true;
	return (err);
}

// index[3] will hold the positions of token_i --> index[0], argv_i --> index[1], exdata_i --> index[2], argv-mem --> index[3]. 
// heredocs seem to be broken -> not sure if there is a problem with the delimiter
int pass_comm(
	t_tokenlist *tokenlist,
	t_msh_data *msh_data
)
{

	int		err;
	t_token	*cur_token;
	size_t	index[4];
	//size_t	argv_mem;
	index[0] = 0;
	index[1] = 0;
	index[2] = 0;
	index[3] = 128;

	// token_i = 0;
	// argv_i = 0;
	// exdata_i = 0;
	err = success;

//	argv_mem = 128;
	msh_data->exec_data[0].argv = ft_calloc(index[3], sizeof (char *));
	while (index[0] < tokenlist->total)
	{
		if (reallocate_argv(msh_data, index[3], index[1], index[2]) != success)
			return (malloc_err);
		cur_token = tokenlist_get(tokenlist, index[0]);
		err = set_tok_built(msh_data, cur_token, index[2], index[1]);
		if (err != success)
			return (err);
		if (legit_token(cur_token))
			err = add_arg_to_execdata(msh_data, cur_token,
										  index[2], &index[1]);
		else if (tok_is_red(cur_token))
		{
			err = set_redir_comm(tokenlist, msh_data, index, STRING);
			if (err != success)
				return (err);
			// next_token = tokenlist_get(tokenlist, ++token_i);
			// if (!next_token || next_token->type != STRING)
			// 	return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
			// add_redirection(&msh_data->exec_data[exdata_i].redirections,
			// 					cur_token->type, next_token->value);
		}
		else if (cur_token->type == HEREDOC)
		{
			err = set_redir_comm(tokenlist, msh_data, index, HEREDOC_DEL);
			if (err != success)
				return (err);
			// next_token = tokenlist_get(tokenlist, ++token_i);
			// if (!next_token || next_token->type != HEREDOC_DEL)
			// 	return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
			// add_redirection(&msh_data->exec_data[exdata_i].redirections,
			// 					cur_token->type, next_token->value);
		}
		else if (cur_token->type == PIPE)
		{
			err = set_pipe_data(msh_data, tokenlist, index);
			if (err != success)
				return (err);
			// next_token = tokenlist_get(tokenlist, token_i + 1);
			// if (!next_token || next_token->type == PIPE)
			// 	return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
			// msh_data->exec_data[exdata_i].output_is_pipe = true;
			// exdata_i++;
			// argv_i = 0;
			// argv_mem = 128;
			// msh_data->exec_data[exdata_i].argv = ft_calloc(argv_mem, sizeof (char *)); // REPEATED CODE ALRET
			// msh_data->exec_data[exdata_i].input_is_pipe = true;
		}
		index[0]++;
	}
	return (err);
}


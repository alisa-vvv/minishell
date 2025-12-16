/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   expansions.c                                        :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/17 14:24:03 by adecheri       #+#    #+#                */
/*   Updated: 2025/06/17 14:24:05 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// expands variables to env value if found
int	expand_new(t_tokenlist *tokenlist, size_t pos, char *str_token,
		t_exp_data *xp_d)
{
	int		offset;
	char	*new_str;
	t_token	*n_token;
	int		err;

	err = success;
	n_token = NULL;
	offset = get_offset(xp_d);
	new_str = exp_token(str_token, xp_d->start_var, xp_d->env_value, offset);
	if (*new_str == '\0' && !xp_d->existing)
	{
		ft_safe_free((unsigned char **)&new_str);
		//ft_safe_free((unsigned char **)&xp_d->env_value);
		xp_d->env_value = NULL;
		err = tokenlist_delete(tokenlist, pos);
	}
	else
	{
		n_token = new_token(tokenlist, new_str, ft_strlen(new_str) + 1);
		if (!n_token)
			return (malloc_err);
		tokenlist_set(tokenlist, pos, n_token);
		ft_safe_free((unsigned char **)&new_str);
	}
	return (err);
}

//	else if (check_in_quote_s(check_token->value, (*exp_data)->start_pos, '"')
		// && !(*exp_data)->env_value && (*exp_data)->start_var[0] == '$'
		// && ft_strncmp((*exp_data)->name, "", 1) == 0)
int	exp_further(t_tokenlist *tokenlist,
	int pos,
	t_exp_data **exp_data)
{
	t_token	*check_token;
	int		err;

	err = success;
	check_token = tokenlist->tokens[pos];
	if (check_in_quote_s(check_token->value, (*exp_data)->start_pos, '\''))
		(*exp_data)->start_var = ft_strchr((*exp_data)->start_var + 1, '$');
	else if (!(*exp_data)->env_value && ft_strncmp(check_token->value, "$",
			2) == 0)
		(*exp_data)->start_var = ft_strchr((*exp_data)->start_var + 1, '$');
	else if (check_in_quote_s(check_token->value, (*exp_data)->start_pos, '"')
		&& !(*exp_data)->env_value && (*exp_data)->start_var[0] == '$'
		&& ft_strncmp((*exp_data)->name, "", 1))
		(*exp_data)->start_var = ft_strchr((*exp_data)->start_var + 1, '$');
	else
	{
		err = expand_new(tokenlist, pos, check_token->value, *exp_data);
		check_token = tokenlist->tokens[pos];
		if (!check_token)
			(*exp_data)->start_var = NULL;
		else
			(*exp_data)->start_var = ft_strchr(check_token->value, '$');
	}
	return (err);
}

//(exp_data->env_value && ft_strncmp(exp_data->env_value, "", 1) != 0)
//gets env value and name to check before expanding further
int	expand_check(t_tokenlist *tlist,
	t_msh_data *msh_data,
	t_exp_data *exp_data,
	int pos)
{
	int	err;

	err = success;
	if (exp_data->name && (ft_strncmp(exp_data->name, "?", 2) == 0))
	{
		exp_data->env_value = ft_itoa(msh_data->last_pipeline_return);
		exp_data->existing = 1;
	}
	else if (env_var_get_value(msh_data->env, exp_data->name, &exp_data->env_value, &exp_data->existing))
		return (malloc_err);
	err = exp_further(tlist, pos, &exp_data);
	if ((exp_data->existing && exp_data->env_value != (void *)'\0'))
		ft_safe_free((unsigned char **)&exp_data->env_value);
	ft_safe_free((unsigned char **)&exp_data->name);
	return (err);
}

// expand known var and otherwise delete and re-position all tokens
int	expand_var(
	t_tokenlist **tokenlist,
	int pos,
	t_msh_data *msh_data,
	t_token *check_token)
{
	t_exp_data	*exp_data;
	int			err;

	err = success;
	exp_data = ft_calloc(1, sizeof(t_exp_data));
	if (!exp_data)
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	exp_data->start_var = ft_strchr(check_token->value, '$');
	while (exp_data->start_var)
	{
		check_token = (*tokenlist)->tokens[pos];
		exp_data->start_pos = exp_data->start_var - check_token->value;
		err = refine_name(exp_data->start_var, &exp_data->name, '$');
		if (err != success)
			return (err);
		err = expand_check(*tokenlist, msh_data, exp_data, pos);
		if (err != success)
			break ;
	}
	return (ft_safe_free((unsigned char **)&exp_data), err);
}

// check lexer on expansion and quotes
int	exp_lexer(t_tokenlist *tokenlist, t_msh_data *msh_data, int type, size_t i)
{
	t_token	*check_token;
	int		err;

	err = success;
	while (i < (size_t)tokenlist->total)
	{
		check_token = (t_token *)tokenlist->tokens[i];
		if (!check_token)
			return (1);
		if (type == PARAMETER && (check_token->type == PARAMETER
				|| check_token->type == DOUBLE_Q
				|| check_token->type == SINGLE_Q))
			err = expand_var(&tokenlist, i, msh_data, check_token);
		else if (type == SINGLE_Q && (int)check_token->type == SINGLE_Q)
			err = rm_quotes(tokenlist, i, '\'');
		else if (type == DOUBLE_Q && (int)check_token->type == DOUBLE_Q)
			err = rm_quotes(tokenlist, i, '"');
		if (err != success)
			break ;
		i++;
	}
	return (err);
}

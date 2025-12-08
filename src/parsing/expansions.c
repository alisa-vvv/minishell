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

//get the difference of the str when exp variables
int	get_offset(t_exp_data *exp_data)
{
	int	offset;

	offset = 0;
	if (exp_data->env_value && exp_data->name)
		offset = ft_strlen(exp_data->name) + 1;
	else if (!exp_data->env_value && !exp_data->name)
	{
		exp_data->env_value = "";
		offset = 1;
	}
	else if (!exp_data->env_value && exp_data->name)
	{
		exp_data->env_value = "";
		offset = ft_strlen(exp_data->name) + 1;
	}
	else
		offset = 1;
	return (offset);
}

// p_printf("NAME = %s\n", exp_data->name);
// expands variables to env value if found
void	expand_new(t_tokenlist *tokenlist, size_t pos, char *str_token,
		t_exp_data *exp_data)
{
	int		offset;
	char	*new_str;
	t_token	*n_token;

	n_token = NULL;
	offset = get_offset(exp_data);
	new_str = exp_str_token(str_token, exp_data->start_var, exp_data->env_value,
			offset);
	if (*new_str == '\0')
	{
		(ft_safe_free((unsigned char **)&new_str));
		exp_data->env_value = NULL;
		tokenlist_delete(tokenlist, pos);
	}
	else
	{
		n_token = new_token(tokenlist, new_str, ft_strlen(new_str) + 1);
		if (!n_token)
			tokenlist_free(tokenlist);
		tokenlist_set(tokenlist, pos, n_token);
		(ft_safe_free((unsigned char **)&new_str));
	}
}

//(*exp_data)->start_var = ft_strchr((*exp_data)->start_var +1, '$');
void	exp_further(t_tokenlist *tokenlist, int pos, t_exp_data **exp_data)
{
	t_token	*check_token;

	check_token = tokenlist->tokens[pos];
	if (check_in_quote_s(check_token->value, (*exp_data)->start_pos, '\''))
		(*exp_data)->start_var = ft_strchr((*exp_data)->start_var + 1, '$');
	else if (!(*exp_data)->env_value && ft_strncmp(check_token->value, "$",
			2) == 0)
		(*exp_data)->start_var = ft_strchr((*exp_data)->start_var + 1, '$');
	else if (check_in_quote_s(check_token->value, (*exp_data)->start_pos, '"')
		&& !(*exp_data)->env_value && (*exp_data)->start_var[0] == '$'
		&& ft_strncmp((*exp_data)->name, "", 1) == 0)
		(*exp_data)->start_var = ft_strchr((*exp_data)->start_var + 1, '$');
	else
	{
		expand_new(tokenlist, pos, check_token->value, *exp_data);
		check_token = tokenlist->tokens[pos];
		if (!check_token)
			(*exp_data)->start_var = NULL;
		else
			(*exp_data)->start_var = ft_strchr(check_token->value, '$');
	}
}

// expand known var and otherwise delete and re-position all tokens
int	expand_var(t_tokenlist **tokenlist,
				int pos,
				t_msh_data *msh_data,
				t_token *check_token)
{
	t_exp_data	*exp_data;

	exp_data = ft_calloc(1, sizeof(t_exp_data));
	if (!exp_data)
		return (write(1, MALLOC_ERR, 15));
	exp_data->start_var = ft_strchr(check_token->value, '$');
	while (exp_data->start_var)
	{
		check_token = (*tokenlist)->tokens[pos];
		exp_data->start_pos = exp_data->start_var - check_token->value;
		exp_data->name = refine_name(exp_data->start_var, exp_data->name, '$');
		if (exp_data->name && (ft_strncmp(exp_data->name, "?", 2) == 0))
			exp_data->env_value = ft_itoa(msh_data->last_pipeline_return);
		else if (env_var_get_value(msh_data->env, exp_data->name,
				&exp_data->env_value) != success)
			dprintf(STDERR_FILENO, "Failed to malloc env\n");
		exp_further(*tokenlist, pos, &exp_data);
		if (exp_data->name)
			(ft_safe_free((unsigned char **)&exp_data->name));
		if (exp_data->env_value && ft_strncmp(exp_data->env_value, "", 1) != 0)
			ft_safe_free((unsigned char **)&exp_data->env_value);
	}
	(ft_safe_free((unsigned char **)&exp_data));
	return (0);
}

// e_printf("Token type = %s\n", enum_to_str(check_token->type));
// check_in_quote(check_token->value, lpos_in_str(check_token->value, '$')
// check lexer on expansion and quotes
int	exp_lexer(t_tokenlist *tokenlist, t_msh_data *msh_data, int type, size_t i)
{
	t_token	*check_token;

	while (i < (size_t)tokenlist->total)
	{
		check_token = (t_token *)tokenlist->tokens[i];
		if (!check_token)
			return (1);
		if (type == PARAMETER && (check_token->type == PARAMETER
				|| check_token->type == DOUBLE_Q
				|| check_token->type == SINGLE_Q))
		{
			expand_var(&tokenlist, i, msh_data, check_token);
		}
		else if (type == SINGLE_Q && (int)check_token->type == SINGLE_Q)
			rm_quotes(tokenlist, i, '\'');
		else if (type == DOUBLE_Q && (int)check_token->type == DOUBLE_Q)
			rm_quotes(tokenlist, i, '"');
		i++;
	}
	return (0);
}

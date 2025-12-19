/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   expansions_utils.c                                  :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/11/04 15:52:14 by adecheri       #+#    #+#                */
/*   Updated: 2025/11/04 15:52:16 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// expand the var when token is in quotes
char	*exp_token(char *str_token, char *start, char *value, int offset)
{
	char	*new_str;
	char	*leftover;
	char	*temp_left;

	temp_left = prep_leftover(str_token, start, offset);
	if (!temp_left)
		return (NULL);
	leftover = ft_strjoin(value, temp_left);
	new_str = ft_strjoin(str_token, leftover);
	if (!new_str || !leftover)
	{
		msh_perror(NULL, MALLOC_ERR, extern_err);
		ft_safe_free((unsigned char **)&leftover);
		ft_safe_free((unsigned char **)&new_str);
		ft_safe_free((unsigned char **)&temp_left);
		return (NULL);
	}
	ft_safe_free((unsigned char **)&leftover);
	ft_safe_free((unsigned char **)&temp_left);
	return (new_str);
}

// e_printf("\nRESULT = %s$\n", result);
// get name of env var from token_name
int	refine_name(char *token_name, char **result, char symbol)
{
	int	i;

	i = 0;
	if (!token_name)
	{
		*result = NULL;
		return (success);
	}
	*result = ft_strdup(token_name + 1);
	if (!*result)
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	while ((*result)[i])
	{
		if (symbol == '$')
		{
			if (char_is_quote((*result)[i]) || (*result)[i] == '$'
				|| ft_isspace((*result)[i]) || char_is_red((*result)[i])
				|| !(ft_isprint((*result)[i]) || (*result)[i] == '_'))
				break ;
		}
		i++;
	}
	(*result)[i] = '\0';
	return (success);
}

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

// prepare leftover parts of str token to keep for the updated string
char	*prep_leftover(char *str_token, char *start, int offset)
{
	char	*temp_left;
	size_t	rem_len;

	if (!str_token)
		return (NULL);
	if (!start)
		return (NULL);
	rem_len = ft_strlen(start + offset) + 1;
	temp_left = ft_calloc(rem_len, sizeof(char));
	if (!temp_left)
		return (msh_perror(NULL, MALLOC_ERR, extern_err), NULL);
	ft_strlcpy(temp_left, start + offset, rem_len + 1);
	*start = '\0';
	return (temp_left);
}

bool	skip_exp(t_token *check_token,
	t_exp_data *exp_data)
{
	if (check_in_quote_s(check_token->value, exp_data->start_pos, '\''))
		return (true);
	else if (!exp_data->env_value && ft_strncmp(exp_data->name, "",
			1) == 0)
		return (true);
	return (false);
}

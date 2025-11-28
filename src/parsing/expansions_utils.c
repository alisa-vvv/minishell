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





// prepare leftover parts of str token to keep for the updated string
char	*prep_leftover(char *str_token, char symbol, int offset)
{
	char	*start;
	char	*temp_left;
	size_t  rem_len;

	start = NULL;
	temp_left = NULL;
	if (!str_token)
		return (NULL);
	start = ft_strrchr(str_token, symbol);
	if (!start)
		return (NULL);
	
	rem_len = ft_strlen(start + offset)+1;
	// p_printf("OFFSET = %s$\n", start +offset);
	// p_printf("LEN IS = %d\n", rem_len);
	temp_left = malloc(rem_len + 1 * sizeof(char));
	if (!temp_left)
		return (NULL);
	ft_strlcpy(temp_left, start + offset, rem_len + 1);
	*start = '\0';
	// p_printf("TEMP LEFT = %s$\n", temp_left);
	return (temp_left);
}

// expand the var when token is in quotes
char	*exp_str_token(char *str_token, char *value, int offset)
{
	char	*new_str;
	char	*leftover;
	char	*temp_left;

	temp_left = NULL;
	temp_left = prep_leftover(str_token, '$', offset);
	leftover = ft_strjoin(value, temp_left);
	new_str = ft_strjoin(str_token, leftover);
	if (!new_str || !leftover)
		return ((ft_safe_free((unsigned char **)&leftover),
				ft_safe_free((unsigned char **)&value),
				ft_safe_free((unsigned char **)&new_str),
				ft_safe_free((unsigned char **)&temp_left), NULL));
	(ft_safe_free((unsigned char **)&leftover),
	ft_safe_free((unsigned char **)&temp_left),
	ft_safe_free((unsigned char **)&str_token));
	return (new_str);
}



// get name of env var from token_name
char	*refine_name_var(char *token_name, char *result, char symbol)
{
	char	*start;
	int		i;

	i = 0;
	start = NULL;
	start = ft_strchr(token_name, symbol);
	// e_printf("\nSTART = %s\n", start);
	result = ft_strdup(start + 1);
	if (!result)
		return (NULL);
	// e_printf("\nRESULT = %s$\n", result);
	while (result[i])
	{
		if (symbol == '$')
		{
			if (char_is_quote(result[i]) || result[i] == '$' || ft_isspace(result[i] || char_is_red(result[i]))
				|| !(ft_isalnum(result[i]) || result[i] == '_'))
				break ;
		}
		i++;
	}
	result[i] = '\0';
	return (result);
}

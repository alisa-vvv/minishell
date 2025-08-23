/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   lexer.c                                             :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/03 18:53:46 by adecheri       #+#    #+#                */
/*   Updated: 2025/06/03 18:53:56 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// make a new token in the array
t_token	*new_token(
	char *str, 
	int len)
{
	t_token	*token;

	if (!*str)
		return (NULL);
	token = malloc(1 * sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = malloc((len + 1) * sizeof(char));
	ft_strlcpy(token->value, str, len);
	if (!token->value)
		return (NULL);
	token->type = match_token(token->value);
	token->command = false; 
	return (token);
}

// counts args to size up elementlist
int	token_count(
	char *str)
{
	int	tokencount;
	int	i;
	i = 0;
	tokencount = 0;
	while (str[i])
	{
		if ((str[i] == '"' || str[i] == '\'') && str[i])
		{
			tokencount++;
            i += move_over_quote(str, i);
		}
		else if ((!check_in_quote(str, i) && !ft_isspace(str[i]) && !str_is_red(str[i]) && str[i]))
		{
			tokencount++;
			while (!ft_isspace(str[i]) && !str_is_red(str[i]) && str[i])
				i++;
		}
		else if (str_is_red(str[i]) && str[i] && !ft_isspace(str[i]))
		{
			tokencount++;
			while(str_is_red(str[i]) && str[i] && !ft_isspace(str[i]))
				i++;
		}
        else
            i++;
	}
    t_printf("token count = %d\n", tokencount);
	return (tokencount);
}

//add token to the list, if failed, release whole list
int add_token(
	element *tokenlist, 
	char *str, 
	int i, 
	size_t len)
{
	t_token	*token;
	l_printf("len = %zu ", len);
	token = new_token(str + i - len, len + 1);
	if (!token)
		return (tokenlist->pf_element_free(tokenlist), write(1, MALLOC_ERR, 15));
	token->pos = tokenlist->element_list.total;
	tokenlist->pf_element_add(tokenlist, token);
	return (0);
}

// pushes tokens in the elementlist from the back, immediately indexing
int	fill_tokenlist(
	element *tokenlist, 
	char *str)
{
	int		i;
	size_t	len;

	i = 0;
	while (str[i])
	{
		len = 0;
        if ((str[i] == '\'' || str[i] == '"') && str[i])
        {
            len = move_over_quote(str, i);
            i += len;
        }
		else if ((len == 0 && str[i]) && !ft_isspace(str[i]) && !str_is_red(str[i]))
        {
            while (str[i] && !ft_isspace(str[i]) && !str_is_red(str[i]))
			{
                len++;
				i++;
			}
        }
		else if (len == 0 && str[i] && !ft_isspace(str[i]) && str_is_red(str[i]))
		{
			while(str[i] && !ft_isspace(str[i]) && str_is_red(str[i]))
			{
				len++;
				i++;
			}
		}
		if (len > 0)
		{
			if (add_token(tokenlist, str, i, len))
				return (1);
		}
        else if (str[i])
            i++;
	}
	return (0);
}


// default option to put trimmed input in tokenlist
int	default_lexer(char *input_line, t_minishell_data *minishell_data)
{
	int		token_c;
	size_t	i;
	t_token	*token_test;

	token_c = 0;
	if (!input_line)
		return (1);
	input_line = trim_str_space(input_line);
	if (val_inputline(input_line))
        return (1);
	token_c = token_count(input_line);
	if (!token_c)
		return (write(1, "Failed to count tokens\n", 23));
	TOKEN_INIT(token_list, token_c);
	fill_tokenlist(&token_list, input_line);
	if (!token_list.element_list.tokens)
		return (write(1, "Failed to init tokenlist\n", 25));
	if (check_lexer(&token_list, minishell_data))
		return (write(1, "Failed check types\n", 19));
	
	return (0);
}




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

//returns len of unquoted token
int	move_o_unquoted(const char *str, int i)
{
	int	len;

	len = 0;
	while (str[i] && !char_is_quote(str[i]) && !ft_isspace(str[i])
		&& !char_is_red(str[i]))
	{
		len++;
		i++;
	}
	if (str[i] && char_is_quote(str[i]) && str[i - 1] == '=')
	{
		i++;
		while (str[i] && !char_is_quote(str[i]))
		{
			len++;
			i++;
		}
		i++;
		len += 2;
	}
	return (len);
}

//checks the length of unquoted str token
int	count_unq(const char *str, int i, int count)
{
	if (!char_is_red(str[i]))
		count++;
	while (str[i] && !char_is_quote(str[i]) && !ft_isspace(str[i]))
	{
		if (char_is_red(str[i]))
		{
			count++;
			if ((char_is_red(str[i]) && i == 0) || (i > 0 && ft_isspace(str[i
							- 1]) && char_is_red(str[i])))
				count++;
			while (str[i] && char_is_red(str[i]))
				i++;
			if (str[i] && !char_is_red(str[i]) && !ft_isspace(str[i]))
				count++;
		}
		i++;
	}
	return (count);
}

//counts args to size up elementlist
int	token_count(
	const char *str,
	int i,
	int tokencount)
{
	while (str[i])
	{
		if ((!check_in_quote(str, i) && !ft_isspace(str[i]))
			&& !char_is_quote(str[i]) && !char_is_red(str[i]))
		{
			tokencount = count_unq(str, i, tokencount);
			i += move_o_unquoted(str, i);
		}
		else if (char_is_quote(str[i]))
		{
			tokencount++;
			i += move_over_quote(str, i);
		}
		else if (char_is_red(str[i]))
		{
			tokencount++;
			while (char_is_red(str[i]))
				i++;
		}
		else
			i++;
	}
	return (tokencount);
}

// pushes tokens in the elementlist from the back, immediately indexing
int	fill_tokenlist(
	t_tokenlist *tokenlist,
	char *str)
{
	int		i;
	size_t	len;
	int		err;

	i = 0;
	len = 0;
	while (str[i])
	{
		len = set_len(str, i);
		i += len;
		if (len > 0)
		{
			err = prep_token(tokenlist, str, i, len);
			if (err != success)
				return (err);
		}
		else if (str[i])
			i++;
	}
	return (success);
}

// test_tokens(token_list);
// default option to put trimmed input in tokenlist
int	default_lexer(
	char *input_line,
	t_msh_data *msh_data
)
{
	int			token_c;
	t_tokenlist	*token_list;
	int			err;

	token_list = NULL;
	token_c = 0;

	if (input_line[0] == '\0')
		return (success);
	input_line = trim_str_space(input_line);
	err = val_inputline(input_line);
	if (err != success)
		return (err);

	token_c = token_count(input_line, 0, token_c);
	if (token_c == 0)
		return (success);
	if (token_c < 0)
		return (msh_perror(NULL, SYNTAX_ERR, parse_err), syntax_err);
	if (tokenlist_init(&token_list, token_c) != success)
		return (malloc_err);
	err = fill_tokenlist(token_list, input_line);
	if (err != success)
	{
		tokenlist_free(token_list);
		return (err);
	}

	err = check_lexer(token_list, msh_data); // currently doing this
	if (err) // fjdskfjkds
	{
		tokenlist_free(token_list);
		return (write(1, "Failed check types\n", 19));
	}
	tokenlist_free(token_list);
	return (success);
}

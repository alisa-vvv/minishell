/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   lexer_utils.c                                       :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/08/23 14:12:40 by adecheri       #+#    #+#                */
/*   Updated: 2025/08/23 14:12:41 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	skip_blanks(char *str, int pos)
{
	int	count;

	count = 0;
	while (!check_in_quote(str, pos) && ft_isspace(*str))
	{
		count++;
		str++;
	}
	// l_printf("%d\n", count);
	return (count);
}

// trims input to have 1 space
char	*trim_str_space(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = ft_strlen(str) - 1;
	while (ft_isspace(str[j] && char_is_quote(str[j])))
	{
		str[j] = '\0';
		j--;
	}
	j = 0;
	while (str[j])
	{
		if ((i == 0 && skip_blanks(str, j) >= 1))
			j += skip_blanks(str, j);
		else if (skip_blanks(str, j) > 1)
			j += skip_blanks(str, j + 1);
		str[i] = str[j];
		i++;
		j++;
	}
	str[i] = '\0';
	l_printf("trimmed str = %s$\n", str);
	return (str);
}

//move over quoted str and return the length of quoted text incl quotes
int	move_over_quote(const char *str, int pos)
{
	int		start;
	char	quote;

	if (!str[pos] || !char_is_quote(str[pos]))
		return (0);
	start = pos;
	quote = str[pos];
	pos++; 
	while (str[pos] && str[pos] != quote)
		pos++;
	if (str[pos] == quote)
		pos++;
	while (str[pos] && !ft_isspace(str[pos]))
	{
		if (char_is_quote(str[pos]))
		{
			pos += move_over_quote(str, pos);
		}
		else 
			pos++;
	} 
	return (pos - start);
}


// returns len of tokens
int	set_len(const char *str, int i)
{
	int	len;

	len = 0;
	if (str[i] && (!check_in_quote(str, i) && !ft_isspace(str[i])))
	{
		len = move_o_unquoted(str, i);
		i += len;
	}
	if (len == 0 && str[i] && char_is_quote(str[i]))
	{
		len = move_over_quote(str, i);
		i += len;
	}
	else if (len == 0 && str[i] && !ft_isspace(str[i]) && char_is_red(str[i]))
	{
		while (str[i] && !ft_isspace(str[i]) && char_is_red(str[i])
			&& !char_is_quote(str[i]))
		{
			len++;
			i++;
		}
	}
	return (len);
}

//*check_token->value == '\0'
// deletes empty tokens from the tokenlist
int	clean_lexer(t_tokenlist *tokenlist, size_t i)
{
	t_token	*check_token;

	p_printf("CLEAN LEXER: %p %i\n", tokenlist->tokens, tokenlist->total);
	test_tokens(tokenlist);
	while (i < tokenlist->total)
	{
		check_token = tokenlist->tokens[i];
		// if (!check_token || !check_token->value)
		// 	return (1);
		if (check_token && check_token->value && *check_token->value == '\0')
		{
			p_printf("DELETE?\n");
			tokenlist_delete(tokenlist, i);
		}
		else if (check_token->type == HEREDOC && lookahead(tokenlist, i))
			lookahead(tokenlist, i)->type = HEREDOC_DEL;
		i++;
	}
	return (0);
}

//e_printf("CHECK TOKEN = %s, POS = %d, ADDRESS = %p", check_token->value, i, &check_token);
// index lexer by traversing
int	index_lexer(t_tokenlist **tokenlist)
{
	size_t	i;
	t_token	*check_token;

	i = 0;
	// e_printf("TOTAL= %zu \n", (size_t)(*tokenlist)->element_list.total);
	while (i < (size_t)(*tokenlist)->total)
	{
		check_token = (t_token *)(*tokenlist)->tokens[i];
		if (!check_token)
			return (write(1, "No tokens available\n", 20));
		else 
			check_token->pos = i;
		i++;
	}
	return (0);
}

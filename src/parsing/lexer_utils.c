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

int	skip_blanks(
	char *str, 
	int pos)
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
char	*trim_str_space(
	char *str)
{
	int	i;
	int	j;

	i = 0;
	j = ft_strlen(str) - 1;
	while (ft_isspace(str[j] && (str[j] != '\'' || str[j] != '"')))
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

int move_multiple_quotes(
	char *str,
	int pos,
	char quote,
	int i)
{
	int count;

	count = 0;
	while (str[pos])
	{
		while (str[pos] == quote)
		{
			count++;
			pos++;
			i++;
		}
		while(str[pos] != quote)
		{
			i++;
			pos++;
		}
		while (str[pos] && str[pos] == quote)
		{
			count--;
			pos++;
			i++;
		}
		pos++;
	}
	return (i);
}


//move over quoted string
int move_over_quote(
	char *str, 
	int pos)
{
    int i;
	int count;
	count = 1;
    i = 0;
    if (str[pos] == '"' || str[pos] == '\'')
	{
        char quote = str[pos];
        pos++;
		while (str[pos++] == quote)
			count++;
        while (str[pos] && str[pos] != quote)
        {
            i++;
            pos++;
		}
        if (str[pos] == quote)
            pos++;
		while (str[pos++] && !ft_isspace(str[pos]))
			i++;
    }
    if (i > 0)
        i += (count *2);
    return(i);
}

//returns len of tokens
int set_len(char *str, int i)
{
	int len;

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
	return (len);
}

//index lexer by traversing - not needed anymore
int index_lexer(
	element **tokenlist)
{
    size_t i;
    i = 0;
	//e_printf("TOTAL= %zu \n", (size_t)(*tokenlist)->element_list.total);
    while (i < (size_t)(*tokenlist)->element_list.total)
    {
        t_token *check_token;
		check_token = (t_token *)(*tokenlist)->element_list.tokens[i];
		if (!check_token)
			return (write(1, "No tokens available\n", 20));
		check_token->pos = i;
		//e_printf("POS= %d \n", check_token->pos);
        i++;
    }
    return (0);
}


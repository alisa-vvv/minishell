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

int	is_EOS(char *str, int i)
{
	if (i == ft_strlen(str))
		return (1);
	return (0);
}

int	skip_blanks(char *str, int pos)
{
	int	count;

	count = 0;
	while (!check_in_quote(str, pos) && ft_isspace(*str))
	{
		count++;
		str++;
	}
    // printf("%d\n", count);
	return (count);
}

// trims input to have one space between tokens to ease validation
// unless the input is quoted
char	*trim_str_space(char *str)
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
    // printf("%s\n", str);
	return (str);
}

// make a new token in the array
t_token	*new_token(char *str, int len)
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
	return (token);
}

int move_over_quote(char *str, int pos)
{
    int i;
    i = 0; 
    if (str[pos] == '"' || str[pos] == '\'')
	{
        char quote = str[pos];
        pos++;
        while (str[pos] && str[pos] != quote)
        {
            i++;
            pos++;
        }
        if (str[pos] == quote)
            pos++;
    }
    if (i > 0)
        i += 2;
    return(i);
}

// counts args to size up elementlist
int	token_count(char *str)
{
	int	tokencount;
	int	i;
	i = 0;
	tokencount = 0;
	while (str[i])
	{
		//i += skip_blanks(str, i);
		if ((str[i] == '"' || str[i] == '\'') && str[i])
		{
			tokencount++;
            i += move_over_quote(str, i);
		}
		else if ((!check_in_quote(str, i) && !ft_isspace(str[i]) && str[i]))
		{
			tokencount++;
			while (!ft_isspace(str[i]) && str[i])
				i++;
		}
        else 
            i++;
	}
    printf("tokencount = %d\n", tokencount);
	return (tokencount);
}


// pushes tokens in the elementlist from the back, immediately indexing
int	fill_tokenlist(element *tokenlist, char *str)
{
	int		i;
	size_t	len;
	t_token	*token;

	i = 0;
	while (str[i])
	{
		len = 0;
        if ((str[i] == '\'' || str[i] == '"') && str[i])
        {
            len = move_over_quote(str, i);
            i += len;
        }
		else if (len == 0)
        {
            while (str[i] && !ft_isspace(str[i]))
            {
                len++;
                i++;
            }
        }
		if (len > 0)
		{
			printf("len = %zu ", len);
            token = new_token(str + i - len, len + 1);
			if (!token)
				return (tokenlist->pf_element_free(tokenlist), write(1, MALLOC_ERR, 15));
			token->pos = tokenlist->element_list.total;
			tokenlist->pf_element_add(tokenlist, token);
		}
        if (!check_in_quote(str, i) && str[i] && ft_isspace(str[i]))
            i++;
        printf("i = %d\n", i);
	}
	return (0);
}

// default option to put trimmed input in tokenlist
int	default_lexer(char *input_line)
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
	i = 0;
	while (i < token_list.element_list.total)
	{
		token_test = (t_token *)token_list.element_list.tokens[i];
		printf("%s\n", token_test->value);
		i++;
	}
	return (0);
}

// set a value to the token so we can expand on those later
int	match_token(char *str_token)
{
	if (str_token[0] == 34)
		return (DOUBLE_Q_OPEN);
	else if (str_token[0] == 39)
		return (SINGLE_Q_OPEN);
	else if (str_token[0] == '(' || str_token[0] == '{')
		return (OPEN_BRACKET);
	else if (str_token[0] == '-')
		return (HYPHEN);
	else if ((str_token[0] == '>') || (str_token[0] == '<')
		|| (str_token[0] == '$') || (str_token[0] == '|')
		|| (str_token[0] == '.') || (str_token[0] == '!'))
		return (NON_TERMINAL);
	else if (ft_isdigit(str_token[0]))
		return (NUMBER);
	else if (ft_isalpha(str_token[0]))
		return (STRING);
	else
		return (UNKNOWN);
}

"et" "tu" "Brutu"
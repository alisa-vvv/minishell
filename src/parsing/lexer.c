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

int is_EOS(char *str, int i)
{
    if (i == ft_strlen(str))
        return (1);
    return (0);
}

void	ft_safefree(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
	return ;
}

void	ft_free_arr(void **array)
{
	int	i;

	if (array)
	{
		i = -1;
		while (array[++i])
			ft_safefree(array[i]);
		ft_safefree(array);
	}
}

t_token *new_token(char *str, int len)
{
    if (!*str)
        return (NULL);
    t_token *token;
    token = malloc(1 * sizeof(t_token));
    if (!token)
        return (MALLOC_ERR);
    token->value = malloc((len + 1) * sizeof(char));
    token->value = ft_strlcpy(token->value, str, len);
    if (!token->value)
        return (MALLOC_ERR);
//    token->type = match(token->value);
    return (token);
}

//counts args to size up elementlist
int token_count(char *str)
{
    int tokencount;
    tokencount = 0;

    while (*str)
    {
        if (ft_isspace(*str))
            str++;
        else
        {
            tokencount++;
            while (!ft_isspace(*str) && *str)
            {
                str++;
            }
        }
    }
    return (tokencount);
}

//pushes tokens in the elementlist from the back
int fill_tokenlist(element **tokenlist, char *str)
{
    size_t len;
    t_token *token;

    while (*str)
    {
        len = 0;
        while (*str && ft_isspace(*str))
            str++;
        while (*str && !ft_isspace(*str))
        {
            len++;
            str++;
        }
        if (len > 0)
        {
            token = new_token(str - len, len + 1);
            if (!token)
            {
                (*tokenlist)->pfElementFree(*tokenlist);
                return (MALLOC_ERR);
            }
            (*tokenlist)->pfElementAdd(*tokenlist, token);
        }
    }
    return (0);
}

//default option to put input as token into dyn array elementlist
int default_lexer(char *input_line)
{
    int token_c;
    token_c = 0;
    if (!input_line)
        return (1);
    token_c = token_count(input_line);
    if (!token_c)
        return (1);
    TOKEN_INIT(token_list, token_c);
    fill_tokenlist(&token_list, input_line);
    if (!token_list.elementList.tokens)
        return (1);
    size_t i = 0;
    t_token *token_test;

    while (i < token_list.elementList.total)
    {
        token_test = (t_token *)token_list.elementList.tokens[i];
        printf("%s\n", token_test->value);
        i++;
    }
    return (0);
}
//set a value to the token so we can expand on those later
int match_token(char *str_token)
{
    if (str_token[0] == 34)
        return DOUBLE_Q_OPEN;
    else if (str_token[0] == 39)
        return (SINGLE_Q_OPEN);
    else if (str_token[0] == '$')
        return (DOLLAR_SIGN);
    else if (str_token[0] == '!')
        return (EXCLAM_MARK);
    else if (str_token[0] == 92)
        return (BACKW_SLASH);
    else if ((str_token[0] == '>') && (str_token[1] == '>'))
        return (REDIRECT);
    else if ((str_token[0] == '.') && (str_token[1] == '.'))
        return (DOUBLE_DOT);
    else if (str_token[0] == '.')
        return (DOT);
    else if (str_token[0] == '|')
        return (PIPE);
    else 
        return (STRING);
}

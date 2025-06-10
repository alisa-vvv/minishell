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
            token = new_token(str - len, len);
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
        return (NULL);
    printf("%s\n", (char *)token_list.pfElementGet(&token_list, 0));
    return (0);
}



// int match_token(char *str_read)
// {
//     int i;
//     i = 0; 
//     if (str_read[i] == '"')
//         return DOUBLE_Q_OPEN;
//     else if (str_read[i] == '')
//         return 

// }

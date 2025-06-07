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

void	ft_freearr(void **array)
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

void push_token(t_token *token, t_token **tlist)
{
    t_token *last;
    if (!tlist)
    {
        *tlist = token;
        return ;
    }
    while ((*tlist)->next)
        last = (*tlist)->next;
    last = token; 
}

t_token *new_token(char *str, int pos, int len)
{
    t_token *token;
    token = ft_calloc(1, sizeof(t_token));
    if (!token)
        return (MALLOC_ERR);
    token->value = ft_strlcpy(token->value, str[pos], len);
    if (!token->value)
        return (MALLOC_ERR);
    token->type = match(token->value);
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

void ft_clear_list(t_token **list)
{
    while ((*list)->next)
        ft_safefree()

}

int fill_tokenlist(t_token **tokenlist, char *str)
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
            token = new_token(str, str - len, len + 1);
            if (!token)
            {
                ft_clear_list(tokenlist);
                return (MALLOC_ERR);
            }
            push_token(token, tokenlist);
        }
    }
    return (0);
}


int default_lexer(char *input_line)
{
    int token_c;
    t_token *token_list;
    //input_line = readline("minishell");
    if (!input_line)
        return (1);
    token_c = token_count(input_line);
    token_list = ft_malloc();
    if (!token_list)
        return (NULL);
    

    return (0);
}

int match_token(char *str_read)
{
    int i;
    i = 0; 
    if (str_read[i] == '"')
        return DOUBLE_Q_OPEN;
    else if (str_read[i] == '"')

}

// t_token *regular_expression(char *str)
// {
//     int i;
//     i = 0; 
//     while(isspace(str[i]))
//         i++;
//     if 
// }
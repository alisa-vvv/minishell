#include "parser.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>

int	ft_isspace(int c)
{
	unsigned char	u_c;

	u_c = (unsigned char) c;
	if ((u_c >= '\t' && u_c <= '\r') || u_c == ' ')
		return (true);
	return (false);
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

int elementTotal(element *e)
{
    if (e)
        return (e->elementList.total);
    return (-1);
}

int elementResize(element *e, int size)
{
    if (e)
    {
        void **tokens = realloc(e->elementList.tokens, sizeof(void *) * size);
        if (tokens)
        {
            e->elementList.tokens = tokens;
            e->elementList.size = size;
            return (0);
        }
    }
    return (-1);
}

int elementPushBack(element *e, void *token)
{
    int status;
    status = -1;
    if (e)
    {
        if (e->elementList.size == e->elementList.total)
        {
            status = elementResize(e, e->elementList.size * 2);
            if (status == -1)
                e->elementList.tokens[e->elementList.total++] = token;
        }
        else 
        {
            e->elementList.tokens[e->elementList.total++] = token;
            status = 0;
        }
    }
    return (status);
}

int elementSet(element *e, int index, void *token)
{
    if (e)
    {
        if ((index >= 0) && ((size_t)index < e->elementList.total))
        {
            e->elementList.tokens[index] = token;
            return (0);
        }
    }
    return (-1);
}

void *elementGet(element *e, int index)
{
    void *token_data;
    token_data = NULL;
    if (e)
    {
        if ((index >= 0) && ((size_t)index < e->elementList.total))
            token_data = e->elementList.tokens[index];
    }
    return (token_data);
}

int elementDelete(element *e, int index)
{
    int i;
    int status; 
    i = 0;
    status = -1;
    if (e)
    {
        if ((index < 0) || ((size_t)index >= e->elementList.total))
            return (-1);
        e->elementList.tokens[index] = NULL;
        i = index;
        while ((size_t)i < e->elementList.total -1)
        {
            e->elementList.tokens[i] = e->elementList.tokens[i + 1];
            e->elementList.tokens[i+1] = NULL;
            i++;
        }
        e->elementList.total--;
        if ((e->elementList.total > 0) && ((e->elementList.total) == (e->elementList.size / 4)))
            elementResize(e, e->elementList.size /2);
        status = 0;
    }
    return (status);
}

int elementFree(element *e)
{
    if (e)
    {
        ft_free_arr(e->elementList.tokens);
        e->elementList.total = 0;
        e->elementList.tokens = NULL;
        return (0);
    }
    return (-1);
}

void element_init(element *e, int size)
{
    e->pfElementTotal = elementTotal;
    e->pfElementResize = elementResize;
    e->pfElementAdd = elementPushBack;
    e->pfElementSet = elementSet;
    e->pfElementGet = elementGet;
    e->pfElementFree = elementFree;
    e->pfElementDelete = elementDelete;

    e->elementList.size = size;
    e->elementList.total = 0;
    e->elementList.tokens = malloc(sizeof(void *) * e->elementList.size);
    if (!e->elementList.tokens)
    {
        fprintf(stderr, "Memory allocation failed in element_init\n");
        exit(EXIT_FAILURE);
    }
}

t_token *new_token(char *str, int len)
{
    if (!*str)
        return (NULL);
    t_token *token;
    token = malloc(1 * sizeof(t_token));
    if (!token)
        return (NULL);
    token->value = malloc((len + 1) * sizeof(char));
    ft_strlcpy(token->value, str, len);
    if (!token->value)
        return (NULL);
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

int fill_tokenlist(element *tokenlist, char *str)
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
                tokenlist->pfElementFree(tokenlist);
                return (1);
            }
            tokenlist->pfElementAdd(tokenlist, token);
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

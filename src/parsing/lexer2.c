/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   lexer2.c                                            :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/18 18:22:33 by adecheri       #+#    #+#                */
/*   Updated: 2025/06/18 18:22:35 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// func to traverse list and find a spec symbol
int	find_symbol(element *tokenlist, int pos, char symbol)
{
	t_token	*check_token;

	while ((size_t)pos < tokenlist->element_list.total)
	{
		check_token = (t_token *)tokenlist->element_list.tokens[pos];
		if (ft_strchr(check_token->value, symbol) != NULL)
			return (pos);
		pos++;
	}
	return (-1);
}

// count occurrence of spec symbol in array
int	count_symbol(element *tokenlist, int pos, char symbol)
{
	int		count;
	int		i;
	t_token	*check_token;

	check_token = tokenlist->pf_element_get(tokenlist, pos);
	count = 0;
	i = 0;
	while ((size_t)pos < tokenlist->element_list.total)
	{
		if (find_symbol(tokenlist, pos, symbol) > -1)
		{
			while (check_token->value[i])
			{
				if (check_token->value[i] == symbol)
					count++;
				i++;
			}
		}
		pos++;
	}
	return (count);
}

// //index lexer by traversing - not needed anymore
// int index_lexer(element **tokenlist)
// {
//     int i;
//     i = 0;

//     while (i < (size_t)(*tokenlist)->elementList.total)
//     {
//         (*tokenlist)->elementList.tokens[i].pos = i;
//         i++;
//     }
//     return (0);
// }

// check if left and right are args for pipe
int	check_pipe(char *str)
{
	int	command;
    int i; 
    i = 0;
	command = 0;
	if (ft_strchr(str, '|') != (void *)0)
	{
		while (&str[i] <= ft_strchr(str, '|'))
		{
			if (ft_isalpha(str[i]))
				command++;
			i++;
		}
		if (!command)
			return (1);
		// check for single pipe
		if (str[i] == '|' && (str + i + 1 == (void *)0 || *str + i + 2 == '"'))
			return (1);
	}
    return (0);
}

// validate input on quotes, pipes
int	val_inputline(char *str)
{
	if (check_in_quote(str, ft_strlen(str) -1))
	    return (write(1, "command not found\n", 18));
	if (check_pipe(str))
		return (write(1, "command not found\n", 18));
    return(0);
}

int	val_redirects(element **tokenlist)
{
	size_t	i;
	t_token	*check_token;

	i = 0;
	check_token = (t_token *)(*tokenlist)->element_list.tokens[i];
	while (i < (size_t)(*tokenlist)->element_list.total)
	{
		if (check_token->type == REDIRECT_IN)
		{
			if (lookahead(tokenlist, i) != STRING && lookahead(tokenlist, i) != PARAMETER)
				return (1);
			//check if leading arg is something or nothing 
		}
		else if (check_token->type == REDIRECT_OUT || REDIRECT_OUT_APP)
		{
			if ()
		}
		i++;
	}
	return (0);
}

int check_lexer(element **tokenlist)
{
	size_t i;
	t_token *check_token;
    i = 0;

	check_token = (t_token *)(*tokenlist)->element_list.tokens[i];
    while (i < (size_t)(*tokenlist)->element_list.total)
    {
        if (check_token->type == PARAMETER)
			expand_var(tokenlist, i);

        i++;
    }

}


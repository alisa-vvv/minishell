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
t_token *new_token(
	element *tokenlist,
	char *str, 
	int len)
{
	t_token	*token;

	if (!*str)
		return (NULL);
	token = calloc(1, sizeof(t_token));
	if (!token)
		return (tokenlist->pf_element_free(tokenlist), write(1, MALLOC_ERR, 15), NULL);
	token->value = malloc((len + 1) * sizeof(char));
	ft_strlcpy(token->value, str, len);
	p_printf("TOKEN VALUE = $%s$\n", token->value);
	if (!token->value)
		return (NULL);
	token->type = match_token(token->value);
	token->command = false;
	token->pos = tokenlist->element_list.total;
	return (token);
}

//returns part of the string before redirect token
char *before_red(char *str_token)
{
	char *result;
	int i; 

	i = 0;
	result = NULL;
	result = ft_strdup(str_token);
	if (!result)
		return (NULL);
	while (result[i])
	{
		if (char_is_red(str_token[i]))
			break;
		i++;
	}
	if (i == 0)
	{
		ft_strlcpy(result, "-1", 3);
		return (result);
	} 
	result[i] = '\0';
	return (result);
}

int l_red(char *str)
{

	if (str[0] == str[1] && str[2] != str[1] && char_is_red(str[0]))
		return (2);
	else if (char_is_red(str[0]) && str[0] != str[1])
		return (1);
	else 
		return (-1);
}


//add new token to the list and updates total
int add_token(
	element *tokenlist, 
	char *str, 
	size_t len)
{
	t_token	*token;

	token = NULL;
	// l_printf("len = %zu ", len);
	token = new_token(tokenlist, str, len);
	if (!token)
		return (tokenlist->pf_element_free(tokenlist), write(1, MALLOC_ERR, 15));
	tokenlist->pf_element_add(tokenlist, token);
	return (0);
}


//splits redirect tokens into 3 tokens "-1" if not specified 
int split_redir(element *tokenlist,
	char *str_b_token)
{
	char *str_piece;
	int len;

	str_piece = before_red(str_b_token);
	if (!str_piece)
		return (write(1, MALLOC_ERR, 16));
	if (add_token(tokenlist, str_piece, ft_strlen(str_piece) + 1))
		return(ft_safe_free((unsigned char **)&str_piece), 1);
	if (ft_strncmp(str_piece, "-1", 2) == 0)
	{
		len = l_red(str_b_token);
		str_piece[0] = '\0';
	}
	else
		len = l_red(str_b_token + (ft_strlen(str_piece)));
	if (len < 0)
		return(ft_safe_free((unsigned char **)&str_piece), 1);
	if (add_token(tokenlist, str_b_token + ft_strlen(str_piece), len + 1))
		return(ft_safe_free((unsigned char **)&str_piece), 1);
	if (str_contains_red(str_b_token + (ft_strlen(str_piece) + len)))
		split_redir(tokenlist, str_b_token + (ft_strlen(str_piece) + len));
	else if (add_token(tokenlist, str_b_token + (ft_strlen(str_piece) + len), ft_strlen(str_b_token) - ft_strlen(str_piece) - len + 1))
		return(ft_safe_free((unsigned char **)&str_piece), 1);
	ft_safe_free((unsigned char **)&str_piece);
	return (0);
}


//preps str for redirect splitting and adding tokens
int prep_token(element *tokenlist,
	char *str,
	int i, 
	int len)
{
	char *str_b_token;

	str_b_token = NULL;
	str_b_token = malloc((len + 1) * sizeof(char));
	if (!str_b_token)
		return (1);
	ft_strlcpy(str_b_token, str + i - len, len +1);
	if (str_contains_red(str_b_token))
	{
		if (split_redir(tokenlist, str_b_token))
			return(ft_safe_free((unsigned char **)&str_b_token), 1);

	}
	else if (add_token(tokenlist, str_b_token, len +1))
		return(ft_safe_free((unsigned char **)&str_b_token), 1);
	ft_safe_free((unsigned char **)&str_b_token);
	return (0);
}



//returns len of unquoted token
int move_o_unquoted(const char *str, int i)
{
	int len;

	len = 0;
	
	while (str[i] && !char_is_quote(str[i]) && !ft_isspace(str[i]) && !char_is_red(str[i]))
	{
		len++;
		i++;
	}
	if (str[i] && char_is_quote(str[i]) && str[i -1] == '=')
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

int count_unq(const char *str, int i, int count)
{
	
	if (!char_is_red(str[i]))
		count++;
	while (str[i] && !char_is_quote(str[i]) && !ft_isspace(str[i]))
	{
		if (char_is_red(str[i]))
		{
			count++;
			if ((char_is_red(str[i]) && i == 0 ) || (i > 0 && ft_isspace(str[i-1]) && char_is_red(str[i])))
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

// counts args to size up elementlist
int	token_count(
	const char *str,
	int i)
{
	int	tokencount;
	
	tokencount = 0;
	while (str[i])
	{
		if ((str[i] && !check_in_quote(str, i) && !ft_isspace(str[i])) && !char_is_quote(str[i]))
		{
			tokencount = count_unq(str, i, tokencount); 
			i += move_o_unquoted(str, i) ;
		}
		else if (str[i] && char_is_quote(str[i]))
		{
			tokencount++;
            i += move_over_quote(str, i);
		}
		else if (str[i] && !ft_isspace(str[i]) && char_is_red(str[i]))
		{
			tokencount++;
			while(str[i] && char_is_red(str[i]) && !ft_isspace(str[i]) && !char_is_quote(str[i]))
				i++;
		}
        else if (str[i])
            i++;
	}
    t_printf("Token count = %d\n", tokencount);
	return (tokencount);
}


// pushes tokens in the elementlist from the back, immediately indexing
int	fill_tokenlist(
	element *tokenlist, 
	char *str)
{
	int		i;
	size_t	len;


	i = 0;
	len = 0;
	if (!str)
		return(1);
	while (str[i])
	{
		len = set_len(str, i);
		i += len;
		if (len > 0)
		{
			if (prep_token(tokenlist, str, i, len))
				return (1);
		}
        else if (str[i])
            i++;
	}
	return (0);
}

// default option to put trimmed input in tokenlist
int	default_lexer(
	char *input_line, 
	t_msh_data *msh_data)
{
	int		token_c;
	element token_list;

	token_c = 0;
	if (!input_line)
		return (1);
	input_line = trim_str_space(input_line);
	if (val_inputline(input_line))
        return (1);
	token_c = token_count(input_line, 0);
	if (!token_c)
		return (write(1, "Failed to count tokens\n", 23));
	element_init(&token_list, token_c);
	fill_tokenlist(&token_list, input_line);
	token_list.element_list.tokens[token_c] = NULL;  
	if (!token_list.element_list.tokens)
		return (write(1, "Failed to init tokenlist\n", 25));
//	test_tokens(token_list);
	if (check_lexer(&token_list, msh_data))
	{
		token_list.pf_element_free(&token_list);
		return (write(1, "Failed check types\n", 19));
	}
	token_list.pf_element_free(&token_list);
	ft_safe_free((unsigned char **)&token_list);
	return (0);
}

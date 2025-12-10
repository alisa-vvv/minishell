/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   expansions_utils2.c                                 :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/12/10 15:19:25 by adecheri       #+#    #+#                */
/*   Updated: 2025/12/10 15:19:27 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"


//get the difference of the str when exp variables
int	get_offset(t_exp_data *exp_data)
{
	int	offset;

	offset = 0;
	if (exp_data->env_value && exp_data->name)
		offset = ft_strlen(exp_data->name) + 1;
	else if (!exp_data->env_value && !exp_data->name)
	{
		exp_data->env_value = "";
		offset = 1;
	}
	else if (!exp_data->env_value && exp_data->name)
	{
		exp_data->env_value = "";
		offset = ft_strlen(exp_data->name) + 1;
	}
	else
		offset = 1;
	return (offset);
}

// prepare leftover parts of str token to keep for the updated string
char	*prep_leftover(char *str_token, char *start, int offset)
{
	char	*temp_left;
	size_t	rem_len;

	if (!str_token)
		return (NULL);
	if (!start)
		return (NULL);
	rem_len = ft_strlen(start + offset) + 1;
	temp_left = ft_calloc(rem_len, sizeof(char));
	if (!temp_left)
		return (msh_perror(NULL, MALLOC_ERR, extern_err), NULL);
	ft_strlcpy(temp_left, start + offset, rem_len + 1);
	*start = '\0';
	return (temp_left);
}

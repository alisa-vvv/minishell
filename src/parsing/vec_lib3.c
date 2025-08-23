/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   vec_lib3.c                                          :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/08/23 13:20:41 by adecheri       #+#    #+#                */
/*   Updated: 2025/08/23 13:20:42 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	element_total(element *e)
{
	if (e)
		return (e->element_list.total);
	return (-1);
}

int element_swap(element *e, void *token, size_t pos_dest, size_t pos_src)
{
    int status; 
    void *tmp;

    status = -1;
    while (pos_src > pos_dest)
    {
        e->element_list.tokens[pos_src -1] = e->element_list.tokens[pos_src + 1];
        pos_src--;
    }
    e->element_list.tokens[pos_dest] = token;
    return (0);
}


//insert into 
int	element_insert(element *e, void *token, int pos)
{
	int	status;

	status = -1;
	if (e)
	{
		if (e->element_list.size == e->element_list.total)
		{
			status = element_resize(e, e->element_list.size, e->element_list.size
					* 2);
			if (status == -1)
				e->element_list.tokens[e->element_list.total++] = token;
		}
		else
		{
			e->element_list.tokens[e->element_list.total++] = token;
            element_swap(e, e->element_list.tokens[e->element_list.total], pos + 1, (size_t)e->element_list.total);
			status = 0;
		}
	}
	return (status);
}

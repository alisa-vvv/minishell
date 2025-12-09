/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   parser2.c                                           :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/12/09 12:51:47 by adecheri       #+#    #+#                */
/*   Updated: 2025/12/09 12:51:49 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"


int prep_execdata(t_tokenlist *tlist, t_msh_data *msh_data)
{
    t_pos *xpos;
    int err;

    err = success;
    xpos = NULL;
    if (tlist)
	{
        set_pipe_cm(tlist, 0);
		xpos = ft_calloc(1, sizeof(t_pos));
		if (!xpos)
			return (msh_perror(NULL, MALLOC_ERR, parse_err), malloc_err);
        err = pass_comm(tlist, msh_data, xpos);
	}
	free(xpos);
    return (err);
}

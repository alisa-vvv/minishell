/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   vec_lib.h                                           :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/29 16:20:56 by adecheri       #+#    #+#                */
/*   Updated: 2025/06/29 16:20:58 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC_LIB_H
# define VEC_LIB_H

# include <stdbool.h>
# include <stddef.h>
// # include "parser.h"

typedef struct s_tokenlist
{
	void	**tokens;
	size_t	size;
	size_t	total;
}			t_tokenlist;

int			tokenlist_total(t_tokenlist *e);
int			tokenlist_resize(t_tokenlist *e, int oldsize, int newsize);
int			tokenlist_add(t_tokenlist *e, void *token);
int			tokenlist_set(t_tokenlist *e, size_t index, void *token);
void		*tokenlist_get(t_tokenlist *e, size_t index);
int			tokenlist_delete(t_tokenlist *e, size_t index);
void		tokenlist_free(t_tokenlist *e);
int			tokenlist_init(t_tokenlist **e, int size);
int			ft_free_s_token(void **check_token);

#endif

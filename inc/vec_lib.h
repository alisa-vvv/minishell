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

# include <stddef.h>
# include <stdbool.h>
// # include "parser.h"


	typedef struct s_tokenlist
	{
		void					**tokens;
		size_t 					size;
		size_t					total;
	}							t_tokenlist;

	//typedef struct s_element	tokenlist;
	// thoughts on this: remove all pf_element_* functions from the struct, not sure why we need them
	// just use the element_* functiuons instead
	// element_get should just return a t_token pointer since we're not using this for anything else


	int						tokenlist_total(t_tokenlist *e);
	int						tokenlist_resize(t_tokenlist *e, int oldsize, int newsize);
	int						tokenlist_add(t_tokenlist *e, void *token);
	int						tokenlist_set(t_tokenlist *e, size_t index, void *token);
	void					*tokenlist_get(t_tokenlist *e, size_t index);
	int						tokenlist_delete(t_tokenlist *e, size_t index);
	void					tokenlist_free(t_tokenlist *e);
	int						tokenlist_init(t_tokenlist **e, int size);
	int						ft_free_s_token(void **check_token);
	// int					element_swap(element *e, void *token, size_t pos1,
	// 						size_t pos2);
	// int					element_insert(element *e, void *token, int pos);

#endif

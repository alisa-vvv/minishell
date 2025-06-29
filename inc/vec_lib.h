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

# define TOKEN_INIT(el, size) element el;\
    element_init(&el, size)

typedef struct s_elementlist
{
    void **tokens;
    size_t size; // in mem
    size_t total;
} t_elementlist;


typedef struct s_element element;

struct s_element
{
    t_elementlist element_list;
    int (*pf_element_total)(element *);
    int (*pf_element_resize)(element *, int, int);
    int (*pf_element_add)(element *, void *);
    int (*pf_element_set)(element *, int, void *);
    void *(*pf_element_get)(element *, int);
    int (*pf_element_delete)(element *, int);
    int (*pf_element_free)(element *);
};

int element_total(element *e);
int element_resize(element *e, int oldsize, int newsize);
int element_push_back(element *e, void *token);
int element_set(element *e, int index, void *token);
void *element_get(element *e, int index);
int element_delete(element *e, int index);
int element_free(element *e);
void element_init(element *e, int size);

#endif
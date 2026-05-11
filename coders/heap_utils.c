/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmatthes <lmatthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 19:28:16 by lmatthes          #+#    #+#             */
/*   Updated: 2026/05/11 20:14:08 by lmatthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_init(t_heap *h, int capacity)
{
	h->data = malloc(sizeof(t_pq_node) * capacity);
	if (!h)
		return (1);
	h->size = 0;
	h->capacity = capacity;
	return (0);
}

void	heap_free(t_heap *h)
{
	if (h->data)
	{
		free(h->data);
		h->data = NULL;
	}
	h->size = 0;
	h->capacity = 0;
}

t_pq_node	heap_peek(t_heap *h)
{
	return (h->data[0]);
}

int	heap_is_empty(t_heap *h)
{
	return (h->size == 0);
}

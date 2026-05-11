/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmatthes <lmatthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 13:44:40 by lmatthes          #+#    #+#             */
/*   Updated: 2026/05/11 16:43:24 by lmatthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// heap_push(), heap_pop(), heap_peek(), heap_init().
// Pure data structure, no threading logic.

#include "codexion.h"

int	heap_init(t_heap *h, int capacity)
{
	(void)h;
	(void)capacity;
	return (0);
}

void	heap_push(t_heap *h, t_pq_node node)
{
	(void)h;
	(void)node;
}

t_pq_node	heap_pop(t_heap *h)
{
	t_pq_node	node;

	(void)h;
	node.coder_id = 0;
	node.priority = 0;
	return (node);
}

t_pq_node	heap_peek(t_heap *h)
{
	t_pq_node	node;

	(void)h;
	node.coder_id = 0;
	node.priority = 0;
	return (node);
}

int	heap_is_empty(t_heap *h)
{
	(void)h;
	return (1);
}

void	heap_free(t_heap *h)
{
	(void)h;
}

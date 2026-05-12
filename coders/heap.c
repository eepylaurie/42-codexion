/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmatthes <lmatthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 13:44:40 by lmatthes          #+#    #+#             */
/*   Updated: 2026/05/12 14:04:41 by lmatthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	swap_nodes(t_pq_node *a, t_pq_node *b)
{
	t_pq_node	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	sift_down(t_heap *h, int i)
{
	int	left;
	int	smallest;

	while (1)
	{
		left = 2 * i + 1;
		smallest = i;
		if (left < h->size && h->data[left].priority < h->data[smallest].priority)
			smallest = left;
		if (left + 1 < h->size && h->data[left + 1].priority < h->data[smallest].priority)
			smallest = left + 1;
		if (smallest == 1)
			break ;
		swap_nodes(&h->data[i], &h->data[smallest]);
		i = smallest;
	}
}

void	heap_push(t_heap *h, t_pq_node node)
{
	int			i;
	int			parent;

	if (h->size >= h->capacity)
		return ;
	h->data[h->size] = node;
	i = h->size;
	h->size++;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (h->data[parent].priority <= h->data[i].priority)
			break ;
		swap_nodes(&h->data[i], &h->data[parent]);
		i = parent;
	}
}

t_pq_node	heap_pop(t_heap *h)
{
	t_pq_node	min;

	min = h->data[0];
	h->size--;
	if (h->size == 0)
		return (min);
	h->data[0] = h->data[h->size];
	sift_down(h, 0);
	return (min);
}

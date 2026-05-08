/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmatthes <lmatthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 13:44:44 by lmatthes          #+#    #+#             */
/*   Updated: 2026/05/08 20:09:29 by lmatthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_one_dongle(t_dongle *d, int capacity)
{
	if (pthread_mutex_init(&d->mutex, NULL) != 0)
		return (1);
	if (pthread_cond_init(&d->cond, NULL) != 0)
	{
		pthread_mutex_destroy(&d->mutex);
		return (1);
	}
	if (heap_init(&d->wait_queue, capacity) != 0)
	{
		pthread_cond_destroy(&d->cond);
		pthread_mutex_destroy(&d->mutex);
		return (1);
	}
	d->taken = 0;
	d->release_time = 0;
	return (0);
}

static void	destroy_dongles_up_to(t_dongle *dongles, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		heap_free(&dongles[i].wait_queue);
		pthread_cond_destroy(&dongles[i].cond);
		pthread_mutex_destroy(&dongles[i].mutex);
		i++;
	}
}

static int	init_sim_mutexes(t_sim *sim)
{
	if (pthread_mutex_init(&sim->stop_mutex, NULL) != 0)
	{
		fprintf(stderr, "Error: failed to init stop_mutex\n");
		return (1);
	}
	if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
	{
		fprintf(stderr, "Error: failed to init log_mutex\n");
		pthread_mutex_destroy(&sim->stop_mutex);
		return (1);
	}
	return (0);
}

static int	init_dongles(t_sim *sim)
{
	int	i;

	sim->dongles = malloc(sizeof(t_dongle) * sim->number_of_coders);
	if (!sim->dongles)
		return (1);
	i = 0;
	while (i < sim->number_of_coders)
	{
		if (init_one_dongle(&sim->dongles[i], sim->number_of_coders) != 0)
		{
			destroy_dongles_up_to(sim->dongles, i);
			free(sim->dongles);
			sim->dongles = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}

int	init_sim(t_sim *sim)
{
	if (init_sim_mutexes(sim) != 0)
		return (1);
	if (init_dongles(sim) != 0)
	{
		pthread_mutex_destroy(&sim->stop_mutex);
		pthread_mutex_destroy(&sim->log_mutex);
		return (1);
	}
	return (0);
}

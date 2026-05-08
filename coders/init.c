/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmatthes <lmatthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 13:44:44 by lmatthes          #+#    #+#             */
/*   Updated: 2026/05/08 16:04:40 by lmatthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// allocates coders and dongles arrays,
// initializes all mutexes and condvars,
// sets up dongle assignments (left/right per coder),
// initializes heaps.

#include "codexion.h"

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

int	init_sim(t_sim *sim)
{
	if (init_sim_mutexes(sim) != 0)
		return (1);
	return (0);
}

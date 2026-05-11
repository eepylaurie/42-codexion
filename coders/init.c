/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmatthes <lmatthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 13:44:44 by lmatthes          #+#    #+#             */
/*   Updated: 2026/05/11 16:40:38 by lmatthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static int	init_coders(t_sim *sim)
{
	int	i;

	sim->coders = malloc(sizeof(t_coder) * sim->number_of_coders);
	if (!sim->coders)
		return (1);
	i = 0;
	while (i < sim->number_of_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].thread = 0;
		sim->coders[i].left_dongle = i;
		sim->coders[i].right_dongle = (i + 1) % sim->number_of_coders;
		sim->coders[i].last_compile_start = 0;
		sim->coders[i].compile_count = 0;
		sim->coders[i].seq_num = 0;
		sim->coders[i].sim = sim;
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
		cleanup_sim(sim);
		return (1);
	}
	if (init_coders(sim) != 0)
	{
		cleanup_sim(sim);
		return (1);
	}
	return (0);
}

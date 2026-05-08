/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmatthes <lmatthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 16:11:15 by lmatthes          #+#    #+#             */
/*   Updated: 2026/05/08 13:15:48 by lmatthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	start_threads(t_sim *sim)
{
	int	i;

	sim->start_ms = get_current_ms();
	i = 0;
	while (i < sim->number_of_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
				coder_routine, &sim->coders[i]) != 0)
		{
			set_sim_stopped(sim);
			return (1);
		}
		i++;
	}
	if (pthread_create(&sim->monitor_thread, NULL,
			monitor_routine, sim) != 0)
	{
		set_sim_stopped(sim);
		return (1);
	}
	return (0);
}

static void	wake_all_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].mutex);
		pthread_cond_broadcast(&sim->dongles[i].cond);
		pthread_mutex_unlock(&sim->dongles[i].mutex);
		i++;
	}
}

static void	join_all(t_sim *sim)
{
	int	i;

	pthread_join(sim->monitor_thread, NULL);
	wake_all_dongles(sim);
	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_sim	sim;

	memset(&sim, 0, sizeof(t_sim));
	if (argc != 9)
	{
		fprintf(stderr, "Usage: ./codexion number_of_coders time_to_burnout "
			"time_to_compile time_to_debug time_to_refactor "
			"number_of_compiles_required dongle_cooldown scheduler\n");
		return (1);
	}
	if (parse_args(argc, argv, &sim) != 0)
		return (1);
	if (init_sim(&sim) != 0)
		return (1);
	if (start_threads(&sim) != 0)
	{
		join_all(&sim);
		cleanup_sim(&sim);
		return (1);
	}
	join_all(&sim);
	cleanup_sim(&sim);
	return (0);
}

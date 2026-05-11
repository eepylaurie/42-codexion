/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmatthes <lmatthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 13:44:54 by lmatthes          #+#    #+#             */
/*   Updated: 2026/05/11 18:19:43 by lmatthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_burnout(t_sim *sim)
{
	long	now;
	long	elapsed;
	int		i;

	now = get_timestamp_ms(sim->start_ms);
	i = 0;
	while (i < sim->number_of_coders)
	{
		elapsed = now - sim->coders[i].last_compile_start;
		if (elapsed >= sim->time_to_burnout)
		{
			log_burnout(sim, sim->coders[i].id);
			set_sim_stopped(sim);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	check_all_done(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		if (sim->coders[i].compile_count < sim->number_of_compiles_required)
			return (0);
		i++;
	}
	set_sim_stopped(sim);
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (!simulation_stopped(sim))
	{
		if (check_burnout(sim))
			return (NULL);
		if (check_all_done(sim))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}

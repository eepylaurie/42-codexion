/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmatthes <lmatthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 13:44:35 by lmatthes          #+#    #+#             */
/*   Updated: 2026/05/11 18:49:02 by lmatthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	dongle_not_available(t_dongle *dongle, t_sim *sim)
{
	long	elapsed;

	if (dongle->taken)
		return (1);
	elapsed = get_timestamp_ms(sim->start_ms) - dongle->release_time;
	if (elapsed < sim->dongle_cooldown)
		return (1);
	return (0);
}

static void	wait_for_dongle(t_dongle *dongle, t_sim *sim)
{
	struct timespec	ts;
	long			now_ms;
	long			wait_ms;

	if (dongle->taken)
	{
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
		return ;
	}
	wait_ms = sim->dongle_cooldown - (get_timestamp_ms(sim->start_ms) - dongle->release_time);
	if (wait_ms <= 0)
		return ;
	now_ms = get_current_ms() + wait_ms;
	ts.tv_sec = now_ms / 1000;
	ts.tv_nsec = (now_ms % 1000) * 1000000L;
	pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
}

int	acquire_dongle(t_dongle *dongle, t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	pthread_mutex_lock(&dongle->mutex);
	while (dongle_not_available(dongle, sim) && !simulation_stopped(sim))
		wait_for_dongle(dongle, sim);
	if (simulation_stopped(sim))
	{
		pthread_mutex_unlock(&dongle->mutex);
		return (1);
	}
	dongle->taken = 1;
	pthread_mutex_unlock(&dongle->mutex);
	log_msg(sim, coder->id, "has taken a dongle");
	return (0);
}

void	release_dongle(t_dongle *dongle, t_sim *sim)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->taken = 0;
	dongle->release_time = get_timestamp_ms(sim->start_ms);
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmatthes <lmatthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 13:45:00 by lmatthes          #+#    #+#             */
/*   Updated: 2026/05/08 13:47:30 by lmatthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_current_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000);
}

long	get_timestamp_ms(long start_ms)
{
	return (get_current_ms() - start_ms);
}

void	precise_sleep(long duration_ms, t_sim *sim)
{
	long	target;

	target = get_current_ms() + duration_ms;
	while (get_current_ms() < target && !simulation_stopped(sim))
		usleep(200);
}

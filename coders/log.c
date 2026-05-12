/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmatthes <lmatthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 13:44:48 by lmatthes          #+#    #+#             */
/*   Updated: 2026/05/12 14:56:24 by lmatthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_msg(t_sim *sim, int coder_id, char *msg)
{
	long	timestamp;

	pthread_mutex_lock(&sim->log_mutex);
	timestamp = get_timestamp_ms(sim->start_ms);
	if (!simulation_stopped(sim))
		printf("%ld %d %s\n", timestamp, coder_id, msg);
	pthread_mutex_unlock(&sim->log_mutex);
}

void	log_burnout(t_sim *sim, int coder_id)
{
	long	timestamp;

	pthread_mutex_lock(&sim->log_mutex);
	timestamp = get_timestamp_ms(sim->start_ms);
	printf("%ld %d burned out\n", timestamp, coder_id);
	pthread_mutex_unlock(&sim->log_mutex);
}

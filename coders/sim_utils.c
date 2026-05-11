/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmatthes <lmatthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 17:19:42 by lmatthes          #+#    #+#             */
/*   Updated: 2026/05/11 17:20:13 by lmatthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_sim_stopped(t_sim *sim)
{
	pthread_mutex_lock(&sim->stop_mutex);
	sim->stopped = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
}

int	simulation_stopped(t_sim *sim)
{
	int	stopped;

	pthread_mutex_lock(&sim->stop_mutex);
	stopped = sim->stopped;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (stopped);
}

long	next_seq_num(t_sim *sim)
{
	(void)sim;
	return (0);
}

long	get_coder_deadline(t_coder *coder)
{
	(void)coder;
	return (0);
}

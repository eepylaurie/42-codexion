/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmatthes <lmatthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 13:44:26 by lmatthes          #+#    #+#             */
/*   Updated: 2026/05/11 17:20:16 by lmatthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	cleanup_sim(t_sim *sim)
{
	if (sim->coders)
	{
		free(sim->coders);
		sim->coders = NULL;
	}
	destroy_all_dongles(sim);
	pthread_mutex_destroy(&sim->stop_mutex);
	pthread_mutex_destroy(&sim->log_mutex);
}

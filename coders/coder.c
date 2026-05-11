/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmatthes <lmatthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 13:44:30 by lmatthes          #+#    #+#             */
/*   Updated: 2026/05/11 18:05:08 by lmatthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	take_both_dongles(t_coder *coder)
{
	int	first;
	int	second;

	if (coder->left_dongle < coder->right_dongle)
	{
		first = coder->left_dongle;
		second = coder->right_dongle;
	}
	else
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	if (acquire_dongle(&coder->sim->dongles[first], coder) != 0)
		return (1);
	if (acquire_dongle(&coder->sim->dongles[second], coder) != 0)
	{
		release_dongle(&coder->sim->dongles[first], coder->sim);
		return (1);
	}
	return (0);
}

static int	do_compile_cycle(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	if (take_both_dongles(coder) != 0)
		return (1);
	coder->last_compile_start = get_timestamp_ms(sim->start_ms);
	log_msg(sim, coder->id, "is compiling");
	precise_sleep(sim->time_to_compile, sim);
	release_dongle(&sim->dongles[coder->left_dongle], sim);
	release_dongle(&sim->dongles[coder->right_dongle], sim);
	coder->compile_count++;
	if (simulation_stopped(sim))
		return (1);
	log_msg(sim, coder->id, "is debugging");
	precise_sleep(sim->time_to_debug, sim);
	if (simulation_stopped(sim))
		return (1);
	log_msg(sim, coder->id, "is refactoring");
	precise_sleep(sim->time_to_refactor, sim);
	return (0);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_sim	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	while (!simulation_stopped(sim))
	{
		if (do_compile_cycle(coder) != 0)
			break ;
		if (coder->compile_count >= sim->number_of_compiles_required)
			break ;
	}
	return (NULL);
}

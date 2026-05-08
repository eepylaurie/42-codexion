/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmatthes <lmatthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 13:44:26 by lmatthes          #+#    #+#             */
/*   Updated: 2026/05/08 13:08:23 by lmatthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// destroys all mutexes/condvars, frees all allocated memory.

#include "codexion.h"

void	cleanup_sim(t_sim *sim)
{
	(void)sim;
}

void	set_sim_stopped(t_sim *sim)
{
	(void)sim;
}

int	simulation_stopped(t_sim *sim)
{
	(void)sim;
	return (1);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmatthes <lmatthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 13:44:35 by lmatthes          #+#    #+#             */
/*   Updated: 2026/05/08 13:08:56 by lmatthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// acquire_dongle() and release_dongle().
// This is where the condition variable wait loop lives,
// where cooldown is checked,
// and where the scheduler picks the next coder from the queue.

#include "codexion.h"

int	acquire_dongle(t_dongle *dongle, t_coder *coder)
{
	(void)dongle;
	(void)coder;
	return (0);
}

void	release_dongle(t_dongle *dongle, t_sim *sim)
{
	(void)dongle;
	(void)sim;
}

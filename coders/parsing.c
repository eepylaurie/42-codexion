/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmatthes <lmatthes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 13:44:57 by lmatthes          #+#    #+#             */
/*   Updated: 2026/05/08 15:45:54 by lmatthes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_valid_positive_int(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	parse_scheduler(char *str, t_sim *sim)
{
	if (strcmp(str, "fifo") == 0)
	{
		sim->scheduler = POLICY_FIFO;
		return (0);
	}
	if (strcmp(str, "edf") == 0)
	{
		sim->scheduler = POLICY_EDF;
		return (0);
	}
	fprintf(stderr, "Error: scheduler must be 'fifo' or 'edf', got: %s\n", str);
	return (1);
}

static void	fill_numeric_fields(char **argv, t_sim *sim)
{
	sim->number_of_coders = atoi(argv[1]);
	sim->time_to_burnout = atoi(argv[2]);
	sim->time_to_compile = atoi(argv[3]);
	sim->time_to_debug = atoi(argv[4]);
	sim->time_to_refactor = atoi(argv[5]);
	sim->number_of_compiles_required = atoi(argv[6]);
	sim->dongle_cooldown = atoi(argv[7]);
}

int	parse_args(int argc, char **argv, t_sim *sim)
{
	int	i;

	(void)argc;
	i = 1;
	while (i <= 7)
	{
		if (!is_valid_positive_int(argv[i]))
		{
			fprintf(stderr, "Error: invalid argument: %s\n", argv[i]);
			return (1);
		}
		i++;
	}
	fill_numeric_fields(argv, sim);
	if (sim->number_of_coders < 1)
	{
		fprintf(stderr, "Error: number_of_coders must be at least 1\n");
		return (1);
	}
	return (parse_scheduler(argv[8], sim));
}

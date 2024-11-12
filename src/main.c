/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dalebran <dalebran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:03:15 by dalebran          #+#    #+#             */
/*   Updated: 2024/11/12 12:44:17 by dalebran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*routine(void *arg)
{
	(void) arg;
	return (NULL);
}

int	main(int ac, char **av)
{
	t_params	params;
	t_philo		*philos;

	if (!args_are_correct(ac, av))
		return (1);
	init_params(&params, ac, av);
	philos = init_philo(&params);
	if (!philos)
		return (1);
	free(philos);
	return (0);
}

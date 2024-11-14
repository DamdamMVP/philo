/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dalebran <dalebran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:01:01 by dalebran          #+#    #+#             */
/*   Updated: 2024/11/14 18:06:37 by dalebran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	create_threads(t_philo *philos)
{
	int	i;
	int	nb_ph;

	nb_ph = philos[0].params->nb_ph;
	i = 0;
	while (i < nb_ph)
	{
		if (pthread_create(&philos[i].thread, NULL, &routine, &philos[i]) != 0)
		{
			ft_error("Failed to create thread");
			return ;
		}
		i++;
	}
}

void	wait_threads(t_philo *philos)
{
	int	i;
	int	nb_ph;

	nb_ph = philos[0].params->nb_ph;
	i = 0;
	while (i < nb_ph)
	{
		if (pthread_join(philos[i].thread, NULL) != 0)
		{
			ft_error("Failed to join thread");
			return ;
		}
		i++;
	}
}

void	destroy_mutexes(t_philo *philos)
{
	int	i;
	int	nb_ph;

	nb_ph = philos[0].params->nb_ph;
	i = 0;
	while (i < nb_ph)
	{
		pthread_mutex_destroy(&philos[i].left_fork);
		i++;
	}
	pthread_mutex_destroy(&philos[0].params->print_mutex);
	pthread_mutex_destroy(&philos[0].params->update_mutex);
}

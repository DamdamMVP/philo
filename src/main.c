/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dalebran <dalebran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:03:15 by dalebran          #+#    #+#             */
/*   Updated: 2024/11/12 15:20:07 by dalebran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*routine(void *arg)
{
	(void) arg;
	return (NULL);
}

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
			perror("Failed to create thread");
			exit(1);
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
			perror("Failed to join thread");
			exit(1);
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
	// Détruire le mutex d'affichage si nécessaire
	// pthread_mutex_destroy(&philos[0].params->print_mutex);
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
	create_threads(philos);
	wait_threads(philos);
	destroy_mutexes(philos);
	free(philos);
	return (0);
}

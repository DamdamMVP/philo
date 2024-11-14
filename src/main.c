/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dalebran <dalebran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:03:15 by dalebran          #+#    #+#             */
/*   Updated: 2024/11/14 18:03:07 by dalebran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep((philo->params->eat_t / 2) * 1000);
	while (1)
	{
		if (should_terminate(philo) || philo->nb_eat == -1)
			break ;
		print_status(philo, "is thinking");
		try_to_eat(philo);
		print_status(philo, "is sleeping");
		usleep(philo->params->sleep_t * 1000);
	}
	return (NULL);
}

void	*monitor_all(void *arg)
{
	t_philo	*philos;
	int		i;

	philos = (t_philo *)arg;
	while (1)
	{
		i = 0;
		while (i < philos[0].params->nb_ph)
		{
			if (should_terminate(&philos[i]))
				return (NULL);
			pthread_mutex_lock(&philos[i].params->update_mutex);
			if (get_current_time_in_ms() > philos[i].last_eat_t
				+ philos[i].params->die_t)
			{
				philos[i].params->simulation_end = 1;
				pthread_mutex_unlock(&philos[i].params->update_mutex);
				print_status(&philos[i], "died");
				return (NULL);
			}
			pthread_mutex_unlock(&philos[i].params->update_mutex);
			i++;
		}
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	t_params	params;
	t_philo		*philos;
	pthread_t	monitor_thread;

	if (!args_are_correct(ac, av))
		return (1);
	if (!init_params(&params, ac, av))
		return (1);
	philos = init_philo(&params);
	if (!philos)
		return (1);
	create_threads(philos);
	if (pthread_create(&monitor_thread, NULL, &monitor_all, philos) != 0)
	{
		ft_error("Failed to create monitor thread");
		destroy_mutexes(philos);
		free(philos);
		return (1);
	}
	pthread_join(monitor_thread, NULL);
	wait_threads(philos);
	destroy_mutexes(philos);
	free(philos);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dalebran <dalebran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:03:15 by dalebran          #+#    #+#             */
/*   Updated: 2024/11/12 17:04:10 by dalebran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo)
{
	if (philo->id == 1)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a right fork");
		pthread_mutex_lock(&philo->left_fork);
		print_status(philo, "has taken a left fork");
	}
	else
	{
		pthread_mutex_lock(&philo->left_fork);
		print_status(philo, "has taken a left fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a right fork");
	}
}

void	eat(t_philo *philo)
{
	print_status(philo, "is eating");
	philo->last_eat_t = get_current_time_in_ms();
	philo->nb_eat++;
	usleep(philo->params->eat_t);
}

void	put_down_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(&philo->left_fork);
}

void	print_status(t_philo *philo, char *status)
{
	long	timestamp;

	pthread_mutex_lock(&philo->params->print_mutex);
	timestamp = get_current_time_in_ms() - philo->params->start_t;
	printf("%ld %d %s\n", timestamp, philo->id, status);
	pthread_mutex_unlock(&philo->params->print_mutex);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (philo->params->nb_ph_must_eat != -1 && philo->nb_eat >= philo->params->nb_ph_must_eat)
			break;
		print_status(philo, "is thinking");
		take_forks(philo);
		eat(philo);
		put_down_forks(philo);
		print_status(philo, "is sleeping");
		usleep(philo->params->sleep_t);
	}
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
	pthread_mutex_destroy(&philos[0].params->print_mutex);
}

void	*monitor_all(void *arg)
{
	t_philo	*philos;
	int		i;
	int		finished;

	philos = (t_philo *)arg;
	while (1)
	{
		i = 0;
		finished = 0;
		while (i < philos[0].params->nb_ph)
		{
			pthread_mutex_lock(&philos[i].params->print_mutex);
			if (get_current_time_in_ms() - philos[i].last_eat_t > philos[i].params->die_t)
			{
				print_status(&philos[i], "died");
				pthread_mutex_unlock(&philos[i].params->print_mutex);
				exit(0);
			}
			if (philos[i].params->nb_ph_must_eat != -1 && philos[i].nb_eat >= philos[i].params->nb_ph_must_eat)
				finished++;
			pthread_mutex_unlock(&philos[i].params->print_mutex);
			i++;
		}
		if (philos[0].params->nb_ph_must_eat != -1 && finished == philos[0].params->nb_ph)
		{
			printf("All philosophers have eaten enough. Simulation ended.\n");
			exit(0);
		}
		usleep(10);
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
	pthread_create(&monitor_thread, NULL, &monitor_all, philos);
	create_threads(philos);
	wait_threads(philos);
	pthread_join(monitor_thread, NULL);
	destroy_mutexes(philos);
	free(philos);
	return (0);
}

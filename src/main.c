/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dalebran <dalebran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:03:15 by dalebran          #+#    #+#             */
/*   Updated: 2024/11/13 12:17:48 by dalebran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	should_terminate(t_philo *philo)
{
	int	end;

	pthread_mutex_lock(&philo->params->update_mutex);
	end = philo->params->simulation_end;
	pthread_mutex_unlock(&philo->params->update_mutex);
	return (end);
}

void	take_forks(t_philo *philo)
{
	if (should_terminate(philo))
		return ;
	pthread_mutex_lock(&philo->left_fork);
	print_status(philo, "has taken a left fork");
	pthread_mutex_lock(philo->right_fork);
	print_status(philo, "has taken a right fork");
}

void	eat(t_philo *philo)
{
	if (should_terminate(philo))
		return ;
	
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->params->update_mutex);
	philo->last_eat_t = get_current_time_in_ms() + philo->params->eat_t;
	philo->nb_eat++;
	if (philo->params->nb_ph_must_eat != -1 && philo->nb_eat >= philo->params->nb_ph_must_eat)
	{
		philo->params->stomach_full++;
		if (philo->params->stomach_full == philo->params->nb_ph)
			philo->params->simulation_end = 1;
	}
	pthread_mutex_unlock(&philo->params->update_mutex);
	usleep(philo->params->eat_t * 1000);
}

void	put_down_forks(t_philo *philo)
{
	print_status(philo, "has put down right fork");
	print_status(philo, "has put down left fork");
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(&philo->left_fork);
}

void	print_status(t_philo *philo, char *status)
{
	long	timestamp;

	pthread_mutex_lock(&philo->params->update_mutex);
	if (philo[0].params->simulation_end == 0 || ft_strcmp(status, "died") == 0)
	{
		pthread_mutex_lock(&philo->params->print_mutex);
		timestamp = get_current_time_in_ms() - philo->params->start_t;
		printf("%ld %d %s\n", timestamp, philo->id, status);
		pthread_mutex_unlock(&philo->params->print_mutex);
	}
	pthread_mutex_unlock(&philo->params->update_mutex);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep((philo->params->eat_t / 2) * 1000);
	while (1)
	{
		if (should_terminate(philo))
			break ;
		print_status(philo, "is thinking");
		take_forks(philo);
		eat(philo);
		put_down_forks(philo);
		print_status(philo, "is sleeping");
		usleep(philo->params->sleep_t * 1000);
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
	pthread_mutex_destroy(&philos[0].params->update_mutex);
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
			pthread_mutex_lock(&philos[i].params->update_mutex);
			if (get_current_time_in_ms() - philos[i].last_eat_t > philos[i].params->die_t)
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
		perror("Failed to create monitor thread");
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

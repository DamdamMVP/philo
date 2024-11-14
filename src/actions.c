/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dalebran <dalebran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:00:36 by dalebran          #+#    #+#             */
/*   Updated: 2024/11/14 17:56:49 by dalebran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo)
{
	if (is_alone(philo))
		return ;
	pthread_mutex_lock(&philo->left_fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(philo->right_fork);
	print_status(philo, "has taken a fork");
}

void	try_to_eat(t_philo *philo)
{
	if (should_terminate(philo))
		return ;
	take_forks(philo);
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->params->update_mutex);
	philo->nb_eat++;
	if (philo->params->nb_ph_must_eat != -1
		&& philo->nb_eat >= philo->params->nb_ph_must_eat)
	{
		philo->params->stomach_full++;
		philo->nb_eat = -1;
		if (philo->params->stomach_full == philo->params->nb_ph)
			philo->params->simulation_end = 1;
	}
	pthread_mutex_unlock(&philo->params->update_mutex);
	usleep(philo->params->eat_t * 1000);
	pthread_mutex_lock(&philo->params->update_mutex);
	philo->last_eat_t = get_current_time_in_ms();
	pthread_mutex_unlock(&philo->params->update_mutex);
	if (philo->params->nb_ph != 1)
		put_down_forks(philo);
}

void	put_down_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
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

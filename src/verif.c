/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verif.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dalebran <dalebran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:57:29 by dalebran          #+#    #+#             */
/*   Updated: 2024/11/13 19:58:06 by dalebran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_positive_number(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	args_are_correct(int ac, char **av)
{
	int	i;

	i = 1;
	if (ac != 5 && ac != 6)
		return (ft_error("Numbers of arguments diff than 5 or 6\n"), 0);
	while (i < ac)
	{
		if (!is_positive_number(av[i]))
			return (ft_error("Arguments must be positive integers\n"), 0);
		if (ft_atoi(av[i]) <= 0)
			return (ft_error("Arguments must be greater than 0\n"), 0);
		i++;
	}
	return (1);
}

int	is_alone(t_philo *philo)
{
	if (philo->params->nb_ph == 1)
	{
		pthread_mutex_lock(&philo->left_fork);
		print_status(philo, "has taken a fork");
		usleep(philo->params->die_t * 1005);
		pthread_mutex_unlock(&philo->left_fork);
		pthread_mutex_lock(&philo->params->update_mutex);
		philo->params->simulation_end = 1;
		pthread_mutex_unlock(&philo->params->update_mutex);
		return (1);
	}
	return (0);
}

int	should_terminate(t_philo *philo)
{
	int	end;

	pthread_mutex_lock(&philo->params->update_mutex);
	end = philo->params->simulation_end;
	pthread_mutex_unlock(&philo->params->update_mutex);
	return (end);
}

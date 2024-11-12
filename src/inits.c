/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dalebran <dalebran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:51:11 by dalebran          #+#    #+#             */
/*   Updated: 2024/11/12 12:50:09 by dalebran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_positive_number(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || ('\t' <= str[i] && str[i] <= '\r'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -sign;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
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

void	init_params(t_params *params, int ac, char **av)
{
	params->nb_ph = ft_atoi(av[1]);
	params->die_t = ft_atoi(av[2]);
	params->eat_t = ft_atoi(av[3]);
	params->sleep_t = ft_atoi(av[4]);
	params->start_t = get_current_time_in_ms();
	if (ac == 6)
		params->nb_ph_must_eat = ft_atoi(av[5]);
	else
		params->nb_ph_must_eat = -1;

	printf("Params initialized:\n");
	printf("Number of philosophers: %d\n", params->nb_ph);
	printf("Time to die: %d\n", params->die_t);
	printf("Time to eat: %d\n", params->eat_t);
	printf("Time to sleep: %d\n", params->sleep_t);
	printf("Number of times each philosopher must eat: %d\n", params->nb_ph_must_eat);
}

t_philo	*init_philo(t_params *params)
{
	t_philo	*philos;
	int		i;

	philos = malloc(sizeof(t_philo) * params->nb_ph);
	if (!philos)
		return (ft_error("Memory allocation failed\n"), NULL);
	i = 0;
	while (i < params->nb_ph)
	{
		philos[i].id = i + 1;
		philos[i].nb_eat = 0;
		philos[i].last_eat_t = params->start_t;
		philos[i].params = params;
		if (pthread_mutex_init(&philos[i].left_fork, NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&philos[i].left_fork);
			return (ft_error("Mutex init failed\n"), free(philos), NULL);
		}
		if (i == params->nb_ph - 1)
			philos[i].right_fork = &philos[0].left_fork;
		else
			philos[i].right_fork = &philos[i + 1].left_fork;
		i++;
	}
	for (int i = 0; i < params->nb_ph; i++)
	{
		printf("Philosopher %d initialized:\n", philos[i].id);
		printf("  Left fork mutex address: %p\n", (void *)&philos[i].left_fork);
		printf("  Right fork mutex address: %p\n", (void *)philos[i].right_fork);
		printf("  Number of times eaten: %d\n", philos[i].nb_eat);
		printf("  Last time eaten: %ld\n", philos[i].last_eat_t);
	}
	return (philos);
}

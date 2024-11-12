/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dalebran <dalebran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:03:27 by dalebran          #+#    #+#             */
/*   Updated: 2024/11/12 16:14:17 by dalebran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct s_params
{
	int				nb_ph;
	int				sleep_t;
	int				eat_t;
	int				die_t;
	int				nb_ph_must_eat;
	long			start_t;
	pthread_mutex_t	print_mutex;
}	t_params;

typedef struct s_philo
{
	int				id;
	int				nb_eat;
	long			last_eat_t;
	pthread_t		thread;
	pthread_mutex_t	left_fork;
	pthread_mutex_t	*right_fork;
	struct s_params	*params;
}	t_philo;

// inits.c
int		is_positive_number(char *str);
int		ft_atoi(const char *str);
int		args_are_correct(int ac, char **av);
int		init_params(t_params *params, int ac, char **av);
t_philo	*init_philo(t_params *params);

// utils.c
int		ft_strlen(const char *s);
void	ft_error(const char *message);
long	get_current_time_in_ms(void);


void	take_forks(t_philo *philo);
void	eat(t_philo *philo);
void	put_down_forks(t_philo *philo);
void	print_status(t_philo *philo, char *status);
void	*routine(void *arg);
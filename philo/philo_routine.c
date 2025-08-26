/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 13:05:59 by makboga           #+#    #+#             */
/*   Updated: 2025/08/26 13:07:15 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	check_and_print(t_philo *philo)
{
	long	timestamp;
	int		required_meals;

	required_meals = philo->sim->number_of_must_eat;
	if (required_meals != -1
		&& philo->meals_eaten == required_meals)
	{
		pthread_mutex_lock(&philo->sim->full_lock);
		philo->sim->full_philos++;
		if (philo->sim->full_philos == philo->sim->num_of_philos)
		{
			timestamp = get_current_time() - philo->sim->start_time;
			pthread_mutex_lock(&philo->sim->write_lock);
			pthread_mutex_unlock(&philo->sim->write_lock);
		}
		pthread_mutex_unlock(&philo->sim->full_lock);
	}
}

void	philo_eat(t_philo *philo)
{
	taken_forks(philo);
	pthread_mutex_lock(&philo->sim->meal_lock);
	philo->eating = 1;
	philo->last_meal = get_current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->sim->meal_lock);
	print_status(philo, "is eating", GREEN);
	ft_usleep(philo->sim->time_to_eat, philo);
	pthread_mutex_lock(&philo->sim->meal_lock);
	check_and_print(philo);
	philo->eating = 0;
	pthread_mutex_unlock(&philo->sim->meal_lock);
	release_forks(philo);
}

void	philo_sleep(t_philo *philo)
{
	if (philo_has_died(philo))
		return ;
	print_status(philo, "is sleeping", BLUE);
	ft_usleep(philo->sim->time_to_sleep, philo);
}

void	philo_think(t_philo *philo)
{
	if (philo_has_died(philo))
		return ;
	print_status(philo, "is thinking", MAGENTA);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->sim->num_of_philos == 1)
	{
		pthread_mutex_lock(philo->l_fork);
		print_status(philo, "has taken a fork", CYAN);
		ft_usleep(philo->sim->time_to_die, philo);
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_lock(&philo->sim->dead_lock);
		philo->sim->someone_died = 1;
		pthread_mutex_unlock(&philo->sim->dead_lock);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		ft_usleep(philo->sim->time_to_eat / 10, philo);
	while (!philo_has_died(philo))
	{
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}

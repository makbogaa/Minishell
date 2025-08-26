/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 12:20:45 by makboga           #+#    #+#             */
/*   Updated: 2025/08/26 13:10:31 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_error(const char *str)
{
	printf(RED "%s" RESET, str);
	exit(EXIT_FAILURE);
}

long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(long time_in_ms, t_philo *philo)
{
	long	start;
	long	now;

	start = get_current_time();
	while (!philo_has_died(philo))
	{
		now = get_current_time();
		if ((now - start) >= time_in_ms)
			break ;
		usleep(500);
	}
}

void	print_status(t_philo *philo, char *status, char *color)
{
	long		timestamp;

	pthread_mutex_lock(&philo->sim->write_lock);
	if (philo_has_died(philo))
	{
		pthread_mutex_unlock(&philo->sim->write_lock);
		return ;
	}
	timestamp = get_current_time() - philo->sim->start_time;
	printf("%s%ld %d %s%s\n", CYAN, timestamp, philo->id, color, status);
	printf(RESET);
	pthread_mutex_unlock(&philo->sim->write_lock);
}

int	philo_has_died(t_philo *philo)
{
	int	result;

	pthread_mutex_lock(&philo->sim->dead_lock);
	result = philo->sim->someone_died;
	pthread_mutex_unlock(&philo->sim->dead_lock);
	return (result);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 13:08:05 by makboga           #+#    #+#             */
/*   Updated: 2025/07/03 13:33:31 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	create_threads(t_sim *sim, pthread_t *monitor_thread)
{
	int	i;

	sim->start_time = get_current_time();
	i = 0;
	while (i < sim->num_of_philos)
	{
		sim->philos[i].last_meal = sim->start_time;
		i++;
	}
	if (pthread_create(monitor_thread, NULL, ft_monitor, sim) != 0)
		destroy_mutexes("Failed to create monitor thread\n", sim);
	i = 0;
	while (i < sim->num_of_philos)
	{
		if (pthread_create(&sim->philos[i].thread, NULL, philo_routine,
				&sim->philos[i]) != 0)
			destroy_mutexes("Failed to create philosopher thread\n", sim);
		i++;
	}
}

static void	join_threads(t_sim *sim, pthread_t *monitor_thread)
{
	int	i;

	if (pthread_join(*monitor_thread, NULL) != 0)
		destroy_mutexes("Thread join error\n", sim);
	i = 0;
	while (i < sim->num_of_philos)
	{
		if (pthread_join(sim->philos[i].thread, NULL) != 0)
			destroy_mutexes("Thread join error\n", sim);
		i++;
	}
}

void	start_threads(t_sim *sim)
{
	pthread_t	monitor_thread;

	create_threads(sim, &monitor_thread);
	join_threads(sim, &monitor_thread);
}

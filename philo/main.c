/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 12:18:31 by makboga           #+#    #+#             */
/*   Updated: 2025/08/19 18:57:42 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	ft_check_arg(char *str)
{
	int	i;

	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '+')
		i++;
	if (!ft_is_digit(str[i]))
		return (false);
	if (str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_is_digit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

static void	ft_init(t_sim *sim, char **argv)
{
	sim->num_of_philos = ft_atoi(argv[1]);
	sim->time_to_die = ft_atoi(argv[2]);
	sim->time_to_eat = ft_atoi(argv[3]);
	sim->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
	{
		sim->number_of_must_eat = ft_atoi(argv[5]);
	}
	else
	{
		sim->number_of_must_eat = -1;
	}
	sim->someone_died = 0;
	sim->full_philos = 0;
	// pthread_mutex_init(&sim->write_lock, NULL); //
	// pthread_mutex_init(&sim->dead_lock, NULL); //
	pthread_mutex_init(&sim->meal_lock, NULL);
	pthread_mutex_init(&sim->full_lock, NULL);
}

int	ft_start_data(char **argv, t_sim *table)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (!ft_check_arg(argv[i]))
		{
			return (1);
		}
		i++;
	}
	ft_init(table, argv);
	if (table->num_of_philos < 1 || table->num_of_philos > PHILO_MAX
		|| table->time_to_die <= 0 || table->time_to_eat <= 0
		|| table->time_to_sleep <= 0)
		return (1);
	if (argv[5] && table->number_of_must_eat <= 0)
		return (1);
	table->start_time = get_current_time();
	return (0);
}

int	main(int ac, char **av)
{
	t_sim	sim;

	if (ac != 5 && ac != 6)
	{
		ft_error("Error: Invalid argument count!\n");
	}
	if (ft_start_data(av, &sim))
	{
		ft_error("Error: Invalid argument error!\n");
	}
	init_forks(&sim);
	init_philos(&sim);
	start_threads(&sim);
	destroy_mutexes(NULL, &sim);
	return (0);
}
 
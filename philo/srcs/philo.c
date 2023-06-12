/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsydelny <dsydelny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 13:19:10 by dsydelny          #+#    #+#             */
/*   Updated: 2023/05/30 18:23:10 by dsydelny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long int	gettodaystime( void )
{
	struct timeval	current;

	if (gettimeofday(&current, NULL) == -1)
		return (-1);
	return ((current.tv_sec * 1000) + (current.tv_usec / 1000));
	// printf("seconds : %ld\nmicro seconds : %ld", val.tv_sec, val.tv_usec);
}

void	*process_func(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	// pthread_mutex_lock(&mutex);
	// some actions inside
	// while (1)
		/* code */
		while (1)
		{
			pthread_mutex_lock(&philo->data->print);
			printf("[%ld] %i\n", gettodaystime() - philo->data->set_to_zero, philo->id); //("my id is %i\n", philo->id);
			usleep(100000);// printf("%i\n", philo->id); //("my id is %i\n", philo->id);
			pthread_mutex_unlock(&philo->data->print);
		}
	// pthread_mutex_unlock(&mutex);
	return (0);
}

int	main(int ac, char **av)
{
	static t_data	data = {0};
	t_philo			*philo;
	//each philo should be a thread
	int	i;
	char *str = "hello";

	printf("%li\n", gettodaystime());
	
	if (ac < 5 || ac > 6)
		return (printf("NOT VALID AMOUNT OF ARGUMENTS\n"), 1);
	if (check_valid_args(av))
		exit(1);
	data.nbphilo = ft_atoi(av[1]);
	philo = calloc(sizeof(t_philo), data.nbphilo);
	if (!philo)
		return (1);
	data.phils = malloc(sizeof(pthread_t) * data.nbphilo);
	if (!data.phils)
		return (free(philo), 1);
	i = 0;
	data.set_to_zero = gettodaystime();
	data.philo = philo;
	pthread_mutex_init(&data.print, NULL);
	while (i < data.nbphilo)
	{
		philo[i].id = i + 1;
		philo[i].data = & data;
		philo[i].str = str;
		i++;
	}
	i = 0;
	while (i < data.nbphilo)
	{
		if (pthread_create(&data.phils[i], NULL, &process_func, &philo[i])) 
		// 2 arg is attribute/for customization 3 func for execute 4 arg for proc func
		{
			// perror("CREATION OF THREAD FAILED\n");
			// return (1);
		}
		i++;
	}
	i = 0;
	while (i < data.nbphilo)
	{
		if (pthread_join(data.phils[i], NULL))
		{
			// perror("CREATION OF THREAD FAILED\n");
			// return (1);
		}
		i++;
	}
	// pthread_mutexattr_destroy(&mutex);
}

/*
	--tool=helgrind 
*/
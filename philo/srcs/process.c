/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsydelny <dsydelny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 15:49:29 by dsydelny          #+#    #+#             */
/*   Updated: 2023/06/15 18:03:51 by dsydelny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_msg(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->data->print);
	printf("%ld	№%d %s\n", gettodaystime() - philo->data->set_to_zero, philo->id, str);
	pthread_mutex_unlock(&philo->data->print);
}

void	*check_time_pass(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	while(philo->data->death)
	{
		if (gettodaystime() - philo->last_lunch >= philo->data->t_t_die)
		{
			philo->data->death = 0;
			print_msg(philo, "died");
		}
	}
	return (NULL);
}

void	*process_func(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (philo->data->death)
	{
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(philo->r_spoon);
			print_msg(philo, "taken a fork");
			pthread_mutex_lock(philo->l_spoon);
			print_msg(philo, "taken a fork");
			print_msg(philo, "is eating");
			philo->last_lunch = gettodaystime();
			usleep(philo->data->t_t_eat);
			pthread_mutex_unlock(philo->l_spoon);
			pthread_mutex_unlock(philo->r_spoon);
		}
		else
		{
			pthread_mutex_lock(philo->l_spoon);
			print_msg(philo, "taken a fork");
			pthread_mutex_lock(philo->r_spoon);
			print_msg(philo, "taken a fork");
			print_msg(philo, "is eating");
			philo->last_lunch = gettodaystime();
			usleep(philo->data->t_t_eat);
			pthread_mutex_unlock(philo->r_spoon);
			pthread_mutex_unlock(philo->l_spoon);
		}
		print_msg(philo, "is sleeping");
		usleep(philo->data->t_t_sleep);

		print_msg(philo, "is thinking");
	}
	return (NULL);
}

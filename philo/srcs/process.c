/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsydelny <dsydelny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 15:49:29 by dsydelny          #+#    #+#             */
/*   Updated: 2023/07/30 19:08:36 by dsydelny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <string.h>

void	print_msg(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->data->print);
	if (!philo->data->death){
		pthread_mutex_unlock(&philo->data->print);
		return;
	}
	printf("%ld	№%d %s\n", gettodaystime() - philo->data->set_to_zero, philo->id, str);
	pthread_mutex_unlock(&philo->data->print);
}

void *check_time_pass(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	while(philo->data->death)
	{
		if (gettodaystime() - philo->data->set_to_zero - philo->last_lunch >= philo->data->t_t_die)
		{
			// if (!philo->data->death)
			// 	return NULL;
			print_msg(philo, "died");
			philo->data->death = 0;
			return NULL;
		}
	}
	return NULL;
}

void	*process_func(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(10);
	while (philo->data->death)
	{
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(philo->r_spoon);
			print_msg(philo, "taken a fork");
			pthread_mutex_lock(philo->l_spoon);
			print_msg(philo, "taken a fork");
			print_msg(philo, "is eating");
			philo->last_lunch = gettodaystime() - philo->data->set_to_zero + philo->data->t_t_eat;
			usleep(philo->data->t_t_eat * 1000);
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
			philo->last_lunch = gettodaystime() - philo->data->set_to_zero + philo->data->t_t_eat;
			fprintf(stderr, ">>>>>>%i\n", philo->data->t_t_eat);
			usleep(philo->data->t_t_eat * 1000);
			pthread_mutex_unlock(philo->r_spoon);
			pthread_mutex_unlock(philo->l_spoon);
		}
		print_msg(philo, "is sleeping");
		usleep(philo->data->t_t_sleep * 1000);
		print_msg(philo, "is thinking");

	}
	return (NULL);
}

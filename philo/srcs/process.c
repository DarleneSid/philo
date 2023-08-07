/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsydelny <dsydelny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 15:49:29 by dsydelny          #+#    #+#             */
/*   Updated: 2023/08/07 15:25:43 by dsydelny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *check_time_pass(void *arg)
{
	t_data *data;
	int		i;
	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		while (i < data->nb_philo)
		{
			pthread_mutex_lock(&data->philo[i].lunchchecker);
			if (gettodaystime() - data->set_to_zero - data->philo[i].last_lunch >= data->t_t_die)
			{
				pthread_mutex_unlock(&data->philo[i].lunchchecker);
				print_msg(data->philo, "died");
				pthread_mutex_lock(&data->deathchecker);
				data->death = 0;
				pthread_mutex_unlock(&data->deathchecker);
				return NULL;
			}
			pthread_mutex_unlock(&data->philo[i].lunchchecker);
			if (data->max_eat)
			{
				pthread_mutex_lock(&data->eatchecker);
				if (data->max_eat == 0)
				{
					pthread_mutex_unlock(&data->eatchecker);
					pthread_mutex_lock(&data->deathchecker);
					data->death = 0;
					pthread_mutex_unlock(&data->deathchecker);
					return NULL;
				}
				pthread_mutex_unlock(&data->eatchecker);
			}
			i++;
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
	while (1)
	{
		if ((philo->data->nb_philo % 2 != 0) && philo->eaten > 0)
		{
			usleep(philo->data->t_t_die * 0.25);
		}
		pthread_mutex_lock(&philo->data->deathchecker);
		if (!philo->data->death)
		{
			pthread_mutex_unlock(&philo->data->deathchecker);
		 	return (NULL) ;
		}
		pthread_mutex_unlock(&philo->data->deathchecker);
		if (philo->id % 2 == 0)
		{
			if (right_fork(philo))
				return (NULL);
		}
		else
		{
			if (left_fork(philo))
				return (NULL);
		}
		if (print_msg(philo, "is sleeping"))
			return (NULL);
		usleep(philo->data->t_t_sleep * 1000);
		if (print_msg(philo, "is thinking"))
			return (NULL);
	}
	return NULL;
}

void	*just_one_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	print_msg(philo, "taken a fork");
	usleep(philo->data->t_t_die * 1000);
	print_msg(philo, "died");
	return NULL;
}

/*
4 410 200 200 = LIVE
4 399 200 200 = DIE

5 610 200 200 = LIVE
5 599 200 200 = DIE

*/

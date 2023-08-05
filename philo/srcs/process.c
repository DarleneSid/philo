/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsydelny <dsydelny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 15:49:29 by dsydelny          #+#    #+#             */
/*   Updated: 2023/08/05 23:05:48 by dsydelny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*print_msg(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->data->deathchecker);
	if (!philo->data->death)
	{
		pthread_mutex_unlock(&philo->data->deathchecker);
		return ("DIED");
	}
	pthread_mutex_unlock(&philo->data->deathchecker);
	pthread_mutex_lock(&philo->data->print);
	printf("%ld	№%d %s\n", gettodaystime() - philo->data->set_to_zero, philo->id, str);
	pthread_mutex_unlock(&philo->data->print);
	return (NULL);
}

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
void	*right_fork(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(philo->r_spoon);
	if (print_msg(philo, "taken a fork"))
	{
		pthread_mutex_unlock(philo->r_spoon);
		return ("DIE");
	}
	pthread_mutex_lock(philo->l_spoon);
	if (print_msg(philo, "taken a fork"))
	{
		pthread_mutex_unlock(philo->r_spoon);
		pthread_mutex_unlock(philo->l_spoon);
		return ("DIE");
	}
	if (print_msg(philo, "is eating"))
	{
		pthread_mutex_unlock(philo->r_spoon);
		pthread_mutex_unlock(philo->l_spoon);
		return ("DIE");
	}
	philo->eaten++;
	pthread_mutex_lock(&philo->data->eatchecker);
	philo->data->max_eat--;
	pthread_mutex_unlock(&philo->data->eatchecker);
	philo->last_lunch = gettodaystime() - philo->data->set_to_zero;
	usleep(philo->data->t_t_eat * 1000);
	pthread_mutex_unlock(philo->r_spoon);
	pthread_mutex_unlock(philo->l_spoon);
	return (NULL);
}

void	*left_fork(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(philo->l_spoon);
	if (print_msg(philo, "taken a fork"))
	{
		pthread_mutex_unlock(philo->l_spoon);
		return ("DIE");
	}
	pthread_mutex_lock(philo->r_spoon);
	if (print_msg(philo, "taken a fork"))
	{
		pthread_mutex_unlock(philo->l_spoon);
		pthread_mutex_unlock(philo->r_spoon);
		return ("DIE");
	}
	if (print_msg(philo, "is eating"))
	{
		pthread_mutex_unlock(philo->l_spoon);
		pthread_mutex_unlock(philo->r_spoon);
		return ("DIE");
	}
	philo->eaten++;
	pthread_mutex_lock(&philo->data->eatchecker);
	philo->data->max_eat--;
	pthread_mutex_unlock(&philo->data->eatchecker);
	pthread_mutex_lock(&philo->lunchchecker);
	philo->last_lunch = gettodaystime() - philo->data->set_to_zero;
	pthread_mutex_unlock(&philo->lunchchecker);
	usleep(philo->data->t_t_eat * 1000);
	pthread_mutex_unlock(philo->l_spoon);
	pthread_mutex_unlock(philo->r_spoon);
	return (NULL);
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

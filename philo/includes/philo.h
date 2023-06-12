/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsydelny <dsydelny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 13:18:33 by dsydelny          #+#    #+#             */
/*   Updated: 2023/05/30 18:18:08 by dsydelny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <sys/wait.h>

typedef struct t_data
{
	int			nbphilo;
	int			ttdie;
	int			ttsleep;
	int			tteat;
	int			maxeat;
	long int	set_to_zero;
	struct t_philo		*philo;
	pthread_mutex_t	print;
	pthread_t	*phils;
}				t_data;

typedef struct t_philo
{
	char		*str;
	int			id;
	t_data		*data;
}				t_philo;


int				check_valid_args(char **av);
int				ft_atoi(char *n);

#endif

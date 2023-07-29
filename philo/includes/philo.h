/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsydelny <dsydelny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 13:18:33 by dsydelny          #+#    #+#             */
/*   Updated: 2023/07/29 20:48:27 by dsydelny         ###   ########.fr       */
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
	int			nb_philo;
	int			t_t_die;
	int			t_t_sleep;
	int			t_t_eat;
	int			max_eat;
	int			death;
	long int	set_to_zero;
	struct t_philo		*philo;
	pthread_mutex_t	print;
	pthread_mutex_t *spoon;
	pthread_t	*phils;
	pthread_t	*check_time;
}				t_data;

typedef struct t_philo
{
	char		*str;
	int			id;
	long int	last_lunch;
	pthread_mutex_t *l_spoon;
	pthread_mutex_t *r_spoon;
	t_data		*data;
}				t_philo;

/*		PARSING		*/
int			check_valid_args(char **av);
int			ft_atoi(char *n);

/*		PHILO		*/
long int	gettodaystime(void);
void		*process_func(void *arg);
void	init(t_data *data, char **av, int ac);

int	check_time_pass(void *arg);

#endif

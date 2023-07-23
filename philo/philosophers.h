#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_party	t_party;

typedef struct s_philosopher
{
	pthread_t 			thread;
	pthread_mutex_t		*fork_own;
	pthread_mutex_t		*fork_borrowed;
	int					meal_count; // chenge to meals_left?
	unsigned int		index; // TODO
	unsigned long long	time_last_ate;
	t_party				*party;
}						t_philosopher;

typedef struct s_party
{
	unsigned int		number_of_philosophers;
	unsigned long long	time_to_die;
	unsigned long long	time_to_eat;
	unsigned long long	time_to_sleep;
	int					number_of_meals_needed;
	t_philosopher		*philosophers;
	pthread_t 			monitoring_thread; // TODO
	pthread_mutex_t		*forks;
	pthread_mutex_t		guard;
	pthread_mutex_t		printing;
	pthread_mutex_t		dying;
	int					someone_dead;
}						t_party;

typedef enum e_return_value
{
	SUCCESS,
	ERROR,
}						t_return_value;

#endif

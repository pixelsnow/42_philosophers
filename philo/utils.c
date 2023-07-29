#include "philosophers.h"


void	print_whats_happening(t_philosopher *philosopher, char *event)
{
	unsigned long long	timestamp;

	timestamp = get_current_time() - philosopher->party->party_start_time;
	timestamp /= 1000ULL;
	pthread_mutex_lock(&(philosopher->party->printing));
	/* if (philosopher->party->someone_dead == 0) */
		printf("%llu\t%d\t%s\n", timestamp, philosopher->index + 1, event);
	pthread_mutex_unlock(&(philosopher->party->printing));
}

t_return_value	custom_usleep(unsigned long long duration, t_party *party)
{
	unsigned long long	start_time;

	start_time = get_current_time();
	while (get_current_time() - start_time < duration)
	{
		pthread_mutex_lock(&(party->dying));
		if (party->someone_dead != 0)
		{
			pthread_mutex_unlock(&(party->dying));
			return (SOMEONE_DIED);
		}
		pthread_mutex_unlock(&(party->dying));
		usleep(500);
	}
	return (SUCCESS);
}

unsigned long long	get_current_time(void)
{
	struct timeval tp;

	gettimeofday(&tp, NULL);
	return ((unsigned long long)tp.tv_sec * 1000000 + tp.tv_usec);
}

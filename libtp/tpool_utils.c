/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tpool_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 10:56:56 by pacovali      #+#    #+#                 */
/*   Updated: 2019/10/28 10:56:56 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_thpool.h"

size_t			tp_get_nbr_proc(void)
{
	return (sysconf(_SC_NPROCESSORS_ONLN));
}

t_job			*tp_get_next_job(t_pool *pool)
{
	t_job	*job;

	if (pool == NULL)
		return (NULL);
	job = pool->first;
	if (job == NULL)
		return (NULL);
	if (job->next != NULL)
		pool->first = job->next;
	else
	{
		pool->first = NULL;
		pool->last = NULL;
	}
	return (job);
}

void			tp_job_destroy(t_job **job)
{
	if (*job)
		free(*job);
	*job = 0;
}

t_job			*tp_job_create(void (*fun)(void*), void *arg)
{
	t_job	*job;

	if (fun == NULL)
		return (NULL);
	job = malloc(sizeof(t_job));
	job = memset(job, 0, sizeof(t_job));
	job->fun = fun;
	job->data = arg;
	job->next = NULL;
	return (job);
}

void			tp_wait_for_queue(t_pool *pool)
{
	if (!pool)
		return ;
	pthread_mutex_lock(&pool->lock);
	while (1)
		if ((!pool->stop && pool->working_threads != 0) ||
			(pool->stop && pool->total_threads != 0))
			pthread_cond_wait(&pool->threads_idle, &pool->lock);
		else
			break ;
	pthread_mutex_unlock(&pool->lock);
}

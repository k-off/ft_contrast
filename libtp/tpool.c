/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tpool.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 10:56:56 by pacovali      #+#    #+#                 */
/*   Updated: 2019/10/28 10:56:56 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_thpool.h"

static void		do_job(t_pool *pool)
{
	t_job		*job;
	char		*str;

	job = tp_get_next_job(pool);
	pool->working_threads++;
	pthread_mutex_unlock(&pool->lock);
	if (job != NULL && job->fun != NULL && job->data != NULL)
	{
		str = (char*)job->data;
		job->fun(job->data);
		tp_job_destroy(&job);
	}
	pthread_mutex_lock(&pool->lock);
	pool->working_threads--;
}

void			*tp_worker(void *args)
{
	t_pool		*pool;

	pool = (t_pool*)args;
	while (1)
	{
		pthread_mutex_lock(&pool->lock);
		while (pool->first == NULL && !pool->stop)
			pthread_cond_wait(&pool->work_exist, &pool->lock);
		if (pool->stop)
			break ;
		do_job(pool);
		if (!pool->stop && pool->working_threads < 1 && pool->first == NULL)
			pthread_cond_signal(&pool->threads_idle);
		pthread_mutex_unlock(&(pool->lock));
	}
	pool->total_threads--;
	pthread_cond_signal(&pool->threads_idle);
	pthread_mutex_unlock(&pool->lock);
	return (NULL);
}

void			tp_exec_queue_add(t_pool *pool, void (*fun)(void*), void *arg)
{
	t_job		*job;

	if (pool == NULL)
		return ;
	job = tp_job_create(fun, arg);
	if (job == NULL)
		return ;
	pthread_mutex_lock(&(pool->lock));
	if (pool->first == NULL)
	{
		pool->first = job;
		pool->last = pool->first;
	}
	else
	{
		pool->last->next = job;
		pool->last = job;
	}
	pthread_cond_broadcast(&(pool->work_exist));
	pthread_mutex_unlock(&(pool->lock));
}

t_pool			*tp_create(size_t threads_qty)
{
	t_pool		*pool;
	pthread_t	thread;
	size_t		i;

	pool = (t_pool*)malloc(sizeof(t_pool));
	pool = memset(pool, 0, sizeof(t_pool));
	pool->total_threads = threads_qty;
	if (!pool)
		exit(!!printf("error: thread pool was not allocated"));
	pthread_mutex_init(&pool->lock, NULL);
	pthread_cond_init(&pool->threads_idle, NULL);
	pthread_cond_init(&pool->work_exist, NULL);
	i = 0;
	while (i < threads_qty)
	{
		pthread_create(&thread, NULL, tp_worker, pool);
		pthread_detach(thread);
		i++;
	}
	return (pool);
}

void			tp_destroy(t_pool *pool)
{
	t_job		*to_destroy;

	if (!pool)
		return ;
	pthread_mutex_lock(&pool->lock);
	while (pool->first)
	{
		to_destroy = pool->first;
		pool->first = pool->first->next;
		tp_job_destroy(&to_destroy);
	}
	pool->stop = 1;
	pthread_cond_broadcast(&pool->work_exist);
	pthread_mutex_unlock(&pool->lock);
}

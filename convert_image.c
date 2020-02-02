/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   convert_image.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 10:56:56 by pacovali      #+#    #+#                 */
/*   Updated: 2019/10/28 10:56:56 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_contrast.h"

static void	skip_to_next(char *line, size_t *i)
{
	while (line[*i] > 32)
		(*i)++;
	while (line[*i] < 33)
		(*i)++;
	if (line[*i] == '#')
		skip_to_next(line, i);
}

static void	check_source_file(t_data *data)
{
	char	*line;

	data->src_fd = open(data->src_name, O_RDONLY);
	if (data->src_fd < 3)
		exit(!!ft_printf("error: %s: couldn't be open\n", data->src_name));
	if (read(data->src_fd, NULL, 0) < 0)
		exit(!!ft_printf("error: %s: couldn't be read\n", data->src_name));
	line = ft_memalloc(sizeof(char) * 1001);
	if (read(data->src_fd, line, 1000) < 1)
		exit(!!ft_printf("error: %s: empty file\n", data->src_name));
	if (line[0] != 'P' || (line[1] != '5' && line[1] != '2'))
		exit(!!ft_printf("error: %s: not a PGM file\n", data->src_name));
	data->hdr_len = 2;
	skip_to_next(line, &data->hdr_len);
	data->width = ft_atoi(&line[data->hdr_len]);
	skip_to_next(line, &data->hdr_len);
	data->height = ft_atoi(&line[data->hdr_len]);
	skip_to_next(line, &data->hdr_len);
	data->max = ft_atoi(&line[data->hdr_len]);
	skip_to_next(line, &data->hdr_len);
	free(line);
	lseek(data->src_fd, 0, SEEK_SET);
	data->header = (char*)ft_memalloc(sizeof(char) * (data->hdr_len + 1));
	read(data->src_fd, data->header, data->hdr_len);
}

static void	create_output_file(t_data *data)
{
	data->out_fd = open(data->out_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->out_fd < 3)
		exit(!!ft_printf("error: output file %s not created", data->out_name));
	write(data->out_fd, data->header, (data->hdr_len));
}

static void	write_to_file(t_data *data, t_pool *pool)
{
	t_out	*tmp;

	while (data->out_first)
	{
		write(data->out_fd, data->out_first->line,
			ft_strlen(data->out_first->line));
		free(data->out_first->line);
		tmp = data->out_first;
		data->out_first = data->out_first->next;
		free(tmp);
	}
	tp_destroy(pool);
}

void		convert_image(t_data *data)
{
	t_pool	*pool;
	int		ret;
	float	factor;

	check_source_file(data);
	if (data->width < 1 || data->height < 1 || data->max < 1)
		exit(!!ft_printf("error: %s: file corrupt\n", data->src_name));
	create_output_file(data);
	pool = tp_create(tp_get_nbr_proc() + 1);
	data->out_first = (t_out*)ft_memalloc(sizeof(t_out));
	data->out_last = data->out_first;
	factor = (259 * (data->contrast + 255)) / (255 * (259 - data->contrast));
	data->out_last->factor = factor;
	ret = get_next_line(data->src_fd, &data->out_last->line);
	while (ret > 0)
	{
		tp_exec_queue_add(pool, working_function, (void*)data->out_last);
		data->out_last->next = (t_out*)ft_memalloc(sizeof(t_out));
		data->out_last = data->out_last->next;
		data->out_last->factor = factor;
		ret = get_next_line(data->src_fd, &data->out_last->line);
	}
	tp_wait_for_queue(pool);
	write_to_file(data, pool);
}

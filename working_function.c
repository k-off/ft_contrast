/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   convert_image_utils.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 10:56:56 by pacovali      #+#    #+#                 */
/*   Updated: 2019/10/28 10:56:56 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_contrast.h"

static void	free_str_arr(char **s, char ***arr, int flag)
{
	int			i;

	if ((flag == 1 || flag == 2) && *s != 0)
	{
		free(*s);
		*s = 0;
	}
	if ((flag == 0 || flag == 2) && arr != 0 && *arr != 0)
	{
		i = 0;
		while (arr[0][i])
		{
			free(arr[0][i]);
			arr[0][i] = 0;
			i++;
		}
		free(*arr);
		*arr = 0;
	}
}

static void	write_to_string(char **output, int number, int is_last, size_t *i)
{
	char	*res;
	int		j;

	number = (number > 255) ? 255 : number;
	number = (number < 0) ? 0 : number;
	if (number == 0)
	{
		output[0][*i] = '0';
		(*i)++;
	}
	else
	{
		j = 0;
		res = ft_itoa(number);
		while (res[j])
		{
			output[0][(*i)] = res[j];
			j++;
			(*i)++;
		}
		free(res);
	}
	output[0][(*i)] = is_last ? '\n' : ' ';
	(*i)++;
	output[0][(*i)] = 0;
}

static void	apply_magic(char **str, float factor)
{
	char			**tmp;
	size_t			i;
	size_t			j;
	size_t			len;
	char			*output;

	tmp = ft_strsplit(*str, ' ');
	i = 0;
	while (tmp[i])
		i++;
	output = (char*)ft_memalloc(sizeof(char) * (i * 4 + 1));
	len = i * 4;
	i = 0;
	j = 0;
	while (tmp[i])
	{
		write_to_string(&output,
			(int)(factor * (float)(ft_atoi(tmp[i]) - 128) + 128),
					!tmp[i + 1], &j);
		i++;
	}
	free_str_arr(str, &tmp, 2);
	*str = output;
}

void		working_function(void *arg)
{
	t_out *out;

	if (arg)
	{
		out = (t_out*)arg;
		if (out->line)
			apply_magic(&out->line, out->factor);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 10:56:56 by pacovali      #+#    #+#                 */
/*   Updated: 2019/10/28 10:56:56 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_contrast.h"

t_data		*get_args(int ac, char **av)
{
	t_data	*data;
	int		i;

	if (ac != 7)
		exit(!!ft_printf("usage: ft_contrast -f src -c contrast -o output\n"));
	i = 0;
	data = (t_data*)ft_memalloc(sizeof(t_data));
	while (i < ac)
	{
		if (!ft_strcmp(av[i], "-f") && ac - i > 1)
			data->src_name = ft_strdup(av[i + 1]);
		if (!ft_strcmp(av[i], "-o") && ac - i > 1)
			data->out_name = ft_strdup(av[i + 1]);
		if (!ft_strcmp(av[i], "-c") && ac - i > 1)
			data->contrast = ((float)ft_atoi(av[i + 1]));
		if (data->contrast < 0 || data->contrast > 100)
			exit(!!ft_printf("error: contrast must be between 0 and 100\n"));
		i++;
	}
	if (data->src_name == NULL || data->out_name == NULL)
		exit(!!ft_printf("usage: ft_contrast -f src -c contrast -o output\n"));
	return (data);
}

int			main(int ac, char **av)
{
	t_data	*data;

	data = get_args(ac, av);
	convert_image(data);
	return (0);
}

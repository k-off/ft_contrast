/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_contrast.h                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 10:56:56 by pacovali      #+#    #+#                 */
/*   Updated: 2019/10/28 10:56:56 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FT_CONTRAST_H
# define __FT_CONTRAST_H

# include "libft/libft.h"
# include "libtp/ft_thpool.h"

typedef struct		s_out
{
	float			factor;
	char			is_ready;
	char			*line;
	struct s_out	*next;
}					t_out;

typedef struct		s_data
{
	int				src_fd;
	int				out_fd;
	char			*src_name;
	char			*out_name;
	char			*header;
	size_t			max;
	float			contrast;
	t_out			*out_first;
	t_out			*out_last;
	size_t			hdr_len;
	size_t			width;
	size_t			height;
}					t_data;

void				convert_image(t_data *data);
void				working_function(void *arg);

#endif

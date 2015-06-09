/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server_fs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/19 15:40:35 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/19 15:40:37 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_srv.h"

int		ftp_srv_fs_open_file(char *name)
{
	return (open(name, O_RDONLY));
}

char	*ftp_srv_fs_read_file(int fd)
{
	int		r;
	char	*buf;

	if (!(buf = (char*)malloc(sizeof(char) * 1024)))
		ft_malloc_error();
	if ((r = read(fd, buf, 1023)) > 0)
	{
		buf[r] = '\0';
		return (buf);
	}
	else
		return (NULL);
}

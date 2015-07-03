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

int		ftp_srv_fs_create_file(char *name)
{
	return (open(name, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR
	| S_IRGRP | S_IROTH));
}

int		ftp_srv_fs_open_file(char *name)
{
	return (open(name, O_RDONLY));
}

char	*ftp_srv_fs_read_file(int fd, int *len)
{
	int		r;
	char	*buf;

	if (!(buf = (char*)malloc(sizeof(char) * (BUFF_SIZE_READ + 1))))
		ft_malloc_error();
	if ((r = read(fd, buf, BUFF_SIZE_READ)) > 0)
	{
		buf[r] = 0;
		*len = r;
		return (buf);
	}
	else
		return (NULL);
}

t_bool	ftp_srv_fs_write_in_file(int fd, char *data, int len)
{
	if (write(fd, data, len) != -1)
		return (TRUE);
	return (FALSE);
}

int		ftp_srv_fs_size_file(int fd)
{
	struct stat info;

	if (fstat(fd, &info) < 0)
		return (-1);
	return (info.st_size);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server_builtins_service_2.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/02 20:09:52 by ybarbier          #+#    #+#             */
/*   Updated: 2015/07/02 20:09:53 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_srv.h"

static void	ftp_srv_builtin_get_send(t_srv_ftp *srv_ftp, int fd)
{
	char	*data;
	char	*new_data;
	int		len_data;

	while ((data = ftp_srv_fs_read_file(fd, &len_data)))
	{
		if (srv_ftp->config.type == ASCII)
		{
			new_data = ftp_srv_crlf(data, SRV_CONF, CLI_CONF);
			ftp_srv_dtp_send_data(srv_ftp, new_data, ft_strlen(new_data));
			free(new_data);
		}
		else
			ftp_srv_dtp_send_data(srv_ftp, data, len_data);
		free(data);
	}
}

void		ftp_srv_builtin_get(t_srv_ftp *srv_ftp, char **args)
{
	int		fd;
	char	*msg;

	if (srv_ftp->sock_data != -1)
	{
		ftp_srv_dtp_accept_connection(srv_ftp);
		if ((ftp_srv_fs_file_allow(srv_ftp, &args[1]) == TRUE) &&
			(fd = ftp_srv_fs_open_file(args[1])) != -1)
		{
			msg = ft_str_arrjoin((char *[])
					{"Opening BINARY mode data connection for ", args[1], " (",
					ft_itoa(ftp_srv_fs_size_file(fd)), " bytes).", NULL});
			ftp_srv_pi_send_response(srv_ftp, 150, msg);
			free(msg);
			ftp_srv_builtin_get_send(srv_ftp, fd);
			ftp_srv_dtp_close_channel(srv_ftp);
			ftp_srv_pi_send_response(srv_ftp, 226, "SUCCESS");
			close(fd);
		}
		else
			ftp_srv_pi_send_response(srv_ftp, 550, "Failed to open file.");
	}
	else
		ftp_srv_pi_send_response(srv_ftp, 425, "Use PORT or PASV first.");
}

static void	ftp_srv_builtin_put_receive(t_srv_ftp *srv_ftp, int n_fd)
{
	int		len;
	char	*data_one;
	char	*new_data;

	while ((data_one = ftp_srv_dtp_read_on_channel_one(srv_ftp, &len)))
	{
		if (srv_ftp->config.type == ASCII)
		{
			new_data = ftp_srv_crlf(data_one, CLI_CONF, SRV_CONF);
			ftp_srv_fs_write_in_file(n_fd, new_data, ft_strlen(new_data));
			free(new_data);
		}
		else
			ftp_srv_fs_write_in_file(n_fd, data_one, len);
		free(data_one);
	}
}

void		ftp_srv_builtin_put(t_srv_ftp *srv_ftp, char **args)
{
	int		n_fd;

	if (srv_ftp->sock_data != -1)
	{
		ftp_srv_dtp_accept_connection(srv_ftp);
		ftp_srv_pi_send_response(srv_ftp, 150,
			"Opening BINARY mode data connection for");
		if (((n_fd = ftp_srv_fs_create_file(args[1])) != -1) &&
			(ftp_srv_fs_file_allow(srv_ftp, &args[1]) == TRUE))
		{
			ftp_srv_builtin_put_receive(srv_ftp, n_fd);
			ftp_srv_dtp_close_channel(srv_ftp);
			ftp_srv_pi_send_response(srv_ftp, 226, "SUCCESS");
			close(n_fd);
		}
		else
			ftp_srv_pi_send_response(srv_ftp, 550, "Failed to create file.");
	}
	else
		ftp_srv_pi_send_response(srv_ftp, 425, "Use PORT or PASV first.");
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client_dtp.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/19 15:38:20 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/19 15:38:21 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_cli.h"

void	ftp_cli_dtp_send_data(t_cli_ftp *cli_ftp, char *data, int len)
{
	send(cli_ftp->sock_data, data, len, 0);
}

char	*ftp_cli_dtp_read_on_channel_one(t_cli_ftp *cli_ftp, int *len)
{
	int		r;
	char	*buf;

	if (!(buf = (char*)malloc(sizeof(char) * (BUFF_SIZE_RECV + 1))))
		ft_malloc_error();
	if ((r = recv(cli_ftp->sock_data, buf, BUFF_SIZE_RECV, 0)) > 0)
	{
		buf[r] = 0;
		*len = r;
		return (buf);
	}
	else
		return (NULL);
}

void	ftp_cli_dtp_read_on_channel(t_cli_ftp *cli_ftp)
{
	int		r;
	char	buf[BUFF_SIZE_RECV + 1];

	while ((r = recv(cli_ftp->sock_data, buf, BUFF_SIZE_RECV, 0)) > 0)
	{
		buf[r] = '\0';
		ft_putstr(buf);
	}
}

int		ftp_cli_dtp_create_channel(t_cli_ftp *cli_ftp)
{
	int		sock;

	if (cli_ftp->host->h_addrtype == AF_INET6)
		sock = ftp_cli_use_ipv6(cli_ftp, cli_ftp->port_data);
	else
		sock = ftp_cli_use_ipv4(cli_ftp, cli_ftp->port_data);
	return (sock);
}

void	ftp_cli_dtp_close_channel(t_cli_ftp *cli_ftp)
{
	close(cli_ftp->sock_data);
	cli_ftp->sock_data = -1;
}

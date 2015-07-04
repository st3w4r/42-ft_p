/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server_dtp.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/19 15:36:11 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/19 15:36:12 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_srv.h"

void	ftp_srv_dtp_accept_connection(t_srv_ftp *srv_ftp)
{
	unsigned int		cslen;
	struct sockaddr_in	csin;

	ftp_srv_ui_display_cmd(srv_ftp, "[CONNECTION LISTEN]");
	cslen = sizeof(csin);
	srv_ftp->cs_data = accept(srv_ftp->sock_data, (struct sockaddr*)&csin,
							&cslen);
	ftp_srv_ui_display_cmd(srv_ftp, "[CONNECTION ACCEPT]");
}

int		ftp_srv_dtp_create_channel(t_srv_ftp *srv_ftp)
{
/*	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	(void)srv_ftp;
	proto = getprotobyname("tcp");
	if (proto == 0)
		ft_error_str_exit("Proto error\n");
	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = 0;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		ft_error_str_exit("Bind error\n");
	if (listen(sock, 42) == -1)
		ft_error_str_exit("Listen error\n");
	return (sock);
*/
	int		sock;

(void)srv_ftp;
	// if (srv_ftp->host->h_addrtype == AF_INET6)
		sock = ftp_srv_use_ipv6(0);
	// else

		// sock = ftp_srv_use_ipv4(0);
	if (listen(sock, 42) == -1)
		ft_error_str_exit("Listen error\n");
	return (sock);
}

void	ftp_srv_dtp_close_channel(t_srv_ftp *srv_ftp)
{
	close(srv_ftp->cs_data);
	close(srv_ftp->sock_data);
	srv_ftp->cs_data = -1;
	srv_ftp->sock_data = -1;
}

void	ftp_srv_dtp_send_data(t_srv_ftp *srv_ftp, char *data, int len)
{
	send(srv_ftp->cs_data, data, len, 0);
}

char	*ftp_srv_dtp_read_on_channel_one(t_srv_ftp *srv_ftp, int *len)
{
	int		r;
	char	*buf;

	if (!(buf = (char*)malloc(sizeof(char) * (BUFF_SIZE_RECV + 1))))
		ft_malloc_error();
	if ((r = recv(srv_ftp->cs_data, buf, BUFF_SIZE_RECV, 0)) > 0)
	{
		buf[r] = 0;
		*len = r;
		return (buf);
	}
	else
		return (NULL);
}

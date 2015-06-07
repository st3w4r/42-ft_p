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

void	ftp_cli_dtp_read_on_channel(t_cli_ftp *cli_ftp)
{
	int r;
	char buf[1024];

	while ((r = recv(cli_ftp->sock_data, buf, 1023, 0)) > 0)
	{
		buf[r] = '\0';
		ft_putstr(buf);
	}
}

void	ftp_cli_dtp_create_channel(t_cli_ftp *cli_ftp)
{
	int					r;
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		ft_error_str_exit("Proto error\n");

	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(cli_ftp->port_data);
	sin.sin_addr.s_addr = inet_addr(cli_ftp->addr_data);
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		ft_error_str_exit("Connect error\n");
	cli_ftp->sock_data = sock;
}

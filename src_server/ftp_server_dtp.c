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

	puts("ACCEPT IN");
	cslen = sizeof(csin);
	srv_ftp->cs_data = accept(srv_ftp->sock_data, (struct sockaddr*)&csin,  &cslen);
	puts("ACCEPT OUT");
}

int		ftp_srv_dtp_create_channel(t_srv_ftp *srv_ftp)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

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
// printf("%d\n", sin.sin_port);
// printf("%d\n", ntohs(sin.sin_addr.s_addr));

	return(sock);
}

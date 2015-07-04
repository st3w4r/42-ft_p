/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server_ui.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/27 21:09:37 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/27 21:09:38 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_srv.h"

void	ftp_srv_ui_display_cmd(t_srv_ftp *srv_ftp, char *cmd)
{
	char	buf[INET6_ADDRSTRLEN];

	ft_putstr("[");
	ft_putstr(inet_ntop(AF_INET6, &(srv_ftp->csin.sin6_addr), buf,
						sizeof(buf)));
	ft_putstr(":");
	ft_putnbr(ntohs(srv_ftp->csin.sin6_port));
	ft_putstr("] ");
	ft_putendl(cmd);
}

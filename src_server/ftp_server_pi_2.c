/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server_pi_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/02 22:50:57 by ybarbier          #+#    #+#             */
/*   Updated: 2015/07/02 22:50:58 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_srv.h"

void	ftp_srv_pi_send_response(t_srv_ftp *srv_ftp, int co, char *msg)
{
	char *res;

	res = ft_itoa(co);
	res = ft_strjoin_free_l(res, " ");
	res = ft_strjoin_free_l(res, msg);
	res = ft_strjoin_free_l(res, "\r\n");
	send(srv_ftp->cs, res, ft_strlen(res), 0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server_pi.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/19 15:35:58 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/19 15:36:01 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_srv.h"

void	ftp_src_pi_cmd(t_cmd cmd)
{
	if (!ft_strcmp(cmd, "CWD"))
		ftp_srv_builtin_cwd(cmd);
	else if (!ft_strcmp(cmd, "PWD"))
		ftp_srv_builtin_pwd(cmd);
	else if (!ft_strcmp(cmd, "LIST"))
		ftp_srv_builtin_list(cmd);
	else if (!ft_strcmp(cmd, "RETR"))
		ftp_srv_builtin_retr(cmd);
	else if (!ft_strcmp(cmd, "STOR"))
		ftp_srv_builtin_stor(cmd);
}

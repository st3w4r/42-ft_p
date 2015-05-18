/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/17 15:12:42 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/17 15:12:43 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_cli.h"

void	ftp_display_prompt(void)
{
	ft_putstr("\033[1;37m");
	ft_putstr("FTP $> ");
	ft_putstr("\033[0m");
}

void	ftp_receive_msg(t_bool done)
{
	if (done)
		ft_putstr_fd("\033[1;34mSUCCESS\n\033[0m", 1);
	else
		ft_putstr_fd("\033[1;31mERROR\n\033[0m", 2);
}

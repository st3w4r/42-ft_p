/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/25 11:36:10 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/25 11:36:13 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_cli.h"

void	ftp_receive_msg(t_bool done)
{
	if (done)
		ft_putstr_fd("\033[1;34mSUCCESS\n\033[0m", 1);
	else
		ft_putstr_fd("\033[1;31mERROR\n\033[0m", 2);
}

char	*ftp_create_cmd_line(char *name, char **args)
{
	char *cmd_line;

	cmd_line = ft_strdup(name);
	while (*args)
	{
		cmd_line = ft_strjoin_free_l(ft_strjoin_free_l(cmd_line, " "), *args);
		++args;
	}
	return (cmd_line);
}

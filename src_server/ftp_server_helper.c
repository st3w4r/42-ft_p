/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/25 11:48:04 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/25 11:48:05 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_srv.h"

void		ftp_redirect_fd(int srcfd, int dstfd)
{
	if (dup2(srcfd, dstfd) == -1)
		ft_error_str("Error redirect fd");
}

void		ftp_fork_process(char *path, char **av)
{
	pid_t			pid;
	int				options;
	int				*status;
	struct rusage	*rusage;

	pid = fork();
	if (pid > 0)
	{
		wait4(pid, status, options, rusage);
	}
	else if (pid == 0)
	{
		if (execv(av[0], av) == -1)
			ft_error_str("Exec format error.\n");
		exit(0);
	}
}

char	*ft_str_toupper(char *str)
{
	char *first;

	first = str;
	while (str && *str)
	{
		*str = ft_toupper(*str);
		++str;
	}
	str = first;
	return (str);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/16 13:30:32 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/16 13:30:33 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_srv.h"

static void		usage(char *str)
{
	ft_putstr("Usage: ");
	ft_putstr(str);
	ft_putstr(" <port>\n");
	exit(1);
}

int				main(int ac, char **av)
{
	int		port;

	if (ac != 2)
		usage(av[0]);
	port = ft_atoi(av[1]);
	ftp_create_socket(port);
	return (0);
}

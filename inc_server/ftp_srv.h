/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_srv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/15 18:33:05 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/15 18:33:08 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTP_SRV_H
# define FTP_SRV_H

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct	s_code_ret {
	int			code_ret;
	char		*msg_ret;
}				t_code_ret;

/*
** Name: ftp_server
** File: ftp_server.c
** Desc: Fucntions start server
*/
void	ftp_create_socket(int port);



#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_cli.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/15 18:32:47 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/15 18:32:48 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTP_CLI_H
# define FTP_CLI_H

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define TRUE 1
#define FALSE 0

typedef unsigned char   t_bool;

/*
** Name: ftp_client
** File: ftp_client.c
** Desc: Fucntions start client
*/
int		ftp_create_client(char *addr, int port);

/*
** Name: ftp_helper
** File: ftp_helper.c
** Desc: Fucntions helpers
*/
void	ftp_display_prompt(void);
void	ftp_receive_msg(t_bool done);


#endif

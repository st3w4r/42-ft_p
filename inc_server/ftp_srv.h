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

# define TRUE 1
# define FALSE 0

# define FREE_ARR(x) { if (x) ft_arrfree(&x); }

typedef unsigned char	t_bool;

typedef struct	s_code_ret {
	int			code_ret;
	char		*msg_ret;
}				t_code_ret;

typedef struct	s_srv_ftp
{
	char	*addr;
	int		port;
	int		sock;
	int		cs;
}				t_srv_ftp;


/*
** Name: ftp_server
** File: ftp_server.c
** Desc: Fucntions start server
*/
void	ftp_create_socket(t_srv_ftp *srv_ftp);


/*
** Name: ftp_server_builtins
** File: ftp_server_builtins.c
** Desc: All builtins servers
*/
void	ftp_srv_builtin_ls(t_srv_ftp *srv_ftp, char **args);
void	ftp_srv_builtin_cd(t_srv_ftp *srv_ftp, char **args);
void	ftp_srv_builtin_pwd(t_srv_ftp *srv_ftp, char **args);
void	ftp_srv_builtin_get(t_srv_ftp *srv_ftp, char **args);
void	ftp_srv_builtin_put(t_srv_ftp *srv_ftp, char **args);
void	ftp_srv_builtin_quit(t_srv_ftp *srv_ftp, char **args);
void	ftp_srv_builtin_user(t_srv_ftp *srv_ftp, char **args);

/*
** Name: ftp_server_helper
** File: ftp_server_helper.c
** Desc: Function supplementary for server
*/
void	ftp_redirect_fd(int srcfd, int dstfd);
void	ftp_fork_process(char *path, char **av);
char	*ft_str_toupper(char *str);


/*
** Name: Struct of commands nvt, list in a global array.
*/
typedef	void	(*t_builtin_func)(t_srv_ftp *, char **);

typedef struct	s_cmd_nvt
{
	char			*name;
	char			**args;
	// char			*line_send;
	t_builtin_func	builtin;
}				t_cmd_nvt;

static t_cmd_nvt	g_cmd_nvt_list[] =
{
	{ "LIST",	NULL, ftp_srv_builtin_ls},
	{ "CWD",	NULL, ftp_srv_builtin_cd},
	{ "PWD",	NULL, ftp_srv_builtin_pwd},
	{ "RETR",	NULL, ftp_srv_builtin_get},
	{ "STOR",	NULL, ftp_srv_builtin_put},
	{ "QUIT",	NULL, ftp_srv_builtin_quit},
	{ "USER",	NULL, ftp_srv_builtin_user},
	{ 0 }
};

#endif

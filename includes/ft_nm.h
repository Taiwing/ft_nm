/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 10:46:42 by yforeau           #+#    #+#             */
/*   Updated: 2022/12/15 11:34:05 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <elf.h>
#include "libft.h"

// ft_nm short options
#define	FT_NM_OPT	"eagurph"

/*
** ft_nm constants
*/
extern const t_opt		g_nm_opt[];
extern const char		*g_nm_help[];
extern const char		*g_nm_usage[];

/*
** ft_nm's global configuration
*/
typedef struct	s_nm_config
{
	// executable name
	const char	*exec;
	// boolean set to 1 if e option is passed
	int			elfmode;
}				t_nm_config;

/*
** functions
*/
char	**parse_arguments(t_nm_config *cfg, int argc, char **argv);

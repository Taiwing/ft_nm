/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 10:46:47 by yforeau           #+#    #+#             */
/*   Updated: 2022/12/15 11:34:01 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int		main(int argc, char **argv)
{
	char		**args = NULL;
	t_nm_config	cfg = { .exec = *argv };

	ft_exitmsg((char *)cfg.exec);
	args = parse_arguments(&cfg, argc, argv);
	(void)args;
	ft_exit(EXIT_SUCCESS, NULL);
	return (EXIT_SUCCESS);
}

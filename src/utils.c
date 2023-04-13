/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 18:30:00 by yforeau           #+#    #+#             */
/*   Updated: 2023/04/13 18:40:03 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

/*
** Returns 1 if the string contains a '\0' in the size_max first characters
*/
int	is_null_terminated(char *string, size_t size_max)
{
	return (!!ft_memchr((void *)string, '\0', size_max));
}

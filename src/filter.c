/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 18:50:43 by yforeau           #+#    #+#             */
/*   Updated: 2023/04/13 18:50:44 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int	filter_debug_out(void *ref, t_nm_symbol *symbol)
{
	(void)ref;

	if (symbol->name[0] == '.')
		return (0);
	return (!ft_strchr(NM_DEBUG_TYPES, symbol->type));
}

int	filter_extern_only(void *ref, t_nm_symbol *symbol)
{
	(void)ref;

	if (symbol->type == 'N')
		return (0);
	return ((symbol->type >= 'A' && symbol->type <= 'Z')
		|| ft_strchr("vw", symbol->type));
}

int	filter_undefined_only(void *ref, t_nm_symbol *symbol)
{
	(void)ref;

	return (!!ft_strchr(NM_UNDEFINED_TYPES, symbol->type));
}

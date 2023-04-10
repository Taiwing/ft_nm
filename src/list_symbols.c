/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_symbols.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 18:52:16 by yforeau           #+#    #+#             */
/*   Updated: 2023/04/10 20:01:00 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void			delete_symbol(void *symbol, size_t size)
{
	(void)size;

	ft_memdel(&symbol);
}

static int			list(t_list **dest, t_nm_file *file, t_nm_config *cfg)
{
	if (file->class == ELFCLASS32)
		return (list32(dest, file, cfg));
	else
		return (list64(dest, file, cfg));
}

static void			filter(t_list **symbols, t_nm_config *cfg)
{
	if (cfg->filter & FILTER_DEBUG)
		ft_lstdel_if(symbols, NULL, filter_debug_out, delete_symbol);
	if (cfg->filter & FILTER_EXTERN)
		ft_lstdel_if(symbols, NULL, filter_extern_only, delete_symbol);
	if (cfg->filter & FILTER_UNDEFINED)
		ft_lstdel_if(symbols, NULL, filter_undefined_only, delete_symbol);
}

static void			sort(t_list *symbols, t_nm_config *cfg)
{
	if (cfg->sort == E_SORT_NONE)
		return;
	if (cfg->sort == E_SORT_DEFAULT)
		ft_lst_quicksort(symbols, ft_lst_size(symbols), sort_symbols_default);
	if (cfg->sort == E_SORT_REVERSE)
		ft_lst_quicksort(symbols, ft_lst_size(symbols), sort_symbols_reverse);
}

static void			print(t_list *symbols, t_nm_file *file)
{
	for (t_list *ptr = symbols; ptr; ptr = ptr->next)
	{
		t_nm_symbol	*symbol = ptr->content;
		if (ft_strchr(NM_UNDEFINED_TYPES, symbol->type))
			ft_printf("%*c %c %s\n", file->class == ELFCLASS32
				? NM_VALUE_PADDING_32 : NM_VALUE_PADDING_64,
				' ', symbol->type, symbol->name);
		else
			ft_printf("%0*llx %c %s\n", file->class == ELFCLASS32
				? NM_VALUE_PADDING_32 : NM_VALUE_PADDING_64,
				symbol->value, symbol->type, symbol->name);
	}
}

int					list_symbols(t_nm_file *file, t_nm_config *cfg)
{
	t_list	*symbols = NULL;

	(void)cfg;
	if (list(&symbols, file, cfg) || !symbols)
	{
		ft_lstdel(&symbols, delete_symbol);
		return (1);
	}
	filter(&symbols, cfg);
	sort(symbols, cfg);
	print(symbols, file);
	ft_lstdel(&symbols, delete_symbol);
	return (0);
}

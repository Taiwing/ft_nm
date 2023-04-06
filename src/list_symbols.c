/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_symbols.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 18:52:16 by yforeau           #+#    #+#             */
/*   Updated: 2023/04/01 21:52:29 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void			delete_symbol(void *symbol, size_t size)
{
	(void)size;

	ft_memdel(&symbol);
}

static int			list(t_list **dest, t_nm_file *file)
{
	if (file->class == ELFCLASS32)
		return (list32(dest, file));
	else if (file->class == ELFCLASS64)
		return (list64(dest, file));
	return (1);
}

#define NM_DEBUG_TYPES		"aAnN"

static int			filter_debug_out(void *ref, t_nm_symbol *symbol)
{
	(void)ref;

	return (!ft_strchr(NM_DEBUG_TYPES, symbol->type));
}

static int			filter_extern_only(void *ref, t_nm_symbol *symbol)
{
	(void)ref;

	return ((symbol->type >= 'A' && symbol->type <= 'Z')
		|| ft_strchr("vw", symbol->type));
}

#define NM_UNDEFINED_TYPES	"Uuvw"

static int			filter_undefined_only(void *ref, t_nm_symbol *symbol)
{
	(void)ref;

	return (!!ft_strchr(NM_UNDEFINED_TYPES, symbol->type));
}

static void			filter_symbols(t_list **symbols, t_nm_config *cfg)
{
	if (cfg->filter & FILTER_DEBUG)
		ft_lstdel_if(symbols, NULL, filter_debug_out, delete_symbol);
	if (cfg->filter & FILTER_EXTERN)
		ft_lstdel_if(symbols, NULL, filter_extern_only, delete_symbol);
	if (cfg->filter & FILTER_UNDEFINED)
		ft_lstdel_if(symbols, NULL, filter_undefined_only, delete_symbol);
}

static int			sort_symbols_default(void *a, void *b)
{
	t_nm_symbol	*sym_a = a, *sym_b = b;
	int			cmp = strcoll(sym_a->name, sym_b->name);

	return (cmp ? cmp : sym_a->idx - sym_b->idx);
}

static int			sort_symbols_reverse(void *a, void *b)
{
	t_nm_symbol	*sym_a = a, *sym_b = b;
	int			cmp = strcoll(sym_b->name, sym_a->name);

	return (cmp ? cmp : sym_a->idx - sym_b->idx);
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

#define NM_UNDEFINED_TYPES	"Uuvw"
#define NM_VALUE_PADDING_32	8
#define NM_VALUE_PADDING_64	16

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
	if (list(&symbols, file) || !symbols)
	{
		ft_lstdel(&symbols, delete_symbol);
		return (1);
	}
	filter_symbols(&symbols, cfg);
	sort(symbols, cfg);
	print(symbols, file);
	ft_lstdel(&symbols, delete_symbol);
	return (0);
}

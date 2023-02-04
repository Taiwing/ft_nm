/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 19:47:15 by yforeau           #+#    #+#             */
/*   Updated: 2023/02/04 18:43:45 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

const char		*g_nm_osabi[ELF_OSABI_MAX + 1] = {
	"System V",
	"HP-UX",
	"NetBSD",
	"Linux",
	"GNU Hurd",
	NULL,
	"Solaris",
	"AIX (Monterey)",
	"IRIX",
	"FreeBSD",
	"Tru64",
	"Novell Modesto",
	"OpenBSD",
	"OpenVMS",
	"NonStop Kernel",
	"AROS",
	"FenixOS",
	"Nuxi CloudABI",
	"Stratus Technologies OpenVOS",
};

const char		*g_nm_type[] = {
	[ET_NONE] = "NONE (None)",
	[ET_REL] = "REL (Relocatable file)",
	[ET_EXEC] = "EXEC (Executable file)",
	[ET_DYN] = "DYN (Shared object file)",
	[ET_CORE] = "CORE (Core file)",
	[ET_CORE + 1] = "OS Specific: (%x)",
	[ET_CORE + 2] = "Processor Specific: (%x)",
	[ET_CORE + 3] = "<unknown>: %x",
};

const char		*g_nm_machine[] = {
	[EM_NONE] = "None",
	[EM_M32] = "WE32100",
	[EM_SPARC] = "Sparc",
	[EM_386] = "Intel 80386",
	[EM_68K] = "MC68000",
	[EM_88K] = "MC88000",
	[EM_486] = "Intel 80486",
	[EM_860] = "Intel 80860",
	[EM_MIPS] = "MIPS R3000",
	[EM_MIPS_RS3_LE] = "MIPS R3000 little-endian",
	[EM_PARISC] = "HPPA",
	[EM_SPARC32PLUS] = "Sparc v8+",
	[EM_PPC] = "PowerPC",
	[EM_PPC64] = "PowerPC64",
	[EM_SPU] = "SPU",
	[EM_ARM] = "ARM",
	[EM_SH] = "Renesas / SuperH SH",
	[EM_SPARCV9] = "Sparc v9",
	[EM_H8_300] = "Renesas H8/300",
	[EM_IA_64] = "Intel IA-64",
	[EM_X86_64] = "Advanced Micro Devices X86-64",
	[EM_S390] = "IBM S/390",
	[EM_CRIS] = "Axis Communications 32-bit embedded processor",
	[EM_M32R] = "Renesas M32R (formerly Mitsubishi M32r)",
	[EM_MN10300] = "mn10300",
	[EM_OPENRISC] = "OpenRISC",
	[EM_ARCOMPACT] = "ARCompact processor",
	[EM_XTENSA] = "Tensilica Xtensa Processor",
	[EM_BLACKFIN] = "Analog Devices Blackfin",
	[EM_UNICORE] = "UniCore-32",
	[EM_ALTERA_NIOS2] = "Altera Nios II",
	[EM_TI_C6000] = "TI C6X DSPs",
	[EM_HEXAGON] = "QUALCOMM Hexagon",
	[EM_NDS32] = "Andes Technology compact code size embedded RISC processor family",
	[EM_AARCH64] = "ARM 64 bit",
	[EM_TILEPRO] = "Tilera TILEPro",
	[EM_MICROBLAZE] = "Xilinx MicroBlaze",
	[EM_TILEGX] = "Tilera TILE-Gx",
	[EM_ARCV2] = "ARCv2 Cores",
	[EM_RISCV] = "RISC-V",
	[EM_BPF] = "Linux BPF - in-kernel virtual machine",
	[EM_CSKY] = "C-SKY",
};

/**********************************************************************/
/*   ____  ____                                                       */
/*  /   /\/   /                                                       */
/* /___/  \  /                                                        */
/* \   \   \/                                                         */
/*  \   \        Copyright (c) 2003-2020 Xilinx, Inc.                 */
/*  /   /        All Right Reserved.                                  */
/* /---/   /\                                                         */
/* \   \  /  \                                                        */
/*  \___\/\___\                                                       */
/**********************************************************************/


/* NOTE: DO NOT EDIT. AUTOMATICALLY GENERATED FILE. CHANGES WILL BE LOST. */

#ifndef DPI_H
#define DPI_H
#ifdef __cplusplus
#define DPI_LINKER_DECL  extern "C" 
#else
#define DPI_LINKER_DECL
#endif

#include "svdpi.h"


typedef struct {
	svBit is_branch;
	svBit is_j;
	svBit is_pc_changer;
	svBit is_jal;
	svBit is_jalr;
	svBit is_pop_ras;
	svBit is_push_ras;
	svBit is_other;
} predecoded_t_SV;


typedef struct {
	svLogicVecVal instr[SV_PACKED_DATA_NELEMS(32)];
	svLogicVecVal current_pc[SV_PACKED_DATA_NELEMS(64)];
	svLogicVecVal immediate[SV_PACKED_DATA_NELEMS(32)];
	svLogicVecVal if_jumps_pc[SV_PACKED_DATA_NELEMS(64)];
	predecoded_t_SV pred_type;
	svBit is_compressed;
	svBit valid;
} predecoded_instr_t_SV;


typedef struct {
	predecoded_instr_t_SV inst_pkg[4];
} instr_packet_t_SV;


typedef struct {
	svBit ex_branch_i;
	svBit ex_jump_i;
	svLogicVecVal ex_result_i[SV_PACKED_DATA_NELEMS(64)];
	svLogicVecVal ex_pc_i[SV_PACKED_DATA_NELEMS(64)];
	svLogicVecVal ex_target_pc_i[SV_PACKED_DATA_NELEMS(64)];
	svBit ex_mispred_i;
} execute_output_t_SV;



/* Imported (by SV) function */
DPI_LINKER_DECL DPI_DLLESPEC 
 void dpi_model(
	const instr_packet_t_SV* instructions ,
	execute_output_t_SV* output_instr);


#endif

/* Process model C form file: WSN_Slot_Node_Proc.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char WSN_Slot_Node_Proc_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A op_runsim 7 61D05971 61D05971 1 DESKTOP-RD4S7T2 51133 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                  ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

#define   	Intrpt_Slot         		20000	//È«ÍøÖ¡ÖÐ¶ÏÂë


#define		Slot_Intrpt	(op_intrpt_type() == OPC_INTRPT_MCAST) && (op_intrpt_code() == Intrpt_Slot)

/* End of Header Block */

#if !defined (VOSD_NO_FIN)
#undef	BIN
#undef	BOUT
#define	BIN		FIN_LOCAL_FIELD(_op_last_line_passed) = __LINE__ - _op_block_origin;
#define	BOUT	BIN
#define	BINIT	FIN_LOCAL_FIELD(_op_last_line_passed) = 0; _op_block_origin = __LINE__;
#else
#define	BINIT
#endif /* #if !defined (VOSD_NO_FIN) */



/* State variable definitions */
typedef struct
	{
	/* Internal state tracking for FSM */
	FSM_SYS_STATE
	} WSN_Slot_Node_Proc_state;


/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	WSN_Slot_Node_Proc_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((WSN_Slot_Node_Proc_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* No Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ };
#endif

/* Undefine optional tracing in FIN/FOUT/FRET */
/* The FSM has its own tracing code and the other */
/* functions should not have any tracing.		  */
#undef FIN_TRACING
#define FIN_TRACING

#undef FOUTRET_TRACING
#define FOUTRET_TRACING

#if defined (__cplusplus)
extern "C" {
#endif
	void WSN_Slot_Node_Proc (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_WSN_Slot_Node_Proc_init (int * init_block_ptr);
	void _op_WSN_Slot_Node_Proc_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_WSN_Slot_Node_Proc_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_WSN_Slot_Node_Proc_alloc (VosT_Obtype, int);
	void _op_WSN_Slot_Node_Proc_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
WSN_Slot_Node_Proc (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (WSN_Slot_Node_Proc ());

		{


		FSM_ENTER ("WSN_Slot_Node_Proc")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (st_0) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "st_0", "WSN_Slot_Node_Proc [st_0 enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_Slot_Node_Proc [st_0 enter execs]", state0_enter_exec)
				{
				op_intrpt_schedule_mcast_global(op_sim_time()+0.5,Intrpt_Slot);
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** state (st_0) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "st_0", "WSN_Slot_Node_Proc [st_0 exit execs]")


			/** state (st_0) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "st_0", "st_1", "tr_0", "WSN_Slot_Node_Proc [st_0 -> st_1 : default / ]")
				/*---------------------------------------------------------*/



			/** state (st_1) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "st_1", state1_enter_exec, "WSN_Slot_Node_Proc [st_1 enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_Slot_Node_Proc [st_1 enter execs]", state1_enter_exec)
				{
				if(Slot_Intrpt) op_intrpt_schedule_mcast_global(op_sim_time()+0.5,Intrpt_Slot);
				}
				FSM_PROFILE_SECTION_OUT (state1_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"WSN_Slot_Node_Proc")


			/** state (st_1) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "st_1", "WSN_Slot_Node_Proc [st_1 exit execs]")


			/** state (st_1) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_Slot_Node_Proc [st_1 trans conditions]", state1_trans_conds)
			FSM_INIT_COND (Slot_Intrpt)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("st_1")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, ;, "Slot_Intrpt", "", "st_1", "st_1", "tr_2", "WSN_Slot_Node_Proc [st_1 -> st_1 : Slot_Intrpt / ]")
				FSM_CASE_TRANSIT (1, 1, state1_enter_exec, ;, "default", "", "st_1", "st_1", "tr_3", "WSN_Slot_Node_Proc [st_1 -> st_1 : default / ]")
				}
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"WSN_Slot_Node_Proc")
		}
	}




void
_op_WSN_Slot_Node_Proc_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_WSN_Slot_Node_Proc_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_WSN_Slot_Node_Proc_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_WSN_Slot_Node_Proc_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_WSN_Slot_Node_Proc_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (WSN_Slot_Node_Proc)",
		sizeof (WSN_Slot_Node_Proc_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_WSN_Slot_Node_Proc_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	WSN_Slot_Node_Proc_state * ptr;
	FIN_MT (_op_WSN_Slot_Node_Proc_alloc (obtype))

	ptr = (WSN_Slot_Node_Proc_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "WSN_Slot_Node_Proc [st_0 enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_WSN_Slot_Node_Proc_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{

	FIN_MT (_op_WSN_Slot_Node_Proc_svar (gen_ptr, var_name, var_p_ptr))

	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}


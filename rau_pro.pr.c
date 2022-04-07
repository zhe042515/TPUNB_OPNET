/* Process model C form file: rau_pro.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char rau_pro_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A modeler 7 60588E7B 60588E7B 1 DESKTOP-RD4S7T2 51133 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                    ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>

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
	/* State Variables */
	Boolean	                		from_wireless                                   ;
	Boolean	                		from_wired                                      ;
	Packet *	               		mypacket                                        ;
	} rau_pro_state;

#define from_wireless           		op_sv_ptr->from_wireless
#define from_wired              		op_sv_ptr->from_wired
#define mypacket                		op_sv_ptr->mypacket

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	rau_pro_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((rau_pro_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


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
	void rau_pro (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_rau_pro_init (int * init_block_ptr);
	void _op_rau_pro_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_rau_pro_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_rau_pro_alloc (VosT_Obtype, int);
	void _op_rau_pro_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
rau_pro (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (rau_pro ());

		{


		FSM_ENTER ("rau_pro")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "init", "rau_pro [init enter execs]")
				FSM_PROFILE_SECTION_IN ("rau_pro [init enter execs]", state0_enter_exec)
				{
				//printf("rau mac initialize succeed\n");
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** state (init) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "init", "rau_pro [init exit execs]")


			/** state (init) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "init", "get_packet", "tr_0", "rau_pro [init -> get_packet : default / ]")
				/*---------------------------------------------------------*/



			/** state (get_packet) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "get_packet", state1_enter_exec, "rau_pro [get_packet enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"rau_pro")


			/** state (get_packet) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "get_packet", "rau_pro [get_packet exit execs]")
				FSM_PROFILE_SECTION_IN ("rau_pro [get_packet exit execs]", state1_exit_exec)
				{
				if(op_intrpt_type()==OPC_INTRPT_STRM)
				{ 
					printf("RAU received a packet");
					mypacket = op_pk_get (op_intrpt_strm());
					if(op_intrpt_strm()==0) 
					{ 
						from_wired = OPC_TRUE;
						from_wireless = OPC_FALSE; 
					} else if(op_intrpt_strm()==1) 
					{
						from_wired = OPC_FALSE; 
						from_wireless = OPC_TRUE; 
					}
				}
				}
				FSM_PROFILE_SECTION_OUT (state1_exit_exec)


			/** state (get_packet) transition processing **/
			FSM_PROFILE_SECTION_IN ("rau_pro [get_packet trans conditions]", state1_trans_conds)
			FSM_INIT_COND (from_wireless)
			FSM_TEST_COND (from_wired)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("get_packet")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 2, state2_enter_exec, ;, "from_wireless", "", "get_packet", "uplink", "tr_1", "rau_pro [get_packet -> uplink : from_wireless / ]")
				FSM_CASE_TRANSIT (1, 3, state3_enter_exec, ;, "from_wired", "", "get_packet", "downlink", "tr_3", "rau_pro [get_packet -> downlink : from_wired / ]")
				FSM_CASE_TRANSIT (2, 1, state1_enter_exec, ;, "default", "", "get_packet", "get_packet", "tr_5", "rau_pro [get_packet -> get_packet : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (uplink) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "uplink", state2_enter_exec, "rau_pro [uplink enter execs]")
				FSM_PROFILE_SECTION_IN ("rau_pro [uplink enter execs]", state2_enter_exec)
				{
				op_pk_send(mypacket,1);
				printf("rau receive packet from wireless and send it to ap,time = %f\n",op_sim_time());
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (uplink) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "uplink", "rau_pro [uplink exit execs]")


			/** state (uplink) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "uplink", "get_packet", "tr_2", "rau_pro [uplink -> get_packet : default / ]")
				/*---------------------------------------------------------*/



			/** state (downlink) enter executives **/
			FSM_STATE_ENTER_FORCED (3, "downlink", state3_enter_exec, "rau_pro [downlink enter execs]")
				FSM_PROFILE_SECTION_IN ("rau_pro [downlink enter execs]", state3_enter_exec)
				{
				op_pk_send(mypacket,0);
				printf("rau receive packet from wired and send it to user,time = %f\n",op_sim_time());
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** state (downlink) exit executives **/
			FSM_STATE_EXIT_FORCED (3, "downlink", "rau_pro [downlink exit execs]")


			/** state (downlink) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "downlink", "get_packet", "tr_4", "rau_pro [downlink -> get_packet : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"rau_pro")
		}
	}




void
_op_rau_pro_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_rau_pro_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_rau_pro_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_rau_pro_svar function. */
#undef from_wireless
#undef from_wired
#undef mypacket

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_rau_pro_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_rau_pro_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (rau_pro)",
		sizeof (rau_pro_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_rau_pro_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	rau_pro_state * ptr;
	FIN_MT (_op_rau_pro_alloc (obtype))

	ptr = (rau_pro_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "rau_pro [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_rau_pro_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	rau_pro_state		*prs_ptr;

	FIN_MT (_op_rau_pro_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (rau_pro_state *)gen_ptr;

	if (strcmp ("from_wireless" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->from_wireless);
		FOUT
		}
	if (strcmp ("from_wired" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->from_wired);
		FOUT
		}
	if (strcmp ("mypacket" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->mypacket);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}


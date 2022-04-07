/* Process model C form file: ap_mac_pro.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char ap_mac_pro_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A modeler 7 6058927C 6058927C 1 DESKTOP-RD4S7T2 51133 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                    ";
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
	Packet *	               		mypacket                                        ;
	int	                    		my_address                                      ;
	int	                    		ap_mac_id                                       ;
	int	                    		ap_node_id                                      ;
	int	                    		ii                                              ;
	Boolean	                		from_source                                     ;
	Boolean	                		from_receiver                                   ;
	int	                    		send_address                                    ;
	int	                    		receive_address                                 ;
	int	                    		frame_type                                      ;
	} ap_mac_pro_state;

#define mypacket                		op_sv_ptr->mypacket
#define my_address              		op_sv_ptr->my_address
#define ap_mac_id               		op_sv_ptr->ap_mac_id
#define ap_node_id              		op_sv_ptr->ap_node_id
#define ii                      		op_sv_ptr->ii
#define from_source             		op_sv_ptr->from_source
#define from_receiver           		op_sv_ptr->from_receiver
#define send_address            		op_sv_ptr->send_address
#define receive_address         		op_sv_ptr->receive_address
#define frame_type              		op_sv_ptr->frame_type

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	ap_mac_pro_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((ap_mac_pro_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


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
	void ap_mac_pro (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_ap_mac_pro_init (int * init_block_ptr);
	void _op_ap_mac_pro_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_ap_mac_pro_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_ap_mac_pro_alloc (VosT_Obtype, int);
	void _op_ap_mac_pro_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
ap_mac_pro (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (ap_mac_pro ());

		{


		FSM_ENTER ("ap_mac_pro")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (ap_mac) enter executives **/
			FSM_STATE_ENTER_UNFORCED (0, "ap_mac", state0_enter_exec, "ap_mac_pro [ap_mac enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (1,"ap_mac_pro")


			/** state (ap_mac) exit executives **/
			FSM_STATE_EXIT_UNFORCED (0, "ap_mac", "ap_mac_pro [ap_mac exit execs]")
				FSM_PROFILE_SECTION_IN ("ap_mac_pro [ap_mac exit execs]", state0_exit_exec)
				{
				if(op_intrpt_type()==OPC_INTRPT_STRM)
				{
					printf("AP received a packet");
					mypacket = op_pk_get (op_intrpt_strm());
					if(op_intrpt_strm()==0)
					{ 
						from_receiver = OPC_FALSE; 
						from_source = OPC_TRUE; 
					} else if(op_intrpt_strm()==1) 
					{ 
						from_receiver = OPC_TRUE;  
						from_source = OPC_FALSE;
						op_pk_nfd_get(mypacket,"receive_address",&receive_address); 
						if(receive_address!=my_address)
						{ 
							op_pk_destroy(mypacket);
							from_receiver = OPC_FALSE; 
							from_source = OPC_FALSE; 
						} 
					}
				}
				}
				FSM_PROFILE_SECTION_OUT (state0_exit_exec)


			/** state (ap_mac) transition processing **/
			FSM_PROFILE_SECTION_IN ("ap_mac_pro [ap_mac trans conditions]", state0_trans_conds)
			FSM_INIT_COND (from_source)
			FSM_TEST_COND (from_receiver)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("ap_mac")
			FSM_PROFILE_SECTION_OUT (state0_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, ;, "from_source", "", "ap_mac", "send", "tr_1", "ap_mac_pro [ap_mac -> send : from_source / ]")
				FSM_CASE_TRANSIT (1, 3, state3_enter_exec, ;, "from_receiver", "", "ap_mac", "receive", "tr_4", "ap_mac_pro [ap_mac -> receive : from_receiver / ]")
				FSM_CASE_TRANSIT (2, 0, state0_enter_exec, ;, "default", "", "ap_mac", "ap_mac", "tr_3", "ap_mac_pro [ap_mac -> ap_mac : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (send) enter executives **/
			FSM_STATE_ENTER_FORCED (1, "send", state1_enter_exec, "ap_mac_pro [send enter execs]")

			/** state (send) exit executives **/
			FSM_STATE_EXIT_FORCED (1, "send", "ap_mac_pro [send exit execs]")


			/** state (send) transition processing **/
			FSM_TRANSIT_FORCE (0, state0_enter_exec, ;, "default", "", "send", "ap_mac", "tr_2", "ap_mac_pro [send -> ap_mac : default / ]")
				/*---------------------------------------------------------*/



			/** state (init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (2, "init", "ap_mac_pro [init enter execs]")
				FSM_PROFILE_SECTION_IN ("ap_mac_pro [init enter execs]", state2_enter_exec)
				{
				ap_mac_id=op_id_self();
				ap_node_id=op_topo_parent(ap_mac_id);
				op_ima_obj_attr_get (ap_node_id, "ap_address", &my_address);
				//printf("ap mac initialize succeed,address = %d\n",my_address);
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (init) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "init", "ap_mac_pro [init exit execs]")


			/** state (init) transition processing **/
			FSM_TRANSIT_FORCE (0, state0_enter_exec, ;, "default", "", "init", "ap_mac", "tr_0", "ap_mac_pro [init -> ap_mac : default / ]")
				/*---------------------------------------------------------*/



			/** state (receive) enter executives **/
			FSM_STATE_ENTER_FORCED (3, "receive", state3_enter_exec, "ap_mac_pro [receive enter execs]")
				FSM_PROFILE_SECTION_IN ("ap_mac_pro [receive enter execs]", state3_enter_exec)
				{
				op_pk_nfd_get(mypacket,"frame_type",&frame_type);
				op_pk_nfd_get(mypacket,"send_address",&send_address);
				if(frame_type==1)
				{
					op_pk_send(mypacket,1);
					printf("ap mac receive data packet and send it to sink,time = %f\n",op_sim_time()); mypacket = op_pk_create_fmt("ack"); 
					op_pk_nfd_set(mypacket, "frame_type", 2);
					op_pk_nfd_set(mypacket, "receive_address", send_address); 
					op_pk_nfd_set(mypacket, "send_address", my_address);
					op_pk_send(mypacket,0); 
					printf("ap send ack packet,time = %f\n",op_sim_time());
				}
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** state (receive) exit executives **/
			FSM_STATE_EXIT_FORCED (3, "receive", "ap_mac_pro [receive exit execs]")


			/** state (receive) transition processing **/
			FSM_TRANSIT_FORCE (0, state0_enter_exec, ;, "default", "", "receive", "ap_mac", "tr_5", "ap_mac_pro [receive -> ap_mac : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (2,"ap_mac_pro")
		}
	}




void
_op_ap_mac_pro_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_ap_mac_pro_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_ap_mac_pro_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_ap_mac_pro_svar function. */
#undef mypacket
#undef my_address
#undef ap_mac_id
#undef ap_node_id
#undef ii
#undef from_source
#undef from_receiver
#undef send_address
#undef receive_address
#undef frame_type

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_ap_mac_pro_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_ap_mac_pro_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (ap_mac_pro)",
		sizeof (ap_mac_pro_state));
	*init_block_ptr = 4;

	FRET (obtype)
	}

VosT_Address
_op_ap_mac_pro_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	ap_mac_pro_state * ptr;
	FIN_MT (_op_ap_mac_pro_alloc (obtype))

	ptr = (ap_mac_pro_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "ap_mac_pro [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_ap_mac_pro_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	ap_mac_pro_state		*prs_ptr;

	FIN_MT (_op_ap_mac_pro_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (ap_mac_pro_state *)gen_ptr;

	if (strcmp ("mypacket" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->mypacket);
		FOUT
		}
	if (strcmp ("my_address" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->my_address);
		FOUT
		}
	if (strcmp ("ap_mac_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ap_mac_id);
		FOUT
		}
	if (strcmp ("ap_node_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ap_node_id);
		FOUT
		}
	if (strcmp ("ii" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ii);
		FOUT
		}
	if (strcmp ("from_source" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->from_source);
		FOUT
		}
	if (strcmp ("from_receiver" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->from_receiver);
		FOUT
		}
	if (strcmp ("send_address" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->send_address);
		FOUT
		}
	if (strcmp ("receive_address" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->receive_address);
		FOUT
		}
	if (strcmp ("frame_type" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->frame_type);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}


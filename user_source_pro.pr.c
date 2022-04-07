/* Process model C form file: user_source_pro.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char user_source_pro_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A modeler 7 60588F48 60588F48 1 DESKTOP-RD4S7T2 51133 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                    ";
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
	int	                    		user_source_id                                  ;
	int	                    		user_node_id                                    ;
	int	                    		ii                                              ;
	char	                   		name[7]                                         ;
	} user_source_pro_state;

#define mypacket                		op_sv_ptr->mypacket
#define my_address              		op_sv_ptr->my_address
#define user_source_id          		op_sv_ptr->user_source_id
#define user_node_id            		op_sv_ptr->user_node_id
#define ii                      		op_sv_ptr->ii
#define name                    		op_sv_ptr->name

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	user_source_pro_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((user_source_pro_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


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
	void user_source_pro (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_user_source_pro_init (int * init_block_ptr);
	void _op_user_source_pro_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_user_source_pro_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_user_source_pro_alloc (VosT_Obtype, int);
	void _op_user_source_pro_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
user_source_pro (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (user_source_pro ());

		{


		FSM_ENTER_NO_VARS ("user_source_pro")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "init", "user_source_pro [init enter execs]")
				FSM_PROFILE_SECTION_IN ("user_source_pro [init enter execs]", state0_enter_exec)
				{
				user_source_id=op_id_self();
				user_node_id=op_topo_parent(user_source_id);
				for(ii=0;ii<7;ii++)
					{ 
						name[ii]=0;
					}
				op_ima_obj_hname_get(user_node_id,name,7);
				my_address=0;
				for(ii=0;ii<7;ii++)
					{ 
						if(name[ii]>=48 && name[ii]<=57) 
						{ 
							my_address = my_address*10+name[ii]-48;
						}
					}
				//printf("user%d source initialize succeed\n",my_address);
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** state (init) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "init", "user_source_pro [init exit execs]")


			/** state (init) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "init", "make", "tr_0", "user_source_pro [init -> make : default / ]")
				/*---------------------------------------------------------*/



			/** state (make) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "make", state1_enter_exec, "user_source_pro [make enter execs]")
				FSM_PROFILE_SECTION_IN ("user_source_pro [make enter execs]", state1_enter_exec)
				{
				mypacket = op_pk_create_fmt("data");
				op_pk_total_size_set (mypacket,5000);
				op_pk_nfd_set(mypacket, "frame_type", 1);
				op_pk_nfd_set(mypacket, "receive_address", 1);
				op_pk_nfd_set(mypacket, "send_address", my_address);
				op_pk_send_delayed(mypacket,0,my_address);
				printf("user%d source make data packet,time=%f\n",my_address,op_sim_time());
				}
				FSM_PROFILE_SECTION_OUT (state1_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"user_source_pro")


			/** state (make) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "make", "user_source_pro [make exit execs]")


			/** state (make) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "make", "make", "tr_1", "user_source_pro [make -> make : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"user_source_pro")
		}
	}




void
_op_user_source_pro_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_user_source_pro_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_user_source_pro_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_user_source_pro_svar function. */
#undef mypacket
#undef my_address
#undef user_source_id
#undef user_node_id
#undef ii
#undef name

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_user_source_pro_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_user_source_pro_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (user_source_pro)",
		sizeof (user_source_pro_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_user_source_pro_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	user_source_pro_state * ptr;
	FIN_MT (_op_user_source_pro_alloc (obtype))

	ptr = (user_source_pro_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "user_source_pro [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_user_source_pro_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	user_source_pro_state		*prs_ptr;

	FIN_MT (_op_user_source_pro_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (user_source_pro_state *)gen_ptr;

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
	if (strcmp ("user_source_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->user_source_id);
		FOUT
		}
	if (strcmp ("user_node_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->user_node_id);
		FOUT
		}
	if (strcmp ("ii" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ii);
		FOUT
		}
	if (strcmp ("name" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->name);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}


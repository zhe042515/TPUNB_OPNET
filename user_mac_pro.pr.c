/* Process model C form file: user_mac_pro.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char user_mac_pro_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A modeler 7 60589890 60589890 1 DESKTOP-RD4S7T2 51133 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                    ";
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
	int	                    		frame_type                                      ;
	Boolean	                		from_source                                     ;
	Boolean	                		from_receiver                                   ;
	int	                    		receive_address                                 ;
	int	                    		user_mac_id                                     ;
	int	                    		user_node_id                                    ;
	int	                    		ii                                              ;
	char	                   		name[7]                                         ;
	int	                    		my_address                                      ;
	} user_mac_pro_state;

#define mypacket                		op_sv_ptr->mypacket
#define frame_type              		op_sv_ptr->frame_type
#define from_source             		op_sv_ptr->from_source
#define from_receiver           		op_sv_ptr->from_receiver
#define receive_address         		op_sv_ptr->receive_address
#define user_mac_id             		op_sv_ptr->user_mac_id
#define user_node_id            		op_sv_ptr->user_node_id
#define ii                      		op_sv_ptr->ii
#define name                    		op_sv_ptr->name
#define my_address              		op_sv_ptr->my_address

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	user_mac_pro_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((user_mac_pro_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


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
	void user_mac_pro (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_user_mac_pro_init (int * init_block_ptr);
	void _op_user_mac_pro_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_user_mac_pro_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_user_mac_pro_alloc (VosT_Obtype, int);
	void _op_user_mac_pro_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
user_mac_pro (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (user_mac_pro ());

		{


		FSM_ENTER ("user_mac_pro")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "init", "user_mac_pro [init enter execs]")
				FSM_PROFILE_SECTION_IN ("user_mac_pro [init enter execs]", state0_enter_exec)
				{
				user_mac_id=op_id_self();
				user_node_id=op_topo_parent(user_mac_id);
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
				//printf("user%d mac initialize succeed\n",my_address);
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** state (init) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "init", "user_mac_pro [init exit execs]")


			/** state (init) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "init", "get_packet", "tr_0", "user_mac_pro [init -> get_packet : default / ]")
				/*---------------------------------------------------------*/



			/** state (get_packet) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "get_packet", state1_enter_exec, "user_mac_pro [get_packet enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"user_mac_pro")


			/** state (get_packet) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "get_packet", "user_mac_pro [get_packet exit execs]")
				FSM_PROFILE_SECTION_IN ("user_mac_pro [get_packet exit execs]", state1_exit_exec)
				{
				if(op_intrpt_type()==OPC_INTRPT_STRM)
				{ 
					mypacket = op_pk_get (op_intrpt_strm());
					if(op_intrpt_strm()==0) 
					{ 
						from_receiver =OPC_FALSE;  
						from_source = OPC_TRUE; 
					} else if(op_intrpt_strm()==1) 
					{ 
						printf("user%d received a stream\n",my_address);
						from_receiver = OPC_TRUE;  
						from_source = OPC_FALSE; 
						op_pk_nfd_get(mypacket,"receive_address",&receive_address);
						if(receive_address!=my_address) 
						{ 
							op_pk_destroy(mypacket);
							from_receiver = OPC_FALSE;  
							from_source = OPC_FALSE; 
						}	
						else
						{
							printf("user%d received a packet\n",my_address);
						}
					}
				}
				}
				FSM_PROFILE_SECTION_OUT (state1_exit_exec)


			/** state (get_packet) transition processing **/
			FSM_PROFILE_SECTION_IN ("user_mac_pro [get_packet trans conditions]", state1_trans_conds)
			FSM_INIT_COND (from_receiver)
			FSM_TEST_COND (from_source)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("get_packet")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 2, state2_enter_exec, ;, "from_receiver", "", "get_packet", "uplink", "tr_1", "user_mac_pro [get_packet -> uplink : from_receiver / ]")
				FSM_CASE_TRANSIT (1, 3, state3_enter_exec, ;, "from_source", "", "get_packet", "downlink", "tr_4", "user_mac_pro [get_packet -> downlink : from_source / ]")
				FSM_CASE_TRANSIT (2, 1, state1_enter_exec, ;, "default", "", "get_packet", "get_packet", "tr_6", "user_mac_pro [get_packet -> get_packet : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (uplink) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "uplink", state2_enter_exec, "user_mac_pro [uplink enter execs]")
				FSM_PROFILE_SECTION_IN ("user_mac_pro [uplink enter execs]", state2_enter_exec)
				{
				op_pk_nfd_get(mypacket,"frame_type",&frame_type);
				printf("frame_type=%d\n",frame_type);
				if(frame_type==2)
				{ 
					op_pk_destroy (mypacket); 
					printf("user%d receive ack packet and destroy it,time = %f\n",my_address,op_sim_time());
				}
				if(frame_type==2)
				{ 
					//printf("user is ready to send\n");
					int user_source_id = op_id_from_name(user_node_id,OPC_OBJTYPE_PROC,"user_source");
					Prohandle user_source_pro = op_pro_root(user_source_id); 
					op_intrpt_schedule_process(user_source_pro,op_sim_time()+5,1);
					printf("user is ready to send\n");
				}
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (uplink) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "uplink", "user_mac_pro [uplink exit execs]")


			/** state (uplink) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "uplink", "get_packet", "tr_2", "user_mac_pro [uplink -> get_packet : default / ]")
				/*---------------------------------------------------------*/



			/** state (downlink) enter executives **/
			FSM_STATE_ENTER_FORCED (3, "downlink", state3_enter_exec, "user_mac_pro [downlink enter execs]")
				FSM_PROFILE_SECTION_IN ("user_mac_pro [downlink enter execs]", state3_enter_exec)
				{
				op_pk_nfd_get(mypacket,"frame_type",&frame_type);
				if(frame_type==1){ 
					op_pk_send (mypacket,0);
					printf("user%d mac receive data packet from source and send to ap,time = %f\n",my_address,op_sim_time());
				}
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** state (downlink) exit executives **/
			FSM_STATE_EXIT_FORCED (3, "downlink", "user_mac_pro [downlink exit execs]")


			/** state (downlink) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "downlink", "get_packet", "tr_5", "user_mac_pro [downlink -> get_packet : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"user_mac_pro")
		}
	}




void
_op_user_mac_pro_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_user_mac_pro_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_user_mac_pro_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_user_mac_pro_svar function. */
#undef mypacket
#undef frame_type
#undef from_source
#undef from_receiver
#undef receive_address
#undef user_mac_id
#undef user_node_id
#undef ii
#undef name
#undef my_address

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_user_mac_pro_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_user_mac_pro_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (user_mac_pro)",
		sizeof (user_mac_pro_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_user_mac_pro_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	user_mac_pro_state * ptr;
	FIN_MT (_op_user_mac_pro_alloc (obtype))

	ptr = (user_mac_pro_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "user_mac_pro [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_user_mac_pro_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	user_mac_pro_state		*prs_ptr;

	FIN_MT (_op_user_mac_pro_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (user_mac_pro_state *)gen_ptr;

	if (strcmp ("mypacket" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->mypacket);
		FOUT
		}
	if (strcmp ("frame_type" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->frame_type);
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
	if (strcmp ("receive_address" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->receive_address);
		FOUT
		}
	if (strcmp ("user_mac_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->user_mac_id);
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
	if (strcmp ("my_address" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->my_address);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}


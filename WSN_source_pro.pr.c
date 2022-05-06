/* Process model C form file: WSN_source_pro.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char WSN_source_pro_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A modeler 7 62738F1A 62738F1A 1 DESKTOP-RD4S7T2 51133 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                    ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

#define		Poisson 			g_data_mode==1
#define		Discrete 			g_data_mode==2
#define		Concentrat			g_data_mode!=1&&g_data_mode!=2


#define		DATA_LEN			10
#define		DATA_PERIOD_P		10*MINUTE
#define		DATA_PERIOD_D		10*MINUTE
#define		DATA_PERIOD_C		1*HOUR
#define		DATA_PERIOD_down	10*MINUTE

#define		SECOND				1
#define		MINUTE				60*SECOND
#define		HOUR				60*MINUTE


#define		INTRPT_PORT			1
#define		GATE_ADRESS			43690
#define		NODENUMBER			200

#define INTRPT_SELF_INIT				((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == intrCode_Init))
#define INTRPT_SELF_SEND				((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == INTRPT_PORT))
#define intrCode_Init				10

int	DATA_TYPE_BURST	= 4;

extern int GateAddress;

static void generate_pk(int delay);
static void nwk_delay_start();

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
	/* State Variables */
	int	                    		TIME_OUT                                        ;
	double	                 		time_out                                        ;
	double	                 		BURST_PROBABILITY                               ;
	int	                    		nodeType                                        ;
	int	                    		node_status                                     ;
	int	                    		nodeID                                          ;
	int	                    		myMACAddress                                    ;
	int	                    		g_data_mode                                     ;
	} WSN_source_pro_state;

#define TIME_OUT                		op_sv_ptr->TIME_OUT
#define time_out                		op_sv_ptr->time_out
#define BURST_PROBABILITY       		op_sv_ptr->BURST_PROBABILITY
#define nodeType                		op_sv_ptr->nodeType
#define node_status             		op_sv_ptr->node_status
#define nodeID                  		op_sv_ptr->nodeID
#define myMACAddress            		op_sv_ptr->myMACAddress
#define g_data_mode             		op_sv_ptr->g_data_mode

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	WSN_source_pro_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((WSN_source_pro_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif

static void generate_pk(int delay)
{
	Packet* packet;
	int r;
	int len;
	int type;
	int dest;
	int pklen;
	int status;
	OpT_Packet_Id pk_id;
	FIN(generate_pk(int delay));
	op_ima_obj_attr_get(nodeID,"g_node_status",&status);
	if(status == 0) FOUT;
	r = (int)(op_dist_uniform (DATA_LEN) + 1);
	len = 10;
	pklen = len*8 + 32;
	if(nodeType == 0)
	{
		type = 8;
		dest = (int) op_dist_uniform (NODENUMBER - 1) + 1;
		//dest = (int) (op_dist_uniform (50) + 1);
		//dest = 80;
		//FOUT;
	}
	else
	{	
		type = 7;
		dest = GATE_ADRESS;
		//FOUT;
	}
	packet = op_pk_create(0);
	op_pk_fd_set(packet,0,OPC_FIELD_TYPE_INTEGER,10,8);
	op_pk_fd_set(packet,1,OPC_FIELD_TYPE_INTEGER,type,8);
	op_pk_fd_set(packet,2,OPC_FIELD_TYPE_INTEGER,dest,16);
	op_pk_fd_set(packet,3,OPC_FIELD_TYPE_INTEGER,r,len*8);
	pk_id = op_pk_id(packet);
	op_pk_send_delayed(packet,0, 0);
	printf("Node %d source make data packet data = %d len = %d, dest = %d,time=%f, id = %d\n",myMACAddress, r, len, dest, op_sim_time(), pk_id);
	
	FOUT;
	
}

static void delay_start()
{
	FIN(delay_start());
	op_intrpt_schedule_self(op_sim_time() + op_dist_uniform(60*10),intrCode_Init);
	//op_intrpt_schedule_self(op_sim_time(),intrCode_Init);
	FOUT;
}

static void send_intrpt()
{
	FIN(send_intrpt());
	op_intrpt_schedule_self(op_sim_time(),INTRPT_PORT);
	FOUT;
}


/* End of Function Block */

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
	void WSN_source_pro (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_WSN_source_pro_init (int * init_block_ptr);
	void _op_WSN_source_pro_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_WSN_source_pro_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_WSN_source_pro_alloc (VosT_Obtype, int);
	void _op_WSN_source_pro_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
WSN_source_pro (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (WSN_source_pro ());

		{


		FSM_ENTER ("WSN_source_pro")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "init", "WSN_source_pro [init enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_source_pro [init enter execs]", state0_enter_exec)
				{
				nodeID = op_topo_parent(op_id_self());
				//printf("Data nodeID = %d\n",nodeID);
				op_ima_obj_attr_get(nodeID,"g_node_type",&nodeType);
				op_ima_obj_attr_get(nodeID,"g_node_ESN_address",&myMACAddress);
				op_ima_obj_attr_get(nodeID,"g_node_data_mode",&g_data_mode);
				
				if(nodeType == 0)	op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD_down, INTRPT_PORT);
				//else op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD + op_dist_uniform (300), INTRPT_PORT);
				//printf("Node %d source start!\n", myMACAddress);
				
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** state (init) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "init", "WSN_source_pro [init exit execs]")


			/** state (init) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_source_pro [init trans conditions]", state0_trans_conds)
			FSM_INIT_COND (Discrete)
			FSM_TEST_COND (Poisson)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("init")
			FSM_PROFILE_SECTION_OUT (state0_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 5, state5_enter_exec, delay_start();, "Discrete", "delay_start()", "init", "delay_discrete", "tr_26", "WSN_source_pro [init -> delay_discrete : Discrete / delay_start()]")
				FSM_CASE_TRANSIT (1, 1, state1_enter_exec, send_intrpt();, "Poisson", "send_intrpt()", "init", "make_poison", "tr_25", "WSN_source_pro [init -> make_poison : Poisson / send_intrpt()]")
				FSM_CASE_TRANSIT (2, 6, state6_enter_exec, send_intrpt();, "default", "send_intrpt()", "init", "make_concentrat", "tr_27", "WSN_source_pro [init -> make_concentrat : default / send_intrpt()]")
				}
				/*---------------------------------------------------------*/



			/** state (make_poison) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "make_poison", state1_enter_exec, "WSN_source_pro [make_poison enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"WSN_source_pro")


			/** state (make_poison) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "make_poison", "WSN_source_pro [make_poison exit execs]")


			/** state (make_poison) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_source_pro [make_poison trans conditions]", state1_trans_conds)
			FSM_INIT_COND (INTRPT_SELF_SEND)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("make_poison")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 2, state2_enter_exec, ;, "INTRPT_SELF_SEND", "", "make_poison", "data_poison", "tr_3", "WSN_source_pro [make_poison -> data_poison : INTRPT_SELF_SEND / ]")
				FSM_CASE_TRANSIT (1, 1, state1_enter_exec, ;, "default", "", "make_poison", "make_poison", "tr_1", "WSN_source_pro [make_poison -> make_poison : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (data_poison) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "data_poison", state2_enter_exec, "WSN_source_pro [data_poison enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_source_pro [data_poison enter execs]", state2_enter_exec)
				{
				if(op_dist_uniform(1) < 0.1) generate_pk(0);
				
				
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (data_poison) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "data_poison", "WSN_source_pro [data_poison exit execs]")
				FSM_PROFILE_SECTION_IN ("WSN_source_pro [data_poison exit execs]", state2_exit_exec)
				{
				//op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD + op_dist_uniform (60),INTRPT_PORT);
				
				
				if(nodeType == 0)	op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD_down, INTRPT_PORT);
				else op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD_P, INTRPT_PORT); //泊松分布或离散均匀
				}
				FSM_PROFILE_SECTION_OUT (state2_exit_exec)


			/** state (data_poison) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "data_poison", "make_poison", "tr_4", "WSN_source_pro [data_poison -> make_poison : default / ]")
				/*---------------------------------------------------------*/



			/** state (make_discrete) enter executives **/
			FSM_STATE_ENTER_UNFORCED (3, "make_discrete", state3_enter_exec, "WSN_source_pro [make_discrete enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (7,"WSN_source_pro")


			/** state (make_discrete) exit executives **/
			FSM_STATE_EXIT_UNFORCED (3, "make_discrete", "WSN_source_pro [make_discrete exit execs]")


			/** state (make_discrete) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_source_pro [make_discrete trans conditions]", state3_trans_conds)
			FSM_INIT_COND (INTRPT_SELF_SEND)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("make_discrete")
			FSM_PROFILE_SECTION_OUT (state3_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 4, state4_enter_exec, ;, "INTRPT_SELF_SEND", "", "make_discrete", "data_discrete", "tr_8", "WSN_source_pro [make_discrete -> data_discrete : INTRPT_SELF_SEND / ]")
				FSM_CASE_TRANSIT (1, 3, state3_enter_exec, ;, "default", "", "make_discrete", "make_discrete", "tr_2", "WSN_source_pro [make_discrete -> make_discrete : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (data_discrete) enter executives **/
			FSM_STATE_ENTER_FORCED (4, "data_discrete", state4_enter_exec, "WSN_source_pro [data_discrete enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_source_pro [data_discrete enter execs]", state4_enter_exec)
				{
				//printf("data send a data to NWK!\n");
				generate_pk(0);
				
				
				
				}
				FSM_PROFILE_SECTION_OUT (state4_enter_exec)

			/** state (data_discrete) exit executives **/
			FSM_STATE_EXIT_FORCED (4, "data_discrete", "WSN_source_pro [data_discrete exit execs]")
				FSM_PROFILE_SECTION_IN ("WSN_source_pro [data_discrete exit execs]", state4_exit_exec)
				{
				//op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD + op_dist_uniform (60),INTRPT_PORT);
				
				
				if(nodeType == 0)	op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD_down, INTRPT_PORT);
				else op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD_D, INTRPT_PORT); //泊松分布或离散均匀
				}
				FSM_PROFILE_SECTION_OUT (state4_exit_exec)


			/** state (data_discrete) transition processing **/
			FSM_TRANSIT_FORCE (3, state3_enter_exec, ;, "default", "", "data_discrete", "make_discrete", "tr_9", "WSN_source_pro [data_discrete -> make_discrete : default / ]")
				/*---------------------------------------------------------*/



			/** state (delay_discrete) enter executives **/
			FSM_STATE_ENTER_UNFORCED (5, "delay_discrete", state5_enter_exec, "WSN_source_pro [delay_discrete enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (11,"WSN_source_pro")


			/** state (delay_discrete) exit executives **/
			FSM_STATE_EXIT_UNFORCED (5, "delay_discrete", "WSN_source_pro [delay_discrete exit execs]")


			/** state (delay_discrete) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_source_pro [delay_discrete trans conditions]", state5_trans_conds)
			FSM_INIT_COND (INTRPT_SELF_INIT)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("delay_discrete")
			FSM_PROFILE_SECTION_OUT (state5_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 3, state3_enter_exec, send_intrpt();, "INTRPT_SELF_INIT", "send_intrpt()", "delay_discrete", "make_discrete", "tr_10", "WSN_source_pro [delay_discrete -> make_discrete : INTRPT_SELF_INIT / send_intrpt()]")
				FSM_CASE_TRANSIT (1, 5, state5_enter_exec, ;, "default", "", "delay_discrete", "delay_discrete", "tr_11", "WSN_source_pro [delay_discrete -> delay_discrete : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (make_concentrat) enter executives **/
			FSM_STATE_ENTER_UNFORCED (6, "make_concentrat", state6_enter_exec, "WSN_source_pro [make_concentrat enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (13,"WSN_source_pro")


			/** state (make_concentrat) exit executives **/
			FSM_STATE_EXIT_UNFORCED (6, "make_concentrat", "WSN_source_pro [make_concentrat exit execs]")


			/** state (make_concentrat) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_source_pro [make_concentrat trans conditions]", state6_trans_conds)
			FSM_INIT_COND (INTRPT_SELF_SEND)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("make_concentrat")
			FSM_PROFILE_SECTION_OUT (state6_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 7, state7_enter_exec, ;, "INTRPT_SELF_SEND", "", "make_concentrat", "data_concentrat", "tr_14", "WSN_source_pro [make_concentrat -> data_concentrat : INTRPT_SELF_SEND / ]")
				FSM_CASE_TRANSIT (1, 6, state6_enter_exec, ;, "default", "", "make_concentrat", "make_concentrat", "tr_12", "WSN_source_pro [make_concentrat -> make_concentrat : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (data_concentrat) enter executives **/
			FSM_STATE_ENTER_FORCED (7, "data_concentrat", state7_enter_exec, "WSN_source_pro [data_concentrat enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_source_pro [data_concentrat enter execs]", state7_enter_exec)
				{
				//printf("data send a data to NWK!\n");
				generate_pk(op_dist_uniform(60));
				
				
				}
				FSM_PROFILE_SECTION_OUT (state7_enter_exec)

			/** state (data_concentrat) exit executives **/
			FSM_STATE_EXIT_FORCED (7, "data_concentrat", "WSN_source_pro [data_concentrat exit execs]")
				FSM_PROFILE_SECTION_IN ("WSN_source_pro [data_concentrat exit execs]", state7_exit_exec)
				{
				//op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD + op_dist_uniform (60),INTRPT_PORT);
				
				if(nodeType == 0)	op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD_down, INTRPT_PORT);
				else op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD_C + op_dist_uniform (60), INTRPT_PORT);
				}
				FSM_PROFILE_SECTION_OUT (state7_exit_exec)


			/** state (data_concentrat) transition processing **/
			FSM_TRANSIT_FORCE (6, state6_enter_exec, ;, "default", "", "data_concentrat", "make_concentrat", "tr_15", "WSN_source_pro [data_concentrat -> make_concentrat : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"WSN_source_pro")
		}
	}




void
_op_WSN_source_pro_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_WSN_source_pro_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_WSN_source_pro_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_WSN_source_pro_svar function. */
#undef TIME_OUT
#undef time_out
#undef BURST_PROBABILITY
#undef nodeType
#undef node_status
#undef nodeID
#undef myMACAddress
#undef g_data_mode

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_WSN_source_pro_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_WSN_source_pro_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (WSN_source_pro)",
		sizeof (WSN_source_pro_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_WSN_source_pro_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	WSN_source_pro_state * ptr;
	FIN_MT (_op_WSN_source_pro_alloc (obtype))

	ptr = (WSN_source_pro_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "WSN_source_pro [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_WSN_source_pro_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	WSN_source_pro_state		*prs_ptr;

	FIN_MT (_op_WSN_source_pro_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (WSN_source_pro_state *)gen_ptr;

	if (strcmp ("TIME_OUT" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->TIME_OUT);
		FOUT
		}
	if (strcmp ("time_out" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->time_out);
		FOUT
		}
	if (strcmp ("BURST_PROBABILITY" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->BURST_PROBABILITY);
		FOUT
		}
	if (strcmp ("nodeType" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->nodeType);
		FOUT
		}
	if (strcmp ("node_status" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->node_status);
		FOUT
		}
	if (strcmp ("nodeID" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->nodeID);
		FOUT
		}
	if (strcmp ("myMACAddress" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->myMACAddress);
		FOUT
		}
	if (strcmp ("g_data_mode" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_data_mode);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}


/* Process model C form file: WSN_Control.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char WSN_Control_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A op_runsim 7 621F5F8B 621F5F8B 1 DESKTOP-RD4S7T2 51133 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                  ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */


#define 	REMOTE_PRO	(op_intrpt_type() == OPC_INTRPT_REMOTE)
#define 	intrCode_Father		4
#define		intrCode_MainRouter 8

#define		MAX_PROBABILITY		3000
#define		REJOIN_PROBABILITY	2
#define		LEAVE_PROBABILITY	4

#define		JOINSEQ_TYPE		0
#define		LEAVE_TYPE			2
#define		MULTICAST_TYPE		5

#define		JOINSEQ_LEN			56
#define		LEAVE_LEN			8
#define		MULTICASTCONTR_LEN	40


#define		JOIN				1
#define		REJOIN				2
#define		LEAVE				3


extern int GateAddress;

static void generate_pk(Packet* payLoad,int len,int type,int dest,double delay);
static void JoinReq();
static void multicastControl(int type,int dest);
static void leaveControl(int dest);

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
	int	                    		nodeType                                        ;
	int	                    		myCapabilityInformation                         ;
	int	                    		myMACAddress                                    ;
	int	                    		father                                          ;
	int	                    		node_status                                     ;
	int	                    		nodeID                                          ;
	Stathandle	             		contr_send_num                                  ;
	int	                    		sendNum                                         ;
	} WSN_Control_state;

#define nodeType                		op_sv_ptr->nodeType
#define myCapabilityInformation 		op_sv_ptr->myCapabilityInformation
#define myMACAddress            		op_sv_ptr->myMACAddress
#define father                  		op_sv_ptr->father
#define node_status             		op_sv_ptr->node_status
#define nodeID                  		op_sv_ptr->nodeID
#define contr_send_num          		op_sv_ptr->contr_send_num
#define sendNum                 		op_sv_ptr->sendNum

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	WSN_Control_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((WSN_Control_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif

static void generate_pk(Packet* payLoad,int len,int type,int dest,double delay)
{
	Packet* packet;
	int pklen = len + 32;
	FIN(generate_pk(Packet* payLoad,int len,int type,int dest,double delay));
	packet = op_pk_create(0);
	op_pk_fd_set(packet,0,OPC_FIELD_TYPE_INTEGER,len,8);
	op_pk_fd_set(packet,1,OPC_FIELD_TYPE_INTEGER,type,8);
	op_pk_fd_set(packet,2,OPC_FIELD_TYPE_INTEGER,dest,16);
	op_pk_fd_set(packet,3,OPC_FIELD_TYPE_PACKET,payLoad,len);
	op_pk_send_delayed(packet,0,delay);
	//op_stat_write(contr_send_num,++sendNum);
	printf("Control send a PK\n");
	//printf("user%d source make control packet,time=%f\n",my_address,op_sim_time());
	FOUT;
	
}

static void JoinReq()
{
	Packet* payLoad;
	double delay;
	FIN(JoinReq());
	delay = op_dist_uniform (2.0);
	op_ima_obj_attr_get(nodeID,"NodeStatus",&node_status);
	if(node_status == 0 || node_status == 2 || node_status == 4)
	{
		op_ima_obj_attr_get(nodeID,"father",&father);
		payLoad = op_pk_create(0);
		op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,myMACAddress,48);
		op_pk_fd_set(payLoad,1,OPC_FIELD_TYPE_INTEGER,myCapabilityInformation,8);
		generate_pk(payLoad,JOINSEQ_LEN,JOINSEQ_TYPE,father,delay);
		printf("Node %d build a joinreq\n",myMACAddress);
	}
	FOUT;
}

static void multicastControl(int type,int dest)
{
	Packet* payLoad;
	int multicastInfo;
	FIN(multicastControl(int type,int dest));
	if(nodeType == 0)
	{
		multicastInfo = 4|type;
		payLoad = op_pk_create(0);
		op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,multicastInfo,8);
		op_pk_fd_set(payLoad,1,OPC_FIELD_TYPE_INTEGER,1,16);
		op_pk_fd_set(payLoad,2,OPC_FIELD_TYPE_INTEGER,2,16);
		generate_pk(payLoad,MULTICASTCONTR_LEN,MULTICAST_TYPE,dest,0);
		printf("&&&&&\n");
	}
	FOUT;
}

static void leaveControl(int dest)
{
	Packet* payLoad;
	int leaveInfo;
	FIN(leaveControl(int dest));
	if(nodeType == 0)
	{
		leaveInfo = 26;
		payLoad = op_pk_create(0);
		op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,leaveInfo,LEAVE_LEN);
		generate_pk(payLoad,LEAVE_LEN,LEAVE_TYPE,dest,0);
	}
	/*
	else
	{
		leaveInfo = 0;
		payLoad = op_pk_create(0);
		op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,leaveInfo,LEAVE_LEN);
		generate_pk(payLoad,LEAVE_LEN,LEAVE_TYPE,dest,0);
	}
	*/
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
	void WSN_Control (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_WSN_Control_init (int * init_block_ptr);
	void _op_WSN_Control_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_WSN_Control_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_WSN_Control_alloc (VosT_Obtype, int);
	void _op_WSN_Control_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
WSN_Control (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (WSN_Control ());

		{


		FSM_ENTER ("WSN_Control")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "init", "WSN_Control [init enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_Control [init enter execs]", state0_enter_exec)
				{
				nodeID = op_topo_parent(op_id_self());
				op_ima_obj_attr_get(nodeID,"g_node_type",&nodeType);
				op_ima_obj_attr_get(nodeID,"g_node_ESN_address",&myMACAddress);
				
				if(nodeType != 0)
				{
					//op_intrpt_schedule_self(op_sim_time()+op_dist_uniform (20.0)+op_dist_uniform (40.0),0);
				}
				else
				{	
					printf("=================");
					//op_intrpt_schedule_self(op_sim_time()+1600,1);
					//op_intrpt_schedule_self(op_sim_time()+2000,2);
					
					//op_intrpt_schedule_self(op_sim_time()+3000,3);
					//op_intrpt_schedule_self(op_sim_time()+1600,4);
				}
				
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** state (init) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "init", "WSN_Control [init exit execs]")


			/** state (init) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "init", "idle", "tr_0", "WSN_Control [init -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "idle", state1_enter_exec, "WSN_Control [idle enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"WSN_Control")


			/** state (idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "idle", "WSN_Control [idle exit execs]")


			/** state (idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_Control [idle trans conditions]", state1_trans_conds)
			FSM_INIT_COND (op_intrpt_type()==OPC_INTRPT_SELF && op_intrpt_code()==0)
			FSM_TEST_COND (op_intrpt_type()==OPC_INTRPT_SELF && op_intrpt_code()==1)
			FSM_TEST_COND (op_intrpt_type()==OPC_INTRPT_SELF && op_intrpt_code()==2)
			FSM_TEST_COND (REMOTE_PRO)
			FSM_TEST_COND (op_intrpt_type()==OPC_INTRPT_SELF && op_intrpt_code()==4)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("idle")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 2, state2_enter_exec, ;, "op_intrpt_type()==OPC_INTRPT_SELF && op_intrpt_code()==0", "", "idle", "Joinreq", "tr_5", "WSN_Control [idle -> Joinreq : op_intrpt_type()==OPC_INTRPT_SELF && op_intrpt_code()==0 / ]")
				FSM_CASE_TRANSIT (1, 3, state3_enter_exec, ;, "op_intrpt_type()==OPC_INTRPT_SELF && op_intrpt_code()==1", "", "idle", "MulticastContr_add", "tr_6", "WSN_Control [idle -> MulticastContr_add : op_intrpt_type()==OPC_INTRPT_SELF && op_intrpt_code()==1 / ]")
				FSM_CASE_TRANSIT (2, 4, state4_enter_exec, ;, "op_intrpt_type()==OPC_INTRPT_SELF && op_intrpt_code()==2", "", "idle", "MulticastContr_subtract", "tr_10", "WSN_Control [idle -> MulticastContr_subtract : op_intrpt_type()==OPC_INTRPT_SELF && op_intrpt_code()==2 / ]")
				FSM_CASE_TRANSIT (3, 5, state5_enter_exec, ;, "REMOTE_PRO", "", "idle", "RemotePro", "tr_13", "WSN_Control [idle -> RemotePro : REMOTE_PRO / ]")
				FSM_CASE_TRANSIT (4, 6, state6_enter_exec, ;, "op_intrpt_type()==OPC_INTRPT_SELF && op_intrpt_code()==4", "", "idle", "Leave", "tr_16", "WSN_Control [idle -> Leave : op_intrpt_type()==OPC_INTRPT_SELF && op_intrpt_code()==4 / ]")
				FSM_CASE_TRANSIT (5, 1, state1_enter_exec, ;, "default", "", "idle", "idle", "tr_1", "WSN_Control [idle -> idle : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (Joinreq) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "Joinreq", state2_enter_exec, "WSN_Control [Joinreq enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_Control [Joinreq enter execs]", state2_enter_exec)
				{
				op_ima_obj_attr_get(nodeID,"NodeStatus",&node_status);
				if(node_status==0)
				{
					JoinReq();
					op_intrpt_schedule_self(op_sim_time()+20.0,0);
				}
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (Joinreq) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "Joinreq", "WSN_Control [Joinreq exit execs]")


			/** state (Joinreq) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "Joinreq", "idle", "tr_7", "WSN_Control [Joinreq -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (MulticastContr_add) enter executives **/
			FSM_STATE_ENTER_FORCED (3, "MulticastContr_add", state3_enter_exec, "WSN_Control [MulticastContr_add enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_Control [MulticastContr_add enter execs]", state3_enter_exec)
				{
				printf("Node %d send a mutilcastContr\n",myMACAddress);
				multicastControl(1,512);
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** state (MulticastContr_add) exit executives **/
			FSM_STATE_EXIT_FORCED (3, "MulticastContr_add", "WSN_Control [MulticastContr_add exit execs]")


			/** state (MulticastContr_add) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "MulticastContr_add", "idle", "tr_9", "WSN_Control [MulticastContr_add -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (MulticastContr_subtract) enter executives **/
			FSM_STATE_ENTER_FORCED (4, "MulticastContr_subtract", state4_enter_exec, "WSN_Control [MulticastContr_subtract enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_Control [MulticastContr_subtract enter execs]", state4_enter_exec)
				{
				printf("Node %d send a mutilcastContr\n",myMACAddress);
				multicastControl(0,512);
				}
				FSM_PROFILE_SECTION_OUT (state4_enter_exec)

			/** state (MulticastContr_subtract) exit executives **/
			FSM_STATE_EXIT_FORCED (4, "MulticastContr_subtract", "WSN_Control [MulticastContr_subtract exit execs]")


			/** state (MulticastContr_subtract) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "MulticastContr_subtract", "idle", "tr_11", "WSN_Control [MulticastContr_subtract -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (RemotePro) enter executives **/
			FSM_STATE_ENTER_FORCED (5, "RemotePro", state5_enter_exec, "WSN_Control [RemotePro enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_Control [RemotePro enter execs]", state5_enter_exec)
				{
				int intrpt_code = op_intrpt_code ();
				printf("Node %d control receive a remote\n",myMACAddress);
				switch(intrpt_code)
				{
					case intrCode_Father : JoinReq();
							break;
					case intrCode_MainRouter :	
							//myCapabilityInformation &= ~(3<<6);
							myCapabilityInformation |= (nodeType<<6);
							JoinReq();
							break;
					default:printf("Control error intrpt_code for type of remote!\n");
				}
				}
				FSM_PROFILE_SECTION_OUT (state5_enter_exec)

			/** state (RemotePro) exit executives **/
			FSM_STATE_EXIT_FORCED (5, "RemotePro", "WSN_Control [RemotePro exit execs]")


			/** state (RemotePro) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "RemotePro", "idle", "tr_12", "WSN_Control [RemotePro -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (Leave) enter executives **/
			FSM_STATE_ENTER_FORCED (6, "Leave", state6_enter_exec, "WSN_Control [Leave enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_Control [Leave enter execs]", state6_enter_exec)
				{
				printf("send a leave control!\n");
				leaveControl(2);
				}
				FSM_PROFILE_SECTION_OUT (state6_enter_exec)

			/** state (Leave) exit executives **/
			FSM_STATE_EXIT_FORCED (6, "Leave", "WSN_Control [Leave exit execs]")


			/** state (Leave) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "Leave", "idle", "tr_17", "WSN_Control [Leave -> idle : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"WSN_Control")
		}
	}




void
_op_WSN_Control_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_WSN_Control_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_WSN_Control_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_WSN_Control_svar function. */
#undef nodeType
#undef myCapabilityInformation
#undef myMACAddress
#undef father
#undef node_status
#undef nodeID
#undef contr_send_num
#undef sendNum

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_WSN_Control_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_WSN_Control_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (WSN_Control)",
		sizeof (WSN_Control_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_WSN_Control_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	WSN_Control_state * ptr;
	FIN_MT (_op_WSN_Control_alloc (obtype))

	ptr = (WSN_Control_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "WSN_Control [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_WSN_Control_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	WSN_Control_state		*prs_ptr;

	FIN_MT (_op_WSN_Control_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (WSN_Control_state *)gen_ptr;

	if (strcmp ("nodeType" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->nodeType);
		FOUT
		}
	if (strcmp ("myCapabilityInformation" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->myCapabilityInformation);
		FOUT
		}
	if (strcmp ("myMACAddress" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->myMACAddress);
		FOUT
		}
	if (strcmp ("father" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->father);
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
	if (strcmp ("contr_send_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->contr_send_num);
		FOUT
		}
	if (strcmp ("sendNum" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->sendNum);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}


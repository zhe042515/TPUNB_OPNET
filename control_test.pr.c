/* Process model C form file: control_test.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char control_test_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A op_runsim 7 623C2AD6 623C2AD6 1 DESKTOP-RD4S7T2 51133 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                  ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

#define MUTILCAST 			3
#define LEAVE 				1
#define REJOIN		 		2



#define Intr_Mutilcast_add			((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == addCode))
#define Intr_Data					((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == dataCode))
#define Intr_Mutilcast_wait			((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == startCode))
#define Intr_Mutilcast_remove			((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == removeCode))
#define Intr_RejoinStart			((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == startCode))
#define Intr_Leave_Start			((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == startCode))
#define Intr_Leave_Send				((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == leaveCode))



#define ROUTERNODE			1
#define GATENODE			0
#define ENDNODE				2
#define BACKUPNODE			3




#define		JOINSEQ_TYPE		0
#define		LEAVE_TYPE			2
#define		MULTICAST_TYPE		5

#define		LEAVE_LEN			8
#define		MULTICASTCONTR_LEN	8
#define		DATA_LEN			10




#define  	dataCode		0
#define		startCode		1
#define		removeCode		2
#define 	addCode			3
#define		leaveCode		4

#define		data_prid		60 * 60

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
	int	                    		contr_send_num                                  ;
	int	                    		sendNum                                         ;
	int	                    		result                                          ;
	int	                    		g_test_type                                     ;
	int	                    		dest                                            ;
	int	                    		status                                          ;
	} control_test_state;

#define nodeType                		op_sv_ptr->nodeType
#define myCapabilityInformation 		op_sv_ptr->myCapabilityInformation
#define myMACAddress            		op_sv_ptr->myMACAddress
#define father                  		op_sv_ptr->father
#define node_status             		op_sv_ptr->node_status
#define nodeID                  		op_sv_ptr->nodeID
#define contr_send_num          		op_sv_ptr->contr_send_num
#define sendNum                 		op_sv_ptr->sendNum
#define result                  		op_sv_ptr->result
#define g_test_type             		op_sv_ptr->g_test_type
#define dest                    		op_sv_ptr->dest
#define status                  		op_sv_ptr->status

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	control_test_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((control_test_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif

static void timer(double timer, int code)
{
	FIN(timer(double timer, int code));
	op_intrpt_schedule_self(op_sim_time() + timer,code);
	FOUT;
}


static void contr_generate_pk(Packet* payLoad,int len,int type,int destAddress,double delay)
{
	Packet* packet;
	int pklen = len + 32;
	FIN(generate_pk(Packet* payLoad,int len,int type,int destAddress,double delay));
	packet = op_pk_create(0);
	op_pk_fd_set(packet,0,OPC_FIELD_TYPE_INTEGER,len,8);
	op_pk_fd_set(packet,1,OPC_FIELD_TYPE_INTEGER,type,8);
	op_pk_fd_set(packet,2,OPC_FIELD_TYPE_INTEGER,destAddress,16);
	op_pk_fd_set(packet,3,OPC_FIELD_TYPE_PACKET,payLoad,len);
	op_pk_send_delayed(packet,0,delay);
	//op_stat_write(contr_send_num,++sendNum);
	printf("Control send a PK\n");
	//printf("user%d source make control packet,time=%f\n",my_address,op_sim_time());
	FOUT;
	
}
/*
*	发送组播添加命令或组播删除命令
*	type = 0表示添加,type = 1表示删除
*	dest为目的节点，num表示需要添加的组播组数量，arr表示需要添加的组播组数组
*/

static void contr_multicast_send(int type,int destAddress,int num, int arr[])
{
	Packet* payLoad;
	int multicastInfo;
	int i = 0;
	FIN(multicastControl(int type,int destAddress));
	if(nodeType == 0)
	{
		multicastInfo = (num<<1)|type;
		payLoad = op_pk_create(0);
		op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,multicastInfo,8);
		for(i = 0;i < num;i++)
			{
			op_pk_fd_set(payLoad,i+1,OPC_FIELD_TYPE_INTEGER,arr[i],16);
			}
		contr_generate_pk(payLoad,MULTICASTCONTR_LEN + (16*num),MULTICAST_TYPE,destAddress,0);
	}
	FOUT;
}

static void generate_mutilcast_data_pk(int mutilcast_id)
{
	Packet* payLoad;
	int r;
	int len;
	int type;
	int destAddress;
	int pklen;
	FIN(generate_mutilcast_data_pk(int mutilcast_id));
	op_ima_obj_attr_get(nodeID,"g_node_status",&status);
	if(status == 0) FOUT;
	r = (int)(op_dist_uniform (DATA_LEN) + 1);
	len = 10;
	pklen = len*8 + 32;
	if(nodeType == 0)
	{
		type = 8;
		destAddress = mutilcast_id;
	}
	else
	{	
		FOUT;
	}
	payLoad = op_pk_create(0);
	//op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,10,8);
	//op_pk_fd_set(payLoad,1,OPC_FIELD_TYPE_INTEGER,type,8);
	//op_pk_fd_set(payLoad,2,OPC_FIELD_TYPE_INTEGER,destAddress,16);
	op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,r,len*8);
	contr_generate_pk(payLoad,len,8,destAddress,0);
	printf("Node %d source make mutilcast data packet data = %d len = %d, dest = %d,time=%f\n",myMACAddress, r, len, destAddress, op_sim_time());
	
	FOUT;
	
}


/*
*destAddress目的地址, rejoin:0不重新入网 1选择新的父节点重新入网 2通过当前父节点重新入网 replace:只有rejoin字段不为0时才有效 0表示保留旧资源 1 重新分配资源
*
*/
static void leaveControl(int destAddress, int rejoin, int replace)
{
	Packet* payLoad;
	int leaveInfo;
	FIN(leaveControl(int destAddress));
	if(nodeType == 0)
	{
		leaveInfo = 2 | rejoin<<2 | replace << 4;
		payLoad = op_pk_create(0);
		op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,leaveInfo,LEAVE_LEN);
		contr_generate_pk(payLoad,LEAVE_LEN,LEAVE_TYPE,destAddress,0);
	}
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
	void control_test (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_control_test_init (int * init_block_ptr);
	void _op_control_test_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_control_test_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_control_test_alloc (VosT_Obtype, int);
	void _op_control_test_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
control_test (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (control_test ());

		{


		FSM_ENTER ("control_test")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "init", "control_test [init enter execs]")
				FSM_PROFILE_SECTION_IN ("control_test [init enter execs]", state0_enter_exec)
				{
				nodeID = op_topo_parent(op_id_self());
				op_ima_obj_attr_get(nodeID,"g_node_type",&nodeType);
				op_ima_obj_attr_get(nodeID,"g_node_ESN_address",&myMACAddress);
				op_ima_obj_attr_get(nodeID,"g_test_type",&g_test_type);
				//printf("control init\n");
				
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** state (init) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "init", "control_test [init exit execs]")


			/** state (init) transition processing **/
			FSM_PROFILE_SECTION_IN ("control_test [init trans conditions]", state0_trans_conds)
			FSM_INIT_COND (g_test_type == MUTILCAST)
			FSM_TEST_COND (g_test_type == LEAVE)
			FSM_TEST_COND (g_test_type == REJOIN)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("init")
			FSM_PROFILE_SECTION_OUT (state0_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 3, state3_enter_exec, timer(1800,startCode);;, "g_test_type == MUTILCAST", "timer(1800,startCode);", "init", "mutilcast_init", "tr_3", "control_test [init -> mutilcast_init : g_test_type == MUTILCAST / timer(1800,startCode);]")
				FSM_CASE_TRANSIT (1, 4, state4_enter_exec, timer(1800,startCode);;, "g_test_type == LEAVE", "timer(1800,startCode);", "init", "leave_init", "tr_5", "control_test [init -> leave_init : g_test_type == LEAVE / timer(1800,startCode);]")
				FSM_CASE_TRANSIT (2, 9, state9_enter_exec, timer(1800,startCode);;, "g_test_type == REJOIN", "timer(1800,startCode);", "init", "rejoin", "tr_15", "control_test [init -> rejoin : g_test_type == REJOIN / timer(1800,startCode);]")
				FSM_CASE_TRANSIT (3, 12, state12_enter_exec, ;, "default", "", "init", "idle", "tr_19", "control_test [init -> idle : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (mutilcast_idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "mutilcast_idle", state1_enter_exec, "control_test [mutilcast_idle enter execs]")
				FSM_PROFILE_SECTION_IN ("control_test [mutilcast_idle enter execs]", state1_enter_exec)
				{
				printf("mutilcast idle\n");
				}
				FSM_PROFILE_SECTION_OUT (state1_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"control_test")


			/** state (mutilcast_idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "mutilcast_idle", "control_test [mutilcast_idle exit execs]")


			/** state (mutilcast_idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("control_test [mutilcast_idle trans conditions]", state1_trans_conds)
			FSM_INIT_COND (Intr_Mutilcast_add)
			FSM_TEST_COND (Intr_Mutilcast_remove)
			FSM_TEST_COND (Intr_Data)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("mutilcast_idle")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 2, state2_enter_exec, timer(data_prid, dataCode);, "Intr_Mutilcast_add", "timer(data_prid, dataCode)", "mutilcast_idle", "send_muilticast_add", "tr_2", "control_test [mutilcast_idle -> send_muilticast_add : Intr_Mutilcast_add / timer(data_prid, dataCode)]")
				FSM_CASE_TRANSIT (1, 7, state7_enter_exec, ;, "Intr_Mutilcast_remove", "", "mutilcast_idle", "send_muilticast_remove", "tr_11", "control_test [mutilcast_idle -> send_muilticast_remove : Intr_Mutilcast_remove / ]")
				FSM_CASE_TRANSIT (2, 8, state8_enter_exec, ;, "Intr_Data", "", "mutilcast_idle", "send_data", "tr_13", "control_test [mutilcast_idle -> send_data : Intr_Data / ]")
				FSM_CASE_TRANSIT (3, 1, state1_enter_exec, ;, "default", "", "mutilcast_idle", "mutilcast_idle", "tr_1", "control_test [mutilcast_idle -> mutilcast_idle : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (send_muilticast_add) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "send_muilticast_add", state2_enter_exec, "control_test [send_muilticast_add enter execs]")
				FSM_PROFILE_SECTION_IN ("control_test [send_muilticast_add enter execs]", state2_enter_exec)
				{
				int arr[] = {1, 2};
				printf("send a add mutilcast!\n");
				contr_multicast_send(1,dest ,2, arr);
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (send_muilticast_add) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "send_muilticast_add", "control_test [send_muilticast_add exit execs]")


			/** state (send_muilticast_add) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "send_muilticast_add", "mutilcast_idle", "tr_10", "control_test [send_muilticast_add -> mutilcast_idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (mutilcast_init) enter executives **/
			FSM_STATE_ENTER_UNFORCED (3, "mutilcast_init", state3_enter_exec, "control_test [mutilcast_init enter execs]")
				FSM_PROFILE_SECTION_IN ("control_test [mutilcast_init enter execs]", state3_enter_exec)
				{
				//printf("in mutilcast init\n");
				if(nodeType == GATENODE) {
					result = 1;
					dest = 512;
				}
				
				else{
					result = 0;
				}
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (7,"control_test")


			/** state (mutilcast_init) exit executives **/
			FSM_STATE_EXIT_UNFORCED (3, "mutilcast_init", "control_test [mutilcast_init exit execs]")


			/** state (mutilcast_init) transition processing **/
			FSM_PROFILE_SECTION_IN ("control_test [mutilcast_init trans conditions]", state3_trans_conds)
			FSM_INIT_COND (result == 1 && Intr_Mutilcast_wait)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("mutilcast_init")
			FSM_PROFILE_SECTION_OUT (state3_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, timer(0, addCode);, "result == 1 && Intr_Mutilcast_wait", "timer(0, addCode)", "mutilcast_init", "mutilcast_idle", "tr_4", "control_test [mutilcast_init -> mutilcast_idle : result == 1 && Intr_Mutilcast_wait / timer(0, addCode)]")
				FSM_CASE_TRANSIT (1, 3, state3_enter_exec, ;, "default", "", "mutilcast_init", "mutilcast_init", "tr_22", "control_test [mutilcast_init -> mutilcast_init : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (leave_init) enter executives **/
			FSM_STATE_ENTER_UNFORCED (4, "leave_init", state4_enter_exec, "control_test [leave_init enter execs]")
				FSM_PROFILE_SECTION_IN ("control_test [leave_init enter execs]", state4_enter_exec)
				{
				if(nodeType == GATENODE) {
					result = 1;
					dest = 2;
				}
				
				else{
					result = 0;
				}
				}
				FSM_PROFILE_SECTION_OUT (state4_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (9,"control_test")


			/** state (leave_init) exit executives **/
			FSM_STATE_EXIT_UNFORCED (4, "leave_init", "control_test [leave_init exit execs]")


			/** state (leave_init) transition processing **/
			FSM_PROFILE_SECTION_IN ("control_test [leave_init trans conditions]", state4_trans_conds)
			FSM_INIT_COND (result == 1 && Intr_Leave_Start)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("leave_init")
			FSM_PROFILE_SECTION_OUT (state4_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 5, state5_enter_exec, timer(0, leaveCode);, "result == 1 && Intr_Leave_Start", "timer(0, leaveCode)", "leave_init", "leave_idle", "tr_6", "control_test [leave_init -> leave_idle : result == 1 && Intr_Leave_Start / timer(0, leaveCode)]")
				FSM_CASE_TRANSIT (1, 4, state4_enter_exec, ;, "default", "", "leave_init", "leave_init", "tr_23", "control_test [leave_init -> leave_init : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (leave_idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (5, "leave_idle", state5_enter_exec, "control_test [leave_idle enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (11,"control_test")


			/** state (leave_idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (5, "leave_idle", "control_test [leave_idle exit execs]")
				FSM_PROFILE_SECTION_IN ("control_test [leave_idle exit execs]", state5_exit_exec)
				{
				result = 0;
				}
				FSM_PROFILE_SECTION_OUT (state5_exit_exec)


			/** state (leave_idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("control_test [leave_idle trans conditions]", state5_trans_conds)
			FSM_INIT_COND (Intr_Leave_Send)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("leave_idle")
			FSM_PROFILE_SECTION_OUT (state5_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 6, state6_enter_exec, ;, "Intr_Leave_Send", "", "leave_idle", "send_leave", "tr_8", "control_test [leave_idle -> send_leave : Intr_Leave_Send / ]")
				FSM_CASE_TRANSIT (1, 5, state5_enter_exec, ;, "default", "", "leave_idle", "leave_idle", "tr_7", "control_test [leave_idle -> leave_idle : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (send_leave) enter executives **/
			FSM_STATE_ENTER_FORCED (6, "send_leave", state6_enter_exec, "control_test [send_leave enter execs]")
				FSM_PROFILE_SECTION_IN ("control_test [send_leave enter execs]", state6_enter_exec)
				{
				printf("send a leave control!\n");
				leaveControl(dest, 0, 0);
				}
				FSM_PROFILE_SECTION_OUT (state6_enter_exec)

			/** state (send_leave) exit executives **/
			FSM_STATE_EXIT_FORCED (6, "send_leave", "control_test [send_leave exit execs]")


			/** state (send_leave) transition processing **/
			FSM_TRANSIT_FORCE (5, state5_enter_exec, ;, "default", "", "send_leave", "leave_idle", "tr_9", "control_test [send_leave -> leave_idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (send_muilticast_remove) enter executives **/
			FSM_STATE_ENTER_FORCED (7, "send_muilticast_remove", state7_enter_exec, "control_test [send_muilticast_remove enter execs]")
				FSM_PROFILE_SECTION_IN ("control_test [send_muilticast_remove enter execs]", state7_enter_exec)
				{
				int arr[] = {1, 2};
				contr_multicast_send(0,dest ,2, arr);
				}
				FSM_PROFILE_SECTION_OUT (state7_enter_exec)

			/** state (send_muilticast_remove) exit executives **/
			FSM_STATE_EXIT_FORCED (7, "send_muilticast_remove", "control_test [send_muilticast_remove exit execs]")


			/** state (send_muilticast_remove) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "send_muilticast_remove", "mutilcast_idle", "tr_12", "control_test [send_muilticast_remove -> mutilcast_idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (send_data) enter executives **/
			FSM_STATE_ENTER_FORCED (8, "send_data", state8_enter_exec, "control_test [send_data enter execs]")
				FSM_PROFILE_SECTION_IN ("control_test [send_data enter execs]", state8_enter_exec)
				{
				generate_mutilcast_data_pk(2);
				}
				FSM_PROFILE_SECTION_OUT (state8_enter_exec)

			/** state (send_data) exit executives **/
			FSM_STATE_EXIT_FORCED (8, "send_data", "control_test [send_data exit execs]")
				FSM_PROFILE_SECTION_IN ("control_test [send_data exit execs]", state8_exit_exec)
				{
				op_intrpt_schedule_self(op_sim_time()+180,removeCode);
				}
				FSM_PROFILE_SECTION_OUT (state8_exit_exec)


			/** state (send_data) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "send_data", "mutilcast_idle", "tr_14", "control_test [send_data -> mutilcast_idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (rejoin) enter executives **/
			FSM_STATE_ENTER_UNFORCED (9, "rejoin", state9_enter_exec, "control_test [rejoin enter execs]")
				FSM_PROFILE_SECTION_IN ("control_test [rejoin enter execs]", state9_enter_exec)
				{
				if(nodeType == GATENODE) {
					result = 1;
					op_intrpt_schedule_self(op_sim_time()+1600,0);
				}
				
				else{
					result = 0;
				}
				}
				FSM_PROFILE_SECTION_OUT (state9_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (19,"control_test")


			/** state (rejoin) exit executives **/
			FSM_STATE_EXIT_UNFORCED (9, "rejoin", "control_test [rejoin exit execs]")


			/** state (rejoin) transition processing **/
			FSM_PROFILE_SECTION_IN ("control_test [rejoin trans conditions]", state9_trans_conds)
			FSM_INIT_COND (result == 1 && Intr_Leave_Start)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("rejoin")
			FSM_PROFILE_SECTION_OUT (state9_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 10, state10_enter_exec, ;, "result == 1 && Intr_Leave_Start", "", "rejoin", "rejoin_idle", "tr_17", "control_test [rejoin -> rejoin_idle : result == 1 && Intr_Leave_Start / ]")
				FSM_CASE_TRANSIT (1, 9, state9_enter_exec, ;, "default", "", "rejoin", "rejoin", "tr_24", "control_test [rejoin -> rejoin : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (rejoin_idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (10, "rejoin_idle", state10_enter_exec, "control_test [rejoin_idle enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (21,"control_test")


			/** state (rejoin_idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (10, "rejoin_idle", "control_test [rejoin_idle exit execs]")
				FSM_PROFILE_SECTION_IN ("control_test [rejoin_idle exit execs]", state10_exit_exec)
				{
				result = 0;
				}
				FSM_PROFILE_SECTION_OUT (state10_exit_exec)


			/** state (rejoin_idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("control_test [rejoin_idle trans conditions]", state10_trans_conds)
			FSM_INIT_COND (Intr_Leave_Send)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("rejoin_idle")
			FSM_PROFILE_SECTION_OUT (state10_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 11, state11_enter_exec, timer(0, leaveCode);, "Intr_Leave_Send", "timer(0, leaveCode)", "rejoin_idle", "send_leave_rejoin", "tr_18", "control_test [rejoin_idle -> send_leave_rejoin : Intr_Leave_Send / timer(0, leaveCode)]")
				FSM_CASE_TRANSIT (1, 10, state10_enter_exec, ;, "default", "", "rejoin_idle", "rejoin_idle", "tr_16", "control_test [rejoin_idle -> rejoin_idle : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (send_leave_rejoin) enter executives **/
			FSM_STATE_ENTER_FORCED (11, "send_leave_rejoin", state11_enter_exec, "control_test [send_leave_rejoin enter execs]")
				FSM_PROFILE_SECTION_IN ("control_test [send_leave_rejoin enter execs]", state11_enter_exec)
				{
				printf("send a rejoin control!\n");
				leaveControl(512, 1, 1);
				}
				FSM_PROFILE_SECTION_OUT (state11_enter_exec)

			/** state (send_leave_rejoin) exit executives **/
			FSM_STATE_EXIT_FORCED (11, "send_leave_rejoin", "control_test [send_leave_rejoin exit execs]")


			/** state (send_leave_rejoin) transition processing **/
			FSM_TRANSIT_FORCE (10, state10_enter_exec, ;, "default", "", "send_leave_rejoin", "rejoin_idle", "tr_21", "control_test [send_leave_rejoin -> rejoin_idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (12, "idle", state12_enter_exec, "control_test [idle enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (25,"control_test")


			/** state (idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (12, "idle", "control_test [idle exit execs]")


			/** state (idle) transition processing **/
			FSM_TRANSIT_FORCE (12, state12_enter_exec, ;, "default", "", "idle", "idle", "tr_20", "control_test [idle -> idle : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"control_test")
		}
	}




void
_op_control_test_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_control_test_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_control_test_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_control_test_svar function. */
#undef nodeType
#undef myCapabilityInformation
#undef myMACAddress
#undef father
#undef node_status
#undef nodeID
#undef contr_send_num
#undef sendNum
#undef result
#undef g_test_type
#undef dest
#undef status

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_control_test_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_control_test_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (control_test)",
		sizeof (control_test_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_control_test_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	control_test_state * ptr;
	FIN_MT (_op_control_test_alloc (obtype))

	ptr = (control_test_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "control_test [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_control_test_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	control_test_state		*prs_ptr;

	FIN_MT (_op_control_test_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (control_test_state *)gen_ptr;

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
	if (strcmp ("result" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->result);
		FOUT
		}
	if (strcmp ("g_test_type" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_test_type);
		FOUT
		}
	if (strcmp ("dest" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->dest);
		FOUT
		}
	if (strcmp ("status" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->status);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}


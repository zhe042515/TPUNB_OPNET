/* Process model C form file: MAC_interface.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char MAC_interface_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A modeler 7 62A6A919 62A6A919 1 DESKTOP-RD4S7T2 51133 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                    ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */


/*
#define Port_MAC_MCPS_up			13
#define Port_MAC_MCPS_down			14

#define Port_MAC_MLME_up			5
#define Port_MAC_MLME_down			6
*/
#define		CEN_NODE				43690		//中心节点id



#define STRM_NWK_MCPS			((op_intrpt_type() == OPC_INTRPT_STRM) &&((op_intrpt_strm() == Port_NWK_MCPS_up_in)||(op_intrpt_strm() == Port_NWK_MCPS_down_in)))
#define STRM_NWK_MLME			((op_intrpt_type() == OPC_INTRPT_STRM) &&(op_intrpt_strm() == Port_NWK_MLME_in))
#define STRM_MAC_MCPS			((op_intrpt_type() == OPC_INTRPT_STRM) &&(op_intrpt_strm() == PHY_MAC0 || op_intrpt_strm() == PHY_MAC2))
#define STRM_MAC_MLME			((op_intrpt_type() == OPC_INTRPT_STRM) &&(op_intrpt_strm() == MLME_MAC0 || op_intrpt_strm() == MLME_MAC2))


#define Scan						0
#define Associate					1
#define Alive						2
#define ChildSEt					3
#define Conflict					4
#define Multicast					5
#define Leave						7
#define Syn_Seq_Set					8
#define BeaconConfirm				9


/*------------------ Input Index ------------------*/
//#define 	APP_MAC 					1		//来自上层发给上有节点的数据
#define 	PHY_MAC0 					2		//来自下游节点的数据
#define 	MLME_MAC0 					3		//来自下游节点的MLME数据
#define 	PHY_MAC2 					0		//来自上游节点的数据
#define 	MLME_MAC2 					1		//来自上游节点的MLME数据
#define		Port_NWK_MCPS_up_in			4		//网络层传来的数据帧(从对上的天线)
#define		Port_NWK_MCPS_down_in		5		//网络层传来的数据帧(从对下的天线)
#define 	Port_NWK_MLME_in			6		//网络层传来的原语

#define		PHY_MAC_down_1				7		//来自下游第二信道的数据
#define		PHY_MAC_down_2				8		//来自下游第三信道的数据
#define		PHY_MAC_down_3				9		//来自下游第四信道的数据

#define		MLME_MAC_down_1				10		//来自下游第二信道的原语
#define		MLME_MAC_down_2				11		//来自下游第三信道的原语
#define		MLME_MAC_down_3				12		//来自下游第四信道的原语




/* ------------------Output Index------------------ */
#define 	MAC_PHY_DOWN 					2		//发给下游节点
#define 	MLME_PHY_DOWN 					3		//MLME发给下游节点
#define 	MAC_PHY_UP 						0		//发给上游节点
#define 	MLME_PHY_UP 					1		//MLME发给上游节点
#define 	Port_NWK_MCPS_up_out			4		//传给网络层的数据帧(对上的天线)
#define 	Port_NWK_MCPS_down_out			5		//传给网络层的数据帧(对下的天线)
#define 	Port_NWK_MLME_out				6		//传给网络层的原语


#define 	MLME_PHY_DOWN_1 				7		//MLME发给第二信道
#define 	MLME_PHY_DOWN_2 				8		//MLME发给第三信道
#define 	MLME_PHY_DOWN_3 				9		//MLME发给第四信道





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
	int	                    		kkk                                             ;
	Objid	                  		my_process_objid                                ;
	Objid	                  		my_node_objid                                   ;
	int	                    		my_node_id                                      ;
	int	                    		my_node_type                                    ;
	} MAC_interface_state;

#define kkk                     		op_sv_ptr->kkk
#define my_process_objid        		op_sv_ptr->my_process_objid
#define my_node_objid           		op_sv_ptr->my_node_objid
#define my_node_id              		op_sv_ptr->my_node_id
#define my_node_type            		op_sv_ptr->my_node_type

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	MAC_interface_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((MAC_interface_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


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
	void MAC_interface (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_MAC_interface_init (int * init_block_ptr);
	void _op_MAC_interface_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_MAC_interface_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_MAC_interface_alloc (VosT_Obtype, int);
	void _op_MAC_interface_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
MAC_interface (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (MAC_interface ());

		{


		FSM_ENTER ("MAC_interface")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "init", "MAC_interface [init enter execs]")
				FSM_PROFILE_SECTION_IN ("MAC_interface [init enter execs]", state0_enter_exec)
				{
				kkk=0;
				my_node_objid = op_topo_parent(op_id_self());
				op_ima_obj_attr_get (my_node_objid,	"g_node_ESN_address",	&my_node_id);
				op_ima_obj_attr_get (my_node_objid,	"g_node_type",	&my_node_type);
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** state (init) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "init", "MAC_interface [init exit execs]")


			/** state (init) transition processing **/
			FSM_TRANSIT_ONLY ((1==1), 1, state1_enter_exec, ;, init, "1==1", "", "init", "idle", "tr_0", "MAC_interface [init -> idle : 1==1 / ]")
				/*---------------------------------------------------------*/



			/** state (idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "idle", state1_enter_exec, "MAC_interface [idle enter execs]")
				FSM_PROFILE_SECTION_IN ("MAC_interface [idle enter execs]", state1_enter_exec)
				{
				/*Packet* pk;
				int type;
				type = 0;
				my_process_objid = op_id_self();
				my_node_objid    = op_topo_parent(my_process_objid);
				op_ima_obj_attr_get (my_node_objid,	"MACAddress",	&my_node_id);
				//printf("node id %d   kkk = %d\n",my_node_id,kkk);
					kkk++;
				if( my_node_id == 2&& kkk == 5)
					{
					pk = op_pk_create(8);
					op_pk_fd_set (pk, 0, OPC_FIELD_TYPE_INTEGER,type,8) ;
					op_pk_send(pk,MLME_PHY2);
					printf("node %d send MLME_SCAN.request\n",my_node_id);
				
					}
				*/
				}
				FSM_PROFILE_SECTION_OUT (state1_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"MAC_interface")


			/** state (idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "idle", "MAC_interface [idle exit execs]")


			/** state (idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("MAC_interface [idle trans conditions]", state1_trans_conds)
			FSM_INIT_COND (STRM_NWK_MCPS)
			FSM_TEST_COND (STRM_NWK_MLME)
			FSM_TEST_COND (STRM_MAC_MCPS)
			FSM_TEST_COND (STRM_MAC_MLME)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("idle")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 2, state2_enter_exec, ;, "STRM_NWK_MCPS", "", "idle", "NWK_MCPS", "tr_1", "MAC_interface [idle -> NWK_MCPS : STRM_NWK_MCPS / ]")
				FSM_CASE_TRANSIT (1, 3, state3_enter_exec, ;, "STRM_NWK_MLME", "", "idle", "NWK_MLME", "tr_2", "MAC_interface [idle -> NWK_MLME : STRM_NWK_MLME / ]")
				FSM_CASE_TRANSIT (2, 5, state5_enter_exec, ;, "STRM_MAC_MCPS", "", "idle", "MAC_MCPS", "tr_5", "MAC_interface [idle -> MAC_MCPS : STRM_MAC_MCPS / ]")
				FSM_CASE_TRANSIT (3, 4, state4_enter_exec, ;, "STRM_MAC_MLME", "", "idle", "MAC_MLME", "tr_7", "MAC_interface [idle -> MAC_MLME : STRM_MAC_MLME / ]")
				FSM_CASE_TRANSIT (4, 1, state1_enter_exec, ;, "default", "", "idle", "idle", "tr_9", "MAC_interface [idle -> idle : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (NWK_MCPS) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "NWK_MCPS", state2_enter_exec, "MAC_interface [NWK_MCPS enter execs]")
				FSM_PROFILE_SECTION_IN ("MAC_interface [NWK_MCPS enter execs]", state2_enter_exec)
				{
				Packet* pk;
				if(op_intrpt_strm() == Port_NWK_MCPS_up_in)
					{
					pk = op_pk_get (op_intrpt_strm());
					op_pk_send(pk, MAC_PHY_UP);
					}
				else
					{
					pk = op_pk_get (op_intrpt_strm());
					op_pk_send(pk, MAC_PHY_DOWN);
					}
				
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (NWK_MCPS) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "NWK_MCPS", "MAC_interface [NWK_MCPS exit execs]")


			/** state (NWK_MCPS) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "NWK_MCPS", "idle", "tr_4", "MAC_interface [NWK_MCPS -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (NWK_MLME) enter executives **/
			FSM_STATE_ENTER_FORCED (3, "NWK_MLME", state3_enter_exec, "MAC_interface [NWK_MLME enter execs]")
				FSM_PROFILE_SECTION_IN ("MAC_interface [NWK_MLME enter execs]", state3_enter_exec)
				{
				Packet* pk;
				Packet* pk2;
				int type;
				pk = op_pk_get (op_intrpt_strm());
				op_pk_fd_get(pk,0,&type);
				//产生MAC命令帧
				//printf("node %d IO recv a mlme from nwk, type = %d\n",my_node_id, type);	
				switch(type)
					{
					case Scan: op_pk_send(pk, MLME_PHY_UP);
							printf("Node %d NWK_MLME received a Scan type command, type = %d\n", my_node_id, type);
							break;
					case Associate:
							op_pk_fd_get(pk,5,&type);
							if(type != 1) op_pk_send(pk, MLME_PHY_UP);
							else op_pk_send(pk, MLME_PHY_DOWN);
							printf("Node %d NWK_MLME received a Associate type command, type = %d\n", my_node_id, type);
							break;
					case ChildSEt:op_pk_send(pk, MLME_PHY_DOWN);
							printf("Node %d NWK_MLME received a ChildSEt type command, type = %d\n", my_node_id, type);
							break;
					case Multicast:op_pk_send(pk, MLME_PHY_UP);
							printf("Node %d NWK_MLME received a Multicast type command, type = %d\n", my_node_id, type);
							break;
					case Alive:op_pk_fd_get(pk,2,&type);
							if(type == 1) op_pk_send(pk, MLME_PHY_DOWN);
							else op_pk_send(pk, MLME_PHY_UP);//主动对父节点进行保活
							printf("Node %d NWK_MLME received a Alive type command, type = %d\n", my_node_id, type);
							break;
					case Leave:op_pk_fd_get(pk,1,&type);
							if(type == 0)
								{
								pk2 = op_pk_copy(pk);
								op_pk_send(pk, MLME_PHY_DOWN);
								op_pk_send(pk2, MLME_PHY_UP);
								}
							else
								{
								op_pk_send(pk, MLME_PHY_UP);
								pk2 = op_pk_create(0);
								op_pk_fd_set (pk2, 0, OPC_FIELD_TYPE_INTEGER, Leave, 8);
								op_pk_fd_set (pk2, 1, OPC_FIELD_TYPE_INTEGER, 2, 8);
								op_pk_send(pk2,Port_NWK_MLME_out);
								}
							
							printf("Node %d NWK_MLME received a Leave type command, type = %d\n", my_node_id, type);
							break;
					case Syn_Seq_Set:op_pk_send(pk, MLME_PHY_UP);
							printf("Node %d NWK_MLME Leave a Syn_Seq_Set type command, type = %d\n", my_node_id, type);
							break;
					default:printf("Node %d NWK_MLME received a error type command, type = %d\n", my_node_id, type);
							op_pk_destroy(pk);
					}
				
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** state (NWK_MLME) exit executives **/
			FSM_STATE_EXIT_FORCED (3, "NWK_MLME", "MAC_interface [NWK_MLME exit execs]")


			/** state (NWK_MLME) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "NWK_MLME", "idle", "tr_3", "MAC_interface [NWK_MLME -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (MAC_MLME) enter executives **/
			FSM_STATE_ENTER_FORCED (4, "MAC_MLME", state4_enter_exec, "MAC_interface [MAC_MLME enter execs]")
				FSM_PROFILE_SECTION_IN ("MAC_interface [MAC_MLME enter execs]", state4_enter_exec)
				{
				Packet* pk;
				int number;
				int type;
				int result;
				if(op_intrpt_strm() == MLME_MAC0 || op_intrpt_strm() == MLME_MAC_down_1 || op_intrpt_strm() == MLME_MAC_down_2 || op_intrpt_strm() == MLME_MAC_down_3)
					{
					pk = op_pk_get(op_intrpt_strm());
					op_pk_fd_get(pk,0,&type);	
					printf("Node %d interface receive a MLME from down\n", my_node_id);
					switch(type)
						{
						case 1:	//	关联确认原语
							{
							op_pk_fd_get(pk,2,&result);
							printf("Node %d receive a associate confirm at %f,result is %d\n",my_node_id,op_sim_time(),result);
							op_pk_send(pk,Port_NWK_MLME_out);
							break;
							}
						case Alive:op_pk_send(pk,Port_NWK_MLME_out);break;
						case Conflict:op_pk_send(pk,Port_NWK_MLME_out);break;
						case Leave:op_pk_send(pk,Port_NWK_MLME_out);printf("Node %d down io mlme leave\n", my_node_id);break;
						case Multicast:op_pk_send(pk,Port_NWK_MLME_out);break;
						case Syn_Seq_Set:op_pk_send(pk,Port_NWK_MLME_out);break;
						case BeaconConfirm:op_pk_send(pk,Port_NWK_MLME_out);break;
						default:
							{
							op_pk_destroy(pk);
							}
						
						}
					}
				else if(op_intrpt_strm() == MLME_MAC2)
					{
					pk = op_pk_get(op_intrpt_strm());
					op_pk_fd_get(pk,0,&type);	
					//printf("receive a MLME from up\n");
					switch(type)
						{
						case 0:	//	扫描确认原语
							{
							printf("Node %d receive a scan confirm at %f\n",my_node_id,op_sim_time());
							op_pk_fd_get(pk,1,&number);
							printf("number is %d\n",number);
							op_pk_send(pk,Port_NWK_MLME_out);
							break;
							}
						case 1:	//	关联确认原语
							{
							op_pk_fd_get(pk,2,&result);
							printf("Node %d receive a associate confirm at %f,result is %d\n",my_node_id,op_sim_time(),result);
							op_pk_send(pk,Port_NWK_MLME_out);
							break;
							}
						case Alive:printf("Node %d recv a alive\n", my_node_id);op_pk_send(pk,Port_NWK_MLME_out);break;
						case Conflict:op_pk_send(pk,Port_NWK_MLME_out);break;
						case Multicast:op_pk_send(pk,Port_NWK_MLME_out);break;
						case Leave:op_pk_send(pk,Port_NWK_MLME_out);printf("Node %d up io mlme leave\n", my_node_id);break;
						case BeaconConfirm:op_pk_send(pk,Port_NWK_MLME_out);break;
						default:
							{
							op_pk_destroy(pk);
							}
						
						}
					}
				}
				FSM_PROFILE_SECTION_OUT (state4_enter_exec)

			/** state (MAC_MLME) exit executives **/
			FSM_STATE_EXIT_FORCED (4, "MAC_MLME", "MAC_interface [MAC_MLME exit execs]")


			/** state (MAC_MLME) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "MAC_MLME", "idle", "tr_8", "MAC_interface [MAC_MLME -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (MAC_MCPS) enter executives **/
			FSM_STATE_ENTER_FORCED (5, "MAC_MCPS", state5_enter_exec, "MAC_interface [MAC_MCPS enter execs]")
				FSM_PROFILE_SECTION_IN ("MAC_interface [MAC_MCPS enter execs]", state5_enter_exec)
				{
				Packet* pk;
				if(op_intrpt_strm() == PHY_MAC0 || op_intrpt_strm() == PHY_MAC_down_1 || op_intrpt_strm() == PHY_MAC_down_2 ||op_intrpt_strm() == PHY_MAC_down_3)
					{
					//printf("node %d interface receive a msg from down\n",my_node_id);
					pk = op_pk_get(op_intrpt_strm());
					op_pk_send(pk,Port_NWK_MCPS_down_out);
					
					}
				else
					{
					//printf("node %d interface receive a msg from up\n",my_node_id);	
					pk = op_pk_get(op_intrpt_strm());
					op_pk_send(pk,Port_NWK_MCPS_up_out);
					
					}
				}
				FSM_PROFILE_SECTION_OUT (state5_enter_exec)

			/** state (MAC_MCPS) exit executives **/
			FSM_STATE_EXIT_FORCED (5, "MAC_MCPS", "MAC_interface [MAC_MCPS exit execs]")


			/** state (MAC_MCPS) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "MAC_MCPS", "idle", "tr_6", "MAC_interface [MAC_MCPS -> idle : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"MAC_interface")
		}
	}




void
_op_MAC_interface_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_MAC_interface_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_MAC_interface_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_MAC_interface_svar function. */
#undef kkk
#undef my_process_objid
#undef my_node_objid
#undef my_node_id
#undef my_node_type

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_MAC_interface_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_MAC_interface_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (MAC_interface)",
		sizeof (MAC_interface_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_MAC_interface_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	MAC_interface_state * ptr;
	FIN_MT (_op_MAC_interface_alloc (obtype))

	ptr = (MAC_interface_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "MAC_interface [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_MAC_interface_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	MAC_interface_state		*prs_ptr;

	FIN_MT (_op_MAC_interface_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (MAC_interface_state *)gen_ptr;

	if (strcmp ("kkk" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->kkk);
		FOUT
		}
	if (strcmp ("my_process_objid" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->my_process_objid);
		FOUT
		}
	if (strcmp ("my_node_objid" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->my_node_objid);
		FOUT
		}
	if (strcmp ("my_node_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->my_node_id);
		FOUT
		}
	if (strcmp ("my_node_type" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->my_node_type);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}


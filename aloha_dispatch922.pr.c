/* Process model C form file: aloha_dispatch922.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char aloha_dispatch922_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A modeler 7 61937761 61937761 1 DESKTOP-RD4S7T2 51133 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                    ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

//-----------------------*����ͷ�ļ�*-----------------------//


//-----------------------���峣��-----------------------//
#define		CEN_NODE				43690		//���Ľڵ�id
#define		Frame_Time				5			//һ��֡ʱ��
#define		Slot_Time				0.5			//һ��ʱ϶ʱ��
#define		Down_Time				0.01		//һ������ʱ϶ʱ��10ms
#define		Max_Slot_Num			11			//һ��֡ʱ϶��
#define		BroadCastAddress		0xffff		//�㲥��ַ
#define		GateAddress				0xaaaa		//���ص�ַ
#define		Max_Depth				8			//������
#define		Max_ACK					5			//����ش��ȴ�ʱ��
#define		BandWidth				4			//Ƶ�δ���
#define		MinBandWidth			470.0		//��СƵ�δ���
#define		MaxBandWidth			658.0		//���Ƶ�δ���
#define		PointWidth				1			//Ƶ�δ���
#define		Active_Time_Beacon		50			//�ű�֡����ʱ�䣬��λΪ֡(5s)
#define		ACTIVE_TIME				50			//�ӽڵ㱣��ʱ��
#define		FATHER_NODE_NUM			6			//Ǳ�ڸ��ڵ������
#define		RETRANS_TIME			6			//�ش��ȴ�ʱ��









#define MAXTTL				2
#define MAX_NODE			200
#define MAX_ROUTER			48
#define MAX_CHILD			50
#define MAX_RouterAddress	128
#define GATEPOINTNUM		8
#define POINTNUM			2
#define MAX_MULTICAST		20



//-------------------------�������-----------------------//
#define		QUEUE_NUM			6		//��������

#define		Data_Queue			0		//Ӧ�ò����ݶ��У����У�
#define		Retrans_Queue		1		//�ش����У����У�
#define		ACK_Queue			4		//ACK���У����У�
#define		Data_Queue_Down		2		//Ӧ�ò����ݶ��У����У�
#define		Retrans_Queue_Down	3		//�ش����У����У�
#define		ACK_Queue_Down		5		//ACK���У����У�


/**------------------����������-------------------**/





/**------------------Interupt Code-------------------**/
#define   	Start_Code					10000
#define   	Frame_Code         			20000	//֡�ж���
#define   	Slot_Code         			20050	//ʱ϶�ж���
#define   	Up_Code         			20100	//�����ж���
#define   	ACK_Code         			30000	//�ش��ж���
#define   	Tpsn_Code         			40000	//����ͬ���ж���
#define		intrCode_Status				2		//����Զ���ж�
#define		Lost_Father					3		//���ڵ�ʧ��Զ���ж�
#define		Scan_Complete				4		//ɨ�����Զ���жϣ�����control��
#define		Children_Active				5		//���ڵ��յ��ӽڵ�ı�����Ϣ
#define 	intrCode_Rejoin				6		//���¼���Զ���ж�
#define 	intrCode_UpStatus			7		//�뿪����Զ���ж�
#define		intrCode_assign				8		//ָ������Զ���ж�




/**------------------����״̬ת������-------------------**/
#define		Self_Start					((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Start_Code))
#define		App_Msg						((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == APP_MAC))
#define		App_Msg_Down				((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == APP_MAC_DOWN))

#define		Send_Packet					((Send_Beacon == 1)||(Send_Msg == 1))
#define		Msg_From_Down				((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == PHY_MAC0))
#define		Msg_From_Up					((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == PHY_MAC2))
#define		Start_Tpsn					((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Tpsn_Code))


#define		New_Slot					((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Slot_Code))
#define		New_Frame					((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Frame_Code))
#define		New_Up						((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Up_Code))
#define 	Join_Net					((op_intrpt_type() == OPC_INTRPT_REMOTE))

/*------------------ Input Index ------------------*/
#define 	APP_MAC 					1		//�����ϲ㷢�����нڵ������
#define 	PHY_MAC0 					0		//�������νڵ������
#define 	PHY_MAC2 					2		//�������νڵ������
#define 	APP_MAC_DOWN 				3		//�����ϲ㷢�����νڵ������
/* ------------------Output Index------------------ */
#define 	MAC_PHY2 					2		//�������νڵ�
#define 	MAC_PHY0 					0		//�������νڵ�
#define 	MAC_NWK 					1		//���������




//************************���������************************//





//************************����ṹ��************************//
struct Potential_Parent
	{
	int short_addr;
	double band;
	int frequency;
	int router_cost;
	int depth;
	int times;
	};

struct neighborTable
	{
	int node_id;
	int node_address;
	int seq;	
	int active_time;
	};

struct Retrans
	{
	int time;
	int count;
	};


struct childrenTable{
	int ESNAddress;
	int NWKAddress;
	int TimeoutCount;
	int CapabilityInformation;
};
struct CTable{
	int number;
	struct childrenTable table[MAX_CHILD];
};

struct routerTable{
	int destAddress;
	int nextHop;
	int status;
	int routerSeq;
	int TimeoutCount;
};
struct RTable{
	int number;
	struct routerTable table[MAX_ROUTER];
};

struct NetworkMsg{
	struct RTable NetworkRT[MAX_NODE];
	struct CTable NetworkCT[MAX_NODE];
};



/** ----------------Function Prototypes------------- **/

static void			Make_Beacon();
static void 		send_msg_to_children();
static void			send_msg_to_father();

//static void			beacon_proc();
static void			ACK1_proc(int seq ,int source ,int dest);
static void			ACK2_proc(int seq ,int source ,int dest);
static void			create_ack(Packet* pk , int i);

static void			set_channel (int i);
static void			change_band ();
static double	 	BandTo(int band);
static double		PointTo(int band,int point);

static int 			nextHop(int dest);
static void			Keep_Alive(int source_mode,int source);
static void			Alive();
static void			create_Actice(int dest,int dest_mode);

/** ----------------Globle Attribute------------- **/
extern	struct		NetworkMsg Network_Msg;

struct Potential_Parent potentialParent[MAX_NODE][FATHER_NODE_NUM];


/*++++++++++++++++++++++++test aloha++++++++++++++++++++++*/
/* Global Variable */
extern int subm_pkts =0;
extern int rcvd_pkts =0;

static void			test ();

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
	int	                    		num_backoffs                                    ;
	Packet *	               		pkptr                                           ;
	int	                    		backoff_exponent                                ;
	Evhandle	               		backoff_evhandle                                ;
	int	                    		max_backoffs                                    ;
	int	                    		aloha_init_performed                            ;
	Prohandle	              		parent_prohandle                                ;
	double	                 		unit_backoff_period                             ;
	Stathandle	             		gbl_sent_stathandle                             ;
	Stathandle	             		gbl_mac_delay_stathandle                        ;
	Stathandle	             		sent_stathandle                                 ;
	Stathandle	             		mac_delay_stathandle                            ;
	int	                    		my_node_id                                      ;	/* �ڵ�ID */
	Objid	                  		my_node_objid                                   ;	/* �ڵ�� */
	Objid	                  		my_process_objid                                ;	/* ���̺� */
	Objid	                  		subnet_objid                                    ;	/* ����� */
	int	                    		frame                                           ;	/* ֡��� */
	int	                    		slot                                            ;	/* ʱ϶��� */
	int	                    		synch_flag                                      ;	/* ͬ����ɱ�ʶ��		1��ʾͬ����� */
	int	                    		pkt_num_in_queue[QUEUE_NUM]                     ;	/* ����i�еİ����� */
	int	                    		my_pan_id                                       ;	/* �ڵ�pan id�� */
	int	                    		my_depth                                        ;	/* ��ǰ�ڵ���� */
	int	                    		acccess_control                                 ;	/* ��ǰ�ڵ��Ƿ��������� */
	int	                    		MAC_sequence                                    ;	/* MAC�����к� */
	int	                    		NET                                             ;	/* ��ʾ�ڵ��Ƿ�������1��ʾ����,0��ʾ��������2��ʾҪ����������3��ʾ������ 4��ʾ��Ҫ��ָ�����ڵ���� */
	int	                    		ack_time[5]                                     ;	/* ����ģ�����ACKʱ�� */
	int	                    		ack_time2[5]                                    ;	/* ����ģ��ACK����ʱ�� */
	Objid	                  		receiver_from_father                            ;	/* ���ո��ڵ���Ϣ�Ľ��ջ�objid */
	Objid	                  		receiver_from_children                          ;	/* �����ӽڵ���Ϣ�Ľ��ջ�objid */
	Objid	                  		sender_to_father                                ;	/* �򸸽ڵ㷢����Ϣ�ķ��ͻ�objid */
	Objid	                  		sender_to_children                              ;	/* ���ӽڵ㷢����Ϣ�ķ��ͻ�objid */
	Objid	                  		channel_of_recvive_from_father                  ;
	Objid	                  		channel_of_recvive_from_children                ;
	Objid	                  		channel_of_send_to_father                       ;
	Objid	                  		channel_of_send_to_children                     ;
	int	                    		pkt                                             ;	/* ���Է��Ͱ��ĸ��� */
	Objid	                  		receive_from_father_comp                        ;
	Objid	                  		receive_from_children_comp                      ;
	Objid	                  		sender_to_father_comp                           ;
	Objid	                  		sender_to_children_comp                         ;
	int	                    		max_packet_count                                ;	/* �������� */
	int	                    		Active_from_up                                  ;	/* ���ڵ�ı�����Ϣ�������������ű�֡ */
	int	                    		Scan_Flag                                       ;	/* �ŵ�ɨ���ʶ����1��ʾ��Ҫɨ�裬0��ʾ����Ҫɨ�� */
	int	                    		my_node_address                                 ;	/* ���ڵ������ַ */
	int	                    		my_node_type                                    ;	/* ���ڵ����� 0:���ؽڵ㣬1��·�ɽڵ㣬2����֪�ڵ㣬3������·�ɽڵ� */
	int	                    		no                                              ;	/* ��С·�ɴ��۱�� */
	int	                    		no2                                             ;	/* ��da ·�ɴ��۱�� */
	int	                    		mincost                                         ;	/* ��С·�ɴ��� */
	int	                    		maxcost                                         ;	/* ���·�ɴ��� */
	struct Potential_Parent			father_table[FATHER_NODE_NUM]                   ;	/* Ǳ�ڸ��ڵ�� */
	Stathandle	             		contr_send_num                                  ;
	int	                    		sendNum                                         ;
	struct neighborTable	   		neighbor_table[21]                              ;	/* �ڽڵ����¼�ڽڵ�����к� */
	struct Retrans	         		retransUp[5]                                    ;
	struct Retrans	         		retransDown[5]                                  ;
	} aloha_dispatch922_state;

#define num_backoffs            		op_sv_ptr->num_backoffs
#define pkptr                   		op_sv_ptr->pkptr
#define backoff_exponent        		op_sv_ptr->backoff_exponent
#define backoff_evhandle        		op_sv_ptr->backoff_evhandle
#define max_backoffs            		op_sv_ptr->max_backoffs
#define aloha_init_performed    		op_sv_ptr->aloha_init_performed
#define parent_prohandle        		op_sv_ptr->parent_prohandle
#define unit_backoff_period     		op_sv_ptr->unit_backoff_period
#define gbl_sent_stathandle     		op_sv_ptr->gbl_sent_stathandle
#define gbl_mac_delay_stathandle		op_sv_ptr->gbl_mac_delay_stathandle
#define sent_stathandle         		op_sv_ptr->sent_stathandle
#define mac_delay_stathandle    		op_sv_ptr->mac_delay_stathandle
#define my_node_id              		op_sv_ptr->my_node_id
#define my_node_objid           		op_sv_ptr->my_node_objid
#define my_process_objid        		op_sv_ptr->my_process_objid
#define subnet_objid            		op_sv_ptr->subnet_objid
#define frame                   		op_sv_ptr->frame
#define slot                    		op_sv_ptr->slot
#define synch_flag              		op_sv_ptr->synch_flag
#define pkt_num_in_queue        		op_sv_ptr->pkt_num_in_queue
#define my_pan_id               		op_sv_ptr->my_pan_id
#define my_depth                		op_sv_ptr->my_depth
#define acccess_control         		op_sv_ptr->acccess_control
#define MAC_sequence            		op_sv_ptr->MAC_sequence
#define NET                     		op_sv_ptr->NET
#define ack_time                		op_sv_ptr->ack_time
#define ack_time2               		op_sv_ptr->ack_time2
#define receiver_from_father    		op_sv_ptr->receiver_from_father
#define receiver_from_children  		op_sv_ptr->receiver_from_children
#define sender_to_father        		op_sv_ptr->sender_to_father
#define sender_to_children      		op_sv_ptr->sender_to_children
#define channel_of_recvive_from_father		op_sv_ptr->channel_of_recvive_from_father
#define channel_of_recvive_from_children		op_sv_ptr->channel_of_recvive_from_children
#define channel_of_send_to_father		op_sv_ptr->channel_of_send_to_father
#define channel_of_send_to_children		op_sv_ptr->channel_of_send_to_children
#define pkt                     		op_sv_ptr->pkt
#define receive_from_father_comp		op_sv_ptr->receive_from_father_comp
#define receive_from_children_comp		op_sv_ptr->receive_from_children_comp
#define sender_to_father_comp   		op_sv_ptr->sender_to_father_comp
#define sender_to_children_comp 		op_sv_ptr->sender_to_children_comp
#define max_packet_count        		op_sv_ptr->max_packet_count
#define Active_from_up          		op_sv_ptr->Active_from_up
#define Scan_Flag               		op_sv_ptr->Scan_Flag
#define my_node_address         		op_sv_ptr->my_node_address
#define my_node_type            		op_sv_ptr->my_node_type
#define no                      		op_sv_ptr->no
#define no2                     		op_sv_ptr->no2
#define mincost                 		op_sv_ptr->mincost
#define maxcost                 		op_sv_ptr->maxcost
#define father_table            		op_sv_ptr->father_table
#define contr_send_num          		op_sv_ptr->contr_send_num
#define sendNum                 		op_sv_ptr->sendNum
#define neighbor_table          		op_sv_ptr->neighbor_table
#define retransUp               		op_sv_ptr->retransUp
#define retransDown             		op_sv_ptr->retransDown

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	aloha_dispatch922_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((aloha_dispatch922_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif


//Build a beacon frame and transmit it-----------------------------------------
static void	Make_Beacon()
	{
	Packet* beacon;
	int 	frame_control;
	int		sequence;
	int		panid;
	int		dest;
	int		source;
	int		beacon_sequence;
	int		max_depth;
	int		current_depth;
	int		slot_number;
	int		access_control;
	int		router_cost;
	int		frequency;
	int		strenth;

	FIN(Make_Beacon());
	beacon = op_pk_create(120);
	frame_control	= 546;
//	MAC_sequence++;
//	if(MAC_sequence>=256)	MAC_sequence = 0;
	sequence		= MAC_sequence;
	panid 			= my_pan_id;
	dest 			= BroadCastAddress;
	source 			= my_node_address;
	beacon_sequence	= 0;
	max_depth 		= Max_Depth;
	current_depth	= my_depth;
	slot_number		= 0;
	access_control	= 0;
	router_cost		= my_depth;
	//op_ima_obj_attr_get (my_node_objid,	"NodePoint", &frequency);
	frequency 		=1;
	strenth			= 0;
	
	
	op_pk_fd_set (beacon, 0, OPC_FIELD_TYPE_INTEGER,frame_control,16) ;
	op_pk_fd_set (beacon, 1, OPC_FIELD_TYPE_INTEGER,sequence,8) ;
	op_pk_fd_set (beacon, 2, OPC_FIELD_TYPE_INTEGER,panid,16) ;
	op_pk_fd_set (beacon, 3, OPC_FIELD_TYPE_INTEGER,dest,16) ;
	op_pk_fd_set (beacon, 4, OPC_FIELD_TYPE_INTEGER,source,16) ;
	op_pk_fd_set (beacon, 5, OPC_FIELD_TYPE_INTEGER,beacon_sequence,8) ;
	op_pk_fd_set (beacon, 6, OPC_FIELD_TYPE_INTEGER,max_depth,4) ;
	op_pk_fd_set (beacon, 7, OPC_FIELD_TYPE_INTEGER,current_depth,4) ;
	op_pk_fd_set (beacon, 8, OPC_FIELD_TYPE_INTEGER,slot_number,4) ;
	op_pk_fd_set (beacon, 9, OPC_FIELD_TYPE_INTEGER,access_control,4);
	op_pk_fd_set (beacon, 10, OPC_FIELD_TYPE_INTEGER, router_cost,8) ;
	op_pk_fd_set (beacon, 11, OPC_FIELD_TYPE_INTEGER, frequency,8) ;
	op_pk_fd_set (beacon, 12, OPC_FIELD_TYPE_INTEGER, strenth,8) ;
		
	op_pk_send_delayed (beacon, MAC_PHY0,0.00001);
	op_stat_write(contr_send_num,++sendNum);
	printf("Node %d has sent a beacon at %d at time%f!!!!!!!!!\n",my_node_id,slot,op_sim_time());
	FOUT;
	}





//Transmit data frame to children node ---------------------------------------------
static void
send_msg_to_children()
	{
	Boolean isEmpty_data;
	Boolean isEmpty_retrans;
	Boolean isEmpty_ack;
	Packet* pk;
	Packet* copy;
	int framecontrol;
	int AR;
	int	num_pkts;
	int j;
	int NWKControl;
	int NWKtype;
	int seq;
	int dest;
	int count=0;
	
	double pk_create_time ;//ԭ���ݵĴ���ʱ��
	
	FIN(send_msg_to_children());
	//pk = op_pk_create();	
	isEmpty_ack = op_subq_empty (ACK_Queue_Down);
	if((isEmpty_ack == OPC_FALSE)&&(ack_time2[0]<=0))//ACK�����Ƿ�Ϊ�գ��ǿ��ȷ���ACK��
		{
		
		pk = op_subq_pk_remove (ACK_Queue_Down, 0);
		op_pk_fd_get(pk,1,&seq);
		op_pk_fd_get(pk,3,&dest);

		printf("\tnode %d send an ack packet at slot%d !dest = %d,seq = %d\n",my_node_id,slot,dest,seq);
		op_pk_send (pk, MAC_PHY0);
		op_stat_write(contr_send_num,++sendNum);
		for(j = 1;j < 5;j++)
			{
			ack_time2[j-1] = ack_time2[j];
			}
		ack_time2[4] = 100;
		FOUT;
		}	
	else						//ACK����Ϊ�գ��ж��ش������Ƿ��а����Է���
		{
		isEmpty_retrans = op_subq_empty (Retrans_Queue_Down);
		if((isEmpty_retrans == OPC_FALSE)&&(retransDown[0].time <= 0)&&(retransDown[0].count<5))
			{
				
				pk = op_subq_pk_remove (Retrans_Queue_Down, 0);
				count = retransDown[0].count;
				
				op_pk_fd_get(pk,1,&seq);
				op_pk_fd_get(pk,3,&dest);
				printf("\tnode %d send a retrans packet at slot%d !dest = %d,seq = %d\n",my_node_id,slot,dest,seq);
				
				pk_create_time= op_pk_creation_time_get(pk);
				copy = op_pk_copy(pk);
				op_pk_creation_time_set(copy,pk_create_time);
				printf("----pk create time = %f,copy create time= %f----\n",op_pk_creation_time_get (pk),op_pk_creation_time_get (copy));
				op_pk_fd_get (pk, 0, &framecontrol) ;
				op_pk_fd_get (pk, 5, &NWKControl) ;
				NWKtype = (NWKControl>>20)&&15;
				op_pk_send (pk, MAC_PHY0);
				if(NWKtype != 7 || NWKtype != 8 || NWKtype != 14)	op_stat_write(contr_send_num,++sendNum);
				for(j = 1;j < 5;j++)
					{
					retransDown[j-1].time = retransDown[j].time;
					retransDown[j-1].count = retransDown[j].count;
					}
				retransDown[4].time = 100;
				retransDown[4].count = 0;
			}
		else					//�ش�����Ϊ�գ�������ͨ���ݰ�
			{
			isEmpty_data = op_subq_empty (Data_Queue_Down);			
			if(isEmpty_data ==  OPC_FALSE)//send queue is not empty
				{
				
				//get the packet from the queue and send it at down frequency
				pk = op_subq_pk_remove (Data_Queue_Down, 0);
				
				op_pk_fd_get(pk,1,&seq);
				op_pk_fd_get(pk,3,&dest);
				printf("\tnode %d send a data packet at slot%d !dest = %d,seq = %d\n",my_node_id,slot,dest,seq);
				
				pk_create_time= op_pk_creation_time_get(pk);
				copy = op_pk_copy(pk);
				op_pk_creation_time_set(copy,pk_create_time);
				op_pk_fd_get (pk, 0, &framecontrol) ;
				op_pk_fd_get (pk, 5, &NWKControl) ;
				NWKtype = (NWKControl>>20)&&15;
				op_pk_send(pk, MAC_PHY0);
				if(NWKtype != 7 || NWKtype != 8 || NWKtype != 14)	op_stat_write(contr_send_num,++sendNum);
				}	
			else
				{
				//printf("slot %d NODE %d has no packet to be send to children!!!\n",slot,my_node_id);
				FOUT;
				}
			}
		printf("\tnode %d send a msg to children!!!\n",my_node_id);
		//��������İ������ش�����
		
		
		AR = (framecontrol>>10)&1;
		if(AR == 1)
			{
			if(count+1<5)
				{
				op_subq_pk_insert(Retrans_Queue_Down,copy,OPC_QPOS_TAIL);
				num_pkts = op_subq_stat (Retrans_Queue_Down, OPC_QSTAT_PKSIZE);
				printf("�ڵ�%d���д�ȷ�϶��а���+1  ��!\n",my_node_id);
				retransDown[num_pkts-1].time = RETRANS_TIME;
				retransDown[num_pkts-1].count = count+1;			
				printf("���д�ȷ�϶��а�����  %d\n",num_pkts);
				}
			else
				{
				printf("node %d send to children default!!!\n");
				}
			}
		else
			{
			op_pk_destroy(copy);
			}
		FOUT;
		}
	}



//Transmit data ftame to father node ---------------------------------------------
static void
send_msg_to_father()
	{
	Boolean isEmpty_data;
	Boolean isEmpty_retrans;
	Boolean isEmpty_ack;
	Packet* pk;
	Packet* copy;
	double     rand_int;

	int		num_pkts;
	int 	j;
	int 	AR;
	int		framecontrol;
	int		nwk_head;
	int		type;
	int 	subtype;
	double	freq;
	int		dest;
	int		seq;
	int 	count=0;
	
	int NWKControl;
	int NWKtype;
	
	double pk_create_time;

	FIN(send_msg_to_father());
	//when node need to send msg to father node ,node need to listen the channel(back off)
	//there is a msg need to be sent
	//random 
	rand_int = op_dist_uniform (1.0);
	//if(1==1)
	if(rand_int < 0.3)
		{			
		if((NET != 0))
			{
			isEmpty_ack = op_subq_empty (ACK_Queue);
			if((isEmpty_ack == OPC_FALSE)&&(ack_time[0]<=0))//ACK�����Ƿ�Ϊ�գ��ǿ��ȷ���ACK��
				{
				pk = op_subq_pk_remove (ACK_Queue, 0);
				
				op_pk_fd_get(pk,1,&seq);
				op_pk_fd_get(pk,3,&dest);
				printf("dest = %d\n",dest);

				printf("\tnode %d send an ack packet!dest = %d,seq = %d\n",my_node_id,dest,seq);
				
				op_pk_send (pk, MAC_PHY2);
				op_stat_write(contr_send_num,++sendNum);
				for(j = 1;j < 5;j++)
					{
					ack_time[j-1] = ack_time[j];
					}
				ack_time[4] = 100;
				FOUT;
				}
			else						//ACK����Ϊ�գ��ж��ش������Ƿ��а����Է���
				{
				isEmpty_retrans = op_subq_empty (Retrans_Queue);
				if((isEmpty_retrans == OPC_FALSE)&&(retransUp[0].time <= 0)&&(retransUp[0].count<5))
					{
					pk = op_subq_pk_remove (Retrans_Queue, 0);
					count = retransUp[0].count;
					
					op_pk_fd_get(pk,1,&seq);
					op_pk_fd_get(pk,3,&dest);
					printf("\tnode %d send a retrans packet!dest = %d,seq = %d\n",my_node_id,dest,seq);
					
					
					pk_create_time= op_pk_creation_time_get(pk);
					copy = op_pk_copy(pk);
					op_pk_creation_time_set(copy,pk_create_time);
					op_pk_fd_get (pk, 0, &framecontrol) ;
					op_pk_fd_get (pk, 5, &NWKControl) ;
					NWKtype = (NWKControl>>20)&&15;
					op_pk_send (pk, MAC_PHY2);
					if(NWKtype != 7 || NWKtype != 8 || NWKtype != 14)	op_stat_write(contr_send_num,++sendNum);
					for(j = 1;j < 5;j++)
						{
						retransUp[j-1].time = retransUp[j].time;
						retransUp[j-1].count = retransUp[j].count;
						}
					retransUp[4].time = 100;
					retransUp[4].count = 0;
					}
				else					//�ش�����Ϊ�գ�������ͨ���ݰ�
					{
					isEmpty_data = op_subq_empty (Data_Queue);
					if(isEmpty_data ==  OPC_FALSE)//send queue is not empty
						{
						//get the packet from the queue and send it at down frequency
						pk = op_subq_pk_remove (Data_Queue, 0);
						
						op_pk_fd_get(pk,1,&seq);
						op_pk_fd_get(pk,3,&dest);
						printf("\tnode %d send a data packet!dest = %d,seq = %d\n",my_node_id,dest,seq);
						
						
						pk_create_time= op_pk_creation_time_get(pk);
						copy = op_pk_copy(pk);
						op_pk_creation_time_set(copy,pk_create_time);
						op_pk_fd_get (pk, 0, &framecontrol) ;
						op_pk_fd_get (pk, 5, &NWKControl) ;
						NWKtype = (NWKControl>>20)&&15;
						op_pk_send(pk, MAC_PHY2);
						if(NWKtype != 7 || NWKtype != 8 || NWKtype != 14)	op_stat_write(contr_send_num,++sendNum);
						}	
					else
						{
						//printf("NODE %d has no packet to be send to father!!!\n",my_node_id);
						FOUT;
						}
					}
				printf("\nsend a msg to father!!!\n");
				op_ima_obj_attr_get (channel_of_send_to_father	,"min frequency",&freq);
				printf("\n--------------send at %f\n",freq);
/*				for(j = 1;j < 5;j++)
						{
						retransUp[j-1].time = retransUp[j].time;
						retransUp[j-1].count = retransUp[j].count;
						}
					retransUp[4].time = 100;
					retransUp[4].count = 0;
*/			
				//��������İ������ش�����
				AR = (framecontrol>>10)&1;
				if(AR == 1)
					{
					if(count+1<5)
						{
						op_subq_pk_insert(Retrans_Queue,copy,OPC_QPOS_TAIL);
						num_pkts = op_subq_stat (Retrans_Queue, OPC_QSTAT_PKSIZE);
						printf("�ڵ�%d���д�ȷ�϶��а���+1  !�\\n",my_node_id);
						retransUp[num_pkts-1].time = RETRANS_TIME;
						retransUp[num_pkts-1].count = count+1;				
						printf("���д�ȷ�϶��а�����  %d\n",num_pkts);
						}
					else
						{
						printf("node %d send to father default!!!\n");
						}
					}		
				else
					{
					op_pk_destroy(copy);
					}
				FOUT;
				}
		
			}
		else
		//else if(NET == 0 )//�ڵ㲻������ֻ�ܷ�����������֡
			{
			isEmpty_retrans = op_subq_empty (Retrans_Queue);
			//printf("isEmpty_retrans = %d,retransUp[0].time = %d,retransUp[0].count= %d\n",isEmpty_retrans,retransUp[0].time,retransUp[0].count);
			if((isEmpty_retrans == OPC_FALSE)&&(retransUp[0].time <= 0)&&(retransUp[0].count<5))
				{
				printf("\n\t�ڵ�%d������!!!!!!!\n",my_node_id);
				pk = op_subq_pk_remove (Retrans_Queue, 0);
				count = retransUp[0].count;					
				op_pk_fd_get(pk,1,&seq);
				op_pk_fd_get(pk,3,&dest);
				printf("\tnode %d send a retrans packet!dest = %d,seq = %d\n",my_node_id,dest,seq);
				pk_create_time= op_pk_creation_time_get(pk);
				op_pk_fd_get (pk, 0, &framecontrol) ;
				op_pk_fd_get (pk, 5, &NWKControl) ;
				NWKtype = (NWKControl>>20)&&15;
				if(NWKtype == 0)//��������֡
					{
					copy = op_pk_copy(pk);
					op_pk_creation_time_set(copy,pk_create_time);
					op_pk_send(pk, MAC_PHY2);				
					}
				else
					{
					op_pk_destroy(pk);
					FOUT;
					}
			//	if(NWKtype != 7 || NWKtype != 8 || NWKtype != 14)	op_stat_write(contr_send_num,++sendNum);
				for(j = 1;j < 5;j++)
					{
					retransUp[j-1].time = retransUp[j].time;
					retransUp[j-1].count = retransUp[j].count;
					}
				retransUp[4].time = 100;
				retransUp[4].count = 0;
				}
			else
				{
				isEmpty_data = op_subq_empty (Data_Queue);
				if(isEmpty_data ==  OPC_FALSE)
					{
					printf("\n\t�ڵ�%d������!!!!!!!\n",my_node_id);
					pk = op_subq_pk_remove (Data_Queue, 0);
					pk_create_time= op_pk_creation_time_get(pk);
					op_pk_fd_get (pk, 0, &framecontrol) ;
					type = (framecontrol>>13)&7;
					if (type == 1)//data frame
						{
						op_pk_fd_get (pk, 5, &nwk_head) ;
						subtype = (nwk_head>>20)&15;
						if(subtype == 0)//��������֡
							{
							copy = op_pk_copy(pk);
							op_pk_creation_time_set(copy,pk_create_time);
							//op_pk_fd_get (pk, 0, &framecontrol) ;//11111111111
							op_pk_send(pk, MAC_PHY2);
							//��������İ������ش�����
							printf("####Node %d has sent a frame to join the net\n",my_node_id);
							}
						else
							{
							op_pk_destroy(pk);
							FOUT;
							}
						}
					else
						{
						op_pk_destroy(pk);
						FOUT;
						}
					}
				else
					{
					FOUT;
					}
				}
			AR = (framecontrol>>10)&1;
			if(AR == 1)
				{
				op_subq_pk_insert(Retrans_Queue,copy,OPC_QPOS_TAIL);
				num_pkts = op_subq_stat (Retrans_Queue, OPC_QSTAT_PKSIZE);
				printf("���д�ȷ�϶��а���+1  !�\\n");
				retransUp[num_pkts-1].time = RETRANS_TIME;
				retransUp[num_pkts-1].count = count+1;
				//retrans[num_pkts-1] = 6;
				printf("���д�ȷ�϶��а�����  %d\n",num_pkts);
				}		
			else
				{
				op_pk_destroy(copy);
				}
			}			
/*			
			
			{
			//printf("\n\t�ڵ�%d������!!!!!!!\n",my_node_id);
			isEmpty_data = op_subq_empty (Data_Queue);
			if(isEmpty_data ==  OPC_FALSE)
				{
				printf("\n\t�ڵ�%d������!!!!!!!\n",my_node_id);
				pk = op_subq_pk_remove (Data_Queue, 0);
				count = retransUp[0].count;
				op_pk_fd_get (pk, 0, &framecontrol) ;
				type = (framecontrol>>13)&7;
				if (type == 1)//data frame
					{
					op_pk_fd_get (pk, 5, &nwk_head) ;
					subtype = (nwk_head>>20)&15;
					if(subtype == 0)//��������֡
						{
						copy = op_pk_copy(pk);
						//op_pk_fd_get (pk, 0, &framecontrol) ;//11111111111
						op_pk_send(pk, MAC_PHY2);
						//��������İ������ش�����
						AR = (framecontrol>>10)&1;
						if(AR == 1)
							{
							op_subq_pk_insert(Retrans_Queue,copy,OPC_QPOS_TAIL);
							num_pkts = op_subq_stat (Retrans_Queue, OPC_QSTAT_PKSIZE);
							printf("���д�ȷ�϶��а���+1  !�\\n");
							retransUp[num_pkts-1].time = 6;
							retransUp[num_pkts-1].count = count+1;
							//retrans[num_pkts-1] = 6;
							printf("���д�ȷ�϶��а�����  %d\n",num_pkts);
							}		
						else
							{
							op_pk_destroy(copy);
							}
						printf("####Node %d has sent a frame to join the net\n",my_node_id);
						FOUT;
						}
					else
						{
						op_pk_destroy(pk);
						}
					}
				op_pk_destroy(pk);
				}
			}*/
		}	
		/*
		//get the packet from the queue and send it at up frequency
		pk = op_subq_pk_access (Data_Queue, 0);
		//determine whether packet need ack reply
		op_pk_fd_get (pk, "FrameControl", &frame_control) ;
		AR = framecontrol&(1<<10);
		if(AR>0){
			op_subq_pk_insert(Retrans_Queue,pk,OPC_QPOS_TAIL);
			num_pkts = op_subq_stat (Retrans_Queue, OPC_QSTAT_PKSIZE);
			printf("�����ش����а���+1  ��");
			retrans[num_pkts-1] = 4;			
			}
		//send 
		op_pk_send (pk, MAC_PHY2);
		op_pk_destroy(pk);
		printf("send a msg to father node!");
		*/
		
	else{
		//printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!node %d back off!!!\n",my_node_id);
		}
	FOUT;
	}

/*
process of receiving a beacon frame from father node;
para: pk:pointer of beacon packet
*/
static void
beacon_proc(Packet* pk)
	{

	//int 	framecontrol;
	int		sequence;
	int		panid;
	//int		dest;
	int		source;
	int		beacon_sequence;
	int		max_depth;
	int		current_depth;
	int		slot_number;
	int		access_control;
	int		router_cost;
	int		frequency;
	int		strenth;
	int 	i;	
	FIN(beacon_proc(Packet* pk));	
	printf("\n\tNODE %d receive a beacon frame\n",my_node_id);
	op_pk_fd_get(pk,1,&sequence);
	op_pk_fd_get(pk,2,&panid);
	op_pk_fd_get(pk,4,&source);
	op_pk_fd_get(pk,5,&beacon_sequence);
	op_pk_fd_get(pk,6,&max_depth);
	op_pk_fd_get(pk,7,&current_depth);
	op_pk_fd_get(pk,8,&slot_number);
	op_pk_fd_get(pk,9,&access_control);
	op_pk_fd_get(pk,10,&router_cost);
	op_pk_fd_get(pk,11,&frequency);
	op_pk_fd_get(pk,12,&strenth);
	if( NET == 1 )
		{	
		/*�յ��ű�֡Ӧ�����ü�ʱ��������δ��ʧ���ڵ�����*/		
		Active_from_up = Active_Time_Beacon;		
		}
	if(Scan_Flag == 1)														//ɨ��״̬,���ڵ���Ϣ���븸�ڵ��
		{	
		if (op_td_get_dbl (pk, OPC_TDA_RA_RCVD_POWER) < 0.00000000000001 )
			{
			//op_pk_destroy(pk);
			FOUT;
			}
		if(maxcost > router_cost)
			{
			for(i = 0;i<FATHER_NODE_NUM;i++)
				{
				if(father_table[i].short_addr == source)
					{
					op_ima_obj_attr_get (channel_of_recvive_from_father	,"min frequency",&father_table[i].band);
					father_table[i].depth = current_depth;
					father_table[i].frequency = frequency;
					father_table[i].router_cost = current_depth;
					FOUT;
					}
				}
			
			father_table[no2].short_addr = source;
			op_ima_obj_attr_get (channel_of_recvive_from_father	,"min frequency",&father_table[no2].band);
			father_table[no2].depth = current_depth;
			father_table[no2].frequency = frequency;
			father_table[no2].router_cost = current_depth;
			maxcost = 0;
			mincost = 10;
			for (i=0;i<FATHER_NODE_NUM;i++)
				{
				if(maxcost < father_table[i].router_cost)
					{
					maxcost = father_table[i].router_cost;
					no2 = i;
					}
				if(mincost > father_table[i].router_cost)
					{
					mincost = father_table[i].router_cost;
					no = i;
					}
				}
			}
/*		printf("\n--------------cost = %d\n",router_cost);
		printf("\n--------------min = %d\n",mincost);
		printf("\n--------------max = %d\n",maxcost);
		printf("\n--------------no = %d\n",no);
		printf("\n--------------no2 = %d\n",no2);*/
		}
	FOUT;
	}



/*
process of receiving a ACK packet from father node
para:	seq:   seq of ACK packet,the same as the packet to be ACK
		dest:  dest of ACK packet,the same as the source of the packet to be confirm
*/
static void
ACK1_proc(int seq ,int source ,int dest)
	{
	int 	i,j;
	Packet* packet;
	Packet* packet_find;
	int 	ack;				//���ڴ���ش������ж�ȡ�İ������к�
	int 	number;
	int 	data_source;		//��ȷ�϶�����֡��Դ�ڵ�
	int		data_dest;			//��ȷ�϶�����֡��Ŀ�Ľڵ�
	
	FIN(ACK1_proc(int seq ,int source ,int dest));
	printf("\t&&&&&NODE %dreveive an ack packet!!!\n",my_node_id);
	number = op_subq_stat (Retrans_Queue, OPC_QSTAT_PKSIZE);
	
	for(i=0;i<number;i++)
		{
		packet = op_subq_pk_access (Retrans_Queue, i);
		op_pk_fd_get(packet,1,&ack);
		op_pk_fd_get(packet,4,&data_source);
		op_pk_fd_get(packet,3,&data_dest);
		if((ack == seq)&&(data_source == dest)&&(data_dest == source))
			{
			packet_find = op_subq_pk_remove (Retrans_Queue, i);
			//ɾ��������****************************************
			for(j = i;j < 4;j++)
				{
				retransUp[j].time = retransUp[j+1].time;
				retransUp[j].count = retransUp[j+1].count;
				}
			retransUp[4].time = 100;
			retransUp[4].count = 0;
			printf("\tNODE %dfind the packet!!!,seq = %d\n",my_node_id,seq);

			op_pk_destroy(packet);
			FOUT;
			}
		//op_pk_destroy(packet);
		}
	printf("wrong ack frame!!!\n");
	FOUT;
	}


/*
process of receiving a ACK packet from children node
para:	seq:   seq of ACK packet,the same as the packet to be ACK
*/
static void
ACK2_proc(int seq ,int source ,int dest)
	{
	int 	i,j;
	Packet* packet;
	Packet* packet_find;
	int 	ack;				//���ڴ���ش������ж�ȡ�İ������к�
	int 	number;
	int 	data_source;		//��ȷ��֡��Դ�ڵ�
	int		data_dest;			//��ȷ��֡��Ŀ�Ľڵ�
	
	FIN(ACK2_proc(int seq ,int source ,int dest));
	printf("\t&&&&&NODE %dreveive an ack packet!!!\n",my_node_id);
	
	number = op_subq_stat (Retrans_Queue_Down, OPC_QSTAT_PKSIZE);
	printf("\t&number = number!!!\n",number);
	for(i=0;i<number;i++)//�ش������а�����
		{
		packet = op_subq_pk_access (Retrans_Queue_Down, i);
		op_pk_fd_get(packet,1,&ack);
		printf("\t1!!\n");
		op_pk_fd_get(packet,4,&data_source);
		printf("\t4!!\n");
		op_pk_fd_get(packet,3,&data_dest);
		printf("\t3!!\n");
		if((ack == seq)&&(data_source == dest)&&(data_dest == source))
			{
			packet_find = op_subq_pk_remove (Retrans_Queue_Down, i);
			//ɾ��������****************************************
			for(j = i;j < 4;j++)
				{
				retransDown[j].time = retransDown[j+1].time;
				retransDown[j].count = retransDown[j+1].count;
				}
			retransDown[4].time = 100;
			retransDown[4].count = 0;
			printf("\tNODE %dfind the packet!!!,seq = %d\n",my_node_id,seq);
			op_pk_destroy(packet);
			FOUT;
			}
		//op_pk_destroy(packet);
		}
	FOUT;
	}

/*
up
create an ACK frame,and push it in ACK queue

1:�������νڵ����ͨack֡					source_mode = 2,dest_mode = 2
2:�������νڵ����Ŀ�ڵ�ack֡				source_mode = 2,dest_mode = 1
3:�������νڵ�ĳ�Դ��ַ��Ŀ�ĵ�ַack֡		source_mode = 3,dest_mode = 2
4:�������νڵ�ĳ�Դ��ַ��Ŀ�ĵ�ַack֡		source_mode = 3,dest_mode = 1

5:�������νڵ����ͨack֡					source_mode = 2,dest_mode = 2
6:�������νڵ�ĳ�Ŀ�ĵ�ַack֡				source_mode = 2,dest_mode = 3
7:�������νڵ����Դ�ڵ�ack֡				source_mode = 1,dest_mode = 2
8:�������νڵ����Դ�ڵ㳤Ŀ�Ľڵ�ack֡		source_mode = 1,dest_mode = 3
*/

static void			create_ack(Packet* pk,int i)
	{
	int pk_source;
	int pk_dest;
	int pk_seq;
	int pk_panid;
	int framecontrol;
	int num_pkts;
	int source_mode;
	int dest_mode;
	Packet* ack;
	
	FIN(create_ack(Packet* pk,int i));
	op_pk_fd_get(pk,0,&framecontrol);
	op_pk_fd_get(pk,1,&pk_seq);
	op_pk_fd_get(pk,2,&pk_panid);
	op_pk_fd_get(pk,3,&pk_dest);
	op_pk_fd_get(pk,4,&pk_source);

	source_mode = framecontrol&3;
	dest_mode	=(framecontrol>>4)&3;
	switch (i)
		{
		case 1://source_mode = 2,dest_mode = 2
		ack = op_pk_create(72);
		framecontrol = 16930;
		op_pk_fd_set(ack,3,OPC_FIELD_TYPE_INTEGER,pk_source,16);
		op_pk_fd_set(ack,4,OPC_FIELD_TYPE_INTEGER,pk_dest,16);
		break;
		
		case 2://source_mode = 2,dest_mode = 1
		ack = op_pk_create(56);
		framecontrol = 16914;
		op_pk_fd_set(ack,3,OPC_FIELD_TYPE_INTEGER,pk_source,0);//ooooooo
		op_pk_fd_set(ack,4,OPC_FIELD_TYPE_INTEGER,pk_dest,16);
		break;
		
		case 3://source_mode = 3,dest_mode = 2
		ack = op_pk_create(104);
		framecontrol = 16931;
		op_pk_fd_set(ack,3,OPC_FIELD_TYPE_INTEGER,pk_source,16);
		op_pk_fd_set(ack,4,OPC_FIELD_TYPE_INTEGER,pk_dest,48);
		break;
		
		case 4://source_mode = 3,dest_mode = 1
		ack = op_pk_create(88);
		framecontrol = 16915;
		op_pk_fd_set(ack,3,OPC_FIELD_TYPE_INTEGER,pk_source,0);//ooooooo
		op_pk_fd_set(ack,4,OPC_FIELD_TYPE_INTEGER,pk_dest,48);
		break;
		
		case 5://source_mode = 2,dest_mode = 2
		ack = op_pk_create(72);
		framecontrol = 16930;
		op_pk_fd_set(ack,3,OPC_FIELD_TYPE_INTEGER,pk_source,16);
		op_pk_fd_set(ack,4,OPC_FIELD_TYPE_INTEGER,pk_dest,16);
		break;
		
		case 6://source_mode = 2,dest_mode = 3
		ack = op_pk_create(104);
		framecontrol = 16946;
		op_pk_fd_set(ack,3,OPC_FIELD_TYPE_INTEGER,pk_source,48);
		op_pk_fd_set(ack,4,OPC_FIELD_TYPE_INTEGER,pk_dest,16);
		break;
			
		case 7://source_mode = 1,dest_mode = 2
		ack = op_pk_create(56);
		framecontrol = 16929;
		op_pk_fd_set(ack,3,OPC_FIELD_TYPE_INTEGER,pk_source,16);
		op_pk_fd_set(ack,4,OPC_FIELD_TYPE_INTEGER,pk_dest,0);//ooooooo
		break;
		
		case 8://source_mode = 1,dest_mode = 3
		ack = op_pk_create(104);
		framecontrol = 16945;
		op_pk_fd_set(ack,3,OPC_FIELD_TYPE_INTEGER,pk_source,48);
		op_pk_fd_set(ack,4,OPC_FIELD_TYPE_INTEGER,pk_dest,0);//ooooooo
		break;
		
		default:
		break;	
		}
	op_pk_fd_set(ack,0,OPC_FIELD_TYPE_INTEGER,framecontrol,16 );
	op_pk_fd_set(ack,1,OPC_FIELD_TYPE_INTEGER,pk_seq,8);
	op_pk_fd_set(ack,2,OPC_FIELD_TYPE_INTEGER,pk_panid,16);
	if((i == 1)||(i == 2)||(i == 3)||(i == 4))
		{
		if (op_subq_pk_insert(ACK_Queue, ack, OPC_QPOS_TAIL) != OPC_QINS_OK)
			{
			printf("\n###Node %d:::data package from route insert into queue failed###\n", my_node_id);
			op_pk_destroy(pk);
			}
		num_pkts = op_subq_stat (ACK_Queue, OPC_QSTAT_PKSIZE);
		if(num_pkts<=5)
			{
			ack_time[num_pkts-1] = 2;
			}	
		else
			{
			printf("node %d up ack queue cannot push in!!!\n",my_node_id);
			}
		}
	else
		{
		if (op_subq_pk_insert(ACK_Queue_Down, ack, OPC_QPOS_TAIL) != OPC_QINS_OK)
			{
			printf("\n###Node %d:::data package from route insert into queue failed###\n", my_node_id);
			op_pk_destroy(pk);
			}
		num_pkts = op_subq_stat (ACK_Queue_Down, OPC_QSTAT_PKSIZE);
		if(num_pkts<=5)
			{
			ack_time2[num_pkts-1] = 2;
			}	
		else
			{
			printf("node %d down ack queue cannot push in!!!\n",my_node_id);
			}
		}
	FOUT;
	}





/*
find a best father node ,set channel of send to father,and recv from father
*/
static void			set_channel (int i)
	{
	double freq;
	double	send;
	FIN(set_channel (int i));
	freq = father_table[i].band;
	send = freq+((father_table[i].frequency));
	op_ima_obj_attr_set (channel_of_send_to_father ,"min frequency",send);
	op_ima_obj_attr_set (channel_of_recvive_from_father	,"min frequency",freq);
	printf("node %d send to father =%f\n",my_node_id,send);
	printf("node %d channel_of_recvive_from_father =%f\n",my_node_id,freq);
	FOUT;
	}


/*
change the receiver's band  ��scan for a channel
*/
static void			change_band()
	{
	double		current_band;
	double		new_band;
	int 		i;
	FIN(change_band());
	op_ima_obj_attr_get (channel_of_recvive_from_father,"min frequency",&current_band);
	printf("\n___________________________________node %d MAC change_band   %f\n\n\n\n",my_node_id,current_band);
	if(current_band == MaxBandWidth)
		{
		if(mincost == 10)					//û��ɨ�赽���ø��ڵ㡣��ͷɨ��
			{
			printf("Node %d has not found a father node!!Keep scanning !!!\n",my_node_id);
			new_band = 470.0;
			Scan_Flag = 1;
			}
		else								//ɨ�赽���ø��ڵ㣬�����ŵ�
			{
			set_channel(no);
			father_table[no].times = 1;
			neighbor_table[0].node_address = CEN_NODE;
			neighbor_table[0].seq = 0;
			Scan_Flag = 0;
			printf("\n-------------mincost-no = %d\n",no);
			for(i = 0;i<FATHER_NODE_NUM;i++)
				{
				potentialParent[my_node_id][i] = father_table[i];
				printf("\n--------------father_table[%d],cost = %d\n",i,father_table[i].router_cost);
				printf("\n--------------father_table[%d],depth= %d\n",i,father_table[i].depth);
				printf("\n--------------father_table[%d],band= %f\n",i,father_table[i].band);
				printf("\n--------------father_table[%d],freq= %d\n",i,father_table[i].frequency);
				}
			op_ima_obj_attr_set(my_node_objid,"father",father_table[no].short_addr);
			op_intrpt_schedule_remote(op_sim_time(), Scan_Complete, 
				op_id_from_name (op_topo_parent(op_id_self()) , OPC_OBJTYPE_PROC, "Control source")) ;
			FOUT;
			}		
		}
	else
		{
		new_band = current_band + BandWidth;
		}

	op_ima_obj_attr_set (channel_of_recvive_from_father	,"min frequency",new_band);	
	FOUT;
	}






static void 		test()
	{
	int 	framecontrol;
	int		sequence;
	int		panid;
	int		dest;
	int		source;
	int		nwkhead;
	
	double				s;
	

	FIN(test());
	
	s = op_dist_uniform(1);
	
	if(s  < 0.05)
		{
		pkptr = op_pk_create(200);
		framecontrol = 9216;
		sequence = MAC_sequence++;
		panid = 123;
		dest = 20;
		source = my_node_id;
		nwkhead = 0;
		
		op_pk_fd_set (pkptr, 0, OPC_FIELD_TYPE_INTEGER,framecontrol,16) ;
		op_pk_fd_set (pkptr, 1, OPC_FIELD_TYPE_INTEGER,sequence,8) ;
		op_pk_fd_set (pkptr, 2, OPC_FIELD_TYPE_INTEGER,panid,16) ;
		op_pk_fd_set (pkptr, 3, OPC_FIELD_TYPE_INTEGER,dest,16) ;
		op_pk_fd_set (pkptr, 4, OPC_FIELD_TYPE_INTEGER,source,16) ;
		op_pk_fd_set (pkptr, 5, OPC_FIELD_TYPE_INTEGER,source,16) ;
		/*if (op_subq_pk_insert(Data_Queue, pkptr, OPC_QPOS_TAIL) != OPC_QINS_OK)
			{
			printf("\n###Node %d:::data package from route insert into queue failed###\n", my_node_id);
			op_pk_destroy(pkptr);
			}
		*/
		
		//printf("j = %d\n\n\n\n",pkt);
		
		op_pk_send (pkptr, MAC_PHY2);
		
		++subm_pkts;
		printf("seq ====%d+++++++NODE %d send a packet+++++++%d_______at slot%d time is %f\n\n\n"
			,sequence,my_node_id,subm_pkts,slot,op_sim_time() );
		
		if (subm_pkts == max_packet_count)
			{
			printf("++++++++++++++++++++++%d\n\n\n",rcvd_pkts );
			printf("++++++++++++++++++++++%d\n\n\n",subm_pkts );
			printf("++++++++++++++++++++++%d\n\n\n",max_packet_count );
			op_sim_end ("max packet count reached.", "", "", ""); 
			

			}

		}
	
	FOUT;
	}

static double 		BandTo(int band)
	{
	double freq;
	FIN(BandTo(int band));
	freq = 470.0+4*band;
	FRET(freq);	
	}

static double 		PointTo(int band,int point)
	{
	double freq;
	FIN(PointTo(int point))	
	freq = 470.0+4*band+point;
	FRET(freq);
	}

/*
static int nextHop(int dest)
{
	int i = 0;
	FIN(nextHop(int dest));
	dest &= (255<<8);
	for(i = 0;i < Network_Msg.NetworkRT[my_node_id].number;i++)
	{
		if(Network_Msg.NetworkRT[my_node_id].table[i].destAddress == dest && Network_Msg.NetworkRT[my_node_id].table[i].status == 0)
		{
			FRET(Network_Msg.NetworkRT[my_node_id].table[i].nextHop);
		}
	}
	FRET(0);
}*/

static int nextHop(int dest)
{
	int i = 0;
	int fatherAddress;
	FIN(nextHop(int dest));
	if((dest&(1<<15)) > 0)
	{
		FRET(dest);
	}
	fatherAddress = dest & (255<<8);
	if(my_node_address == fatherAddress)
	{
		FRET(dest);
	}
	for(i = 0;i < Network_Msg.NetworkRT[my_node_id].number;i++)
	{
		if(Network_Msg.NetworkRT[my_node_id].table[i].destAddress == fatherAddress && Network_Msg.NetworkRT[my_node_id].table[i].status == 0)
		{
			FRET(Network_Msg.NetworkRT[my_node_id].table[i].nextHop);
		}
	}
	FRET(0);
}


/*���±���ʱ�䣬�ڵ㱣��*/
static void Keep_Alive(int source_mode,int source)
	{
	int j;
	int next;
	FIN(Keep_Alive(int source_mode,int source));
	if (source_mode == 2)
		{
		next = nextHop(source);
		for(j = 0;j < Network_Msg.NetworkCT[my_node_id].number;j++)
			{
			if(next ==  Network_Msg.NetworkCT[my_node_id].table[j].NWKAddress)
				{
				Network_Msg.NetworkCT[my_node_id].table[j].TimeoutCount = ACTIVE_TIME;
				break;
				}			
			}
		}
	else if (source_mode == 3)
		{
		for(j = 0;j < Network_Msg.NetworkCT[my_node_id].number;j++)
			{
			if(source ==  Network_Msg.NetworkCT[my_node_id].table[j].ESNAddress)
				{
				Network_Msg.NetworkCT[my_node_id].table[j].TimeoutCount = ACTIVE_TIME;
				break;
				}			
			}
		}
	FOUT;
	}
/*����������Ϣ֡  frame_type = 3*/
static void	create_Actice(int dest,int dest_mode)
	{
	Packet* alive;
	int 	frame_control;
	int		sequence;
	int		panid;
	int		source;
	FIN(create_Actice(int dest));
	printf("create_Actice\n");
	if (dest_mode == 2)
		{
		alive = op_pk_create(72);
		frame_control	= 26146;
		op_pk_fd_set (alive, 0, OPC_FIELD_TYPE_INTEGER,frame_control,16) ;
		op_pk_fd_set (alive, 3, OPC_FIELD_TYPE_INTEGER,dest,16) ;
		}
	else if(dest_mode == 3)
		{
		alive = op_pk_create(104);
		frame_control	= 26162;
		op_pk_fd_set (alive, 0, OPC_FIELD_TYPE_INTEGER,frame_control,16) ;
		op_pk_fd_set (alive, 3, OPC_FIELD_TYPE_INTEGER,dest,16) ;
		}
	
	MAC_sequence++;
	if(MAC_sequence>=256)	MAC_sequence = 0;
	sequence		= MAC_sequence;
	panid 			= my_pan_id;
	source 			= my_node_address;
	
	op_pk_fd_set (alive, 1, OPC_FIELD_TYPE_INTEGER,sequence,8) ;
	op_pk_fd_set (alive, 2, OPC_FIELD_TYPE_INTEGER,panid,16) ;
	op_pk_fd_set (alive, 4, OPC_FIELD_TYPE_INTEGER,source,16) ;
	printf("dest = %d\n",dest);
	if (op_subq_pk_insert(Data_Queue_Down, alive, OPC_QPOS_TAIL) != OPC_QINS_OK)
		{
		printf("\n###Node %d:::data package from route insert into queue failed###\n", my_node_id);
		op_pk_destroy(alive);
		}
	FOUT;
	}

/*�жϽڵ��Ƿ�ʧ��*/
static void Alive()
	{
	int j;
	int dest;
	int dest_mode;
	FIN(Alive());
		{
		printf("Alive\n");
		for(j = 0;j < Network_Msg.NetworkCT[my_node_id].number;j++)
			{
			if(Network_Msg.NetworkCT[my_node_id].table[j].TimeoutCount <= 5)
				{
				dest = Network_Msg.NetworkCT[my_node_id].table[j].NWKAddress;
				dest_mode = 2;
				if(dest<=0)
					{
					dest = Network_Msg.NetworkCT[my_node_id].table[j].ESNAddress;
					dest_mode = 3;
					}
				create_Actice(dest,dest_mode);
				}			
			}
		}
	FOUT;
	}



static void Find_Father()
	{
	int 	i;
	int 	mincost1 = 10;
	int		no1 = 10;
	FIN(Find_Father());
	for (i=0;i<FATHER_NODE_NUM;i++)
		{
		if(mincost1 > father_table[i].router_cost)
			{
			if(father_table[i].times == 0)
				{
				mincost1 = father_table[i].router_cost;
				no1 = i;
				}
			continue;
			}
		}
	if(no1 != 10)				//Ǳ�ڸ��ڵ�����п��ø��ڵ�
		{
		set_channel(no1);
		op_ima_obj_attr_set(my_node_objid,"father",father_table[no1].short_addr);
		op_intrpt_schedule_remote(op_sim_time(), Scan_Complete, 
			op_id_from_name (op_topo_parent(op_id_self()) , OPC_OBJTYPE_PROC, "Control source")) ;
		}	
	else						//Ǳ�ڸ��ڵ�����޿��ø��ڵ㣬��Ҫ����ɨ��
		{
		for (i=0;i<FATHER_NODE_NUM;i++)		//����Ǳ�ڸ��ڵ��
			{
			father_table[i].short_addr = 0;
			father_table[i].band = 0;
			father_table[i].depth = 0;
			father_table[i].frequency = 0;
			father_table[i].router_cost = 10;
			father_table[i].times = 0;
			}
		op_ima_obj_attr_set (channel_of_recvive_from_father,"min frequency",MaxBandWidth);
		mincost = 10;
		Scan_Flag = 1;
		change_band();
		}
	FOUT;
	}

/*�뿪����ǰ��������գ�Ȼ����Ѱ�ҿ��ø��ڵ�*/
static void Leave_NET()
	{
	Boolean isEmpty_data_up;
	Boolean isEmpty_retrans_up;
	Boolean isEmpty_ack_up;
	Boolean isEmpty_data_down;
	Boolean isEmpty_retrans_down;
	Boolean isEmpty_ack_down;
	int band;
	int Father;
	FIN(Leave_NET());
	isEmpty_ack_up = op_subq_empty (ACK_Queue);
	isEmpty_retrans_up = op_subq_empty (Retrans_Queue);
	isEmpty_data_up = op_subq_empty (Data_Queue);
	isEmpty_ack_down = op_subq_empty (ACK_Queue_Down);
	isEmpty_retrans_down = op_subq_empty (Retrans_Queue_Down);
	isEmpty_data_down = op_subq_empty (Data_Queue_Down);
	if((isEmpty_ack_up == OPC_TRUE)&&(isEmpty_retrans_up == OPC_TRUE)&&(isEmpty_data_up == OPC_TRUE)&&
		(isEmpty_ack_down == OPC_TRUE)&&(isEmpty_retrans_down == OPC_TRUE)&&(isEmpty_data_down == OPC_TRUE))
		{							//���������
		printf("\tNode %d queue is empty!!!\n ",my_node_id);
		if(NET == 2)			//��������
			{
			NET = 0;
			op_ima_obj_attr_set (my_node_objid,	"NodeStatus", 0);
			printf("\tNode %d leave the net and rejoin the net!!!\n ",my_node_id);
			Find_Father();						//Ѱ���µĸ��ڵ�	
			}
		else if(NET == 3)			//������
			{
			NET = 0;
			printf("\tNode %d leave the net!!!\n ",my_node_id);
			op_ima_obj_attr_set (my_node_objid,	"NodeStatus", 0);
			}
		else if(NET == 4)
			{
			NET = 0;
			op_ima_obj_attr_get (my_node_objid,	"NodeBand",	&band);
			op_ima_obj_attr_get (my_node_objid,	"father",	&Father);
			//printf("band = %d\n",band);
			op_ima_obj_attr_set (channel_of_recvive_from_father		,"min frequency",BandTo(band));
			op_ima_obj_attr_set (channel_of_send_to_father	,"min frequency",PointTo(band,1));
			op_intrpt_schedule_remote(op_sim_time(), intrCode_assign, 
				op_id_from_name (op_topo_parent(op_id_self()) , OPC_OBJTYPE_PROC, "Control source")) ;
			}		
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
	void aloha_dispatch922 (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_aloha_dispatch922_init (int * init_block_ptr);
	void _op_aloha_dispatch922_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_aloha_dispatch922_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_aloha_dispatch922_alloc (VosT_Obtype, int);
	void _op_aloha_dispatch922_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
aloha_dispatch922 (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (aloha_dispatch922 ());

		{


		FSM_ENTER ("aloha_dispatch922")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_UNFORCED_NOLABEL (0, "init", "aloha_dispatch922 [init enter execs]")
				FSM_PROFILE_SECTION_IN ("aloha_dispatch922 [init enter execs]", state0_enter_exec)
				{
				
				
				
				
				my_process_objid = op_id_self();															//��ȡ���ڵ��ʵ��
				my_node_objid    = op_topo_parent(my_process_objid);
				//subnet_objid     = op_topo_parent (my_node_objid);	
						
				op_ima_obj_attr_get (my_node_objid,	"MACAddress",	&my_node_id);
				op_ima_obj_attr_get (my_node_objid,	"NodeType"	,	&my_node_type);
				op_ima_obj_attr_get (my_node_objid,	"NodeStatus",	&NET);
				
				printf("���ڵ��MAC��ַʱ%d\n",my_node_id);
				//op_ima_obj_attr_get (my_node_objid,	"pan_id",	&my_pan_id);
				//op_ima_obj_attr_get (my_node_objid,	"depth",	&my_depth);
				//op_ima_obj_attr_get (my_node_objid,	"acccess_control",	&acccess_control);
				receiver_from_father = op_id_from_name (my_node_objid, OPC_OBJTYPE_RARX , "receive from father"); 
				receiver_from_children = op_id_from_name (my_node_objid, OPC_OBJTYPE_RARX , "receive from children");
				sender_to_father = op_id_from_name (my_node_objid, OPC_OBJTYPE_RATX , "send to father");
				sender_to_children = op_id_from_name (my_node_objid, OPC_OBJTYPE_RATX , "send to children");
				
				receive_from_father_comp = op_topo_child (receiver_from_father, OPC_OBJTYPE_COMP , 0); 
				receive_from_children_comp = op_topo_child (receiver_from_children, OPC_OBJTYPE_COMP , 0);
				sender_to_father_comp = op_topo_child (sender_to_father, OPC_OBJTYPE_COMP , 0); 
				sender_to_children_comp = op_topo_child (sender_to_children, OPC_OBJTYPE_COMP , 0); 
				
				channel_of_recvive_from_father = op_topo_child (receive_from_father_comp, OPC_OBJTYPE_RARXCH , 0); 
				channel_of_recvive_from_children = op_topo_child (receive_from_children_comp, OPC_OBJTYPE_RARXCH , 0); 
				channel_of_send_to_father = op_topo_child (sender_to_father_comp, OPC_OBJTYPE_RATXCH , 0); 
				channel_of_send_to_children = op_topo_child (sender_to_children_comp, OPC_OBJTYPE_RATXCH , 0); 
				/*if(my_node_id == 0)
					{
					op_ima_obj_attr_get (channel_of_send_to_children,	"min frequency",	&k);
					printf("node %d min frequency = %d\n\n",my_node_id,k);
					op_ima_obj_attr_get (sender_to_children,	"name",	&nam);
					op_ima_obj_attr_set (channel_of_send_to_children,	"min frequency",	500);
					op_ima_obj_attr_get (channel_of_send_to_children,	"min frequency",	&k);
					printf("node %d min frequency = %d\n\n",my_node_id,k);
					printf("name = %s\n\n",nam);
				}
				if(my_node_id == 1111)
					{
					op_ima_obj_attr_get (channel_of_recvive_from_father,	"min frequency",	&k);
					printf("node %d min frequency = %d\n\n",my_node_id,k);
				
					op_ima_obj_attr_set (channel_of_recvive_from_father,	"min frequency",	500);
					op_ima_obj_attr_get (channel_of_recvive_from_father,	"min frequency",	&k);
					printf("node %d min frequency = %d\n\n",my_node_id,k);
				}
				*/
				
					
				printf("\n!!!Node %d Start at %lf!!!.\n",my_node_id, op_sim_time());
				op_intrpt_schedule_self(op_sim_time(), Start_Code);	
				
				
				
				
				
				contr_send_num=op_stat_reg("group1.pk_contr_number",OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				sendNum = 0;
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (1,"aloha_dispatch922")


			/** state (init) exit executives **/
			FSM_STATE_EXIT_UNFORCED (0, "init", "aloha_dispatch922 [init exit execs]")
				FSM_PROFILE_SECTION_IN ("aloha_dispatch922 [init exit execs]", state0_exit_exec)
				{
				/*++++++++++test aloha++++++++++*/
				/*op_ima_sim_attr_get_int32 ("max packet count",&max_packet_count); 
				
				
				if (my_node_id == 20)
					{
					rcvd_pkts = 0; 
				
					subm_pkts = 0; 
					}
				
				
					op_ima_obj_attr_set (channel_of_send_to_children,	"min frequency",	1000);
					op_ima_obj_attr_set (channel_of_send_to_father,	"min frequency",	50);
					op_ima_obj_attr_set (channel_of_recvive_from_father,	"min frequency",	1000);
					if(my_node_id == 20)
					op_ima_obj_attr_set (channel_of_recvive_from_children,	"min frequency",	50);
				
				*/
				}
				FSM_PROFILE_SECTION_OUT (state0_exit_exec)


			/** state (init) transition processing **/
			FSM_TRANSIT_ONLY ((Self_Start), 4, state4_enter_exec, ;, init, "Self_Start", "", "init", "Init", "tr_0", "aloha_dispatch922 [init -> Init : Self_Start / ]")
				/*---------------------------------------------------------*/



			/** state (idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "idle", state1_enter_exec, "aloha_dispatch922 [idle enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"aloha_dispatch922")


			/** state (idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "idle", "aloha_dispatch922 [idle exit execs]")
				FSM_PROFILE_SECTION_IN ("aloha_dispatch922 [idle exit execs]", state1_exit_exec)
				{
				//printf("beacon=%d\n",Send_Beacon);
				//printf("msg=%d\n",Send_Msg);
				//printf("type=%d\n",op_intrpt_type());
				//printf("code=%d\n",op_intrpt_code());
				}
				FSM_PROFILE_SECTION_OUT (state1_exit_exec)


			/** state (idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("aloha_dispatch922 [idle trans conditions]", state1_trans_conds)
			FSM_INIT_COND (App_Msg)
			FSM_TEST_COND (Start_Tpsn)
			FSM_TEST_COND (Msg_From_Up)
			FSM_TEST_COND (Msg_From_Down)
			FSM_TEST_COND (New_Up)
			FSM_TEST_COND (New_Frame)
			FSM_TEST_COND (New_Slot)
			FSM_TEST_COND (Join_Net)
			FSM_TEST_COND (App_Msg_Down)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("idle")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 5, state5_enter_exec, ;, "App_Msg", "", "idle", "App_Msg_Up", "tr_3", "aloha_dispatch922 [idle -> App_Msg_Up : App_Msg / ]")
				FSM_CASE_TRANSIT (1, 3, state3_enter_exec, ;, "Start_Tpsn", "", "idle", "beacon proc", "tr_5", "aloha_dispatch922 [idle -> beacon proc : Start_Tpsn / ]")
				FSM_CASE_TRANSIT (2, 2, state2_enter_exec, ;, "Msg_From_Up", "", "idle", "msg from up", "tr_7", "aloha_dispatch922 [idle -> msg from up : Msg_From_Up / ]")
				FSM_CASE_TRANSIT (3, 6, state6_enter_exec, ;, "Msg_From_Down", "", "idle", "msg from down", "tr_15", "aloha_dispatch922 [idle -> msg from down : Msg_From_Down / ]")
				FSM_CASE_TRANSIT (4, 8, state8_enter_exec, ;, "New_Up", "", "idle", "up intrpt", "tr_19", "aloha_dispatch922 [idle -> up intrpt : New_Up / ]")
				FSM_CASE_TRANSIT (5, 9, state9_enter_exec, ;, "New_Frame", "", "idle", "frame intrpt", "tr_21", "aloha_dispatch922 [idle -> frame intrpt : New_Frame / ]")
				FSM_CASE_TRANSIT (6, 7, state7_enter_exec, ;, "New_Slot", "", "idle", "slot_intrpt", "tr_23", "aloha_dispatch922 [idle -> slot_intrpt : New_Slot / ]")
				FSM_CASE_TRANSIT (7, 10, state10_enter_exec, ;, "Join_Net", "", "idle", "set channel", "tr_25", "aloha_dispatch922 [idle -> set channel : Join_Net / ]")
				FSM_CASE_TRANSIT (8, 11, state11_enter_exec, ;, "App_Msg_Down", "", "idle", "App_Msg_Down", "tr_29", "aloha_dispatch922 [idle -> App_Msg_Down : App_Msg_Down / ]")
				FSM_CASE_TRANSIT (9, 1, state1_enter_exec, ;, "default", "", "idle", "idle", "tr_18", "aloha_dispatch922 [idle -> idle : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (msg from up) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "msg from up", state2_enter_exec, "aloha_dispatch922 [msg from up enter execs]")
				FSM_PROFILE_SECTION_IN ("aloha_dispatch922 [msg from up enter execs]", state2_enter_exec)
				{
				int framecontrol;
				int type;
				Packet* pk;
				int seq;
				int AR;
				int source;
				int dest;
				int	dest_mode;
				int source_mode;
				//int n;
				int i = 0;
				int flag = 0;
				
				int next;
				//receice a packet from father node
				if((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == PHY_MAC2))
					{
					pk = op_pk_get(PHY_MAC2);
					//printf("---------------------------------------------------------cve_power = %.16lf\n",op_td_get_dbl (pk, OPC_TDA_RA_RCVD_POWER) );
					op_pk_fd_get (pk, 4, &source);
					if(source == my_node_id)
						{
						op_pk_destroy(pk);
						printf("From father receive a packet from my node!!!Dropped it !!!!!!!!!\n");
						}
					else
						{
						printf("\tNODE %d reveive a packet from up!!!\n",my_node_id);
						op_pk_fd_get (pk, 0, &framecontrol) ;
						op_pk_fd_get (pk, 1, &seq) ;
						op_pk_fd_get (pk, 3, &dest);
						printf("seq = %d\t",seq);
						printf("source = %d\t",source);
						printf("dest = %d\t",dest);
						AR = (framecontrol>>10)&1;
						printf("ar= %d\n",AR);
						dest_mode = (framecontrol>>4)&3;
						printf("dest mode = %d\t",dest_mode);
						source_mode = framecontrol&3;
						printf("source_mode = %d\t",source_mode);
						type = (framecontrol>>13)&7;
						printf("type= %d\n",type);
						if(type == 0)
							{									//beacon frame ,
							if(synch_flag == 0)
								{	
								//δͬ��,����ͬ����������ʱ������ʱ϶�ж�
				//				op_intrpt_schedule_self(op_sim_time()+(5-0.73439), Tpsn_Code);
								}
							beacon_proc(pk);		
							op_pk_destroy(pk);
							}
						else if(type == 2)
							{
							printf("node %d receive an ack packet from up!!!\n",my_node_id);
							if((dest_mode == 3)&&(source_mode == 2))
								{
								if(dest == my_node_id)
									{
									printf("node %d receive an ack packet and process it!!!\n",my_node_id);
									ACK1_proc(seq,source,dest);		
									op_pk_destroy(pk);
									}
								else
									{
									op_pk_destroy(pk);
									}
								}
							else if((dest_mode == 3)&&(source_mode == 1))
								{
								if(dest == my_node_id)
									{
									printf("node %d receive an ack packet and process it!!!\n",my_node_id);
									ACK1_proc(seq,source,dest);		
									op_pk_destroy(pk);
									}
								else
									{
									op_pk_destroy(pk);
									}
								}
							else if((dest_mode == 2)&&(source_mode == 2))			
								{
								if(dest == my_node_address)
									{
									printf("node %d receive an ack packet and process it!!!\n",my_node_id);
									ACK1_proc(seq,source,dest);		
									op_pk_destroy(pk);
									}
								else
									{	
									if(nextHop(dest) == 0)
										{
										printf("destroy!!!!\n");
										op_pk_destroy(pk);
										}
									else
										{								//ACKframe ,
										printf("node %d receive an ack packet and process it!!!\n",my_node_id);
										ACK1_proc(seq,source,dest);		
										op_pk_destroy(pk);
										}
									}				
								}
							else if((dest_mode == 2)&&(source_mode == 1))			
								{
								if(dest == my_node_address)
									{
									printf("node %d receive an ack packet and process it!!!\n",my_node_id);
									ACK1_proc(seq,source,dest);		
									op_pk_destroy(pk);
									}
								else
									{	
									if(nextHop(dest) == 0)
										{
										printf("destroy!!!!\n");
										op_pk_destroy(pk);
										}
									else
										{								//ACKframe ,
										printf("node %d receive an ack packet and process it!!!\n",my_node_id);
										ACK1_proc(seq,source,dest);		
										op_pk_destroy(pk);
										}
									}				
								}		
							else
								{
								op_pk_destroy(pk);
								}
							}
						else if(type == 1||type == 3)
							{	//data frame
							if((seq>5)&&(neighbor_table[0].seq>=seq))
								{
								printf("neighbor_table[0].seq = %d ;seq =%d\n",neighbor_table[0].seq,seq);
								op_pk_destroy(pk);
								}
							else
								{			
							if((dest_mode == 2)&&(source_mode == 2))
								{
								/*if((seq>5)&&(neighbor_table[0].seq>=seq))
									{
									op_pk_destroy(pk);
									}
								else*/
									{
									neighbor_table[0].seq = seq;
									//��·�ɱ�
									if(dest == my_node_address)
										{
										if(AR ==1)
											{
											printf("\tNODE %d create an ACK1 packet!!!\n",my_node_id);
											create_ack( pk, 1);
											}
										if(type == 1)
										op_pk_send_delayed (pk, MAC_NWK,1);
										}
									else
										{
										next = nextHop(dest);
										if(next!=0)
											{
											if(AR ==1)
												{
												printf("\tNODE %d create an ACK packet!!!\n",my_node_id);
												create_ack( pk,1);
												}
											if(type == 1)
											op_pk_send_delayed (pk, MAC_NWK,1);
											}
										else
											{
											op_pk_destroy(pk);
											}
										}
									}
								}
							else if((dest_mode == 2)&&(source_mode == 1))
								{
								/*if((seq>5)&&(neighbor_table[0].seq>=seq))
									{
									printf("seq = %d neighbor_table[0].seq = %d\n ",seq,neighbor_table[0].seq);
				
									op_pk_destroy(pk);
									}
								else*/
									{
									neighbor_table[0].seq = seq;
									if(dest == my_node_address)
										{
										if(AR ==1)
											{
											printf("\tNODE %d create an ACK2 packet!!!\n",my_node_id);
											create_ack( pk, 2);
											}
										if(type == 1)
										op_pk_send_delayed (pk, MAC_NWK,1);
										}
									else
										{
										next = nextHop(dest);
										if(next!=0)
											{
											if(AR ==1)
												{
												printf("\tNODE %d create an ACK packet!!!\n",my_node_id);
												create_ack( pk, 2);
												}
											if(type == 1)
											op_pk_send_delayed (pk, MAC_NWK,1);
											}
										else
											{
											op_pk_destroy(pk);
											}
										}
									}				
								}			
							else if((dest_mode == 3)&&(source_mode == 2))
								{
								neighbor_table[0].seq = seq;
								if(my_node_id == dest)
									{
									if(AR ==1)
										{
										printf("\tNODE %d create an ACK3 packet!!!\n",my_node_id);
										create_ack( pk, 3);
										}
									if(type == 1)
									op_pk_send_delayed (pk, MAC_NWK,1);
									}
								else
									{
									op_pk_destroy(pk);
									}
								}
							else if((dest_mode == 3)&&(source_mode == 1))
								{
								neighbor_table[0].seq = seq;
								if(my_node_id == dest)
									{
									if(AR ==1)
										{
										printf("\tNODE %d create an ACK4 packet!!!\n",my_node_id);
										create_ack( pk, 4);
										}
									if(type == 1)
									op_pk_send_delayed (pk, MAC_NWK,1);
									}
								else
									{
									op_pk_destroy(pk);
									}
								}
							}
							}
						else
							{
							op_pk_destroy(pk);
							}
						}
					}
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (msg from up) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "msg from up", "aloha_dispatch922 [msg from up exit execs]")


			/** state (msg from up) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "msg from up", "idle", "tr_8", "aloha_dispatch922 [msg from up -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (beacon proc) enter executives **/
			FSM_STATE_ENTER_FORCED (3, "beacon proc", state3_enter_exec, "aloha_dispatch922 [beacon proc enter execs]")

			/** state (beacon proc) exit executives **/
			FSM_STATE_EXIT_FORCED (3, "beacon proc", "aloha_dispatch922 [beacon proc exit execs]")
				FSM_PROFILE_SECTION_IN ("aloha_dispatch922 [beacon proc exit execs]", state3_exit_exec)
				{
				synch_flag=1;												//ͬ����ɱ�־
				op_intrpt_schedule_self(op_sim_time() ,		Frame_Code);	//֡���ж�
				op_intrpt_schedule_self(op_sim_time() ,		Slot_Code);		//ʱ϶���ж�
				printf("node %d slot start at %f",my_node_id,op_sim_time());
				}
				FSM_PROFILE_SECTION_OUT (state3_exit_exec)


			/** state (beacon proc) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "beacon proc", "idle", "tr_6", "aloha_dispatch922 [beacon proc -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (Init) enter executives **/
			FSM_STATE_ENTER_FORCED (4, "Init", state4_enter_exec, "aloha_dispatch922 [Init enter execs]")
				FSM_PROFILE_SECTION_IN ("aloha_dispatch922 [Init enter execs]", state4_enter_exec)
				{
				int 	 i;
				int                 new_seed = my_node_id;
				PrgT_Random_Gen    	*my_rng;
				
				double asd;
				
				
				//��ʼ��ͳ����***************************************
				pkt = 0;
				frame	= 0;
				slot	= 0;
				Active_from_up = 0;
				mincost 	= 10;
				maxcost		= 10;
				no		= 0;
				no2		= 0;
				
				
				NET = 0;//�ڵ㿪��ʱ������
				for(i = 0;i < 5;i++)
				{
				ack_time[i] = 100;
				ack_time2[i] = 100;
				}
				/*
				for(i = 0;i < 5;i++)
				{
				retrans[i] = 100;
				retrans_down[i] = 100;
				}
				*/
				for(i = 0;i < 5;i++)
				{
				retransUp[i].time = 100;
				retransUp[i].count = 0;
				retransDown[i].time = 100;
				retransDown[i].count = 0;
				}
				
				
				
				
				//Ǳ�ڸ��ڵ���ʼ��---------------------------------
				for (i=0;i<FATHER_NODE_NUM;i++)
					{
					father_table[i].short_addr = 0;
					father_table[i].band = 0;
					father_table[i].depth = 0;
					father_table[i].frequency = 0;
					father_table[i].router_cost = 10;
					father_table[i].times = 0;
					}
				//�ڽڵ����кű��ʼ��------------------------------
				for(i = 0;i < 21;i++)
					{
					neighbor_table[i].node_id = 0;
					neighbor_table[i].seq = 0;
					neighbor_table[i].node_address = 0;
					}
				
				
				
				
				
				//���г�ʼ��-----------------------------------------
				
				/* Clear SubQ */
				op_subq_flush(Data_Queue);
				op_subq_flush(Retrans_Queue);
				op_subq_flush(Data_Queue_Down);
				op_subq_flush(Retrans_Queue_Down);
				op_subq_flush(ACK_Queue_Down);
				op_subq_flush(ACK_Queue);
				
				/*
				for (i = 0; i < QUEUE_NUM; i++)								//��ʼ�������еİ���
				{
					pkt_num_in_queue[i] = 0;
				}
				*/
				
				//���Ľڵ�Ϊͬ����׼�ڵ㣬��ͬ��---------------------
				if(my_node_type == 0){										//���Ľڵ㣬ֱ�ӽ���NET	
				//	op_ima_obj_attr_set (my_node_objid,	"ShortAddress"	,	CEN_NODE);
					my_node_address = CEN_NODE;
					synch_flag=1;                                      		//ͬ�����
					NET = 1;												//���ؽڵ�ֱ������
					Scan_Flag = 0;											//���ؽڵ㲻��Ҫɨ���ŵ�
					my_depth = 0;
					op_ima_obj_attr_set (channel_of_send_to_children		,"min frequency",470.0);
				//	printf("Node %d channel_of_send_to_children = %d\n",my_node_id,channel_of_send_to_children);
					op_ima_obj_attr_get (channel_of_send_to_children		,"min frequency",&asd);	
				//	printf("Node %d channel_of_send_to_children = %f\n",my_node_id,asd);
					op_ima_obj_attr_set (channel_of_recvive_from_children	,"min frequency",471.0);
				
				}
				
				if(my_node_type != 0){
					synch_flag=0;											//�����ؽڵ�
					NET = 0;												//�ֽ׶νڵ㿪��������ʱ϶������Ҫͬ������
					Scan_Flag = 1;											//��Ҫɨ��
					op_ima_obj_attr_set (channel_of_recvive_from_father		,"min frequency",470.0);
					op_ima_obj_attr_get (channel_of_recvive_from_father		,"min frequency",&asd);	
					op_ima_obj_attr_set (channel_of_send_to_father			,"min frequency",471.0);
				}															//�����ܽڵ㣬����������
				
				
				
				/*test for send msg to father node*/
				/*
				if(my_node_id == 1111){
					NET = 1;
					printf("node%d is in net",my_node_id);
				}
				*/
				
				
				//��ʼ��MAC�����к�
				/* create a new random number generator */
				my_rng = op_prg_random_gen_create (new_seed);
				
				/* generate a random integer in the interval [0,255] */
				MAC_sequence = (op_prg_random_integer_gen (my_rng) % 256) ;
				
				/* destroy the random number generator */
				op_prg_random_gen_destroy (my_rng); 
				
				printf("Node %d Init start(MAC address)\n",my_node_id);
				printf("Node %d MAC_sequence == %d\n",my_node_id,MAC_sequence);
				
				
				
				
				
				
				//if(my_node_id==CEN_NODE)
				//	{
					op_intrpt_schedule_self(op_sim_time() + Frame_Time, Frame_Code);
					op_intrpt_schedule_self(op_sim_time() + Slot_Time, Slot_Code);
				
				//	}
				}
				FSM_PROFILE_SECTION_OUT (state4_enter_exec)

			/** state (Init) exit executives **/
			FSM_STATE_EXIT_FORCED (4, "Init", "aloha_dispatch922 [Init exit execs]")


			/** state (Init) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "Init", "idle", "tr_2", "aloha_dispatch922 [Init -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (App_Msg_Up) enter executives **/
			FSM_STATE_ENTER_FORCED (5, "App_Msg_Up", state5_enter_exec, "aloha_dispatch922 [App_Msg_Up enter execs]")
				FSM_PROFILE_SECTION_IN ("aloha_dispatch922 [App_Msg_Up enter execs]", state5_enter_exec)
				{
				/* recv app data, save in subqueue */
				
				int dest = -1;
				int type = -1;
				int id = -1;
				//	int i;
				int subtype;
				Packet* pk;
				int framecontrol;
				int nwk_head;
				int seq;
				
				if((NET == 1)||(NET == 0)||(NET == 4))
					{
					pk = op_pk_get(APP_MAC);
					MAC_sequence++;
					if(MAC_sequence >= 256)
						{
						MAC_sequence = 0;
						}
					seq = MAC_sequence;
					op_pk_fd_get(pk,0, &framecontrol);
					op_pk_fd_get(pk,3, &dest);
					op_pk_fd_get(pk,5, &nwk_head);
				
					op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,seq,8);
					printf("seq = %d\n",seq);
				
					subtype = (nwk_head>>20)&15;
				
					type = (framecontrol>>13)&7;
					printf("type = %d\n",type);
					printf("subtype = %d\n",subtype);
					printf("dest = %d\n",dest);
					if (op_subq_pk_insert(Data_Queue, pk, OPC_QPOS_TAIL) != OPC_QINS_OK)
						{
						printf("\n###Node %d:::data package from route insert into queue failed###\n", my_node_id);
						op_pk_destroy(pk);
						}
					printf("node %d MAC receive a frame dest is father node!!!!\n",my_node_id);
					}
				}
				FSM_PROFILE_SECTION_OUT (state5_enter_exec)

			/** state (App_Msg_Up) exit executives **/
			FSM_STATE_EXIT_FORCED (5, "App_Msg_Up", "aloha_dispatch922 [App_Msg_Up exit execs]")


			/** state (App_Msg_Up) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "App_Msg_Up", "idle", "tr_4", "aloha_dispatch922 [App_Msg_Up -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (msg from down) enter executives **/
			FSM_STATE_ENTER_FORCED (6, "msg from down", state6_enter_exec, "aloha_dispatch922 [msg from down enter execs]")
				FSM_PROFILE_SECTION_IN ("aloha_dispatch922 [msg from down enter execs]", state6_enter_exec)
				{
				int framecontrol;
				int type;
				Packet* pk;
				int seq;
				int AR;
				int source;
				int dest;
				int dest_mode;
				int source_mode;
				//double snr;
				int n;
				int i = 0;
				int flag = 0;
				
				//receice a packet from children node
				
				
				
				if((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == PHY_MAC0))
				{
				
				if((my_node_type == 0)||(my_node_type == 1))
					{
					pk = op_pk_get(PHY_MAC0);
					//printf("---------------------------------------------------------cve_power = %.16lf\n",op_td_get_dbl (pk, OPC_TDA_RA_RCVD_POWER) );
					op_pk_fd_get (pk, 4, &source);
					if(source == my_node_address)
						{
						op_pk_destroy(pk);
						printf("From children receive a packet from my node!!!Dropped it !!!!!!!!!\n");
						}
					else
						{
						printf("\tNODE %d reveive a packet from down!!!\n",my_node_id);
						op_pk_fd_get (pk, 0, &framecontrol) ;
						op_pk_fd_get (pk, 1, &seq) ;
						op_pk_fd_get (pk, 3, &dest);
						printf("seq = %d\n",seq);
						printf("source = %d\n",source);
						printf("dest = %d\n",dest);
						AR = (framecontrol>>10)&1;
						printf("ar= %d\n",AR);
						dest_mode = (framecontrol>>4)&3;
						printf("dest mode = %d\n",dest_mode);
						source_mode = framecontrol&3;
						printf("source_mode = %d\n",source_mode);
						type = (framecontrol>>13)&7;
						printf("type= %d\n",type);
						
						Keep_Alive(source_mode,source);
						if(type == 2)							//ack frame
							{
							printf("node %d receive an ack packet from down!!!\n",my_node_id);
							if((dest_mode == 2)&&(source_mode == 2))
								{
								if(dest == my_node_address)
									{
									printf("node %d receive an ack packet and process it!!!\n",my_node_id);
									ACK2_proc(seq,source,dest);		
									op_pk_destroy(pk);
									}
								else
									{
									if(nextHop(dest) == 0)
										{
										printf("destroy!!!!\n");
										op_pk_destroy(pk);
										}
									else
										{								//ACKframe ,
										printf("node %d receive an ack packet and process it!!!\n",my_node_id);
										ACK2_proc(seq,source,dest);		
										op_pk_destroy(pk);
										}
									}				
								}
							else if((dest_mode == 1)&&(source_mode == 2))
								{
								if(my_node_address == CEN_NODE)
									{
									printf("node %d receive an ack packet and process it!!!\n",my_node_id);
									ACK2_proc(seq,source,dest);		
									op_pk_destroy(pk);
									}
								else
									{
									printf("node %d receive an ack packet and process it!!!\n",my_node_id);
									ACK2_proc(seq,source,dest);		
									op_pk_destroy(pk);
									}				
								}	
							else if((dest_mode == 1)&&(source_mode == 3))
								{
								if(my_node_address == CEN_NODE)
									{
									printf("node %d receive an ack packet and process it!!!\n",my_node_id);
									ACK2_proc(seq,source,dest);		
									op_pk_destroy(pk);					
									}
								else
									{
									printf("node %d receive an ack packet and process it!!!\n",my_node_id);
									ACK2_proc(seq,source,dest);		
									op_pk_destroy(pk);
									}			
								}
							else if((dest_mode == 2)&&(source_mode == 3))
								{
								if(dest == my_node_address)
									{
									printf("node %d receive an ack packet and process it!!!\n",my_node_id);
									ACK2_proc(seq,source,dest);		
									op_pk_destroy(pk);
									}
								else
									{
									if(nextHop(dest) == 0)
										{
										printf("destroy!!!!\n");
										op_pk_destroy(pk);
										}
									else
										{								//ACKframe ,
										printf("node %d receive an ack packet and process it!!!\n",my_node_id);
										ACK2_proc(seq,source,dest);		
										op_pk_destroy(pk);
										}
									}				
								}
							else
								{
								op_pk_destroy(pk);
								}
							}
						else if(type == 1)						//data frame
							{
							if((dest_mode == 2)&&(source_mode == 2))
								{
								for(n = 20;n >=1;n--)
									{
									if(neighbor_table[n].node_address == nextHop(source))
										{
										flag = 1;						
										break;
										}
									if(neighbor_table[n].node_address == 0)
										{
										i = n;
										}
									}
								
								
								if((seq>=5)&&(neighbor_table[n].seq>=seq)&&(flag == 1))
									{
									op_pk_destroy(pk);//----------------------------
									}
								else
									{				
									if(AR == 1)
										{
										printf("\tNODE %d create an ACK5 packet!!!\n",my_node_id);
										create_ack(pk, 5);
										}
									op_pk_send_delayed (pk, MAC_NWK,1);
									}
								
								if(flag == 0)
									{
									neighbor_table[i].node_address = nextHop(source);
									neighbor_table[i].seq = seq;
									}
								else
									{
									neighbor_table[n].seq = seq;
									}
								
								}
							else if((dest_mode == 1)&&(source_mode == 2))
								{
								for(n = 20;n >=1;n--)
									{
									if(neighbor_table[n].node_address == nextHop(source))
										{
										flag = 1;
										break;
										}
									if(neighbor_table[n].node_address == 0)
										{
										i = n;
										}
									}
								printf("nextHop(source)= %d\n",nextHop(source));
								printf("\t\tdaozhele\n");
								if((seq>=5)&&(neighbor_table[n].seq>=seq)&&(flag == 1))
									{
									printf("seq = %d neighbor_table[n].seq = %d\n ",seq,neighbor_table[n].seq);
									op_pk_destroy(pk);//-----------------------------
									}
								else
									{	
									if(AR == 1)
										{
										printf("\tNODE %d create an ACK7 packet!!!\n",my_node_id);
										create_ack(pk, 7);
										}
									op_pk_send_delayed (pk, MAC_NWK,1);
									}
								if(flag == 0)
									{
									neighbor_table[i].node_address = nextHop(source);
									neighbor_table[i].seq = seq;
									}
								else
									{
									neighbor_table[n].seq = seq;
									}
								}
							else if((dest_mode == 1)&&(source_mode == 3))
								{
								if(AR == 1)
									{
									printf("\tNODE %d create an ACK8 packet!!!\n",my_node_id);
									create_ack(pk, 8);
									}
								op_pk_send_delayed (pk, MAC_NWK,1);
								}
							else if((dest_mode == 2)&&(source_mode == 3))
								{
								if(AR == 1)
									{
									printf("\tNODE %d create an ACK6 packet!!!\n",my_node_id);
									create_ack(pk, 6);
									}
								op_pk_send_delayed (pk, MAC_NWK,1);
								}
							}
						else
							{
							op_pk_destroy(pk);
							}
						}
					}
				/*
				else			//test for slot aloha
					{
					pk = op_pk_get(PHY_MAC0);
					op_pk_fd_get (pk, 4, &source);
					op_pk_fd_get (pk, 1, &seq);
				
					if(source != my_node_id)
						{
						n = op_td_get_int (pk, OPC_TDA_RA_NUM_COLLS);
						snr = op_td_get_dbl (pk, OPC_TDA_RA_SNR);
								++rcvd_pkts;	
						printf("+++++++++++++++++++++snr+%f\n\n\n",snr);
						printf("+++++++++++++++++++++n+%d\n\n\n",n);
						printf("++++++++++++++++++++++%d  time is %f\n\n\n",rcvd_pkts,op_sim_time() );
						printf("++++++++++++++++++++++%d\n\n\n",seq );
						printf("++++++++++++++++++++++%d\n\n\n",rcvd_pkts );
						}
					op_pk_destroy(pk);
					}
				*/
				}
				}
				FSM_PROFILE_SECTION_OUT (state6_enter_exec)

			/** state (msg from down) exit executives **/
			FSM_STATE_EXIT_FORCED (6, "msg from down", "aloha_dispatch922 [msg from down exit execs]")


			/** state (msg from down) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "msg from down", "idle", "tr_16", "aloha_dispatch922 [msg from down -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (slot_intrpt) enter executives **/
			FSM_STATE_ENTER_FORCED (7, "slot_intrpt", state7_enter_exec, "aloha_dispatch922 [slot_intrpt enter execs]")
				FSM_PROFILE_SECTION_IN ("aloha_dispatch922 [slot_intrpt enter execs]", state7_enter_exec)
				{
				int i;
				
				
				//int 	framecontrol;
				//int		sequence;
				//int		panid;
				//int		dest;
				//int		source;
				//int		nwkhead;
				
				
				
				
				//new slot start interupt--------------------------------------------------------------------
				if ((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Slot_Code))
				{
				
				//test ***********************************************************************
				/*
				if(my_node_id == 1111)
					{
					
					if(pkt<4)
						{
						pkptr = op_pk_create(100);
						framecontrol = 9216;
						sequence = MAC_sequence++;
						panid = 123;
						dest = 0;
						source = my_node_id;
						nwkhead = 0;
						
						op_pk_fd_set (pkptr, 0, OPC_FIELD_TYPE_INTEGER,framecontrol,16) ;
						op_pk_fd_set (pkptr, 1, OPC_FIELD_TYPE_INTEGER,sequence,8) ;
						op_pk_fd_set (pkptr, 2, OPC_FIELD_TYPE_INTEGER,panid,16) ;
						op_pk_fd_set (pkptr, 3, OPC_FIELD_TYPE_INTEGER,dest,16) ;
						op_pk_fd_set (pkptr, 4, OPC_FIELD_TYPE_INTEGER,source,16) ;
						op_pk_fd_set (pkptr, 5, OPC_FIELD_TYPE_INTEGER,source,16) ;
						if (op_subq_pk_insert(Data_Queue, pkptr, OPC_QPOS_TAIL) != OPC_QINS_OK)
							{
							printf("\n###Node %d:::data package from route insert into queue failed###\n", my_node_id);
							op_pk_destroy(pkptr);
							}
						printf("j = %d\n\n\n\n",pkt);
						pkt++;
						}
					}
				
				
				
				*/
				
				
				
				
				
					//op_intrpt_schedule_self(op_sim_time() + Down_Time, Up_Code);
				
				slot++;
				if (slot == Max_Slot_Num)
					{
						slot = 1;
					}
				if((my_node_type == 0)||(my_node_type == 1))
				{
					if (slot == 1)
					{
				//		printf("\n###Node %d,****BEACON SLOT**** slot number is %d.###\n", my_node_id, slot);
						if(NET == 1){
						//creat a beacon frame and send it
						Make_Beacon();
						}
					}
					else
					{
				//		printf("\n###Node %d,****DATA SLOT**** slot number is %d.###\n", my_node_id, slot);
						if(NET != 0){
						//cheak msg queue to children node and send it
						send_msg_to_children();
						}
					}
				}
					
					if((NET == 2)||(NET == 3)||(NET == 4))
					{
						Leave_NET();	//�������Ƿ�Ϊ��,����Ϊ�վ������շ��Ż�״̬	
					}
					
					
					
						for(i = 0;i < 5;i++)
						{
						if(ack_time[i]<=3)
							{
							ack_time[i]--;
							}
						if(ack_time2[i]<=3)
							{
							ack_time2[i]--;
							}
						/*
						if(retrans[i]<=10)
							{
							retrans[i]--;
							}
						if(retrans_down[i]<=10)
							{
							retrans_down[i]--;
							}*/
						if(retransUp[i].time<=10)
							{
							retransUp[i].time--;
							}
						if(retransDown[i].time<=10)
							{
							retransDown[i].time--;
							}
						
						}
				/*	
					if(my_node_type == CEN_NODE)
						{
						for(i = 0;i<5;i++)
							{
							printf("NODE %d ###retrans[%d] = %d\n",my_node_id,i,retrans[i]);
							}
						for(i = 0;i<5;i++)
							{
							printf("NODE %d ###ACK_time2[%d] = %d\n",my_node_id,i,ack_time2[i]);
							}
						}
					if(my_node_id == 1111)
						{
						for(i = 0;i<5;i++)
							{
							printf("NODE %d ###ACK_time[%d] = %d\n",my_node_id,i,ack_time[i]);
							}
						for(i = 0;i<5;i++)
							{
							printf("NODE %d ###retrans[%d] = %d\n",my_node_id,i,retrans[i]);
							}
						}
				*/	
				
				
					//make a new slot interupt
					op_intrpt_schedule_self(op_sim_time() + Slot_Time, Slot_Code);
					//make a interupt of sending msgs to father node
					op_intrpt_schedule_self(op_sim_time() + Down_Time, Up_Code);
					
					
						
				}	
				}
				FSM_PROFILE_SECTION_OUT (state7_enter_exec)

			/** state (slot_intrpt) exit executives **/
			FSM_STATE_EXIT_FORCED (7, "slot_intrpt", "aloha_dispatch922 [slot_intrpt exit execs]")


			/** state (slot_intrpt) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "slot_intrpt", "idle", "tr_24", "aloha_dispatch922 [slot_intrpt -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (up intrpt) enter executives **/
			FSM_STATE_ENTER_FORCED (8, "up intrpt", state8_enter_exec, "aloha_dispatch922 [up intrpt enter execs]")
				FSM_PROFILE_SECTION_IN ("aloha_dispatch922 [up intrpt enter execs]", state8_enter_exec)
				{
				//�ڵ㲻����Ҳ���Ը����ڵ㷢����Ϣ
				//interupt of sending msg to up node interupt
				if((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Up_Code))
					{
					if(1 == 1)
						{
						send_msg_to_father();	
						}
				/*	test for aloha	*/
				/*
					if(my_node_id != 20)
						{
						//if(slot!=1)
						test();
						//printf("NODE %d send a packet %d!!!!!\n",my_node_id,subm_pkts);
						}
				*/
					}
				}
				FSM_PROFILE_SECTION_OUT (state8_enter_exec)

			/** state (up intrpt) exit executives **/
			FSM_STATE_EXIT_FORCED (8, "up intrpt", "aloha_dispatch922 [up intrpt exit execs]")


			/** state (up intrpt) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "up intrpt", "idle", "tr_20", "aloha_dispatch922 [up intrpt -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (frame intrpt) enter executives **/
			FSM_STATE_ENTER_FORCED (9, "frame intrpt", state9_enter_exec, "aloha_dispatch922 [frame intrpt enter execs]")
				FSM_PROFILE_SECTION_IN ("aloha_dispatch922 [frame intrpt enter execs]", state9_enter_exec)
				{
				//new frame start interupt-------------------------------------------------------------
				//printf("Node %d idle start!!!!!!\n",my_node_id);
				//printf("----------------------------------------------------\n");
				
				if ((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Frame_Code))
				{
				
					frame++;													//֡��+1
					if (frame == 1000)
						{
						frame = 0;
						}
					
					if(NET == 0)
						{
						if(Scan_Flag == 1)										//��Ҫɨ���ŵ�
							{
							if((frame %4) == 0)
								{
								change_band ();
								}
							}
						}
					else if (NET == 1)
						{
						Alive();
						Active_from_up--;
						if(Active_from_up == 0)
							{
							op_intrpt_schedule_remote(op_sim_time(), Lost_Father, 
								op_id_from_name (op_topo_parent(op_id_self()) , OPC_OBJTYPE_PROC, "NWK")) ;
							}
						}
					
					
					
				//-------------------------���Դ�ӡ�Ĵ��룬����ȥ��-----------------------//	
				//	printf("\n\n###Node %d:::Node %d, frame number is %d.###\n", my_node_id, my_node_id, frame);
					
					
					op_intrpt_schedule_self(op_sim_time() + Frame_Time, Frame_Code);
				
				}
				}
				FSM_PROFILE_SECTION_OUT (state9_enter_exec)

			/** state (frame intrpt) exit executives **/
			FSM_STATE_EXIT_FORCED (9, "frame intrpt", "aloha_dispatch922 [frame intrpt exit execs]")


			/** state (frame intrpt) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "frame intrpt", "idle", "tr_22", "aloha_dispatch922 [frame intrpt -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (set channel) enter executives **/
			FSM_STATE_ENTER_FORCED (10, "set channel", state10_enter_exec, "aloha_dispatch922 [set channel enter execs]")
				FSM_PROFILE_SECTION_IN ("aloha_dispatch922 [set channel enter execs]", state10_enter_exec)
				{
				int band;
				//int point;
				if(op_intrpt_code() == intrCode_Status)
					{
					op_ima_obj_attr_get (my_node_objid,	"NodeStatus", &NET);
					op_ima_obj_attr_get (my_node_objid,	"NodeBand",	&band);
					//op_ima_obj_attr_get (my_node_objid,	"NodePoint", &point);
					my_depth = father_table[no].depth+1;					//���ñ��ڵ���ȣ��յ�����ȷ���жϴ�����
					op_ima_obj_attr_get (my_node_objid,	"ShortAddress",	&my_node_address);
					printf("node %d depth is %d,short address is %d",my_node_id,my_depth,my_node_address);
					op_ima_obj_attr_set (channel_of_send_to_children		,"min frequency",BandTo(band));
					op_ima_obj_attr_set (channel_of_recvive_from_children	,"min frequency",PointTo(band,1));
					printf("node %d channel_of_recvive_from_children =%f\n",my_node_id,PointTo(band,1));
					}
				
				else if(op_intrpt_code() == intrCode_Rejoin	)
					{
					op_ima_obj_attr_get (my_node_objid,	"NodeStatus", &NET);
					
					}
				
				}
				FSM_PROFILE_SECTION_OUT (state10_enter_exec)

			/** state (set channel) exit executives **/
			FSM_STATE_EXIT_FORCED (10, "set channel", "aloha_dispatch922 [set channel exit execs]")


			/** state (set channel) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "set channel", "idle", "tr_26", "aloha_dispatch922 [set channel -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (App_Msg_Down) enter executives **/
			FSM_STATE_ENTER_FORCED (11, "App_Msg_Down", state11_enter_exec, "aloha_dispatch922 [App_Msg_Down enter execs]")
				FSM_PROFILE_SECTION_IN ("aloha_dispatch922 [App_Msg_Down enter execs]", state11_enter_exec)
				{
				/* recv app data, save in subqueue */
				
				int dest = -1;
				int type = -1;
				int id = -1;
				//	int i;
				int subtype;
				Packet* pk;
				int framecontrol;
				int nwk_head;
				int seq;
				
				if((my_node_type == 0)||(my_node_type == 1))
					{
					if((NET == 1))
						{
						pk = op_pk_get(APP_MAC_DOWN);
						MAC_sequence++;
						if(MAC_sequence >= 256)
							{
							MAC_sequence = 0;
							}
						seq = MAC_sequence;
						op_pk_fd_get(pk,0, &framecontrol);
						op_pk_fd_get(pk,3, &dest);
						op_pk_fd_get(pk,5, &nwk_head);
				
						op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,seq,8);
						printf("seq = %d\n",seq);
				
						subtype = (nwk_head>>20)&15;
				
						type = (framecontrol>>13)&7;
						printf("type = %d\n",type);
						printf("subtype = %d\n",subtype);
						printf("dest = %d\n",dest);
						if (op_subq_pk_insert(Data_Queue_Down, pk, OPC_QPOS_TAIL) != OPC_QINS_OK)
							{
							printf("\n###Node %d:::data package from route insert into queue failed###\n", my_node_id);
							op_pk_destroy(pk);
							}
						printf("node %d MAC receive a frame dest is children node!!!!\n",my_node_id);
						}
					}
				}
				FSM_PROFILE_SECTION_OUT (state11_enter_exec)

			/** state (App_Msg_Down) exit executives **/
			FSM_STATE_EXIT_FORCED (11, "App_Msg_Down", "aloha_dispatch922 [App_Msg_Down exit execs]")


			/** state (App_Msg_Down) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "App_Msg_Down", "idle", "tr_30", "aloha_dispatch922 [App_Msg_Down -> idle : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"aloha_dispatch922")
		}
	}




void
_op_aloha_dispatch922_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_aloha_dispatch922_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_aloha_dispatch922_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_aloha_dispatch922_svar function. */
#undef num_backoffs
#undef pkptr
#undef backoff_exponent
#undef backoff_evhandle
#undef max_backoffs
#undef aloha_init_performed
#undef parent_prohandle
#undef unit_backoff_period
#undef gbl_sent_stathandle
#undef gbl_mac_delay_stathandle
#undef sent_stathandle
#undef mac_delay_stathandle
#undef my_node_id
#undef my_node_objid
#undef my_process_objid
#undef subnet_objid
#undef frame
#undef slot
#undef synch_flag
#undef pkt_num_in_queue
#undef my_pan_id
#undef my_depth
#undef acccess_control
#undef MAC_sequence
#undef NET
#undef ack_time
#undef ack_time2
#undef receiver_from_father
#undef receiver_from_children
#undef sender_to_father
#undef sender_to_children
#undef channel_of_recvive_from_father
#undef channel_of_recvive_from_children
#undef channel_of_send_to_father
#undef channel_of_send_to_children
#undef pkt
#undef receive_from_father_comp
#undef receive_from_children_comp
#undef sender_to_father_comp
#undef sender_to_children_comp
#undef max_packet_count
#undef Active_from_up
#undef Scan_Flag
#undef my_node_address
#undef my_node_type
#undef no
#undef no2
#undef mincost
#undef maxcost
#undef father_table
#undef contr_send_num
#undef sendNum
#undef neighbor_table
#undef retransUp
#undef retransDown

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_aloha_dispatch922_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_aloha_dispatch922_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (aloha_dispatch922)",
		sizeof (aloha_dispatch922_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_aloha_dispatch922_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	aloha_dispatch922_state * ptr;
	FIN_MT (_op_aloha_dispatch922_alloc (obtype))

	ptr = (aloha_dispatch922_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "aloha_dispatch922 [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_aloha_dispatch922_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	aloha_dispatch922_state		*prs_ptr;

	FIN_MT (_op_aloha_dispatch922_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (aloha_dispatch922_state *)gen_ptr;

	if (strcmp ("num_backoffs" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->num_backoffs);
		FOUT
		}
	if (strcmp ("pkptr" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pkptr);
		FOUT
		}
	if (strcmp ("backoff_exponent" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->backoff_exponent);
		FOUT
		}
	if (strcmp ("backoff_evhandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->backoff_evhandle);
		FOUT
		}
	if (strcmp ("max_backoffs" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->max_backoffs);
		FOUT
		}
	if (strcmp ("aloha_init_performed" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->aloha_init_performed);
		FOUT
		}
	if (strcmp ("parent_prohandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->parent_prohandle);
		FOUT
		}
	if (strcmp ("unit_backoff_period" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->unit_backoff_period);
		FOUT
		}
	if (strcmp ("gbl_sent_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->gbl_sent_stathandle);
		FOUT
		}
	if (strcmp ("gbl_mac_delay_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->gbl_mac_delay_stathandle);
		FOUT
		}
	if (strcmp ("sent_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->sent_stathandle);
		FOUT
		}
	if (strcmp ("mac_delay_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->mac_delay_stathandle);
		FOUT
		}
	if (strcmp ("my_node_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->my_node_id);
		FOUT
		}
	if (strcmp ("my_node_objid" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->my_node_objid);
		FOUT
		}
	if (strcmp ("my_process_objid" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->my_process_objid);
		FOUT
		}
	if (strcmp ("subnet_objid" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->subnet_objid);
		FOUT
		}
	if (strcmp ("frame" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->frame);
		FOUT
		}
	if (strcmp ("slot" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->slot);
		FOUT
		}
	if (strcmp ("synch_flag" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->synch_flag);
		FOUT
		}
	if (strcmp ("pkt_num_in_queue" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->pkt_num_in_queue);
		FOUT
		}
	if (strcmp ("my_pan_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->my_pan_id);
		FOUT
		}
	if (strcmp ("my_depth" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->my_depth);
		FOUT
		}
	if (strcmp ("acccess_control" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->acccess_control);
		FOUT
		}
	if (strcmp ("MAC_sequence" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->MAC_sequence);
		FOUT
		}
	if (strcmp ("NET" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->NET);
		FOUT
		}
	if (strcmp ("ack_time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->ack_time);
		FOUT
		}
	if (strcmp ("ack_time2" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->ack_time2);
		FOUT
		}
	if (strcmp ("receiver_from_father" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->receiver_from_father);
		FOUT
		}
	if (strcmp ("receiver_from_children" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->receiver_from_children);
		FOUT
		}
	if (strcmp ("sender_to_father" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->sender_to_father);
		FOUT
		}
	if (strcmp ("sender_to_children" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->sender_to_children);
		FOUT
		}
	if (strcmp ("channel_of_recvive_from_father" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->channel_of_recvive_from_father);
		FOUT
		}
	if (strcmp ("channel_of_recvive_from_children" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->channel_of_recvive_from_children);
		FOUT
		}
	if (strcmp ("channel_of_send_to_father" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->channel_of_send_to_father);
		FOUT
		}
	if (strcmp ("channel_of_send_to_children" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->channel_of_send_to_children);
		FOUT
		}
	if (strcmp ("pkt" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pkt);
		FOUT
		}
	if (strcmp ("receive_from_father_comp" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->receive_from_father_comp);
		FOUT
		}
	if (strcmp ("receive_from_children_comp" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->receive_from_children_comp);
		FOUT
		}
	if (strcmp ("sender_to_father_comp" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->sender_to_father_comp);
		FOUT
		}
	if (strcmp ("sender_to_children_comp" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->sender_to_children_comp);
		FOUT
		}
	if (strcmp ("max_packet_count" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->max_packet_count);
		FOUT
		}
	if (strcmp ("Active_from_up" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Active_from_up);
		FOUT
		}
	if (strcmp ("Scan_Flag" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Scan_Flag);
		FOUT
		}
	if (strcmp ("my_node_address" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->my_node_address);
		FOUT
		}
	if (strcmp ("my_node_type" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->my_node_type);
		FOUT
		}
	if (strcmp ("no" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->no);
		FOUT
		}
	if (strcmp ("no2" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->no2);
		FOUT
		}
	if (strcmp ("mincost" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->mincost);
		FOUT
		}
	if (strcmp ("maxcost" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->maxcost);
		FOUT
		}
	if (strcmp ("father_table" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->father_table);
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
	if (strcmp ("neighbor_table" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->neighbor_table);
		FOUT
		}
	if (strcmp ("retransUp" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->retransUp);
		FOUT
		}
	if (strcmp ("retransDown" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->retransDown);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}


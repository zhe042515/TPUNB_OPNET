/* Process model C form file: WSN_MAC_DOWN2.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char WSN_MAC_DOWN2_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A modeler 7 6283B36C 6283B36C 1 DESKTOP-RD4S7T2 51133 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                    ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

//-----------------------*����ͷ�ļ�*-----------------------//
#include <math.h>

//-----------------------���峣��-----------------------//
#define		CEN_NODE				43690		//���Ľڵ�id
#define		Frame_Time				5			//һ��֡ʱ��
#define		Slot_Time				0.5			//һ��ʱ϶ʱ��
#define		Down_Time				0.01		//һ������ʱ϶ʱ��10ms
#define		Protection_time			0.03		//����������յ����а��������뿪������
#define		Wait_time				0.1			//�ȴ�������೤ʱ��δ�յ����а���ʼ������������
#define		Max_Slot_Num			10			//һ��֡ʱ϶��
#define		BroadCastAddress		0xffff		//�㲥��ַ
#define		GateAddress				0xaaaa		//���ص�ַ
#define		Max_Depth				8			//������
#define		Max_ACK					5			//����ش��ȴ�ʱ��
#define		BandWidth				4			//Ƶ�δ���
#define		MinBandWidth			470.0		//��СƵ�δ���
#define		MaxBandWidth			658.0		//���Ƶ�δ���
#define		PointWidth				1			//Ƶ�δ���
#define		Active_Time_Beacon		50			//�ű�֡����ʱ�䣬��λΪ֡(5s)
#define		ACTIVE_TIME				30*60*2		//�ӽڵ㱣��ʱ��
#define		RETRANS_TIME			6			//�ش��ȴ�ʱ��
#define		ACK_TIME				2			//������ACK�ȴ�ʱ��
#define		FATHER_NODE_NUM			6			//Ǳ�ڸ��ڵ������
#define		CHILDREN_NODE_NUM		20			//�ӽڵ������


#define		MAX_SEQUENCE_NUM		6			//��󻬶����������0��ֵ�ǵڶ������кŵ�ֵ��
#define		MAX_RETRANS_NUM			5			//����ش����г���
#define		MAX_ACK_NUM				10			//���ACK���г���

//����Ϊ6���������

/*----------------------------�ڵ�����--------------------------*/
#define		GATE_NODE				0			//���ؽڵ�
#define		ROUTE_NODE				1			//·�ɽڵ�
#define		END_NODE				2			//�ն˽ڵ�
#define		BACKUP_NODE				3			//���ݽڵ�
/*----------------------------����״̬--------------------------*/
#define		ONLINE					1			//����
#define		OFFLINE					0			//����


#define		ACCESS					2			//�������
#define		REJECT					0			//�ܾ�����
/*----------------------------֡����--------------------------*/
#define		BEACON_FRAME			0			//�ű�֡
#define		DATA_FRAME				1			//����֡
#define		ACK_FRAME				2			//ACKȷ��֡
#define		MAC_CONTROL_FRAME		3			//MAC����֡
#define		JOIN_REQ_FRAME			0			//��������֡
#define		ASSOCIATE_REQ_FRAME		0			//��������֡
#define		ASSOCIATE_RESP_FRAME	1			//�����ظ�֡
#define		KEEP_ALIVE_FRAME		2			//����֡
/*----------------------------ԭ������------------------------*/
#define		MLME_SCAN_REQUEST		0			//�ŵ�ɨ��ԭ��
#define		MLME_SCAN_confirm		0			//�ŵ�ɨ��ظ�ԭ��
#define		MLME_ASSOCIATE_REQUEST	1			//�ŵ�����ԭ��
#define		MLME_ASSOCIATE_confirm	1			//�ŵ������ظ�ԭ��
#define		MLME_ALIVE_REQUEST		2			//�ڵ㱣��ԭ��
#define		MLME_ALIVE_confirm		2			//�ڵ㱣��ظ�ԭ��
#define		MLME_UPDATE_NEIGHBOR_REQUEST	3	//�ڵ�����ӽڵ��ԭ��
#define		MLME_CONFLICT_confirm	4			//�ڵ��ַ��ͻ�ظ�ԭ��
#define		MLME_MULTICAST_confirm	5			//�ڵ��鲥�ظ�ԭ��
#define		MLME_SET_CHANNEL		6			//�����ŵ�Ƶ��Ƶ��ԭ����ݽڵ㹤��ʹ�ã�
#define		MLME_LEAVE_REQUEST		7			//��������ԭ��
#define		MLME_LEAVE_confirm		7			//����ȷ��ԭ��


#define		SENDING					1			//���ڷ��ͣ����ɽ���
#define		RECEIVING				2			//û�з��ͣ����Խ���



#define		MAC_LEAVING				1			//MAC����������
/*----------------------------�հ���ʶ��ֵ------------------------*/
#define		Receiced				1			//�յ��˰�
#define		Nopacket				2			//δ�յ���
/*----------------------------ɨ�踳ֵ------------------------*/
#define		NEED_SCAN				1			//��Ҫɨ��
#define		NO_SCAN					0			//����Ҫɨ��
/*----------------------------ͬ����ֵ------------------------*/
#define		COMPLETE_SYNCH			1			//���ͬ��
#define		NEED_SYNCH				0			//��Ҫͬ��
/*------------------------�����ظ���ʶ��ֵ--------------------*/
#define		RECV_ASSOCIATE_RESP		1			//�յ������ظ�
#define		NO_ASSOCIATE_RESP		0			//δ�յ������ظ�
/*------------------------��ַģʽ--------------------*/
#define		GATE_ADDRESS_MODE		1			//ʡ��ģʽ
#define		SHORT_ADDRESS_MODE		2			//16λ�̵�ַģʽ
#define		ESN_ADDRESS_MODE		3			//48λESN��ַģʽ

#define 	MAXTTL					2
#define 	MAX_NODE				200
#define 	MAX_ROUTER				48
#define 	MAX_CHILD				50
#define 	MAX_RouterAddress		128
#define 	GATEPOINTNUM			8
#define 	POINTNUM				2
#define 	MAX_MULTICAST			20
//-------------------------�������-----------------------//
#define		QUEUE_NUM			3		//��������

#define		Data_Queue			0		//Ӧ�ò����ݶ��У����У�
#define		Retrans_Queue		1		//�ش����У����У�
#define		ACK_Queue			2		//ACK���У����У�



/**------------------����������-------------------**/


/**------------------Interupt Code-------------------**/
#define   	Start_Code					10000
#define   	Intrpt_Slot_Down         	20001	//ȫ��֡�ж���
#define   	UpTime_Code         		20080	//ʱ϶�ж���
#define   	Up_Code         			20100	//�����ж���
#define		Scan_Code					50000	//����ɨ���ж�
#define		ChangeBand_Code				60000	//�л�ɨ��Ƶ���ж�
#define		Send_Complete_Code			70000	//��������ж�

#define		Children_Active				5		//���ڵ��յ��ӽڵ�ı�����Ϣ




/**------------------����״̬ת������-------------------**/
#define		Self_Start					((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Start_Code))
#define 	Join_Net					((op_intrpt_type() == OPC_INTRPT_REMOTE))

#define		C_STRM_NWK_DATA				((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == IN_NWK_MAC_DATA))
#define		C_STRM_NWK_MLME				((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == IN_NWK_MAC_MLME))
#define		C_STRM_MSG_FROM_CHILD_NODE	((op_intrpt_type() == OPC_INTRPT_STRM) && ((op_intrpt_strm() == IN_PHY_MAC)||(op_intrpt_strm() == 4)||(op_intrpt_strm() == 5)||(op_intrpt_strm() == 3)))
#define		C_SELF_NEW_SLOT				(((op_intrpt_type() == OPC_INTRPT_REMOTE) && (op_intrpt_code() == Intrpt_Slot_Down))||((op_intrpt_type() == OPC_INTRPT_SELF)&& (op_intrpt_code() == Intrpt_Slot_Down)))
#define		C_SELF_UP_TIME				((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Up_Code))
#define		C_SENDING_COMPLETE			((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Send_Complete_Code))




/*------------------ Input Index ------------------*/
//#define 	APP_MAC 					1		//�����ϲ㷢�����νڵ������
//#define 	PHY_MAC0 					0		//�������νڵ������
//#define 	PHY_MAC2 					0		//�������νڵ������
//#define 	APP_MAC_DOWN 				3		//�����ϲ㷢�����νڵ������
//#define 	MLME_FROM_NWK 				2		//������MAC���ԭ��
#define 	IN_PHY_MAC 					0		//�������νڵ������
#define 	IN_NWK_MAC_DATA 			1		//�����ϲ㷢�����νڵ������
#define 	IN_NWK_MAC_MLME 			2		//������MAC���ԭ��


/* ------------------Output Index------------------ */
//#define 	MAC_PHY2 					2		//�������νڵ�
//#define 	MAC_PHY0 					0		//�������νڵ�
//#define 	MAC_NWK 					1		//���������
//#define 	MAC_IO		 				2		//MAC���������ԭ��
#define 	OUT_MAC_PHY 				0		//�������νڵ�
#define 	OUT_MAC_NWK_DATA 			1		//���������
#define 	OUT_MAC_NWK_MLME	 		2		//MAC���������ԭ��



//************************���������************************//





//************************����ṹ��************************//
struct Potential_Parent
	{
	int short_addr;
	int panid;
	int band;
	int frequency;
	int router_cost;
	int depth;
	int times;
	};

struct neighborTable
	{
	int node_id;
	int node_address;
	int band;
	int frequency;
	int seq[MAX_SEQUENCE_NUM];	//0����Ѿ��յ������к�
	int active_time;
	int g_mac_seq;				//���ڵ㵽���ӽڵ���������к�
	};

struct Retrans
	{
	int time;
	int count;
	};
/*����ṹ��*/
struct KeepAliveFrame
	{
	int dest;
	int seq;
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

static void 	mac_init();
static void 	MLME_SCAN_CONFIRM();
static void 	MLME_ASSOCIATE_CONFIRM(int result);
static void 	MLME_ALIVE_CONFIRM(int dest,int result);

static void 	MLME_LEAVE_CONFIRM(int result);
static void		make_beacon();
static void 	set_channel_down(int fatherBand,int frequency);
static void 	create_associate_response_frame(int dest,int result);
static void 	update_ack_time();
static void 	update_retrans_time(int i);
static void 	delete_keep_alive(int dest);
static void 	delete_neighbor_node(int address ,int dest_mode);
static void 	send_msg_to_children();
static void		create_ack(Packet* pk,int i,int n);
static void 	mac_frame_proc(Packet* pk);
static void		ack_frame_proc(Packet* pk);
static void 	ack_proc(int seq ,int source ,int dest,int need_seq);
static int 		next_hop(int dest);
static int 		get_sequence_to_node(int dest,int mode);
static int		update_sequence_from_node(int source, int seq, int AR,int source_mode);
static void 	update_child_seq(int n);
static void 	update_alive_time(int source);
static void		create_actice_frame(int dest,int seq);
static void 	get_IES(Packet* pk);
static void 	sending_delay(int pk_size);
static void 	cheak_mac_queue();
static void 	mac_leave();

static void			make_ack_frame();
static void			create_sub_ack(Packet* pk,int n);
static void 		route_sink (Packet* pk);


/** ----------------Globle Attribute------------- **/
int globle_down_packet_number;
int globle_down_receive_ack_number;
long globle_down_route_cost; 
long globle_down_link_cost; 
long globle_down_net_cost;





//extern	struct		NetworkMsg Network_Msg;
extern struct NetworkMsg Network_Msg;

struct Potential_Parent potentialParent[MAX_NODE][FATHER_NODE_NUM];
int    Active_from_up[MAX_NODE];


/*++++++++++++++++++++++++test aloha++++++++++++++++++++++*/
/* Global Variable */
//extern int subm_pkts =0;
//extern int rcvd_pkts =0;

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
	Objid	                  		subnet_objid                                    ;	/* ����� */
	Objid	                  		receiver_from_father                            ;	/* ���ո��ڵ���Ϣ�Ľ��ջ�objid */
	Objid	                  		receiver_from_children                          ;	/* �����ӽڵ���Ϣ�Ľ��ջ�objid */
	Objid	                  		sender_to_father                                ;	/* �򸸽ڵ㷢����Ϣ�ķ��ͻ�objid */
	Objid	                  		sender_to_children                              ;	/* ���ӽڵ㷢����Ϣ�ķ��ͻ�objid */
	Objid	                  		channel_of_recvive_from_father                  ;
	Objid	                  		channel_of_recvive_from_children                ;
	Objid	                  		channel_of_send_to_father                       ;
	Objid	                  		channel_of_send_to_children                     ;
	Objid	                  		receive_from_father_comp                        ;
	Objid	                  		receive_from_children_comp                      ;
	Objid	                  		sender_to_father_comp                           ;
	Objid	                  		sender_to_children_comp                         ;
	int	                    		frame                                           ;	/* ֡��� */
	int	                    		ack_time[MAX_ACK_NUM]                           ;	/* ����ģ�����ACKʱ�� */
	struct Retrans	         		retrans[MAX_RETRANS_NUM]                        ;	/* �ش���ʱ�� */
	struct neighborTable	   		neighbor_table[CHILDREN_NODE_NUM]               ;	/* �ڽڵ����¼�ڽڵ�����к� */
	struct Potential_Parent			father_table[FATHER_NODE_NUM]                   ;	/* Ǳ�ڸ��ڵ�� */
	int	                    		pkt                                             ;	/* ���Է��Ͱ��ĸ��� */
	Packet *	               		pkptr                                           ;	/* ���ڴ�Ÿ����ڵ�����ݰ� */
	int	                    		activeFrameSeq                                  ;	/* ����֡�����к� */
	struct KeepAliveFrame	  		keepAlive[20]                                   ;	/* ����ṹ�� */
	int	                    		MulticastContent                                ;	/* �鲥ά����Ϣ */
	int	                    		g_mac_node_id                                   ;
	int	                    		g_mac_short_address                             ;
	int	                    		g_mac_node_depth                                ;
	int	                    		g_mac_node_type                                 ;
	int	                    		g_mac_pan_id                                    ;
	int	                    		g_mac_sequence                                  ;
	int	                    		g_mac_node_status                               ;
	Objid	                  		g_mac_node_objid                                ;
	Objid	                  		g_mac_process_objid                             ;
	int	                    		g_mac_slot_number                               ;
	int	                    		g_mac_synch_flag                                ;	/* ͬ����ɱ�ʶ��   1�����ͬ��   0��δ��� */
	int	                    		g_mac_no_packet_flag                            ;
	int	                    		g_mac_down_band                                 ;	/* �ڵ�����Ƶ�α�� */
	int	                    		g_mac_recv_frequency                            ;	/* �ڵ���յ�Ƶ��� */
	Log_Handle	             		g_mac_down_debug_log_handle                     ;
	Log_Handle	             		g_mac_down_info_log_handle                      ;
	double	                 		g_mac_slot_start_time                           ;	/* ʱ϶��ʼ��ʱ�� */
	double	                 		g_mac_current_time                              ;	/* ��ǰʱ�� */
	int	                    		g_mac_leave_flag                                ;	/* ����״̬ */
	double	                 		g_mac_data_rate                                 ;	/* �������� */
	double	                 		g_mac_sending_time                              ;	/* ����ʱ�� */
	int	                    		g_mac_nwk_synch_seq                             ;	/* �����ͬ�����к� */
	int	                    		g_mac_sending_flag                              ;
	int	                    		g_mac_leaving                                   ;
	int	                    		g_mac_receiving_flag                            ;	/* ����״̬��ʶ��   1�����ڷ��ͣ�2��û�з��� */
	Stathandle	             		pkt_success_stathandle                          ;	/* �ڵ㱾��ͳ�ƶ����� */
	Stathandle	             		g_pkt_success_stathandle                        ;	/* ȫ��ͳ�ƶ����� */
	Stathandle	             		route_cost_stathandle                           ;	/* �ڵ�ͳ�Ʊ���·�ɿ��� */
	Stathandle	             		g_route_cost_stathandle                         ;	/* ȫ��·�ɿ��� */
	Stathandle	             		link_cost_stathandle                            ;	/* �ڵ�ͳ����·���� */
	Stathandle	             		g_link_cost_stathandle                          ;	/* ȫ����·���� */
	Stathandle	             		net_cost_stathandle                             ;	/* �ڵ�ͳ�����翪�� */
	Stathandle	             		g_net_cost_stathandle                           ;	/* ȫ�����翪�� */
	Stathandle	             		all_cost_stathandle                             ;	/* ���ڵ����а����ܿ��� */
	Stathandle	             		g_all_cost_stathandle                           ;	/* ȫ���ܿ��� */
	Stathandle	             		packet_number_stathandle                        ;	/* ���ڵ㷢�Ͱ��ĸ�������ҪACK�� */
	Stathandle	             		g_packet_number_stathandle                      ;	/* ȫ�����Ͱ��ĸ�������ҪACK�� */
	int	                    		down_packet_number                              ;
	int	                    		down_receive_ack_number                         ;
	double	                 		down_route_cost                                 ;
	double	                 		down_link_cost                                  ;
	double	                 		down_net_cost                                   ;
	Stathandle	             		down_success_rate                               ;
	Stathandle	             		g_down_success_rate                             ;
	double	                 		frame_error_rate                                ;
	} WSN_MAC_DOWN2_state;

#define subnet_objid            		op_sv_ptr->subnet_objid
#define receiver_from_father    		op_sv_ptr->receiver_from_father
#define receiver_from_children  		op_sv_ptr->receiver_from_children
#define sender_to_father        		op_sv_ptr->sender_to_father
#define sender_to_children      		op_sv_ptr->sender_to_children
#define channel_of_recvive_from_father		op_sv_ptr->channel_of_recvive_from_father
#define channel_of_recvive_from_children		op_sv_ptr->channel_of_recvive_from_children
#define channel_of_send_to_father		op_sv_ptr->channel_of_send_to_father
#define channel_of_send_to_children		op_sv_ptr->channel_of_send_to_children
#define receive_from_father_comp		op_sv_ptr->receive_from_father_comp
#define receive_from_children_comp		op_sv_ptr->receive_from_children_comp
#define sender_to_father_comp   		op_sv_ptr->sender_to_father_comp
#define sender_to_children_comp 		op_sv_ptr->sender_to_children_comp
#define frame                   		op_sv_ptr->frame
#define ack_time                		op_sv_ptr->ack_time
#define retrans                 		op_sv_ptr->retrans
#define neighbor_table          		op_sv_ptr->neighbor_table
#define father_table            		op_sv_ptr->father_table
#define pkt                     		op_sv_ptr->pkt
#define pkptr                   		op_sv_ptr->pkptr
#define activeFrameSeq          		op_sv_ptr->activeFrameSeq
#define keepAlive               		op_sv_ptr->keepAlive
#define MulticastContent        		op_sv_ptr->MulticastContent
#define g_mac_node_id           		op_sv_ptr->g_mac_node_id
#define g_mac_short_address     		op_sv_ptr->g_mac_short_address
#define g_mac_node_depth        		op_sv_ptr->g_mac_node_depth
#define g_mac_node_type         		op_sv_ptr->g_mac_node_type
#define g_mac_pan_id            		op_sv_ptr->g_mac_pan_id
#define g_mac_sequence          		op_sv_ptr->g_mac_sequence
#define g_mac_node_status       		op_sv_ptr->g_mac_node_status
#define g_mac_node_objid        		op_sv_ptr->g_mac_node_objid
#define g_mac_process_objid     		op_sv_ptr->g_mac_process_objid
#define g_mac_slot_number       		op_sv_ptr->g_mac_slot_number
#define g_mac_synch_flag        		op_sv_ptr->g_mac_synch_flag
#define g_mac_no_packet_flag    		op_sv_ptr->g_mac_no_packet_flag
#define g_mac_down_band         		op_sv_ptr->g_mac_down_band
#define g_mac_recv_frequency    		op_sv_ptr->g_mac_recv_frequency
#define g_mac_down_debug_log_handle		op_sv_ptr->g_mac_down_debug_log_handle
#define g_mac_down_info_log_handle		op_sv_ptr->g_mac_down_info_log_handle
#define g_mac_slot_start_time   		op_sv_ptr->g_mac_slot_start_time
#define g_mac_current_time      		op_sv_ptr->g_mac_current_time
#define g_mac_leave_flag        		op_sv_ptr->g_mac_leave_flag
#define g_mac_data_rate         		op_sv_ptr->g_mac_data_rate
#define g_mac_sending_time      		op_sv_ptr->g_mac_sending_time
#define g_mac_nwk_synch_seq     		op_sv_ptr->g_mac_nwk_synch_seq
#define g_mac_sending_flag      		op_sv_ptr->g_mac_sending_flag
#define g_mac_leaving           		op_sv_ptr->g_mac_leaving
#define g_mac_receiving_flag    		op_sv_ptr->g_mac_receiving_flag
#define pkt_success_stathandle  		op_sv_ptr->pkt_success_stathandle
#define g_pkt_success_stathandle		op_sv_ptr->g_pkt_success_stathandle
#define route_cost_stathandle   		op_sv_ptr->route_cost_stathandle
#define g_route_cost_stathandle 		op_sv_ptr->g_route_cost_stathandle
#define link_cost_stathandle    		op_sv_ptr->link_cost_stathandle
#define g_link_cost_stathandle  		op_sv_ptr->g_link_cost_stathandle
#define net_cost_stathandle     		op_sv_ptr->net_cost_stathandle
#define g_net_cost_stathandle   		op_sv_ptr->g_net_cost_stathandle
#define all_cost_stathandle     		op_sv_ptr->all_cost_stathandle
#define g_all_cost_stathandle   		op_sv_ptr->g_all_cost_stathandle
#define packet_number_stathandle		op_sv_ptr->packet_number_stathandle
#define g_packet_number_stathandle		op_sv_ptr->g_packet_number_stathandle
#define down_packet_number      		op_sv_ptr->down_packet_number
#define down_receive_ack_number 		op_sv_ptr->down_receive_ack_number
#define down_route_cost         		op_sv_ptr->down_route_cost
#define down_link_cost          		op_sv_ptr->down_link_cost
#define down_net_cost           		op_sv_ptr->down_net_cost
#define down_success_rate       		op_sv_ptr->down_success_rate
#define g_down_success_rate     		op_sv_ptr->g_down_success_rate
#define frame_error_rate        		op_sv_ptr->frame_error_rate

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	WSN_MAC_DOWN2_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((WSN_MAC_DOWN2_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif

/*********************************************/
/*********************************************/
/*MAC���ʼ����������ʼ��״̬��Ϣ�����У��ṹ���ڴ棬MAC���к�*/
/*********************************************/
/*********************************************/
static void mac_init()
	{
	int 	 			i;
	int                 new_seed;
	double				a;
	double				b;
	PrgT_Random_Gen    	*my_rng;
	FIN(mac_init());
	new_seed = g_mac_node_id;
	frame	= 0;
	g_mac_slot_number	= 0;
	g_mac_node_status = OFFLINE;										//�ڵ㿪��ʱ������	
	op_ima_obj_attr_get (channel_of_send_to_children,"data rate",&g_mac_data_rate);
	for(i = 0;i < MAX_ACK_NUM;i++)
		{
		ack_time[i] = 100;
		}
	
	for(i = 0;i < MAX_RETRANS_NUM;i++)
		{
		retrans[i].time = 100;
		retrans[i].count = 0;
		}
	//�ڽڵ����кű��ʼ��------------------------------
	for(i=0;i<CHILDREN_NODE_NUM;i++)
		{
		neighbor_table[i].node_id = 0;
		neighbor_table[i].seq[0] = 0;
		neighbor_table[i].band = 0;
		neighbor_table[i].node_address = 0;
		neighbor_table[i].frequency = 0;
		neighbor_table[i].active_time = 0;
		keepAlive[i].dest = 0;
		keepAlive[i].seq = 0;
		}
	//���г�ʼ��-----------------------------------------
	/* Clear SubQ */
	op_subq_flush(Data_Queue);
	op_subq_flush(Retrans_Queue);
	op_subq_flush(ACK_Queue);
	//���Ľڵ�Ϊͬ����׼�ڵ㣬��ͬ��---------------------
	if(g_mac_node_type == GATE_NODE)
		{										//���Ľڵ㣬ֱ�ӽ���NET	
		//	op_ima_obj_attr_set (g_mac_node_objid,	"ShortAddress"	,	CEN_NODE);
		op_ima_obj_attr_set (channel_of_recvive_from_children,"min frequency",470.0);
		op_ima_obj_attr_set (channel_of_send_to_children,"min frequency",470.0);
		op_ima_obj_attr_set (g_mac_node_objid,	"g_node_depth",0);
		op_ima_obj_attr_get (channel_of_recvive_from_children,"min frequency",&a);
		op_ima_obj_attr_get (channel_of_send_to_children,"min frequency",&b);
		printf("node %d channel_of_recvive_from_children =%f channel_of_send_to_children =%f \n",g_mac_node_id,a,b);
		op_ima_obj_attr_get (g_mac_node_objid,	"g_node_short_address",	&g_mac_short_address);
		op_ima_obj_attr_set (g_mac_node_objid,	"g_node_status",	ONLINE);
		g_mac_short_address = CEN_NODE;
		g_mac_synch_flag = COMPLETE_SYNCH;                          //ͬ�����
		g_mac_node_status = ONLINE;									//���ؽڵ�ֱ������
		g_mac_node_depth = 0;
		}
	if(g_mac_node_type != GATE_NODE)
		{
		g_mac_synch_flag = NEED_SYNCH;								//�����ؽڵ�
		g_mac_node_status = OFFLINE;								//�ֽ׶νڵ㿪��������ʱ϶������Ҫͬ������
		}
	//��ʼ��MAC�����к�
	my_rng = op_prg_random_gen_create (new_seed);
	g_mac_sequence = (op_prg_random_integer_gen (my_rng) % 256) ;
	op_prg_random_gen_destroy (my_rng); 
	FOUT;
	}



/*********************************************/
/*********************************************/
/*create MLME_ALICE.confirm,and send it to NWK*/
/*0:success    1:fail*/
/*********************************************/
/*********************************************/
static void MLME_ALIVE_CONFIRM(int dest,int result)
	{
	Packet* MLME_ALIVE;
	int type;
	FIN(MLME_ALIVE_CONFIRM(int dest,int result));
	type = MLME_ALIVE_confirm;
	MLME_ALIVE = op_pk_create(0); //32 
	op_pk_fd_set (MLME_ALIVE, 0, OPC_FIELD_TYPE_INTEGER,type,8) ;
	op_pk_fd_set (MLME_ALIVE, 1, OPC_FIELD_TYPE_INTEGER,dest,16) ;
	op_pk_fd_set (MLME_ALIVE, 2, OPC_FIELD_TYPE_INTEGER,result,8) ;
	op_pk_send(MLME_ALIVE,OUT_MAC_NWK_MLME);	
	FOUT;
	}



/*********************************************/
/*********************************************/
/*create MLME_CONFLICT.confirm,and send it to NWK*/
/*0:success    1:fail*/
/*********************************************/
/*********************************************/
static void MLME_CONFLICT_CONFIRM(int address)
	{
	Packet* MLME_CONFLICT;
	int type;
	FIN(MLME_CONFLICT_CONFIRM(int address));
	type = MLME_CONFLICT_confirm;
	MLME_CONFLICT = op_pk_create(0);  //24
	op_pk_fd_set (MLME_CONFLICT, 0, OPC_FIELD_TYPE_INTEGER,type,8) ;
	op_pk_fd_set (MLME_CONFLICT, 1, OPC_FIELD_TYPE_INTEGER,address,16) ;
	op_pk_send(MLME_CONFLICT,OUT_MAC_NWK_MLME);	
	FOUT;
	}




/*********************************************/
/*********************************************/
/*create MLME_MULTICAST.confirm,and send it to NWK*/
/*********************************************/
/*********************************************/
static void MLME_MULTICAST_CONFIRM()
	{
	Packet* MLME_MULTICAST;
	int type;
	FIN(MLME_MULTICAST_CONFIRM());
	type = MLME_MULTICAST_confirm;
	MLME_MULTICAST = op_pk_create(0);  //24
	op_pk_fd_set (MLME_MULTICAST, 0, OPC_FIELD_TYPE_INTEGER,type,8) ;
	op_pk_fd_set (MLME_MULTICAST, 1, OPC_FIELD_TYPE_INTEGER,MulticastContent,16) ;
	op_pk_send(MLME_MULTICAST,OUT_MAC_NWK_MLME);	
	FOUT;
	}
/*********************************************/
/*********************************************/
/*create MLME_LEAVE_CONFIRM,and send it to NWK*/
/*0:success    1:fail*/
/*********************************************/
/*********************************************/
static void MLME_LEAVE_CONFIRM(int result)
	{
	Packet* MLME_LEAVE;
	int type;
	FIN(MLME_LEAVE_CONFIRM(int result));
	type = MLME_LEAVE_confirm;
	MLME_LEAVE = op_pk_create(0);  //24
	op_pk_fd_set (MLME_LEAVE, 0, OPC_FIELD_TYPE_INTEGER,type,8) ;
	op_pk_fd_set (MLME_LEAVE, 1, OPC_FIELD_TYPE_INTEGER,result,8) ;
	op_pk_send(MLME_LEAVE,OUT_MAC_NWK_MLME);	
	FOUT;
	}



/*********************************************/
/*********************************************/
/*cheak queue to father*/
/*********************************************/
/*********************************************/
static void cheak_mac_queue()
	{
	Boolean isEmpty_data;
	int nums;
	int i;
	int framecontrol;
	int type;
	int subtype;
	Packet* pk;
	Packet* copy;
	FIN(cheak_mac_queue());
	op_subq_flush(Retrans_Queue);
	op_subq_flush(ACK_Queue);
	isEmpty_data = op_subq_empty (Data_Queue);
	if(!isEmpty_data)
		{
		if(g_mac_node_type == ROUTE_NODE)
			{
			nums = op_subq_stat (Data_Queue, OPC_QSTAT_PKSIZE);
			for(i = 0;i<nums;i++)
				{
				pk = op_subq_pk_access (Data_Queue, i);
				op_pk_fd_get(pk,0,&framecontrol);
				type = (framecontrol>>13)&7;
				if(type == 1)
					{
					op_pk_fd_get(pk,7,&subtype);
					if(((subtype>>12)&15) == 2)
						{
						copy = op_pk_copy(pk);
						op_subq_flush(Data_Queue);
						op_subq_pk_insert(Data_Queue, copy, OPC_QPOS_TAIL);
						break;
						}
					}		
				}	
			}
		else if(g_mac_node_type == END_NODE||g_mac_node_type == BACKUP_NODE)
			{
			op_subq_flush(Data_Queue);
			}	
		}
	isEmpty_data = op_subq_empty (Data_Queue);
	if(isEmpty_data)
		{
		if(g_mac_node_status != OFFLINE) MLME_LEAVE_CONFIRM(2);//���������������
		op_intrpt_clear_self ();
		op_strm_flush(IN_PHY_MAC);
		mac_leave();
		}
	FOUT;
	}
/*********************************************/
/*********************************************/
/*MAC leaving father*/
/*********************************************/
/*********************************************/
static void mac_leave()
	{
	FIN(mac_leave());
	mac_init();	
	g_mac_leaving = 0;//�������	
	set_channel_down(100,0);
	FOUT;
	}	



/*********************************************/
/*********************************************/
//Build a beacon frame and transmit it---------
/*********************************************/
/*********************************************/
static void	make_beacon()
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
	int 	pk_size;
	int 	i;
	FIN(make_beacon());
	beacon = op_pk_create(0);	//120
	frame_control	= 546;
	sequence		= 0;//g_mac_sequence;
	//g_mac_sequence 	= (g_mac_sequence+1)%256;
	panid 			= g_mac_pan_id;
	dest 			= BroadCastAddress;
	source 			= g_mac_short_address;
	beacon_sequence	= 0;
	max_depth 		= Max_Depth;
	current_depth	= g_mac_node_depth;
	slot_number		= 0;
	access_control	= ACCESS;
	router_cost		= g_mac_node_depth;
	frequency 		= 0;
	strenth			= 0;
	//if(g_mac_node_depth == 8) access_control = REJECT;
	for(i = 0;i<CHILDREN_NODE_NUM;i++)
		{
		if(neighbor_table[i].node_id == 0)
			{
			break;
			}
		else
			{
			if(i == CHILDREN_NODE_NUM-1)
				{
				access_control = REJECT;
				}
			}
		}
	op_pk_fd_set (beacon, 0, OPC_FIELD_TYPE_INTEGER ,frame_control,16) ;
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
	//printf("Node %d has sent a beacon at g_mac_slot_number%d at time%f!!!!!!!!!\n",g_mac_node_id,g_mac_slot_number,op_sim_time());
	//op_prg_log_entry_write(g_mac_down_debug_log_handle, "Node %d has sent a beacon at g_mac_slot_number%d!\n",g_mac_node_id,g_mac_slot_number);
	pk_size = op_pk_total_size_get(beacon);
	
	globle_down_link_cost = globle_down_link_cost+pk_size;
	down_link_cost = down_link_cost+pk_size;
	
	g_mac_sending_time = pk_size/g_mac_data_rate;
	op_pk_send_delayed (beacon, OUT_MAC_PHY,0.001);
	op_intrpt_schedule_self(op_sim_time()+g_mac_sending_time, Send_Complete_Code);
	FOUT;
	}




static void sending_delay(int pk_size)
	{
	FIN(sending_delay(int pk_size));
	g_mac_sending_time = pk_size/g_mac_data_rate;
	op_intrpt_schedule_self(op_sim_time() + g_mac_sending_time, Send_Complete_Code);
	FOUT;
	}


/*********************************************/
/*********************************************/
/*set receiver's and sender's channel*/
/*********************************************/
/*********************************************/
static void set_channel_down(int Band,int frequency)
	{
	double recvFreq;
	double sendFreq;
	FIN(set_channel_down(int Band,int frequency));
	sendFreq = 470.0+4*Band;
	recvFreq = sendFreq+frequency*1.0;
	op_ima_obj_attr_set (channel_of_recvive_from_children,"min frequency",recvFreq);
	op_ima_obj_attr_set (channel_of_send_to_children,"min frequency",sendFreq);
	FOUT;
	}


/*********************************************/
/*********************************************/
/*��ȡ�ӽڵ�Ķ�Ӧ���к����к������Ӻ�ֵ*/
/*parameter  dest :�ӽڵ��ַ   
	mode��ʶdest�ǳ���ַ���Ƕ̵�ַ 
	2����̵�ַ 3������ַ
	�ڵ�û�����кžͷ���-1*/
/*********************************************/
/*********************************************/
static int get_sequence_to_node(int dest,int mode)
	{
	int i;
	FIN(get_sequence_to_node(int dest,int type));
	if(mode == SHORT_ADDRESS_MODE)
		{
		for(i = 0;i<CHILDREN_NODE_NUM ;i++)
			{
			if(g_mac_node_id == 0&&dest == 199) printf("neighbor_table[%d].node_address = %d\n",i,neighbor_table[i].node_address);
			if(neighbor_table[i].node_address == next_hop(dest)||neighbor_table[i].node_address == dest)
				{
				neighbor_table[i].g_mac_seq = (neighbor_table[i].g_mac_seq+1)%256;
				FRET(neighbor_table[i].g_mac_seq);
				}
			}
		}
	else if(mode == ESN_ADDRESS_MODE)
		{
		for(i = 0;i<CHILDREN_NODE_NUM ;i++)
			{
			if(neighbor_table[i].node_id == dest)
				{
				neighbor_table[i].g_mac_seq = (neighbor_table[i].g_mac_seq+1)%256;
				FRET(neighbor_table[i].g_mac_seq);
				}
			}
		}
	FRET(-1);
	}



/*********************************************/
/*********************************************/
/*�����ӽڵ���������к��շ�״̬*/
/*********************************************/
/*********************************************/
static void update_child_seq(int n)
	{
	int i;
	int j;
	int count = 0;
	FIN(update_child_seq(int n));
	for(i = 1;i < MAX_SEQUENCE_NUM;i++)
		{
		printf("neighbor_table[n].seq[%d] = %d\n",i,neighbor_table[n].seq[1]);
		if(neighbor_table[n].seq[1] == 0)
			{
			break;
			}
		else if(neighbor_table[n].seq[1] == 1)
			{
			count++;
			for(j = 1;j<MAX_SEQUENCE_NUM-1;j++)
				{
				neighbor_table[n].seq[j] = neighbor_table[n].seq[j+1];
				}
			neighbor_table[n].seq[MAX_SEQUENCE_NUM-1] = 0;
			}
		if(i == 5)
			{
			printf("jinlaile\n");
			neighbor_table[n].seq[0] = (neighbor_table[n].seq[0]+5)%256;
			for(j = 1;j<MAX_SEQUENCE_NUM;j++)
				{
				neighbor_table[n].seq[j] = 0;
				}
			FOUT;
			}
		}
	neighbor_table[n].seq[0] = (neighbor_table[n].seq[0]+count)%256;
	FOUT;
	}


/*********************************************/
/*********************************************/
/*�����ӽڵ�source���������к�
source���ӽڵ��ַ
mode���ӽڵ��ַ����  2�̵�ַ  3����ַ
AR���Ƿ���ҪACK�ظ�
����ֵ  1����Ҫ������ظ�ACK����Ҫ���������  
		2����Ҫ������ظ�ACK������Ҫ���������  
		3��ֱ��ɾ��*/
/*********************************************/
/*********************************************/
static int update_sequence_from_node(int source, int seq, int AR,int source_mode)
	{
	int n;
	int diff;
	int flag;
	FIN(update_sequence_from_node(int source, int seq, int AR,int source_mode));
	flag = 0;
	if(source_mode == SHORT_ADDRESS_MODE)
		{
		for(n = 0;n < CHILDREN_NODE_NUM;n++)
			{
			if(neighbor_table[n].node_address == next_hop(source)||neighbor_table[n].node_address == source)
				{
				flag = 1;				//���ӽڵ�����ҵ��˶�Ӧ�ڵ�			
				break;
				}
			}
		}
	else if(source_mode == ESN_ADDRESS_MODE)
		{
		for(n = 0;n <CHILDREN_NODE_NUM;n++)
			{
			if(neighbor_table[n].node_id == source)
				{
				flag = 1;				//���ӽڵ�����ҵ��˶�Ӧ�ڵ�			
				break;
				}
			}
		//������µ��ӽڵ�����
		if(flag == 0)
			{
			flag = 2;
			}
		}
	else
		{		
		flag = 0;
		}
	
	if(flag == 0)//����Դ��ַ�����ӽڵ����(�ǹ�������)
		{
		printf("Node %d receive a wrong packet!!! Source node isn't child\n",g_mac_node_id);
		op_prg_log_entry_write(g_mac_down_debug_log_handle, "Node %d receive a wrong packet!!! Source node isn't child\n",g_mac_node_id);
		FRET(3);	
		}
	else if(flag == 2)
		{
		printf("Node %d receive a MAC frame ,source is new = %d\n",g_mac_node_id,source);
		FRET(1);
		}
	else if(flag == 1)							//����Դ��ַ���ӽڵ����
		{
		diff = seq - neighbor_table[n].seq[0];
		printf("Node %d receive a packet ,neighbor_table[%d].seq[0] = %d,seq = %d diff = %d\n",g_mac_node_id,n,neighbor_table[n].seq[0],seq,diff);
		if(diff < -250) diff = seq+256-neighbor_table[n].seq[0];	//����255
		//if(diff <= 0&&diff>(-5))									//�Ѿ����չ���֡
		if((diff <= 0&&diff>(-5))||diff>=250)
			{
			printf("Node %d receive an packet from down seq is old!!!\n",g_mac_node_id);
			op_prg_log_entry_write(g_mac_down_debug_log_handle, "Node %d receive an packet from down seq is old!!!\n",g_mac_node_id);
			FRET(2);
			}
		else if(diff<(MAX_SEQUENCE_NUM-1)&&diff>0)		//����֡
			{
			//printf("111diff = %d,neighbor_table[n].seq[diff] = %d\n",diff,neighbor_table[n].seq[diff]);
			if(neighbor_table[n].seq[diff] == 0) 		//֡û�б����չ�
				{
				neighbor_table[n].seq[diff] = 1;		//���к�Ϊseq[0]+diff �İ��յ���
			//	printf("222diff = %d,neighbor_table[n].seq[diff] = %d\n",diff,neighbor_table[n].seq[diff]);
				update_child_seq(n);
				FRET(1);
				}
			FRET(2);									//֡�����ܹ���
			}
		else if(diff>=6&&diff<250)											//�쳣֡���
			{
			printf("Node %d receive an packet from down seq is over!!!\n",g_mac_node_id);
			op_prg_log_entry_write(g_mac_down_info_log_handle, "Node %d receive an packet from down seq is over!!!\n",g_mac_node_id);
			if(source_mode == SHORT_ADDRESS_MODE) 
				MLME_CONFLICT_CONFIRM(next_hop(source));	//����̵�ַ��ͻ
			else if(source_mode == ESN_ADDRESS_MODE)
				MLME_CONFLICT_CONFIRM(source);	//����̵�ַ��ͻ
			FRET(3);
			}
		else
			{
			printf("\t\tNode %d downhuachuang == 5,to node %d\n",g_mac_node_id,neighbor_table[n].node_id);
			if(diff == MAX_SEQUENCE_NUM-1) neighbor_table[n].seq[diff] = 1;
			if(neighbor_table[n].seq[2]+neighbor_table[n].seq[3]+neighbor_table[n].seq[4]+neighbor_table[n].seq[5]>=2) 
				{
			//	printf("neighbor_table[n].seq[1] = 1;\n");
				neighbor_table[n].seq[1] = 1;
				update_child_seq(n);
				FRET(1);
				}
			else
				{
				
				}
			}
		}
	FRET(3);
	}




/*********************************************/
/*********************************************/
/*���������ظ�֡,����֡���뷢�Ͷ��ж���ͷ*/
/*********************************************/
/*********************************************/
static void create_associate_response_frame(int dest,int result)
	{
	int framecontrol;
	int seq;
	int pan;
	int dest_addr;
	int source;
	int length;
	int subcontrol;
	int type;
	Packet* associateResponse;
	FIN(create_associate_response_frame(int dest,int result));
	framecontrol = 25138;//011 0001000 11 00 10
	
	//seq = g_mac_sequence;
	//g_mac_sequence = (g_mac_sequence+1)%256;
	seq = get_sequence_to_node(dest,ESN_ADDRESS_MODE);
	pan = g_mac_pan_id;
	dest_addr = dest;
	source = g_mac_short_address;
	length = 3;
	subcontrol = 24;
	associateResponse = op_pk_create(0);//120
	op_pk_fd_set (associateResponse, 0, OPC_FIELD_TYPE_INTEGER,framecontrol,16) ;
	op_pk_fd_set (associateResponse, 1, OPC_FIELD_TYPE_INTEGER,seq,8) ;
	op_pk_fd_set (associateResponse, 2, OPC_FIELD_TYPE_INTEGER,pan,16) ;
	op_pk_fd_set (associateResponse, 3, OPC_FIELD_TYPE_INTEGER,dest_addr,16) ;
	op_pk_fd_set (associateResponse, 4, OPC_FIELD_TYPE_INTEGER,source,48) ;
	op_pk_fd_set (associateResponse, 5, OPC_FIELD_TYPE_INTEGER,length,8) ;
	op_pk_fd_set (associateResponse, 7, OPC_FIELD_TYPE_INTEGER,subcontrol,8) ;
	op_pk_fd_set (associateResponse, 8, OPC_FIELD_TYPE_INTEGER,result,8) ;
	op_pk_fd_get(associateResponse,0,&framecontrol);
	type = (framecontrol>>13)&7;
	printf("Node %d create associate response type = %d\n",g_mac_node_id,type);
	op_prg_log_entry_write(g_mac_down_info_log_handle, "Node %d create associate response type = %d\n",g_mac_node_id,type);
	op_subq_pk_insert(Data_Queue,associateResponse,OPC_QPOS_TAIL);
	FOUT;
	}


/*********************************************/
/*********************************************/
/*����ack_time[5]   (�����͵�ACK֡�ļ�ʱ�����ã�ͬʱ��������֡�ļ�ʱ��λ��)*/
/*********************************************/
/*********************************************/
static void update_ack_time()
	{
	int j;
	FIN(update_ack_time());
	for(j = 1;j < MAX_ACK_NUM;j++)
		{
		ack_time[j-1] = ack_time[j];
		}
	ack_time[MAX_ACK_NUM-1] = 100;
	FOUT;
	}



/*********************************************/
/*********************************************/
/*�����ش���ʱ��   (ɾ�����͵�֡�ļ�ʱ������������֡�ļ�ʱ��)*/
/*********************************************/
/*********************************************/
static void update_retrans_time(int i)
	{
	int j;
	FIN(update_retrans_time(int j));
	for(j = i;j < MAX_RETRANS_NUM;j++)
		{
		retrans[j-1].time = retrans[j].time;
		retrans[j-1].count = retrans[j].count;
		}
	retrans[MAX_RETRANS_NUM-1].time = 100;
	retrans[MAX_RETRANS_NUM-1].count = 0;
	FOUT;	
	}

/*********************************************/
/*********************************************/
/*���ñ���ṹ���Ӧ��Ŀ*/
/*********************************************/
/*********************************************/
static void delete_keep_alive(int dest)
	{
	int i;
	FIN(delete_keep_alive(int dest));
	for(i = 0;i<CHILDREN_NODE_NUM;i++)
		{
		if(keepAlive[i].dest == dest)//�ýڵ��Ѿ��ڱ�����
			{
			keepAlive[i].dest = 0;
			keepAlive[i].seq = 0;
			break;
			}		
		}
	FOUT;	
	}



static void check_keep_alive(int dest)
	{
	int retrans_num;
	int data_num;
	Packet* pk;
	int seq;
	int i;
	int k;
	FIN(check_keep_alive(int dest));
	if(next_hop(dest)!=-1)
		{
		for(k = 0;k<CHILDREN_NODE_NUM;k++)
			{
			if(keepAlive[k].dest == dest)//�ýڵ��Ѿ��ڱ�����
				{
				data_num = op_subq_stat (Data_Queue, OPC_QSTAT_PKSIZE);
				for(i = 0;i<data_num;i++)
					{
					pk = op_subq_pk_access (Data_Queue, i);
					op_pk_fd_get(pk,1,&seq);
					if(seq == keepAlive[k].seq)
						{
						delete_keep_alive(next_hop(dest));
						FOUT;
						}	
					}
				retrans_num = op_subq_stat (Retrans_Queue, OPC_QSTAT_PKSIZE);
				for(i = 0;i<retrans_num;i++)
					{
					pk = op_subq_pk_access (Retrans_Queue, i);
					op_pk_fd_get(pk,1,&seq);
					if(seq == keepAlive[k].seq)
						{
						delete_keep_alive(next_hop(dest));
						FOUT;
						}	
					}
				}		
			}
		}	
	FOUT;	
	}








/*********************************************/
/*********************************************/
/*ɾ���ڽӵ�����Ӧ��Ŀ*/
/*********************************************/
/*********************************************/
static void delete_neighbor_node(int address ,int dest_mode)
	{
	int i;
	FIN(delete_neighbor_node(int address));
	if(dest_mode == SHORT_ADDRESS_MODE)
		{
		for(i = 0;i<CHILDREN_NODE_NUM ;i++)
			{
			if(neighbor_table[i].node_address == address)
				{
				break;
				}
			}
		}
	else if(dest_mode == ESN_ADDRESS_MODE)
		{
		for(i = 0;i<CHILDREN_NODE_NUM ;i++)
			{
			if(neighbor_table[i].node_id == address)
				break;
			}
		}
	neighbor_table[i].node_id = 0;
	neighbor_table[i].band = 0;
	neighbor_table[i].frequency = 0;
	neighbor_table[i].seq[0] = 0;
	neighbor_table[i].active_time = 0;
	neighbor_table[i].g_mac_seq = 0;	
	FOUT;
	}



/*********************************************/
/*********************************************/
/*Transmit data ftame to children node*/
/*********************************************/
/*********************************************/
static void send_msg_to_children()
	{
	Boolean isEmpty_data;
	Boolean isEmpty_retrans;
	Boolean isEmpty_ack;
	Packet* pk;
	Packet* copy;
	int framecontrol;
	int AR;
	int	num_pkts;
	int i;
	int type;
	int seq;
	int dest;
	int count=0;
	int pk_size;
	int framecontrol1;
	int type1;
	int dest_mode;
//	int nwk_head;
//	int subtype;
	OpT_Packet_Id pk_id; 
	double pk_create_time ;//ԭ���ݵĴ���ʱ��
	
	FIN(send_msg_to_children());	
	isEmpty_ack = op_subq_empty (ACK_Queue);
	if((isEmpty_ack == OPC_FALSE)&&(ack_time[0]<=0))//ACK�����Ƿ�Ϊ�գ��ǿ��ȷ���ACK��
		{		
		printf("\nNode %d send_ack at slot %d!!!time = %f\n",g_mac_node_id,g_mac_slot_number,op_sim_time());
		make_ack_frame();
		FOUT;
		}	
	else												//ACK����Ϊ�գ��ж��ش������Ƿ��а����Է���
		{
		isEmpty_retrans = op_subq_empty (Retrans_Queue);
		if((isEmpty_retrans == OPC_FALSE)&&(retrans[0].time <= 0))
			{
			if(retrans[0].count>=3)						//�ش�����3�ε�֡����
				{
				update_retrans_time(1);
				pk = op_subq_pk_remove (Retrans_Queue, 0);
				op_pk_fd_get(pk,0,&framecontrol);
				op_pk_fd_get(pk,3,&dest);
				op_pk_fd_get(pk,1,&seq);
				type = (framecontrol>>13)&7;
				dest_mode = (framecontrol>>4)&3;
				if(type == MAC_CONTROL_FRAME)			//����֡�ش���������
					{
					op_pk_fd_get(pk,1,&seq);
					op_pk_destroy(pk);
					MLME_ALIVE_CONFIRM(next_hop(dest),1);			
					if(dest_mode == ESN_ADDRESS_MODE) 
						{
						delete_keep_alive(dest);				//ɾ������ṹ����Ŀ
						delete_neighbor_node(dest,dest_mode);	//ɾ���ڽӵ����Ŀ		
						}
					else if(dest_mode == SHORT_ADDRESS_MODE) 
						{
						delete_keep_alive(next_hop(dest));		//ɾ������ṹ����Ŀ
						delete_neighbor_node(next_hop(dest),dest_mode);	//ɾ���ڽӵ����Ŀ
						}			
					}
				else									//����֡����ʧ�ܾͿ�ʼ��������֡
					{
					op_pk_destroy(pk);
					for(i = 0;i<CHILDREN_NODE_NUM;i++)
						{
						if(keepAlive[i].dest == next_hop(dest))	//�ýڵ��Ѿ��ڱ�����
							{
							op_intrpt_schedule_self(op_sim_time(), Send_Complete_Code);
							FOUT;
							}
						}
					if(dest_mode == SHORT_ADDRESS_MODE) create_actice_frame(next_hop(dest),seq);//��������֡	
					else if(dest_mode == ESN_ADDRESS_MODE) create_actice_frame(dest,seq);
					}
				op_intrpt_schedule_self(op_sim_time(), Send_Complete_Code);
				FOUT;
				}
			pk = op_subq_pk_remove (Retrans_Queue, 0);
			count = retrans[0].count;				
			op_pk_fd_get(pk,1,&seq);
			op_pk_fd_get(pk,3,&dest);
			printf("Node %d send a retrans packet at g_mac_slot_number%d !dest = %d,seq = %d,time = %f\n",g_mac_node_id,g_mac_slot_number,dest,seq,op_sim_time());	
			op_prg_log_entry_write(g_mac_down_debug_log_handle, "Node %d send a retrans packet at g_mac_slot_number%d !dest = %d,seq = %d\n",g_mac_node_id,g_mac_slot_number,dest,seq);
			pk_create_time= op_pk_creation_time_get(pk);
			copy = op_pk_copy(pk);
			op_pk_creation_time_set(copy,pk_create_time);			
			op_pk_fd_get (pk, 0, &framecontrol) ;
			type = (framecontrol>>13)&7;
			pk_size = op_pk_total_size_get(pk);
			sending_delay(pk_size);
			
			/*if(type == 1)
				{
				op_pk_fd_get (pk, 7, &nwk_head) ;
				subtype = (nwk_head>>12)&15;
				if(subtype == 0||subtype == 1||subtype == 2||subtype == 3||subtype == 4||subtype == 5)
					{
					globle_down_route_cost = globle_down_route_cost+pk_size;
					down_route_cost = down_route_cost+pk_size;
					} 
				}*/
			
			route_sink( pk);
			globle_down_link_cost = globle_down_link_cost+pk_size;
			down_link_cost = down_link_cost+pk_size;
			op_pk_send (pk, OUT_MAC_PHY);
			update_retrans_time(1);
			}
		else												//�ش�����Ϊ�գ�������ͨ���ݰ�
			{
			num_pkts = op_subq_stat (Retrans_Queue, OPC_QSTAT_PKSIZE);
			if(num_pkts>=MAX_RETRANS_NUM)
				{
				op_intrpt_schedule_self(op_sim_time(), Send_Complete_Code);
				FOUT;
				}
			isEmpty_data = op_subq_empty (Data_Queue);			
			if(isEmpty_data ==  OPC_FALSE)					//�������ݶ��зǿ�
				{				
				pk = op_subq_pk_remove (Data_Queue, 0);
				//pk_size = op_pk_total_size_get(pk);
				op_pk_fd_get(pk,0,&framecontrol1);
				type1 = (framecontrol1>>13)&7;
				op_pk_fd_get(pk,1,&seq);
				op_pk_fd_get(pk,3,&dest);
				printf("Node %d send a data packet at g_mac_slot_number%d !dest = %d,seq = %d,type = %d,slot = %f\n",g_mac_node_id,g_mac_slot_number,dest,seq,type1,op_sim_time());				
				op_prg_log_entry_write(g_mac_down_debug_log_handle, "Node %d send a data packet at g_mac_slot_number%d !dest = %d,seq = %d,type = %d\n",g_mac_node_id,g_mac_slot_number,dest,seq,type1);
				pk_create_time= op_pk_creation_time_get(pk);
				copy = op_pk_copy(pk);
				op_pk_creation_time_set(copy,pk_create_time);
				op_pk_fd_get (pk, 0, &framecontrol) ;
				pk_size = op_pk_total_size_get(pk);
				sending_delay(pk_size);
				pk_id = op_pk_id(pk);
				route_sink (pk);
				op_pk_send(pk, OUT_MAC_PHY);
				if(type1 == 3)
					{
					globle_down_link_cost = globle_down_link_cost+pk_size;
					down_link_cost = down_link_cost+pk_size;
					}	
				}	
			else
				{											//���Ͷ���Ϊ�գ���ʱ϶û��֡��Ҫ����
				op_intrpt_schedule_self(op_sim_time(), Send_Complete_Code);
				FOUT;
				}
			}		
		AR = (framecontrol>>10)&1;
		if(AR == 1)
			{
			down_packet_number++;
			globle_down_packet_number++;
			if(count+1<5)
				{
				op_subq_pk_insert(Retrans_Queue,copy,OPC_QPOS_TAIL);
				num_pkts = op_subq_stat (Retrans_Queue, OPC_QSTAT_PKSIZE);
				retrans[num_pkts-1].time = RETRANS_TIME;
				retrans[num_pkts-1].count = count+1;			
				printf("Node %d ���д�ȷ�϶��а�����  %d\n",g_mac_node_id,num_pkts);
				op_prg_log_entry_write(g_mac_down_debug_log_handle,"Node %d ���д�ȷ�϶��а�����  %d\n",g_mac_node_id,num_pkts);
				}
			}
		else
			{
			op_pk_destroy(copy);
			}
		FOUT;
		}
	}


//���յ����ӽڵ㴫��������֡������ACKƬ��
static void			create_sub_ack(Packet* pk,int n)
	{
	int pk_source;
	int pk_dest;
	int pk_seq;
	int pk_panid;
	int num_pkts;
	int source_mode;
	int need_seq;
	Packet* ack;
	Packet* ack_in_queue;
	int ack_dest;
	int ack_seq;
	int ack_dest_mode;
	int NWKcontrol;
	int k;
	int framecontrol;
	FIN(create_sub_ack(Packet* pk,int n));
	ack = op_pk_create(0);
	op_pk_fd_get(pk,0,&framecontrol);
	op_pk_fd_get(pk,1,&pk_seq);
	op_pk_fd_get(pk,2,&pk_panid);
	op_pk_fd_get(pk,3,&pk_dest);
	op_pk_fd_get(pk,4,&pk_source);
	op_pk_fd_get(pk,7,&NWKcontrol);
	source_mode = framecontrol&3;
	if(((framecontrol>>13)==DATA_FRAME)&&(NWKcontrol>>12)==13)//�Ǹ����ݽڵ�İ�
		{
		g_mac_nwk_synch_seq;
		}
	num_pkts = op_subq_stat (ACK_Queue, OPC_QSTAT_PKSIZE);
	for(k=0;k<num_pkts;k++)
		{
		ack_in_queue = op_subq_pk_access (ACK_Queue, k);
		op_pk_fd_get(ack_in_queue,2,&ack_seq);
		op_pk_fd_get(ack_in_queue,1,&ack_dest);
		op_pk_fd_get(ack_in_queue,0,&ack_dest_mode);
		if((ack_seq == pk_seq)&&(source_mode == ack_dest_mode)&&(next_hop(ack_dest)==next_hop(pk_source)))
			{
			printf("ack packet is already in ack queue!!!\n");
			FOUT;
			}
		}	
	need_seq = (neighbor_table[n].seq[0]+1)%256;		
	if(source_mode == SHORT_ADDRESS_MODE)
		op_pk_fd_set(ack,1,OPC_FIELD_TYPE_INTEGER,pk_source,16);
	else if(source_mode == ESN_ADDRESS_MODE)
		op_pk_fd_set(ack,1,OPC_FIELD_TYPE_INTEGER,pk_source,48);
	
	op_pk_fd_set(ack,0,OPC_FIELD_TYPE_INTEGER,source_mode,2);
	op_pk_fd_set(ack,2,OPC_FIELD_TYPE_INTEGER,pk_seq,8);
	op_pk_fd_set(ack,3,OPC_FIELD_TYPE_INTEGER,need_seq,8);
	printf("Node %d create sub_ack packet pk_seq is %d!!!\n",g_mac_node_id,pk_seq);
	if (op_subq_pk_insert(ACK_Queue, ack, OPC_QPOS_TAIL) != OPC_QINS_OK)
		{
		printf("\n###Node %d:::data package from route insert into queue failed###\n", g_mac_node_id);
		op_pk_destroy(pk);
		}
	num_pkts = op_subq_stat (ACK_Queue, OPC_QSTAT_PKSIZE);
	if(num_pkts <= MAX_ACK_NUM)
		{
		ack_time[num_pkts-1] = ACK_TIME;
		}	
	else
		{
		printf("Node %d up ack queue cannot push in!!!\n",g_mac_node_id);
		op_prg_log_entry_write(g_mac_down_debug_log_handle, "Node %d up ack queue cannot push in!!!\n",g_mac_node_id);
		}
	FOUT;
	}


//����ACK֡ʱ��ACKƬ�ξۺϳ�Ϊһ��������ACK�ۺ�֡
static void			make_ack_frame()
	{
	Packet* ack_frame;
	Packet* sub_ack;
	int ack_nums;
	int length;
	int framecontrol;
	int pk_panid;
	int i;
	int ack_seq;
	int ack_dest;
	int pk_size;
	int count;
	FIN(make_ack_frame());
	ack_frame = op_pk_create(0);
	if(g_mac_node_type == GATE_NODE)
		{
		framecontrol =  18977;//0100 1010 0010 0001�̰�Ŀ�ĵ�ַ����Դ��ַ
		}
	else
		{
		framecontrol =  18978;//0100 1010 0010 0010�̰�Ŀ�ĵ�ַ����Դ��ַ
		op_pk_fd_set(ack_frame,4,OPC_FIELD_TYPE_INTEGER,g_mac_short_address,16);
		}
	pk_panid = g_mac_pan_id;
	op_pk_fd_set(ack_frame,0,OPC_FIELD_TYPE_INTEGER,framecontrol,16 );
	op_pk_fd_set(ack_frame,1,OPC_FIELD_TYPE_INTEGER,0,0 );
	op_pk_fd_set(ack_frame,2,OPC_FIELD_TYPE_INTEGER,pk_panid,16);
	op_pk_fd_set(ack_frame,3,OPC_FIELD_TYPE_INTEGER,65535,16);
	ack_nums = op_subq_stat (ACK_Queue, OPC_QSTAT_PKSIZE);
	printf("ack queue ack_num = %d\n",ack_nums);
	count = 0;
	for(i = 0;i < ack_nums;i++)
		{
		//printf("i = %d,ack_time[0] = %d\n",i,ack_time[0]);
		count++;
		if(ack_time[0] <= 0)
			{
			//printf("ack_num1 = %d\n",ack_nums);
			sub_ack = op_subq_pk_remove (ACK_Queue, 0);
			op_pk_fd_get(sub_ack,2,&ack_seq);
			op_pk_fd_get(sub_ack,1,&ack_dest);
			printf("ack_dest = %d;;;\t ack_seq = %d\n",ack_dest,ack_seq);
			length = op_pk_total_size_get(sub_ack);
			op_pk_fd_set(ack_frame,(11+i),OPC_FIELD_TYPE_PACKET,sub_ack,length);
			update_ack_time();
			ack_nums--;
			if(ack_nums == i)
				{
				op_pk_fd_set(ack_frame,10,OPC_FIELD_TYPE_INTEGER,count,3);			
				pk_size = op_pk_total_size_get(ack_frame);
				//printf("pk_size1 = %d\n",pk_size);
				sending_delay(pk_size);
				
				globle_down_link_cost = globle_down_link_cost+pk_size;
				down_link_cost = down_link_cost+pk_size;
				
				op_pk_send(ack_frame,OUT_MAC_PHY);
				//printf("send1111\n");
				FOUT;
				}
			i--;
			}
		else
			{				
			op_pk_fd_set(ack_frame,10,OPC_FIELD_TYPE_INTEGER,count,3);			
			pk_size = op_pk_total_size_get(ack_frame);
			//if(g_mac_node_id == 0) printf("pk_size2 = %d\n",pk_size);
			sending_delay(pk_size);
			
			globle_down_link_cost = globle_down_link_cost+pk_size;
			down_link_cost = down_link_cost+pk_size;
			
			op_pk_send(ack_frame,OUT_MAC_PHY);
			//if(g_mac_node_id == 0) printf("send2222\n");
			FOUT;
			}
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

n ָ�ӽڵ�����
*/

static void			create_ack(Packet* pk,int i,int n)
	{
	int pk_source;
	int pk_dest;
	int pk_seq;
	int pk_panid;
	int framecontrol;
	int num_pkts;
	int source_mode;
	int dest_mode;
//	int i;
	int k;
	int need_seq;
	Packet* ack;
	Packet* ack_in_queue;
	int ack_source;
	int ack_dest;
	int ack_seq;
	int NWKcontrol;
	FIN(create_ack(Packet* pk,int i,int n));
	ack = op_pk_create(0);//80
	op_pk_fd_get(pk,0,&framecontrol);
	op_pk_fd_get(pk,1,&pk_seq);
	op_pk_fd_get(pk,2,&pk_panid);
	op_pk_fd_get(pk,3,&pk_dest);
	op_pk_fd_get(pk,4,&pk_source);
	op_pk_fd_get(pk,7,&NWKcontrol);
	if(((framecontrol>>13)==DATA_FRAME)&&(NWKcontrol>>12)==13)//�Ǹ����ݽڵ�İ�
		{
		g_mac_nwk_synch_seq;
		}
	num_pkts = op_subq_stat (ACK_Queue, OPC_QSTAT_PKSIZE);
	for(k=0;k<num_pkts;k++)
		{
		ack_in_queue = op_subq_pk_access (ACK_Queue, k);
		op_pk_fd_get(ack_in_queue,1,&ack_seq);
		op_pk_fd_get(ack_in_queue,4,&ack_source);
		op_pk_fd_get(ack_in_queue,3,&ack_dest);
		if((ack_seq == pk_seq)&&(pk_dest==ack_source)&&(ack_dest==pk_source))
			{
			printf("ack packet is already in ack queue!!!\n");
			FOUT;
			}
		
		}
	
	source_mode = framecontrol&3;
	dest_mode	=(framecontrol>>4)&3;
	switch (i)
		{
		case 1://source_mode = 2,dest_mode = 2
		//ack = op_pk_create(0);//80
		framecontrol = 16930;
		op_pk_fd_set(ack,3,OPC_FIELD_TYPE_INTEGER,pk_source,16);
		op_pk_fd_set(ack,4,OPC_FIELD_TYPE_INTEGER,pk_dest,16);
		break;
		
		case 2://source_mode = 2,dest_mode = 1
		//ack = op_pk_create(0);//64
		framecontrol = 16914;
		op_pk_fd_set(ack,3,OPC_FIELD_TYPE_INTEGER,pk_source,0);//ooooooo
		op_pk_fd_set(ack,4,OPC_FIELD_TYPE_INTEGER,pk_dest,16);
		break;
		
		case 3://source_mode = 3,dest_mode = 2
		//ack = op_pk_create(0);//112
		framecontrol = 16931;
		op_pk_fd_set(ack,3,OPC_FIELD_TYPE_INTEGER,pk_source,16);
		op_pk_fd_set(ack,4,OPC_FIELD_TYPE_INTEGER,pk_dest,48);
		break;
		
		case 4://source_mode = 3,dest_mode = 1
		//ack = op_pk_create(0);//96
		framecontrol = 16915;
		op_pk_fd_set(ack,3,OPC_FIELD_TYPE_INTEGER,pk_source,0);//ooooooo
		op_pk_fd_set(ack,4,OPC_FIELD_TYPE_INTEGER,pk_dest,48);
		break;
		
		case 5://source_mode = 2,dest_mode = 2
		//ack = op_pk_create(0);//80
		framecontrol = 16930;
		op_pk_fd_set(ack,3,OPC_FIELD_TYPE_INTEGER,pk_source,16);
		op_pk_fd_set(ack,4,OPC_FIELD_TYPE_INTEGER,pk_dest,16);
		break;
		
		case 6://source_mode = 2,dest_mode = 3
		//ack = op_pk_create(0);//112
		framecontrol = 16946;
		op_pk_fd_set(ack,3,OPC_FIELD_TYPE_INTEGER,pk_source,48);
		op_pk_fd_set(ack,4,OPC_FIELD_TYPE_INTEGER,pk_dest,16);
		break;
			
		case 7://source_mode = 1,dest_mode = 2
		//ack = op_pk_create(0);//64
		framecontrol = 16929;
		op_pk_fd_set(ack,3,OPC_FIELD_TYPE_INTEGER,pk_source,16);
		op_pk_fd_set(ack,4,OPC_FIELD_TYPE_INTEGER,pk_dest,0);//ooooooo
		break;
		
		case 8://source_mode = 1,dest_mode = 3	
		//ack = op_pk_create(0);//112
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
	
	for(i = 1; i< MAX_SEQUENCE_NUM;i++)
		{
		if(neighbor_table[n].seq[i] == 0)
			{
			break;
			}
		}
	need_seq = (neighbor_table[n].seq[0]+i)%256;	
	op_pk_fd_set(ack,5,OPC_FIELD_TYPE_INTEGER,need_seq,8);//����Ҫ��֡�����кŷ���ȥ
	//if((i == 1)||(i == 2)||(i == 3)||(i == 4))
		{
		if (op_subq_pk_insert(ACK_Queue, ack, OPC_QPOS_TAIL) != OPC_QINS_OK)
			{
			printf("\n###Node %d:::data package from route insert into queue failed###\n", g_mac_node_id);
			op_pk_destroy(pk);
			}
		num_pkts = op_subq_stat (ACK_Queue, OPC_QSTAT_PKSIZE);
		if(num_pkts<=MAX_ACK_NUM)
			{
			ack_time[num_pkts-1] = ACK_TIME;
			}	
		else
			{
			printf("Node %d up ack queue cannot push in!!!\n",g_mac_node_id);
			op_prg_log_entry_write(g_mac_down_debug_log_handle, "Node %d up ack queue cannot push in!!!\n",g_mac_node_id);

			}
		}
	FOUT;
	}




/*********************************************/
/*********************************************/
/*��MAC ����֡�Ĵ���*/
/*********************************************/
/*********************************************/
static void mac_frame_proc(Packet* pk)
	{
//	Packet* packet;
	int 	i;
	int 	framecontrol;
	int		seq;
	int		type;
	int 	subcontrol;
	int		length;
	int		subtype;
	int 	dest;
	int 	source;
	int 	j;
	int 	IE;
	FIN(mac_frame_proc(Packet* pk));
	op_pk_fd_get(pk,0,&framecontrol);
	op_pk_fd_get(pk,1,&seq);
	op_pk_fd_get(pk,3,&dest);
	op_pk_fd_get(pk,4,&source);
	op_pk_fd_get(pk,5,&length);
	type = (framecontrol>>13)&7;
	IE = (framecontrol>>6)&1; 
		{
		op_pk_fd_get(pk,7,&subcontrol);
		subtype = (subcontrol>>4)&15;
		if(subtype == KEEP_ALIVE_FRAME)
			{
			printf("Node %d receive a KEEP_ALIVE_FRAME frame at %f\n",g_mac_node_id,op_sim_time());
			op_prg_log_entry_write(g_mac_down_debug_log_handle,"Node %d receive an MAC KEEP_ALIVE_FRAME\n",g_mac_node_id);	
			if(IE == 1) get_IES(pk);
			for(i = 0;i<CHILDREN_NODE_NUM ;i++)
				{
				if(neighbor_table[i].node_address == source)
					{
					neighbor_table[i].active_time = ACTIVE_TIME;
					create_sub_ack(pk, i);
					break;
					}
				}
			}
		if(subtype == ASSOCIATE_REQ_FRAME && g_mac_node_status == ONLINE)				//��������֡����
			{
			printf("Node %d receive an ASSOCIATE_REQ_FRAME frame at %f\n",g_mac_node_id,op_sim_time());
			op_prg_log_entry_write(g_mac_down_debug_log_handle,"Node %d receive an MAC ASSOCIATE_REQ_FRAME\n",g_mac_node_id);	
			for(i = 0;i<CHILDREN_NODE_NUM ;i++)
				{
				
				if(neighbor_table[i].node_id == 0)
					{
					printf("i = %d\n",i);
					//printf("00000\n");
					neighbor_table[i].node_id = source;
					neighbor_table[i].band = g_mac_down_band;
					neighbor_table[i].frequency = g_mac_recv_frequency;
					neighbor_table[i].seq[0] = seq;
					neighbor_table[i].g_mac_seq = (int)op_dist_uniform(255); //Ϊ���ӽڵ��ʼ��һ�����к�
					for(j = 1;j<MAX_SEQUENCE_NUM;j++)
						{
						neighbor_table[i].seq[j] = 0;
						}
					neighbor_table[i].active_time = ACTIVE_TIME;
					create_associate_response_frame(source,2);//�����ɹ�
					//printf("neighbor_table[%d].node_id = %d\n",i,neighbor_table[i].node_id);
					break;
					}
				else
					{
					if( i == CHILDREN_NODE_NUM-1)
						{
						create_associate_response_frame(source,1);//����ʧ��
						}
					}
				}
			}
		}
	op_pk_destroy(pk);
	FOUT;
	}




/*********************************************/
/*********************************************/
/*��ACK֡�Ĵ���*/
/*********************************************/
/*********************************************/
static void ack_frame_proc(Packet* pk)
	{
	int need_seq;
	int AR;		
	int i;
	int framecontrol;
	int dest_mode ;
	int source_mode;
	int seq;
	int dest;
	int source;
	FIN(ack_frame_proc(Packet* pk));
	op_pk_fd_get (pk, 0, &framecontrol) ;
	op_pk_fd_get (pk, 1, &seq) ;
	op_pk_fd_get (pk, 3, &dest);
	op_pk_fd_get (pk, 4, &source);
	op_pk_fd_get (pk, 5, &need_seq);
	AR = (framecontrol>>10)&1;
	dest_mode = (framecontrol>>4)&3;
	source_mode = framecontrol&3;	
	for(i = 0;i<CHILDREN_NODE_NUM;i++)
		{
		if(source == keepAlive[i].dest)		//һ���ڵ㱣��
			{
			if(seq == keepAlive[i].seq)
				{
				//��ɱ���ԭ��
				MLME_ALIVE_CONFIRM(source,0);
				keepAlive[i].dest = 0;
				keepAlive[i].seq = 0;
				break;
				}
			}
		}	
//	printf("Node %d receive an ack packet and process it!!!\n",g_mac_node_id);
	op_prg_log_entry_write(g_mac_down_debug_log_handle,"Node %d receive an ack packet and process it!!!\n",g_mac_node_id);
	ack_proc(seq,source,dest,need_seq);		
	op_pk_destroy(pk);
	FOUT;
	}


/*********************************************/
/*********************************************/
/*��ACK֡�Ĵ���
seq:   seq of ACK packet,the same as the packet to be ACK
dest:  dest of ACK packet,the same as the source of the packet to be confirm*/
/*********************************************/
/*********************************************/
static void ack_proc(int seq ,int source ,int dest,int need_seq)
	{
	int 	i,j;
	Packet* packet;
	int 	ack;				//���ڴ���ش������ж�ȡ�İ������к�
	int 	number;
	int 	data_source;		//��ȷ�϶�����֡��Դ�ڵ�
	int		data_dest;			//��ȷ�϶�����֡��Ŀ�Ľڵ�
	int find_ack = 0;
	FIN(ack_proc(int seq ,int source ,int dest, int need_seq));
	number = op_subq_stat (Retrans_Queue, OPC_QSTAT_PKSIZE);
		
	for(i=0;i<number;i++)
		{
		packet = op_subq_pk_access (Retrans_Queue, i);
		op_pk_fd_get(packet,1,&ack);
		op_pk_fd_get(packet,4,&data_source);
		op_pk_fd_get(packet,3,&data_dest);
		if((ack == seq)&&(data_source == dest)&&(data_dest == source))
			{
			find_ack = 1;
			packet = op_subq_pk_remove (Retrans_Queue, i);
			//ɾ��������****************************************
			for(j = i;j < MAX_RETRANS_NUM-1;j++)
				{
				retrans[j].time = retrans[j+1].time;
				retrans[j].count = retrans[j+1].count;
				}
			retrans[MAX_RETRANS_NUM-1].time = 100;
			retrans[MAX_RETRANS_NUM-1].count = 0;
			down_receive_ack_number++;
			globle_down_receive_ack_number++;
			printf("\tNODE %dfind the packet!!!,seq = %d\n",g_mac_node_id,seq);
			op_prg_log_entry_write(g_mac_down_debug_log_handle,"Node %d find the packet!!!,seq = %d\n",g_mac_node_id,seq);
			//op_pk_destroy(packet);
			i--;
			number--;
			//continue;;
			break;
			}
		}
	if(find_ack == 1)
		{
		if(next_hop(data_dest)==next_hop(source)&&(ack-need_seq)<-250) ack = ack+256;
		else if(next_hop(data_dest)==next_hop(source)&&(need_seq<5&&ack>250)) ack = ack-256;
		number = op_subq_stat (Retrans_Queue, OPC_QSTAT_PKSIZE);
		for(i=0;i<number;i++)
			{
			packet = op_subq_pk_access (Retrans_Queue, i);
			op_pk_fd_get(packet,1,&ack);
			if(ack<need_seq)
				{
				down_receive_ack_number++;
				globle_down_receive_ack_number++;
				packet = op_subq_pk_remove (Retrans_Queue, i);
				//ɾ��������****************************************
				for(j = i;j < MAX_RETRANS_NUM-1;j++)
					{
					retrans[j].time = retrans[j+1].time;
					retrans[j].count = retrans[j+1].count;
					}
				retrans[MAX_RETRANS_NUM-1].time = 100;
				retrans[MAX_RETRANS_NUM-1].count = 0;
				//op_pk_destroy(packet);
				i--;
				number--;
				continue;
				}		
			}	
		}
	FOUT;
	}



/*********************************************/
/*********************************************/
/*��ѯ·�ɱ�*/
/*********************************************/
/*********************************************/
static int next_hop(int dest)
	{
	int i = 0;
	int router;
	int gate;
	FIN(next_hop(int dest));
	if(dest == 0) FRET(43690);	
	router = dest&(255<<8);
	gate = dest&(1<<15);
	if(gate > 0)
		{
		FRET(dest);
		}
	if(router == g_mac_short_address)
		{
		FRET(dest);
		}
	for(i = 0;i < Network_Msg.NetworkRT[g_mac_node_id].number;i++)
		{
		if(((Network_Msg.NetworkRT[g_mac_node_id].table[i].destAddress)<<8) == router && Network_Msg.NetworkRT[g_mac_node_id].table[i].status !=2 )
			{
			FRET((Network_Msg.NetworkRT[g_mac_node_id].table[i].nextHop)<<8);
			}
		}
	FRET(-1);
	}




/*********************************************/
/*********************************************/
/*���±���ʱ�䣬�ڵ㱣��*/
/*********************************************/
/*********************************************/
static void update_alive_time(int source)
	{
	int i;
	int next;
	FIN(update_alive_time(int source));
	//printf("update_alive_time\n");
	next = next_hop(source);
	for(i = 0;i<CHILDREN_NODE_NUM;i++)
		{
		if(next == neighbor_table[i].node_address)
			{
			neighbor_table[i].active_time = ACTIVE_TIME;
			break;
			}
		}
	FOUT;
	}




/*********************************************/
/*********************************************/
//����������ϢMAC����֡
//��������֡�ڱ�ʱ϶����
/*********************************************/
/*********************************************/
static void	create_actice_frame(int dest,int seq)
	{
	Packet* alive;
	int 	frame_control;
	int		panid;
	int		source;
	int		subcontrol;
	int 	length;
	int 	k;
	int 	i;
	int 	num_pkts;
	FIN(create_actice_frame(int dest,int seq));
	printf("Node %d create_actice_frame dest is %d\n",g_mac_node_id,dest);
	op_prg_log_entry_write(g_mac_down_debug_log_handle,"Node %d create_actice_frame dest is %d\n",g_mac_node_id,dest);
	alive = op_pk_create(0);//88
	frame_control	= 26146;			//011 0011000 10 00 10
	//seq = g_mac_sequence;
	//g_mac_sequence = (g_mac_sequence+1)%256;
	//if(seq == -1) seq = get_sequence_to_node(dest,SHORT_ADDRESS_MODE);
	seq = get_sequence_to_node(dest,SHORT_ADDRESS_MODE);
	panid 			= g_mac_pan_id;
	source 			= g_mac_short_address;
	length			= 88;
	subcontrol 		= 32;
	activeFrameSeq	= seq; 
	op_pk_fd_set (alive, 0, OPC_FIELD_TYPE_INTEGER,frame_control,16) ;
	op_pk_fd_set (alive, 1, OPC_FIELD_TYPE_INTEGER,seq,8) ;
	op_pk_fd_set (alive, 2, OPC_FIELD_TYPE_INTEGER,panid,16) ;
	op_pk_fd_set (alive, 3, OPC_FIELD_TYPE_INTEGER,dest,16) ;
	op_pk_fd_set (alive, 4, OPC_FIELD_TYPE_INTEGER,source,16) ;
	op_pk_fd_set (alive, 5, OPC_FIELD_TYPE_INTEGER,length,8) ;
	op_pk_fd_set (alive, 7, OPC_FIELD_TYPE_INTEGER,subcontrol,8) ;
	if (op_subq_pk_insert(Data_Queue, alive, OPC_QPOS_TAIL) != OPC_QINS_OK)
		{
		printf("\n###Node %d:::data package from route insert into queue failed###\n", g_mac_node_id);
		op_prg_log_entry_write(g_mac_down_debug_log_handle,"Node %d:::data package from route insert into queue failed###\n", g_mac_node_id);
		op_pk_destroy(alive);
		}
	num_pkts = op_subq_stat (Data_Queue, OPC_QSTAT_PKSIZE);
	k = CHILDREN_NODE_NUM;	
	for(i = 0;i<CHILDREN_NODE_NUM;i++)
		{
		if(keepAlive[i].dest == dest)//�ýڵ��Ѿ��ڱ�����
			{
			FOUT;
			}
		if(keepAlive[i].dest == 0)
			{
			if(k>i) k = i;
			}
		}
	keepAlive[k].dest = dest;
	keepAlive[k].seq = activeFrameSeq;	
	FOUT;
	}



/*********************************************/
/*********************************************/
/*Get IEs*/
/*********************************************/
/*********************************************/
static void get_IES(Packet* pk)
	{
	FIN(get_IES(Packet* pk));
	op_pk_fd_get(pk,6,&MulticastContent);
	MLME_MULTICAST_CONFIRM();
	FOUT;
	}



static void set_channel_start_send ()
	{
	FIN(set_channel_start_send());
	op_strm_flush(IN_PHY_MAC);
	g_mac_receiving_flag = SENDING;
	FOUT;
	}
static void set_channel_start_receive ()
	{
	FIN(set_channel_start_receive());
	op_strm_flush(IN_PHY_MAC);
	g_mac_receiving_flag = RECEIVING;
	//if(g_mac_node_id == 0) printf("send complete,start receive at %f\n",op_sim_time());
	FOUT;
	}


static void route_sink (Packet* pk)
	{
	int framecontrol;
	int type;
	int subtype;
	int nwk_head;
	double pk_size;
	FIN(route_sink(Packet* pk));
	op_pk_fd_get (pk, 0, &framecontrol) ;
	type = (framecontrol>>13)&7;
	pk_size = op_pk_total_size_get(pk);	
	if(type == DATA_FRAME)
		{
		op_pk_fd_get (pk, 7, &nwk_head) ;
		subtype = (nwk_head>>12)&15;
		if(subtype == 0||subtype == 1||subtype == 2||subtype == 3||subtype == 4||subtype == 6||subtype == 11||subtype == 12||subtype == 13||subtype == 14)
			{
			globle_down_route_cost = globle_down_route_cost+pk_size;
			down_route_cost = down_route_cost+pk_size;
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
	void WSN_MAC_DOWN2 (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_WSN_MAC_DOWN2_init (int * init_block_ptr);
	void _op_WSN_MAC_DOWN2_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_WSN_MAC_DOWN2_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_WSN_MAC_DOWN2_alloc (VosT_Obtype, int);
	void _op_WSN_MAC_DOWN2_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
WSN_MAC_DOWN2 (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (WSN_MAC_DOWN2 ());

		{


		FSM_ENTER ("WSN_MAC_DOWN2")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "init", "WSN_MAC_DOWN2 [init enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_DOWN2 [init enter execs]", state0_enter_exec)
				{
				PrgT_Log_Severity_Type   severity;
				Compcode                 status;
				Objid				receiver_from_children1;
				Objid				receiver_from_children2;
				Objid				receiver_from_children3;
				Objid				channel_of_recvive_from_children1;
				Objid				channel_of_recvive_from_children2;
				Objid				channel_of_recvive_from_children3;
				Objid				receive_from_children_comp1;
				Objid				receive_from_children_comp2;
				Objid				receive_from_children_comp3;
				double a;
				double b;
				double c;
				
				
				//Packet* pk;
				
				
				g_mac_process_objid = op_id_self();															//��ȡ���ڵ��ʵ��
				g_mac_node_objid    = op_topo_parent(g_mac_process_objid);
				//subnet_objid     = op_topo_parent (my_node_objid);			
				op_ima_obj_attr_get (g_mac_node_objid,	"g_node_ESN_address",	&g_mac_node_id);
				op_ima_obj_attr_get (g_mac_node_objid,	"g_node_type"	,	&g_mac_node_type);
				op_ima_obj_attr_get (g_mac_node_objid,	"g_node_status",	&g_mac_node_status);
				op_ima_obj_attr_get (g_mac_node_objid,	"g_node_error_rate",	&frame_error_rate);
				
				
				
				
				//printf("���ڵ��MAC��ַ��%d\n",g_mac_node_id);
				
				
				receiver_from_children = op_id_from_name (g_mac_node_objid, OPC_OBJTYPE_RARX , "receive from children");
				sender_to_children = op_id_from_name (g_mac_node_objid, OPC_OBJTYPE_RATX , "send to children");
				
				receive_from_children_comp = op_topo_child (receiver_from_children, OPC_OBJTYPE_COMP , 0);
				sender_to_children_comp = op_topo_child (sender_to_children, OPC_OBJTYPE_COMP , 0); 
				
				channel_of_recvive_from_children = op_topo_child (receive_from_children_comp, OPC_OBJTYPE_RARXCH , 0); 
				channel_of_send_to_children = op_topo_child (sender_to_children_comp, OPC_OBJTYPE_RATXCH , 0); 	
				mac_init();
				
				
				
				if(g_mac_node_type == GATE_NODE)
					{
					receiver_from_children1 = op_id_from_name (g_mac_node_objid, OPC_OBJTYPE_RARX , "receive from children1");
					receiver_from_children2 = op_id_from_name (g_mac_node_objid, OPC_OBJTYPE_RARX , "receive from children2");
					receiver_from_children3 = op_id_from_name (g_mac_node_objid, OPC_OBJTYPE_RARX , "receive from children3");
					receive_from_children_comp1 = op_topo_child (receiver_from_children1, OPC_OBJTYPE_COMP , 0);
					receive_from_children_comp2 = op_topo_child (receiver_from_children2, OPC_OBJTYPE_COMP , 0);
					receive_from_children_comp3 = op_topo_child (receiver_from_children3, OPC_OBJTYPE_COMP , 0);
					channel_of_recvive_from_children1 = op_topo_child (receive_from_children_comp1, OPC_OBJTYPE_RARXCH , 0); 
					channel_of_recvive_from_children2 = op_topo_child (receive_from_children_comp2, OPC_OBJTYPE_RARXCH , 0); 
					channel_of_recvive_from_children3 = op_topo_child (receive_from_children_comp3, OPC_OBJTYPE_RARXCH , 0); 
					op_ima_obj_attr_set (channel_of_recvive_from_children1,"min frequency",471.0);
					op_ima_obj_attr_set (channel_of_recvive_from_children2,"min frequency",472.0);
					op_ima_obj_attr_set (channel_of_recvive_from_children3,"min frequency",473.0);
					op_ima_obj_attr_get (channel_of_recvive_from_children1,"min frequency",&a);
					op_ima_obj_attr_get (channel_of_recvive_from_children2,"min frequency",&b);
					op_ima_obj_attr_get (channel_of_recvive_from_children3,"min frequency",&c);
					printf("a = %f,b = %f,c = %f\n",a,b,c);
					
					}
				
				pkt_success_stathandle		=op_stat_reg ("Traffic Sink.down_pk_success",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				g_pkt_success_stathandle	=op_stat_reg ("Traffic Sink.down_g_pk_success",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				route_cost_stathandle		=op_stat_reg ("Traffic Sink.down_route_cost",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				g_route_cost_stathandle		=op_stat_reg ("Traffic Sink.down_g_route_cost",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				link_cost_stathandle		=op_stat_reg ("Traffic Sink.down_link_cost",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				g_link_cost_stathandle		=op_stat_reg ("Traffic Sink.down_g_link_cost",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				net_cost_stathandle			=op_stat_reg ("Traffic Sink.down_net_cost",			OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				g_net_cost_stathandle		=op_stat_reg ("Traffic Sink.down_g_net_cost",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				all_cost_stathandle			=op_stat_reg ("Traffic Sink.down_all_cost",			OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				g_all_cost_stathandle		=op_stat_reg ("Traffic Sink.down_g_all_cost",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				packet_number_stathandle	=op_stat_reg ("Traffic Sink.down_packet_number",	OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				g_packet_number_stathandle	=op_stat_reg ("Traffic Sink.down_g_packet_number",	OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				down_success_rate			=op_stat_reg ("Traffic Sink.down_success_rate",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				g_down_success_rate			=op_stat_reg ("Traffic Sink.down_g_success_rate",	OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				
				g_mac_down_info_log_handle = op_prg_log_handle_create(OpC_Log_Category_Configuration, "MAC_DOWN", "info", 1);
				status = op_prg_log_handle_severity_get (g_mac_down_info_log_handle, &severity);
				if ((status != OPC_COMPCODE_FAILURE) && (severity != PrgC_Log_Severity_Information))
				   op_prg_log_handle_severity_set (&g_mac_down_info_log_handle, PrgC_Log_Severity_Information); 
				
				g_mac_down_debug_log_handle = op_prg_log_handle_create(OpC_Log_Category_Configuration, "MAC_DOWN", "debug", 1);
				status = op_prg_log_handle_severity_get (g_mac_down_debug_log_handle, &severity);
				if ((status != OPC_COMPCODE_FAILURE) && (severity != PrgC_Log_Severity_Debug))
				   op_prg_log_handle_severity_set (&g_mac_down_debug_log_handle, PrgC_Log_Severity_Debug); 
				
				
				op_prg_log_entry_write(g_mac_down_info_log_handle, "Node %d down mac start\n",g_mac_node_id );
				
				//contr_send_num=op_stat_reg("group1.pk_contr_number",OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				//sendNum = 0;
				//op_intrpt_schedule_self(op_sim_time(), Start_Code);
				
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** state (init) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "init", "WSN_MAC_DOWN2 [init exit execs]")


			/** state (init) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "init", "idle", "tr_37", "WSN_MAC_DOWN2 [init -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "idle", state1_enter_exec, "WSN_MAC_DOWN2 [idle enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_DOWN2 [idle enter execs]", state1_enter_exec)
				{
				/*
				if((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == UpTime_Code))
					{
					if(packet_flag == 2)
						{
						op_intrpt_schedule_self(op_sim_time(), Up_Code);
						}
					}
				*/
				g_mac_sending_flag = 2;
				if(C_SENDING_COMPLETE)
					{
					//printf("Node %d send complete at %f\n",g_mac_node_id,op_sim_time());
					//op_prg_log_entry_write(g_mac_down_debug_log_handle, "Node %d send complete\n",g_mac_node_id);
					}
				if(g_mac_leaving == MAC_LEAVING)
					{
					cheak_mac_queue();
					}
				}
				FSM_PROFILE_SECTION_OUT (state1_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"WSN_MAC_DOWN2")


			/** state (idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "idle", "WSN_MAC_DOWN2 [idle exit execs]")


			/** state (idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_MAC_DOWN2 [idle trans conditions]", state1_trans_conds)
			FSM_INIT_COND (C_SELF_NEW_SLOT)
			FSM_TEST_COND (C_STRM_MSG_FROM_CHILD_NODE)
			FSM_TEST_COND (C_STRM_NWK_DATA)
			FSM_TEST_COND (C_STRM_NWK_MLME)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("idle")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 4, state4_enter_exec, set_channel_start_send ();, "C_SELF_NEW_SLOT", "set_channel_start_send ()", "idle", "slot_intrpt", "tr_24", "WSN_MAC_DOWN2 [idle -> slot_intrpt : C_SELF_NEW_SLOT / set_channel_start_send ()]")
				FSM_CASE_TRANSIT (1, 3, state3_enter_exec, ;, "C_STRM_MSG_FROM_CHILD_NODE", "", "idle", "msg from down", "tr_29", "WSN_MAC_DOWN2 [idle -> msg from down : C_STRM_MSG_FROM_CHILD_NODE / ]")
				FSM_CASE_TRANSIT (2, 2, state2_enter_exec, ;, "C_STRM_NWK_DATA", "", "idle", "App_Msg_Down", "tr_31", "WSN_MAC_DOWN2 [idle -> App_Msg_Down : C_STRM_NWK_DATA / ]")
				FSM_CASE_TRANSIT (3, 5, state5_enter_exec, ;, "C_STRM_NWK_MLME", "", "idle", "MLME", "tr_35", "WSN_MAC_DOWN2 [idle -> MLME : C_STRM_NWK_MLME / ]")
				FSM_CASE_TRANSIT (4, 1, state1_enter_exec, ;, "default", "", "idle", "idle", "tr_34", "WSN_MAC_DOWN2 [idle -> idle : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (App_Msg_Down) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "App_Msg_Down", state2_enter_exec, "WSN_MAC_DOWN2 [App_Msg_Down enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_DOWN2 [App_Msg_Down enter execs]", state2_enter_exec)
				{
				/* recv app data, save in subqueue */
				int dest = -1;
				int type = -1;
				int id = -1;
				int subtype;
				Packet* pk;
				int framecontrol;
				int nwk_head;
				int dest_mode;
				int	pkt_num;
				int seq;
				if((g_mac_node_type != END_NODE)&&(g_mac_leaving != MAC_LEAVING))
					{
					pk = op_pk_get(IN_NWK_MAC_DATA);
					op_pk_fd_get(pk,0, &framecontrol);
					op_pk_fd_get(pk,3, &dest);
					op_pk_fd_get(pk,7, &nwk_head);
					//seq = g_mac_sequence;
					//g_mac_sequence = (g_mac_sequence+1)%256;
					dest_mode = (framecontrol>>4)&3;
					printf("DEST == %d,dest_mode = %d,next_hop(dest) = %d\n",dest,dest_mode,next_hop(dest));	
					pkt_num = op_subq_stat (Data_Queue, OPC_QSTAT_PKSIZE);
					if(pkt_num <= 5)
						{
						if(dest!=0xfffc&&dest!=0xfffd&&dest!=0xfffe&&dest!=0xffff) 
							{
							seq = get_sequence_to_node(dest,dest_mode);
							}
						else 		//�㲥֡���鲥������к�Ϊ0
							{
							seq = 0;
							}
						op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,seq,8);
						printf("Node %d MAC receive a frame from NWK dest is children node!  seq = %d  ;;;",g_mac_node_id,seq);
						subtype = (nwk_head>>12)&15;
						type = (framecontrol>>13)&7;
						printf("type = %d  ;;;",type);
						printf("subtype = %d  ;;;",subtype);
						printf("dest = %d\n",dest);	
						if (op_subq_pk_insert(Data_Queue, pk, OPC_QPOS_TAIL) != OPC_QINS_OK)
							{
							printf("\n###Node %d:::data package from route insert into queue failed###\n", g_mac_node_id);
							op_prg_log_entry_write(g_mac_down_debug_log_handle, "Node %d:::data package from route insert into queue failed###\n", g_mac_node_id);
							op_pk_destroy(pk);
							}
						op_prg_log_entry_write(g_mac_down_debug_log_handle, "Node %d MAC receive a frame from NWK dest is father node!seq = %d,type = %d,subtype = %d,dest = %d!!!\n",g_mac_node_id,seq,type,subtype,dest);
						}
					else
						{
						printf("Node %d Data_queue is full\n", g_mac_node_id);
						}
					}
					
					
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (App_Msg_Down) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "App_Msg_Down", "WSN_MAC_DOWN2 [App_Msg_Down exit execs]")


			/** state (App_Msg_Down) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_MAC_DOWN2 [App_Msg_Down trans conditions]", state2_trans_conds)
			FSM_INIT_COND (g_mac_sending_flag == 2)
			FSM_TEST_COND (g_mac_sending_flag == 1)
			FSM_TEST_LOGIC ("App_Msg_Down")
			FSM_PROFILE_SECTION_OUT (state2_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, ;, "g_mac_sending_flag == 2", "", "App_Msg_Down", "idle", "tr_30", "WSN_MAC_DOWN2 [App_Msg_Down -> idle : g_mac_sending_flag == 2 / ]")
				FSM_CASE_TRANSIT (1, 4, state4_enter_exec, ;, "g_mac_sending_flag == 1", "", "App_Msg_Down", "slot_intrpt", "tr_54", "WSN_MAC_DOWN2 [App_Msg_Down -> slot_intrpt : g_mac_sending_flag == 1 / ]")
				}
				/*---------------------------------------------------------*/



			/** state (msg from down) enter executives **/
			FSM_STATE_ENTER_FORCED (3, "msg from down", state3_enter_exec, "WSN_MAC_DOWN2 [msg from down enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_DOWN2 [msg from down enter execs]", state3_enter_exec)
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
				int n;
				int mult;
				int IEs;
				int flag = 0;
				int need_ack;
				double error;
				//receice a packet from father node
				if(C_STRM_MSG_FROM_CHILD_NODE && g_mac_receiving_flag == RECEIVING)
				{
				g_mac_no_packet_flag = Receiced;
				op_intrpt_schedule_self(op_sim_time()+Protection_time, Up_Code);
				if((g_mac_node_type == GATE_NODE)||(g_mac_node_type == ROUTE_NODE))				//·�ɽڵ�����ؽڵ�
					{
					pk = op_pk_get(op_intrpt_strm());
					error = op_dist_uniform (1.0);
					if(error<=frame_error_rate)
						{
						op_pk_fd_get (pk, 4, &source);
						if(source == g_mac_node_id)
							{
							op_pk_destroy(pk);
							printf("From father receive a packet from my node!!!Dropped it !!!!!!!!!\n");
							}
						else
							{
							update_alive_time(source);						//��������
							printf("Node %d reveive a packet from down!!!\n",g_mac_node_id);		
							op_pk_fd_get (pk, 0, &framecontrol) ;
							op_pk_fd_get (pk, 1, &seq) ;
							op_pk_fd_get (pk, 3, &dest);
							mult = (framecontrol>>8)&1;
							AR = (framecontrol>>10)&1;
							dest_mode = (framecontrol>>4)&3;
							source_mode = framecontrol&3;
							type = (framecontrol>>13)&7;
							IEs = (framecontrol>>6)&1; 
							printf("seq = %d\t",seq);
							printf("source = %d\t",source);
							printf("dest = %d\t",dest);
							printf("ar= %d\n",AR);
							printf("dest mode = %d\t",dest_mode);
							printf("source_mode = %d\t",source_mode);
							printf("type= %d\n",type);
				//			op_prg_log_entry_write(g_mac_down_debug_log_handle, "Node %d reveive a packet from down!!!seq = %d,source = %d,dest = %d,ar= %d,dest mode = %d,source_mode = %d,type= %d\n",
				//			g_mac_node_id,seq,source,dest,AR,dest_mode,source_mode,type);
							if(type == BEACON_FRAME)
								{									//beacon frame ,	
								op_pk_destroy(pk);
								}
							else if(type == ACK_FRAME)					//ack frame
								{
								if(IEs == 1) get_IES(pk);
								ack_frame_proc(pk);                                                                         
								}
							else if((type == DATA_FRAME||type == MAC_CONTROL_FRAME)&&g_mac_leaving != MAC_LEAVING)
								{
								//printf("Node %d receive an data or MAC control packet from down!!!\n",g_mac_node_id);
								op_prg_log_entry_write(g_mac_down_debug_log_handle,"Node %d receive an data or MAC control packet from down!!!\n",g_mac_node_id);
								need_ack = update_sequence_from_node(source, seq, AR,source_mode);
								//printf("Node %d need_ack = %d\n",g_mac_node_id,need_ack);
								if(need_ack == 3)
									{
									op_pk_destroy(pk);
									}
								else
									{
									if(source_mode == SHORT_ADDRESS_MODE)
										{
										/*Ѱ�����ӽڵ���е�λ��*/
										for(n = 0;n <CHILDREN_NODE_NUM;n++)
											{
											if(neighbor_table[n].node_address == next_hop(source))
												{		
												break;
												}
											}
										/*����֡����*/
										if(type == DATA_FRAME) 
											{
											if(AR == 1)
												{
												create_sub_ack(pk, n);
												}	
											if(need_ack == 1) op_pk_send_delayed (pk, OUT_MAC_NWK_DATA,0.5);
											}
										/*����֡����*/
										else if(type == MAC_CONTROL_FRAME)
											{
											if(need_ack == 1) mac_frame_proc(pk);
											else if(need_ack == 2)
												{
												if(AR == 1)
													create_sub_ack(pk, n);						
												}
											}
										}
									else if(source_mode == ESN_ADDRESS_MODE)
										{
										for(n = 0;n <CHILDREN_NODE_NUM;n++)
											{
											if(neighbor_table[n].node_id == source)
												{	
												break;
												}
											}
										if(type == MAC_CONTROL_FRAME) 
											{
											if(need_ack == 1) mac_frame_proc(pk);
											else if(need_ack == 2)
												{
												if(AR == 1)
													create_sub_ack(pk, n);					
												}
											}
										else if(type == DATA_FRAME)
											{
											if(AR == 1)
												{
												create_sub_ack(pk, n);
												}
											if(need_ack == 1) op_pk_send_delayed (pk, OUT_MAC_NWK_DATA,0.5);
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
					else
						{
						printf("Node %d received an error packet down!!!\n",g_mac_node_id);
						op_pk_destroy(pk);
						}	
					}
				}
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** state (msg from down) exit executives **/
			FSM_STATE_EXIT_FORCED (3, "msg from down", "WSN_MAC_DOWN2 [msg from down exit execs]")


			/** state (msg from down) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "msg from down", "idle", "tr_55", "WSN_MAC_DOWN2 [msg from down -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (slot_intrpt) enter executives **/
			FSM_STATE_ENTER_UNFORCED (4, "slot_intrpt", state4_enter_exec, "WSN_MAC_DOWN2 [slot_intrpt enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_DOWN2 [slot_intrpt enter execs]", state4_enter_exec)
				{
				int i;
				double rate;
				double g_rate;
				
				g_mac_sending_flag = 1;
				if(C_SELF_NEW_SLOT)
					{
					op_stat_write (packet_number_stathandle, 		down_packet_number);
					op_stat_write (pkt_success_stathandle, 			down_receive_ack_number);
					op_stat_write (g_packet_number_stathandle, 		globle_down_packet_number);
					op_stat_write (g_pkt_success_stathandle, 		globle_down_receive_ack_number);
					rate = (double)down_receive_ack_number/(double)down_packet_number;
					g_rate = (double)globle_down_receive_ack_number/(double)globle_down_packet_number;
					op_stat_write (down_success_rate, 			rate);
					op_stat_write (g_down_success_rate, 		g_rate);	
					op_stat_write (link_cost_stathandle, 		down_link_cost);
					op_stat_write (g_link_cost_stathandle, 		globle_down_link_cost);
					op_stat_write (route_cost_stathandle, 		down_route_cost);
					op_stat_write (g_route_cost_stathandle, 	globle_down_route_cost);
					
					
					g_mac_slot_start_time = op_sim_time();
					if(g_mac_node_id == 0)
						{
						//printf("new slot start at %f\n",g_mac_slot_start_time);
						/*for(i = 0;i<MAX_RETRANS_NUM;i++)
							{
							printf("retrans[%d].time = %d\n",i,retrans[i].time);
							printf("retrans[%d].count = %d\n",i,retrans[i].count);
							}*/
						}
					op_intrpt_schedule_self(op_sim_time() + Slot_Time, Intrpt_Slot_Down);				//�Լ���ʱ϶�ж�
					g_mac_slot_number = (g_mac_slot_number+1)%10;
					//g_mac_no_packet_flag = Nopacket;
					//op_intrpt_schedule_self(op_sim_time()+Wait_time, UpTime_Code);	
					op_ima_obj_attr_get (g_mac_node_objid,	"g_node_status",	&g_mac_node_status);
					if(g_mac_node_status == ONLINE)
						{
						op_ima_obj_attr_get (g_mac_node_objid,	"g_node_short_address",	&g_mac_short_address);
						op_ima_obj_attr_get (g_mac_node_objid,	"g_node_band",	&g_mac_down_band);
						op_ima_obj_attr_get (g_mac_node_objid,	"g_node_frequency",&g_mac_recv_frequency);
						op_ima_obj_attr_get (g_mac_node_objid,	"g_node_depth",&g_mac_node_depth);
						if(g_mac_node_type == ROUTE_NODE) set_channel_down(g_mac_down_band,0);
						if(g_mac_node_type == END_NODE || g_mac_node_type == BACKUP_NODE) set_channel_down(100,1);
				
						
						for(i = 0;i<CHILDREN_NODE_NUM;i++)
							{
							if(neighbor_table[i].node_id != 0) 
								{
								neighbor_table[i].active_time--;
								}
							if(neighbor_table[i].active_time == 50)		//�ﵽ����Σ��ֵ
								{
								create_actice_frame(neighbor_table[i].node_address,-1);
								neighbor_table[i].active_time = 49;
								}
							if(neighbor_table[i].active_time == 1)		//�ڵ�ʧ��
								{
								MLME_ALIVE_CONFIRM(neighbor_table[i].node_address,1);
								neighbor_table[i].active_time = -1;	
								}
							}
								
						for(i = 0;i < MAX_RETRANS_NUM;i++)
							{
							if(retrans[i].time<10)
								{
								//if(g_mac_node_id == 0)printf("retrans[%d].time = %d\n",i,retrans[i].time);
								retrans[i].time--;
								}
							}
						for(i = 0;i < MAX_ACK_NUM;i++)
							{
							if(ack_time[i]<=3)
								{
								ack_time[i]--;
								}
							}
						if(g_mac_slot_number == 0 && g_mac_node_status == ONLINE && g_mac_node_type != END_NODE )
							{
							if(g_mac_node_type != BACKUP_NODE) 
								make_beacon();
							else 
								op_intrpt_schedule_self(op_sim_time(), Send_Complete_Code);
							}
						else if(g_mac_slot_number!=0 && g_mac_node_status == ONLINE && g_mac_node_type != END_NODE)
							{
							send_msg_to_children();
							}
						else
							{
							op_intrpt_schedule_self(op_sim_time(), Send_Complete_Code);
							}
						}
					}
				}
				FSM_PROFILE_SECTION_OUT (state4_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (9,"WSN_MAC_DOWN2")


			/** state (slot_intrpt) exit executives **/
			FSM_STATE_EXIT_UNFORCED (4, "slot_intrpt", "WSN_MAC_DOWN2 [slot_intrpt exit execs]")


			/** state (slot_intrpt) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_MAC_DOWN2 [slot_intrpt trans conditions]", state4_trans_conds)
			FSM_INIT_COND (C_SENDING_COMPLETE)
			FSM_TEST_COND (C_STRM_NWK_DATA)
			FSM_TEST_COND (C_STRM_NWK_MLME)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("slot_intrpt")
			FSM_PROFILE_SECTION_OUT (state4_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, set_channel_start_receive ();, "C_SENDING_COMPLETE", "set_channel_start_receive ()", "slot_intrpt", "idle", "tr_25", "WSN_MAC_DOWN2 [slot_intrpt -> idle : C_SENDING_COMPLETE / set_channel_start_receive ()]")
				FSM_CASE_TRANSIT (1, 2, state2_enter_exec, ;, "C_STRM_NWK_DATA", "", "slot_intrpt", "App_Msg_Down", "tr_51", "WSN_MAC_DOWN2 [slot_intrpt -> App_Msg_Down : C_STRM_NWK_DATA / ]")
				FSM_CASE_TRANSIT (2, 5, state5_enter_exec, ;, "C_STRM_NWK_MLME", "", "slot_intrpt", "MLME", "tr_58", "WSN_MAC_DOWN2 [slot_intrpt -> MLME : C_STRM_NWK_MLME / ]")
				FSM_CASE_TRANSIT (3, 4, state4_enter_exec, ;, "default", "", "slot_intrpt", "slot_intrpt", "tr_38", "WSN_MAC_DOWN2 [slot_intrpt -> slot_intrpt : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (MLME) enter executives **/
			FSM_STATE_ENTER_FORCED (5, "MLME", state5_enter_exec, "WSN_MAC_DOWN2 [MLME enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_DOWN2 [MLME enter execs]", state5_enter_exec)
				{
				/* recv app data, save in subqueue */
				//int dest;
				int type;
				int id;
				int address;
				int control;
				int i;
				int band;
				int frequency;
				Packet* pk;
				if(C_STRM_NWK_MLME)
					{
					pk = op_pk_get(IN_NWK_MAC_MLME);
					op_pk_fd_get(pk,0,&type);
					switch(type)
						{
						case MLME_UPDATE_NEIGHBOR_REQUEST: 
							{
							op_pk_fd_get(pk,1,&id);
							op_pk_fd_get(pk,2,&address);
							op_pk_fd_get(pk,3,&control);
							for(i = 0;i<CHILDREN_NODE_NUM;i++)
								{
								if(neighbor_table[i].node_id == id)
									{
									if(control == 0)			//��Ӷ̵�ַ
										{
										neighbor_table[i].node_address = address;
										printf("Node %d add neighbor_table[%d].node_address = %d id = %d",g_mac_node_id,i,address,neighbor_table[i].node_id);	
										op_prg_log_entry_write(g_mac_down_info_log_handle,"Node %d add neighbor_table[%d].node_address = %d",g_mac_node_id,i,address);	
										}
									else if(control == 1)		//ɾ���ڵ���Ϣ
										{
										neighbor_table[i].node_id = 0;
										neighbor_table[i].node_address = 0;
										neighbor_table[i].band = 0;
										neighbor_table[i].frequency = 0;
										neighbor_table[i].seq[0] = 0;
										neighbor_table[i].active_time = 0;
										}
									break;
									}
								}
							break;
							}
							
						case MLME_SET_CHANNEL:
							{
							if(g_mac_node_type == BACKUP_NODE)
								{
								op_ima_obj_attr_get (g_mac_node_objid,	"g_node_type",	&g_mac_node_type);
								op_pk_fd_get(pk,0,&band);
								op_pk_fd_get(pk,1,&frequency);
								set_channel_down(band,frequency);		//����Ƶ��Ƶ��
								break;
								}
							break;
							}
						case MLME_LEAVE_REQUEST:
							{
							printf("Node %d down pretend to leave!\n",g_mac_node_id);		
							op_prg_log_entry_write(g_mac_down_info_log_handle,"Node %d down pretend to leave!\n",g_mac_node_id);				
							g_mac_leaving = MAC_LEAVING;
							break;
							}
						default : break;
						}
					op_pk_destroy(pk);
					}
				}
				FSM_PROFILE_SECTION_OUT (state5_enter_exec)

			/** state (MLME) exit executives **/
			FSM_STATE_EXIT_FORCED (5, "MLME", "WSN_MAC_DOWN2 [MLME exit execs]")


			/** state (MLME) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_MAC_DOWN2 [MLME trans conditions]", state5_trans_conds)
			FSM_INIT_COND (g_mac_sending_flag == 2)
			FSM_TEST_COND (g_mac_sending_flag != 2)
			FSM_TEST_LOGIC ("MLME")
			FSM_PROFILE_SECTION_OUT (state5_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, ;, "g_mac_sending_flag == 2", "", "MLME", "idle", "tr_36", "WSN_MAC_DOWN2 [MLME -> idle : g_mac_sending_flag == 2 / ]")
				FSM_CASE_TRANSIT (1, 4, state4_enter_exec, ;, "g_mac_sending_flag != 2", "", "MLME", "slot_intrpt", "tr_61", "WSN_MAC_DOWN2 [MLME -> slot_intrpt : g_mac_sending_flag != 2 / ]")
				}
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"WSN_MAC_DOWN2")
		}
	}




void
_op_WSN_MAC_DOWN2_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_WSN_MAC_DOWN2_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_WSN_MAC_DOWN2_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_WSN_MAC_DOWN2_svar function. */
#undef subnet_objid
#undef receiver_from_father
#undef receiver_from_children
#undef sender_to_father
#undef sender_to_children
#undef channel_of_recvive_from_father
#undef channel_of_recvive_from_children
#undef channel_of_send_to_father
#undef channel_of_send_to_children
#undef receive_from_father_comp
#undef receive_from_children_comp
#undef sender_to_father_comp
#undef sender_to_children_comp
#undef frame
#undef ack_time
#undef retrans
#undef neighbor_table
#undef father_table
#undef pkt
#undef pkptr
#undef activeFrameSeq
#undef keepAlive
#undef MulticastContent
#undef g_mac_node_id
#undef g_mac_short_address
#undef g_mac_node_depth
#undef g_mac_node_type
#undef g_mac_pan_id
#undef g_mac_sequence
#undef g_mac_node_status
#undef g_mac_node_objid
#undef g_mac_process_objid
#undef g_mac_slot_number
#undef g_mac_synch_flag
#undef g_mac_no_packet_flag
#undef g_mac_down_band
#undef g_mac_recv_frequency
#undef g_mac_down_debug_log_handle
#undef g_mac_down_info_log_handle
#undef g_mac_slot_start_time
#undef g_mac_current_time
#undef g_mac_leave_flag
#undef g_mac_data_rate
#undef g_mac_sending_time
#undef g_mac_nwk_synch_seq
#undef g_mac_sending_flag
#undef g_mac_leaving
#undef g_mac_receiving_flag
#undef pkt_success_stathandle
#undef g_pkt_success_stathandle
#undef route_cost_stathandle
#undef g_route_cost_stathandle
#undef link_cost_stathandle
#undef g_link_cost_stathandle
#undef net_cost_stathandle
#undef g_net_cost_stathandle
#undef all_cost_stathandle
#undef g_all_cost_stathandle
#undef packet_number_stathandle
#undef g_packet_number_stathandle
#undef down_packet_number
#undef down_receive_ack_number
#undef down_route_cost
#undef down_link_cost
#undef down_net_cost
#undef down_success_rate
#undef g_down_success_rate
#undef frame_error_rate

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_WSN_MAC_DOWN2_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_WSN_MAC_DOWN2_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (WSN_MAC_DOWN2)",
		sizeof (WSN_MAC_DOWN2_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_WSN_MAC_DOWN2_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	WSN_MAC_DOWN2_state * ptr;
	FIN_MT (_op_WSN_MAC_DOWN2_alloc (obtype))

	ptr = (WSN_MAC_DOWN2_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "WSN_MAC_DOWN2 [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_WSN_MAC_DOWN2_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	WSN_MAC_DOWN2_state		*prs_ptr;

	FIN_MT (_op_WSN_MAC_DOWN2_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (WSN_MAC_DOWN2_state *)gen_ptr;

	if (strcmp ("subnet_objid" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->subnet_objid);
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
	if (strcmp ("frame" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->frame);
		FOUT
		}
	if (strcmp ("ack_time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->ack_time);
		FOUT
		}
	if (strcmp ("retrans" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->retrans);
		FOUT
		}
	if (strcmp ("neighbor_table" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->neighbor_table);
		FOUT
		}
	if (strcmp ("father_table" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->father_table);
		FOUT
		}
	if (strcmp ("pkt" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pkt);
		FOUT
		}
	if (strcmp ("pkptr" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pkptr);
		FOUT
		}
	if (strcmp ("activeFrameSeq" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->activeFrameSeq);
		FOUT
		}
	if (strcmp ("keepAlive" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->keepAlive);
		FOUT
		}
	if (strcmp ("MulticastContent" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->MulticastContent);
		FOUT
		}
	if (strcmp ("g_mac_node_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_node_id);
		FOUT
		}
	if (strcmp ("g_mac_short_address" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_short_address);
		FOUT
		}
	if (strcmp ("g_mac_node_depth" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_node_depth);
		FOUT
		}
	if (strcmp ("g_mac_node_type" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_node_type);
		FOUT
		}
	if (strcmp ("g_mac_pan_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_pan_id);
		FOUT
		}
	if (strcmp ("g_mac_sequence" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_sequence);
		FOUT
		}
	if (strcmp ("g_mac_node_status" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_node_status);
		FOUT
		}
	if (strcmp ("g_mac_node_objid" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_node_objid);
		FOUT
		}
	if (strcmp ("g_mac_process_objid" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_process_objid);
		FOUT
		}
	if (strcmp ("g_mac_slot_number" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_slot_number);
		FOUT
		}
	if (strcmp ("g_mac_synch_flag" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_synch_flag);
		FOUT
		}
	if (strcmp ("g_mac_no_packet_flag" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_no_packet_flag);
		FOUT
		}
	if (strcmp ("g_mac_down_band" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_down_band);
		FOUT
		}
	if (strcmp ("g_mac_recv_frequency" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_recv_frequency);
		FOUT
		}
	if (strcmp ("g_mac_down_debug_log_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_down_debug_log_handle);
		FOUT
		}
	if (strcmp ("g_mac_down_info_log_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_down_info_log_handle);
		FOUT
		}
	if (strcmp ("g_mac_slot_start_time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_slot_start_time);
		FOUT
		}
	if (strcmp ("g_mac_current_time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_current_time);
		FOUT
		}
	if (strcmp ("g_mac_leave_flag" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_leave_flag);
		FOUT
		}
	if (strcmp ("g_mac_data_rate" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_data_rate);
		FOUT
		}
	if (strcmp ("g_mac_sending_time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_sending_time);
		FOUT
		}
	if (strcmp ("g_mac_nwk_synch_seq" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_nwk_synch_seq);
		FOUT
		}
	if (strcmp ("g_mac_sending_flag" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_sending_flag);
		FOUT
		}
	if (strcmp ("g_mac_leaving" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_leaving);
		FOUT
		}
	if (strcmp ("g_mac_receiving_flag" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_receiving_flag);
		FOUT
		}
	if (strcmp ("pkt_success_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pkt_success_stathandle);
		FOUT
		}
	if (strcmp ("g_pkt_success_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_pkt_success_stathandle);
		FOUT
		}
	if (strcmp ("route_cost_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->route_cost_stathandle);
		FOUT
		}
	if (strcmp ("g_route_cost_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_route_cost_stathandle);
		FOUT
		}
	if (strcmp ("link_cost_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->link_cost_stathandle);
		FOUT
		}
	if (strcmp ("g_link_cost_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_link_cost_stathandle);
		FOUT
		}
	if (strcmp ("net_cost_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->net_cost_stathandle);
		FOUT
		}
	if (strcmp ("g_net_cost_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_net_cost_stathandle);
		FOUT
		}
	if (strcmp ("all_cost_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->all_cost_stathandle);
		FOUT
		}
	if (strcmp ("g_all_cost_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_all_cost_stathandle);
		FOUT
		}
	if (strcmp ("packet_number_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->packet_number_stathandle);
		FOUT
		}
	if (strcmp ("g_packet_number_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_packet_number_stathandle);
		FOUT
		}
	if (strcmp ("down_packet_number" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->down_packet_number);
		FOUT
		}
	if (strcmp ("down_receive_ack_number" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->down_receive_ack_number);
		FOUT
		}
	if (strcmp ("down_route_cost" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->down_route_cost);
		FOUT
		}
	if (strcmp ("down_link_cost" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->down_link_cost);
		FOUT
		}
	if (strcmp ("down_net_cost" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->down_net_cost);
		FOUT
		}
	if (strcmp ("down_success_rate" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->down_success_rate);
		FOUT
		}
	if (strcmp ("g_down_success_rate" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_down_success_rate);
		FOUT
		}
	if (strcmp ("frame_error_rate" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->frame_error_rate);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}


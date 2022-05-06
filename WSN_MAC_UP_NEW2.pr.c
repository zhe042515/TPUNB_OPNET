/* Process model C form file: WSN_MAC_UP_NEW2.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char WSN_MAC_UP_NEW2_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A modeler 7 62738F02 62738F02 1 DESKTOP-RD4S7T2 51133 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                    ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

//-----------------------*声明头文件*-----------------------//
#include <math.h>

//-----------------------定义常量-----------------------//
#define		CEN_NODE				43690		//中心节点id
#define		Frame_Time				5			//一个帧时长
#define		Slot_Time				0.5			//一个时隙时长
#define		Down_Time				0.01		//一个下行时隙时长10ms
#define		Protection_time			0.03		//保护间隔，收到下行包过多少秒开启上行
#define		Wait_time				0.03		//等待间隔，多长时间未收到下行包开始发送上行数据
#define		Max_Slot_Num			10			//一个帧时隙数
#define		BroadCastAddress		0xffff		//广播地址
#define		GateAddress				0xaaaa		//网关地址
#define		Max_Depth				8			//最大深度
#define		Max_ACK					5			//最大重传等待时间
#define		BandWidth				4			//频段带宽
#define		MinBandWidth			470.0		//最小频段带宽
#define		MaxBandWidth			658.0		//最大频段带宽
#define		PointWidth				1			//频段带宽
#define		Active_Time_Beacon		50			//信标帧保活时间，单位为帧(5s)
#define		ACTIVE_TIME				5000000		//子节点保活时间
#define		RETRANS_TIME			6			//重传等待时间
#define		ACK_TIME				2			//创建的ACK等待时间
#define		FATHER_NODE_NUM			6			//潜在父节点表数量
#define		MAX_ASSOCIATE_TIME		6			//关联最大等待时间
#define		MAX_SEQUENCE_NUM		6			//最大滑动窗格个数（0号值是第二个序列号的值）
#define		MAX_RETRANS_NUM			10			//最大重传队列长度
//长度为6，保存五个
#define		macMaxBE				7			//最大退避因子(5~9)
#define		macMinBE				3			//最小退避因子固定为3
/*----------------------------节点类型--------------------------*/
#define		GATE_NODE				0			//网关节点
#define		ROUTE_NODE				1			//路由节点
#define		END_NODE				2			//终端节点
#define		BACKUP_NODE				3			//备份节点
/*----------------------------网络状态--------------------------*/
#define		ONLINE					1			//在网
#define		OFFLINE					0			//离网

#define		ACCESS					2			//允许接入
#define		REJECT					0			//拒绝接入


/*----------------------------帧类型--------------------------*/
#define		BEACON_FRAME			0			//信标帧
#define		DATA_FRAME				1			//数据帧
#define		ACK_FRAME				2			//ACK确认帧
#define		MAC_CONTROL_FRAME		3			//MAC命令帧
#define		RETRANS_FRAME			4			//重传帧
#define		DELEDTE_FRAME			5			//此次检测没有帧需要被发送，但是需要再次检查
#define		NO_FRAME				6			//没有帧需要被发送，不需要再次检查

#define		JOIN_REQ_FRAME			0			//入网请求帧
#define		ASSOCIATE_REQ_FRAME		0			//关联请求帧
#define		ASSOCIATE_RESP_FRAME	1			//关联回复帧
#define		KEEP_ALIVE_FRAME		2			//保活帧
/*----------------------------原语类型------------------------*/
#define		MLME_SCAN_REQUEST		0			//信道扫描原语
#define		MLME_SCAN_confirm		0			//信道扫描回复原语
#define		MLME_ASSOCIATE_REQUEST	1			//信道关联原语
#define		MLME_ASSOCIATE_confirm	1			//信道关联回复原语
#define		MLME_ALIVE_REQUEST		2			//节点保活原语
#define		MLME_ALIVE_confirm		2			//节点保活回复原语
#define		MLME_CONFLICT_confirm	4			//节点地址冲突回复原语
#define		MLME_MULTICAST_confirm	5			//节点组播回复原语
#define		MLME_SET_CHANNEL		6			//设置信道频段频点原语（备份节点工作使用）
#define		MLME_LEAVE_REQUEST		7			//离网请求原语
#define		MLME_LEAVE_confirm		7			//离网确认原语



#define		SENDING					1			//正在发送，不可接收
#define		RECEIVING				2			//没有发送，可以接收


/*----------------------------收包标识赋值------------------------*/
#define		Receiced				1			//收到了包
#define		Nopacket				2			//未收到包
/*------------------------------扫描赋值------------------------*/
#define		NEED_SCAN				1			//需要扫描
#define		NO_SCAN					0			//不需要扫描
/*----------------------------同步状态赋值------------------------*/
#define		SYNCH_COMPLETE			1			//完成同步
#define		NEED_SYNCH				0			//未完成同步
/*--------------------------关联回复标识赋值--------------------*/
#define		RECV_ASSOCIATE_RESP		1			//收到关联回复
#define		NO_ASSOCIATE_RESP		0			//未收到关联回复
/*------------------------------地址模式--------------------*/
#define		GATE_ADDRESS_MODE		1			//16位短地址模式
#define		SHORT_ADDRESS_MODE		2			//16位短地址模式
#define		ESN_ADDRESS_MODE		3			//48位ESN地址模式
/*------------------------------离网模式--------------------*/
#define		MAC_LEAVING				1			//MAC层正在离网
#define		LEAVING_COMPELTE_NEED_SYNCH		2	//MAC层离网完成未同步
#define		COMPLETE_LEAVING		3			//MAC层离网且完成同步



//#define		CSMA_MODE				0			//1使用CSMA；０使用ALOHA
#define 	MAXTTL					2
#define 	MAX_NODE				200
#define 	MAX_ROUTER				48
#define 	MAX_CHILD				50
#define 	MAX_RouterAddress		128
#define 	GATEPOINTNUM			8
#define 	POINTNUM				2
#define 	MAX_MULTICAST			20
//-------------------------定义队列-----------------------//
#define		QUEUE_NUM				3		//队列数量

#define		Data_Queue				0		//应用层数据队列（上行）
#define		Retrans_Queue			1		//重传队列（上行）
#define		ACK_Queue				2		//ACK队列（上行）
/**------------------Interupt Code-------------------**/
#define   	Start_Code					10000
#define   	Intrpt_Slot         		20000	//全网帧中断码
#define   	Intrpt_Slot_Down         	20001	//全网帧中断码
#define   	Up_Code         			20100	//上行中断码
#define		Scan_Code					50000	//开启扫描中断
#define		ChangeBand_Code				60000	//切换扫描频段中断
#define		Associate_Code				70000	//开启关联中断
#define		Associate_Success_Code		70001	//关联success中断
#define		Associate_Fail_Code			70002	//关联fail中断
#define		Associate_Timeout_Code		70003	//关联timeout中断
#define		Send_Completed_Code			80000	//发送完成中断
#define		Slot_Completed_Code			80001	//时隙结束中断
#define		Channel_Detect_Code			80002	//信道检测中断
#define		Mac_Leave_Code				90000	//离网中断


#define		Children_Active				5		//父节点收到子节点的保活消息

#define 	WPANC_CSMA_TIMER_EXPIRED 	-1		//退避时间到中断
/**------------------定义状态转移条件-------------------**/
#define 	Join_Net							((op_intrpt_type() == OPC_INTRPT_REMOTE))
#define		C_SELF_NODE_START					((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Start_Code))
#define		C_STRM_NWK_DATA						((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == IN_NWK_MAC_DATA))
#define		C_SELF_START_ASSOCIATE				((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Associate_Code))
#define		C_SELF_ASSOCIATE_SUCCESS			((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Associate_Success_Code))
#define		C_SELF_ASSOCIATE_FAIT				((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Associate_Fail_Code))
#define		C_STRM_NWK_MLME						((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == IN_NWK_MAC_MLME))
#define		C_SELF_START_SCAN					((g_mac_scan_flag == NEED_SCAN)&&(op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Scan_Code))
#define		C_STRM_MSG_FROM_FATHER_NODE			((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == IN_PHY_MAC))
#define		C_SELF_NEW_SLOT						((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Intrpt_Slot))
#define		C_SELF_UP_TIME						((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Up_Code))
#define		C_SELF_CHANGE_RECEIVER_CHANNEL		((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == ChangeBand_Code))
#define		C_SELF_SLOT_COMPLETED				((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Slot_Completed_Code))
#define		C_SELF_SEND_COMPLETED				((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Send_Completed_Code))
#define		C_SELF_CHANNEL_DETECT				((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Channel_Detect_Code))
#define		C_SLEF_LEAVE						((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Mac_Leave_Code))


#define 	BACKOFF_TIMER_EXPIRED 				(op_intrpt_type () == OPC_INTRPT_SELF && op_intrpt_code () == WPANC_CSMA_TIMER_EXPIRED)
#define 	BACKOFF_LIMIT_REACHED 				(g_mac_num_backoffs == g_mac_max_backoffs)
//#define 	CH_FREE 							((csma_is_channel_free () == OPC_TRUE)&&(csma_send_is_free () == OPC_TRUE))
#define 	CH_FREE 							((csma_is_channel_free () == OPC_TRUE))

/*------------------ Input Index ------------------*/
#define 	IN_PHY_MAC 					0		//来自上游节点的数据
#define 	IN_NWK_MAC_DATA 			1		//来自上层发给上有节点的数据
#define 	IN_NWK_MAC_MLME 			2		//网络层给MAC层的原语
/* ------------------Output Index------------------ */

#define 	OUT_MAC_PHY 				0		//发给上游节点
#define 	OUT_MAC_NWK_DATA 			1		//发给网络层
#define 	OUT_MAC_NWK_MLME	 		2		//MAC层给网络层的原语
//************************定义包类型************************//
//************************定义结构体************************//
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
	int seq[MAX_SEQUENCE_NUM];	//0存放已经收到的序列号	
	int active_time;
	};

struct Retrans
	{
	int time;
	int count;
	};
/*保活结构体*/
struct KeepAliveFrame
	{
	int dest;
	int seq;
	};

struct childrenTable{
	int ESNAddress;
	int NWKAddress;
	int status;		//2表示删除
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

struct Father{
	struct Potential_Parent father[6];
};

struct Fa{
	struct Father father[MAX_NODE];
};

/** ----------------Function Prototypes------------- **/

static void 	mac_init();
static void 	scan_channel();
static void		change_band();
static void 	MLME_SCAN_CONFIRM();
static void 	MLME_ASSOCIATE_CONFIRM(int result);
static void 	MLME_ALIVE_CONFIRM(int dest,int result);
static void 	MLME_CONFLICT_CONFIRM(int address);
static void 	MLME_LEAVE_CONFIRM(int result);
static void 	set_channel(int fatherBand,int frequency);
static void 	create_associate_frame(int dest,int panid);
static void 	send_msg_to_father();
static void		create_ack(Packet* pk,int i);
static void 	mac_frame_proc(Packet* pk);
static void 	ack_proc(int seq ,int source ,int dest,int need_seq);
static int 		next_hop(int dest);
static void 	update_alive_time(int source);
static void 	add_IES(Packet* pk);
static void		create_actice_frame(int dest,int seq);
static void 	beacon_proc(Packet* pk);
static void 	update_ack_time();
static void 	update_retrans_time(int i);
static void 	delete_keep_alive();
static void 	delete_neighbor_node();
static void 	update_father_seq();
static int 		update_sequence_from_node(int source, int seq,int source_mode);
static void 	ack_frame_proc(Packet* pk);
static int 		channel_from_father_detect();
static void 	cheak_mac_queue();
static void 	mac_leave();
static void 	update_windows();
static void 	ack_frame_proc1(Packet* pk);
static void 	ack_proc1(int dest,int seq,int need_seq );
static void 	set_channel_start_receive ();
static void 	set_channel_start_send ();

static void 	csma_init ();
static void 	csma_schedule_backoff ();
static int 		csma_is_channel_free ();
static void 	get_packet_to_father();
static void 	csma_send_packet ();
static void 		route_sink (Packet* pk);
static void 		link_sink (Packet* pk);


/** ----------------Globle Attribute------------- **/
int globle_up_packet_number;
int globle_up_receive_ack_number;

long globle_up_route_cost; 
long globle_up_link_cost; 
long globle_up_net_cost;




//extern	struct		NetworkMsg Network_Msg;
extern struct NetworkMsg Network_Msg;

struct Fa potentialParent;
int    Active_from_up[MAX_NODE];


/*++++++++++++++++++++++++test aloha++++++++++++++++++++++*/
/* Global Variable */
//extern int subm_pkts =0;
//extern int rcvd_pkts =0;


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
	Objid	                  		subnet_objid                                    ;	/* 网络号 */
	Objid	                  		receiver_from_father                            ;	/* 接收父节点信息的接收机objid */
	Objid	                  		receiver_from_children                          ;	/* 接收子节点信息的接收机objid */
	Objid	                  		sender_to_father                                ;	/* 向父节点发送信息的发送机objid */
	Objid	                  		sender_to_children                              ;	/* 向子节点发送信息的发送机objid */
	Objid	                  		channel_of_recvive_from_father                  ;
	Objid	                  		channel_of_recvive_from_children                ;
	Objid	                  		channel_of_send_to_father                       ;
	Objid	                  		channel_of_send_to_children                     ;
	Objid	                  		receive_from_father_comp                        ;
	Objid	                  		receive_from_children_comp                      ;
	Objid	                  		sender_to_father_comp                           ;
	Objid	                  		sender_to_children_comp                         ;
	int	                    		frame                                           ;	/* 帧序号 */
	int	                    		ack_time[5]                                     ;	/* 上行模拟接收ACK时延 */
	struct Retrans	         		retrans[MAX_RETRANS_NUM]                        ;	/* 重传计时器 */
	struct neighborTable	   		neighbor_table                                  ;	/* 邻节点表，记录邻节点的序列号 */
	struct Father	          		father_table                                    ;	/* 潜在父节点表 */
	int	                    		pkt                                             ;	/* 测试发送包的个数 */
	Packet *	               		pkptr                                           ;	/* 用于存放给父节点的数据包 */
	int	                    		pk_type                                         ;	/* 发送的上行包的类型（pkptr） 1表示数据包；2表示ack包；3表示重传包 */
	int	                    		pk_size                                         ;	/* 发送上行数据包长度 */
	int	                    		send_time                                       ;	/* 上行数据包发送需要的时间 */
	int	                    		sending_rate                                    ;	/* 上行发送速率 */
	int	                    		up_start_time                                   ;	/* 上行时隙开始时间 */
	int	                    		can_send                                        ;	/* 决定是否进行cca,判断依据是时间是否来得及  1表示时间来得及，0表示时间来不及需要回退到idle状态 */
	Evhandle	               		backoff_evhandle                                ;
	Stathandle	             		contr_send_num                                  ;
	int	                    		sendNum                                         ;
	int	                    		first_back                                      ;	/* 第一次退避的标识符  1表示需要进行第一次退避 */
	int	                    		mincost                                         ;	/* 最小路由代价 */
	int	                    		maxcost                                         ;	/* 最大路由代价 */
	int	                    		no                                              ;	/* 最小路由代价编号 */
	int	                    		no2                                             ;	/* 最大路由代价编号 */
	struct KeepAliveFrame	  		keepAlive                                       ;	/* 保活结构体 */
	int	                    		MulticastContent                                ;	/* 组播维护内容 */
	int	                    		g_mac_node_id                                   ;	/* 节点esn长地址 */
	int	                    		g_mac_short_address                             ;	/* 节点短地址 */
	int	                    		g_mac_node_depth                                ;	/* 节点深度 */
	int	                    		g_mac_node_type                                 ;	/* 节点类型   0是网关，1是路由节点 */
	int	                    		g_mac_pan_id                                    ;	/* 节点pan_id */
	int	                    		g_mac_sequence                                  ;	/* 节点mac序列号 */
	int	                    		g_mac_node_status                               ;	/* 节点状态    1表示在网,0表示不在网 */
	Objid	                  		g_mac_node_objid                                ;
	Objid	                  		g_mac_process_objid                             ;
	int	                    		g_mac_num_backoffs                              ;	/* 退避次数 */
	int	                    		g_mac_slot_number                               ;	/* 节点时隙号 */
	int	                    		g_mac_synch_flag                                ;	/* 节点同步完成标识符 1：已同步  0：未同步 */
	int	                    		g_mac_scan_flag                                 ;	/* 节点扫描标识符  1：需要扫描；0：不需要扫描 */
	int	                    		g_mac_max_backoffs                              ;	/* 最大退避次数 */
	int	                    		g_mac_backoff_exponent                          ;	/* 退避指数 */
	double	                 		g_mac_unit_backoff_period                       ;	/* 回退单位时间 */
	int	                    		g_mac_associate_target                          ;	/* 关联目的节点 */
	int	                    		g_mac_recv_associate_response                   ;	/* 节点是否收到关联回复帧   1:收到；0：未收到 */
	int	                    		g_mac_associate_pan_id                          ;	/* 关联目的节点pan_id */
	int	                    		g_mac_associate_times                           ;	/* 关联次数 */
	double	                 		g_mac_backoff_duration                          ;	/* 本次退避的时间 */
	Log_Handle	             		g_mac_up_debug_log_handle                       ;	/* mac日志 */
	Log_Handle	             		g_mac_up_info_log_handle                        ;
	double	                 		g_mac_slot_start_time                           ;	/* 时隙开始的时间 */
	double	                 		g_mac_current_time                              ;	/* 当前时间 */
	int	                    		g_mac_leave_flag                                ;	/* 离网状态 */
	double	                 		g_mac_data_rate                                 ;	/* 发送速率 */
	double	                 		g_mac_sending_time                              ;	/* 发送时间 */
	double	                 		g_mac_rand                                      ;	/* 随机数 */
	int	                    		g_mac_pk_type                                   ;	/* 获取到要发的包的类型，1为数据包，2为ACk包，3为重传包 */
	int	                    		g_mac_sending_flag                              ;	/* 标志节点正处于发送状态 1：正发送 2；未发送 */
	int	                    		g_mac_leaving                                   ;	/* 离网状态，1：正在离网 */
	Objid	                  		father_receiver                                 ;
	Objid	                  		channel_of_father_received                      ;
	Objid	                  		father_received_comp                            ;
	double	                 		receiveFrequency                                ;
	int	                    		g_mac_receiving_flag                            ;	/* 发送状态标识符   1：正在发送，2：没有发送 */
	Stathandle	             		pkt_success_stathandle                          ;
	Stathandle	             		g_pkt_success_stathandle                        ;
	Stathandle	             		packet_number_stathandle                        ;
	Stathandle	             		g_packet_number_stathandle                      ;
	int	                    		up_packet_number                                ;
	int	                    		up_receive_ack_number                           ;
	Stathandle	             		up_success_rate                                 ;
	Stathandle	             		g_up_success_rate                               ;
	Stathandle	             		up_route_cost_stathandle                        ;
	Stathandle	             		g_up_route_cost_stathandle                      ;
	Stathandle	             		up_link_cost_stathandle                         ;
	Stathandle	             		g_up_link_cost_stathandle                       ;
	Stathandle	             		up_net_cost_stathandle                          ;
	Stathandle	             		g_up_net_cost_stathandle                        ;
	double	                 		up_route_cost                                   ;
	double	                 		up_link_cost                                    ;
	double	                 		up_net_cost                                     ;
	int	                    		retrans_flag                                    ;
	int	                    		back_off_count                                  ;
	int	                    		slot_complete_flag                              ;	/* 时隙结束标识，1 标识结束，2，未结束 */
	int	                    		CSMA_MODE                                       ;	/* 1使用CSMA；０使用ALOHA */
	double	                 		frame_error_rate                                ;
	} WSN_MAC_UP_NEW2_state;

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
#define pk_type                 		op_sv_ptr->pk_type
#define pk_size                 		op_sv_ptr->pk_size
#define send_time               		op_sv_ptr->send_time
#define sending_rate            		op_sv_ptr->sending_rate
#define up_start_time           		op_sv_ptr->up_start_time
#define can_send                		op_sv_ptr->can_send
#define backoff_evhandle        		op_sv_ptr->backoff_evhandle
#define contr_send_num          		op_sv_ptr->contr_send_num
#define sendNum                 		op_sv_ptr->sendNum
#define first_back              		op_sv_ptr->first_back
#define mincost                 		op_sv_ptr->mincost
#define maxcost                 		op_sv_ptr->maxcost
#define no                      		op_sv_ptr->no
#define no2                     		op_sv_ptr->no2
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
#define g_mac_num_backoffs      		op_sv_ptr->g_mac_num_backoffs
#define g_mac_slot_number       		op_sv_ptr->g_mac_slot_number
#define g_mac_synch_flag        		op_sv_ptr->g_mac_synch_flag
#define g_mac_scan_flag         		op_sv_ptr->g_mac_scan_flag
#define g_mac_max_backoffs      		op_sv_ptr->g_mac_max_backoffs
#define g_mac_backoff_exponent  		op_sv_ptr->g_mac_backoff_exponent
#define g_mac_unit_backoff_period		op_sv_ptr->g_mac_unit_backoff_period
#define g_mac_associate_target  		op_sv_ptr->g_mac_associate_target
#define g_mac_recv_associate_response		op_sv_ptr->g_mac_recv_associate_response
#define g_mac_associate_pan_id  		op_sv_ptr->g_mac_associate_pan_id
#define g_mac_associate_times   		op_sv_ptr->g_mac_associate_times
#define g_mac_backoff_duration  		op_sv_ptr->g_mac_backoff_duration
#define g_mac_up_debug_log_handle		op_sv_ptr->g_mac_up_debug_log_handle
#define g_mac_up_info_log_handle		op_sv_ptr->g_mac_up_info_log_handle
#define g_mac_slot_start_time   		op_sv_ptr->g_mac_slot_start_time
#define g_mac_current_time      		op_sv_ptr->g_mac_current_time
#define g_mac_leave_flag        		op_sv_ptr->g_mac_leave_flag
#define g_mac_data_rate         		op_sv_ptr->g_mac_data_rate
#define g_mac_sending_time      		op_sv_ptr->g_mac_sending_time
#define g_mac_rand              		op_sv_ptr->g_mac_rand
#define g_mac_pk_type           		op_sv_ptr->g_mac_pk_type
#define g_mac_sending_flag      		op_sv_ptr->g_mac_sending_flag
#define g_mac_leaving           		op_sv_ptr->g_mac_leaving
#define father_receiver         		op_sv_ptr->father_receiver
#define channel_of_father_received		op_sv_ptr->channel_of_father_received
#define father_received_comp    		op_sv_ptr->father_received_comp
#define receiveFrequency        		op_sv_ptr->receiveFrequency
#define g_mac_receiving_flag    		op_sv_ptr->g_mac_receiving_flag
#define pkt_success_stathandle  		op_sv_ptr->pkt_success_stathandle
#define g_pkt_success_stathandle		op_sv_ptr->g_pkt_success_stathandle
#define packet_number_stathandle		op_sv_ptr->packet_number_stathandle
#define g_packet_number_stathandle		op_sv_ptr->g_packet_number_stathandle
#define up_packet_number        		op_sv_ptr->up_packet_number
#define up_receive_ack_number   		op_sv_ptr->up_receive_ack_number
#define up_success_rate         		op_sv_ptr->up_success_rate
#define g_up_success_rate       		op_sv_ptr->g_up_success_rate
#define up_route_cost_stathandle		op_sv_ptr->up_route_cost_stathandle
#define g_up_route_cost_stathandle		op_sv_ptr->g_up_route_cost_stathandle
#define up_link_cost_stathandle 		op_sv_ptr->up_link_cost_stathandle
#define g_up_link_cost_stathandle		op_sv_ptr->g_up_link_cost_stathandle
#define up_net_cost_stathandle  		op_sv_ptr->up_net_cost_stathandle
#define g_up_net_cost_stathandle		op_sv_ptr->g_up_net_cost_stathandle
#define up_route_cost           		op_sv_ptr->up_route_cost
#define up_link_cost            		op_sv_ptr->up_link_cost
#define up_net_cost             		op_sv_ptr->up_net_cost
#define retrans_flag            		op_sv_ptr->retrans_flag
#define back_off_count          		op_sv_ptr->back_off_count
#define slot_complete_flag      		op_sv_ptr->slot_complete_flag
#define CSMA_MODE               		op_sv_ptr->CSMA_MODE
#define frame_error_rate        		op_sv_ptr->frame_error_rate

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	WSN_MAC_UP_NEW2_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((WSN_MAC_UP_NEW2_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif

/*********************************************/
/*********************************************/
/*MAC层初始化函数，初始化状态信息，队列，结构体内存，MAC序列号*/
/*********************************************/
/*********************************************/
static void mac_init()
	{
	int 	 			i;
	int                 new_seed;
	PrgT_Random_Gen    	*my_rng;
	FIN(mac_init());
	//初始化统计量***************************************
	new_seed = g_mac_node_id;
	frame	= 0;
	g_mac_slot_number = 0;
	g_mac_associate_target = -1;
	mincost 	= 10;
	maxcost		= 10;
	no		= 0;
	no2		= 0;	
	if(CSMA_MODE == 1)
		{
		csma_init ();
		}
	op_ima_obj_attr_get (channel_of_send_to_father,"data rate",&g_mac_data_rate);
	g_mac_node_status = OFFLINE;										//节点开机时不在网		
	for(i = 0;i < Max_ACK;i++)
		{
		ack_time[i] = 100;
		}	
	for(i = 0;i < MAX_RETRANS_NUM;i++)
		{
		retrans[i].time = 100;
		retrans[i].count = 0;
		}
	//潜在父节点表初始化---------------------------------
	if(g_mac_leaving != 1)
		{
		for (i=0;i<FATHER_NODE_NUM;i++)
			{
			father_table.father[i].short_addr = 0;
			father_table.father[i].panid = 0;
			father_table.father[i].band = 0;
			father_table.father[i].depth = 0;
			father_table.father[i].frequency = 0;
			father_table.father[i].router_cost = 10;
			father_table.father[i].times = 0;
			}
		}
	//邻节点序列号表初始化------------------------------
	neighbor_table.node_id = 0;
	neighbor_table.seq[0] = 0;
	neighbor_table.band = 0;
	neighbor_table.node_address = 0;
	neighbor_table.frequency = 0;
	neighbor_table.active_time = 0;
	keepAlive.dest = 0;
	keepAlive.seq = 0;	

	//队列初始化-----------------------------------------
	/* Clear SubQ */
	op_subq_flush(Data_Queue);
	op_subq_flush(Retrans_Queue);
	op_subq_flush(ACK_Queue);
	//中心节点为同步基准节点，已同步---------------------
	if(g_mac_node_type == GATE_NODE)
		{										//中心节点，直接进入NET	
		op_ima_obj_attr_set (g_mac_node_objid,	"g_node_short_address"	,	CEN_NODE);
		g_mac_short_address = CEN_NODE;
		g_mac_synch_flag = SYNCH_COMPLETE;                                      		//同步完成
		op_intrpt_schedule_self(op_sim_time() + Slot_Time, Intrpt_Slot);				//自己的时隙中断
		op_intrpt_schedule_remote(op_sim_time() + Slot_Time ,Intrpt_Slot_Down ,			
			op_id_from_name(g_mac_node_objid,OPC_OBJTYPE_QUEUE,"MAC_DOWN"));				//对下的MAC的时隙中断
		printf("Gate node start slot intrupt\n");
		g_mac_node_status = ONLINE;											//网关节点直接在网
		g_mac_scan_flag = NO_SCAN;											//网关节点不需要扫描信道
		g_mac_node_depth = 0;
		op_ima_obj_attr_set (channel_of_recvive_from_father		,"min frequency",0.0);
		op_ima_obj_attr_set (channel_of_send_to_father			,"min frequency",0.0); 
		}
	
	if(g_mac_node_type != GATE_NODE)
		{
		g_mac_synch_flag = SYNCH_COMPLETE;											//非网关节点
		g_mac_node_status = OFFLINE;											//现阶段节点开机就启动时隙，不需要同步步骤
		g_mac_scan_flag = NO_SCAN;												//需要扫描
		op_ima_obj_attr_set (channel_of_recvive_from_father		,"min frequency",470.0);
		op_ima_obj_attr_set (channel_of_send_to_father			,"min frequency",470.0);
		}
	set_channel_start_receive ();
	//初始化MAC层序列号
	/* create a new random number generator */
	my_rng = op_prg_random_gen_create (new_seed);
	/* generate a random integer in the interval [0,255] */
	g_mac_sequence = (op_prg_random_integer_gen (my_rng) % 256) ;
	/* destroy the random number generator */
	op_prg_random_gen_destroy (my_rng); 
	printf("Node %d Init start(MAC address)\n",g_mac_node_id);
	FOUT;
	}





/*********************************************/
/*********************************************/
/*scan */
/*********************************************/
/*********************************************/
static void scan_channel()
	{
	FIN(scan_channel());
	op_ima_obj_attr_set (channel_of_recvive_from_father	,"min frequency",470.0);
	op_intrpt_schedule_self(op_sim_time() + 10, ChangeBand_Code);
	FOUT;
	}



/*********************************************/
/*********************************************/
/*change the receiver's band  ，scan for a channel*/
/*********************************************/
/*********************************************/
static void	change_band()
	{
	double		current_band;
	double		new_band;
	FIN(change_band());
	op_ima_obj_attr_get (channel_of_recvive_from_father,"min frequency",&current_band);
	if(current_band == MaxBandWidth)
		{
		//向网络层汇报扫描结果，构建扫描完成原语
		printf("Node %d MAC send a MLME_SCAN.confirm to INTERFACE\n",g_mac_node_id);
		op_prg_log_entry_write(g_mac_up_info_log_handle, "Node %d MAC send a MLME_SCAN.confirm to INTERFACE\n",g_mac_node_id);
		potentialParent.father[g_mac_node_id] = father_table;
		op_prg_log_entry_write(g_mac_up_info_log_handle, "Node %d MAC father_table.father[0].short_addr = %d,father_table.father[0].band =%d\n",g_mac_node_id,father_table.father[0].short_addr,father_table.father[0].band);
		MLME_SCAN_CONFIRM();
		g_mac_scan_flag = NO_SCAN;
		FOUT;
		}
	else
		{
		g_mac_scan_flag = NEED_SCAN;
		new_band = current_band + BandWidth;
		op_ima_obj_attr_set (channel_of_recvive_from_father	,"min frequency",new_band);
		op_intrpt_schedule_self(op_sim_time() + 10, ChangeBand_Code);
		FOUT;
		}
	}



/*********************************************/
/*********************************************/
/*create MLME_SCAN.confirm,and seng it to NWK*/
/*********************************************/
/*********************************************/
static void MLME_SCAN_CONFIRM()
	{
	Packet* MLME_SCAN;
	int type;
	int i;
	int number;				//潜在父节点个数
	FIN(MLME_SCAN_CONFIRM());
	type = MLME_SCAN_confirm;
	number = 0;
	for(i = 0;i<6;i++)
		{
		if(father_table.father[i].short_addr != 0)
			{
			number++;
			}
		}
	MLME_SCAN = op_pk_create(0);//16
	op_pk_fd_set (MLME_SCAN, 1, OPC_FIELD_TYPE_INTEGER,number,8) ;
	op_pk_fd_set (MLME_SCAN, 0, OPC_FIELD_TYPE_INTEGER,type,8) ;
	op_prg_log_entry_write(g_mac_up_info_log_handle, "Node %d MAC father_table.father[0].short_addr = %d,father_table.father[0].band %d\n",g_mac_node_id,potentialParent.father[g_mac_node_id].father[0].short_addr,potentialParent.father[g_mac_node_id].father[0].band);
	op_pk_send(MLME_SCAN,OUT_MAC_NWK_MLME);
	FOUT;
	}


/*********************************************/
/*********************************************/
/*create MLME_ASSOCIATE.confirm,and send it to NWK*/
/*0:success    1:fail*/
/*********************************************/
/*********************************************/
static void MLME_ASSOCIATE_CONFIRM(int result)
	{
	Packet* MLME_ASSOCIATE;
	int type;
	FIN(MLME_SCAN_CONFIRM());
	type = MLME_ASSOCIATE_confirm;
	MLME_ASSOCIATE = op_pk_create(0); //32 
	op_pk_fd_set (MLME_ASSOCIATE, 0, OPC_FIELD_TYPE_INTEGER,type,8) ;
	op_pk_fd_set (MLME_ASSOCIATE, 1, OPC_FIELD_TYPE_INTEGER,g_mac_associate_target,16) ;
	op_pk_fd_set (MLME_ASSOCIATE, 2, OPC_FIELD_TYPE_INTEGER,result,8) ;
	op_pk_send(MLME_ASSOCIATE,OUT_MAC_NWK_MLME);	
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
	MLME_ALIVE = op_pk_create(0);  //32
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
	FIN(cheak_mac_queue());
	op_subq_flush(Data_Queue);
	op_subq_flush(Retrans_Queue);
	op_subq_flush(ACK_Queue);
	MLME_LEAVE_CONFIRM(1);//对上天线完成离网		
	mac_leave();
	op_intrpt_schedule_self(op_sim_time(), Mac_Leave_Code);
	op_prg_log_entry_write(g_mac_up_info_log_handle, "g_mac_synch_flag =%d!!!!!!!\n",g_mac_synch_flag);
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
	op_intrpt_clear_self ();
	op_strm_flush(IN_PHY_MAC);
	op_strm_flush(IN_NWK_MAC_MLME);
	op_strm_flush(IN_NWK_MAC_DATA);
	g_mac_leaving = LEAVING_COMPELTE_NEED_SYNCH;	
	FOUT;
	}	



/*********************************************/
/*********************************************/
/*beacon process*/
/*********************************************/
/*********************************************/
static void beacon_proc(Packet* pk)
	{
	//int 	framecontrol;
	int		sequence;
	int		panid;
	int		source;
	int		beacon_sequence;
	int		max_depth;
	int		current_depth;
	int		slot_number;
	int		access_control;
	int		router_cost;
	double	band;
	int		frequency;
	int		strenth;
	int 	i;	
	FIN(beacon_proc(Packet* pk));
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
	if( g_mac_node_status == 1 )
		{	

		}
	//printf("op_td_get_dbl (pk, OPC_TDA_RA_RCVD_POWER) = %.16f\n",op_td_get_dbl (pk, OPC_TDA_RA_RCVD_POWER));
	if(g_mac_scan_flag == NEED_SCAN||g_mac_leaving == LEAVING_COMPELTE_NEED_SYNCH)														//扫描状态,父节点信息存入父节点表
		{	
		//if (op_td_get_dbl (pk, OPC_TDA_RA_RCVD_POWER) < 0.00000000000001 )
		if (op_td_get_dbl (pk, OPC_TDA_RA_RCVD_POWER) < 0.00000000000001 )

			{
			op_pk_destroy(pk);
			FOUT;
			}
		if (g_mac_synch_flag == NEED_SYNCH)
			{
			op_prg_log_entry_write(g_mac_up_info_log_handle, "SYNCH");
			op_intrpt_schedule_self(op_sim_time() + Slot_Time-0.11820, Intrpt_Slot);		//自己的时隙中断
			op_intrpt_schedule_remote(op_sim_time() + Slot_Time-0.11820 ,Intrpt_Slot_Down ,			
				op_id_from_name(g_mac_node_objid,OPC_OBJTYPE_QUEUE,"MAC_DOWN"));				//对下的MAC的时隙中断
			g_mac_synch_flag = SYNCH_COMPLETE;
			g_mac_leaving = COMPLETE_LEAVING;
			}
		if(maxcost > router_cost && access_control == ACCESS)
			{
			for(i = 0;i<FATHER_NODE_NUM;i++)
				{
				if(father_table.father[i].short_addr == source)
					{
					father_table.father[i].panid = panid;
					op_ima_obj_attr_get (channel_of_recvive_from_father	,"min frequency",&band);
					father_table.father[i].band = (int)((band-470)/4);
					father_table.father[i].depth = current_depth;
					father_table.father[i].frequency = frequency;
					father_table.father[i].router_cost = current_depth;
					op_prg_log_entry_write(g_mac_up_debug_log_handle, "111node %d channel_of_recvive_from_father = %f ;band = %d\n",g_mac_node_id,band,father_table.father[i].band);
					op_pk_destroy(pk);
					FOUT;
					}
				}
			father_table.father[no2].panid = panid;
			father_table.father[no2].short_addr = source;
			op_ima_obj_attr_get (channel_of_recvive_from_father	,"min frequency",&band);
			father_table.father[no2].band = (int)((band-470)/4);
			father_table.father[no2].depth = current_depth;
			father_table.father[no2].frequency = frequency;
			father_table.father[no2].router_cost = current_depth;
			op_prg_log_entry_write(g_mac_up_debug_log_handle, "node %d channel_of_recvive_from_father = %f ;band = %d\n",g_mac_node_id,band,father_table.father[no2].band);
			maxcost = 0;
			mincost = 10;
			for (i=0;i<FATHER_NODE_NUM;i++)
				{
				if(maxcost < father_table.father[i].router_cost)
					{
					maxcost = father_table.father[i].router_cost;
					no2 = i;
					}
				if(mincost > father_table.father[i].router_cost)
					{
					mincost = father_table.father[i].router_cost;
					no = i;
					}
				}
			}
		}
	else
		{
		if (op_td_get_dbl (pk, OPC_TDA_RA_RCVD_POWER) < 0.00000000000001 )
			{
			op_pk_destroy(pk);
			FOUT;
			}
		//if (g_mac_synch_flag == NEED_SYNCH && access_control == ACCESS)
		if (g_mac_synch_flag == NEED_SYNCH)
			{
			op_prg_log_entry_write(g_mac_up_info_log_handle, "SYNCH");
			op_intrpt_schedule_self(op_sim_time() + Slot_Time-0.11820, Intrpt_Slot);		//自己的时隙中断
			op_intrpt_schedule_remote(op_sim_time() + Slot_Time-0.11820 ,Intrpt_Slot_Down ,			
				op_id_from_name(g_mac_node_objid,OPC_OBJTYPE_QUEUE,"MAC_DOWN"));				//对下的MAC的时隙中断
			g_mac_synch_flag = SYNCH_COMPLETE;
			g_mac_leaving = COMPLETE_LEAVING;
			}
		}
	op_pk_destroy(pk);
	FOUT;
	}



/*********************************************/
/*********************************************/
/*set receiver's and sender's channel*/
/*********************************************/
/*********************************************/
static void set_channel(int fatherBand,int frequency)
	{
	double recvFreq;
	double sendFreq;
	double a;
	double b;
	FIN(set_channel(int fatherBand,int frequency));
	recvFreq = 470.0+4*fatherBand;
	receiveFrequency = recvFreq;
	sendFreq = recvFreq+frequency*1.0;
	op_ima_obj_attr_set (channel_of_recvive_from_father,"min frequency",recvFreq);
	op_ima_obj_attr_set (channel_of_send_to_father,"min frequency",sendFreq);
	op_ima_obj_attr_get (channel_of_recvive_from_father,"min frequency",&a);
	op_ima_obj_attr_get (channel_of_send_to_father,"min frequency",&b);
	printf("node %d channel_of_recvive_from_father = %f ;channel_of_send_to_father = %f\n",g_mac_node_id,a,b);	
	op_prg_log_entry_write(g_mac_up_debug_log_handle, "node %d channel_of_recvive_from_father = %f ;channel_of_send_to_father = %f\n",g_mac_node_id,a,b);	
	
	op_ima_obj_attr_set (channel_of_father_received,"min frequency",sendFreq);
	FOUT;
	}



/*********************************************/
/*********************************************/
/*创建关联请求帧,并将帧放入发送队列队列头*/
/*********************************************/
/*********************************************/
static void create_associate_frame(int dest,int panid)
	{
	int framecontrol;
	int seq;
	int pan;
	int dest_addr;
	int source;
	int length;
	int subcontrol;
	int type;
	int num_pkts;
	int k;
	int pk_dest;
	Packet* associateRequest;
	FIN(create_associate_frame(int dest,int panid));
	//判断队列中是否已经存在关联请求帧
	num_pkts = op_subq_stat (Data_Queue, OPC_QSTAT_PKSIZE);
	for(k=0;k<num_pkts;k++)
		{
		associateRequest = op_subq_pk_access (Data_Queue, k);
		op_pk_fd_get(associateRequest,3,&pk_dest);
		if(pk_dest==dest)
			{
			printf("Node %d Associate request packet is already in data queue!!!\n",g_mac_node_id);
			FOUT;
			}		
		}
	g_mac_associate_times++;
	framecontrol = 25123;//011 0001000 10 00 11
	type = (framecontrol>>13)&7;
	seq = g_mac_sequence;
	g_mac_sequence = (g_mac_sequence+1)%256;
	pan = panid;
	dest_addr = dest;
	source = g_mac_node_id;
	length = 3;
	subcontrol = 0;
	associateRequest = op_pk_create(0);//120
	op_pk_fd_set (associateRequest, 0, OPC_FIELD_TYPE_INTEGER,framecontrol,16) ;
	op_pk_fd_set (associateRequest, 1, OPC_FIELD_TYPE_INTEGER,seq,8) ;
	op_pk_fd_set (associateRequest, 2, OPC_FIELD_TYPE_INTEGER,pan,16) ;
	op_pk_fd_set (associateRequest, 3, OPC_FIELD_TYPE_INTEGER,dest_addr,16) ;
	op_pk_fd_set (associateRequest, 4, OPC_FIELD_TYPE_INTEGER,source,48) ;
	op_pk_fd_set (associateRequest, 5, OPC_FIELD_TYPE_INTEGER,length,8) ;
	op_pk_fd_set (associateRequest, 7, OPC_FIELD_TYPE_INTEGER,subcontrol,8) ;
	printf("Node %d MAC create an associate frame at %f type = %d dest_addr = %d ,source = %d  \n",g_mac_node_id,op_sim_time(),type,dest_addr,source);
	op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d MAC create an associate frame,dest = %d ,source = %d\n",g_mac_node_id,dest_addr,source);	
	op_subq_pk_insert(Data_Queue,associateRequest,OPC_QPOS_TAIL);	
	FOUT;
	}



/*********************************************/
/*********************************************/
/*更新ack_time[5]   (将发送的ACK帧的计时器重置，同时更新其他帧的计时器位置)*/
/*********************************************/
/*********************************************/
static void update_ack_time()
	{
	int j;
	FIN(update_ack_time());
	for(j = 1;j < 5;j++)
		{
		ack_time[j-1] = ack_time[j];
		}
	ack_time[4] = 100;
	FOUT;
	}



/*********************************************/
/*********************************************/
/*更新重传计时器   (删除发送的帧的计时器，更新其他帧的计时器)*/
/*********************************************/
/*********************************************/
static void update_retrans_time(int i)
	{
	int j;
	FIN(update_retrans_time(int i));
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
/*重置保活结构体对应条目*/
/*********************************************/
/*********************************************/
static void delete_keep_alive()
	{
	FIN(delete_keep_alive());
	keepAlive.dest = 0;
	keepAlive.seq = 0;		
	FOUT;	
	}



static void check_keep_alive()
	{
	int retrans_num;
	int data_num;
	Packet* pk;
	int seq;
	int i;
	FIN(check_keep_alive());
	if(keepAlive.dest!=0)
		{
		data_num = op_subq_stat (Data_Queue, OPC_QSTAT_PKSIZE);
		for(i = 0;i<data_num;i++)
			{
			pk = op_subq_pk_access (Data_Queue, i);
			op_pk_fd_get(pk,1,&seq);
			if(seq == keepAlive.seq)
				{
				delete_keep_alive();
				FOUT;
				}	
			}
		retrans_num = op_subq_stat (Retrans_Queue, OPC_QSTAT_PKSIZE);
		for(i = 0;i<retrans_num;i++)
			{
			pk = op_subq_pk_access (Retrans_Queue, i);
			op_pk_fd_get(pk,1,&seq);
			if(seq == keepAlive.seq)
				{
				delete_keep_alive();
				FOUT;
				}	
			}
		}		
	FOUT;	
	}




/*********************************************/
/*********************************************/
/*删除父节点相关信息*/
/*********************************************/
/*********************************************/
static void delete_neighbor_node()
	{
	FIN(delete_neighbor_node());
	neighbor_table.node_id = 0;
	neighbor_table.band = 0;
	neighbor_table.frequency = 0;
	neighbor_table.seq[0] = 0;
	neighbor_table.active_time = 0;
	FOUT;
	}


	
	

/*********************************************/
/*********************************************/
/*get packet to father node*/
/*********************************************/
/*********************************************/
static void get_msg_to_father()
	{
	Boolean isEmpty_data;
	Boolean isEmpty_retrans;
	Boolean isEmpty_ack;
	int		framecontrol;
	int		seq;
	int		nwk_head;
	int		type;
	int 	subtype;
	int		dest;
	int 	count=0;	
	int NWKControl;
	int NWKtype;
	int subcontrol;	
	FIN(get_msg_to_father());
	if (g_mac_node_status == ONLINE)
		{
		isEmpty_ack = op_subq_empty (ACK_Queue);
		if((isEmpty_ack == OPC_FALSE)&&(ack_time[0]<=0))
			{
			pkptr = op_subq_pk_access (ACK_Queue, 0);
			//add_IES(pk);
			g_mac_pk_type = ACK_FRAME;
			FOUT;
			}
		else
			{
			isEmpty_retrans = op_subq_empty (Retrans_Queue);
			if((isEmpty_retrans == OPC_FALSE)&&(retrans[0].time <= 0))
				{
				if(retrans[0].count>=3)					//重传超过3次的帧处理
					{
					update_retrans_time(1);
					pkptr = op_subq_pk_remove (Retrans_Queue, 0);
					op_pk_fd_get(pkptr,0,&framecontrol);
					op_pk_fd_get(pkptr,3,&dest);
					type = (framecontrol>>13)&7;
					if(type == MAC_CONTROL_FRAME)		//保活帧,对父节点保活失败
						{
						printf("Node %d keep alive to node %d fail!\n",g_mac_node_id,neighbor_table.node_address);
						op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d keep alive to node %d fail!\n",g_mac_node_id,neighbor_table.node_address);
						op_pk_destroy(pkptr);
						MLME_ALIVE_CONFIRM(neighbor_table.node_address,1);				
						delete_keep_alive();					//删除保活结构体条目
						delete_neighbor_node();					//删除父接点表条目	
						}
					else									//其他帧发送失败就开始创建保活帧
						{
						op_pk_fd_get(pkptr,1,&seq);
						printf("Node %d keep alive to node %d!\n",g_mac_node_id,neighbor_table.node_address);
						op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d keep alive to node %d!\n",g_mac_node_id,neighbor_table.node_address);
						op_pk_destroy(pkptr);
						if(keepAlive.dest == neighbor_table.node_address)			//该节点已经在保活了
							{
							printf("Node %d keepAlive = %d\n",g_mac_node_id,keepAlive.dest);
							g_mac_pk_type = DELEDTE_FRAME;
							FOUT;
							}
						create_actice_frame(neighbor_table.node_address,seq);//创建保活帧	
						}
					g_mac_pk_type = DELEDTE_FRAME;
					FOUT;
					}
				retrans_flag = 1;
				pkptr = op_subq_pk_access (Retrans_Queue, 0);
				//add_IES(pk);
				g_mac_pk_type = RETRANS_FRAME;
				FOUT;
				}
			else
				{
				isEmpty_data = op_subq_empty (Data_Queue);
				if(isEmpty_data ==  OPC_FALSE)		//发送数据队列非空
					{
					pkptr = op_subq_pk_access (Data_Queue, 0);
					//add_IES(pk);
					g_mac_pk_type = DATA_FRAME;
					FOUT;
					}	
				else
					{
					g_mac_pk_type = NO_FRAME;
					FOUT;
					}		
				}
			}		
		}
	else if(g_mac_node_status == OFFLINE )		//节点不在网，只能发送入网请求帧,关联请求帧
		{
		isEmpty_ack = op_subq_empty (ACK_Queue);
		if((isEmpty_ack == OPC_FALSE)&&(ack_time[0]<=0))
			{
			pkptr = op_subq_pk_access (ACK_Queue, 0);
			//add_IES(pk);
			g_mac_pk_type = ACK_FRAME;
			FOUT;
			}	
		isEmpty_retrans = op_subq_empty (Retrans_Queue);
		//if((isEmpty_retrans == OPC_FALSE)&&(retrans[0].time <= 0)&&(retrans[0].count<3))
		if((isEmpty_retrans == OPC_FALSE)&&(retrans[0].time <= 0))
			{
			printf("节点%d不在网!!!!!!!\n",g_mac_node_id);
			if(retrans[0].count>=3)
				{
				update_retrans_time(1);
				pkptr = op_subq_pk_remove (Retrans_Queue, 0);
				op_pk_fd_get(pkptr,3,&dest);
				printf("Node %d keep alive to node %d fail!\n",g_mac_node_id,dest);
				op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d keep alive to node %d fail!\n",g_mac_node_id,dest);
				op_pk_destroy(pkptr);
				MLME_ALIVE_CONFIRM(dest,1);				
				delete_neighbor_node();					//删除父接点表条目
				FOUT;
				}
			pkptr = op_subq_pk_access (Retrans_Queue, 0);
			op_pk_fd_get (pkptr, 0, &framecontrol) ;
			type = (framecontrol>>13)&7;					
			op_pk_fd_get (pkptr, 7, &NWKControl) ;
			NWKtype = (NWKControl>>12)&&15;
			if(NWKtype == JOIN_REQ_FRAME)					//入网请求帧
				{
				g_mac_pk_type = RETRANS_FRAME;
				}
			else
				{
				op_subq_pk_remove (Retrans_Queue, 0);
				op_pk_destroy(pkptr);
				update_retrans_time(1);
				g_mac_pk_type = DELEDTE_FRAME;
				}
			FOUT;
			}
		else
			{
			isEmpty_data = op_subq_empty (Data_Queue);
			if(isEmpty_data ==  OPC_FALSE)
				{
				pkptr = op_subq_pk_access (Data_Queue, 0);
				op_pk_fd_get (pkptr,0, &framecontrol) ;
				type = (framecontrol>>13)&7;
				if (type == DATA_FRAME)//data frame
					{
					op_pk_fd_get (pkptr, 7, &nwk_head) ;
					subtype = (nwk_head>>12)&15;
					if(subtype == JOIN_REQ_FRAME)//入网请求帧
						{
						printf("Node %d pretend to send a data frame to join the net\n",g_mac_node_id);
						op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d pretend to send a data frame to join the net\n",g_mac_node_id);
						g_mac_pk_type = DATA_FRAME;
						}
					else
						{
						op_subq_pk_remove (Data_Queue, 0);
						op_pk_destroy(pkptr);
						g_mac_pk_type = DELEDTE_FRAME;
						}
					}
				else if(type == MAC_CONTROL_FRAME)
					{
					op_pk_fd_get (pkptr, 7, &subcontrol) ;
					if(((subcontrol>>4)&15)==ASSOCIATE_REQ_FRAME)//关联请求帧
						{
						printf("Node %d pretend to send an associate frame!!!!!!!\n",g_mac_node_id);
						op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d pretend to send an associate frame!!!!!!!\n",g_mac_node_id);
						g_mac_pk_type = MAC_CONTROL_FRAME;
						}
					else
						{
						op_subq_pk_remove (Data_Queue, 0);
						op_pk_destroy(pkptr);
						g_mac_pk_type = DELEDTE_FRAME;
						}
					}
				else
					{
					op_subq_pk_remove (Data_Queue, 0);
					op_pk_destroy(pkptr);
					g_mac_pk_type = DELEDTE_FRAME;
					}
				}
			else
				{
				g_mac_pk_type = NO_FRAME;
				}
			FOUT;
			}		
		}
	FOUT;
	}



/*********************************************/
/*********************************************/
/*send packet to father node*/
/*********************************************/
/*********************************************/
static void send_msg()
	{
	int framecontrol;
	int AR;
	int dest;
	int seq;
	int	num_pkts;
	double freq;
	Packet* copy;
	double pk_create_time;
	int count;
	FIN(send_msg());
	retrans_flag = 2;
	op_pk_fd_get(pkptr,0,&framecontrol);
	AR = (framecontrol>>10)&1;
	op_pk_fd_get(pkptr,1,&seq);
	op_pk_fd_get(pkptr,3,&dest);
	add_IES(pkptr);
	pk_create_time= op_pk_creation_time_get(pkptr);
	copy = op_pk_copy(pkptr);
	op_pk_creation_time_set(copy,pk_create_time);
	num_pkts = op_subq_stat (Retrans_Queue, OPC_QSTAT_PKSIZE);
	op_ima_obj_attr_get (channel_of_send_to_father,"min frequency",&freq);
	printf("Node %d send freq =%f,time is %f\n",g_mac_node_id,freq,op_sim_time());
	switch(g_mac_pk_type)
		{
		case DATA_FRAME:
			{
			if(num_pkts>=MAX_RETRANS_NUM) 
				{
				op_intrpt_schedule_self(op_sim_time(), Slot_Completed_Code);
				FOUT;
				}
			printf("Node %d send a data packet!dest = %d,seq = %d\n",g_mac_node_id,dest,seq);	
			op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d send a data packet!dest = %d,seq = %d\n",g_mac_node_id,dest,seq);
			route_sink (pkptr);
			op_pk_send(pkptr, OUT_MAC_PHY);
			op_subq_pk_remove(Data_Queue, 0);
			count = 0;
			break;
			}
		case ACK_FRAME:
			{
			printf("Node %d send an ack packet!dest = %d,seq = %d\n",g_mac_node_id,dest,seq);
			op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d send an ack packet!dest = %d,seq = %d\n",g_mac_node_id,dest,seq);
			link_sink(pkptr);
			op_pk_send(pkptr, OUT_MAC_PHY);
			op_subq_pk_remove(ACK_Queue, 0);
			update_ack_time();
			printf("g_mac_sending_time = %f\n",g_mac_sending_time);
			op_intrpt_schedule_self(op_sim_time()+g_mac_sending_time, Send_Completed_Code);
			FOUT;
			}
		case MAC_CONTROL_FRAME:
			{
			if(num_pkts>=MAX_RETRANS_NUM) 
				{
				op_intrpt_schedule_self(op_sim_time(), Slot_Completed_Code);
				FOUT;
				}
			printf("Node %d send a mac control packet!dest = %d,seq = %d\n",g_mac_node_id,dest,seq);
			op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d  send a mac control packet!dest = %d,seq = %d\n",g_mac_node_id,dest,seq);
			link_sink(pkptr);
			op_pk_send(pkptr, OUT_MAC_PHY);
			op_subq_pk_remove(Data_Queue, 0);
			count = 0;
			break;
			}
		case RETRANS_FRAME:
			{
			count = retrans[0].count;
			printf("Node %d send a retrans packet!dest = %d,seq = %d,retrans_time = %d\n",g_mac_node_id,dest,seq,count+1);
			op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d send a retrans packet!dest = %d,seq = %d\n",g_mac_node_id,dest,seq);
			route_sink (pkptr);
			link_sink(pkptr);
			op_pk_send (pkptr, OUT_MAC_PHY);
			op_subq_pk_remove(Retrans_Queue, 0);
			update_retrans_time(1);
			break;
			}
		default:	break;		
		}
	printf("g_mac_sending_time = %f\n",g_mac_sending_time);
	op_intrpt_schedule_self(op_sim_time()+g_mac_sending_time, Send_Completed_Code);
	if(AR == 1)
		{
		globle_up_packet_number++;
		up_packet_number++;
		op_subq_pk_insert(Retrans_Queue,copy,OPC_QPOS_TAIL);
		num_pkts = op_subq_stat (Retrans_Queue, OPC_QSTAT_PKSIZE);
		retrans[num_pkts-1].time = RETRANS_TIME;
		retrans[num_pkts-1].count = count+1;
		printf("Node %d 上行待确认队列包数是  %d\n",g_mac_node_id,num_pkts);
		op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d 上行待确认队列包数是  %d\n",g_mac_node_id,num_pkts);
		}		
	else
		{
		op_pk_destroy(copy);
		}
	FOUT;
	}







/*
up
create an ACK frame,and push it in ACK queue

1:发给上游节点的普通ack帧					source_mode = 2,dest_mode = 2
2:发给上游节点的无目节点ack帧				source_mode = 2,dest_mode = 1
3:发给上游节点的长源地址短目的地址ack帧		source_mode = 3,dest_mode = 2
4:发给上游节点的长源地址无目的地址ack帧		source_mode = 3,dest_mode = 1

5:发给下游节点的普通ack帧					source_mode = 2,dest_mode = 2
6:发给下游节点的长目的地址ack帧				source_mode = 2,dest_mode = 3
7:发给下游节点的无源节点ack帧				source_mode = 1,dest_mode = 2
8:发给下游节点的无源节点长目的节点ack帧		source_mode = 1,dest_mode = 3
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
	int need_seq;
	Packet* ack;
	Packet* ack_in_queue;
	int ack_source;
	int ack_dest;
	int ack_seq;
	int k;
	FIN(create_ack(Packet* pk,int i));
	op_pk_fd_get(pk,0,&framecontrol);
	op_pk_fd_get(pk,1,&pk_seq);
	op_pk_fd_get(pk,2,&pk_panid);
	op_pk_fd_get(pk,3,&pk_dest);
	op_pk_fd_get(pk,4,&pk_source);
	
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
		ack = op_pk_create(0);//80
		framecontrol = 16930;
		op_pk_fd_set(ack,3,OPC_FIELD_TYPE_INTEGER,pk_source,16);
		op_pk_fd_set(ack,4,OPC_FIELD_TYPE_INTEGER,pk_dest,16);
		break;
		
		case 2://source_mode = 2,dest_mode = 1
		ack = op_pk_create(0);//64
		framecontrol = 16914;
		op_pk_fd_set(ack,3,OPC_FIELD_TYPE_INTEGER,pk_source,0);//ooooooo
		op_pk_fd_set(ack,4,OPC_FIELD_TYPE_INTEGER,pk_dest,16);
		break;
		
		case 3://source_mode = 3,dest_mode = 2
		ack = op_pk_create(0);//112
		framecontrol = 16931;
		op_pk_fd_set(ack,3,OPC_FIELD_TYPE_INTEGER,pk_source,16);
		op_pk_fd_set(ack,4,OPC_FIELD_TYPE_INTEGER,pk_dest,48);
		break;
		
		case 4://source_mode = 3,dest_mode = 1
		ack = op_pk_create(0);//96
		framecontrol = 16915;
		op_pk_fd_set(ack,3,OPC_FIELD_TYPE_INTEGER,pk_source,0);//ooooooo
		op_pk_fd_set(ack,4,OPC_FIELD_TYPE_INTEGER,pk_dest,48);
		break;
		
		case 5://source_mode = 2,dest_mode = 2
		ack = op_pk_create(0);//80
		framecontrol = 16930;
		op_pk_fd_set(ack,3,OPC_FIELD_TYPE_INTEGER,pk_source,16);
		op_pk_fd_set(ack,4,OPC_FIELD_TYPE_INTEGER,pk_dest,16);
		break;
		
		case 6://source_mode = 2,dest_mode = 3
		ack = op_pk_create(0);//112
		framecontrol = 16946;
		op_pk_fd_set(ack,3,OPC_FIELD_TYPE_INTEGER,pk_source,48);
		op_pk_fd_set(ack,4,OPC_FIELD_TYPE_INTEGER,pk_dest,16);
		break;
			
		case 7://source_mode = 1,dest_mode = 2
		ack = op_pk_create(0);//64
		framecontrol = 16929;
		op_pk_fd_set(ack,3,OPC_FIELD_TYPE_INTEGER,pk_source,16);
		op_pk_fd_set(ack,4,OPC_FIELD_TYPE_INTEGER,pk_dest,0);//ooooooo
		break;
		
		case 8://source_mode = 1,dest_mode = 3
		ack = op_pk_create(0);//112
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
		if(neighbor_table.seq[i] == 0)
			{
			break;
			}
		}
	need_seq = (neighbor_table.seq[0]+i)%256;	
	op_pk_fd_set(ack,5,OPC_FIELD_TYPE_INTEGER,need_seq,8);//将需要的帧的序列号发过去
//	if((i == 1)||(i == 2)||(i == 3)||(i == 4))
		{
		if (op_subq_pk_insert(ACK_Queue, ack, OPC_QPOS_TAIL) != OPC_QINS_OK)
			{
			printf("\n###Node %d:::data package from route insert into queue failed###\n", g_mac_node_id);
			op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d:::data package from route insert into queue failed\n", g_mac_node_id);
			op_pk_destroy(pk);
			}
		num_pkts = op_subq_stat (ACK_Queue, OPC_QSTAT_PKSIZE);
		if(num_pkts<=5)
			{
			ack_time[num_pkts-1] = ACK_TIME;
			}	
		else
			{
			printf("node %d up ack queue cannot push in!!!\n",g_mac_node_id);
			op_prg_log_entry_write(g_mac_up_debug_log_handle, "node %d up ack queue cannot push in!!!\n",g_mac_node_id);
			}
		}
	FOUT;
	}


/*********************************************/
/*********************************************/
/*更新父节点的最新序列号收发状态*/
/*********************************************/
/*********************************************/
static void update_father_seq()
	{
	int i;
	int j;
	int count = 0;
	FIN(update_father_seq());
	for(i = 1;i < MAX_SEQUENCE_NUM;i++)
		{
		if(neighbor_table.seq[1] == 0)
			{
			break;
			}
		else if(neighbor_table.seq[1] == 1)
			{
			count++;
			for(j = 1;j<MAX_SEQUENCE_NUM-1;j++)
				{
				neighbor_table.seq[j] = neighbor_table.seq[j+1];
				}
			neighbor_table.seq[MAX_SEQUENCE_NUM-1] = 0;
			}
		if(i==5)
			{
			neighbor_table.seq[0] = (neighbor_table.seq[0]+5)%256;
			neighbor_table.seq[1] = 1;
			for(j = 2;j<MAX_SEQUENCE_NUM;j++)
				{
				neighbor_table.seq[j] = 0;
				}
			FOUT;
			}
		}
	neighbor_table.seq[0] = (neighbor_table.seq[0]+count)%256;
	FOUT;
	}


/*********************************************/
/*********************************************/
/*更新父节点的最新序列号
mode：子节点地址类型  2短地址  3长地址
AR：是否需要ACK回复
返回值  1：需要处理包回复ACK，需要交给网络层  
		2：需要处理包回复ACK，不需要交给网络层  
		3；直接删除
*/
/*********************************************/
/*********************************************/
static int update_sequence_from_node(int source, int seq,int source_mode)
	{
	int diff;
	FIN(update_sequence_from_node(int source, int seq,int source_mode));
		{
		diff = seq - neighbor_table.seq[0];
		if(diff < -250) diff = seq+256-neighbor_table.seq[0];	//超过255
		
		//if(diff <= 0&&diff>(-5))									//已经接收过的帧
		if((diff <= 0&&diff>(-5))||diff>=250)
			{
			printf("Node %d receive an packet from up seq is old!!!\n",g_mac_node_id);
			op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d receive an packet from up seq is old!!!\n",g_mac_node_id);
			FRET(2);
			}
		else if(diff<(MAX_SEQUENCE_NUM-1)&&diff>0)		//正常帧
			{
			if(neighbor_table.seq[diff] == 0) 		//帧没有被接收过
				{
				neighbor_table.seq[diff] = 1;		//序列号为seq[0]+diff 的包收到了
				update_father_seq();
				FRET(1);
				}
			FRET(2);									//帧被接受过了
			}
		else if(diff>6&&diff<250)											//异常帧序号
			{
			printf("Node %d receive an packet from up seq is over!!!\n",g_mac_node_id);
			op_prg_log_entry_write(g_mac_up_info_log_handle, "Node %d receive an packet from up seq is over!!!\n",g_mac_node_id);
			MLME_CONFLICT_CONFIRM(neighbor_table.node_address);	//报告短地址冲突
			FRET(3);
			}
		else
			{
			printf("\t\t\thuachuang == 5\n");
			if(neighbor_table.seq[2]+neighbor_table.seq[3]+neighbor_table.seq[4]>=2) 
				{
				neighbor_table.seq[1] = 1;	
				update_father_seq();
				FRET(1);
				}
			FRET(3);
			}
		}		
	}





/*********************************************/
/*********************************************/
/*对MAC 命令帧的处理*/
/*********************************************/
/*********************************************/
static void mac_frame_proc(Packet* pk)
	{
	//Packet* packet;
	int 	framecontrol;
	int		seq;
	int		type;
	int 	subcontrol;
	int		length;
	int		subtype;
	int 	dest;
	int 	source;
	FIN(mac_frame_proc(Packet* pk));
	op_pk_fd_get(pk,0,&framecontrol);
	op_pk_fd_get(pk,1,&seq);
	op_pk_fd_get(pk,3,&dest);
	op_pk_fd_get(pk,4,&source);
	op_pk_fd_get(pk,5,&length);
	type = (framecontrol>>13)&7;
	if(type == MAC_CONTROL_FRAME)
		{
		printf("Node %d receive a MAC control frame!\n",g_mac_node_id);
		op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d receive a MAC control frame!\n",g_mac_node_id);
		op_pk_fd_get(pk,7,&subcontrol);
		subtype = (subcontrol>>4)&15;
		if(subtype == KEEP_ALIVE_FRAME)
			{
			neighbor_table.active_time = ACTIVE_TIME;
			create_ack(pk,1);
			}
		}
	FOUT;
	}






static void ack_frame_proc1(Packet* pk)
	{
	int need_seq;
	int framecontrol;
	int dest_mode ;
	int seq;
	int dest;
	int ack_num;
	int i;
	Packet* sub_ack;
	FIN(ack_frame_proc1(Packet* pk));
	//if(g_mac_node_id ==1 )printf("Node %d receive an ack packet from up!!!\n",g_mac_node_id);
	op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d receive an ack packet from up!!!\n",g_mac_node_id);
	op_pk_fd_get (pk, 0, &framecontrol) ;
	op_pk_fd_get (pk, 10, &ack_num) ;
	//printf("ack_num = %d\n",ack_num);
	for(i = 0;i<ack_num;i++)
		{
//		printf("i = %d\n",i);
		op_pk_fd_get (pk, (11+i), &sub_ack) ;
		op_pk_fd_get (sub_ack, 0, &dest_mode) ;
		op_pk_fd_get (sub_ack, 1, &dest) ;
		op_pk_fd_get (sub_ack, 2, &seq) ;
		op_pk_fd_get (sub_ack, 3, &need_seq) ;
		//if(g_mac_node_id ==1 )printf("dest_mode %d dest %d seq %d next_hop(dest) =%d!!!\n",dest_mode,dest,seq,next_hop(dest));
		if(keepAlive.dest!=0)
			{
			if(seq == keepAlive.seq)
				{
				//完成保活原语
				//MLME_ALIVE_CONFIRM(source,0);
				delete_keep_alive();
				}
			}
		if(dest_mode == ESN_ADDRESS_MODE && dest == g_mac_node_id )//长地址,
			{
			ack_proc1(dest,seq,need_seq);
			}
		else if(dest_mode == SHORT_ADDRESS_MODE && next_hop(dest) > 0)//短地址
			{
			ack_proc1(dest,seq,need_seq);
			}
		//op_pk_destroy(sub_ack);
		}
	op_pk_destroy(pk);
	FOUT;	
	}



static void ack_proc1(int dest,int seq,int need_seq )
	{
	int 	i,j;
	Packet* packet;
	int 	ack;				//用于存放重传队列中读取的包的序列号
	int 	number;
	int 	data_source;		//待确认队列中帧的源节点
	int		data_dest;			//待确认队列中帧的目的节点
	int		find_ack;
	FIN(ack_proc1(int dest,int seq,int need_seq ));
	find_ack = 0;
	number = op_subq_stat (Retrans_Queue, OPC_QSTAT_PKSIZE);	
	for(i=0;i<number;i++)
		{
		packet = op_subq_pk_access (Retrans_Queue, i);
		op_pk_fd_get(packet,1,&ack);
		op_pk_fd_get(packet,4,&data_source);
		op_pk_fd_get(packet,3,&data_dest);
		//if(g_mac_node_id ==1 )printf("packet seq =%d ,source = %d!!!\n",ack,data_source);
		if((ack == seq)&&(data_source == dest))
			{
			find_ack = 1;
			packet = op_subq_pk_remove (Retrans_Queue, i);
			//删除计数器****************************************
			for(j = i;j < MAX_RETRANS_NUM-1;j++)
				{
				retrans[j].time = retrans[j+1].time;
				retrans[j].count = retrans[j+1].count;
				}
			retrans[MAX_RETRANS_NUM-1].time = 100;
			retrans[MAX_RETRANS_NUM-1].count = 0;
			globle_up_receive_ack_number++;
			up_receive_ack_number++;
			printf("\tNODE %dfind the packet!!!,seq = %d need_seq = %d\n",g_mac_node_id,seq,need_seq);
			op_prg_log_entry_write(g_mac_up_debug_log_handle,"NODE %dfind the packet!!!,seq = %d\n",g_mac_node_id,seq);
			//op_pk_destroy(packet);
			i--;
			number--;
			//printf("i = %d .number =%d\n",i,number);
			//FOUT;
			break;
			}
		}
	if(find_ack == 1)
		{
		number = op_subq_stat (Retrans_Queue, OPC_QSTAT_PKSIZE);
		for(i=0;i<number;i++)
			{
		/*--------------------------------------------------------------------------*/
			packet = op_subq_pk_access (Retrans_Queue, i);
			op_pk_fd_get(packet,1,&ack);
			if((ack-need_seq)<-250) ack = ack+256;
			if(need_seq<5&&ack>250) ack = ack-256;
			if(ack<need_seq)
				{
				globle_up_receive_ack_number++;
				up_receive_ack_number++;
				packet = op_subq_pk_remove (Retrans_Queue, i);
				//删除计数器****************************************
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
/*对ACK帧的处理*/
/*********************************************/
/*********************************************/
static void ack_frame_proc(Packet* pk)
	{
	int need_seq;
	int AR;		
	int framecontrol;
	int dest_mode ;
	int source_mode;
	int seq;
	int dest;
	int source;
	FIN(ack_frame_proc(Packet* pk));
	printf("Node %d receive an ack packet from up!!!\n",g_mac_node_id);
	op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d receive an ack packet from up!!!\n",g_mac_node_id);
	op_pk_fd_get (pk, 0, &framecontrol) ;
	op_pk_fd_get (pk, 1, &seq) ;
	op_pk_fd_get (pk, 3, &dest);
	op_pk_fd_get (pk, 4, &source);
	op_pk_fd_get (pk, 5, &need_seq);
	AR = (framecontrol>>10)&1;
	dest_mode = (framecontrol>>4)&3;
	source_mode = framecontrol&3;	
	//if(source == keepAlive.dest)		//一跳节点保活
	if(keepAlive.dest!=0)	
		{
		if(seq == keepAlive.seq)
			{
			//完成保活原语
			//MLME_ALIVE_CONFIRM(source,0);
			delete_keep_alive();
			}
		}	
	if(dest_mode == SHORT_ADDRESS_MODE)
		{
		if((dest == g_mac_short_address)||(next_hop(dest) != -1))
			{
			ack_proc(seq,source,dest,need_seq);		
			op_pk_destroy(pk);
			}
		else
			{
			op_pk_destroy(pk);
			}	
		FOUT;
		}
	else if(dest_mode == ESN_ADDRESS_MODE)
		{
		if(dest == g_mac_node_id)
			{
			ack_proc(seq,source,dest,need_seq);		
			op_pk_destroy(pk);
			}
		else
			{
			op_pk_destroy(pk);
			}	
		FOUT;
		}
	else
		{
		op_pk_destroy(pk);
		FOUT;
		}
	FOUT;
	}




/*********************************************/
/*********************************************/
/*对ACK帧的处理
seq:   seq of ACK packet,the same as the packet to be ACK
dest:  dest of ACK packet,the same as the source of the packet to be confirm*/
/*********************************************/
/*********************************************/
static void ack_proc(int seq ,int source ,int dest,int need_seq)
	{
	int 	i,j;
	Packet* packet;
	int 	ack;				//用于存放重传队列中读取的包的序列号
	int 	number;
	int 	data_source;		//待确认队列中帧的源节点
	int		data_dest;			//待确认队列中帧的目的节点
//	int 	cur_time;
//	int 	cur_count;
	
	FIN(ack_proc(int seq ,int source ,int dest,int need_seq));
	number = op_subq_stat (Retrans_Queue, OPC_QSTAT_PKSIZE);	
	for(i=0;i<number;i++)
		{
		packet = op_subq_pk_access (Retrans_Queue, i);
		op_pk_fd_get(packet,1,&ack);
		op_pk_fd_get(packet,4,&data_source);
		op_pk_fd_get(packet,3,&data_dest);
		if((ack == seq)&&(data_source == dest)&&(data_dest == source))
			{
			packet = op_subq_pk_remove (Retrans_Queue, i);
			//删除计数器****************************************
			for(j = i;j < MAX_RETRANS_NUM-1;j++)
				{
				retrans[j].time = retrans[j+1].time;
				retrans[j].count = retrans[j+1].count;
				}
			retrans[MAX_RETRANS_NUM-1].time = 100;
			retrans[MAX_RETRANS_NUM-1].count = 0;
			printf("\tNODE %dfind the packet!!!,seq = %d\n",g_mac_node_id,seq);
			op_prg_log_entry_write(g_mac_up_debug_log_handle,"NODE %dfind the packet!!!,seq = %d\n",g_mac_node_id,seq);
			op_pk_destroy(packet);
			i--;
			number--;
			FOUT;
			}		
		/*--------------------------------------------------------------------------*/
		if((ack-need_seq)<-250) ack = ack+256;
		if(need_seq<5&&ack>250) ack = ack-256;
		//if((ack<need_seq)&&(next_hop(dest)==next_hop(data_source)))
		if(ack<need_seq)
			{
			packet = op_subq_pk_remove (Retrans_Queue, i);
			//删除计数器****************************************
			for(j = i;j < MAX_RETRANS_NUM-1;j++)
				{
				retrans[j].time = retrans[j+1].time;
				retrans[j].count = retrans[j+1].count;
				}
			retrans[MAX_RETRANS_NUM-1].time = 100;
			retrans[MAX_RETRANS_NUM-1].count = 0;
			op_pk_destroy(packet);
			i--;
			number--;
			continue;
			}
		//if((ack==need_seq)&&(next_hop(dest)==next_hop(data_source)))
		/*if(ack==need_seq)
			{
			op_subq_pk_swap (Retrans_Queue, OPC_QPOS_HEAD, i);
			cur_time = retrans[i].time;
			cur_count = retrans[i].count;
			retrans[i].time =  retrans[0].time;
			retrans[i].count =  retrans[0].count;
			retrans[0].time = cur_time;
			retrans[0].count = cur_count;
			continue;
			}*/
		}
	printf("wrong ack frame!!!\n");
	FOUT;
	}



/*********************************************/
/*********************************************/
/*查询路由表*/
/*********************************************/
/*********************************************/
static int next_hop(int dest)
	{
	int i = 0;
	int router;
	FIN(next_hop(int dest));
	if(dest == 0) FRET(43690);
	router = dest&(255<<8);
	if(dest == g_mac_node_id||dest == g_mac_short_address)
		{
		FRET(dest);
		}
	if(router == 0) FRET(-1);
	if(router == g_mac_short_address)
		{
		FRET(dest);
		}		
	for(i = 0;i < Network_Msg.NetworkRT[g_mac_node_id].number;i++)
		{
		if(((Network_Msg.NetworkRT[g_mac_node_id].table[i].destAddress)<<8) == router && Network_Msg.NetworkRT[g_mac_node_id].table[i].status == 0)
			{
			FRET((Network_Msg.NetworkRT[g_mac_node_id].table[i].nextHop)<<8);
			}
		}
	FRET(-1);
	}



/*********************************************/
/*********************************************/
/*更新保活时间，节点保活*/
/*********************************************/
/*********************************************/
static void update_alive_time(int source)
	{
	FIN(update_alive_time(int source));
	neighbor_table.active_time = ACTIVE_TIME;
	FOUT;
	}




/*********************************************/
/*********************************************/
/*Add IEs*/
/*********************************************/
/*********************************************/
static void add_IES(Packet* pk)
	{
	int framecontrol;
	FIN(add_IES(Packet* pk));
	op_pk_fd_get(pk,0,&framecontrol);
	if(MulticastContent == 0)
		{
		FOUT;
		}
	else
		{
		framecontrol = framecontrol + 64;//IE oresent = 1;
		op_pk_fd_set (pk, 0, OPC_FIELD_TYPE_INTEGER,framecontrol,16) ;
		op_pk_fd_set(pk,6,OPC_FIELD_TYPE_INTEGER,MulticastContent,16);
		}
	FOUT;
	}




/*********************************************/
/*********************************************/
//创建保活消息MAC命令帧
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
	FIN(create_actice_frame(int dest,int seq));
	alive = op_pk_create(0);//112
	frame_control	= 26146;			//011 0011000 10 00 10
	panid 			= g_mac_pan_id;
	source 			= g_mac_short_address;
	length			= 88;
	subcontrol 		= 32;
	seq = g_mac_sequence;
	g_mac_sequence = (g_mac_sequence+1)%256;
	op_pk_fd_set (alive, 0, OPC_FIELD_TYPE_INTEGER,frame_control,16) ;
	op_pk_fd_set (alive, 1, OPC_FIELD_TYPE_INTEGER,seq,8) ;
	op_pk_fd_set (alive, 2, OPC_FIELD_TYPE_INTEGER,panid,16) ;
	op_pk_fd_set (alive, 3, OPC_FIELD_TYPE_INTEGER,dest,16) ;
	op_pk_fd_set (alive, 4, OPC_FIELD_TYPE_INTEGER,source,16) ;
	op_pk_fd_set (alive, 5, OPC_FIELD_TYPE_INTEGER,length,8) ;
	op_pk_fd_set (alive, 7, OPC_FIELD_TYPE_INTEGER,subcontrol,8) ;
	add_IES(alive);
	if(keepAlive.dest != 0)//该节点已经在保活了
		{
		op_pk_destroy(alive);
		FOUT;
		}
	if (op_subq_pk_insert(Data_Queue, alive, OPC_QPOS_TAIL) != OPC_QINS_OK)
		{
		printf("\n###Node %d:::data package from route insert into queue failed###\n", g_mac_node_id);
		op_prg_log_entry_write(g_mac_up_debug_log_handle,"Node %d:::data package from route insert into queue failed###\n", g_mac_node_id);
		op_pk_destroy(alive);
		}
	keepAlive.dest = neighbor_table.node_address;
	keepAlive.seq = seq;
	FOUT;
	}






/*********************************************/
/*********************************************/
/*CSMA-CA*/
/*********************************************/
/*********************************************/
/*CSMA-CA参数初始化*/
static void csma_init ()
	{
	FIN(csma_init ());
	g_mac_num_backoffs = 0;//当前包退避次数
	g_mac_max_backoffs = 5;//最大退避次数
	g_mac_backoff_exponent = macMinBE;//------------
	g_mac_unit_backoff_period = 0.01;	
	FOUT;	
	}



/*********************************************/
/*计算退避时间*/
static void csma_schedule_backoff ()
	{
	double  backoff_periods = 0.0;
	
	FIN (csma_schedule_backoff ());	
	if (g_mac_num_backoffs != 0)
		{
		if (g_mac_backoff_exponent+1 > macMaxBE)
			g_mac_backoff_exponent = macMaxBE;
		else
			g_mac_backoff_exponent = g_mac_backoff_exponent+1;
		}
	// calculate the backoff duration which is a multiple of the backoff periods
	backoff_periods = (int)(op_dist_uniform (pow (2.0, g_mac_backoff_exponent) - 1));
	//printf("Node %d  g_mac_backoff_exponent = %d backoff_periods = %f,current time is %f\n",g_mac_node_id,g_mac_backoff_exponent,backoff_periods,op_sim_time());
	g_mac_backoff_duration = backoff_periods* g_mac_unit_backoff_period;
	//printf("Node %d backoff time = %f\n",g_mac_node_id,g_mac_backoff_duration);
//	op_intrpt_schedule_self (op_sim_time () + g_mac_backoff_duration, WPANC_CSMA_TIMER_EXPIRED);
//	g_mac_num_backoffs ++;
	FOUT;
	}


static void csma_schedule_backoff1 ()
	{	
	FIN (csma_schedule_backoff1 ());
	g_mac_backoff_duration = op_dist_uniform(0.015);
	FOUT;
	}

static void csma_backoff ()
	{
	FIN (csma_backoff ());	
	op_intrpt_schedule_self (op_sim_time () + g_mac_backoff_duration, WPANC_CSMA_TIMER_EXPIRED);
	g_mac_num_backoffs ++;
	FOUT;
	}



/*********************************************/
/*判断信道是否空闲*/
static int csma_is_channel_free ()
	{
	double rx_power = 0.0;
	int channel_status;
	
	FIN (csma_is_channel_free ());	
	rx_power = op_stat_local_read (2);	
	if (rx_power > 0.0)
		{
		channel_status = OPC_FALSE;
		}
	else
		{
		channel_status = OPC_TRUE;
		}
	FRET (channel_status);	
	}


/*********************************************/
/*判断信道是否空闲*/
static int channel_from_father_detect()
	{
	double rx_power = 0.0;
	int channel_status;
	
	FIN (channel_from_father_detect ());	
	rx_power = op_stat_local_read (1);	
	if (rx_power > 0.0)
		{
		channel_status = OPC_FALSE;
		}
	else
		{
		channel_status = OPC_TRUE;
		}
	FRET (channel_status);	
	}


/*
static void set_channel_start_send ()
	{
	double recvFreq;
	FIN(set_channel_start_send());
	recvFreq = 4700.0;
	op_strm_flush(IN_PHY_MAC);
	op_ima_obj_attr_set (channel_of_recvive_from_father,"min frequency",recvFreq);
	printf("Node %d receive from father frequency id %f,time is %f\n",g_mac_node_id,recvFreq,op_sim_time());
	FOUT;
	}


static void set_channel_start_receive ()
	{
	FIN(set_channel_start_receive());
	op_strm_flush(IN_PHY_MAC);
	op_ima_obj_attr_set (channel_of_recvive_from_father,"min frequency",receiveFrequency);
	printf("Node %d receive from father frequency id %f,time is %f\n",g_mac_node_id,receiveFrequency,op_sim_time());
	FOUT;
	}
*/

static void set_channel_start_send ()
	{
	FIN(set_channel_start_send());
	op_strm_flush(IN_PHY_MAC);
	g_mac_receiving_flag = SENDING;
	//if(g_mac_node_id == 1) printf("start send at %f\n",op_sim_time());
	FOUT;
	}
static void set_channel_start_receive ()
	{
	double a;
	FIN(set_channel_start_receive());
	op_strm_flush(IN_PHY_MAC);
	g_mac_receiving_flag = RECEIVING;
	op_ima_obj_attr_get (channel_of_recvive_from_father,"min frequency",&a);
	//if(g_mac_node_id == 1) printf("start receive at %f,receive at %f\n",op_sim_time(),a);
	FOUT;
	}


static void route_sink (Packet* pk)
	{
	int framecontrol;
	int type;
	int subtype;
	int nwk_head;
	double pk_size1;
	FIN(route_sink(Packet* pk));
	op_pk_fd_get (pk, 0, &framecontrol) ;
	type = (framecontrol>>13)&7;
	pk_size1 = op_pk_total_size_get(pk);	
	if(type == DATA_FRAME)
		{
		op_pk_fd_get (pk, 7, &nwk_head) ;
		subtype = (nwk_head>>12)&15;
		if(subtype == 0||subtype == 1||subtype == 2||subtype == 3||subtype == 4||subtype == 6||subtype == 11||subtype == 12||subtype == 13||subtype == 14)
			{
			globle_up_route_cost = globle_up_route_cost+pk_size1;
			up_route_cost = up_route_cost+pk_size1;
			} 
		}
	FOUT;
	}


static void link_sink (Packet* pk)
	{
	double pk_size1;
	FIN(route_sink(Packet* pk));
	pk_size1 = op_pk_total_size_get(pk);	
	globle_up_link_cost = globle_up_link_cost+pk_size1;
	up_link_cost = up_link_cost+pk_size1;
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
	void WSN_MAC_UP_NEW2 (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_WSN_MAC_UP_NEW2_init (int * init_block_ptr);
	void _op_WSN_MAC_UP_NEW2_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_WSN_MAC_UP_NEW2_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_WSN_MAC_UP_NEW2_alloc (VosT_Obtype, int);
	void _op_WSN_MAC_UP_NEW2_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
WSN_MAC_UP_NEW2 (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (WSN_MAC_UP_NEW2 ());

		{


		FSM_ENTER ("WSN_MAC_UP_NEW2")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "init", "WSN_MAC_UP_NEW2 [init enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_UP_NEW2 [init enter execs]", state0_enter_exec)
				{
				PrgT_Log_Severity_Type   severity;
				Compcode                 status;
				
				g_mac_process_objid = op_id_self();															//获取本节点的实体
				g_mac_node_objid    = op_topo_parent(g_mac_process_objid);
				
				//subnet_objid     = op_topo_parent (my_node_objid);			
				op_ima_obj_attr_get (g_mac_node_objid,	"g_node_ESN_address",	&g_mac_node_id);
				op_ima_obj_attr_get (g_mac_node_objid,	"g_node_type"	,	&g_mac_node_type);
				op_ima_obj_attr_get (g_mac_node_objid,	"g_node_status",	&g_mac_node_status);
				op_ima_obj_attr_get (g_mac_node_objid,	"g_node_error_rate",	&frame_error_rate);
				op_ima_obj_attr_get (g_mac_node_objid,	"g_node_MAC_mode",	&CSMA_MODE);
				
				
				receiver_from_father = op_id_from_name (g_mac_node_objid, OPC_OBJTYPE_RARX , "receive from father"); 
				sender_to_father = op_id_from_name (g_mac_node_objid, OPC_OBJTYPE_RATX , "send to father");
				
				receive_from_father_comp = op_topo_child (receiver_from_father, OPC_OBJTYPE_COMP , 0); 
				sender_to_father_comp = op_topo_child (sender_to_father, OPC_OBJTYPE_COMP , 0); 
				
				channel_of_recvive_from_father = op_topo_child (receive_from_father_comp, OPC_OBJTYPE_RARXCH , 0); 
				channel_of_send_to_father = op_topo_child (sender_to_father_comp, OPC_OBJTYPE_RATXCH , 0); 
				
				g_mac_up_info_log_handle = op_prg_log_handle_create(OpC_Log_Category_Configuration, "MAC_UP", "info", 1);
				status = op_prg_log_handle_severity_get (g_mac_up_info_log_handle, &severity);
				if ((status != OPC_COMPCODE_FAILURE) && (severity != PrgC_Log_Severity_Information))
				   op_prg_log_handle_severity_set (&g_mac_up_info_log_handle, PrgC_Log_Severity_Information); 
				
				g_mac_up_debug_log_handle = op_prg_log_handle_create(OpC_Log_Category_Configuration, "MAC_UP", "debug", 1);
				status = op_prg_log_handle_severity_get (g_mac_up_debug_log_handle, &severity);
				if ((status != OPC_COMPCODE_FAILURE) && (severity != PrgC_Log_Severity_Debug))
				   op_prg_log_handle_severity_set (&g_mac_up_debug_log_handle, PrgC_Log_Severity_Debug); 
				
				mac_init();
				
				op_prg_log_entry_write(g_mac_up_info_log_handle, "Node %d up mac start\n",g_mac_node_id );
				
				//contr_send_num=op_stat_reg("group1.pk_contr_number",OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				//sendNum = 0;
				//op_intrpt_schedule_self(op_sim_time(), Start_Code);
				father_receiver = op_id_from_name (g_mac_node_objid, OPC_OBJTYPE_RARX , "father received"); 
				father_received_comp = op_topo_child (father_receiver, OPC_OBJTYPE_COMP , 0); 
				channel_of_father_received = op_topo_child (father_received_comp, OPC_OBJTYPE_RARXCH , 0); 
				
				
				
				
				pkt_success_stathandle		=op_stat_reg ("Traffic Sink.up_pk_success",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				g_pkt_success_stathandle	=op_stat_reg ("Traffic Sink.up_g_pk_success",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				packet_number_stathandle	=op_stat_reg ("Traffic Sink.up_packet_number",	OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				g_packet_number_stathandle	=op_stat_reg ("Traffic Sink.up_g_packet_number",	OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				up_success_rate				=op_stat_reg ("Traffic Sink.up_success_rate",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				g_up_success_rate			=op_stat_reg ("Traffic Sink.up_g_success_rate",	OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				up_route_cost_stathandle	=op_stat_reg ("Traffic Sink.up_route_cost",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				g_up_route_cost_stathandle	=op_stat_reg ("Traffic Sink.up_g_route_cost",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				up_link_cost_stathandle		=op_stat_reg ("Traffic Sink.up_link_cost",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				g_up_link_cost_stathandle	=op_stat_reg ("Traffic Sink.up_g_link_cost",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				//up_net_cost_stathandle		=op_stat_reg ("Traffic Sink.up_net_cost",			OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				//g_up_net_cost_stathandle	=op_stat_reg ("Traffic Sink.up_g_net_cost",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				
				
				
				
				
				
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** state (init) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "init", "WSN_MAC_UP_NEW2 [init exit execs]")


			/** state (init) transition processing **/
			FSM_TRANSIT_FORCE (6, state6_enter_exec, ;, "default", "", "init", "waiting", "tr_3", "WSN_MAC_UP_NEW2 [init -> waiting : default / ]")
				/*---------------------------------------------------------*/



			/** state (scan) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "scan", state1_enter_exec, "WSN_MAC_UP_NEW2 [scan enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_UP_NEW2 [scan enter execs]", state1_enter_exec)
				{
				int framecontrol;
				int type;
				double freq;
				Packet* pk;
				
				
				Packet* pk2;
				
				if((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == 3))
					{
					pk2 = op_pk_get(3);
					op_pk_destroy(pk2);
					}
				
				
				if(C_SELF_NEW_SLOT)
					{
					op_intrpt_schedule_self(op_sim_time() + Slot_Time, Intrpt_Slot);				//自己的时隙中断
					g_mac_slot_number = (g_mac_slot_number+1)%10;
					}
				
				if (C_SELF_START_SCAN)
					{
					op_prg_log_entry_write(g_mac_up_debug_log_handle, "NODE %d start Scan!!!!!\n",g_mac_node_id );
					scan_channel();
					op_ima_obj_attr_get (channel_of_recvive_from_father	,"min frequency",&freq);	
					}
				
				if (C_SELF_CHANGE_RECEIVER_CHANNEL)
					{
					change_band();
					op_ima_obj_attr_get (channel_of_recvive_from_father	,"min frequency",&freq);
					}
				
				if (C_STRM_MSG_FROM_FATHER_NODE)
					{
					pk= op_pk_get(IN_PHY_MAC);
					op_pk_fd_get(pk,0,&framecontrol);
					type = (framecontrol>>13)&7;
					if(type == BEACON_FRAME) 
						{
						op_ima_obj_attr_get (channel_of_recvive_from_father	,"min frequency",&freq);
						//printf("Node %d receive beacon frequency is %f!!!\n",g_mac_node_id,freq);
						//printf("Node %d receive beacon at %f!!!\n",g_mac_node_id,op_sim_time());
						beacon_proc(pk);
						}
					}
				}
				FSM_PROFILE_SECTION_OUT (state1_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"WSN_MAC_UP_NEW2")


			/** state (scan) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "scan", "WSN_MAC_UP_NEW2 [scan exit execs]")


			/** state (scan) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_MAC_UP_NEW2 [scan trans conditions]", state1_trans_conds)
			FSM_INIT_COND (g_mac_scan_flag == NO_SCAN)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("scan")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 6, state6_enter_exec, ;, "g_mac_scan_flag == NO_SCAN", "", "scan", "waiting", "tr_10", "WSN_MAC_UP_NEW2 [scan -> waiting : g_mac_scan_flag == NO_SCAN / ]")
				FSM_CASE_TRANSIT (1, 1, state1_enter_exec, ;, "default", "", "scan", "scan", "tr_14", "WSN_MAC_UP_NEW2 [scan -> scan : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (associating) enter executives **/
			FSM_STATE_ENTER_UNFORCED (2, "associating", state2_enter_exec, "WSN_MAC_UP_NEW2 [associating enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_UP_NEW2 [associating enter execs]", state2_enter_exec)
				{
				int type;
				int framecontrol;
				int dest;
				int source;
				int subtype;
				int subcontrol;
				int result;
				int sequence;
				int i;
				OpT_Packet_Id pk_id;
				Packet* pk;
				int pkptr_size;
				Packet* pk2;
				
				if((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == 3))
					{
					pk2 = op_pk_get(3);
					op_pk_destroy(pk2);
					}
				//int send_to_Father;
				
				//时隙远程中断
				if(C_SELF_NEW_SLOT)
					{
					set_channel_start_receive ();
					g_mac_slot_start_time = op_sim_time();
					op_intrpt_schedule_self(op_sim_time() + Slot_Time, Intrpt_Slot);				//自己的时隙中断
					g_mac_slot_number = (g_mac_slot_number+1)%10;
					op_intrpt_schedule_self(op_sim_time()+Wait_time, Channel_Detect_Code);
					}
				
				//没有收到帧就开启上传
				if(C_SELF_CHANNEL_DETECT)
					{
					if(OPC_TRUE == channel_from_father_detect())
						{
						op_intrpt_schedule_self(op_sim_time(), Up_Code);
						}
					}
				if(C_SELF_UP_TIME)
					{
					set_channel_start_send ();
					//上行时间到，发送关联请求帧
					g_mac_num_backoffs = 0;
					get_msg_to_father();
					while(g_mac_pk_type == DELEDTE_FRAME)
						{
						get_msg_to_father();
						}
					if(g_mac_pk_type!=NO_FRAME)
						{
						pkptr_size = op_pk_total_size_get(pkptr);
						g_mac_sending_time = pkptr_size/g_mac_data_rate;
						//printf("g_mac_sending_time = %f\n",g_mac_sending_time);
						if(CSMA_MODE == 0)				//使用ALOHA
							{
							g_mac_rand = op_dist_uniform (1.0);
							if(g_mac_rand<0.3)
								send_msg();
							}
						else if(CSMA_MODE == 1)
							{
							csma_schedule_backoff ();
							csma_backoff();
							}
						}
					}
						
				if(BACKOFF_TIMER_EXPIRED)					//退避时间到
					{
					get_msg_to_father();
					while(g_mac_pk_type == DELEDTE_FRAME)
						{
						get_msg_to_father();
						}
					if(g_mac_pk_type!=NO_FRAME)
						{
						if(CH_FREE)
							{
							send_msg();
							g_mac_num_backoffs = 0;
							g_mac_backoff_exponent = macMinBE;
							}
						else if(!CH_FREE&&!BACKOFF_LIMIT_REACHED)		//信道繁忙，退避次数未到最大值
							{
							g_mac_current_time = op_sim_time();
							csma_schedule_backoff ();
							if(g_mac_current_time+g_mac_sending_time+g_mac_backoff_duration-g_mac_slot_start_time<0.498)
								{
								csma_backoff();
								}
							}
						else											//信道繁忙，退避次数到达最大值
							csma_init();
						}
					}
				
				
				if(C_STRM_MSG_FROM_FATHER_NODE && g_mac_receiving_flag == RECEIVING)
					{
					op_intrpt_schedule_self(op_sim_time()+Protection_time, Up_Code);		
					pk = op_pk_get(IN_PHY_MAC);
					pk_id = op_pk_id(pk);
					op_pk_fd_get(pk,0,&framecontrol);
					op_pk_fd_get(pk,1,&sequence);
					op_pk_fd_get(pk,7,&subcontrol);
					type = (framecontrol>>13)&7;
					printf("type = %d\t;",type);
					subtype = (subcontrol>>4)&15;
					printf("subtype = %d\t",subtype);
					printf("Node %d MAC receive a packet!!!\n",g_mac_node_id);
					op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d MAC receive a packet!!!,type = %d,subtype = %d,seq = %d\n",g_mac_node_id,type,subtype,sequence);
					if(type == MAC_CONTROL_FRAME && subtype == ASSOCIATE_RESP_FRAME)
						//关联回复帧
						{		
						op_pk_fd_get(pk,3,&dest);
						if(dest == g_mac_node_id)
							{
							op_pk_fd_get(pk,4,&source);
							//printf("source = %d,associate_target = %d\n",source,g_mac_associate_target);
							if(source == g_mac_associate_target)
								{
								//g_mac_sequence = (g_mac_sequence+1)%256;
								printf("Node %d MAC receive an associate response frame at %f\n",g_mac_node_id,op_sim_time());
								op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d MAC receive an associate response frame\n",g_mac_node_id);
								g_mac_recv_associate_response = RECV_ASSOCIATE_RESP; 	//收到目标节点关联回复帧
								g_mac_associate_times = 0;								//重置关联次数
								op_pk_fd_get(pk,8,&result);
								if(result == 2)
									//关联成功
									{
									printf("Node %d associate success\n",g_mac_node_id);
									op_prg_log_entry_write(g_mac_up_info_log_handle, "Node %d associate success\n",g_mac_node_id);
									op_subq_flush(Data_Queue);
									MLME_ASSOCIATE_CONFIRM(0);
									for(i=0;i<FATHER_NODE_NUM;i++)
										{
										if(father_table.father[i].short_addr == g_mac_associate_target)
											{
											op_ima_obj_attr_set (g_mac_node_objid,	"g_node_depth",father_table.father[i].depth+1);
											op_ima_obj_attr_get (g_mac_node_objid,	"g_node_depth",&g_mac_node_depth);
											break;
											}
										}
									neighbor_table.seq[0] = sequence;
									neighbor_table.active_time = ACTIVE_TIME;
									op_intrpt_schedule_self(op_sim_time(), Associate_Success_Code);
									}
								else
									//关联失败
									{
									printf("Node %d associate failed\n",g_mac_node_id);
									op_prg_log_entry_write(g_mac_up_info_log_handle, "Node %d associate failed\n",g_mac_node_id);
									MLME_ASSOCIATE_CONFIRM(1);
									/*
									for(i = 0;i<FATHER_NODE_NUM;i++)
										{
										if(father_table.father[i].short_addr == source)
											{
											father_table.father[i].short_addr = 0;
											father_table.father[i].panid = 0;
											father_table.father[i].band = 0;
											father_table.father[i].depth = 0;
											father_table.father[i].frequency = 0;
											father_table.father[i].router_cost = 0;
											father_table.father[i].times = 0;
											}
										}	
									*/
									neighbor_table.node_address = 0;
									neighbor_table.band = 0;
									neighbor_table.frequency = 0;
									op_intrpt_schedule_self(op_sim_time(), Associate_Fail_Code);
									}
								}
							}	
						op_pk_destroy(pk);
						}
					else if(type == BEACON_FRAME)
						{
						beacon_proc(pk);
						}
					else
						{
						op_pk_destroy(pk);
						}
				
					}
				if(C_SELF_START_ASSOCIATE)
					{
					if(g_mac_synch_flag == NEED_SYNCH)		//同步失败，直接关联失败
						{
						printf("Node %d synch failed!\n",g_mac_node_id);
						MLME_ASSOCIATE_CONFIRM(1);
						g_mac_associate_times = 0;
						neighbor_table.node_address = 0;
						neighbor_table.band = 0;
						neighbor_table.frequency = 0;
						op_intrpt_schedule_self(op_sim_time(), Associate_Fail_Code);
						}
					else
						{
						if(g_mac_recv_associate_response == NO_ASSOCIATE_RESP)  		//没有收到关联回复帧
							{
							if(g_mac_associate_times < 3)	//关联请求帧的发送次数小于3，再发一次关联请求
								{
								create_associate_frame(g_mac_associate_target,g_mac_associate_pan_id);
								op_intrpt_schedule_self(op_sim_time()+MAX_ASSOCIATE_TIME, Associate_Code);
								}
							else
								{						//关联失败
								printf("Node %d associate failed,more than three times\n",g_mac_node_id);
								op_prg_log_entry_write(g_mac_up_info_log_handle, "Node %d associate failed,more than three times\n",g_mac_node_id);
								MLME_ASSOCIATE_CONFIRM(1);
								g_mac_associate_times = 0;
								neighbor_table.node_address = 0;
								neighbor_table.band = 0;
								neighbor_table.frequency = 0;
								op_intrpt_schedule_self(op_sim_time(), Associate_Fail_Code);
								}
							}			
						}	
					}
				
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (5,"WSN_MAC_UP_NEW2")


			/** state (associating) exit executives **/
			FSM_STATE_EXIT_UNFORCED (2, "associating", "WSN_MAC_UP_NEW2 [associating exit execs]")


			/** state (associating) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_MAC_UP_NEW2 [associating trans conditions]", state2_trans_conds)
			FSM_INIT_COND (C_SELF_ASSOCIATE_SUCCESS)
			FSM_TEST_COND (C_SELF_ASSOCIATE_FAIT)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("associating")
			FSM_PROFILE_SECTION_OUT (state2_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 3, state3_enter_exec, ;, "C_SELF_ASSOCIATE_SUCCESS", "", "associating", "online", "tr_8", "WSN_MAC_UP_NEW2 [associating -> online : C_SELF_ASSOCIATE_SUCCESS / ]")
				FSM_CASE_TRANSIT (1, 6, state6_enter_exec, ;, "C_SELF_ASSOCIATE_FAIT", "", "associating", "waiting", "tr_18", "WSN_MAC_UP_NEW2 [associating -> waiting : C_SELF_ASSOCIATE_FAIT / ]")
				FSM_CASE_TRANSIT (2, 2, state2_enter_exec, ;, "default", "", "associating", "associating", "tr_21", "WSN_MAC_UP_NEW2 [associating -> associating : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (online) enter executives **/
			FSM_STATE_ENTER_UNFORCED (3, "online", state3_enter_exec, "WSN_MAC_UP_NEW2 [online enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_UP_NEW2 [online enter execs]", state3_enter_exec)
				{
				Packet* pk2;
				Packet* pk;
				int type;
				
				//if(g_mac_node_id == 1) printf("Node %d  in 6 ,time is %f\n",g_mac_node_id,op_sim_time());
				if((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == 3))
					{
					pk2 = op_pk_get(3);
					op_pk_destroy(pk2);
					}
				
				
				if(C_SELF_CHANNEL_DETECT)
					{
					if(OPC_TRUE == channel_from_father_detect())		//信道空闲状态
						{
						op_intrpt_schedule_self(op_sim_time(), Up_Code);	//信道空闲直接开启上行
						}
					}
				if(C_SELF_UP_TIME && CSMA_MODE == 1)
					{
				
					set_channel_start_send ();
					get_msg_to_father();
					while(g_mac_pk_type == DELEDTE_FRAME)
						{
						get_msg_to_father();
						}
					if(g_mac_pk_type == DATA_FRAME||g_mac_pk_type == ACK_FRAME||g_mac_pk_type == MAC_CONTROL_FRAME||g_mac_pk_type == RETRANS_FRAME)
						{
						//printf("222Node %d g_mac_pk_type = %d\n",g_mac_node_id,g_mac_pk_type);
						csma_init();
						//printf("Node %d first back off\n",g_mac_node_id);
						csma_schedule_backoff ();
						csma_backoff ();
						}			
					}
				
				
				if(g_mac_leaving == MAC_LEAVING)
					{
					cheak_mac_queue();
					}
				
				if(C_STRM_NWK_MLME&&g_mac_node_status != ONLINE&&g_mac_leaving != LEAVING_COMPELTE_NEED_SYNCH)
					{
					//if(g_mac_node_id == 199) printf("g_mac_node_status = %d time = %f\n",g_mac_node_status,op_sim_time());
					pk = op_pk_get(IN_NWK_MAC_MLME);
					//printf("pk_id = %d ,time is %f\n",op_pk_id(pk),op_sim_time());
					op_pk_fd_get(pk,0,&type);
					if(type == MLME_LEAVE_REQUEST)
						{
						//MLME_LEAVE_CONFIRM(1);//对上天线完成离网		
						mac_leave();
						op_intrpt_schedule_self(op_sim_time(), Mac_Leave_Code);
						}
					op_pk_destroy(pk);
					}
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (7,"WSN_MAC_UP_NEW2")


			/** state (online) exit executives **/
			FSM_STATE_EXIT_UNFORCED (3, "online", "WSN_MAC_UP_NEW2 [online exit execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_UP_NEW2 [online exit execs]", state3_exit_exec)
				{
				//if(g_mac_node_id == 1) printf("Node %d  out 6 ,time is %f\n",g_mac_node_id,op_sim_time());
				}
				FSM_PROFILE_SECTION_OUT (state3_exit_exec)


			/** state (online) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_MAC_UP_NEW2 [online trans conditions]", state3_trans_conds)
			FSM_INIT_COND (C_SELF_NEW_SLOT)
			FSM_TEST_COND (C_STRM_MSG_FROM_FATHER_NODE)
			FSM_TEST_COND (C_SELF_UP_TIME&&CSMA_MODE==0)
			FSM_TEST_COND (C_SLEF_LEAVE)
			FSM_TEST_COND (C_STRM_NWK_MLME&&g_mac_node_status == ONLINE)
			FSM_TEST_COND (C_STRM_NWK_DATA)
			FSM_TEST_COND (BACKOFF_TIMER_EXPIRED&&CSMA_MODE==1&&slot_complete_flag==2)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("online")
			FSM_PROFILE_SECTION_OUT (state3_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 5, state5_enter_exec, set_channel_start_receive ();, "C_SELF_NEW_SLOT", "set_channel_start_receive ()", "online", "slot_intrpt", "tr_24", "WSN_MAC_UP_NEW2 [online -> slot_intrpt : C_SELF_NEW_SLOT / set_channel_start_receive ()]")
				FSM_CASE_TRANSIT (1, 4, state4_enter_exec, ;, "C_STRM_MSG_FROM_FATHER_NODE", "", "online", "msg from up", "tr_29", "WSN_MAC_UP_NEW2 [online -> msg from up : C_STRM_MSG_FROM_FATHER_NODE / ]")
				FSM_CASE_TRANSIT (2, 7, state7_enter_exec, set_channel_start_send ();, "C_SELF_UP_TIME&&CSMA_MODE==0", "set_channel_start_send ()", "online", "send", "tr_56", "WSN_MAC_UP_NEW2 [online -> send : C_SELF_UP_TIME&&CSMA_MODE==0 / set_channel_start_send ()]")
				FSM_CASE_TRANSIT (3, 6, state6_enter_exec, ;, "C_SLEF_LEAVE", "", "online", "waiting", "tr_74", "WSN_MAC_UP_NEW2 [online -> waiting : C_SLEF_LEAVE / ]")
				FSM_CASE_TRANSIT (4, 9, state9_enter_exec, ;, "C_STRM_NWK_MLME&&g_mac_node_status == ONLINE", "", "online", "MLME", "tr_108", "WSN_MAC_UP_NEW2 [online -> MLME : C_STRM_NWK_MLME&&g_mac_node_status == ONLINE / ]")
				FSM_CASE_TRANSIT (5, 8, state8_enter_exec, ;, "C_STRM_NWK_DATA", "", "online", "App_Msg_Up", "tr_110", "WSN_MAC_UP_NEW2 [online -> App_Msg_Up : C_STRM_NWK_DATA / ]")
				FSM_CASE_TRANSIT (6, 10, state10_enter_exec, back_off_count = 0;, "BACKOFF_TIMER_EXPIRED&&CSMA_MODE==1&&slot_complete_flag==2", "back_off_count = 0", "online", "cca_back_off", "tr_125", "WSN_MAC_UP_NEW2 [online -> cca_back_off : BACKOFF_TIMER_EXPIRED&&CSMA_MODE==1&&slot_complete_flag==2 / back_off_count = 0]")
				FSM_CASE_TRANSIT (7, 3, state3_enter_exec, ;, "default", "", "online", "online", "tr_130", "WSN_MAC_UP_NEW2 [online -> online : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (msg from up) enter executives **/
			FSM_STATE_ENTER_FORCED (4, "msg from up", state4_enter_exec, "WSN_MAC_UP_NEW2 [msg from up enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_UP_NEW2 [msg from up enter execs]", state4_enter_exec)
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
				int i = 0;
				int mult;
				int flag = 0;
				int need_ack;
				double error;
				op_ima_obj_attr_get (g_mac_node_objid,	"g_node_status",	&g_mac_node_status);
				if(g_mac_node_status == ONLINE) op_ima_obj_attr_get (g_mac_node_objid,	"g_node_short_address",	&g_mac_short_address);
				if(C_STRM_MSG_FROM_FATHER_NODE && g_mac_receiving_flag == RECEIVING)
				{
				op_intrpt_schedule_self(op_sim_time()+Protection_time, Up_Code);
				pk = op_pk_get(IN_PHY_MAC);
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
						update_alive_time(neighbor_table.node_address);						//保活重置	
						op_pk_fd_get (pk, 0, &framecontrol) ;
						op_pk_fd_get (pk, 1, &seq) ;
						op_pk_fd_get (pk, 3, &dest);
						dest_mode = (framecontrol>>4)&3;
						source_mode = framecontrol&3;
						mult = (framecontrol>>8)&1;
						type = (framecontrol>>13)&7;
						AR = (framecontrol>>10)&1;
						//printf("Node %d next_hop(dest) = %d!!!\n",g_mac_node_id,next_hop(dest));	
						if(next_hop(dest) > 0)
							{
							printf("Node %d MAC reveive a packet from up node!!!\n",g_mac_node_id);	
							printf("seq = %d\t",seq);
							printf("source = %d\t",source);
							printf("dest = %d\t",dest);
							printf("ar= %d\n",AR);
							printf("dest mode = %d\t",dest_mode);
							printf("source_mode = %d\t",source_mode);		
							printf("type= %d\n",type);
							}
						if(mult == 1||(dest == 65535&&type!=0)||dest ==65534||dest ==65533||dest ==65532)
							{
							//需要组播
							if(type == ACK_FRAME)
								{
								ack_frame_proc1(pk);
								}
							else
								{
								printf("Node %d receive a multcast frame\n",g_mac_node_id);	
								op_pk_send_delayed(pk, OUT_MAC_NWK_DATA,0.5);
								}
							}
						else
							{
							if(type == BEACON_FRAME&&g_mac_leaving != MAC_LEAVING)
								{
								//printf("Node %d receive a beacon at slot %d ;time = %f\n",g_mac_node_id,g_mac_slot_number,op_sim_time());	
								//beacon frame ,
								beacon_proc(pk);		
								}
							else if(type == ACK_FRAME)
								{				
								ack_frame_proc(pk);
								}
							else if((type == DATA_FRAME||type == MAC_CONTROL_FRAME)&&g_mac_leaving != MAC_LEAVING)
								{
								if(g_mac_leaving == MAC_LEAVING)	op_pk_destroy(pk);
								op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d receive an data or MAC control packet from up!!!\n",g_mac_node_id);
								if(dest_mode == SHORT_ADDRESS_MODE)
									{								//需要本节点接受的帧
									if((dest == g_mac_short_address)||(next_hop(dest) != -1))
										{
										printf("Node %d receive an data or MAC control packet from up!!!\n",g_mac_node_id);
										need_ack = update_sequence_from_node(source, seq,source_mode);
										}
									else							//不需要本节点接收的帧
										{
										need_ack = 3;
										}
									}
								else if(dest_mode == ESN_ADDRESS_MODE)
									{
									if(dest == g_mac_node_id)
										{
										printf("Node %d receive an data or MAC control packet from up!!!\n",g_mac_node_id);
										need_ack = update_sequence_from_node(source, seq,source_mode);
										}
									else
										{
										need_ack = 3;
										}					
									}
								
								if(need_ack == 3)
									{
									op_pk_destroy(pk);
									}
								else
									{
									if(dest_mode == SHORT_ADDRESS_MODE)
										{
										if(type == DATA_FRAME)
											{
											if((source_mode == SHORT_ADDRESS_MODE)&&(AR == 1))
												{
												printf("Node %d create an ACK1 packet!!!\n",g_mac_node_id);
												create_ack(pk, 1);
												}
											else if((source_mode == GATE_ADDRESS_MODE)&&(AR == 1))
												{
												printf("Node %d create an ACK2 packet!!!\n",g_mac_node_id);
												create_ack(pk, 2);
												}
											if(need_ack == 1) op_pk_send_delayed (pk, OUT_MAC_NWK_DATA,0.5);
											}
										else if(type == MAC_CONTROL_FRAME)
											{
											if(need_ack == 1) mac_frame_proc(pk);
											else if(need_ack == 2) 
												{
												if((source_mode == SHORT_ADDRESS_MODE)&&(AR == 1))
													create_ack(pk, 1);
												else if((source_mode == GATE_ADDRESS_MODE)&&(AR == 1))
													create_ack(pk, 2);
												}
											}
										}
									else if(dest_mode == ESN_ADDRESS_MODE)
										{
										if(type == DATA_FRAME)
											{
											if((source_mode == SHORT_ADDRESS_MODE)&&(AR == 1))
												{
												printf("Node %d create an ACK3 packet!!!\n",g_mac_node_id);
												create_ack( pk, 3);
												}
											else if((source_mode == GATE_ADDRESS_MODE)&&(AR == 1))
												{
												printf("Node %d create an ACK4 packet!!!\n",g_mac_node_id);
												create_ack(pk, 4);
												}
											if(need_ack == 1) op_pk_send_delayed (pk, OUT_MAC_NWK_DATA,0.5);
											}
										else if(type == MAC_CONTROL_FRAME)
											{
											if(need_ack == 1) mac_frame_proc(pk);
											else if(need_ack == 2) 
												{
												if((source_mode == SHORT_ADDRESS_MODE)&&(AR == 1))
													create_ack(pk, 3);
												else if((source_mode == GATE_ADDRESS_MODE)&&(AR == 1))
													create_ack(pk, 4);
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
								op_pk_destroy(pk);
								}
							}		
						}
					}
				else
					{
					printf("Node %d received an error packet up!!!\n",g_mac_node_id);
					op_pk_destroy(pk);
					}	
				}
				}
				FSM_PROFILE_SECTION_OUT (state4_enter_exec)

			/** state (msg from up) exit executives **/
			FSM_STATE_EXIT_FORCED (4, "msg from up", "WSN_MAC_UP_NEW2 [msg from up exit execs]")


			/** state (msg from up) transition processing **/
			FSM_TRANSIT_FORCE (3, state3_enter_exec, ;, "default", "", "msg from up", "online", "tr_70", "WSN_MAC_UP_NEW2 [msg from up -> online : default / ]")
				/*---------------------------------------------------------*/



			/** state (slot_intrpt) enter executives **/
			FSM_STATE_ENTER_FORCED (5, "slot_intrpt", state5_enter_exec, "WSN_MAC_UP_NEW2 [slot_intrpt enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_UP_NEW2 [slot_intrpt enter execs]", state5_enter_exec)
				{
				int i;
				double band;
				int frequency;
				double rate;
				double g_rate;
				if(C_SELF_NEW_SLOT)
					{
					op_stat_write (packet_number_stathandle, 		up_packet_number);
					op_stat_write (pkt_success_stathandle, 			up_receive_ack_number);
					op_stat_write (g_packet_number_stathandle, 		globle_up_packet_number);
					op_stat_write (g_pkt_success_stathandle, 		globle_up_receive_ack_number);
					rate = (double)up_receive_ack_number/(double)up_packet_number;
					g_rate = (double)globle_up_receive_ack_number/(double)globle_up_packet_number;
					op_stat_write (up_success_rate, 			rate);
					op_stat_write (g_up_success_rate, 		g_rate);
					
					op_stat_write (up_link_cost_stathandle, 		up_link_cost);
					op_stat_write (g_up_link_cost_stathandle, 		globle_up_link_cost);
					op_stat_write (up_route_cost_stathandle, 		up_route_cost);
					op_stat_write (g_up_route_cost_stathandle, 		globle_up_route_cost);	
					
						
					
					op_ima_obj_attr_get (g_mac_node_objid,	"g_node_status",	&g_mac_node_status);
					if(g_mac_node_status == ONLINE) 
						{
						op_ima_obj_attr_get (g_mac_node_objid,	"g_node_short_address",	&g_mac_short_address);
						op_ima_obj_attr_get (g_mac_node_objid,	"g_node_frequency",	&frequency);
						op_ima_obj_attr_get (channel_of_recvive_from_father,"min frequency",&band);
						op_ima_obj_attr_set (channel_of_send_to_father,"min frequency",(band+frequency));
						op_ima_obj_attr_set (channel_of_father_received,"min frequency",band+frequency);
						}
					g_mac_slot_start_time = op_sim_time();
					slot_complete_flag = 2;
					op_intrpt_schedule_self(op_sim_time() + Slot_Time, Intrpt_Slot);				//自己的时隙中断
					g_mac_slot_number = (g_mac_slot_number+1)%10;
					op_intrpt_schedule_self(op_sim_time()+Wait_time, Channel_Detect_Code);
						
					for(i = 0;i < Max_ACK;i++)
						{
						if(ack_time[i]<=3)
							{
							ack_time[i]--;
							}
						}
					for(i = 0;i <MAX_RETRANS_NUM;i++)
						{
						if(retrans[i].time<=10)
							{
							retrans[i].time--;
							}
						}
					neighbor_table.active_time--;
					if(neighbor_table.active_time == 50)					//达到保活危险值
						{
						create_actice_frame(neighbor_table.node_address,-1);
						neighbor_table.active_time = 49;
						}
					if(neighbor_table.active_time == 1)					//节点失活
						{
						MLME_ALIVE_CONFIRM(neighbor_table.node_address,1);
						}
					}
				}
				FSM_PROFILE_SECTION_OUT (state5_enter_exec)

			/** state (slot_intrpt) exit executives **/
			FSM_STATE_EXIT_FORCED (5, "slot_intrpt", "WSN_MAC_UP_NEW2 [slot_intrpt exit execs]")


			/** state (slot_intrpt) transition processing **/
			FSM_TRANSIT_FORCE (3, state3_enter_exec, ;, "default", "", "slot_intrpt", "online", "tr_25", "WSN_MAC_UP_NEW2 [slot_intrpt -> online : default / ]")
				/*---------------------------------------------------------*/



			/** state (waiting) enter executives **/
			FSM_STATE_ENTER_UNFORCED (6, "waiting", state6_enter_exec, "WSN_MAC_UP_NEW2 [waiting enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_UP_NEW2 [waiting enter execs]", state6_enter_exec)
				{
				int type;
				int fatherBand;
				int frequency;
				int i;
				int source;
				int framecontrol;
				Packet* pk;
				Packet* pk2;
				/************************************/
				/************************************/
				/************************************/
				if((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == 3))
					{
					pk2 = op_pk_get(3);
					op_pk_destroy(pk2);
					}
				
				//时隙中断
				if(C_SELF_NEW_SLOT)
					{
					set_channel_start_receive ();
					op_intrpt_schedule_self(op_sim_time() + Slot_Time, Intrpt_Slot);			//自己的时隙中断
					//printf("UP node %d, slot %d,time %f\n",my_node_id,slot,op_sim_time());
					g_mac_slot_number = (g_mac_slot_number+1)%10;
					op_intrpt_schedule_self(op_sim_time()+Wait_time, Channel_Detect_Code);
					}
				
				//没有收到帧就开启上传时间
				if(C_SELF_CHANNEL_DETECT)
					{
					if(OPC_TRUE == channel_from_father_detect())
						{
						op_intrpt_schedule_self(op_sim_time(), Up_Code);
						}
					}
				
				if(C_STRM_MSG_FROM_FATHER_NODE && g_mac_receiving_flag == RECEIVING)
					{
					//收到信息后10ms后开始向上游节点发送信息
					pk = op_pk_get(IN_PHY_MAC);
					op_pk_fd_get(pk,0,&framecontrol);
					type = (framecontrol>>13)&7;
					op_pk_fd_get (pk, 4, &source);
					if(g_mac_synch_flag == NEED_SYNCH && type == BEACON_FRAME && source == g_mac_associate_target)
						{
						beacon_proc(pk);
						}
					else
						{
						op_pk_destroy(pk);
						}
					op_intrpt_schedule_self(op_sim_time()+Protection_time, Up_Code);
					}
				
				
				if(C_STRM_NWK_MLME)
					{
					pkptr = op_pk_get(IN_NWK_MAC_MLME);
					op_pk_fd_get(pkptr,0,&type);
					if(type == MLME_SCAN_REQUEST)			//信道扫描原语
						{
						printf("Node %d MAC Receive a MLME_SCAN.request\n",g_mac_node_id);
						op_prg_log_entry_write(g_mac_up_info_log_handle, "Node %d MAC Receive a MLME_SCAN.request\n",g_mac_node_id );
						for (i=0;i<FATHER_NODE_NUM;i++)
							{
							father_table.father[i].short_addr = 0;
							father_table.father[i].panid = 0;
							father_table.father[i].band = 0;
							father_table.father[i].depth = 0;
							father_table.father[i].frequency = 0;
							father_table.father[i].router_cost = 10;
							father_table.father[i].times = 0;
							}
						g_mac_scan_flag = NEED_SCAN;
						op_intrpt_schedule_self(op_sim_time(), Scan_Code);
						}
					if(type == MLME_ASSOCIATE_REQUEST)							//关联请求原语
						{
						printf("Node %d MAC Receive a MLME_ASSOCIATE.request\n",g_mac_node_id);
						op_prg_log_entry_write(g_mac_up_info_log_handle, "Node %d MAC Receive a MLME_ASSOCIATE.request\n",g_mac_node_id );		
						op_pk_fd_get(pkptr,1,&g_mac_associate_target);
						op_pk_fd_get(pkptr,2,&g_mac_associate_pan_id);
						op_pk_fd_get(pkptr,3,&fatherBand);
						op_pk_fd_get(pkptr,4,&frequency);
						set_channel(fatherBand,frequency);						//设置收发信机频点
						if(g_mac_node_type != GATE_NODE) g_mac_synch_flag = NEED_SYNCH;
						neighbor_table.node_address = g_mac_associate_target;
						neighbor_table.band = fatherBand;
						neighbor_table.frequency = frequency;
						g_mac_recv_associate_response = NO_ASSOCIATE_RESP;
						g_mac_associate_times = 0;
						op_intrpt_schedule_self(op_sim_time()+6, Associate_Code);
						}
					op_pk_destroy(pkptr);
					}
				}
				FSM_PROFILE_SECTION_OUT (state6_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (13,"WSN_MAC_UP_NEW2")


			/** state (waiting) exit executives **/
			FSM_STATE_EXIT_UNFORCED (6, "waiting", "WSN_MAC_UP_NEW2 [waiting exit execs]")


			/** state (waiting) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_MAC_UP_NEW2 [waiting trans conditions]", state6_trans_conds)
			FSM_INIT_COND (C_SELF_START_SCAN)
			FSM_TEST_COND (C_SELF_START_ASSOCIATE)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("waiting")
			FSM_PROFILE_SECTION_OUT (state6_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, ;, "C_SELF_START_SCAN", "", "waiting", "scan", "tr_4", "WSN_MAC_UP_NEW2 [waiting -> scan : C_SELF_START_SCAN / ]")
				FSM_CASE_TRANSIT (1, 2, state2_enter_exec, ;, "C_SELF_START_ASSOCIATE", "", "waiting", "associating", "tr_17", "WSN_MAC_UP_NEW2 [waiting -> associating : C_SELF_START_ASSOCIATE / ]")
				FSM_CASE_TRANSIT (2, 6, state6_enter_exec, ;, "default", "", "waiting", "waiting", "tr_13", "WSN_MAC_UP_NEW2 [waiting -> waiting : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (send) enter executives **/
			FSM_STATE_ENTER_UNFORCED (7, "send", state7_enter_exec, "WSN_MAC_UP_NEW2 [send enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_UP_NEW2 [send enter execs]", state7_enter_exec)
				{
				int pkptr_size;
				//int count;
				//int slot_number;
				//int send_number;
				Packet* pk2;
				int type;
				Packet* pk;
				int band;
				int frequency;
				//int a;
				int dest = -1;
				//int id = -1;
				int subtype;
				int framecontrol;
				int nwk_head;
				int	pkt_num;
				int seq;
				
				/*无用包*/
				if((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == 3))
					{
					pk2 = op_pk_get(3);
					op_pk_destroy(pk2);
					}
				
				
				/********************************************************************/
				/********************************************************************/
				/********************************************************************/
				/********************************************************************/
				/*发送数据*/
				//if(C_SELF_UP_TIME&&retrans_flag == 1)
				if(C_SELF_UP_TIME)
					{
					g_mac_rand = op_dist_uniform (1.0);
					if(g_mac_rand<0.3)
						{	
						get_msg_to_father();
						while(g_mac_pk_type == DELEDTE_FRAME)
							{
							get_msg_to_father();
							}
						if(g_mac_pk_type == DATA_FRAME||g_mac_pk_type == ACK_FRAME||g_mac_pk_type == MAC_CONTROL_FRAME||g_mac_pk_type == RETRANS_FRAME)
						//if(g_mac_pk_type == DATA_FRAME||g_mac_pk_type == ACK_FRAME||g_mac_pk_type == MAC_CONTROL_FRAME)
							{
							pkptr_size = op_pk_total_size_get(pkptr);
							g_mac_sending_time = pkptr_size/g_mac_data_rate;
					//		printf("g_mac_sending_time = %f\n",g_mac_sending_time);
							send_msg();
							}
						else if(g_mac_pk_type == NO_FRAME)
						//else if(g_mac_pk_type == NO_FRAME||g_mac_pk_type == RETRANS_FRAME)
							{
							op_intrpt_schedule_self(op_sim_time(), Slot_Completed_Code);
							}
						
						}
					else
						{
						op_intrpt_schedule_self(op_sim_time(), Slot_Completed_Code);
						}
					}
				/*
				else if(C_SELF_UP_TIME&&retrans_flag == 2)
					{
					count = retrans[0].count;
					g_mac_rand = op_dist_uniform (3*count*2);
					slot_number = ((int)g_mac_rand)/2;
					op_intrpt_schedule_self(op_sim_time(), Slot_Completed_Code);
					op_intrpt_schedule_self(op_sim_time()+0.5*slot_number+send_number+0.15, 10);
					retrans_flag = 3;
					}
				
				if(retrans_flag == 3&&(op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == 10))
					{
					send_msg();
					}
				
				*/
				if(C_SELF_SEND_COMPLETED)
					{
					printf("Node %d send complete\n",g_mac_node_id);
					g_mac_current_time = op_sim_time();
					if(g_mac_rand<0.3)
						{	
						get_msg_to_father();
						while(g_mac_pk_type == DELEDTE_FRAME)
							{
							get_msg_to_father();
							}
						if(g_mac_pk_type == DATA_FRAME||g_mac_pk_type == ACK_FRAME||g_mac_pk_type == MAC_CONTROL_FRAME||g_mac_pk_type == RETRANS_FRAME)
							{
							pkptr_size = op_pk_total_size_get(pkptr);
							g_mac_sending_time = pkptr_size/g_mac_data_rate;
					//		printf("Node %d g_mac_current_time =%f,g_mac_sending_time =%f,g_mac_slot_start_time =%f\n",g_mac_node_id,g_mac_current_time,g_mac_sending_time,g_mac_slot_start_time);
							if(g_mac_current_time+g_mac_sending_time-g_mac_slot_start_time < 0.498)
								{
								send_msg();
								}
							else
								{
								op_intrpt_schedule_self(op_sim_time(), Slot_Completed_Code);
								}
							}
						else if(g_mac_pk_type == NO_FRAME)
							{
							op_intrpt_schedule_self(op_sim_time(), Slot_Completed_Code);
							}
						}
					else
						{
						op_intrpt_schedule_self(op_sim_time(), Slot_Completed_Code);
						}
					}
				
				
				
				
				
				/********************************************************************/
				/********************************************************************/
				/********************************************************************/
				/********************************************************************/
				/*网络层数据到达处理*/
				if(C_STRM_NWK_DATA)
					{
					if(g_mac_leaving != MAC_LEAVING)
						{
						pk = op_pk_get(IN_NWK_MAC_DATA);
						pkt_num = op_subq_stat (Data_Queue, OPC_QSTAT_PKSIZE);
						if(pkt_num<=5)
							{
							seq = g_mac_sequence;
							g_mac_sequence = (g_mac_sequence+1)%256;
							op_pk_fd_get(pk,0, &framecontrol);
							op_pk_fd_get(pk,3, &dest);
							op_pk_fd_get(pk,7, &nwk_head);
							op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,seq,8);
							printf("Node %d MAC receive a frame from NWK dest is father node!  seq = %d  ;;;",g_mac_node_id,seq);
							subtype = (nwk_head>>12)&15;
							type = (framecontrol>>13)&7;
							printf("type = %d  ;;;",type);
							printf("subtype = %d  ;;;",subtype);
							printf("dest = %d\n",dest);
							if (op_subq_pk_insert(Data_Queue, pk, OPC_QPOS_TAIL) != OPC_QINS_OK)
								{
								printf("\n###Node %d:::data package from route insert into queue failed###\n", g_mac_node_id);
								op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d:::data package from route insert into queue failed###\n", g_mac_node_id);
								op_pk_destroy(pk);
								}
							op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d MAC receive a frame from NWK dest is father node!seq = %d,type = %d,subtype = %d,dest = %d!!!\n",g_mac_node_id,seq,type,subtype,dest);
							}
						else
							{
							printf("Node %d Data_queue is full\n", g_mac_node_id);
							}
						}
					}
				
				
				/********************************************************************/
				/********************************************************************/
				/********************************************************************/
				/********************************************************************/
				/*原语到达处理*/
				if(C_STRM_NWK_MLME&&g_mac_node_status == ONLINE)
					{
					pk = op_pk_get(IN_NWK_MAC_MLME);
					op_pk_fd_get(pk,0,&type);
					switch(type)
						{
						case 5: //组播
							{
							printf("Node %d receive a MLME_MULTICAST.request!\n",g_mac_node_id);
							op_prg_log_entry_write(g_mac_up_info_log_handle,"Node %d receive a MLME_MULTICAST.request!\n",g_mac_node_id);			
							op_pk_fd_get(pk,1,&MulticastContent);
							break;
							}
						case MLME_SET_CHANNEL:
							{
							if(g_mac_node_type == BACKUP_NODE)
								{
								op_ima_obj_attr_get (g_mac_node_objid,	"g_node_type",	&g_mac_node_type);
								op_pk_fd_get(pk,0,&band);
								op_pk_fd_get(pk,1,&frequency);
								set_channel(band,frequency);		//设置频段频点
								break;
								}
							
							}
						case MLME_LEAVE_REQUEST:
							{
							//a =op_pk_id(pk);
							//printf("pk_id = %d ,time is %f \n",a,op_sim_time());
							printf("Node %d up pretend to leave!\n",g_mac_node_id);		
							op_prg_log_entry_write(g_mac_up_info_log_handle,"Node %d up pretend to leave!\n",g_mac_node_id);				
							g_mac_leaving = MAC_LEAVING;
							break;
							}
						default : break;
						}
					op_pk_destroy(pk);
					}
				}
				FSM_PROFILE_SECTION_OUT (state7_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (15,"WSN_MAC_UP_NEW2")


			/** state (send) exit executives **/
			FSM_STATE_EXIT_UNFORCED (7, "send", "WSN_MAC_UP_NEW2 [send exit execs]")


			/** state (send) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_MAC_UP_NEW2 [send trans conditions]", state7_trans_conds)
			FSM_INIT_COND (C_SELF_SLOT_COMPLETED)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("send")
			FSM_PROFILE_SECTION_OUT (state7_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 3, state3_enter_exec, ;, "C_SELF_SLOT_COMPLETED", "", "send", "online", "tr_58", "WSN_MAC_UP_NEW2 [send -> online : C_SELF_SLOT_COMPLETED / ]")
				FSM_CASE_TRANSIT (1, 7, state7_enter_exec, ;, "default", "", "send", "send", "tr_59", "WSN_MAC_UP_NEW2 [send -> send : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (App_Msg_Up) enter executives **/
			FSM_STATE_ENTER_FORCED (8, "App_Msg_Up", state8_enter_exec, "WSN_MAC_UP_NEW2 [App_Msg_Up enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_UP_NEW2 [App_Msg_Up enter execs]", state8_enter_exec)
				{
				/* recv app data, save in subqueue */
				int dest = -1;
				int type = -1;
				int id = -1;
				int subtype;
				Packet* pk;
				int framecontrol;
				int nwk_head;
				int	pkt_num;
				int seq;
				//if((NET == 1)||(NET == 0)||(NET == 4))
				//if(((g_mac_node_status == ONLINE)||(g_mac_node_status == OFFLINE))&& g_mac_leaving != MAC_LEAVING)
				if(g_mac_leaving != MAC_LEAVING)
					{
					pk = op_pk_get(IN_NWK_MAC_DATA);
					pkt_num = op_subq_stat (Data_Queue, OPC_QSTAT_PKSIZE);
					if(pkt_num<=5)
						{
						seq = g_mac_sequence;
						g_mac_sequence = (g_mac_sequence+1)%256;
						op_pk_fd_get(pk,0, &framecontrol);
						op_pk_fd_get(pk,3, &dest);
						op_pk_fd_get(pk,7, &nwk_head);
						op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,seq,8);
						printf("Node %d MAC receive a frame from NWK dest is father node!  seq = %d  ;;;",g_mac_node_id,seq);
						subtype = (nwk_head>>12)&15;
						type = (framecontrol>>13)&7;
						printf("type = %d  ;;;",type);
						printf("subtype = %d  ;;;",subtype);
						printf("dest = %d\n",dest);
						if (op_subq_pk_insert(Data_Queue, pk, OPC_QPOS_TAIL) != OPC_QINS_OK)
							{
							printf("\n###Node %d:::data package from route insert into queue failed###\n", g_mac_node_id);
							op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d:::data package from route insert into queue failed###\n", g_mac_node_id);
							op_pk_destroy(pk);
							}
						op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d MAC receive a frame from NWK dest is father node!seq = %d,type = %d,subtype = %d,dest = %d!!!\n",g_mac_node_id,seq,type,subtype,dest);
						}
					else
						{
						printf("Node %d Data_queue is full\n", g_mac_node_id);
						}
					}
				}
				FSM_PROFILE_SECTION_OUT (state8_enter_exec)

			/** state (App_Msg_Up) exit executives **/
			FSM_STATE_EXIT_FORCED (8, "App_Msg_Up", "WSN_MAC_UP_NEW2 [App_Msg_Up exit execs]")


			/** state (App_Msg_Up) transition processing **/
			FSM_TRANSIT_FORCE (3, state3_enter_exec, ;, "default", "", "App_Msg_Up", "online", "tr_112", "WSN_MAC_UP_NEW2 [App_Msg_Up -> online : default / ]")
				/*---------------------------------------------------------*/



			/** state (MLME) enter executives **/
			FSM_STATE_ENTER_FORCED (9, "MLME", state9_enter_exec, "WSN_MAC_UP_NEW2 [MLME enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_UP_NEW2 [MLME enter execs]", state9_enter_exec)
				{
				/* recv app data, save in subqueue */
				//int dest;
				int type;
				Packet* pk;
				int band;
				int frequency;
				int a;
				if(C_STRM_NWK_MLME)
					{
					pk = op_pk_get(IN_NWK_MAC_MLME);
					op_pk_fd_get(pk,0,&type);
					switch(type)
						{
						case 5: //组播
							{
							printf("Node %d receive a MLME_MULTICAST.request!\n",g_mac_node_id);
							op_prg_log_entry_write(g_mac_up_info_log_handle,"Node %d receive a MLME_MULTICAST.request!\n",g_mac_node_id);			
							op_pk_fd_get(pk,1,&MulticastContent);
							break;
							}
						case MLME_SET_CHANNEL:
							{
							if(g_mac_node_type == BACKUP_NODE)
								{
								op_ima_obj_attr_get (g_mac_node_objid,	"g_node_type",	&g_mac_node_type);
								op_pk_fd_get(pk,0,&band);
								op_pk_fd_get(pk,1,&frequency);
								set_channel(band,frequency);		//设置频段频点
								break;
								}
							
							}
						case MLME_LEAVE_REQUEST:
							{
							a =op_pk_id(pk);
							printf("pk_id = %d ,time is %f \n",a,op_sim_time());
							printf("Node %d up pretend to leave!\n",g_mac_node_id);		
							op_prg_log_entry_write(g_mac_up_info_log_handle,"Node %d up pretend to leave!\n",g_mac_node_id);				
							g_mac_leaving = MAC_LEAVING;
							break;
							}
						default : break;
						}
					op_pk_destroy(pk);
					}
				}
				FSM_PROFILE_SECTION_OUT (state9_enter_exec)

			/** state (MLME) exit executives **/
			FSM_STATE_EXIT_FORCED (9, "MLME", "WSN_MAC_UP_NEW2 [MLME exit execs]")


			/** state (MLME) transition processing **/
			FSM_TRANSIT_FORCE (3, state3_enter_exec, ;, "default", "", "MLME", "online", "tr_109", "WSN_MAC_UP_NEW2 [MLME -> online : default / ]")
				/*---------------------------------------------------------*/



			/** state (cca_back_off) enter executives **/
			FSM_STATE_ENTER_UNFORCED (10, "cca_back_off", state10_enter_exec, "WSN_MAC_UP_NEW2 [cca_back_off enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_UP_NEW2 [cca_back_off enter execs]", state10_enter_exec)
				{
				int dest = -1;
				int type = -1;
				int id = -1;
				int subtype;
				Packet* pk;
				int framecontrol;
				int nwk_head;
				int seq;
				int band;
				int frequency;
				int pkt_num;
				int pkptr_size;
				Packet* pk2;
				
				if((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == 3))
					{
					pk2 = op_pk_get(3);
					op_pk_destroy(pk2);
					}
				
				g_mac_current_time = op_sim_time();
				
				//if(g_mac_node_id == 1) printf("Node %d  in 5 ,time is %f\n",g_mac_node_id,op_sim_time());
				
				
				//if(CH_FREE&&!BACKOFF_LIMIT_REACHED&&back_off_count == 0)
				if((op_stat_local_read (2)==0)&&!BACKOFF_LIMIT_REACHED&&back_off_count == 0)
					{
					//if(g_mac_node_id == 1)printf("free Node %d g_mac_pk_type = %d,current time = %f\n",g_mac_node_id,g_mac_pk_type,g_mac_current_time);
					pkptr_size = op_pk_total_size_get(pkptr);
					g_mac_sending_time = pkptr_size/g_mac_data_rate;
					//printf("free Node %d g_mac_current_time = %f,g_mac_sending_time time = %f,g_mac_backoff_duration = %f,g_mac_slot_start_time = %f ,result = %f\n",g_mac_node_id,g_mac_current_time,g_mac_sending_time,g_mac_backoff_duration,g_mac_slot_start_time,g_mac_current_time + g_mac_sending_time + g_mac_backoff_duration - g_mac_slot_start_time);
					csma_schedule_backoff1 ();
					//if(g_mac_node_id == 1)printf("free Node %d g_mac_current_time = %f,g_mac_sending_time time = %f,g_mac_backoff_duration = %f,g_mac_slot_start_time = %f ,result = %f\n",g_mac_node_id,g_mac_current_time,g_mac_sending_time,g_mac_backoff_duration,g_mac_slot_start_time,g_mac_current_time + g_mac_sending_time + g_mac_backoff_duration - g_mac_slot_start_time);
					if(g_mac_current_time + g_mac_sending_time + g_mac_backoff_duration - g_mac_slot_start_time > 0.498)
						{
						//printf("Node %d g_mac_slot_start_time at %f\n",g_mac_node_id,g_mac_slot_start_time);
						//printf("Node %d slot complete at %f\n",g_mac_node_id,g_mac_current_time);
						back_off_count = 0;
						slot_complete_flag = 1;
						op_intrpt_schedule_self(op_sim_time(), Slot_Completed_Code);
						if(g_mac_node_id == 1) printf("Node %d  back 7 ,time is %f\n",g_mac_node_id,op_sim_time());
						}
					else
						{
						
						printf("Node %d back off channel free at %f\n",g_mac_node_id,g_mac_current_time);
						back_off_count = 1;
						g_mac_backoff_exponent = macMinBE;
						csma_backoff ();
						}
					}
				
				
				//if(!CH_FREE&&!BACKOFF_LIMIT_REACHED)
				if((op_stat_local_read (2)>0)&&!BACKOFF_LIMIT_REACHED&&back_off_count == 0)
					{
					//if(g_mac_node_id == 1)printf("busy Node %d g_mac_pk_type = %d,current time = %f\n",g_mac_node_id,g_mac_pk_type,g_mac_current_time);
					pkptr_size = op_pk_total_size_get(pkptr);
					//if(g_mac_node_id == 1)printf("busy2 Node %d g_mac_pk_type = %d,current time = %f\n",g_mac_node_id,g_mac_pk_type,g_mac_current_time);
				
					g_mac_sending_time = pkptr_size/g_mac_data_rate;
					csma_schedule_backoff ();		
					//printf("busy Node %d g_mac_current_time = %f,g_mac_sending_time time = %f,g_mac_backoff_duration = %f,g_mac_slot_start_time = %f ,result = %f\n",g_mac_node_id,g_mac_current_time,g_mac_sending_time,g_mac_backoff_duration,g_mac_slot_start_time,g_mac_current_time + g_mac_sending_time + g_mac_backoff_duration - g_mac_slot_start_time);
					if(g_mac_current_time + g_mac_sending_time + g_mac_backoff_duration - g_mac_slot_start_time > 0.498)
						{ 
						//printf("Node %d g_mac_slot_start_time at %f\n",g_mac_node_id,g_mac_slot_start_time);
						//if(g_mac_node_id == 1)printf("Node %d slot complete at %f\n",g_mac_node_id,g_mac_current_time);
						back_off_count = 0;
						slot_complete_flag = 1;
						op_intrpt_schedule_self(op_sim_time(), Slot_Completed_Code);	
						//if(g_mac_node_id == 1) printf("Node %d  back 8 ,back_off_count = %d,time is %f\n",g_mac_node_id,back_off_count,op_sim_time());
						}
					else
						{
						printf("Node %d back off channel busy at %f\n",g_mac_node_id,g_mac_current_time);
						back_off_count = 0;		
						csma_backoff ();
						}
					}
				
				
				
				if(C_STRM_NWK_DATA)
					{
					if(g_mac_leaving != MAC_LEAVING)
						{
						pk = op_pk_get(IN_NWK_MAC_DATA);
						seq = g_mac_sequence;
						g_mac_sequence = (g_mac_sequence+1)%256;
						op_pk_fd_get(pk,0, &framecontrol);
						op_pk_fd_get(pk,3, &dest);
						op_pk_fd_get(pk,7, &nwk_head);
						op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,seq,8);
						printf("\t\tseq = %d  ;;;",seq);
						subtype = (nwk_head>>12)&15;
						type = (framecontrol>>13)&7;
						printf("type = %d  ;;;",type);
						printf("subtype = %d  ;;;",subtype);
						printf("dest = %d\n",dest);
						pkt_num = op_subq_stat (Data_Queue, OPC_QSTAT_PKSIZE);
						if(pkt_num <= 5)
							{
							if (op_subq_pk_insert(Data_Queue, pk, OPC_QPOS_TAIL) != OPC_QINS_OK)
								{
								printf("\n###Node %d:::data package from route insert into queue failed###\n", g_mac_node_id);
								op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d:::data package from route insert into queue failed###\n", g_mac_node_id);
								op_pk_destroy(pk);
								}
							op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d MAC receive a frame from NWK dest is father node!seq = %d,type = %d,subtype = %d,dest = %d!!!\n",g_mac_node_id,seq,type,subtype,dest);
							}
						else
							{
							printf("Node %d Data_queue is full\n", g_mac_node_id);
							}
						}
					}
				
				
				if(C_STRM_NWK_MLME)
					{
					pk = op_pk_get(IN_NWK_MAC_MLME);
					op_pk_fd_get(pk,0,&type);
					switch(type)
						{
						case 5: 
							{
							printf("Node %d receive a MLME_MULTICAST.request!\n",g_mac_node_id);
							op_prg_log_entry_write(g_mac_up_info_log_handle,"Node %d receive a MLME_MULTICAST.request!\n",g_mac_node_id);			
							op_pk_fd_get(pk,1,&MulticastContent);
							break;
							}
						case MLME_SET_CHANNEL:
							{
							if(g_mac_node_type == BACKUP_NODE)
								{
								op_ima_obj_attr_get (g_mac_node_objid,	"g_node_type",	&g_mac_node_type);
								op_pk_fd_get(pk,0,&band);
								op_pk_fd_get(pk,1,&frequency);
								set_channel(band,frequency);		//设置频段频点
								break;
								}
							
							}
						case MLME_LEAVE_REQUEST:
							{
							printf("Node %d up pretend to leave!\n",g_mac_node_id);		
							op_prg_log_entry_write(g_mac_up_info_log_handle,"Node %d up pretend to leave!\n",g_mac_node_id);				
							g_mac_leaving = MAC_LEAVING;
							break;
							}
						default : break;
						}
					op_pk_destroy(pk);
					}
				
				
				
				/*
				printf("Node %d g_mac_pk_type = %d,current time = %f\n",g_mac_node_id,g_mac_pk_type,g_mac_current_time);
				pkptr_size = op_pk_total_size_get(pkptr);
				printf("1111Node %d g_mac_pk_type = %d,current time = %f\n",g_mac_node_id,g_mac_pk_type,g_mac_current_time);
				
				g_mac_sending_time = 0.002+pkptr_size/g_mac_data_rate;
				//printf("g_mac_sending_time = %f\n",g_mac_sending_time);
				printf("Node %d g_mac_current_time = %f,g_mac_sending_time time = %f,g_mac_backoff_duration = %f,g_mac_slot_start_time = %f ,result = %f\n",g_mac_node_id,g_mac_current_time,g_mac_sending_time,g_mac_backoff_duration,g_mac_slot_start_time,g_mac_current_time + g_mac_sending_time + g_mac_backoff_duration - g_mac_slot_start_time);
				if(g_mac_current_time + g_mac_sending_time + g_mac_backoff_duration - g_mac_slot_start_time > 0.480)
					{
					printf("1111Node %d g_mac_current_time = %f,g_mac_sending_time time = %f,g_mac_backoff_duration = %f,g_mac_slot_start_time = %f ,result = %f\n",g_mac_node_id,g_mac_current_time,g_mac_sending_time,g_mac_backoff_duration,g_mac_slot_start_time,g_mac_current_time + g_mac_sending_time + g_mac_backoff_duration - g_mac_slot_start_time);
				
					printf("Node %d g_mac_slot_start_time at %f\n",g_mac_node_id,g_mac_slot_start_time);
					printf("Node %d slot complete at %f\n",g_mac_node_id,g_mac_current_time);
					back_off_count = 0;
					op_intrpt_schedule_self(op_sim_time(), Slot_Completed_Code);	
					}*/
				}
				FSM_PROFILE_SECTION_OUT (state10_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (21,"WSN_MAC_UP_NEW2")


			/** state (cca_back_off) exit executives **/
			FSM_STATE_EXIT_UNFORCED (10, "cca_back_off", "WSN_MAC_UP_NEW2 [cca_back_off exit execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_UP_NEW2 [cca_back_off exit execs]", state10_exit_exec)
				{
				//if(g_mac_node_id == 1) printf("Node %d  out 5 ,back_off_count = %d,ime is %f\n",g_mac_node_id,back_off_count,op_sim_time());
				}
				FSM_PROFILE_SECTION_OUT (state10_exit_exec)


			/** state (cca_back_off) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_MAC_UP_NEW2 [cca_back_off trans conditions]", state10_trans_conds)
			FSM_INIT_COND (CH_FREE&&!BACKOFF_LIMIT_REACHED&&back_off_count ==1&&slot_complete_flag == 2)
			FSM_TEST_COND (BACKOFF_LIMIT_REACHED||(C_SELF_SLOT_COMPLETED&&slot_complete_flag == 1))
			FSM_TEST_COND (!CH_FREE&&!BACKOFF_LIMIT_REACHED&&back_off_count ==1&&slot_complete_flag == 2)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("cca_back_off")
			FSM_PROFILE_SECTION_OUT (state10_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 11, state11_enter_exec, ;, "CH_FREE&&!BACKOFF_LIMIT_REACHED&&back_off_count ==1&&slot_complete_flag == 2", "", "cca_back_off", "transmit", "tr_122", "WSN_MAC_UP_NEW2 [cca_back_off -> transmit : CH_FREE&&!BACKOFF_LIMIT_REACHED&&back_off_count ==1&&slot_complete_flag == 2 / ]")
				FSM_CASE_TRANSIT (1, 3, state3_enter_exec, csma_init ();, "BACKOFF_LIMIT_REACHED||(C_SELF_SLOT_COMPLETED&&slot_complete_flag == 1)", "csma_init ()", "cca_back_off", "online", "tr_126", "WSN_MAC_UP_NEW2 [cca_back_off -> online : BACKOFF_LIMIT_REACHED||(C_SELF_SLOT_COMPLETED&&slot_complete_flag == 1) / csma_init ()]")
				FSM_CASE_TRANSIT (2, 10, state10_enter_exec, ;, "!CH_FREE&&!BACKOFF_LIMIT_REACHED&&back_off_count ==1&&slot_complete_flag == 2", "", "cca_back_off", "cca_back_off", "tr_127", "WSN_MAC_UP_NEW2 [cca_back_off -> cca_back_off : !CH_FREE&&!BACKOFF_LIMIT_REACHED&&back_off_count ==1&&slot_complete_flag == 2 / ]")
				FSM_CASE_TRANSIT (3, 10, state10_enter_exec, ;, "default", "", "cca_back_off", "cca_back_off", "tr_128", "WSN_MAC_UP_NEW2 [cca_back_off -> cca_back_off : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (transmit) enter executives **/
			FSM_STATE_ENTER_UNFORCED (11, "transmit", state11_enter_exec, "WSN_MAC_UP_NEW2 [transmit enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_UP_NEW2 [transmit enter execs]", state11_enter_exec)
				{
				int dest = -1;
				int type = -1;
				int id = -1;
				int subtype;
				Packet* pk;
				int framecontrol;
				int nwk_head;
				int seq;
				int band;
				int frequency;
				int pkt_num;
				//csma_send_packet ();
				int pkptr_size;
				Packet* pk2;
				
				if((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == 3))
					{
					pk2 = op_pk_get(3);
					op_pk_destroy(pk2);
					}
				
				
				//if(g_mac_node_id == 1) printf("g_mac_num_backoffs = %d time = %f\n",g_mac_num_backoffs,op_sim_time());
				//if(g_mac_node_id == 1) printf("Node %d  in 4 ,time is %f\n",g_mac_node_id,op_sim_time());
				
				g_mac_current_time = op_sim_time();
				
				
				if(C_SELF_SEND_COMPLETED&&CH_FREE)
					{
					printf("Node %d send complete ,current time = %f\n",g_mac_node_id,g_mac_current_time);
					get_msg_to_father();	
					while(g_mac_pk_type == DELEDTE_FRAME)
						{
						get_msg_to_father();
						}
					printf("1Node %d g_mac_pk_type= %d current time = %f\n",g_mac_node_id,g_mac_pk_type,g_mac_current_time);	
					if(g_mac_pk_type == DATA_FRAME||g_mac_pk_type == ACK_FRAME||g_mac_pk_type == MAC_CONTROL_FRAME||g_mac_pk_type == RETRANS_FRAME)
						{
						printf("Node %d get a packet g_mac_pk_type = %d,current time = %f\n",g_mac_node_id,g_mac_pk_type,g_mac_current_time);
						pkptr_size = op_pk_total_size_get(pkptr);
						printf("22Node %d get a packet g_mac_pk_type = %d,current time = %f\n",g_mac_node_id,g_mac_pk_type,g_mac_current_time);
						//printf("Node %d get size 11,current time = %f",g_mac_node_id,g_mac_current_time);
				
						g_mac_sending_time = pkptr_size/g_mac_data_rate;
						//printf("g_mac_sending_time = %f\n",g_mac_sending_time);
						if(g_mac_current_time+g_mac_sending_time-g_mac_slot_start_time < 0.498)
							{
							printf("Node %d send a packet g_mac_pk_type = %d,current time = %f\n",g_mac_node_id,g_mac_pk_type,g_mac_current_time);
							send_msg();
							//op_intrpt_schedule_self(op_sim_time()+0.04, 666);
							g_mac_backoff_exponent = macMinBE;
							}
						else
							{
							//if(g_mac_node_id == 1) printf("Node %d  back 1 ,time is %f\n",g_mac_node_id,op_sim_time());
							op_intrpt_schedule_self(op_sim_time(), Slot_Completed_Code);
							}
						}
					else
						{
						printf("Node %d  back 2 ,time is %f\n",g_mac_node_id,op_sim_time());
						op_intrpt_schedule_self(op_sim_time(), Slot_Completed_Code);
						}
					}
				
				if(C_SELF_SEND_COMPLETED&&!CH_FREE)
					{
					//if(g_mac_node_id == 1) printf("Node %d  back 9 ,time is %f\n",g_mac_node_id,op_sim_time());
					op_intrpt_schedule_self(op_sim_time(), Slot_Completed_Code);
					}
				
				
				//if(CH_FREE||C_SELF_SEND_COMPLETED)
				if(CH_FREE&&!BACKOFF_LIMIT_REACHED&&back_off_count ==1)
					{
					back_off_count =2;
					get_msg_to_father();
					while(g_mac_pk_type == DELEDTE_FRAME)
						{
						get_msg_to_father();
						}
					if(g_mac_pk_type == DATA_FRAME||g_mac_pk_type == ACK_FRAME||g_mac_pk_type == MAC_CONTROL_FRAME||g_mac_pk_type == RETRANS_FRAME)
						{
						pkptr_size = op_pk_total_size_get(pkptr);
						g_mac_sending_time = pkptr_size/g_mac_data_rate;
						//printf("g_mac_sending_time = %f\n",g_mac_sending_time);
						if(g_mac_current_time+g_mac_sending_time-g_mac_slot_start_time < 0.498)
							{
							printf("333Node %d send a packet g_mac_pk_type = %d,g_mac_sending_time = %f,current time = %f\n",g_mac_node_id,g_mac_pk_type,g_mac_sending_time,g_mac_current_time);
							send_msg();
							//op_intrpt_schedule_self(op_sim_time()+0.04, 666);
							g_mac_backoff_exponent = macMinBE;
							}
						else
							{
							//if(g_mac_node_id == 1) printf("Node %d  back 4 ,time is %f\n",g_mac_node_id,op_sim_time());
							op_intrpt_schedule_self(op_sim_time(), Slot_Completed_Code);
							}
						}
					else
						{
						//if(g_mac_node_id == 1) printf("Node %d  back 5 ,time is %f\n",g_mac_node_id,op_sim_time());
						op_intrpt_schedule_self(op_sim_time(), Slot_Completed_Code);
						}
					}
				
				
				/*if((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == 666))
					{
					csma_is_channel_free ();
					}*/
				
				
				if(C_STRM_NWK_DATA)
					{
					if(g_mac_leaving != MAC_LEAVING)
						{
						pk = op_pk_get(IN_NWK_MAC_DATA);
						seq = g_mac_sequence;
						g_mac_sequence = (g_mac_sequence+1)%256;
						op_pk_fd_get(pk,0, &framecontrol);
						op_pk_fd_get(pk,3, &dest);
						op_pk_fd_get(pk,7, &nwk_head);
						op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,seq,8);
						printf("\t\tseq = %d  ;;;",seq);
						subtype = (nwk_head>>12)&15;
						type = (framecontrol>>13)&7;
						printf("type = %d  ;;;",type);
						printf("subtype = %d  ;;;",subtype);
						printf("dest = %d\n",dest);
						pkt_num = op_subq_stat (Data_Queue, OPC_QSTAT_PKSIZE);
						if(pkt_num <= 5)
							{
							if (op_subq_pk_insert(Data_Queue, pk, OPC_QPOS_TAIL) != OPC_QINS_OK)
								{
								printf("\n###Node %d:::data package from route insert into queue failed###\n", g_mac_node_id);
								op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d:::data package from route insert into queue failed###\n", g_mac_node_id);
								op_pk_destroy(pk);
								}
							op_prg_log_entry_write(g_mac_up_debug_log_handle, "Node %d MAC receive a frame from NWK dest is father node!seq = %d,type = %d,subtype = %d,dest = %d!!!\n",g_mac_node_id,seq,type,subtype,dest);
							}
						else
							{
							printf("Node %d Data_queue is full\n", g_mac_node_id);
							}
						}
					}
				
				
				if(C_STRM_NWK_MLME)
					{
					pk = op_pk_get(IN_NWK_MAC_MLME);
					op_pk_fd_get(pk,0,&type);
					switch(type)
						{
						case 5: 
							{
							printf("Node %d receive a MLME_MULTICAST.request!\n",g_mac_node_id);
							op_prg_log_entry_write(g_mac_up_info_log_handle,"Node %d receive a MLME_MULTICAST.request!\n",g_mac_node_id);			
							op_pk_fd_get(pk,1,&MulticastContent);
							break;
							}
						case MLME_SET_CHANNEL:
							{
							if(g_mac_node_type == BACKUP_NODE)
								{
								op_ima_obj_attr_get (g_mac_node_objid,	"g_node_type",	&g_mac_node_type);
								op_pk_fd_get(pk,0,&band);
								op_pk_fd_get(pk,1,&frequency);
								set_channel(band,frequency);		//设置频段频点
								break;
								}
							
							}
						case MLME_LEAVE_REQUEST:
							{
							printf("Node %d up pretend to leave!\n",g_mac_node_id);		
							op_prg_log_entry_write(g_mac_up_info_log_handle,"Node %d up pretend to leave!\n",g_mac_node_id);				
							g_mac_leaving = MAC_LEAVING;
							break;
							}
						default : break;
						}
					op_pk_destroy(pk);
					}
				}
				FSM_PROFILE_SECTION_OUT (state11_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (23,"WSN_MAC_UP_NEW2")


			/** state (transmit) exit executives **/
			FSM_STATE_EXIT_UNFORCED (11, "transmit", "WSN_MAC_UP_NEW2 [transmit exit execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_UP_NEW2 [transmit exit execs]", state11_exit_exec)
				{
				//if(g_mac_node_id == 1) 	printf("op_intrpt_code = %d, out 4  at %f\n",op_intrpt_code(),op_sim_time());
				}
				FSM_PROFILE_SECTION_OUT (state11_exit_exec)


			/** state (transmit) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_MAC_UP_NEW2 [transmit trans conditions]", state11_trans_conds)
			FSM_INIT_COND (C_SELF_SLOT_COMPLETED)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("transmit")
			FSM_PROFILE_SECTION_OUT (state11_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 3, state3_enter_exec, slot_complete_flag = 1;, "C_SELF_SLOT_COMPLETED", "slot_complete_flag = 1", "transmit", "online", "tr_123", "WSN_MAC_UP_NEW2 [transmit -> online : C_SELF_SLOT_COMPLETED / slot_complete_flag = 1]")
				FSM_CASE_TRANSIT (1, 11, state11_enter_exec, ;, "default", "", "transmit", "transmit", "tr_129", "WSN_MAC_UP_NEW2 [transmit -> transmit : default / ]")
				}
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"WSN_MAC_UP_NEW2")
		}
	}




void
_op_WSN_MAC_UP_NEW2_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_WSN_MAC_UP_NEW2_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_WSN_MAC_UP_NEW2_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_WSN_MAC_UP_NEW2_svar function. */
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
#undef pk_type
#undef pk_size
#undef send_time
#undef sending_rate
#undef up_start_time
#undef can_send
#undef backoff_evhandle
#undef contr_send_num
#undef sendNum
#undef first_back
#undef mincost
#undef maxcost
#undef no
#undef no2
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
#undef g_mac_num_backoffs
#undef g_mac_slot_number
#undef g_mac_synch_flag
#undef g_mac_scan_flag
#undef g_mac_max_backoffs
#undef g_mac_backoff_exponent
#undef g_mac_unit_backoff_period
#undef g_mac_associate_target
#undef g_mac_recv_associate_response
#undef g_mac_associate_pan_id
#undef g_mac_associate_times
#undef g_mac_backoff_duration
#undef g_mac_up_debug_log_handle
#undef g_mac_up_info_log_handle
#undef g_mac_slot_start_time
#undef g_mac_current_time
#undef g_mac_leave_flag
#undef g_mac_data_rate
#undef g_mac_sending_time
#undef g_mac_rand
#undef g_mac_pk_type
#undef g_mac_sending_flag
#undef g_mac_leaving
#undef father_receiver
#undef channel_of_father_received
#undef father_received_comp
#undef receiveFrequency
#undef g_mac_receiving_flag
#undef pkt_success_stathandle
#undef g_pkt_success_stathandle
#undef packet_number_stathandle
#undef g_packet_number_stathandle
#undef up_packet_number
#undef up_receive_ack_number
#undef up_success_rate
#undef g_up_success_rate
#undef up_route_cost_stathandle
#undef g_up_route_cost_stathandle
#undef up_link_cost_stathandle
#undef g_up_link_cost_stathandle
#undef up_net_cost_stathandle
#undef g_up_net_cost_stathandle
#undef up_route_cost
#undef up_link_cost
#undef up_net_cost
#undef retrans_flag
#undef back_off_count
#undef slot_complete_flag
#undef CSMA_MODE
#undef frame_error_rate

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_WSN_MAC_UP_NEW2_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_WSN_MAC_UP_NEW2_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (WSN_MAC_UP_NEW2)",
		sizeof (WSN_MAC_UP_NEW2_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_WSN_MAC_UP_NEW2_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	WSN_MAC_UP_NEW2_state * ptr;
	FIN_MT (_op_WSN_MAC_UP_NEW2_alloc (obtype))

	ptr = (WSN_MAC_UP_NEW2_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "WSN_MAC_UP_NEW2 [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_WSN_MAC_UP_NEW2_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	WSN_MAC_UP_NEW2_state		*prs_ptr;

	FIN_MT (_op_WSN_MAC_UP_NEW2_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (WSN_MAC_UP_NEW2_state *)gen_ptr;

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
		*var_p_ptr = (void *) (&prs_ptr->neighbor_table);
		FOUT
		}
	if (strcmp ("father_table" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->father_table);
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
	if (strcmp ("pk_type" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pk_type);
		FOUT
		}
	if (strcmp ("pk_size" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pk_size);
		FOUT
		}
	if (strcmp ("send_time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->send_time);
		FOUT
		}
	if (strcmp ("sending_rate" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->sending_rate);
		FOUT
		}
	if (strcmp ("up_start_time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->up_start_time);
		FOUT
		}
	if (strcmp ("can_send" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->can_send);
		FOUT
		}
	if (strcmp ("backoff_evhandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->backoff_evhandle);
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
	if (strcmp ("first_back" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->first_back);
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
	if (strcmp ("keepAlive" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->keepAlive);
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
	if (strcmp ("g_mac_num_backoffs" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_num_backoffs);
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
	if (strcmp ("g_mac_scan_flag" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_scan_flag);
		FOUT
		}
	if (strcmp ("g_mac_max_backoffs" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_max_backoffs);
		FOUT
		}
	if (strcmp ("g_mac_backoff_exponent" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_backoff_exponent);
		FOUT
		}
	if (strcmp ("g_mac_unit_backoff_period" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_unit_backoff_period);
		FOUT
		}
	if (strcmp ("g_mac_associate_target" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_associate_target);
		FOUT
		}
	if (strcmp ("g_mac_recv_associate_response" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_recv_associate_response);
		FOUT
		}
	if (strcmp ("g_mac_associate_pan_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_associate_pan_id);
		FOUT
		}
	if (strcmp ("g_mac_associate_times" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_associate_times);
		FOUT
		}
	if (strcmp ("g_mac_backoff_duration" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_backoff_duration);
		FOUT
		}
	if (strcmp ("g_mac_up_debug_log_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_up_debug_log_handle);
		FOUT
		}
	if (strcmp ("g_mac_up_info_log_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_up_info_log_handle);
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
	if (strcmp ("g_mac_rand" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_rand);
		FOUT
		}
	if (strcmp ("g_mac_pk_type" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_pk_type);
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
	if (strcmp ("father_receiver" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->father_receiver);
		FOUT
		}
	if (strcmp ("channel_of_father_received" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->channel_of_father_received);
		FOUT
		}
	if (strcmp ("father_received_comp" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->father_received_comp);
		FOUT
		}
	if (strcmp ("receiveFrequency" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->receiveFrequency);
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
	if (strcmp ("up_packet_number" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->up_packet_number);
		FOUT
		}
	if (strcmp ("up_receive_ack_number" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->up_receive_ack_number);
		FOUT
		}
	if (strcmp ("up_success_rate" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->up_success_rate);
		FOUT
		}
	if (strcmp ("g_up_success_rate" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_up_success_rate);
		FOUT
		}
	if (strcmp ("up_route_cost_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->up_route_cost_stathandle);
		FOUT
		}
	if (strcmp ("g_up_route_cost_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_up_route_cost_stathandle);
		FOUT
		}
	if (strcmp ("up_link_cost_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->up_link_cost_stathandle);
		FOUT
		}
	if (strcmp ("g_up_link_cost_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_up_link_cost_stathandle);
		FOUT
		}
	if (strcmp ("up_net_cost_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->up_net_cost_stathandle);
		FOUT
		}
	if (strcmp ("g_up_net_cost_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_up_net_cost_stathandle);
		FOUT
		}
	if (strcmp ("up_route_cost" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->up_route_cost);
		FOUT
		}
	if (strcmp ("up_link_cost" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->up_link_cost);
		FOUT
		}
	if (strcmp ("up_net_cost" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->up_net_cost);
		FOUT
		}
	if (strcmp ("retrans_flag" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->retrans_flag);
		FOUT
		}
	if (strcmp ("back_off_count" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->back_off_count);
		FOUT
		}
	if (strcmp ("slot_complete_flag" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->slot_complete_flag);
		FOUT
		}
	if (strcmp ("CSMA_MODE" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->CSMA_MODE);
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


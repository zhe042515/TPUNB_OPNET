/* Process model C form file: WSN_MAC_DOWN_NEW.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char WSN_MAC_DOWN_NEW_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A modeler 7 62A6F6D7 62A6F6D7 1 DESKTOP-RD4S7T2 51133 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                    ";
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
#define		Wait_time				0.1			//等待间隔，多长时间未收到下行包开始发送上行数据
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
#define		ACTIVE_TIME				30*2*60		//子节点保活时间
#define		RETRANS_TIME			6			//重传等待时间
#define		ACK_TIME				2			//创建的ACK等待时间
#define		FATHER_NODE_NUM			6			//潜在父节点表数量
#define		CHILDREN_NODE_NUM		20			//子节点表数量
#define		BACKUP_NODE_NUM			10			//备份节点表数量



#define		MAX_SEQUENCE_NUM		6			//最大滑动窗格个数（0号值是第二个序列号的值）
#define		MAX_RETRANS_NUM			5			//最大重传队列长度
#define		MAX_ACK_NUM				10			//最大ACK队列长度

//长度为6，保存五个

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
#define		JOIN_REQ_FRAME			0			//入网请求帧
#define		ASSOCIATE_REQ_FRAME		0			//关联请求帧
#define		ASSOCIATE_RESP_FRAME	1			//关联回复帧
#define		KEEP_ALIVE_FRAME		2			//保活帧
#define		BACKUP_ASSOCIATE_FRAME	3			//备份节点关联帧

/*----------------------------原语类型------------------------*/
#define		MLME_SCAN_REQUEST		0			//信道扫描原语
#define		MLME_SCAN_confirm		0			//信道扫描回复原语
#define		MLME_ASSOCIATE_REQUEST	1			//信道关联原语
#define		MLME_ASSOCIATE_confirm	1			//信道关联回复原语
#define		MLME_ALIVE_REQUEST		2			//节点保活原语
#define		MLME_ALIVE_confirm		2			//节点保活回复原语
#define		MLME_UPDATE_NEIGHBOR_REQUEST	3	//节点更新子节点表原语
#define		MLME_CONFLICT_confirm	4			//节点地址冲突回复原语
#define		MLME_MULTICAST_confirm	5			//节点组播回复原语
#define		MLME_SET_CHANNEL		6			//设置信道频段频点原语（备份节点工作使用）
#define		MLME_LEAVE_REQUEST		7			//离网请求原语
#define		MLME_LEAVE_confirm		7			//离网确认原语
#define		MLME_BACKUP_seq_confirm	8			//路由备份序列号原语确认
#define		MLME_BACKUP_NODE_INFO	1			//路由备份序列号原语确认

#define		SENDING					1			//正在发送，不可接收
#define		RECEIVING				2			//没有发送，可以接收



#define		MAC_LEAVING				1			//MAC层正在离网
/*----------------------------收包标识赋值------------------------*/
#define		Receiced				1			//收到了包
#define		Nopacket				2			//未收到包
/*----------------------------扫描赋值------------------------*/
#define		NEED_SCAN				1			//需要扫描
#define		NO_SCAN					0			//不需要扫描
/*----------------------------同步赋值------------------------*/
#define		COMPLETE_SYNCH			1			//完成同步
#define		NEED_SYNCH				0			//需要同步
/*------------------------关联回复标识赋值--------------------*/
#define		RECV_ASSOCIATE_RESP		1			//收到关联回复
#define		NO_ASSOCIATE_RESP		0			//未收到关联回复
/*------------------------地址模式--------------------*/
#define		GATE_ADDRESS_MODE		1			//省略模式
#define		SHORT_ADDRESS_MODE		2			//16位短地址模式
#define		ESN_ADDRESS_MODE		3			//48位ESN地址模式


#define		BEACON_FRAME			0			//信标帧
#define		DATA_FRAME				1			//数据帧
#define		ACK_FRAME				2			//ACK确认帧
#define		MAC_CONTROL_FRAME		3			//MAC命令帧
#define		RETRANS_FRAME			4			//重传帧
#define		DELEDTE_FRAME			5			//此次检测没有帧需要被发送，但是需要再次检查
#define		NO_FRAME				6			//没有帧需要被发送，不需要再次检查



#define 	MAXTTL					2
#define 	MAX_NODE				200
#define 	MAX_ROUTER				48
#define 	MAX_CHILD				20
#define 	MAX_RouterAddress		128
#define 	GATEPOINTNUM			8
#define 	POINTNUM				2
#define 	MAX_MULTICAST			20
//-------------------------定义队列-----------------------//
#define		QUEUE_NUM			3		//队列数量

#define		Data_Queue			0		//应用层数据队列（上行）
#define		Retrans_Queue		1		//重传队列（上行）
#define		ACK_Queue			2		//ACK队列（上行）



/**------------------定义流索引-------------------**/


/**------------------Interupt Code-------------------**/
#define   	Start_Code					10000
#define   	Intrpt_Slot_Down         	20001	//全网帧中断码
#define   	UpTime_Code         		20080	//时隙中断码
#define   	Up_Code         			20100	//上行中断码
#define		Scan_Code					50000	//开启扫描中断
#define		ChangeBand_Code				60000	//切换扫描频段中断
#define		Send_Complete_Code			70000	//发送完毕中断
#define		Channel_Detect_Code			80002	//信道检测中断

#define		Associate_Code				70001	//开启关联中断



#define		Children_Active				5		//父节点收到子节点的保活消息




/**------------------定义状态转移条件-------------------**/
#define		Self_Start					((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Start_Code))
#define 	Join_Net					((op_intrpt_type() == OPC_INTRPT_REMOTE))

#define		C_STRM_NWK_DATA				((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == IN_NWK_MAC_DATA))
#define		C_STRM_NWK_MLME				((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == IN_NWK_MAC_MLME))
#define		C_STRM_MSG_FROM_CHILD_NODE	((op_intrpt_type() == OPC_INTRPT_STRM) && ((op_intrpt_strm() == IN_PHY_MAC)||(op_intrpt_strm() == 4)||(op_intrpt_strm() == 5)||(op_intrpt_strm() == 3)))
#define		C_SELF_NEW_SLOT				(((op_intrpt_type() == OPC_INTRPT_REMOTE) && (op_intrpt_code() == Intrpt_Slot_Down))||((op_intrpt_type() == OPC_INTRPT_SELF)&& (op_intrpt_code() == Intrpt_Slot_Down)))
#define		C_SELF_UP_TIME				((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Up_Code))
#define		C_SENDING_COMPLETE			((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Send_Complete_Code))
#define		C_SELF_CHANNEL_DETECT		((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Channel_Detect_Code))
#define		C_SELF_START_ASSOCIATE		((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == Associate_Code))


#define 	CH_FREE 					((csma_is_channel_free () == OPC_TRUE))


/*------------------ Input Index ------------------*/
//#define 	APP_MAC 					1		//来自上层发给下游节点的数据
//#define 	PHY_MAC0 					0		//来自下游节点的数据
//#define 	PHY_MAC2 					0		//来自下游节点的数据
//#define 	APP_MAC_DOWN 				3		//来自上层发给下游节点的数据
//#define 	MLME_FROM_NWK 				2		//网络层给MAC层的原语
#define 	IN_PHY_MAC 					0		//来自下游节点的数据
#define 	IN_NWK_MAC_DATA 			1		//来自上层发给下游节点的数据
#define 	IN_NWK_MAC_MLME 			2		//网络层给MAC层的原语


/* ------------------Output Index------------------ */
//#define 	MAC_PHY2 					2		//发给上游节点
//#define 	MAC_PHY0 					0		//发给下游节点
//#define 	MAC_NWK 					1		//发给网络层
//#define 	MAC_IO		 				2		//MAC层给网络层的原语
#define 	OUT_MAC_PHY 				0		//发给下游节点
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

struct backupNodeTable
	{
	int node_id;
	int short_addr;
	int major_node_address;
	int band;
	int frequency;
	int seq[MAX_SEQUENCE_NUM];	//0存放已经收到的序列号
	int active_time;
	int g_mac_seq;				//本节点到此子节点的最新序列号
	};

struct neighborTable
	{
	int node_id;
	int node_address;
	int band;
	int frequency;
	int seq[MAX_SEQUENCE_NUM];	//0存放已经收到的序列号
	int active_time;
	int g_mac_seq;				//本节点到此子节点的最新序列号
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
	int flag;				//0:正常保活；1：第三方保活
	};


struct childrenTable{
	int ESNAddress;
	int NWKAddress;
	int status;
	int CapabilityInformation;
	int JoinCount;
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
static void 	MLME_ASSOCIATE_CONFIRM(int result,int associatetype);
static void 	MLME_ALIVE_CONFIRM(int dest,int result,int flag);

static void 	MLME_LEAVE_CONFIRM(int result);
static void		make_beacon();
static void 	set_channel_down(int fatherBand,int frequency);
static void 	create_associate_response_frame(int dest,int result, int resptype);
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
static int 		ischild_node(int dest);
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



static void 	ack_frame_proc1(Packet* pk);
static void 	ack_proc1(int dest,int seq,int need_seq );
static void update_father_seq(int k);
static int update_sequence_from_fa_node(int source, int seq,int source_mode);
static int get_sequence_to_backup_node(int dest);
static int update_sequence_from_major_node(int source, int seq,int source_mode);
static void update_major_father_seq();
static void			create_sub_ack2(Packet* pk);
static void delete_backup_node(int source);

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
	int	                    		ack_time[MAX_ACK_NUM]                           ;	/* 上行模拟接收ACK时延 */
	struct Retrans	         		retrans[MAX_RETRANS_NUM]                        ;	/* 重传计时器 */
	struct neighborTable	   		neighbor_table[CHILDREN_NODE_NUM]               ;	/* 邻节点表，记录邻节点的序列号 */
	struct Potential_Parent			father_table[FATHER_NODE_NUM]                   ;	/* 潜在父节点表 */
	int	                    		pkt                                             ;	/* 测试发送包的个数 */
	Packet *	               		pkptr                                           ;	/* 用于存放给父节点的数据包 */
	int	                    		activeFrameSeq                                  ;	/* 保活帧的序列号 */
	struct KeepAliveFrame	  		keepAlive[20]                                   ;	/* 保活结构体 */
	int	                    		MulticastContent                                ;	/* 组播维护信息 */
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
	int	                    		g_mac_synch_flag                                ;	/* 同步完成标识符   1：完成同步   0：未完成 */
	int	                    		g_mac_no_packet_flag                            ;
	int	                    		g_mac_down_band                                 ;	/* 节点下行频段编号 */
	int	                    		g_mac_recv_frequency                            ;	/* 节点接收的频点号 */
	Log_Handle	             		g_mac_down_debug_log_handle                     ;
	Log_Handle	             		g_mac_down_info_log_handle                      ;
	double	                 		g_mac_slot_start_time                           ;	/* 时隙开始的时间 */
	double	                 		g_mac_current_time                              ;	/* 当前时间 */
	int	                    		g_mac_leave_flag                                ;	/* 离网状态 */
	double	                 		g_mac_data_rate                                 ;	/* 发送速率 */
	double	                 		g_mac_sending_time                              ;	/* 发送时间 */
	int	                    		g_mac_nwk_synch_seq                             ;	/* 网络层同步序列号 */
	int	                    		g_mac_sending_flag                              ;
	int	                    		g_mac_leaving                                   ;
	int	                    		g_mac_receiving_flag                            ;	/* 发送状态标识符   1：正在发送，2：没有发送 */
	Stathandle	             		pkt_success_stathandle                          ;	/* 节点本地统计丢包率 */
	Stathandle	             		g_pkt_success_stathandle                        ;	/* 全网统计丢包率 */
	Stathandle	             		route_cost_stathandle                           ;	/* 节点统计本地路由开销 */
	Stathandle	             		g_route_cost_stathandle                         ;	/* 全网路由开销 */
	Stathandle	             		link_cost_stathandle                            ;	/* 节点统计链路开销 */
	Stathandle	             		g_link_cost_stathandle                          ;	/* 全网链路开销 */
	Stathandle	             		net_cost_stathandle                             ;	/* 节点统计网络开销 */
	Stathandle	             		g_net_cost_stathandle                           ;	/* 全网网络开销 */
	Stathandle	             		all_cost_stathandle                             ;	/* 本节点所有包的总开销 */
	Stathandle	             		g_all_cost_stathandle                           ;	/* 全网总开销 */
	Stathandle	             		packet_number_stathandle                        ;	/* 本节点发送包的个数（需要ACK） */
	Stathandle	             		g_packet_number_stathandle                      ;	/* 全网发送包的个数（需要ACK） */
	int	                    		down_packet_number                              ;
	int	                    		down_receive_ack_number                         ;
	double	                 		down_route_cost                                 ;
	double	                 		down_link_cost                                  ;
	double	                 		down_net_cost                                   ;
	Stathandle	             		down_success_rate                               ;
	Stathandle	             		g_down_success_rate                             ;
	double	                 		frame_error_rate                                ;
	struct backupNodeTable	 		g_mac_backupNodeTable[BACKUP_NODE_NUM]          ;	/* 备份节点表 */
	int	                    		g_mac_backup_status                             ;
	struct neighborTable	   		backup_father_table                             ;
	double	                 		g_mac_rand                                      ;
	int	                    		g_mac_pk_type                                   ;
	int	                    		g_mac_associate_result                          ;	/* 1:关联成功 2；关联失败 */
	int	                    		g_mac_associate_times                           ;	/* 关联次数 */
	int	                    		g_mac_associate_target                          ;
	int	                    		g_mac_associate_pan_id                          ;
	} WSN_MAC_DOWN_NEW_state;

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
#define g_mac_backupNodeTable   		op_sv_ptr->g_mac_backupNodeTable
#define g_mac_backup_status     		op_sv_ptr->g_mac_backup_status
#define backup_father_table     		op_sv_ptr->backup_father_table
#define g_mac_rand              		op_sv_ptr->g_mac_rand
#define g_mac_pk_type           		op_sv_ptr->g_mac_pk_type
#define g_mac_associate_result  		op_sv_ptr->g_mac_associate_result
#define g_mac_associate_times   		op_sv_ptr->g_mac_associate_times
#define g_mac_associate_target  		op_sv_ptr->g_mac_associate_target
#define g_mac_associate_pan_id  		op_sv_ptr->g_mac_associate_pan_id

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	WSN_MAC_DOWN_NEW_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((WSN_MAC_DOWN_NEW_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


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
	double				a;
	double				b;
	PrgT_Random_Gen    	*my_rng;
	FIN(mac_init());
	new_seed = g_mac_node_id;
	frame	= 0;
	g_mac_slot_number	= 0;
	g_mac_node_status = OFFLINE;										//节点开机时不在网	
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
	//邻节点序列号表初始化------------------------------
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
		keepAlive[i].flag = 0;
		}
	//队列初始化-----------------------------------------
	/* Clear SubQ */
	op_subq_flush(Data_Queue);
	op_subq_flush(Retrans_Queue);
	op_subq_flush(ACK_Queue);
	//中心节点为同步基准节点，已同步---------------------
	if(g_mac_node_type == GATE_NODE)
		{										//中心节点，直接进入NET	
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
		g_mac_synch_flag = COMPLETE_SYNCH;                          //同步完成
		g_mac_node_status = ONLINE;									//网关节点直接在网
		g_mac_node_depth = 0;
		}
	
	if(g_mac_node_type != GATE_NODE)
		{
		g_mac_synch_flag = NEED_SYNCH;								//非网关节点
		g_mac_node_status = OFFLINE;								//现阶段节点开机就启动时隙，不需要同步步骤
		}
	//初始化MAC层序列号
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
static void MLME_ALIVE_CONFIRM(int dest,int result,int flag)
	{
	Packet* MLME_ALIVE;
	int i;
	int type;
	FIN(MLME_ALIVE_CONFIRM(int dest,int result,int flag));
	type = MLME_ALIVE_confirm;
	MLME_ALIVE = op_pk_create(0); //32 
	for(i = 0;i< CHILDREN_NODE_NUM; i++)
		{
		if(keepAlive[i].dest == dest)
			break;
		}
	op_pk_fd_set (MLME_ALIVE, 0, OPC_FIELD_TYPE_INTEGER,type,8) ;
	op_pk_fd_set (MLME_ALIVE, 1, OPC_FIELD_TYPE_INTEGER,dest,16) ;
	op_pk_fd_set (MLME_ALIVE, 2, OPC_FIELD_TYPE_INTEGER,result,8) ;
	op_pk_fd_set (MLME_ALIVE, 3, OPC_FIELD_TYPE_INTEGER,keepAlive[i].flag,8) ;
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


static void MLME_BACKUP_CONFIRM(int seq)
	{
	Packet* MLME_BACKUP;
	int type;
	FIN(MLME_BACKUP_CONFIRM(int seq));
	type = MLME_BACKUP_seq_confirm;
	MLME_BACKUP = op_pk_create(0);  //24
	op_pk_fd_set (MLME_BACKUP, 0, OPC_FIELD_TYPE_INTEGER,type,8) ;
	op_pk_fd_set (MLME_BACKUP, 1, OPC_FIELD_TYPE_INTEGER,seq,8) ;
	op_pk_send(MLME_BACKUP,OUT_MAC_NWK_MLME);	
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
		if(g_mac_node_status != OFFLINE) MLME_LEAVE_CONFIRM(2);//对下天线完成离网
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
	g_mac_leaving = 0;//完成离网	
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
//	if(g_mac_node_id == 19) printf("Node %d has sent a beacon at g_mac_slot_number%d at time%f!!!!!!!!!\n",g_mac_node_id,g_mac_slot_number,op_sim_time());
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
/*获取子节点的对应序列号序列号先增加后赋值*/
/*parameter  dest :子节点地址   
	mode标识dest是长地址还是短地址 
	2代表短地址 3代表长地址
	节点没有序列号就返回-1*/
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
//			printf("neighbor_table[i].node_address = %d\n",neighbor_table[i].node_address);	
			if(neighbor_table[i].node_address == next_hop(dest)||neighbor_table[i].node_address == dest)
				{
				neighbor_table[i].g_mac_seq = (neighbor_table[i].g_mac_seq+1)%256;
//				printf("i = %d,dest = %d,seq = %d\n",i,dest,neighbor_table[i].g_mac_seq);
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
/*更新子节点的最新序列号收发状态*/
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
//		printf("neighbor_table[n].seq[%d] = %d\n",i,neighbor_table[n].seq[1]);
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
/*更新子节点source的最新序列号
source：子节点地址
mode：子节点地址类型  2短地址  3长地址
AR：是否需要ACK回复
返回值  1：需要处理包回复ACK，需要交给网络层  
		2：需要处理包回复ACK，不需要交给网络层  
		3；直接删除*/
/*********************************************/
/*********************************************/
static int update_sequence_from_node(int source, int seq, int AR,int source_mode)
	{
	int n;
	int diff;
	int flag;
	int i;
	FIN(update_sequence_from_node(int source, int seq, int AR,int source_mode));
	flag = 0;
	if(source_mode == SHORT_ADDRESS_MODE)
		{
		for(i = 0;i<BACKUP_NODE_NUM;i++)
			{
			if(g_mac_backupNodeTable[i].short_addr == source)
				{
				FRET(5);
				}
			}
		for(n = 0;n < CHILDREN_NODE_NUM;n++)
			{
			if(neighbor_table[n].node_address == next_hop(source)||neighbor_table[n].node_address == source)
				{
				flag = 1;				//在子节点表中找到了对应节点			
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
				flag = 1;				//在子节点表中找到了对应节点			
				break;
				}
			}
		//如果是新的子节点入网
		if(flag == 0)
			{
			flag = 2;
			}
		}
	else
		{		
		flag = 0;
		}
	
	if(flag == 0)//包的源地址不在子节点表里(非关联请求)
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
	else if(flag == 1)							//包的源地址在子节点表里
		{
		diff = seq - neighbor_table[n].seq[0];
		printf("Node %d receive a packet ,neighbor_table[%d].seq[0] = %d,seq = %d diff = %d\n",g_mac_node_id,n,neighbor_table[n].seq[0],seq,diff);
		if(diff < -250) diff = seq+256-neighbor_table[n].seq[0];	//超过255
		//if(diff <= 0&&diff>(-5))									//已经接收过的帧
		if((diff <= 0&&diff>(-5))||diff>=250)
			{
			printf("Node %d receive an packet from down seq is old!!!\n",g_mac_node_id);
			op_prg_log_entry_write(g_mac_down_debug_log_handle, "Node %d receive an packet from down seq is old!!!\n",g_mac_node_id);
			FRET(2);
			}
		else if(diff<(MAX_SEQUENCE_NUM-1)&&diff>0)		//正常帧
			{
			//printf("111diff = %d,neighbor_table[n].seq[diff] = %d\n",diff,neighbor_table[n].seq[diff]);
			if(neighbor_table[n].seq[diff] == 0) 		//帧没有被接收过
				{
				neighbor_table[n].seq[diff] = 1;		//序列号为seq[0]+diff 的包收到了
			//	printf("222diff = %d,neighbor_table[n].seq[diff] = %d\n",diff,neighbor_table[n].seq[diff]);
				update_child_seq(n);
				FRET(1);
				}
			FRET(2);									//帧被接受过了
			}
		else if(diff>=6&&diff<250)											//异常帧序号
			{
			printf("Node %d receive an packet from down seq is over!!!\n",g_mac_node_id);
			op_prg_log_entry_write(g_mac_down_info_log_handle, "Node %d receive an packet from down seq is over!!!\n",g_mac_node_id);
			if(source_mode == SHORT_ADDRESS_MODE) 
				MLME_CONFLICT_CONFIRM(next_hop(source));	//报告短地址冲突
			else if(source_mode == ESN_ADDRESS_MODE)
				MLME_CONFLICT_CONFIRM(source);	//报告短地址冲突
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
/*创建关联回复帧,并将帧放入发送队列队列头*/
//resptype  1: 正常   2：备份节点第三方    3:备份节点替换主节点从新关联时
/*********************************************/
/*********************************************/
static void create_associate_response_frame(int dest,int result, int resptype)
	{
	int framecontrol;
	int seq;
	int pan;
//	int dest_addr;
	int source;
	int length;
	int subcontrol;
	int type;
	Packet* associateResponse;
	FIN(create_associate_response_frame(int dest,int result, int resptype));
	associateResponse = op_pk_create(0);//120
	framecontrol = 25138;//011 0001000 11 00 10	
	//seq = g_mac_sequence;
	//g_mac_sequence = (g_mac_sequence+1)%256;
	if(resptype == 1)
		{
		framecontrol = 25138;//011 0001000 11 00 10
		seq = get_sequence_to_node(dest,ESN_ADDRESS_MODE);
		op_pk_fd_set (associateResponse, 3, OPC_FIELD_TYPE_INTEGER,dest,48) ;
		op_pk_fd_set (associateResponse, 9, OPC_FIELD_TYPE_INTEGER,1,2) ;
		}
	else if(resptype == 2)
		{
		framecontrol = 25122;//011 0001000 11 00 10
		seq = get_sequence_to_backup_node(dest);
		op_pk_fd_set (associateResponse, 3, OPC_FIELD_TYPE_INTEGER,dest,16) ;
		op_pk_fd_set (associateResponse, 9, OPC_FIELD_TYPE_INTEGER,1,2) ;
		}
	else if(resptype == 3)
		{
		framecontrol = 25122;//011 0001000 10 00 10
		seq = get_sequence_to_node(dest,SHORT_ADDRESS_MODE);
		op_pk_fd_set (associateResponse, 3, OPC_FIELD_TYPE_INTEGER,dest,16) ;
		op_pk_fd_set (associateResponse, 9, OPC_FIELD_TYPE_INTEGER,2,2) ;
		}
	pan = g_mac_pan_id;
	//dest_addr = dest;
	source = g_mac_short_address;
	length = 3;
	subcontrol = 24;	
	op_pk_fd_set (associateResponse, 0, OPC_FIELD_TYPE_INTEGER,framecontrol,16) ;
	op_pk_fd_set (associateResponse, 1, OPC_FIELD_TYPE_INTEGER,seq,8) ;
	op_pk_fd_set (associateResponse, 2, OPC_FIELD_TYPE_INTEGER,pan,16) ;
	op_pk_fd_set (associateResponse, 4, OPC_FIELD_TYPE_INTEGER,source,16) ;
	op_pk_fd_set (associateResponse, 5, OPC_FIELD_TYPE_INTEGER,length,8) ;
	op_pk_fd_set (associateResponse, 7, OPC_FIELD_TYPE_INTEGER,subcontrol,8) ;
	op_pk_fd_set (associateResponse, 8, OPC_FIELD_TYPE_INTEGER,result,8) ;
	op_pk_fd_get(associateResponse,0,&framecontrol);
	type = (framecontrol>>13)&7;
	printf("Node %d create associate response type = %d ,seq = %d\n",g_mac_node_id,type,seq);
	op_prg_log_entry_write(g_mac_down_info_log_handle, "Node %d create associate response type = %d\n",g_mac_node_id,type);
	op_subq_pk_insert(Data_Queue,associateResponse,OPC_QPOS_TAIL);
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
	for(j = 1;j < MAX_ACK_NUM;j++)
		{
		ack_time[j-1] = ack_time[j];
		}
	ack_time[MAX_ACK_NUM-1] = 100;
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
/*重置保活结构体对应条目*/
/*********************************************/
/*********************************************/
static void delete_keep_alive(int dest)
	{
	int i;
	FIN(delete_keep_alive(int dest));
	for(i = 0;i<CHILDREN_NODE_NUM;i++)
		{
		if(keepAlive[i].dest == dest)//该节点已经在保活了
			{
			keepAlive[i].dest = 0;
			keepAlive[i].seq = 0;
			keepAlive[i].flag = 0;
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
			if(keepAlive[k].dest == dest)//该节点已经在保活了
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
/*删除邻接点表项对应条目*/
/*********************************************/
/*********************************************/
static void delete_neighbor_node(int address ,int dest_mode)
	{
	int i;
	FIN(delete_neighbor_node(int address ,int dest_mode));
	if(dest_mode == SHORT_ADDRESS_MODE)
		{
		printf("Node %d mac delete %d (SHORT_ADDRESS_MODE) \n",g_mac_node_id,address);
		for(i = 0;i<CHILDREN_NODE_NUM ;i++)
			{
			if(neighbor_table[i].node_address == address)
				{
				printf("i = %d,address = %d,yuan node id = %d",i,address,neighbor_table[i].node_id);
				break;
				}
			}
		}
	else if(dest_mode == ESN_ADDRESS_MODE)
		{
		printf("Node %d mac delete %d (ESN_ADDRESS_MODE) ",g_mac_node_id,address);
		for(i = 0;i<CHILDREN_NODE_NUM ;i++)
			{
			if(neighbor_table[i].node_id == address)
				break;
			}
		}
	neighbor_table[i].node_id = 0;
	neighbor_table[i].node_address = 0;
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
	double pk_create_time ;//原数据的创建时间
	
	FIN(send_msg_to_children());	
	isEmpty_ack = op_subq_empty (ACK_Queue);
	if((isEmpty_ack == OPC_FALSE)&&(ack_time[0]<=0))//ACK队列是否为空，非空先发送ACK包
		{		
		printf("\nNode %d send_ack at slot %d!!!time = %f\n",g_mac_node_id,g_mac_slot_number,op_sim_time());
		make_ack_frame();
		FOUT;
		}	
	else												//ACK队列为空，判断重传队列是否有包可以发送
		{
		isEmpty_retrans = op_subq_empty (Retrans_Queue);
		if((isEmpty_retrans == OPC_FALSE)&&(retrans[0].time <= 0))
			{
			if(retrans[0].count>=3)						//重传超过3次的帧处理
				{
				update_retrans_time(1);
				pk = op_subq_pk_remove (Retrans_Queue, 0);
				op_pk_fd_get(pk,0,&framecontrol);
				op_pk_fd_get(pk,3,&dest);
				op_pk_fd_get(pk,1,&seq);
				type = (framecontrol>>13)&7;
				dest_mode = (framecontrol>>4)&3;
				if(type == MAC_CONTROL_FRAME)			//保活帧重传超过三次
					{
					op_pk_fd_get(pk,1,&seq);
					op_pk_destroy(pk);
					MLME_ALIVE_CONFIRM(next_hop(dest),1,0);			
					if(dest_mode == ESN_ADDRESS_MODE) 
						{
						delete_keep_alive(dest);				//删除保活结构体条目
						delete_neighbor_node(dest,dest_mode);	//删除邻接点表条目		
						}
					else if(dest_mode == SHORT_ADDRESS_MODE) 
						{
						delete_keep_alive(next_hop(dest));		//删除保活结构体条目
						delete_neighbor_node(next_hop(dest),dest_mode);	//删除邻接点表条目
						}			
					}
				else									//其他帧发送失败就开始创建保活帧
					{
					op_pk_destroy(pk);
					for(i = 0;i<CHILDREN_NODE_NUM;i++)
						{
						if(keepAlive[i].dest == next_hop(dest))	//该节点已经在保活了
							{
							op_intrpt_schedule_self(op_sim_time(), Send_Complete_Code);
							FOUT;
							}
						}
					if(dest_mode == SHORT_ADDRESS_MODE) 
						{
						if(g_mac_node_id == 1) printf("dest = %d,next_hop(dest) = %d\n",dest,next_hop(dest));
						create_actice_frame(next_hop(dest),seq);//创建保活帧
						}
					else if(dest_mode == ESN_ADDRESS_MODE) 
						{
						if(g_mac_node_id == 1) printf("ESN_ADDRESS_MODEdest = %d\n",dest);
						create_actice_frame(dest,-20);
						}
					}
				op_intrpt_schedule_self(op_sim_time(), Send_Complete_Code);
				FOUT;
				}
			pk = op_subq_pk_remove (Retrans_Queue, 0);
			count = retrans[0].count;				
			op_pk_fd_get(pk,1,&seq);
			op_pk_fd_get(pk,3,&dest);
			printf("Node %d send a retrans packet at g_mac_slot_number%d !dest = %d,seq = %d,retrans_time = %d,time = %f\n",g_mac_node_id,g_mac_slot_number,dest,seq,count,op_sim_time());	
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
		else												//重传队列为空，发送普通数据包
			{
			num_pkts = op_subq_stat (Retrans_Queue, OPC_QSTAT_PKSIZE);
			if(num_pkts>=MAX_RETRANS_NUM)
				{
				op_intrpt_schedule_self(op_sim_time(), Send_Complete_Code);
				FOUT;
				}
			isEmpty_data = op_subq_empty (Data_Queue);			
			if(isEmpty_data ==  OPC_FALSE)					//发送数据队列非空
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
				{											//发送队列为空，本时隙没有帧需要发送
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
				printf("Node %d 下行待确认队列包数是  %d\n",g_mac_node_id,num_pkts);
				op_prg_log_entry_write(g_mac_down_debug_log_handle,"Node %d 下行待确认队列包数是  %d\n",g_mac_node_id,num_pkts);
				}
			}
		else
			{
			op_pk_destroy(copy);
			}
		FOUT;
		}
	}


//接收到从子节点传来的数据帧，产生ACK片段
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
	if(((framecontrol>>13)==DATA_FRAME)&&(NWKcontrol>>12)==13)//是给备份节点的包
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


//发送ACK帧时将ACK片段聚合成为一个完整的ACK聚合帧
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
		framecontrol =  18977;//0100 1010 0010 0001短板目的地址，无源地址
		}
	else
		{
		framecontrol =  18978;//0100 1010 0010 0010短板目的地址，短源地址
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
				sending_delay(pk_size);
				globle_down_link_cost = globle_down_link_cost+pk_size;
				down_link_cost = down_link_cost+pk_size;
				op_pk_send(ack_frame,OUT_MAC_PHY);
				FOUT;
				}
			i--;
			}
		else
			{				
			op_pk_fd_set(ack_frame,10,OPC_FIELD_TYPE_INTEGER,count,3);			
			pk_size = op_pk_total_size_get(ack_frame);
			sending_delay(pk_size);
			globle_down_link_cost = globle_down_link_cost+pk_size;
			down_link_cost = down_link_cost+pk_size;
			op_pk_send(ack_frame,OUT_MAC_PHY);
			FOUT;
			}
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

n 指子节点表序号
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
	if(((framecontrol>>13)==DATA_FRAME)&&(NWKcontrol>>12)==13)//是给备份节点的包
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
	if(n<20)
		{
		for(i = 1; i< MAX_SEQUENCE_NUM;i++)
			{
			if(neighbor_table[n].seq[i] == 0)
				{
				break;
				}
			}
		need_seq = (neighbor_table[n].seq[0]+i)%256;
		}
	else if(n = 66)
		{
		for(i = 1; i< MAX_SEQUENCE_NUM;i++)
			{
			if(backup_father_table.seq[i] == 0)
				{
				break;
				}
			}
		need_seq = (backup_father_table.seq[0]+i)%256;
		}
		
	op_pk_fd_set(ack,5,OPC_FIELD_TYPE_INTEGER,need_seq,8);//将需要的帧的序列号发过去
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
/*对MAC 命令帧的处理*/
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
	int		k;
	int 	IE;
	int		result;
	int		isChild;
	int 	associateFlag;
	int		source_mode;
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
		else if(subtype == ASSOCIATE_REQ_FRAME && g_mac_node_status == ONLINE)				//关联请求帧处理
			{
			source_mode = framecontrol&3;
			printf("Node %d receive an ASSOCIATE_REQ_FRAME frame at %f\t",g_mac_node_id,op_sim_time());
			op_prg_log_entry_write(g_mac_down_debug_log_handle,"Node %d receive an MAC ASSOCIATE_REQ_FRAME\n",g_mac_node_id);	
			op_pk_fd_get(pk,9,&associateFlag);
			printf("associateFlag = %d\n",associateFlag);
			if(source_mode == ESN_ADDRESS_MODE)
				{				
				for(i = 0;i<CHILDREN_NODE_NUM ;i++)
					{
					if(neighbor_table[i].node_id == 0&&neighbor_table[i].node_address == 0)
						{
						neighbor_table[i].node_id = source;
						neighbor_table[i].band = g_mac_down_band;
						neighbor_table[i].frequency = g_mac_recv_frequency;
						neighbor_table[i].seq[0] = seq;
						neighbor_table[i].g_mac_seq = (int)op_dist_uniform(255); //为此子节点初始化一个序列号
						for(j = 1;j<MAX_SEQUENCE_NUM;j++)
							{
							neighbor_table[i].seq[j] = 0;
							}
						neighbor_table[i].active_time = ACTIVE_TIME;
						if(associateFlag == 2) 
							{
							create_associate_response_frame(source,2,3);//关联成功
							}
						else 
							{
							create_associate_response_frame(source,2,1);//关联成功
							}
						break;
						}
					else
						{
						if( i == CHILDREN_NODE_NUM-1)
							{						
							if(associateFlag == 2)
								{
								create_associate_response_frame(source,1,3);//关联失败
								}
							else
								{
								create_associate_response_frame(source,1,1);//关联失败
								}
							}
						}
					}				
				}
			else if(source_mode == SHORT_ADDRESS_MODE)
				{
				isChild = ischild_node(source);
				printf("isChild = %d\n",isChild);
				for(i = 0;i<CHILDREN_NODE_NUM ;i++)
					{
					if(neighbor_table[i].node_address == source)
						{
						delete_backup_node(source);
						printf("Node %d add backup node neighbor_table[%d].node_address = %d yuan id = %d",g_mac_node_id,i,source,neighbor_table[i].node_id);	
						neighbor_table[i].band = g_mac_down_band;
						neighbor_table[i].frequency = g_mac_recv_frequency;
						neighbor_table[i].seq[0] = seq;
						neighbor_table[i].g_mac_seq = (int)op_dist_uniform(255); //为此子节点初始化一个序列号
						for(j = 1;j<MAX_SEQUENCE_NUM;j++)
							{
							neighbor_table[i].seq[j] = 0;
							}
						neighbor_table[i].active_time = ACTIVE_TIME;
						if(associateFlag == 2) 
							{
							create_associate_response_frame(source,2,3);//关联成功
							}
						else 
							{
							create_associate_response_frame(source,2,1);//关联成功
							}
						break;
						}
					}
				if(i == CHILDREN_NODE_NUM)
					{
					for(k = 0;k<CHILDREN_NODE_NUM;k++)
						{
						if(neighbor_table[k].node_address == 0 && isChild == 1)
							{
							delete_backup_node(source);
							neighbor_table[k].node_address = source;
							neighbor_table[k].band = g_mac_down_band;
							neighbor_table[k].frequency = g_mac_recv_frequency;
							neighbor_table[k].seq[0] = seq;
							neighbor_table[k].g_mac_seq = (int)op_dist_uniform(255); //为此子节点初始化一个序列号
							printf("new k = %d ,neighbor_table[k].node_address = %d,yuan id = %d\n",k,neighbor_table[k].node_address,neighbor_table[k].node_id);
							for(j = 1;j<MAX_SEQUENCE_NUM;j++)
								{
								neighbor_table[i].seq[j] = 0;
								}
							neighbor_table[i].active_time = ACTIVE_TIME;
							if(associateFlag == 2) 
								{
								create_associate_response_frame(source,2,3);//关联成功
								}
							else 
								{
								create_associate_response_frame(source,2,1);//关联成功
								}
							break;
							}
						else
							{
							if( k == CHILDREN_NODE_NUM-1)
								{	
								delete_backup_node(source);
								if(associateFlag == 2)
									{
									create_associate_response_frame(source,1,3);//关联失败
									}
								else
									{
									create_associate_response_frame(source,1,1);//关联失败
									}
								}
							}
						}
					}				
				}	
			}
		else if(subtype == BACKUP_ASSOCIATE_FRAME && g_mac_node_status == ONLINE)//备份节点的关联
			{
			printf("Node %d receive an BACKUP_ASSOCIATE_FRAME frame at %f\n",g_mac_node_id,op_sim_time());
			op_prg_log_entry_write(g_mac_down_debug_log_handle,"Node %d receive an MAC ASSOCIATE_REQ_FRAME\n",g_mac_node_id);
			for(i = 0;i<BACKUP_NODE_NUM ;i++)
				{
				if(g_mac_backupNodeTable[i].short_addr == 0)
					{
					printf("Node %d add backup node table number is %d\n",g_mac_node_id,i);
					//g_mac_backupNodeTable[i].node_id = source;
					g_mac_backupNodeTable[i].short_addr = source;
					g_mac_backupNodeTable[i].seq[0] = seq;
					g_mac_backupNodeTable[i].g_mac_seq = (int)op_dist_uniform(255); //为此子节点初始化一个序列号
					for(j = 1;j<BACKUP_NODE_NUM;j++)
						{
						g_mac_backupNodeTable[i].seq[j] = 0;
						}
					//g_mac_backupNodeTable[i].active_time = ACTIVE_TIME;
					create_associate_response_frame(source,2,2);//关联成功
					//printf("neighbor_table[%d].node_id = %d\n",i,neighbor_table[i].node_id);
					break;
					}
				else
					{
					if( i == BACKUP_NODE_NUM-1)
						{
						create_associate_response_frame(source,1,2);//关联失败
						}
					}
				}
			
			}
		else if(subtype == ASSOCIATE_RESP_FRAME && g_mac_node_type == BACKUP_NODE)
			{
			op_pk_fd_get(pk,3,&dest);
			op_pk_fd_get(pk,4,&source);
			if(dest == g_mac_short_address&&source == g_mac_associate_target)
				{
				printf("BACKUP NODE %d MAC receive an associate response frame at %f\n",g_mac_node_id,op_sim_time());
				g_mac_associate_result = 1;//收到回复
				g_mac_associate_times = 0;
				op_pk_fd_get(pk,8,&result);
				if(result == 2)
					//关联成功
					{
					printf("BACKUP NODE %d associate success\n",g_mac_node_id);
					op_subq_flush(Data_Queue);
					MLME_ASSOCIATE_CONFIRM(0,1);
					backup_father_table.node_address = g_mac_associate_target;
					backup_father_table.seq[0] = seq;
					}
				else
					//关联失败
					{
					printf("BACKUP NODE %d associate failed\n",g_mac_node_id);
					MLME_ASSOCIATE_CONFIRM(1,1);
					}				
				}
			}
		}
	op_pk_destroy(pk);
	FOUT;
	}


static void delete_backup_node(int source)
	{
	int i;
	int j;
	FIN(delete_backup_node(int source));
	for(i = 0;i<BACKUP_NODE_NUM;i++)
		{
		if(g_mac_backupNodeTable[i].short_addr == source)//曾经是备份节点
			{
			g_mac_backupNodeTable[i].short_addr = 0;
			g_mac_backupNodeTable[i].node_id = 0;
			g_mac_backupNodeTable[i].active_time = 0;
			g_mac_backupNodeTable[i].g_mac_seq = 0;
			for(j = 0;j<MAX_SEQUENCE_NUM;j++)
				{
				g_mac_backupNodeTable[i].seq[j] = 0;
				}
			break;
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
		if(source == keepAlive[i].dest)		//一跳节点保活
			{
			if(seq == keepAlive[i].seq)
				{
				//完成保活原语
				MLME_ALIVE_CONFIRM(source,0,keepAlive[i].flag);
				keepAlive[i].dest = 0;
				keepAlive[i].seq = 0;
				keepAlive[i].flag = 0;
				break;
				}
			}
		}	
	op_prg_log_entry_write(g_mac_down_debug_log_handle,"Node %d receive an ack packet and process it!!!\n",g_mac_node_id);
	ack_proc(seq,source,dest,need_seq);		
	op_pk_destroy(pk);
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
			//删除计数器****************************************
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
/*查询路由表*/
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


static int ischild_node(int dest)
	{
	int i;
	FIN(ischild_node(int dest));
/*	for(i = 0;i < BACKUP_NODE_NUM;i++)
		{
		printf("g_mac_backupNodeTable[i].short_addr = %d\n",g_mac_backupNodeTable[i].short_addr);
		if(g_mac_backupNodeTable[i].short_addr == dest)
			{
			g_mac_backupNodeTable[i].short_addr = 0;
			g_mac_backupNodeTable[i].seq[0] = 0;
			g_mac_backupNodeTable[i].g_mac_seq = 0; //为此子节点初始化一个序列号
			for(j = 1;j<BACKUP_NODE_NUM;j++)
				{
				g_mac_backupNodeTable[i].seq[j] = 0;
				}
			FRET(1);		
			}
		}
*/	
	//printf("Network_Msg.NetworkCT[g_mac_node_id].number = %d, dest = %d\n",Network_Msg.NetworkCT[g_mac_node_id].number, dest);
	for(i = 0;i < Network_Msg.NetworkCT[g_mac_node_id].number;i++)
		{
		//printf("Network_Msg.NetworkCT[g_mac_node_id].table[i].NWKAddress = %d\n",Network_Msg.NetworkCT[g_mac_node_id].table[i].NWKAddress);
		if(Network_Msg.NetworkCT[g_mac_node_id].table[i].NWKAddress == dest)
			{
			FRET(1);
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
//创建保活消息MAC命令帧
//保活命令帧在本时隙发送
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
	int		flag;
	FIN(create_actice_frame(int dest,int seq));
	flag = seq;
	printf("Node %d create_actice_frame dest is %d\n",g_mac_node_id,dest);
	op_prg_log_entry_write(g_mac_down_debug_log_handle,"Node %d create_actice_frame dest is %d\n",g_mac_node_id,dest);
	alive = op_pk_create(0);//88
	//seq = g_mac_sequence;
	//g_mac_sequence = (g_mac_sequence+1)%256;
	//if(seq == -1) seq = get_sequence_to_node(dest,SHORT_ADDRESS_MODE);
	if(flag == -20)
		{
		frame_control	= 26147;			//011 0011000 10 00 11
		seq = get_sequence_to_node(dest,ESN_ADDRESS_MODE);
		}
	else
		{
		frame_control	= 26146;			//011 0011000 10 00 10
		seq = get_sequence_to_node(dest,SHORT_ADDRESS_MODE);
		}
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
		if(keepAlive[i].dest == dest)//该节点已经在保活了
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
	if(flag == -10)
		{
		keepAlive[k].flag = 1;
		}
	else
		{
		keepAlive[k].flag = 0;
		}
	FOUT;
	}


/*********************************************/
/*********************************************/
/*Get IEs*/
/*********************************************/
/*********************************************/
static void get_IES(Packet* pk)
	{
	int type;
	int backup_seq;
	FIN(get_IES(Packet* pk));
	op_pk_fd_get(pk,60,&type);
	if(type == 0)
		{
		op_pk_fd_get(pk,6,&MulticastContent);
		MLME_MULTICAST_CONFIRM();
		}
	else if(type == 1)
		{
		op_pk_fd_get(pk,6,&backup_seq);
		MLME_BACKUP_CONFIRM(backup_seq);
		}
	FOUT;
	}


/*判断信道是否空闲*/
static int csma_is_channel_free ()
	{
	double rx_power = 0.0;
	int channel_status;
	
	FIN (csma_is_channel_free ());	
	rx_power = op_stat_local_read (0);	
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
		if(dest_mode == ESN_ADDRESS_MODE && dest == g_mac_node_id )//长地址,
			{
			ack_proc1(dest,seq,need_seq);
			}
		else if(dest_mode == SHORT_ADDRESS_MODE && dest == g_mac_short_address)//短地址
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
			printf("\tBACKUP NODE %dfind the packet!!!,seq = %d need_seq = %d\n",g_mac_node_id,seq,need_seq);
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


static void create_backup_associate_frame(int dest,int panid)
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
	FIN(create_backup_associate_frame(int dest,int panid));
	//判断队列中是否已经存在关联请求帧
	num_pkts = op_subq_stat (Data_Queue, OPC_QSTAT_PKSIZE);
	for(k=0;k<num_pkts;k++)
		{
		associateRequest = op_subq_pk_access (Data_Queue, k);
		op_pk_fd_get(associateRequest,3,&pk_dest);
		if(pk_dest==dest)
			{
			printf("BACKUP node %d Associate request packet is already in data queue!!!time is %f\n",g_mac_node_id,op_sim_time());
			FOUT;
			}		
		}
	framecontrol = 25122;//011 0001000 10 00 10
	type = (framecontrol>>13)&7;
	seq = g_mac_sequence;
	g_mac_sequence = (g_mac_sequence+1)%256;
	pan = panid;
	dest_addr = dest;
	source = g_mac_short_address;
	length = 3;
	subcontrol = (BACKUP_ASSOCIATE_FRAME<<4);
	associateRequest = op_pk_create(0);//120
	op_pk_fd_set (associateRequest, 0, OPC_FIELD_TYPE_INTEGER,framecontrol,16) ;
	op_pk_fd_set (associateRequest, 1, OPC_FIELD_TYPE_INTEGER,seq,8) ;
	op_pk_fd_set (associateRequest, 2, OPC_FIELD_TYPE_INTEGER,pan,16) ;
	op_pk_fd_set (associateRequest, 3, OPC_FIELD_TYPE_INTEGER,dest_addr,16) ;
	op_pk_fd_set (associateRequest, 4, OPC_FIELD_TYPE_INTEGER,source,48) ;
	op_pk_fd_set (associateRequest, 5, OPC_FIELD_TYPE_INTEGER,length,8) ;
	op_pk_fd_set (associateRequest, 7, OPC_FIELD_TYPE_INTEGER,subcontrol,8) ;
	printf("BACKUP node %d MAC create an associate frame at %f type = %d dest_addr = %d ,source = %d seq = %d \n",g_mac_node_id,op_sim_time(),type,dest_addr,source,seq);
	op_subq_pk_insert(Data_Queue,associateRequest,OPC_QPOS_TAIL);
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
	int		type;
	int		dest;
	int 	count=0;	
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
					op_pk_destroy(pkptr);
					g_mac_pk_type = DELEDTE_FRAME;
					FOUT;
					}
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
	op_pk_fd_get(pkptr,0,&framecontrol);
	AR = (framecontrol>>10)&1;
	op_pk_fd_get(pkptr,1,&seq);
	op_pk_fd_get(pkptr,3,&dest);
	//add_IES(pkptr);
	pk_create_time= op_pk_creation_time_get(pkptr);
	copy = op_pk_copy(pkptr);
	op_pk_creation_time_set(copy,pk_create_time);
	num_pkts = op_subq_stat (Retrans_Queue, OPC_QSTAT_PKSIZE);
	op_ima_obj_attr_get (channel_of_send_to_children,"min frequency",&freq);
	printf("BACKUP node %d send freq =%f,time is %f\n",g_mac_node_id,freq,op_sim_time());
	switch(g_mac_pk_type)
		{
		case DATA_FRAME:
			{
			if(num_pkts>=MAX_RETRANS_NUM) 
				{
				op_intrpt_schedule_self(op_sim_time(), Send_Complete_Code);
				FOUT;
				}
			printf("BACKUP node %d send a data packet!dest = %d,seq = %d\n",g_mac_node_id,dest,seq);	
			route_sink (pkptr);
			op_pk_send(pkptr, OUT_MAC_PHY);
			op_subq_pk_remove(Data_Queue, 0);
			count = 0;
			break;
			}
		case ACK_FRAME:
			{
			printf("BACKUP node %d send an ack packet!dest = %d,seq = %d\n",g_mac_node_id,dest,seq);
			//link_sink(pkptr);
			op_pk_send(pkptr, OUT_MAC_PHY);
			op_subq_pk_remove(ACK_Queue, 0);
			update_ack_time();
			op_intrpt_schedule_self(op_sim_time()+g_mac_sending_time, Send_Complete_Code);
			FOUT;
			}
		case MAC_CONTROL_FRAME:
			{
			if(num_pkts>=MAX_RETRANS_NUM) 
				{
				op_intrpt_schedule_self(op_sim_time(), Send_Complete_Code);
				FOUT;
				}
			printf("BACKUP node %d send a mac control packet!dest = %d,seq = %d\n",g_mac_node_id,dest,seq);
			//link_sink(pkptr);
			op_pk_send(pkptr, OUT_MAC_PHY);
			op_subq_pk_remove(Data_Queue, 0);
			count = 0;
			break;
			}
		case RETRANS_FRAME:
			{
			count = retrans[0].count;
			printf("BACKUP node %d send a retrans packet!dest = %d,seq = %d,retrans_time = %d\n",g_mac_node_id,dest,seq,count+1);
			route_sink (pkptr);
			//link_sink(pkptr);
			op_pk_send (pkptr, OUT_MAC_PHY);
			op_subq_pk_remove(Retrans_Queue, 0);
			update_retrans_time(1);
			break;
			}
		default:	break;		
		}
	printf("BACKUP node g_mac_sending_time = %f\n",g_mac_sending_time);
	op_intrpt_schedule_self(op_sim_time()+g_mac_sending_time, Send_Complete_Code);
	if(AR == 1)
		{
		op_subq_pk_insert(Retrans_Queue,copy,OPC_QPOS_TAIL);
		num_pkts = op_subq_stat (Retrans_Queue, OPC_QSTAT_PKSIZE);
		retrans[num_pkts-1].time = RETRANS_TIME;
		retrans[num_pkts-1].count = count+1;
		printf("BACKUP node %d 上行待确认队列包数是  %d\n",g_mac_node_id,num_pkts);
		}		
	else
		{
		op_pk_destroy(copy);
		}
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
	sendFreq = recvFreq+frequency*1.0;
	op_ima_obj_attr_set (channel_of_recvive_from_children,"min frequency",recvFreq);
	op_ima_obj_attr_set (channel_of_send_to_children,"min frequency",sendFreq);
	op_ima_obj_attr_get (channel_of_recvive_from_children,"min frequency",&a);
	op_ima_obj_attr_get (channel_of_send_to_children,"min frequency",&b);
	printf("BACKUP node %d channel_of_recvive_from_major father  = %f ;channel_of_send_to_major father = %f\n",g_mac_node_id,a,b);	
	FOUT;
	}


/*********************************************/
/*********************************************/
/*create MLME_ASSOCIATE.confirm,and send it to NWK*/
/*0:success    1:fail*/
/*associatetype 0:正常关联   1：备份节点关联 */
/*********************************************/
/*********************************************/
static void MLME_ASSOCIATE_CONFIRM(int result,int associatetype)
	{
	Packet* MLME_ASSOCIATE;
	int type;
	FIN(MLME_SCAN_CONFIRM());
	type = MLME_ASSOCIATE_confirm;
	MLME_ASSOCIATE = op_pk_create(0); //32 
	op_pk_fd_set (MLME_ASSOCIATE, 0, OPC_FIELD_TYPE_INTEGER,type,8) ;
	op_pk_fd_set (MLME_ASSOCIATE, 1, OPC_FIELD_TYPE_INTEGER,g_mac_associate_target,16) ;
	op_pk_fd_set (MLME_ASSOCIATE, 2, OPC_FIELD_TYPE_INTEGER,result,8) ;
	op_pk_fd_set (MLME_ASSOCIATE, 3, OPC_FIELD_TYPE_INTEGER,associatetype,8) ;
	op_pk_send(MLME_ASSOCIATE,OUT_MAC_NWK_MLME);	
	FOUT;
	}


static int get_sequence_to_backup_node(int dest)
	{
	int i;
	FIN(get_sequence_to_backup_node(int dest));
	for(i = 0;i<BACKUP_NODE_NUM ;i++)
		{
		if(g_mac_backupNodeTable[i].short_addr == dest)
			{
			g_mac_backupNodeTable[i].g_mac_seq = (g_mac_backupNodeTable[i].g_mac_seq+1)%256;
			FRET(g_mac_backupNodeTable[i].g_mac_seq);
			}
		}
	FRET(-1);
	}

/*********************************************/
/*********************************************/
/*更新子节点备份节点的最新序列号收发状态*/
/*********************************************/
/*********************************************/
static void update_father_seq(int k)
	{
	int i;
	int j;
	int count = 0;
	FIN(update_father_seq(int k));
	for(i = 1;i < MAX_SEQUENCE_NUM;i++)
		{
		if(g_mac_backupNodeTable[k].seq[1] == 0)
			{
			break;
			}
		else if(g_mac_backupNodeTable[k].seq[1] == 1)
			{
			count++;
			for(j = 1;j<MAX_SEQUENCE_NUM-1;j++)
				{
				g_mac_backupNodeTable[k].seq[j] = g_mac_backupNodeTable[k].seq[j+1];
				}
			g_mac_backupNodeTable[k].seq[MAX_SEQUENCE_NUM-1] = 0;
			}
		if(i==5)
			{
			g_mac_backupNodeTable[k].seq[0] = (g_mac_backupNodeTable[k].seq[0]+5)%256;
			g_mac_backupNodeTable[k].seq[1] = 1;
			for(j = 2;j<MAX_SEQUENCE_NUM;j++)
				{
				g_mac_backupNodeTable[k].seq[j] = 0;
				}
			FOUT;
			}
		}
	g_mac_backupNodeTable[k].seq[0] = (g_mac_backupNodeTable[k].seq[0]+count)%256;
	FOUT;
	}


/*********************************************/
/*********************************************/
/*更新子路由节点备份节点的最新序列号
mode：子节点地址类型  2短地址  3长地址
AR：是否需要ACK回复
返回值  1：需要处理包回复ACK，需要交给网络层  
		2：需要处理包回复ACK，不需要交给网络层  
		3；直接删除
*/
/*********************************************/
/*********************************************/
static int update_sequence_from_fa_node(int source, int seq,int source_mode)
	{
	int diff;
	int k = 100;
	int i;
	
	FIN(update_sequence_from_fa_node(int source, int seq,int source_mode));
	printf("BACKUP node %d update_sequence_from_fa_node!!!\n",g_mac_node_id);
	for(i = 0;i<BACKUP_NODE_NUM;i++)
		{
		if(g_mac_backupNodeTable[i].short_addr == source)
			{
			k = i;
			break;
			}
		}
	if(k<10)	
		{
		diff = seq - g_mac_backupNodeTable[k].seq[0];
		if(diff < -250) diff = seq+256-g_mac_backupNodeTable[k].seq[0];	//超过255
		printf("BACKUP nodediff = %d!!!\n",diff);
		//if(diff <= 0&&diff>(-5))									//已经接收过的帧
		if((diff <= 0&&diff>(-5))||diff>=250)
			{
			printf("BACKUP node %d receive an packet from up seq is old!!!\n",g_mac_node_id);
			FRET(2);
			}
		else if(diff<(MAX_SEQUENCE_NUM-1)&&diff>0)		//正常帧
			{
			if(g_mac_backupNodeTable[k].seq[diff] == 0) 		//帧没有被接收过
				{
				g_mac_backupNodeTable[k].seq[diff] = 1;		//序列号为seq[0]+diff 的包收到了
				update_father_seq(k);
				FRET(1);
				}
			FRET(2);									//帧被接受过了
			}
		else if(diff>6&&diff<250)											//异常帧序号
			{
			printf("BACKUP node %d receive an packet from up seq is over!!!\n",g_mac_node_id);
			MLME_CONFLICT_CONFIRM(g_mac_backupNodeTable[k].short_addr);	//报告短地址冲突
			FRET(3);
			}
		else
			{
			printf("\t\t\tBACKUP nodehuachuang == 5\n");
			if(g_mac_backupNodeTable[k].seq[2]+g_mac_backupNodeTable[k].seq[3]+g_mac_backupNodeTable[k].seq[4]>=2) 
				{
				g_mac_backupNodeTable[k].seq[1] = 1;	
				update_father_seq(k);
				FRET(1);
				}
			FRET(3);
			}
		}
	FRET(3);
	}











/*********************************************/
/*********************************************/
/*更新父节点的最新序列号收发状态*/
/*********************************************/
/*********************************************/
static void update_major_father_seq()
	{
	int i;
	int j;
	int count = 0;
	FIN(update_major_father_seq());
	for(i = 1;i < MAX_SEQUENCE_NUM;i++)
		{
		if(backup_father_table.seq[1] == 0)
			{
			break;
			}
		else if(backup_father_table.seq[1] == 1)
			{
			count++;
			for(j = 1;j<MAX_SEQUENCE_NUM-1;j++)
				{
				backup_father_table.seq[j] = backup_father_table.seq[j+1];
				}
			backup_father_table.seq[MAX_SEQUENCE_NUM-1] = 0;
			}
		if(i==5)
			{
			backup_father_table.seq[0] = (backup_father_table.seq[0]+5)%256;
			backup_father_table.seq[1] = 1;
			for(j = 2;j<MAX_SEQUENCE_NUM;j++)
				{
				backup_father_table.seq[j] = 0;
				}
			FOUT;
			}
		}
	backup_father_table.seq[0] = (backup_father_table.seq[0]+count)%256;
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
static int update_sequence_from_major_node(int source, int seq,int source_mode)
	{
	int diff;
	FIN(update_sequence_from_major_node(int source, int seq,int source_mode));
		{
		diff = seq - backup_father_table.seq[0];
		if(diff < -250) diff = seq+256-backup_father_table.seq[0];	//超过255
		
		//if(diff <= 0&&diff>(-5))									//已经接收过的帧
		if((diff <= 0&&diff>(-5))||diff>=250)
			{
			printf("BACKUP node %d receive an packet from up seq is old!!!\n",g_mac_node_id);
			FRET(2);
			}
		else if(diff<(MAX_SEQUENCE_NUM-1)&&diff>0)		//正常帧
			{
			if(backup_father_table.seq[diff] == 0) 		//帧没有被接收过
				{
				backup_father_table.seq[diff] = 1;		//序列号为seq[0]+diff 的包收到了
				update_major_father_seq();
				FRET(1);
				}
			FRET(2);									//帧被接受过了
			}
		else if(diff>6&&diff<250)											//异常帧序号
			{
			printf("BACKUP node %d receive an packet from up seq is over!!!\n",g_mac_node_id);
			MLME_CONFLICT_CONFIRM(backup_father_table.node_address);	//报告短地址冲突
			FRET(3);
			}
		else
			{
			printf("\t\t\tBACKUP nodehuachuang == 5\n");
			if(backup_father_table.seq[2]+backup_father_table.seq[3]+backup_father_table.seq[4]>=2) 
				{
				backup_father_table.seq[1] = 1;	
				update_major_father_seq();
				FRET(1);
				}
			FRET(3);
			}
		}		
	}




//接收到从备份节点传来的数据帧，产生ACK片段
static void			create_sub_ack2(Packet* pk)
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
	int n;
	int framecontrol;
	FIN(create_sub_ack2(Packet* pk));
	ack = op_pk_create(0);
	op_pk_fd_get(pk,0,&framecontrol);
	op_pk_fd_get(pk,1,&pk_seq);
	op_pk_fd_get(pk,2,&pk_panid);
	op_pk_fd_get(pk,3,&pk_dest);
	op_pk_fd_get(pk,4,&pk_source);
	op_pk_fd_get(pk,7,&NWKcontrol);
	source_mode = framecontrol&3;
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
	for(n = 0;n<BACKUP_NODE_NUM;n++)
		{
		if(g_mac_backupNodeTable[n].short_addr == pk_source)
			{
			break;
			}
		}
	need_seq = (g_mac_backupNodeTable[n].seq[0]+1)%256;		
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





static void set_channel_start_send ()
	{
	double a;
	double b;
	FIN(set_channel_start_send());
	if(g_mac_node_id == 3) 
		{
	//printf("Node %d start send at %f !\n",g_mac_node_id,op_sim_time());
	op_ima_obj_attr_get (channel_of_recvive_from_children,"min frequency",&a);
	op_ima_obj_attr_get (channel_of_send_to_children,"min frequency",&b);
	//printf("\t\t\tDDDDDDnode %d channel_of_recvive_from_major father  = %f ;channel_of_send_to_major father = %f\n",g_mac_node_id,a,b);	
	}
	op_strm_flush(IN_PHY_MAC);
	g_mac_receiving_flag = SENDING;
	FOUT;
	}
static void set_channel_start_receive ()
	{
	double a;
	double b;
	FIN(set_channel_start_receive());
	op_strm_flush(IN_PHY_MAC);
	if(g_mac_node_id == 3) 
		{
		//printf("Node %d start recv at %f !\n",g_mac_node_id,op_sim_time());
	op_ima_obj_attr_get (channel_of_recvive_from_children,"min frequency",&a);
	op_ima_obj_attr_get (channel_of_send_to_children,"min frequency",&b);
	//printf("\t\t\tDDDDDDnode %d channel_of_recvive_from_major father  = %f ;channel_of_send_to_major father = %f\n",g_mac_node_id,a,b);	
		}
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
	void WSN_MAC_DOWN_NEW (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_WSN_MAC_DOWN_NEW_init (int * init_block_ptr);
	void _op_WSN_MAC_DOWN_NEW_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_WSN_MAC_DOWN_NEW_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_WSN_MAC_DOWN_NEW_alloc (VosT_Obtype, int);
	void _op_WSN_MAC_DOWN_NEW_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
WSN_MAC_DOWN_NEW (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (WSN_MAC_DOWN_NEW ());

		{


		FSM_ENTER ("WSN_MAC_DOWN_NEW")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "init", "WSN_MAC_DOWN_NEW [init enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_DOWN_NEW [init enter execs]", state0_enter_exec)
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
				
				
				g_mac_process_objid = op_id_self();															//获取本节点的实体
				g_mac_node_objid    = op_topo_parent(g_mac_process_objid);
				//subnet_objid     = op_topo_parent (my_node_objid);			
				op_ima_obj_attr_get (g_mac_node_objid,	"g_node_ESN_address",	&g_mac_node_id);
				op_ima_obj_attr_get (g_mac_node_objid,	"g_node_type"	,	&g_mac_node_type);
				op_ima_obj_attr_get (g_mac_node_objid,	"g_node_status",	&g_mac_node_status);
				op_ima_obj_attr_get (g_mac_node_objid,	"g_node_error_rate",	&frame_error_rate);
				
				
				
				
				//printf("本节点的MAC地址是%d\n",g_mac_node_id);
				
				
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
			FSM_STATE_EXIT_FORCED (0, "init", "WSN_MAC_DOWN_NEW [init exit execs]")


			/** state (init) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "init", "idle", "tr_37", "WSN_MAC_DOWN_NEW [init -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "idle", state1_enter_exec, "WSN_MAC_DOWN_NEW [idle enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_DOWN_NEW [idle enter execs]", state1_enter_exec)
				{
				
				Packet* pk;
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
				if(C_SELF_CHANNEL_DETECT)
					{
					if(CH_FREE)		//信道空闲状态
						{
						op_intrpt_schedule_self(op_sim_time(), Up_Code);	//信道空闲直接开启上行
						}
					}
				
				if(g_mac_node_type == BACKUP_NODE&&g_mac_backup_status == 1&&C_SELF_START_ASSOCIATE)
					{
					if(g_mac_associate_result == 2&&g_mac_associate_times<4)
						{
						create_backup_associate_frame(g_mac_associate_target,g_mac_associate_pan_id);
						op_intrpt_schedule_self(op_sim_time()+6, Associate_Code);
						g_mac_associate_times++;
						}
					else if(g_mac_associate_times == 4)
						{
						MLME_ASSOCIATE_CONFIRM(1,1);
						g_mac_associate_times = 0;
						}
					}
				
				if(C_STRM_MSG_FROM_CHILD_NODE&&g_mac_node_type==BACKUP_NODE&&g_mac_backup_status != 1)
					{
					pk = op_pk_get(op_intrpt_strm());
					op_pk_destroy(pk);
					}
				}
				FSM_PROFILE_SECTION_OUT (state1_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"WSN_MAC_DOWN_NEW")


			/** state (idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "idle", "WSN_MAC_DOWN_NEW [idle exit execs]")


			/** state (idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_MAC_DOWN_NEW [idle trans conditions]", state1_trans_conds)
			FSM_INIT_COND (C_SELF_NEW_SLOT)
			FSM_TEST_COND (C_STRM_MSG_FROM_CHILD_NODE&&g_mac_node_type != BACKUP_NODE)
			FSM_TEST_COND (C_STRM_NWK_DATA)
			FSM_TEST_COND (C_STRM_NWK_MLME)
			FSM_TEST_COND (C_STRM_MSG_FROM_CHILD_NODE&&g_mac_node_type==BACKUP_NODE&&g_mac_backup_status == 1)
			FSM_TEST_COND (C_SELF_UP_TIME&&g_mac_node_type == BACKUP_NODE)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("idle")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 4, state4_enter_exec, set_channel_start_send ();, "C_SELF_NEW_SLOT", "set_channel_start_send ()", "idle", "slot_intrpt", "tr_24", "WSN_MAC_DOWN_NEW [idle -> slot_intrpt : C_SELF_NEW_SLOT / set_channel_start_send ()]")
				FSM_CASE_TRANSIT (1, 3, state3_enter_exec, ;, "C_STRM_MSG_FROM_CHILD_NODE&&g_mac_node_type != BACKUP_NODE", "", "idle", "msg from down", "tr_29", "WSN_MAC_DOWN_NEW [idle -> msg from down : C_STRM_MSG_FROM_CHILD_NODE&&g_mac_node_type != BACKUP_NODE / ]")
				FSM_CASE_TRANSIT (2, 2, state2_enter_exec, ;, "C_STRM_NWK_DATA", "", "idle", "App_Msg_Down", "tr_31", "WSN_MAC_DOWN_NEW [idle -> App_Msg_Down : C_STRM_NWK_DATA / ]")
				FSM_CASE_TRANSIT (3, 5, state5_enter_exec, ;, "C_STRM_NWK_MLME", "", "idle", "MLME", "tr_35", "WSN_MAC_DOWN_NEW [idle -> MLME : C_STRM_NWK_MLME / ]")
				FSM_CASE_TRANSIT (4, 6, state6_enter_exec, ;, "C_STRM_MSG_FROM_CHILD_NODE&&g_mac_node_type==BACKUP_NODE&&g_mac_backup_status == 1", "", "idle", "msg from up", "tr_62", "WSN_MAC_DOWN_NEW [idle -> msg from up : C_STRM_MSG_FROM_CHILD_NODE&&g_mac_node_type==BACKUP_NODE&&g_mac_backup_status == 1 / ]")
				FSM_CASE_TRANSIT (5, 7, state7_enter_exec, set_channel_start_send ();, "C_SELF_UP_TIME&&g_mac_node_type == BACKUP_NODE", "set_channel_start_send ()", "idle", "send_up", "tr_65", "WSN_MAC_DOWN_NEW [idle -> send_up : C_SELF_UP_TIME&&g_mac_node_type == BACKUP_NODE / set_channel_start_send ()]")
				FSM_CASE_TRANSIT (6, 1, state1_enter_exec, ;, "default", "", "idle", "idle", "tr_34", "WSN_MAC_DOWN_NEW [idle -> idle : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (App_Msg_Down) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "App_Msg_Down", state2_enter_exec, "WSN_MAC_DOWN_NEW [App_Msg_Down enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_DOWN_NEW [App_Msg_Down enter execs]", state2_enter_exec)
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
				op_ima_obj_attr_get (g_mac_node_objid,	"g_node_type"	,	&g_mac_node_type);
				op_ima_obj_attr_get (g_mac_node_objid,	"g_node_status",	&g_mac_node_status);
				if((g_mac_node_type != END_NODE)&&(g_mac_leaving != MAC_LEAVING)&&(g_mac_node_type != BACKUP_NODE))
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
					if(pkt_num <= 5&& ((next_hop(dest) != -1)||dest_mode == 3))
						{
						if(dest!=0xfffc&&dest!=0xfffd&&dest!=0xfffe&&dest!=0xffff)
							{
							seq = get_sequence_to_backup_node(dest);
					//		printf("Node %d get backup seq1 = %d\n",g_mac_node_id,seq);
							if(seq == -1)
								{
								seq = get_sequence_to_node(dest,dest_mode);
					//			printf("Node %d get backup seq2 = %d\n",g_mac_node_id,seq);
								}
							}
						else 		//广播帧和组播真的序列号为0
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
						op_pk_destroy(pk);
						}
					}
					
				if((g_mac_leaving != MAC_LEAVING)&&(g_mac_node_type == BACKUP_NODE))
					{
					pk = op_pk_get(IN_NWK_MAC_DATA);
					op_pk_fd_get(pk,0, &framecontrol);
					op_pk_fd_get(pk,3, &dest);
					op_pk_fd_get(pk,7, &nwk_head);
					//seq = g_mac_sequence;
					//g_mac_sequence = (g_mac_sequence+1)%256;
					dest_mode = (framecontrol>>4)&3;
					printf("DEST == %d,dest_mode = %d\n",dest,dest_mode);	
					pkt_num = op_subq_stat (Data_Queue, OPC_QSTAT_PKSIZE);
					if(pkt_num <= 5)
						{
						seq = g_mac_sequence;
						g_mac_sequence = (g_mac_sequence+1)%256;
						op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,seq,8);
						printf("BACKUP node %d MAC receive a frame from NWK dest is children node!  seq = %d  ;;;",g_mac_node_id,seq);
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
						printf("BACKUP node %d Data_queue is full\n", g_mac_node_id);
						}
					}
					
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (App_Msg_Down) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "App_Msg_Down", "WSN_MAC_DOWN_NEW [App_Msg_Down exit execs]")


			/** state (App_Msg_Down) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_MAC_DOWN_NEW [App_Msg_Down trans conditions]", state2_trans_conds)
			FSM_INIT_COND (g_mac_sending_flag == 2)
			FSM_TEST_COND (g_mac_sending_flag == 1)
			FSM_TEST_LOGIC ("App_Msg_Down")
			FSM_PROFILE_SECTION_OUT (state2_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, ;, "g_mac_sending_flag == 2", "", "App_Msg_Down", "idle", "tr_30", "WSN_MAC_DOWN_NEW [App_Msg_Down -> idle : g_mac_sending_flag == 2 / ]")
				FSM_CASE_TRANSIT (1, 4, state4_enter_exec, ;, "g_mac_sending_flag == 1", "", "App_Msg_Down", "slot_intrpt", "tr_54", "WSN_MAC_DOWN_NEW [App_Msg_Down -> slot_intrpt : g_mac_sending_flag == 1 / ]")
				}
				/*---------------------------------------------------------*/



			/** state (msg from down) enter executives **/
			FSM_STATE_ENTER_FORCED (3, "msg from down", state3_enter_exec, "WSN_MAC_DOWN_NEW [msg from down enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_DOWN_NEW [msg from down enter execs]", state3_enter_exec)
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
				int subcontrol;
				int subtype;
				double error;
				//receice a packet from father node
				if(C_STRM_MSG_FROM_CHILD_NODE && g_mac_receiving_flag == RECEIVING)
				{
				g_mac_no_packet_flag = Receiced;
				if((g_mac_node_type == GATE_NODE)||(g_mac_node_type == ROUTE_NODE))				//路由节点和网关节点
					{
					pk = op_pk_get(op_intrpt_strm());
					error = op_dist_uniform (1.0);
					if(error<=frame_error_rate)
						{
						op_pk_fd_get (pk, 4, &source);
						if(source == g_mac_node_id)
							{
							op_pk_destroy(pk);
							printf("From child receive a packet from my node!!!Dropped it !!!!!!!!!\n");
							}
						else
							{
							update_alive_time(source);						//保活重置
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
							printf("seq = %d\t;source = %d\t;dest = %d\t;ar= %d\n",seq,source,dest,AR);
							printf("dest mode = %d\t;source_mode = %d\t;type= %d\n",dest_mode,source_mode,type);
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
								printf("Node %d receive an data or MAC control packet from down!!!\n",g_mac_node_id);
								op_prg_log_entry_write(g_mac_down_debug_log_handle,"Node %d receive an data or MAC control packet from down!!!\n",g_mac_node_id);
								if(type == MAC_CONTROL_FRAME)
									{
									op_pk_fd_get(pk,7,&subcontrol);
									subtype = (subcontrol>>4)&15;
									if(subtype == BACKUP_ASSOCIATE_FRAME)
										{
										mac_frame_proc(pk);
										need_ack = 4;
										}
									else if(subtype == ASSOCIATE_REQ_FRAME)
										{
										if(source_mode == SHORT_ADDRESS_MODE)	//	备份节点切换身份后从新关联主节点的父节点
											{
											need_ack = 1;
											}
										else				//	普通节点关联
											{
											need_ack = update_sequence_from_node(source, seq, AR,source_mode);
											}
										}
									else
										{
										need_ack = update_sequence_from_node(source, seq, AR,source_mode);
										}
									}
								else
									{
									need_ack = update_sequence_from_node(source, seq, AR,source_mode);	
									}		
								if(need_ack == 3)
									{
									op_pk_destroy(pk);
									}
								else if(need_ack == 4)
									{
									
									}
								else if(need_ack == 5)
									{
									need_ack = update_sequence_from_fa_node(source, seq,source_mode);
									printf("Node %d need_ack = %d\n",g_mac_node_id,need_ack);
									if(type == DATA_FRAME&&need_ack<3) 
										{
										if(AR == 1)
											{
											create_sub_ack2(pk);
											}	
										if(need_ack == 1) op_pk_send_delayed (pk, OUT_MAC_NWK_DATA,0.5);
										}
									}
								else
									{
									if(source_mode == SHORT_ADDRESS_MODE)
										{
										/*寻找在子节点表中的位置*/
										for(n = 0;n <CHILDREN_NODE_NUM;n++)
											{
											if(neighbor_table[n].node_address == next_hop(source))
												{		
												break;
												}
											}
										/*数据帧处理*/
										if(type == DATA_FRAME) 
											{
											if(AR == 1)
												{
												create_sub_ack(pk, n);
												}	
											if(need_ack == 1) op_pk_send_delayed (pk, OUT_MAC_NWK_DATA,0.5);
											}
										/*控制帧处理*/
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
				else
					{
					op_strm_flush(IN_PHY_MAC);
					}
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** state (msg from down) exit executives **/
			FSM_STATE_EXIT_FORCED (3, "msg from down", "WSN_MAC_DOWN_NEW [msg from down exit execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_DOWN_NEW [msg from down exit execs]", state3_exit_exec)
				{
				op_strm_flush(IN_PHY_MAC);
				}
				FSM_PROFILE_SECTION_OUT (state3_exit_exec)


			/** state (msg from down) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "msg from down", "idle", "tr_55", "WSN_MAC_DOWN_NEW [msg from down -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (slot_intrpt) enter executives **/
			FSM_STATE_ENTER_UNFORCED (4, "slot_intrpt", state4_enter_exec, "WSN_MAC_DOWN_NEW [slot_intrpt enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_DOWN_NEW [slot_intrpt enter execs]", state4_enter_exec)
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
					if(g_mac_node_id == 19&&g_mac_slot_start_time>8000)
						{
						
						
						
						}
					op_intrpt_schedule_self(op_sim_time() + Slot_Time, Intrpt_Slot_Down);				//自己的时隙中断
					g_mac_slot_number = (g_mac_slot_number+1)%10;
					//g_mac_no_packet_flag = Nopacket;
					//op_intrpt_schedule_self(op_sim_time()+Wait_time, UpTime_Code);	
					op_ima_obj_attr_get (g_mac_node_objid,	"g_node_type"	,	&g_mac_node_type);
					op_ima_obj_attr_get (g_mac_node_objid,	"g_node_status",	&g_mac_node_status);
					if(g_mac_node_status == ONLINE)
						{
						op_ima_obj_attr_get (g_mac_node_objid,	"g_node_short_address",	&g_mac_short_address);
						op_ima_obj_attr_get (g_mac_node_objid,	"g_node_band",	&g_mac_down_band);
						op_ima_obj_attr_get (g_mac_node_objid,	"g_node_frequency",&g_mac_recv_frequency);
						op_ima_obj_attr_get (g_mac_node_objid,	"g_node_depth",&g_mac_node_depth);
						if(g_mac_node_type == ROUTE_NODE) set_channel_down(g_mac_down_band,0);
						else if(g_mac_node_type == END_NODE) set_channel_down(100,1);
						else if(g_mac_node_type == BACKUP_NODE&&g_mac_backup_status!=1) set_channel_down(100,1);
				
						
						for(i = 0;i<CHILDREN_NODE_NUM;i++)
							{
							if(neighbor_table[i].node_id != 0||neighbor_table[i].node_address != 0) 
								{
								neighbor_table[i].active_time--;
								}
							if(neighbor_table[i].active_time == 50)		//达到保活危险值
								{
								if(g_mac_node_id == 1)
									{
									printf("i = %d,neighbor_table[i].node_id =%d,neighbor_table[i].node_address = %d\n",i,neighbor_table[i].node_id,neighbor_table[i].node_address);
									printf("beidong\n"); 					
									}
								
								create_actice_frame(neighbor_table[i].node_address,-1);
								neighbor_table[i].active_time = 49;
								}
							if(neighbor_table[i].active_time == 1)		//节点失活
								{
								MLME_ALIVE_CONFIRM(neighbor_table[i].node_address,1,0);
								neighbor_table[i].active_time = -1;	
								}
							}
								
						for(i = 0;i < MAX_RETRANS_NUM;i++)
							{
							if(retrans[i].time<10)
								{
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
								{
								op_intrpt_schedule_self(op_sim_time(), Send_Complete_Code);
								if(g_mac_backup_status == 1)
									{
									op_intrpt_schedule_self(op_sim_time()+Wait_time, Channel_Detect_Code);
									}
								}
								
							}
						else if(g_mac_slot_number!=0 && g_mac_node_status == ONLINE && g_mac_node_type != END_NODE)
							{
							if(g_mac_node_type != BACKUP_NODE) 
								send_msg_to_children();
							else 
								{
								op_intrpt_schedule_self(op_sim_time(), Send_Complete_Code);
								if(g_mac_backup_status == 1)
									{
									op_intrpt_schedule_self(op_sim_time()+Wait_time, Channel_Detect_Code);
									}
								}			
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
			FSM_EXIT (9,"WSN_MAC_DOWN_NEW")


			/** state (slot_intrpt) exit executives **/
			FSM_STATE_EXIT_UNFORCED (4, "slot_intrpt", "WSN_MAC_DOWN_NEW [slot_intrpt exit execs]")


			/** state (slot_intrpt) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_MAC_DOWN_NEW [slot_intrpt trans conditions]", state4_trans_conds)
			FSM_INIT_COND (C_SENDING_COMPLETE)
			FSM_TEST_COND (C_STRM_NWK_DATA)
			FSM_TEST_COND (C_STRM_NWK_MLME)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("slot_intrpt")
			FSM_PROFILE_SECTION_OUT (state4_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, set_channel_start_receive ();, "C_SENDING_COMPLETE", "set_channel_start_receive ()", "slot_intrpt", "idle", "tr_25", "WSN_MAC_DOWN_NEW [slot_intrpt -> idle : C_SENDING_COMPLETE / set_channel_start_receive ()]")
				FSM_CASE_TRANSIT (1, 2, state2_enter_exec, ;, "C_STRM_NWK_DATA", "", "slot_intrpt", "App_Msg_Down", "tr_51", "WSN_MAC_DOWN_NEW [slot_intrpt -> App_Msg_Down : C_STRM_NWK_DATA / ]")
				FSM_CASE_TRANSIT (2, 5, state5_enter_exec, ;, "C_STRM_NWK_MLME", "", "slot_intrpt", "MLME", "tr_58", "WSN_MAC_DOWN_NEW [slot_intrpt -> MLME : C_STRM_NWK_MLME / ]")
				FSM_CASE_TRANSIT (3, 4, state4_enter_exec, ;, "default", "", "slot_intrpt", "slot_intrpt", "tr_38", "WSN_MAC_DOWN_NEW [slot_intrpt -> slot_intrpt : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (MLME) enter executives **/
			FSM_STATE_ENTER_FORCED (5, "MLME", state5_enter_exec, "WSN_MAC_DOWN_NEW [MLME enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_DOWN_NEW [MLME enter execs]", state5_enter_exec)
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
				int dest;
				
				
				
				int fatherBand;
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
									if(control == 0)			//添加短地址
										{
										neighbor_table[i].node_address = address;
										printf("Node %d add neighbor_table[%d].node_address = %d id = %d",g_mac_node_id,i,address,neighbor_table[i].node_id);	
										op_prg_log_entry_write(g_mac_down_info_log_handle,"Node %d add neighbor_table[%d].node_address = %d",g_mac_node_id,i,address);	
										}
									else if(control == 1)		//删除节点信息
										{
										printf("Node %d delete neighbor_table[%d].node_address = %d id = %d",g_mac_node_id,i,address,neighbor_table[i].node_id);
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
								set_channel_down(band,frequency);		//设置频段频点
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
						case MLME_ASSOCIATE_REQUEST:
							{
							if(g_mac_node_type == BACKUP_NODE)
								{
								printf("Node %d down receive a MLME_ASSOCIATE_REQUEST!\n",g_mac_node_id);
								op_pk_fd_get(pk,1,&g_mac_associate_target);
								op_pk_fd_get(pk,2,&g_mac_associate_pan_id);
								op_pk_fd_get(pk,3,&fatherBand);
								op_pk_fd_get(pk,4,&frequency);
								set_channel(fatherBand,frequency);
								create_backup_associate_frame(g_mac_associate_target,g_mac_associate_pan_id);
								op_intrpt_schedule_self(op_sim_time()+6, Associate_Code);
								g_mac_backup_status = 1;
								g_mac_associate_times = 1;
								g_mac_associate_result = 2;
								}
							break;
							}
						case MLME_ALIVE_REQUEST:
							{
							op_pk_fd_get(pk,1,&dest);
							if(g_mac_node_id == 1)printf("MLMEbeidong\n");
							create_actice_frame(dest,-10);
							break;
							}
						default : break;
						}
					op_pk_destroy(pk);
					}
				}
				FSM_PROFILE_SECTION_OUT (state5_enter_exec)

			/** state (MLME) exit executives **/
			FSM_STATE_EXIT_FORCED (5, "MLME", "WSN_MAC_DOWN_NEW [MLME exit execs]")


			/** state (MLME) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_MAC_DOWN_NEW [MLME trans conditions]", state5_trans_conds)
			FSM_INIT_COND (g_mac_sending_flag == 2)
			FSM_TEST_COND (g_mac_sending_flag != 2)
			FSM_TEST_LOGIC ("MLME")
			FSM_PROFILE_SECTION_OUT (state5_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, ;, "g_mac_sending_flag == 2", "", "MLME", "idle", "tr_36", "WSN_MAC_DOWN_NEW [MLME -> idle : g_mac_sending_flag == 2 / ]")
				FSM_CASE_TRANSIT (1, 4, state4_enter_exec, ;, "g_mac_sending_flag != 2", "", "MLME", "slot_intrpt", "tr_61", "WSN_MAC_DOWN_NEW [MLME -> slot_intrpt : g_mac_sending_flag != 2 / ]")
				}
				/*---------------------------------------------------------*/



			/** state (msg from up) enter executives **/
			FSM_STATE_ENTER_FORCED (6, "msg from up", state6_enter_exec, "WSN_MAC_DOWN_NEW [msg from up enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_DOWN_NEW [msg from up enter execs]", state6_enter_exec)
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
				if(C_STRM_MSG_FROM_CHILD_NODE && g_mac_receiving_flag == RECEIVING)
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
						printf("BACKUP node From father receive a packet from my node!!!Dropped it !!!!!!!!!\n");
						}
					else
						{
						op_pk_fd_get (pk, 0, &framecontrol) ;
						op_pk_fd_get (pk, 1, &seq) ;
						op_pk_fd_get (pk, 3, &dest);
						dest_mode = (framecontrol>>4)&3;
						source_mode = framecontrol&3;
						mult = (framecontrol>>8)&1;
						type = (framecontrol>>13)&7;
						AR = (framecontrol>>10)&1;
						//printf("Node %d next_hop(dest) = %d!!!\n",g_mac_node_id,next_hop(dest));	
						if(dest == g_mac_short_address)
							{
							printf("BACKUP node %d MAC reveive a packet from major's father node!!!\n",g_mac_node_id);	
							printf("seq = %d\t;source = %d\t;dest = %d\t;ar= %d\n",seq,source,dest,AR);
							printf("dest mode = %d\t;source_mode = %d\t;type= %d\n",dest_mode,source_mode,type);
							}
						if(mult == 1||(dest == 65535&&type!=0)||dest ==65534||dest ==65533||dest ==65532)
							{
							//需要组播
							if(type == ACK_FRAME)
								{
								ack_frame_proc1(pk);
								}
							}
						else
							{
							if(type == BEACON_FRAME&&g_mac_leaving != MAC_LEAVING)
								{
								//printf("Node %d receive a beacon at slot %d ;time = %f\n",g_mac_node_id,g_mac_slot_number,op_sim_time());	
								//beacon frame ,
								op_pk_destroy(pk);		
								}
							else if(type == ACK_FRAME)
								{				
								ack_frame_proc(pk);
								}
							else if((type == DATA_FRAME||type == MAC_CONTROL_FRAME)&&g_mac_leaving != MAC_LEAVING)
								{
								if(g_mac_leaving == MAC_LEAVING)	op_pk_destroy(pk);
								else if(dest_mode == SHORT_ADDRESS_MODE)
									{								//需要本节点接受的帧
									if((dest == g_mac_short_address))
										{
										printf("BACKUP node %d receive an data or MAC control packet from major's father!!!\n",g_mac_node_id);
										need_ack = update_sequence_from_major_node(source, seq,source_mode);
									//	printf("need_ack= %d\n",need_ack);
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
										printf("BACKUP node %d receive an data or MAC control packet from major's father!!!\n",g_mac_node_id);
										need_ack = update_sequence_from_major_node(source, seq,source_mode);
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
												printf("BACKUP node %d create an ACK1 packet!!!\n",g_mac_node_id);
												create_ack(pk, 1,66);
												}
											else if((source_mode == GATE_ADDRESS_MODE)&&(AR == 1))
												{
												printf("BACKUP node %d create an ACK2 packet!!!\n",g_mac_node_id);
												create_ack(pk, 2,66);
												}
											if(need_ack == 1) op_pk_send_delayed (pk, OUT_MAC_NWK_DATA,0.5);
											else op_pk_destroy(pk);			//uuuuuuuuuuuuuuuuuuuuuuuuuuu
											}
										else if(type == MAC_CONTROL_FRAME)
											{
											if(need_ack == 1) mac_frame_proc(pk);
											else if(need_ack == 2) 
												{
												if((source_mode == SHORT_ADDRESS_MODE)&&(AR == 1))
													create_ack(pk, 1,66);
												else if((source_mode == GATE_ADDRESS_MODE)&&(AR == 1))
													create_ack(pk, 2,66);
												op_pk_destroy(pk);			//uuuuuuuuuuuuuuuuuuuuuuuuuuu
												}
											}
										}
									else if(dest_mode == ESN_ADDRESS_MODE)
										{
										if(type == DATA_FRAME)
											{
											if((source_mode == SHORT_ADDRESS_MODE)&&(AR == 1))
												{
												printf("BACKUP node %d create an ACK3 packet!!!\n",g_mac_node_id);
												create_ack( pk, 3,66);
												}
											else if((source_mode == GATE_ADDRESS_MODE)&&(AR == 1))
												{
												printf("BACKUP node %d create an ACK4 packet!!!\n",g_mac_node_id);
												create_ack(pk, 4,66);
												}
											if(need_ack == 1) op_pk_send_delayed (pk, OUT_MAC_NWK_DATA,0.5);
											else op_pk_destroy(pk);			//uuuuuuuuuuuuuuuuuuuuuuuuuuu
											}
										else if(type == MAC_CONTROL_FRAME)
											{
											if(need_ack == 1) mac_frame_proc(pk);
											else if(need_ack == 2) 
												{
												if((source_mode == SHORT_ADDRESS_MODE)&&(AR == 1))
													create_ack(pk, 3,66);
												else if((source_mode == GATE_ADDRESS_MODE)&&(AR == 1))
													create_ack(pk, 4,66);
												op_pk_destroy(pk);			//uuuuuuuuuuuuuuuuuuuuuuuuuuu
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
					printf("BACKUP node %d received an error packet up!!!\n",g_mac_node_id);
					op_pk_destroy(pk);
					}	
				}else
					{
					op_strm_flush(IN_PHY_MAC);
					}
				}
				FSM_PROFILE_SECTION_OUT (state6_enter_exec)

			/** state (msg from up) exit executives **/
			FSM_STATE_EXIT_FORCED (6, "msg from up", "WSN_MAC_DOWN_NEW [msg from up exit execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_DOWN_NEW [msg from up exit execs]", state6_exit_exec)
				{
				op_strm_flush(IN_PHY_MAC);
				}
				FSM_PROFILE_SECTION_OUT (state6_exit_exec)


			/** state (msg from up) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "msg from up", "idle", "tr_63", "WSN_MAC_DOWN_NEW [msg from up -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (send_up) enter executives **/
			FSM_STATE_ENTER_UNFORCED (7, "send_up", state7_enter_exec, "WSN_MAC_DOWN_NEW [send_up enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_MAC_DOWN_NEW [send_up enter execs]", state7_enter_exec)
				{
				int pkptr_size;
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
							op_intrpt_schedule_self(op_sim_time(), Send_Complete_Code);
							}
						
						}
					else
						{
						op_intrpt_schedule_self(op_sim_time(), Send_Complete_Code);
						}
					}
				
				/********************************************************************/
				/********************************************************************/
				/********************************************************************/
				/********************************************************************/
				/*网络层数据到达处理*/
				if(C_STRM_NWK_DATA&&g_mac_node_type == BACKUP_NODE)
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
								op_pk_destroy(pk);
								}
							}
						else
							{
							printf("Node %d Data_queue is full\n", g_mac_node_id);
							}
						}
					}
				
				/*网络层数据到达处理*/
				if(C_STRM_NWK_DATA&&g_mac_node_type != BACKUP_NODE)
					{
					if(g_mac_leaving != MAC_LEAVING)
						{
						pk = op_pk_get(IN_NWK_MAC_DATA);
						pkt_num = op_subq_stat (Data_Queue, OPC_QSTAT_PKSIZE);
						if(pkt_num<=5)
							{
							seq = get_sequence_to_backup_node(dest);
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
								op_pk_destroy(pk);
								}
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
						default : break;
						}
					op_pk_destroy(pk);
					}
				
				
				if(g_mac_node_type == BACKUP_NODE&&g_mac_backup_status == ONLINE&&C_SELF_START_ASSOCIATE)
					{
					if(g_mac_associate_result == 2&&g_mac_associate_times<4)
						{
						create_backup_associate_frame(g_mac_associate_target,g_mac_associate_pan_id);
						op_intrpt_schedule_self(op_sim_time()+6, Associate_Code);
						g_mac_associate_times++;
						}
					else if(g_mac_associate_times == 4)
						{
						MLME_ASSOCIATE_CONFIRM(1,1);
						g_mac_associate_times = 0;
						}
					}
				}
				FSM_PROFILE_SECTION_OUT (state7_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (15,"WSN_MAC_DOWN_NEW")


			/** state (send_up) exit executives **/
			FSM_STATE_EXIT_UNFORCED (7, "send_up", "WSN_MAC_DOWN_NEW [send_up exit execs]")


			/** state (send_up) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_MAC_DOWN_NEW [send_up trans conditions]", state7_trans_conds)
			FSM_INIT_COND (C_SENDING_COMPLETE)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("send_up")
			FSM_PROFILE_SECTION_OUT (state7_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, set_channel_start_receive ();, "C_SENDING_COMPLETE", "set_channel_start_receive ()", "send_up", "idle", "tr_66", "WSN_MAC_DOWN_NEW [send_up -> idle : C_SENDING_COMPLETE / set_channel_start_receive ()]")
				FSM_CASE_TRANSIT (1, 7, state7_enter_exec, ;, "default", "", "send_up", "send_up", "tr_67", "WSN_MAC_DOWN_NEW [send_up -> send_up : default / ]")
				}
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"WSN_MAC_DOWN_NEW")
		}
	}




void
_op_WSN_MAC_DOWN_NEW_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_WSN_MAC_DOWN_NEW_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_WSN_MAC_DOWN_NEW_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_WSN_MAC_DOWN_NEW_svar function. */
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
#undef g_mac_backupNodeTable
#undef g_mac_backup_status
#undef backup_father_table
#undef g_mac_rand
#undef g_mac_pk_type
#undef g_mac_associate_result
#undef g_mac_associate_times
#undef g_mac_associate_target
#undef g_mac_associate_pan_id

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_WSN_MAC_DOWN_NEW_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_WSN_MAC_DOWN_NEW_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (WSN_MAC_DOWN_NEW)",
		sizeof (WSN_MAC_DOWN_NEW_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_WSN_MAC_DOWN_NEW_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	WSN_MAC_DOWN_NEW_state * ptr;
	FIN_MT (_op_WSN_MAC_DOWN_NEW_alloc (obtype))

	ptr = (WSN_MAC_DOWN_NEW_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "WSN_MAC_DOWN_NEW [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_WSN_MAC_DOWN_NEW_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	WSN_MAC_DOWN_NEW_state		*prs_ptr;

	FIN_MT (_op_WSN_MAC_DOWN_NEW_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (WSN_MAC_DOWN_NEW_state *)gen_ptr;

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
	if (strcmp ("g_mac_backupNodeTable" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->g_mac_backupNodeTable);
		FOUT
		}
	if (strcmp ("g_mac_backup_status" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_backup_status);
		FOUT
		}
	if (strcmp ("backup_father_table" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->backup_father_table);
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
	if (strcmp ("g_mac_associate_result" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_associate_result);
		FOUT
		}
	if (strcmp ("g_mac_associate_times" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_associate_times);
		FOUT
		}
	if (strcmp ("g_mac_associate_target" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_associate_target);
		FOUT
		}
	if (strcmp ("g_mac_associate_pan_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_mac_associate_pan_id);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}


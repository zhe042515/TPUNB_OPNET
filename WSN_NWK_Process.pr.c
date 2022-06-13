/* Process model C form file: WSN_NWK_Process.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char WSN_NWK_Process_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A modeler 7 62A6F564 62A6F564 1 DESKTOP-RD4S7T2 51133 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                    ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */


//----------------------------------������������-------------------------------------//
#define NWK_JOIN_CONTROL			1
#define NWK_MAX_JOIN_COUNT			20
#define NWK_PROXY_JOIN_TIMER		10
#define NWK_Timer_RESEND_JOIN		100



//----------------------------------Node Type Define-------------------------------------//
#define ROUTERNODE			1
#define GATENODE			0
#define ENDNODE				2
#define BACKUPNODE			3

#define GateAddress			43690


//------------------------------Node Frequency Num Define---------------------------------//
/*
	�ڵ�Ƶ����
*/
#define GATE_FREQNUM				4//���ؽڵ�ʹ�õ�Ƶ����
#define ROUTER_FREQ_NUM				1//·�ɽڵ�ʹ�õ�Ƶ����

//-----------------------------Timer Parameters Define-----------------------------------//
#define NWK_TIMER_MAX_INIT						60 * 20
#define NWK_TIMER_UPDATE						5
#define NWK_TIMER_RT							100
#define NWK_COUNT_CT_STATUS						1
// NWK_TIMER 
#define NWK_COUNT_BACKUP						1 
//·��ά�����ں�·��ά����ʱʱ�䣬��λ���룬���ڷ���ʱÿ10s��ѯ�������Ҫ������ѯ���ڣ�/ NWK_TIMER 
#define NWK_COUNT_RT_ACTIVE						30*60 / NWK_TIMER 
#define NWK_COUNT_RT_TIMEOUT					200 / NWK_TIMER 
#define NWK_TIMER_UPDATE_STATUS					20
#define NWK_TIMER_JOIN							100


#define NWK_TIMER								10
#define NWK_TIMER_VALID_FATHER_TABLE			30 * 60 
#define NWK_COUNT_MAPPING_TABLE					120000
#define NWK_COUNT_ADDRESS_POOL					120 * 60 * 24
#define NWK_COUNT_ADDRESS_POOL_OLD				120 * 60 * 24
#define NWK_TIMER_MASTER_MONITOR				60
#define NWK_TIMER_MUTICAST_MAINTAIN				60*130

#define NWK_COUNT_MONITOR						1
#define NWK_COUNT_VIRIFY						10


#define NWK_Minites								6

//---------------------------------Stream Port Define------------------------------------//
#define IN_DATA_TO_NWK					1
#define IN_CON_TO_NWK					0
#define IN_MCPS_UP_TO_NWK				2
#define IN_MCPS_DOWN_TO_NWK				3
#define IN_MLME_TO_NWK					4
#define OUT_NWK_TO_MLME					2
#define OUT_NWK_TO_MCPS_UP				0
#define OUT_NWK_TO_MCPS_DOWN			1
#define OUT_NWK_TO_SINK_UP				3
#define OUT_NWK_TO_SINK_DOWN			4

//------------------------------------Status Define----------------------------------------//
#define NWK_ROUTING_TABLE_Active				0
#define NWK_ROUTING_TABLE_TIMEOUT				1
#define NWK_ROUTING_TABLE_DELETED				2
#define NWK_CHILD_TABLE_ILLEGAL					0
#define NWK_CHILD_TABLE_LEGITIMATE				1
#define NWK_CHILD_TABLE_DELETED					2

//----------------------------- Interrupt Code Define------------------------------------//
#define intrCode_Timer				0
#define intrCode_Join				1
#define intrCode_ChangeMainRouter	2
#define intrCode_ReJoin				3
#define intrCode_Father				4
#define intrCode_Init				5
#define intrCode_MASTER_MONITOR		6
#define intrCode_Resend_Join		7

//----------------------------Upper Limit Parameters Define------------------------------//
//�ڵ���������
#define NWK_MAX_RADIUS									8
//���������ڵ�����
#define NWK_MAX_NODE_NUM								200
//���������·��������
#define NWK_MAX_ROUTER_NUM								48
//�ڵ�����ӽڵ�����
#define NWK_MAX_CHILD_NUM								20
//���·������ַ����
#define NWK_MAX_Router_Address_NUM						128
//���������֧�ֵ��鲥������
#define NWK_MAX_MULTICAST_NUM							16
//Ǳ�ڸ��ڵ���������
#define NWK_MAX_FATHERTABLE_NUM							6
//����ͬ��ʱһ������ܹ������·�ɱ���Ŀ��
#define NWK_MAX_ROUTER_TABLE_INFO_NUM					9
//����ͬ��ʱһ������ܹ�������ӽڵ����Ŀ��
#define NWK_MAX_CHILD_TABLE_INFO_NUM					5
//����ͬ�����еĳ���
#define NWK_QUEUE_LEN									20

//7bits�޷����������ֵ�Ķ���
#define NWK_7bitMAX										128
//8bits�޷����������ֵ�Ķ���
#define NWK_8bitMAX										256


//-------------------------------Online Condition Define---------------------------------//
#define INTRPT_STRM_DATA				((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == IN_DATA_TO_NWK))
#define INTRPT_STRM_CON					((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == IN_CON_TO_NWK))
#define INTRPT_STRM_MCPS				((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == IN_MCPS_UP_TO_NWK || op_intrpt_strm() == IN_MCPS_DOWN_TO_NWK))
#define INTRPT_STRM_MCPS_UP				((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == IN_MCPS_UP_TO_NWK))
#define INTRPT_STRM_MCPS_DOWN			((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == IN_MCPS_DOWN_TO_NWK))
#define INTRPT_STRM_MLME				((op_intrpt_type() == OPC_INTRPT_STRM) && (op_intrpt_strm() == IN_MLME_TO_NWK))
#define INTRPT_SELF_UPDATE				((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() != intrCode_ReJoin) && (op_intrpt_code() != intrCode_Join))
#define INTRPT_SELF_JOIN				((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == intrCode_Join))
#define INTRPT_SELF_REJOIN				((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == intrCode_ReJoin))
#define INTRPT_SELF_INIT				((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == intrCode_Init))
#define INTRPT_RESEND_JOIN				((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == intrCode_Resend_Join))


//--------------------------------Other Condition Define---------------------------------//
#define SUCCESS				(g_nwk_result == 1)
#define FAILT				(g_nwk_result == 0)
#define JOINFAILT			(INTRPT_SELF_JOIN)
#define NON_FATHER			(g_nwk_available_potential_father_number == 0)
#define NONA_FATHER			(g_nwk_available_potential_father_number > 0)
#define REASSOCIATE			(SUCCESS&&g_nwk_specify_father>0)
#define REJOIN_SAMEFATHER	((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == intrCode_ReJoin)&&g_nwk_specify_father == g_nwk_my_father_short_address)

//---------------------------------Field Length Define-----------------------------------//
#define FRAMCONTR_LEN		16
#define MACSEQ_LEN			8
#define PANID_LEN			16
#define SHORTADDR_LEN		16
#define LONGADDR_LEN		48
#define NWKCONTR_LEN		16
#define LENGTH_LEN			8
#define IES_LEN				24

//------------------------------Broadcast Address Define---------------------------------//
#define BroadCast			65535//ȫ���㲥��ַ
#define RouterCast			65534//ȫ��·�ɽڵ�㲥��ַ
#define ChildrenCast		65533//ȫ�ӽڵ�㲥��ַ
#define ChildrenEndCast		65532//�ӽڵ����ն˽ڵ��ַ

//----------------------------------MLME Type Define-------------------------------------//
/*
	ԭ������Ͷ���
*/
#define SCAN				0
#define ASSOCIATE			1
#define ALIVE				2
#define ChildSEt			3
#define Conflict			4
#define Multicast			5
#define MASTER_MONITOR		6
#define LEAVE				7
#define SYN_SEQ_SET			8
#define BEACON				9


//---------------------------------g_nwk_result Type Define------------------------------------//
#define SUCCESSFUL			0
#define FAIL				1





struct FatherTable{
	int NWKAddress;
	int PanID;
	int DeviceType;
	int Depth;
	int RoutingCost;
	int Band;
	int Frequency;
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
	struct childrenTable table[NWK_MAX_CHILD_NUM];
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
	struct routerTable table[NWK_MAX_ROUTER_NUM];
};


struct SynchronizeTable{
	int sequence;
	int status;
	int routerStart;
	int routerEnd;
	int childStart;
	int childEnd;
};

struct SynchronizeQueue{
	int number;//��¼ͬ�������д�����ͬ����Ϣ������
	int count;
	char p_start;//��¼ͬ�����е���ʼλ��
	char p_end;
	struct SynchronizeTable table[NWK_QUEUE_LEN];
};

struct NetworkMsg{
	struct RTable NetworkRT[NWK_MAX_NODE_NUM];
	struct CTable NetworkCT[NWK_MAX_NODE_NUM];
};


struct childMulticastTable{
	int group;
	int maintain;
	int timerCount;
};

struct mappingTable{
	int shortAddress;
	int count;
};
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

struct Father{
	struct Potential_Parent father[NWK_MAX_FATHERTABLE_NUM];
};

struct Fa{
	struct Father father[NWK_MAX_NODE_NUM];
};

struct RAddress{//����ά���ķ����·�����ĵ�ַ��
	char old;//��ַ��״̬,0:����δʹ��,1:����ʹ�ù�
	int ESNAddress;//�ڵ��ַ,-1��ʾδ����
	int count;//�����������ڼ�ʱ
};

struct EAddress{//���ڵ�ά�����ն˵�ַ��
	char old;//��ַ��״̬,0:����δʹ��,1:����ʹ�ù�
	int ESNAddress;//�ڵ��ַ
	int count;//�����������ڼ�ʱ
};

struct R2BEntry{
	int Router;
	int Backup;
};

struct ChildRouter2BackupMap{
	int p;
	int index;
	int num;
	struct R2BEntry entrys[NWK_MAX_CHILD_NUM];// NWK_MAX_CHILD_NUMӰ����룬�����Ҹ�
};

struct NetworkMsg Network_Msg;

extern struct Fa potentialParent;

struct mappingTable mapping[NWK_MAX_NODE_NUM];//���ؽڵ�ʹ�õĳ��̵�ַӳ���

struct RAddress g_nwk_router_address_pool[NWK_MAX_ROUTER_NUM];//���ؽڵ㱣���·�ɵ�ַ��

int bandTable[NWK_MAX_ROUTER_NUM];//���ؽڵ�ά����Ƶ�γ�

//------------------------------------ȫ��ͳ����---------------------------------------//
int g_pk_send_down = 0;
int g_pk_recv_down = 0;
int g_pk_send_up = 0;
int g_pk_recv_up = 0;
int onlineNodes[NWK_MAX_NODE_NUM];
int onlineNodesNUmber;
int multicast_receive_num = 0;
int leave_node_number = 0;

int frequency_distr[] = {0, 0, 0, 0};


static void pk_stat_up(Packet* pkptr);
static void pk_stat_down(Packet* pkptr);
static void nwk_delay_start();
static void nwk_init_RCtable();
static void nwk_init_gate();
static void nwk_init_router();
static void nwk_online();
static void nwk_scan_request(double delay);
static void nwk_associate_request();
static void nwk_leave_request(int flag);
//����MAC��Leaveԭ�ﲢ����
static int nwk_leave_confirm(int res);
//���յ�MAC��ͬ�����к�ԭ�����δ���
static void nwk_syn_seq_comfirn(int seq);
static void nwk_keep_alive(int shortAddress, int res, int flag);
static void nwk_keep_alive_request(int shortAddress, int flag);
static void nwk_join_request();


static void nwk_generate_packet(int pklen,int multicast,int ar,int sourceMode,int destMode,int source,int dest,int type,int TTL,int NWKLen,Packet* PayLoad,int streamPort,double delay);
static int  nwk_get_nexthop(int dest);
static void nwk_forward_packet(Packet* pkptr,int streamPort);

static void nwk_process_Join_request(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
static int nwk_process_access_confirm(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
static void nwk_process_routing(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
static void nwk_process_multicast_control(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
static void nwk_process_leave(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
static void nwk_send_main_router_request();
static void nwk_process_main_router_request(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
static void nwk_process_main_router_status(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
//��ַ��ͻ���մ���
static void addressconflict(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
static void nwk_process_backup_router_info(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
//�����л�֪ͨ����Ľ��մ���
static void nwk_process_backup_router_switch(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len, int port);
static void nwk_process_nwk_status(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
static void nwk_process_routing_control(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
static void nwk_process_recover_node(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
static void nwk_process_up_data(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
static void nwk_process_down_data(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);


/*
	�����ɾ�Ĳ�
*/
static void generateCT(int ESNAddress,int NWKAddress,int CapabilityInformation, int status);
static void sendChildSetConfirm(int ESNAddress, int NWKAddress);

static void updateCT();
static int get_illegal_CT_number();

static void DeleteCT(int type, int ESNAddress);
static void generateRT(int destAddress,int nextHop,int status,int routerSeq);
static void UpdateRT(int NWKAddress, int status);
static void deleteRouting(int routerSeq, int NWKAddress);
static void UpdateBackup();
static int queryRTIndex(int NWKAddress);
static int CTNWKAddresss(int ESNAddress);
static void UPCTStatus(int ESNAddress,int NWKAddress);
static void UPCTDeviceType(int NWKAddress);
static int quiryNWKAddress(int MACAddress);
static int quiryRouterSeq(int NWKAddress);
static int CTinclude(int NWKAddress);
static int bandQuire(int NWKAddress);
static int quiryMT(int multicastID);
static int quiryChildMT(int dest);
static void maintainCMT(int multicastMain);
//���Ҷ̵�ַ��ӳ����е�λ�ü�ESN����ַ����������/����ַ
static int quiryNWKAddressFromMap(int NWKAddress);
static int queryFT(int fatherAddress);
static int RouterNumber();
static int CTNumber();
static int EndNumber();

/*
	��Դ�ķ�����ͷ�
*/
static int nwk_distribute_address(int deviceType, int shortAddress, int ESNAddress);
static int nwk_distribute_band(int NWKAddress);
static void nwk_update_address(int NWKAddress);
static int nwk_distribute_frequency(int type);
static void nwk_send_conflict(int shortAddress);
//�޸ĵ�ַӳ�����Ϣ
static void nwk_update_mapping(int ESN_address, int NWK_address);
//�����Ĵ���
static void nwk_leave_net(int NWKAddress,int rejoin,int children,int assign);
//���ýڵ�״̬Ϊ����״̬
static void nwk_leave_set();

/*
	������Ϣͬ��
*/
static void nwk_push_synchronize_queue(int status, int rstart, int rend,int cstart, int cend);
static void nwk_send_synchronize_queue();
static void nwk_init_send_backup_info();
static void nwk_send_backup_info(int type,int addOrReduce,int NWKAddress,int start,int num);

/*
	���ݽڵ������·��
*/
static void nwk_master_monitor();
static void nwk_master_inactivated_process();

/*
	���͵�����״̬��ѯ����
*/
static void nwk_send_master_status(int dest, int request, int short_address, int status);
/*
	��������֤������մ���
*/
static void nwk_master_status_process(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
//����·�ɽڵ��滻���ڵ�
static void nwk_change_slave2master();

/*
	��ѯ����ӳ���
*/
static int g_nwk_query_r2b_map(int mainRouter);

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
	int	                    		g_nwk_result                                    ;	/* ���ڱ�ʶɨ���� */
	int	                    		g_nwk_nodeID                                    ;	/* �ڵ�ID */
	struct FatherTable	     		g_nwk_father_table                              ;	/* Ǳ�ڸ��ڵ�� */
	int	                    		g_nwk_my_father_short_address                   ;	/* ѡ��ĸ��ڵ� */
	int	                    		g_nwk_capability                                ;	/* �ڵ����� */
	int	                    		g_nwk_node_type                                 ;	/* �ڵ����� */
	int	                    		g_nwk_my_ESN_address                            ;	/* �ڵ㳤��ַ */
	int	                    		g_nwk_join_type                                 ;	/* �ڵ��������� */
	struct CTable	          		g_nwk_children_table                            ;	/* �ӽڵ�� */
	struct RTable	          		g_nwk_routing_table                             ;	/* ·�ɱ� */
	int	                    		g_nwk_my_short_address                          ;	/* �����̵�ַ */
	struct EAddress	        		g_nwk_end_address_pool[NWK_MAX_NODE_NUM]        ;	/* �ڵ�Ϊ�ն˽ڵ����ĵ�ַ�� */
	int	                    		g_nwk_routing_table_start                       ;	/* ��ʶ·�ɱ��ݵĽ��� */
	int	                    		g_nwk_children_table_start                      ;	/* ��ʶ�ӽڵ��Ľ��� */
	int	                    		g_nwk_myBand                                    ;	/* �ڵ�����Ƶ�κ� */
	int	                    		g_nwk_PANID                                     ;	/* PAN��ʶ�� */
	int	                    		g_nwk_backup_router                             ;	/* ���ݽڵ�����ʶ */
	int	                    		g_nwk_my_frequency                              ;	/* �����豸����Ƶ��� */
	int	                    		g_nwk_my_routing_sequence                       ;	/* ����·�����к� */
	int	                    		g_nwk_my_multicast_table                        ;	/* �����鲥�� */
	struct childMulticastTable			g_nwk_children_multicast_table                  ;	/* ���νڵ��鲥�� */
	int	                    		g_nwk_main_routing_address                      ;	/* ��·�ɳ���ַ */
	int	                    		g_nwk_main_address                              ;	/* ��·�ɶ̵�ַ */
	int	                    		g_nwk_main_router_band                          ;	/* ��·��Ƶ�� */
	int	                    		g_nwk_main_router_frequency                     ;	/* ��·������Ƶ�� */
	int	                    		g_nwk_main_router_father                        ;	/* ��·�ɵĸ��ڵ�̵�ַ */
	struct SynchronizeQueue			g_nwk_synchronize_queue                         ;	/* ����ͬ����ͬ������ */
	int	                    		g_nwk_available_potential_father_number         ;	/* Ǳ�ڸ��ڵ����ø��ڵ��� */
	int	                    		g_nwk_specify_father                            ;	/* ָʾ�Ƿ���ָ�����ڵ����� */
	int	                    		g_nwk_synchronize_sequence                      ;	/* ͬ�����к� */
	int	                    		g_nwk_mulcast_maintain                          ;	/* �鲥ά��ʹ�� */
	Log_Handle	             		g_nwk_info_log_handle                           ;	/* ��Ϣ��־��� */
	Log_Handle	             		g_nwk_debugger_log_handle                       ;	/* ������־��� */
	int	                    		g_nwk_syn_seq                                   ;	/* ����ͬ�����к� */
	char	                   		g_nwk_macLeaveFlag[2]                           ;	/* ��¼MAC������״̬ */
	Stathandle	             		bits_rcvd_stathandle                            ;
	Stathandle	             		bitssec_rcvd_stathandle                         ;
	Stathandle	             		pkts_rcvd_stathandle                            ;
	Stathandle	             		pktssec_rcvd_stathandle                         ;
	Stathandle	             		ete_delay_stathandle                            ;
	Stathandle	             		bits_rcvd_gstathandle                           ;
	Stathandle	             		bitssec_rcvd_gstathandle                        ;
	Stathandle	             		pkts_rcvd_gstathandle                           ;
	Stathandle	             		pktssec_rcvd_gstathandle                        ;
	Stathandle	             		ete_delay_gstathandle                           ;
	int	                    		rcvd_pkts                                       ;
	int	                    		send_pkts                                       ;
	Stathandle	             		recv_send_rate                                  ;
	Stathandle	             		ete_delay_l_n                                   ;
	Stathandle	             		online_number_gstathandle                       ;	/* �ڵ���������ͳ���� */
	Stathandle	             		recv_send_rate_down                             ;
	Stathandle	             		ete_delay_gstathandle_down                      ;
	Stathandle	             		multicast_count                                 ;
	Stathandle	             		leave_node_count                                ;
	int	                    		g_nwk_test_type                                 ;
	double	                 		g_nwk_join_time                                 ;	/* �ڵ�����ʱ��ͳ�� */
	Stathandle	             		join_time_lstathandle                           ;
	Stathandle	             		send_up_num                                     ;	/* �������ݵ����ݰ�����ͳ�� */
	Stathandle	             		send_dowm_num                                   ;
	Stathandle	             		recv_up_num                                     ;
	Stathandle	             		recv_dowm_num                                   ;
	struct ChildRouter2BackupMap			g_nwk_r2b_Map                                   ;	/* ��¼�ӽڵ��������ڵ��ӳ���ϵ */
	int	                    		g_nwk_beacon_confirm                            ;	/* ��¼һ���������յ����ű�δ�յ���ȷ��ԭ������� */
	int	                    		g_nwk_beacon_confirm_count                      ;	/* �ȴ���������֤����ļ�ʱ�� */
	int	                    		g_nwk_reassociate_leave_flag                    ;	/* ������ʶ */
	} WSN_NWK_Process_state;

#define g_nwk_result            		op_sv_ptr->g_nwk_result
#define g_nwk_nodeID            		op_sv_ptr->g_nwk_nodeID
#define g_nwk_father_table      		op_sv_ptr->g_nwk_father_table
#define g_nwk_my_father_short_address		op_sv_ptr->g_nwk_my_father_short_address
#define g_nwk_capability        		op_sv_ptr->g_nwk_capability
#define g_nwk_node_type         		op_sv_ptr->g_nwk_node_type
#define g_nwk_my_ESN_address    		op_sv_ptr->g_nwk_my_ESN_address
#define g_nwk_join_type         		op_sv_ptr->g_nwk_join_type
#define g_nwk_children_table    		op_sv_ptr->g_nwk_children_table
#define g_nwk_routing_table     		op_sv_ptr->g_nwk_routing_table
#define g_nwk_my_short_address  		op_sv_ptr->g_nwk_my_short_address
#define g_nwk_end_address_pool  		op_sv_ptr->g_nwk_end_address_pool
#define g_nwk_routing_table_start		op_sv_ptr->g_nwk_routing_table_start
#define g_nwk_children_table_start		op_sv_ptr->g_nwk_children_table_start
#define g_nwk_myBand            		op_sv_ptr->g_nwk_myBand
#define g_nwk_PANID             		op_sv_ptr->g_nwk_PANID
#define g_nwk_backup_router     		op_sv_ptr->g_nwk_backup_router
#define g_nwk_my_frequency      		op_sv_ptr->g_nwk_my_frequency
#define g_nwk_my_routing_sequence		op_sv_ptr->g_nwk_my_routing_sequence
#define g_nwk_my_multicast_table		op_sv_ptr->g_nwk_my_multicast_table
#define g_nwk_children_multicast_table		op_sv_ptr->g_nwk_children_multicast_table
#define g_nwk_main_routing_address		op_sv_ptr->g_nwk_main_routing_address
#define g_nwk_main_address      		op_sv_ptr->g_nwk_main_address
#define g_nwk_main_router_band  		op_sv_ptr->g_nwk_main_router_band
#define g_nwk_main_router_frequency		op_sv_ptr->g_nwk_main_router_frequency
#define g_nwk_main_router_father		op_sv_ptr->g_nwk_main_router_father
#define g_nwk_synchronize_queue 		op_sv_ptr->g_nwk_synchronize_queue
#define g_nwk_available_potential_father_number		op_sv_ptr->g_nwk_available_potential_father_number
#define g_nwk_specify_father    		op_sv_ptr->g_nwk_specify_father
#define g_nwk_synchronize_sequence		op_sv_ptr->g_nwk_synchronize_sequence
#define g_nwk_mulcast_maintain  		op_sv_ptr->g_nwk_mulcast_maintain
#define g_nwk_info_log_handle   		op_sv_ptr->g_nwk_info_log_handle
#define g_nwk_debugger_log_handle		op_sv_ptr->g_nwk_debugger_log_handle
#define g_nwk_syn_seq           		op_sv_ptr->g_nwk_syn_seq
#define g_nwk_macLeaveFlag      		op_sv_ptr->g_nwk_macLeaveFlag
#define bits_rcvd_stathandle    		op_sv_ptr->bits_rcvd_stathandle
#define bitssec_rcvd_stathandle 		op_sv_ptr->bitssec_rcvd_stathandle
#define pkts_rcvd_stathandle    		op_sv_ptr->pkts_rcvd_stathandle
#define pktssec_rcvd_stathandle 		op_sv_ptr->pktssec_rcvd_stathandle
#define ete_delay_stathandle    		op_sv_ptr->ete_delay_stathandle
#define bits_rcvd_gstathandle   		op_sv_ptr->bits_rcvd_gstathandle
#define bitssec_rcvd_gstathandle		op_sv_ptr->bitssec_rcvd_gstathandle
#define pkts_rcvd_gstathandle   		op_sv_ptr->pkts_rcvd_gstathandle
#define pktssec_rcvd_gstathandle		op_sv_ptr->pktssec_rcvd_gstathandle
#define ete_delay_gstathandle   		op_sv_ptr->ete_delay_gstathandle
#define rcvd_pkts               		op_sv_ptr->rcvd_pkts
#define send_pkts               		op_sv_ptr->send_pkts
#define recv_send_rate          		op_sv_ptr->recv_send_rate
#define ete_delay_l_n           		op_sv_ptr->ete_delay_l_n
#define online_number_gstathandle		op_sv_ptr->online_number_gstathandle
#define recv_send_rate_down     		op_sv_ptr->recv_send_rate_down
#define ete_delay_gstathandle_down		op_sv_ptr->ete_delay_gstathandle_down
#define multicast_count         		op_sv_ptr->multicast_count
#define leave_node_count        		op_sv_ptr->leave_node_count
#define g_nwk_test_type         		op_sv_ptr->g_nwk_test_type
#define g_nwk_join_time         		op_sv_ptr->g_nwk_join_time
#define join_time_lstathandle   		op_sv_ptr->join_time_lstathandle
#define send_up_num             		op_sv_ptr->send_up_num
#define send_dowm_num           		op_sv_ptr->send_dowm_num
#define recv_up_num             		op_sv_ptr->recv_up_num
#define recv_dowm_num           		op_sv_ptr->recv_dowm_num
#define g_nwk_r2b_Map           		op_sv_ptr->g_nwk_r2b_Map
#define g_nwk_beacon_confirm    		op_sv_ptr->g_nwk_beacon_confirm
#define g_nwk_beacon_confirm_count		op_sv_ptr->g_nwk_beacon_confirm_count
#define g_nwk_reassociate_leave_flag		op_sv_ptr->g_nwk_reassociate_leave_flag

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	WSN_NWK_Process_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((WSN_NWK_Process_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif

static void pk_stat_up(Packet* pkptr)
{
	double		pk_size;
	double		ete_delay;
	FIN(pk_stat(Packet* pkptr));
	rcvd_pkts++;
	
	//printf("+++++++recv %d%f\n",rcvd_pkts,op_sim_time());

	/* Caclulate metrics to be updated.		*/
	pk_size = (double) op_pk_total_size_get (pkptr);
	ete_delay = op_sim_time () - op_pk_creation_time_get (pkptr);

	/* Update local statistics.				*/
	op_stat_write (bits_rcvd_stathandle, 		pk_size);
	op_stat_write (pkts_rcvd_stathandle, 		1.0);
	op_stat_write (ete_delay_stathandle, 		ete_delay);

	op_stat_write (bitssec_rcvd_stathandle, 	pk_size);
	op_stat_write (bitssec_rcvd_stathandle, 	0.0);
	op_stat_write (pktssec_rcvd_stathandle, 	1.0);
	op_stat_write (pktssec_rcvd_stathandle, 	0.0);

	/* Update global statistics.	*/
	op_stat_write (bits_rcvd_gstathandle, 		pk_size);
	op_stat_write (pkts_rcvd_gstathandle, 		1.0);
	op_stat_write (ete_delay_gstathandle, 		ete_delay);

	op_stat_write (bitssec_rcvd_gstathandle, 	pk_size);
	op_stat_write (bitssec_rcvd_gstathandle, 	0.0);
	op_stat_write (pktssec_rcvd_gstathandle, 	1.0);
	op_stat_write (pktssec_rcvd_gstathandle, 	0.0);
	
	op_stat_write (recv_up_num, 	g_pk_recv_up);
	
	op_stat_write (recv_send_rate, 	((double)g_pk_recv_up)/((double)g_pk_send_up));
	FOUT;
}

static void pk_stat_down(Packet* pkptr)
{
	double		pk_size;
	double		ete_delay;
	FIN(pk_stat(Packet* pkptr));
	rcvd_pkts++;
	
	//printf("+++++++recv %d%f\n",rcvd_pkts,op_sim_time());

	/* Caclulate metrics to be updated.		*/
	pk_size = (double) op_pk_total_size_get (pkptr);
	ete_delay = op_sim_time () - op_pk_creation_time_get (pkptr);

	/* Update local statistics.				*/
	
	op_stat_write (ete_delay_gstathandle_down, 		ete_delay);


	op_stat_write (recv_dowm_num, 	g_pk_recv_down);
	
	op_stat_write (recv_send_rate_down, 	((double)g_pk_recv_down)/((double)g_pk_send_down));
	FOUT;
}

/*
	�ڵ�����˱�����
*/
static void nwk_delay_start()
{
	FIN(nwk_delay_start());
	if(g_nwk_test_type == 4) op_intrpt_schedule_self(op_sim_time() + NWK_TIMER_MAX_INIT,intrCode_Init);
	else op_intrpt_schedule_self(op_sim_time(), intrCode_Init);
	FOUT;
}
/*
	��ʼ���ӽڵ�� ·�ɱ� �鲥��
*/
static void nwk_init_RCtable()
{
	FIN(nwk_init_RCtable());
	g_nwk_routing_table.number = 0;
	Network_Msg.NetworkCT[g_nwk_my_ESN_address].number = 0;
	if(g_nwk_my_ESN_address == 4) printf("ccccc");
	Network_Msg.NetworkRT[g_nwk_my_ESN_address] = g_nwk_routing_table;
	g_nwk_routing_table_start = 0;
	g_nwk_children_table_start = 0;
	g_nwk_children_multicast_table.timerCount = NWK_TIMER_MUTICAST_MAINTAIN;
	if(g_nwk_join_type ==3 && g_nwk_backup_router > 0)
		{
		//printf("sim_time:\tlevel\tnode\tNWK\tlog_type\ncontext:\n");
		}
	FOUT;
}

/*
	��ʼ�����ؽڵ�ı���
*/
static void nwk_init_gate()
{
	int i ;
	FIN(nwk_init_gate());
	for(i = 0;i < NWK_MAX_ROUTER_NUM; i++)
		{
		g_nwk_router_address_pool[i].ESNAddress = -1;
		g_nwk_router_address_pool[i].old = 0;
		bandTable[i] = 0;
		}
	for(i = 0;i < NWK_MAX_NODE_NUM; i++)
		{
		mapping[i].shortAddress = 0;
		mapping[i].count = -1;
		onlineNodes[i] = 0;
		g_nwk_end_address_pool[i].ESNAddress = -1;
		g_nwk_end_address_pool[i].old = 0;
		}
	onlineNodesNUmber = 1;
	mapping[0].shortAddress = GateAddress;
	mapping[0].count = -1;
	nwk_init_RCtable();
	FOUT;
}

/*
	��ʼ��·�ɽڵ�
*/
static void nwk_init_router()
{
	int i ;
	FIN(nwk_init_router());
	for(i = 0;i < NWK_MAX_NODE_NUM; i++)
		{
		g_nwk_end_address_pool[i].ESNAddress = -1;
		g_nwk_end_address_pool[i].old = 0;
		}
	g_nwk_backup_router = -1;
	g_nwk_routing_table_start = -1;
	g_nwk_children_table_start = -1;
	g_nwk_synchronize_queue.number = 0;
	g_nwk_synchronize_queue.p_start = 0;
	g_nwk_synchronize_queue.p_end = 1;
	nwk_init_RCtable();
	g_nwk_backup_router = 0;
	g_nwk_r2b_Map.num = 0;
	g_nwk_r2b_Map.p = 0;
	g_nwk_r2b_Map.index = 0;
	FOUT;
}

/*
	��������״̬�Ĵ�ӡ
*/
static void nwk_online()
{
	FIN(nwk_online());
	op_intrpt_schedule_self(op_sim_time() + NWK_TIMER,intrCode_Timer);
	//if(g_nwk_my_ESN_address != 2)	op_ima_obj_attr_set(g_nwk_nodeID,"g_node_status",1);
	op_ima_obj_attr_set(g_nwk_nodeID,"g_node_status",1);
	op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d online!\n", g_nwk_my_ESN_address);
	FOUT;
}

/*
	��MAC����MLME-SCAN.requestԭ��Ҫ��MAC�����ŵ�ɨ��
*/
static void nwk_scan_request(double delay)
	{
	int len = 1;
	Packet* mlme;
	FIN(nwk_scan_request(double delay));
	mlme = op_pk_create(0);
	op_pk_fd_set (mlme, 0, OPC_FIELD_TYPE_INTEGER, SCAN, len*8);
	//op_pk_send_delayed(mlme, OUT_NWK_TO_MLME, (g_nwk_my_ESN_address-1) * 60);
	op_pk_send_delayed(mlme, OUT_NWK_TO_MLME, delay);
	//printf("66666\n");
	FOUT;
	}

/*
	��Ǳ�ڸ��ڵ����ѡ��һ�����ʵĸ��ڵ����MAC����MLME-ASSOCIATE.request
*/
static void nwk_associate_request(int flag)
	{
	int i;
	int len = 6;
	Packet* mlme;
	int minCost = 1000000;
	int index = -1;
	FIN(nwk_associate_request(int flag));
	if(g_nwk_specify_father > 0 && g_nwk_father_table.NWKAddress > 0)
		{
		//printf("g_nwk_specify_father = %d\n", g_nwk_specify_father);
		g_nwk_specify_father = 0;
		}
	else
		{
		for(i = 0;i < NWK_MAX_FATHERTABLE_NUM;i++)
			{
			//��Ǳ�ڸ��ڵ������ѡ���ʵĸ��ڵ�
			if(potentialParent.father[g_nwk_my_ESN_address].father[i].short_addr == 0) continue;
		
			if(minCost > potentialParent.father[g_nwk_my_ESN_address].father[i].router_cost && potentialParent.father[g_nwk_my_ESN_address].father[i].times == 0)
				{
				minCost = potentialParent.father[g_nwk_my_ESN_address].father[i].router_cost;
				index = i;
				}
			}
		if(index >= 0)
			{
			potentialParent.father[g_nwk_my_ESN_address].father[index].times++;
			g_nwk_father_table.NWKAddress = potentialParent.father[g_nwk_my_ESN_address].father[index].short_addr;
			g_nwk_father_table.PanID = potentialParent.father[g_nwk_my_ESN_address].father[index].panid;
			g_nwk_father_table.Depth = potentialParent.father[g_nwk_my_ESN_address].father[index].depth;
			g_nwk_father_table.RoutingCost = potentialParent.father[g_nwk_my_ESN_address].father[index].router_cost;
			g_nwk_father_table.Band = potentialParent.father[g_nwk_my_ESN_address].father[index].band;
			g_nwk_father_table.Frequency = potentialParent.father[g_nwk_my_ESN_address].father[index].frequency;
			g_nwk_my_father_short_address = g_nwk_father_table.NWKAddress;
			//potentialParent.father[g_nwk_my_ESN_address].father[i].times += 1;
			g_nwk_available_potential_father_number--;
			
			}
		else
			{
			printf("Node %d û�к��ʵĸ��豸������ʧ�ܣ�\n", g_nwk_my_ESN_address);
			//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d û�к��ʵĸ��豸������ʧ�ܣ�\n", g_nwk_my_ESN_address);
			FOUT;
			}
		}
	mlme = op_pk_create(0);
	op_pk_fd_set (mlme, 0, OPC_FIELD_TYPE_INTEGER, ASSOCIATE, 8);
	op_pk_fd_set (mlme, 1, OPC_FIELD_TYPE_INTEGER, g_nwk_father_table.NWKAddress, 16);
	op_pk_fd_set (mlme, 2, OPC_FIELD_TYPE_INTEGER, g_nwk_father_table.PanID, 8);
	op_pk_fd_set (mlme, 3, OPC_FIELD_TYPE_INTEGER, g_nwk_father_table.Band, 8);
	op_pk_fd_set (mlme, 4, OPC_FIELD_TYPE_INTEGER, g_nwk_father_table.Frequency, 8);
	op_pk_fd_set (mlme, 5, OPC_FIELD_TYPE_INTEGER, flag, 8);
	//op_pk_send(mlme, OUT_NWK_TO_MLME);
	if(g_nwk_test_type == 4 || flag == 2)
		{
		op_pk_send_delayed(mlme, OUT_NWK_TO_MLME, 0.1);
		if(g_nwk_join_time == 0)	g_nwk_join_time = op_sim_time();
		}
	else 
		{
		op_pk_send_delayed(mlme, OUT_NWK_TO_MLME, op_dist_uniform(20) * 20);
		if(g_nwk_join_time == 0)	g_nwk_join_time = op_sim_time();
		}
	//printf("Node %d send a associate to MAC, father = %d, band = %d\n", g_nwk_my_ESN_address, g_nwk_father_table.NWKAddress, g_nwk_father_table.Band);
	////op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d send a associate to MAC, father = %d, band = %d\n", g_nwk_my_ESN_address, g_nwk_father_table.NWKAddress, potentialParent.father[g_nwk_my_ESN_address].father[index].band);
	FOUT;
	}
/*
	��MAC��������ԭ��
*/
static void nwk_leave_request(int flag)
{
	int len = 1;
	Packet* mlme;
	FIN(nwk_leave_request(int flag));
	mlme = op_pk_create(0);
	op_pk_fd_set (mlme, 0, OPC_FIELD_TYPE_INTEGER, LEAVE, 8);
	op_pk_fd_set (mlme, 1, OPC_FIELD_TYPE_INTEGER, flag, 8);
	op_pk_send_delayed(mlme, OUT_NWK_TO_MLME, 0);
	//op_prg_log_entry_write(g_nwk_debugger_log_handle, "waiting leaving\n");
	FOUT;
}
/*
	�յ�����ȷ��ԭ��
*/
static int nwk_leave_confirm(int res)
{
	FIN(nwk_leave_confirm(int res));
	g_nwk_macLeaveFlag[res - 1] = 1;
	if(g_nwk_macLeaveFlag[0] == 1 && g_nwk_macLeaveFlag[1] == 1)//����MACȫ������
		{
		g_nwk_macLeaveFlag[0] = 0;
		g_nwk_macLeaveFlag[1] = 0;
		//op_prg_log_entry_write(g_nwk_debugger_log_handle, "recev all leave mlme\n", res);
		FRET(1);
		}
	FRET(0);
}

/**/
static void nwk_beacon_confirm(int res)
{
	FIN(nwk_beacon_confirm(int res));
	if(res == 0)
		{
		//�յ��ű�ȷ��ԭ��
		g_nwk_beacon_confirm = 0;
		}
	if(res == 1)
		{
		g_nwk_beacon_confirm++;
		if(g_nwk_beacon_confirm >= NWK_COUNT_MONITOR && g_nwk_main_router_father > 0)
			{
			nwk_master_inactivated_process();
			}
		}
	FOUT;
}

/*
	��MAC����ͬ�����к�����ԭ��
*/
static void nwk_syn_seq_set_MAC()
{
	int len = 2;
	Packet* mlme;
	FIN(nwk_syn_seq_set_MAC());
	mlme = op_pk_create(0);
	op_pk_fd_set (mlme, 0, OPC_FIELD_TYPE_INTEGER, SYN_SEQ_SET, 8);
	op_pk_fd_set (mlme, 1, OPC_FIELD_TYPE_INTEGER, g_nwk_syn_seq, 8);
	op_pk_send(mlme, OUT_NWK_TO_MLME);
	//op_prg_log_entry_write(g_nwk_debugger_log_handle, "set mac syn seq %d\n", g_nwk_syn_seq);
	FOUT;
}

/*
	���յ�MAC��ͬ�����к�ԭ�����
*/
static void nwk_syn_seq_comfirn(int seq)
{
	FIN(nwk_syn_seq_comfirn(int seq));
	printf("Node %d recv a mlme syn_seq_set seq = %d, local_seq = %d\n", g_nwk_my_ESN_address, seq, g_nwk_syn_seq);
	if(g_nwk_backup_router > 0 && seq == g_nwk_syn_seq + 1)
		{
		
		g_nwk_synchronize_queue.p_start++;
		//printf("Node %d p_start = %d\n", g_nwk_my_ESN_address, g_nwk_synchronize_queue.p_start);
		if(g_nwk_synchronize_queue.p_start >= NWK_QUEUE_LEN)
			{
			g_nwk_synchronize_queue.p_start = 0;
			}
		g_nwk_syn_seq++;
		g_nwk_synchronize_queue.number--;
		g_nwk_synchronize_queue.count = NWK_COUNT_BACKUP;
		if(g_nwk_synchronize_queue.number > 0)
			{
			nwk_send_synchronize_queue();
			}
		}
	FOUT;
}
/*
	���յ�������ԭ��Ĵ���
*/
static void nwk_keep_alive(int shortAddress, int res, int flag)
{
	Packet* pk;
	int len, pklen;
	int dest;
	FIN(nwk_keep_alive(int shortAddress, int res, int flag));
	printf("shortAddress = %d,res = %d flag = %d\n", shortAddress, res, flag);
	//op_prg_log_entry_write(g_nwk_debugger_log_handle, "shortAddress = %d,res = %d\n", shortAddress, res);
	if(flag == 1)
		{
		dest = g_nwk_query_r2b_map(shortAddress);
		if(dest == -1)
			{
			printf("dest = %d, do not need response\n", dest);
			FOUT;
			}
		
		if(res == 1) 
			{
			res = 0x7f;
			}
		nwk_send_master_status(dest, 1, -1, res);
		FOUT;
		}
	if(res == SUCCESSFUL && shortAddress != g_nwk_my_father_short_address)//����ɹ������ʾĿ��ڵ�·�ɶ�ʧ
		{
		//����·�ɱ�״̬
		UpdateRT(shortAddress, NWK_ROUTING_TABLE_Active);
		FOUT;
		//�������״̬����
		len = 3;
		pk = op_pk_create(0);
		op_pk_fd_set (pk, 0, OPC_FIELD_TYPE_INTEGER, shortAddress, 16);
		op_pk_fd_set (pk, 1, OPC_FIELD_TYPE_INTEGER, 0, 8);
		pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
		pklen += len*8;
		nwk_generate_packet(pklen, 0, 1, 2, 1, g_nwk_my_short_address, GateAddress, 10, NWK_MAX_RADIUS - 1, len, pk, OUT_NWK_TO_MCPS_UP, 0);//��������״̬����֪ͨ����
		}
	else if(shortAddress == g_nwk_my_father_short_address && g_nwk_node_type != GATENODE)//�븸�ڵ���·ʧ��
		{
		if(g_nwk_node_type == BACKUPNODE)
			{
			//��������֤
			nwk_send_master_status(g_nwk_main_router_father, 0, g_nwk_main_address, -1);
			}
		else	nwk_leave_net(g_nwk_my_short_address, 1, 0, 0);
		}
	/*
	else if(queryRTIndex(shortAddress) >= 0 && 1 == 1)//��·�ɽڵ�ʧ�����·�ɽڵ��б��ݽڵ�
		{
		nwk_send_master_status_req(shortAddress, );
		}
	*/
	else//���ӽڵ㱣��ʧ����Ҫ����·�ɱ����ӽڵ���ҷ���·��ɾ������֪ͨ����
		{
		DeleteCT(2, shortAddress);
		UpdateRT(shortAddress, NWK_ROUTING_TABLE_DELETED);
		}
	FOUT;
}

/*
	Ҫ��MAC������Ŀ�Ľڵ㱣���ԭ��
*/
static void nwk_keep_alive_request(int shortAddress, int flag)
{
	Packet* mlme;
	FIN(nwk_keep_alive_request(int shortAddress, int flag));
	mlme = op_pk_create(0);
	op_pk_fd_set (mlme, 0, OPC_FIELD_TYPE_INTEGER, ALIVE, 8);
	op_pk_fd_set (mlme, 1, OPC_FIELD_TYPE_INTEGER, shortAddress, 16);
	op_pk_fd_set (mlme, 2, OPC_FIELD_TYPE_INTEGER, flag, 8);
	//printf("flag = %d\n", flag);
	op_pk_send(mlme, OUT_NWK_TO_MLME);
	FOUT;
}




/*
	��������������飬��MAC����MLME-DATA.request��ͬʱ����һ��������ʱ��
*/
static void nwk_join_request()
	{
	int pklen = 16 * 8;
	int len;
	int info;
	Packet* mcps;
	FIN(nwk_join_request());
	info = (g_nwk_join_type<<6) | g_nwk_capability;
	if(g_nwk_join_type == 0)
		{
		len = 7;
		mcps = op_pk_create(0);
		op_pk_fd_set (mcps, 0, OPC_FIELD_TYPE_INTEGER, g_nwk_my_ESN_address, 48);
		op_pk_fd_set (mcps, 1, OPC_FIELD_TYPE_INTEGER, g_nwk_join_type, 2);
		op_pk_fd_set (mcps, 2, OPC_FIELD_TYPE_INTEGER, info, 6);
		}
	else
		{
		if(g_nwk_node_type == ROUTERNODE)
			{
			len = 10;
			mcps = op_pk_create(0);
			op_pk_fd_set (mcps, 0, OPC_FIELD_TYPE_INTEGER, g_nwk_my_ESN_address, 48);
			op_pk_fd_set (mcps, 1, OPC_FIELD_TYPE_INTEGER, g_nwk_join_type, 2);
			op_pk_fd_set (mcps, 2, OPC_FIELD_TYPE_INTEGER, info, 6);
			op_pk_fd_set (mcps, 3, OPC_FIELD_TYPE_INTEGER, g_nwk_my_short_address, 16);
			op_pk_fd_set (mcps, 4, OPC_FIELD_TYPE_INTEGER, g_nwk_myBand, 8);
			}
		else if(g_nwk_node_type == ENDNODE || g_nwk_node_type == BACKUPNODE)
			{
			len = 9;
			mcps = op_pk_create(0);
			op_pk_fd_set (mcps, 0, OPC_FIELD_TYPE_INTEGER, g_nwk_my_ESN_address, 48);
			op_pk_fd_set (mcps, 1, OPC_FIELD_TYPE_INTEGER, g_nwk_join_type, 2);
			op_pk_fd_set (mcps, 2, OPC_FIELD_TYPE_INTEGER, info, 6);
			op_pk_fd_set (mcps, 3, OPC_FIELD_TYPE_INTEGER, g_nwk_my_short_address, 16);
			}
		}
	pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LONGADDR_LEN + LENGTH_LEN;
	pklen += len*8;
	if(g_nwk_test_type == 4)
		{
		nwk_generate_packet(pklen,0,1,3,2,g_nwk_my_ESN_address,g_nwk_my_father_short_address,0,0,len,mcps,OUT_NWK_TO_MCPS_UP, 0);
		}
	else 
		{
		nwk_generate_packet(pklen,0,1,3,2,g_nwk_my_ESN_address,g_nwk_my_father_short_address,0,0,len,mcps,OUT_NWK_TO_MCPS_UP, 1 + op_dist_uniform(NWK_MAX_CHILD_NUM*0.5));
		}
	printf("Node %d send a joinSeq!type = %d father = %d!\n", g_nwk_my_ESN_address, g_nwk_join_type, g_nwk_my_father_short_address);
	//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d send a joinSeq!type = %d!\n", g_nwk_my_ESN_address, g_nwk_join_type);
	op_intrpt_schedule_self(op_sim_time() + NWK_TIMER_JOIN,intrCode_Join);
	FOUT;
	}

/*
	����һ��֡��ͨ��MCPSԭ�﷢�͸�MAC
*/
static void nwk_generate_packet(int pklen,int multicast,int ar,int sourceMode,int destMode,int source,int dest,int type,int TTL,int NWKLen,Packet* PayLoad,int streamPort,double delay)
{
	Packet* pkptr;
	int frameControl = 10752|(multicast<<8)|(ar<<10);
	int NWKControl = 0;
	int id;
	FIN(nwk_generate_packet(int pklen,int multicast,int ar,int sourceMode,int destMode,int source,int dest,int type,int TTL,int NWKLen,Packet* PayLoad,int streamPort,double delay));
	if(streamPort == OUT_NWK_TO_MCPS_UP && g_nwk_mulcast_maintain > 0)
		{
		frameControl |= (1<<6);
		pklen += IES_LEN;
		NWKLen += (IES_LEN / 8);
		pkptr = op_pk_create (0);
		op_pk_fd_set (pkptr,6,OPC_FIELD_TYPE_INTEGER,g_nwk_mulcast_maintain,IES_LEN);
		////op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d send a pk with g_nwk_mulcast_maintain = %d\n", g_nwk_my_ESN_address, g_nwk_mulcast_maintain);
		}
	else	pkptr = op_pk_create (0);
	frameControl |= sourceMode;
	frameControl |= (destMode<<4);
	//set framControl,index = 0
	op_pk_fd_set (pkptr, 0, OPC_FIELD_TYPE_INTEGER, frameControl, FRAMCONTR_LEN);
	//set MACSequence,index = 1
	op_pk_fd_set (pkptr, 1, OPC_FIELD_TYPE_INTEGER, 0, MACSEQ_LEN);
	//set PANID,index = 2
	op_pk_fd_set (pkptr, 2, OPC_FIELD_TYPE_INTEGER, g_nwk_PANID, PANID_LEN);
	//set destAddress,index = 3
	if(destMode == 1)	op_pk_fd_set (pkptr,3,OPC_FIELD_TYPE_INTEGER,0,0);
	else if(destMode == 3)	op_pk_fd_set (pkptr,3,OPC_FIELD_TYPE_INTEGER,dest, LONGADDR_LEN);
	else op_pk_fd_set (pkptr,3,OPC_FIELD_TYPE_INTEGER,dest, SHORTADDR_LEN);
	//set srcAddress,index = 4
	if(sourceMode == 1)	op_pk_fd_set (pkptr,4,OPC_FIELD_TYPE_INTEGER,0,0);
	else if(sourceMode == 3)	op_pk_fd_set (pkptr,4,OPC_FIELD_TYPE_INTEGER,source,LONGADDR_LEN);
	else op_pk_fd_set (pkptr,4,OPC_FIELD_TYPE_INTEGER,source,SHORTADDR_LEN);
	op_pk_fd_set (pkptr,5,OPC_FIELD_TYPE_INTEGER,NWKLen + 2,8);
	//set NWKControl,index = 7
	NWKControl |= (type<<12);
	NWKControl |= (TTL<<8);
	op_pk_fd_set (pkptr,7,OPC_FIELD_TYPE_INTEGER,NWKControl,16);
	//set NWKPayLoad,index = 8
	op_pk_fd_set (pkptr,8,OPC_FIELD_TYPE_PACKET,PayLoad,NWKLen*8);
	id = op_pk_id(pkptr);
	op_pk_send_delayed(pkptr,streamPort,delay);
	//printf("====>pk id = %d\n", id);
	////op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d send a pk to %d, len = %d, packet id = %d\n", g_nwk_my_ESN_address, dest, op_pk_total_size_get(pkptr), id);
	FOUT;
}
/*
	����·�ɱ�
*/
static int nwk_get_nexthop(int dest)
{
	int i = 0;
	int router, gate;
	FIN(nwk_get_nexthop(int dest));
	gate = dest&(1<<15);
	if(gate == 1)
		{
		FRET(dest);
		}
	router = dest&(255<<8);
	if(router == g_nwk_my_short_address)
		{
		i = CTinclude(dest);
		//printf("-----------i = %d\n", i);
		if(i >= 0 && Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].status == NWK_CHILD_TABLE_LEGITIMATE)
			{
			//printf("---------status = %d\n", Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].status);
			dest = Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].NWKAddress;
			FRET(dest);
			}
		else	FRET(-1);
		}
	for(i = 0;i < g_nwk_routing_table.number;i++)
	{
		if(((g_nwk_routing_table.table[i].destAddress)<<8) == router && g_nwk_routing_table.table[i].status != NWK_ROUTING_TABLE_DELETED)
		{
			FRET((g_nwk_routing_table.table[i].nextHop)<<8);
		}
	}
	FRET(-1);
}


/*
	ת������
*/
static void nwk_forward_packet(Packet* pkptr,int streamPort)
{
	int NWKControl;
	int TTL;
	FIN(nwk_forward_packet(Packet* pkptr));
	op_pk_fd_get(pkptr,7,&NWKControl);
	TTL = 15&(NWKControl>>8);
	NWKControl ^= (TTL<<8);
	TTL--;
	if(TTL<0)
	{
		op_pk_destroy(pkptr);
		printf("PK was destroyed becouse of TTL down!\n");
		//op_prg_log_entry_write(g_nwk_debugger_log_handle, "PK was destroyed becouse of TTL down!\n");
		FOUT;
	}
	NWKControl |= (TTL<<8);
	op_pk_fd_set(pkptr,7,OPC_FIELD_TYPE_INTEGER,NWKControl,16);
	op_pk_send(pkptr,streamPort);
	//op_stat_write(contr_send_num,++sendNum);
	////op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d nwk_forward_packet !\n\n",g_nwk_my_ESN_address);
	FOUT;
}


/*
	�����������Ľ��մ���
*/
static void nwk_process_Join_request(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int ESNAddress;
	int join;//��������
	int l_nwk_capabilityInformation;
	int deviceType;
	
	int control;
	int NWKAddress = 0;
	int band = -1;
	int frequency;
	
	int nexthop;
	int shortAddress = -1;
	int pklen;
	Packet* pk;
	FIN(nwk_process_Join_request(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	op_pk_fd_get(payLoad,0,&ESNAddress);
	op_pk_fd_get(payLoad,1,&join);
	op_pk_fd_get(payLoad,2,&l_nwk_capabilityInformation);
	deviceType = ((l_nwk_capabilityInformation>>4) & 3);
	if(join != 0)//�������Ͳ���Э������
		{
		op_pk_fd_get(payLoad,3,&shortAddress);
		if(deviceType == ROUTERNODE)	op_pk_fd_get(payLoad,4,&band);
		}
	/*
		�Ǵ�����������
	*/
	if(destMode == 2 && sourceMode == 3)
		{
		//point = distPoint();
		if(g_nwk_node_type == ROUTERNODE)
			{
			NWKAddress = nwk_distribute_address(deviceType, shortAddress, ESNAddress);//�����ַ
			//op_prg_log_entry_write(g_nwk_debugger_log_handle, "join = %d, device = %d\n",join, deviceType);
			
			if(NWK_JOIN_CONTROL == 1)//�Ƿ���������������,0��,1��
				{
				if(get_illegal_CT_number() > NWK_MAX_JOIN_COUNT)
					{
					pk = op_pk_create(0);
					control = 0xa0;
					op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,control,48);
					len = 1;
					pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
					pklen += len*8;
					nwk_generate_packet(pklen,0,1,2,3,g_nwk_my_short_address,source,1,0,len,pk,OUT_NWK_TO_MCPS_DOWN,0);//�����ݻ������Ļظ�
					printf("Node %d �������������Ժ�����\n", g_nwk_my_ESN_address);
					FOUT;
					}
				}
			
			//����������������
			if(join == 0 && deviceType == ENDNODE)
				{
				pk = op_pk_create(0);
				op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,ESNAddress,48);
				op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,join,2);
				op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER,l_nwk_capabilityInformation,6);
				op_pk_fd_set(pk,3,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
				len = 9;
				}
			else if(join == 0 && deviceType == ROUTERNODE)
				{
				pk = op_pk_create(0);
				op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,ESNAddress,48);
				op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,join,2);
				op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER,l_nwk_capabilityInformation,6);
				len = 7;
				}
			else if(join != 0 && deviceType == ENDNODE)
				{
				pk = op_pk_create(0);
				op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,ESNAddress,48);
				op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,join,2);
				op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER,l_nwk_capabilityInformation,6);
				op_pk_fd_set(pk,3,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
				len = 9;
				}
			else if(join != 0 && deviceType == ROUTERNODE)
				{
				pk = op_pk_create(0);
				op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,ESNAddress,48);
				op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,join,2);
				op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER,l_nwk_capabilityInformation,6);
				op_pk_fd_set(pk,3,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
				op_pk_fd_set(pk,4,OPC_FIELD_TYPE_INTEGER,band,8);
				len = 10;
				}
			else if(join == 0 && deviceType == BACKUPNODE)
				{
				pk = op_pk_create(0);
				op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,ESNAddress,48);
				op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,join,2);
				op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER,l_nwk_capabilityInformation,6);
				op_pk_fd_set(pk,3,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
				len = 9;
				}
			else
				{
				printf("Node %d received a error joinreq! \n", g_nwk_my_ESN_address);
				//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d received a error joinreq! \n", g_nwk_my_ESN_address);
				FOUT;
				}
			pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
			pklen += len*8;
			nwk_generate_packet(pklen,0,1,2,1,g_nwk_my_short_address,GateAddress,0,NWK_MAX_RADIUS-1,len,pk,OUT_NWK_TO_MCPS_UP,0);//���ʹ�����������
			generateCT(ESNAddress,NWKAddress,l_nwk_capabilityInformation, NWK_CHILD_TABLE_ILLEGAL);
			//op_stat_write(contr_send_num,++sendNum);
			//printf("Node %d send a sourceSeq!!!\n",g_nwk_my_ESN_address);
			////op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d send a sourceSeq!!!\n",g_nwk_my_ESN_address);
			}
		else//���ؽڵ㴦��Ǵ�����������
			{
			//printf("deviceType = %d\n",deviceType);
			
			NWKAddress = nwk_distribute_address(deviceType, shortAddress, ESNAddress);//�����ַ
			if(NWKAddress == -1)
				{
				printf("NWKAddress distr failt!\n");
				//op_prg_log_entry_write(g_nwk_debugger_log_handle, "NWKAddress distr failt!\n");
				FOUT;
				}
			
			if(deviceType == ROUTERNODE)
				{
				frequency = nwk_distribute_frequency(2);//����Ƶ��
				//The gateway node establishes a routing table for the directly connected router and sends a confirm
				generateRT(NWKAddress,NWKAddress,0,0);
				//Assign band
				band = nwk_distribute_band(NWKAddress);
				control = 31;
				control |= (g_nwk_join_type<<5);
				pk = op_pk_create(0);//create a accessConfirm_payLoad
				op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,control,8);
				op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,ESNAddress,48);
				op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER,0,8);//RouterSequence
				op_pk_fd_set(pk,3,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
				op_pk_fd_set(pk,4,OPC_FIELD_TYPE_INTEGER,band,8);
				op_pk_fd_set(pk,5,OPC_FIELD_TYPE_INTEGER,frequency,8);
				len = 12;
				}
			else if(deviceType == ENDNODE)
				{
				frequency = nwk_distribute_frequency(0);//����Ƶ��
				control = 26;
				control |= (g_nwk_join_type<<5);
				pk = op_pk_create(0);//create a accessConfirm_payLoad
				op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,control,8);
				op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INT64,ESNAddress,48);
				op_pk_fd_set(pk,3,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
				op_pk_fd_set(pk,5,OPC_FIELD_TYPE_INTEGER,frequency,8);
				len = 10;
				}
			else
				{
				printf("Node %d received a error joinreq! \n", g_nwk_my_ESN_address);
				//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d received a error joinreq! \n", g_nwk_my_ESN_address);
				FOUT;
				}
			pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN  + LONGADDR_LEN + LENGTH_LEN;
			pklen += len*8;
			nwk_generate_packet(pklen,0,1,1,3,GateAddress,source,1,0,len,pk,OUT_NWK_TO_MCPS_DOWN,0);//���ͷǴ�������ȷ��
			if(join == 2)//���ȼ��ͣ���ʱ��������
				{
				
				
				//��Ҫ���·��.
				
				
				}
			//op_stat_write(contr_send_num,++sendNum);
			//printf("Node %d send a accessConfirm!!!\n",g_nwk_my_ESN_address);
			////op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d send a accessConfirm!!!\n",g_nwk_my_ESN_address);
			//Build the mapping table
			nwk_update_mapping(ESNAddress, NWKAddress);
			//printf("mapping[%d]= %d\n",ESNAddress-1,mapping[ESNAddress]);
			generateCT(ESNAddress,NWKAddress,l_nwk_capabilityInformation, NWK_CHILD_TABLE_LEGITIMATE);
			}
		}
	
	/*
		������������
	*/
	else
		{
		//printf("Node %d received a agent joinSeq\n", g_nwk_my_ESN_address);
		////op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d received a agent joinSeq\n", g_nwk_my_ESN_address);
		if(g_nwk_node_type != GATENODE)	FOUT;//ֻ�����ؽڵ��ܹ����մ��������������
		frequency = nwk_distribute_frequency(1);
		NWKAddress = nwk_distribute_address(deviceType, shortAddress, ESNAddress);//Assign network address;
		if(deviceType==ROUTERNODE)
			{
			nexthop = nwk_get_nexthop(source);
			//printf("------------nexthop = %d\n",nexthop);
			//op_prg_log_entry_write(g_nwk_debugger_log_handle, "------------nexthop = %d\n",nexthop);
			if(nexthop <= 0) FOUT;
			//Assign band
			band = nwk_distribute_band(NWKAddress);
			if(band == -1)
				{
				printf("band = -1\n\n\n\n");
				}
			generateRT(NWKAddress,nexthop,0,0);//Ϊ������·�ɽڵ㽨��·�ɱ�
			len = 12;
			control = 159;
			control |= (g_nwk_join_type<<5);
			pk = op_pk_create(0);//create a accessConfirm_payLoad
			op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,control,8);
			op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,ESNAddress,48);
			op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER,0,8);
			op_pk_fd_set(pk,3,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
			op_pk_fd_set(pk,4,OPC_FIELD_TYPE_INTEGER,band,8);
			op_pk_fd_set(pk,5,OPC_FIELD_TYPE_INTEGER,frequency,8);
			//Build the mapping table
			nwk_update_mapping(ESNAddress, NWKAddress);
			//mapping[ESNAddress].shortAddress = NWKAddress;//Build the mapping table
			//mapping[ESNAddress].count = NWK_COUNT_MAPPING_TABLE;
			printf("mapping[%d]= %d\n",ESNAddress,mapping[ESNAddress].shortAddress);
			//op_prg_log_entry_write(g_nwk_debugger_log_handle, "mapping[%d]= %d\n",ESNAddress,mapping[ESNAddress].shortAddress);
			}
		else
			{
			op_pk_fd_get(payLoad,3,&shortAddress);
			len = 10;
			control = 146;
			control |= (g_nwk_join_type<<5);
			pk = op_pk_create(0);//create a accessConfirm_payLoad
			op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,control,8);
			op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,ESNAddress,48);
			op_pk_fd_set(pk,5,OPC_FIELD_TYPE_INTEGER,frequency,8);
			//Build the mapping table
			nwk_update_mapping(ESNAddress, shortAddress);
			//mapping[ESNAddress].shortAddress = shortAddress;
			//mapping[ESNAddress].count = NWK_COUNT_MAPPING_TABLE;
			printf("mapping[%d]= %d\n",ESNAddress,mapping[ESNAddress].shortAddress);
			}
		pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
		pklen += len*8;
		nwk_generate_packet(pklen,0,1,1,2,GateAddress,source,1, NWK_MAX_RADIUS-1, len,pk,OUT_NWK_TO_MCPS_DOWN,0);//���ʹ�������ȷ��
		if(join == 2)//���ȼ��ͣ���ʱ��������
				{
				
				
				//��Ҫ���·��.
				
				
				}
		//op_stat_write(contr_send_num,++sendNum);
		//printf("Node %d send a accessConfirm!!!\n",g_nwk_my_ESN_address);
		////op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d send a accessConfirm!!!\n",g_nwk_my_ESN_address);
		}
	FOUT;
}


/*
	����ȷ�ϵĽ��մ���

	����ֵ: 0 ����ȷ�Ϸ��鴦��ɹ�, 1 �յ��ӳ�����������ȷ��, -1 ����ʧ��
*/

static int nwk_process_access_confirm(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int permit,shortAddrComp,bandComp,frequencyComp,routerSeqComp,join;
	int ESNAddress,accessContr,NWKAddress,routerSeq,band;
	int pklen;
	int frequency;
	int nexthop;
	
	Packet* pk;
	FIN(nwk_process_access_confirm(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	
	op_pk_fd_get(payLoad,0,&accessContr);
	//printf("accessContr = %d\n", accessContr);
	//op_prg_log_entry_write(g_nwk_debugger_log_handle, "accessContr = %d\n", accessContr);
	op_pk_fd_get(payLoad,5,&frequency);
	permit = 1&(accessContr>>4);
	join = 3&(accessContr>>5);
	shortAddrComp = 1&(accessContr>>3);
	bandComp = 1&(accessContr>>2);
	frequencyComp = 1&(accessContr>>1);
	//printf("Node %d receive a accessConfirm pk dest = %d\n",g_nwk_my_ESN_address,dest);
	////op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d receive a accessConfirm pk dest = %d\n",g_nwk_my_ESN_address,dest);
	if(permit == 1)
	{
		//��������ȷ��
		if(destMode == 2&&dest == g_nwk_my_short_address)
		{
			//printf("Node %d received a agent access\n", g_nwk_my_ESN_address);
			////op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d received a agent access\n", g_nwk_my_ESN_address);
			op_pk_fd_get(payLoad,1,&ESNAddress);
			if(shortAddrComp == 1)//�������ڵ���·�ɽڵ�
				{
				op_pk_fd_get(payLoad,3,&NWKAddress);
				op_pk_fd_get(payLoad,2,&routerSeq);
				op_pk_fd_get(payLoad,4,&band);
				if(routerSeq >= 0)		generateRT(NWKAddress,NWKAddress,0,routerSeq);
				else					generateRT(NWKAddress,NWKAddress,0,0);
				len = 6;
				pk = op_pk_create(0);
				op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,accessContr,8);
				op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER,routerSeq,8);
				op_pk_fd_set(pk,3,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
				op_pk_fd_set(pk,4,OPC_FIELD_TYPE_INTEGER,band,8);
				op_pk_fd_set(pk,5,OPC_FIELD_TYPE_INTEGER,frequency,8);
				}
			//printf("---------------------------NWKAddress = %d\n",NWKAddress);
			//printf("------------------------------ENDNWKAddress = %d\n",NWKAddress);
			else//�������ڵ����ն��豸
				{
				NWKAddress = CTNWKAddresss(ESNAddress);
				//printf("Node %d NWKAddress dist = %d\n", g_nwk_my_ESN_address, NWKAddress);
				//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d NWKAddress dist = %d\n", g_nwk_my_ESN_address, NWKAddress);
				if(NWKAddress <= 0)
					{
					printf("Node %d NWKAddress dist error\n", g_nwk_my_ESN_address);
					//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d NWKAddress dist error\n", g_nwk_my_ESN_address);
					FRET(-1);
					}
				accessContr |= (1<<3);
				len = 4;
				pk = op_pk_create(0);
				op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,accessContr,8);
				op_pk_fd_set(pk,3,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
				op_pk_fd_set(pk,5,OPC_FIELD_TYPE_INTEGER,frequency,8);
				}
			pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LONGADDR_LEN + LENGTH_LEN;
			pklen += len*8;
			nwk_generate_packet(pklen,0,1,2,3,g_nwk_my_short_address,ESNAddress,1,0,len,pk,OUT_NWK_TO_MCPS_DOWN,0);//���ͷǴ�������ȷ��
			//printf("Node %d send a accessConfirm pk dest = %d\n",g_nwk_my_ESN_address,ESNAddress);
			//op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d send a accessConfirm pk dest = %d\n",g_nwk_my_ESN_address,ESNAddress);
			//op_stat_write(contr_send_num,++sendNum);
			UPCTStatus(ESNAddress,NWKAddress);
			
			//printf("UPCTStatus\n");
			//op_prg_log_entry_write(g_nwk_debugger_log_handle, "UPCTStatus\n");
		}
		else if(destMode == 2 && shortAddrComp == 1)//�м�·�ɵĽ��մ���
			{
			printf("Node %d receive a accessConfirm but dest is not child\n", g_nwk_my_ESN_address);
			//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d receive a accessConfirm but dest is not child\n", g_nwk_my_ESN_address);
			op_pk_fd_get(payLoad,1,&ESNAddress);
			op_pk_fd_get(payLoad,3,&NWKAddress);
			op_pk_fd_get(payLoad,2,&routerSeq);
			op_pk_fd_get(payLoad,4,&band);
			nexthop = nwk_get_nexthop(dest);
			if(nexthop < 0)	FRET(-1);
			if(routerSeq >= 0)		generateRT(NWKAddress,nexthop,0,routerSeq);
			else					generateRT(NWKAddress,nexthop,0,0);
			}
		//�Ǵ�������ȷ��
		else if(destMode == 3 && dest == g_nwk_my_ESN_address)
		{
			//printf("Node %d received a not agent access\n", g_nwk_my_ESN_address);
			//op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d received a not agent access\n", g_nwk_my_ESN_address);
			routerSeqComp = 1&accessContr;
			//printf("Node %d shortAddrComp = %d\n", g_nwk_my_ESN_address, shortAddrComp);
			////op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d shortAddrComp = %d\n", g_nwk_my_ESN_address, shortAddrComp);
			if(shortAddrComp==1)	
				{
				op_pk_fd_get(payLoad,3,&NWKAddress);
				g_nwk_my_short_address = NWKAddress;
				op_ima_obj_attr_set(g_nwk_nodeID,"g_node_short_address",g_nwk_my_short_address);
				}
			if(bandComp==1)			
				{
				op_pk_fd_get(payLoad,4,&band);
				g_nwk_myBand = band;
				op_ima_obj_attr_set(g_nwk_nodeID,"g_node_band",g_nwk_myBand);
				}
			if(frequencyComp==1)		
				{
				op_pk_fd_get(payLoad,5,&frequency);
				g_nwk_my_frequency = frequency;
				op_ima_obj_attr_set(g_nwk_nodeID,"g_node_frequency",g_nwk_my_frequency);
				}
			if(routerSeqComp==1)	
				{
				op_pk_fd_get(payLoad,2,&routerSeq);
				g_nwk_my_routing_sequence = routerSeq;
				}
			if(join == g_nwk_join_type && NWKAddress != g_nwk_my_short_address)
				{
				if(join == 1)
					{
					
					}
				else if(join == 2)
					{
					
					}
				}
			
			/*
			
			����õ���Դ֪ͨMAC��
			
			*/
			op_intrpt_disable(OPC_INTRPT_SELF, intrCode_Join, OPC_TRUE);//�ܾ���һ��������ʱ���ж�
			
			//ͳ�������ڵ�����
			if(onlineNodes[g_nwk_my_ESN_address] == 0)
				{
				onlineNodes[g_nwk_my_ESN_address] = 1;
				onlineNodesNUmber += 1;
				op_stat_write (online_number_gstathandle, 		onlineNodesNUmber);
				}
			if(g_nwk_test_type == 4) 
				{
				g_nwk_join_time = op_sim_time() - g_nwk_join_time;
				op_stat_write(join_time_lstathandle, g_nwk_join_time);
				}
			
			//printf("----------------\n");
			if(g_nwk_node_type == ROUTERNODE)
				{
				printf("NodeBand = %d       ",g_nwk_myBand);
				//op_prg_log_entry_write(g_nwk_info_log_handle, "NodeBand = %d       ",g_nwk_myBand);
				}
			printf("g_nwk_my_short_address = %d, g_nwk_my_frequency = %d\n",g_nwk_my_short_address, g_nwk_my_frequency);
			//op_prg_log_entry_write(g_nwk_info_log_handle, "g_nwk_my_short_address = %d, g_nwk_my_frequency = %d\n",g_nwk_my_short_address, g_nwk_my_frequency);
			
			if(g_nwk_node_type == BACKUPNODE && g_nwk_my_father_short_address != g_nwk_main_address)		
				{
				op_prg_log_entry_write(g_nwk_info_log_handle, "backup router online\n");
				op_intrpt_schedule_self(op_sim_time() + 20 + op_dist_uniform (10),intrCode_ChangeMainRouter);//���ݵȴ���Ѱ����·��
				}
			else if(g_nwk_node_type == BACKUPNODE && g_nwk_my_father_short_address == g_nwk_main_address)
				{
				// ֪ͨ��·�ɱ��ڵ�ı��ݽڵ����
				
				pk = op_pk_create(0);
				op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,3,8);
				pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + SHORTADDR_LEN + LENGTH_LEN;
				len = 1;
				pklen += 8*len;
				nwk_generate_packet(pklen,0,1,2,2,g_nwk_my_short_address,g_nwk_main_address,12,1, len, pk, OUT_NWK_TO_MCPS_UP,0);//������·����Ϣ�ظ�
				printf("Node %d send a notify to main router\n",g_nwk_my_ESN_address);
		
				
				}
		}	
	}
	//Rejection is not considered in the simulation
	else
	{
		if(join == 1)//�յ�����ڵ�ϣ����ʱ������ָʾ������ȴ�
			{
			op_intrpt_disable(OPC_INTRPT_SELF, intrCode_Join, OPC_TRUE);//�ܾ���һ��������ʱ���ж�
			printf("Node %d permit == 0\n", g_nwk_my_ESN_address);
			FRET(1);
			}
		printf("permit == 0\n");
		FRET(-1);
		//op_prg_log_entry_write(g_nwk_debugger_log_handle, "permit == 0\n");
	}
	FRET(0);;
}



/*
	·��ά������Ľ��մ���
*/
static void nwk_process_routing(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int control, routerSeq, shortAddress;
	int pklen;
	int index;
	int model;
	Packet* pk;
	FIN(nwk_process_routing(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	op_pk_fd_get(payLoad,0,&control);
	op_pk_fd_get(payLoad,2,&shortAddress);
	
	//op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d receive a routing pk ,control = %d, shortAddress\n", g_nwk_my_ESN_address, control, shortAddress);
	if(control == 0)//·�ɻظ��Ľ��մ���
		{
		UpdateRT(shortAddress, NWK_ROUTING_TABLE_Active);
		}
	else if(control == 4)
		{
		printf("Node %d receive a routing pk ,control = %d, shortAddress = %d\n", g_nwk_my_ESN_address, control, shortAddress);
		UpdateRT(shortAddress, NWK_ROUTING_TABLE_DELETED);
		}
	else if(control == 1)//·��ɾ���Ľ��մ���
		{
		op_pk_fd_get(payLoad,1,&routerSeq);
		if(routerSeq>=0)
			{
			printf("Node %d receive a routing pk ,control = %d, shortAddress = %d\n", g_nwk_my_ESN_address, control, shortAddress);
			deleteRouting(routerSeq,shortAddress);
			}
		if(g_nwk_node_type == GATENODE)
			{
			//��Դ�ݲ��ͷ�
			/*
			if(mapping[ESNAddress]==NWKAddress)
				{
				mapping[ESNAddress] = 0;
				}
			*/
			}
		}
	else if(control == 2)//·������Ľ��մ���
		{
		
		//����·�ɻظ�����
		if(dest == g_nwk_my_short_address)
			{
			index = queryRTIndex(shortAddress);
			if(index < 0) FOUT;
			if(g_nwk_my_short_address == shortAddress || (index >= 0 && g_nwk_routing_table.table[index].status != NWK_ROUTING_TABLE_DELETED))
				{
				control = 0;
				}
			else
				{
				control = 4;
				}
			
			//����·�ɻظ�����
			len = 3;
			pk = op_pk_create(0);
			op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,control,8);
			op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER,shortAddress,16);
			pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
			if(source == GateAddress)	
				{
				model = 1;
				}
			else
				{
				model = 2;
				pklen += SHORTADDR_LEN;
				}
			pklen += 8*len;
			nwk_generate_packet(pklen, 0, 1, 2, model, g_nwk_my_short_address, source, 11, 0 , len, pk, OUT_NWK_TO_MCPS_UP, 0);//����·��ά���ظ�
			//printf("Node %d send a router response pk for router %d!, control = %d, model = %d\n", g_nwk_my_ESN_address, shortAddress, control, model);
			//op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d send a router response pk for router %d!, control = %d, model = %d\n", g_nwk_my_ESN_address, shortAddress, control, model);
			}
		}
	FOUT;
}



/*
	�鲥���Ʒ���Ľ��մ���
*/
static void nwk_process_multicast_control(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int i;
	int MulticastInformation,type,number;
	int muticastID;
	Packet* mlme;
	FIN(nwk_process_multicast_control(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	op_pk_fd_get(payLoad,0,&MulticastInformation);
	type = 1&MulticastInformation;
	number = 15&(MulticastInformation>>1);
	//printf("Node %d receive a MulticastContr to dest=%d!\n",g_nwk_my_ESN_address,dest);
	//op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d receive a MulticastContr to dest=%d!\n",g_nwk_my_ESN_address,dest);
	//printf("%d\n",MulticastInformation);	
	//op_prg_log_entry_write(g_nwk_debugger_log_handle, "%d\n",MulticastInformation);	
	for(i = 0;i<number;i++)
	{
		op_pk_fd_get(payLoad,i+1,&muticastID);
		if(dest == g_nwk_my_short_address)
		{
			if(type == 1)	g_nwk_my_multicast_table |= (1<<(muticastID - 1));
			else			g_nwk_my_multicast_table &= (~(1<<(muticastID - 1)));
			//printf("Node %d add/subtract a muticastID = %d\n",g_nwk_my_ESN_address,muticastID);
			//op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d add/subtract a muticastID = %d\n",g_nwk_my_ESN_address,muticastID);
		}
		else if(g_nwk_node_type == ROUTERNODE)
		{
			if(type == 1)	g_nwk_children_multicast_table.group |= (1<<(muticastID - 1));
			//printf("Node %d add a muticastID = %d to ChildMT\n",g_nwk_my_ESN_address,muticastID);
			//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d add a muticastID = %d to ChildMT\n",g_nwk_my_ESN_address,muticastID);
		}
	}
	g_nwk_mulcast_maintain = (g_nwk_my_multicast_table | g_nwk_children_multicast_table.group);
	//printf("g_nwk_mulcast_maintain = %d\n", g_nwk_mulcast_maintain);
	//op_prg_log_entry_write(g_nwk_debugger_log_handle, "g_nwk_mulcast_maintain = %d\n", g_nwk_mulcast_maintain);
	/*
	ͨ��ԭ������MAC���鲥ά��
	*/
	len = 2;
	mlme = op_pk_create(0);
	op_pk_fd_set (mlme, 0, OPC_FIELD_TYPE_INTEGER, Multicast, 8);
	op_pk_fd_set (mlme, 1, OPC_FIELD_TYPE_INTEGER, g_nwk_mulcast_maintain, 16);
	op_pk_send(mlme, OUT_NWK_TO_MLME);
	FOUT;
}


/*
	��������Ľ��մ���
*/
static void nwk_process_leave(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int leaveInfo;
	int rejoin;
	int request;
	int removeChildren;
	int replaceRource;
	
	int pklen;
	
	int control = 1;
	int routerSeq;
	//int model;
	//int NWKAddress;
	Packet* pk;
	FIN(nwk_process_leave(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	//printf("Node %d received a leaveControl pk\n",g_nwk_my_ESN_address);
	//op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d received a leaveControl pk\n",g_nwk_my_ESN_address);
	op_pk_fd_get(payLoad,0,&leaveInfo);
	replaceRource = (leaveInfo>>4)&1;
	rejoin = (leaveInfo>>2)&3;
	request = (leaveInfo>>1)&1;
	removeChildren = leaveInfo&1;
	if(dest != g_nwk_my_short_address && dest != ChildrenCast)
		{
		if(request == 1)
			{
			deleteRouting(65535, dest);
			}
		FOUT;
		}
	if(request == 1)
		{
		//printf("removeChildren == %d && Network_Msg.NetworkCT[g_nwk_my_ESN_address].number = %d\n",removeChildren,Network_Msg.NetworkCT[g_nwk_my_ESN_address].number);
		//op_prg_log_entry_write(g_nwk_debugger_log_handle, "removeChildren == %d && Network_Msg.NetworkCT[g_nwk_my_ESN_address].number = %d\n",removeChildren,Network_Msg.NetworkCT[g_nwk_my_ESN_address].number);
		/*if(removeChildren == 1 && Network_Msg.NetworkCT[g_nwk_my_ESN_address].number > 0)
			{
			len = 1;
			pk = op_pk_create(0);
			op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,0,8);
			pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + SHORTADDR_LEN + LENGTH_LEN;
			pklen += 8*len;
			nwk_generate_packet(pklen, 0, 0, 2, 2,g_nwk_my_short_address, ChildrenEndCast, 2, 0, len, op_pk_copy (pk), OUT_NWK_TO_MCPS_DOWN, 0);
			if(g_nwk_my_father_short_address == GateAddress)
				{
				model = 1;
				pklen -= SHORTADDR_LEN;
				}
			else
				{
				model = 2;
				}
			nwk_generate_packet(pklen, 0, 0, 2, 2,g_nwk_my_short_address, g_nwk_my_father_short_address, 2, 0, len, pk, OUT_NWK_TO_MCPS_UP, 0);
			}
		*/
		nwk_leave_net(g_nwk_my_short_address,rejoin,removeChildren,replaceRource);
		}
	else
		{
		nwk_leave_net(source,rejoin,removeChildren,-1);
		routerSeq = quiryRouterSeq(source);
		len = 4;
		pk = op_pk_create (0);//����·��ɾ������
		op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,control,8);
		op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,routerSeq,8);
		op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER,source,16);
		pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
		pklen += len*8;
		nwk_generate_packet(pklen, 0, 1, 2, 1, g_nwk_my_short_address, GateAddress, 11, NWK_MAX_RADIUS-1, len, pk, OUT_NWK_TO_MCPS_UP, 0);//����·��ɾ������
		}
	FOUT;
}


/*
	��·�ɷ����ѯ�ķ���
*/
static void nwk_send_main_router_request()
{
	int len, pklen;

	Packet* pk;
	FIN(nwk_send_main_router_request());
	len = 7;
	pk = op_pk_create(0);
	op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,1,8);
	op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,g_nwk_main_routing_address,48);
	pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
	pklen += 8*len;
	nwk_generate_packet(pklen,0,1,2,1,g_nwk_my_short_address,GateAddress,12,NWK_MAX_RADIUS-1, len, pk, OUT_NWK_TO_MCPS_UP,0);//������·�ɲ�ѯ����
	FOUT;
}

/*
	��·�ɷ���Ľ��մ���
*/
static void nwk_process_main_router_request(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int type, ESNAddress, NWKAddress, band;
	int pklen, model;
	struct R2BEntry T;
	Packet* pk;
	FIN(nwk_process_main_router_request(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	op_pk_fd_get(payLoad,0,&type);
	//printf("mainRouterPro type = %d\n",type);
	//op_prg_log_entry_write(g_nwk_debugger_log_handle, "mainRouterPro type = %d\n",type);
	if(type == 1 && g_nwk_node_type == GATENODE)
		{
		op_pk_fd_get(payLoad,1,&ESNAddress);
		NWKAddress = mapping[ESNAddress].shortAddress;
		op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d received a mainRouterQuery query router %d's NWKAddress = %d\n",g_nwk_my_ESN_address,ESNAddress,NWKAddress);
		if(NWKAddress <= 0)
		{
			FOUT;
		}
		band = bandQuire(NWKAddress);
		len = 4;
		pk = op_pk_create(0);
		op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,2,8);
		op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
		op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER,band,8);
		pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
		pklen += 8*len;
		nwk_generate_packet(pklen,0,1,1,2,g_nwk_my_short_address,source,12,NWK_MAX_RADIUS-1, len, pk, OUT_NWK_TO_MCPS_DOWN,0);//������·����Ϣ�ظ�
		printf("Node %d received a mainRouterQuery query router %d's NWKAddress = %d\n",g_nwk_my_ESN_address,ESNAddress,NWKAddress);
		//op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d received a mainRouterQuery query router %d's NWKAddress = %d\n",g_nwk_my_ESN_address,ESNAddress,NWKAddress);
		}
	else if(type == 2 && g_nwk_node_type != GATENODE)
		{
		op_pk_fd_get(payLoad,1,&NWKAddress);
		op_pk_fd_get(payLoad,2,&band);
		printf("need rejoin!!!,fatherAddress = %d,band = %d\n",NWKAddress,band);
		op_prg_log_entry_write(g_nwk_info_log_handle, "need rejoin!!!,fatherAddress = %d,band = %d\n",NWKAddress,band);
		g_nwk_main_address = NWKAddress;
		g_nwk_main_router_band = band;
		if(g_nwk_main_address == g_nwk_my_father_short_address)
			{
			len = 1;
			g_nwk_capability |= g_nwk_node_type<<4;
			pk = op_pk_create (0);
			op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,3,8);
			pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + SHORTADDR_LEN + LENGTH_LEN;
			pklen += 8*len;
			nwk_generate_packet(pklen,0,1,2,2,g_nwk_my_short_address,g_nwk_my_father_short_address, 12, 0, len,pk,OUT_NWK_TO_MCPS_UP,0);//���ͱ���·�ɽ���֪ͨ
			}
		else
			{
			len = 1;
			g_nwk_capability |= g_nwk_node_type<<4;
			pk = op_pk_create (0);
			op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,4,8);
			pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + SHORTADDR_LEN + LENGTH_LEN;
			pklen += 8*len;
			if(g_nwk_my_father_short_address == GateAddress)
				{
				model = 1;
				pklen -= SHORTADDR_LEN;
				}
			else
				{
				model = 2;
				}
			nwk_generate_packet(pklen,0,1,2,model,g_nwk_my_short_address,g_nwk_my_father_short_address, 2, 0, len,pk,OUT_NWK_TO_MCPS_UP,0);//��������֪ͨ
			//printf("Node %d send a leave advice\n",g_nwk_my_ESN_address);
			//op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d send a leave advice\n",g_nwk_my_ESN_address);
			g_nwk_my_father_short_address = g_nwk_main_address;
			g_nwk_father_table.NWKAddress = g_nwk_main_address;
			g_nwk_father_table.Band = band;
			//printf("band = %d\n", g_nwk_father_table.Band);
			g_nwk_specify_father = 1;
			nwk_leave_net(g_nwk_my_short_address,3,0,1);
			}
		}
	else if(type == 3 && g_nwk_node_type != GATENODE)
		{
		UPCTDeviceType(source);
		}
	else if(type == 4 && g_nwk_node_type == ROUTERNODE || g_nwk_node_type == GATENODE)
		{
		op_pk_fd_get(payLoad,1,&NWKAddress);
		T.Router = NWKAddress;
		T.Backup = source;
		if(g_nwk_r2b_Map.p == g_nwk_r2b_Map.index && g_nwk_r2b_Map.num != 0)
			{
			printf("Node %d ����ӳ�������\n", g_nwk_my_ESN_address);
			FOUT;
			}
		g_nwk_r2b_Map.entrys[g_nwk_r2b_Map.index] = T;
		g_nwk_r2b_Map.num++;
		g_nwk_r2b_Map.index++;
		if(g_nwk_r2b_Map.index >= NWK_MAX_CHILD_NUM)
			{
			g_nwk_r2b_Map.index %= NWK_MAX_CHILD_NUM;
			}
		printf("Node %d add a r2b map, index = %d, router = %d, backup = %d\n", g_nwk_my_ESN_address, g_nwk_r2b_Map.index, g_nwk_r2b_Map.entrys[g_nwk_r2b_Map.index].Router, g_nwk_r2b_Map.entrys[g_nwk_r2b_Map.index].Backup);
		}
	FOUT;
}

/*
	��·��״̬����
*/
static void nwk_process_main_router_status(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int control;
	int request;
	int status;
	int NWKAddress;
	int pklen;
	
	Packet* pk;
	FIN(nwk_process_main_router_status(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	op_pk_fd_get(payLoad,0,&control);
	request = (control>>7)&1;
	if(request == 0 && g_nwk_node_type == ROUTERNODE)//query
	{
		op_pk_fd_get(payLoad,1,&NWKAddress);
		if( CTinclude(NWKAddress) >= 0)
		{
			//��ýڵ㷢�ͱ�����Ϣ
		}
	}
	else if(request == 1 && g_nwk_node_type == BACKUPNODE)//response
	{
		status = control&127;
		if(status == 0)//����״̬
			{
		
			}
		else if(status == 127)//ʧ��״̬�������л�
			{
			g_nwk_my_short_address = g_nwk_main_address;
			g_nwk_myBand = g_nwk_main_router_band;
			g_nwk_my_frequency = g_nwk_main_router_frequency;
			len = 8;
			pk = op_pk_create(0);//���������л�����֪ͨ���ؽڵ�
			op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,g_nwk_my_short_address,16);
			op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,g_nwk_my_ESN_address,48);
			pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
			pklen += 8*len;
			nwk_generate_packet(pklen, 0, 1, 2, 1, g_nwk_my_short_address, GateAddress, 3, NWK_MAX_RADIUS-1, len, pk, OUT_NWK_TO_MCPS_UP, 0);
			}
	}
	FOUT;
}

/*
	��ַ��ͻ����Ĵ���
*/
static void addressconflict(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int Mode;
	int control, shortAddress, ESNAddress;
	int endFlag, routerFlag;
	int pklen;
	Packet* pk;
	FIN(addressconflict(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	op_pk_fd_get(payLoad,0,&control);
	if(control == 0)
		{
		len = 9;
		pk = op_pk_create(0);
		op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,1,16);
		op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,g_nwk_my_short_address,16);
		op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER,g_nwk_my_ESN_address,48);
		pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
		if(g_nwk_my_father_short_address == GateAddress)
			{
			Mode = 1;
			}
		else
			{
			Mode = 2;
			pklen += SHORTADDR_LEN;
			}
		pklen += 8*len;
		nwk_generate_packet(pklen, 0, 1, 2, Mode, g_nwk_my_short_address, g_nwk_my_father_short_address, 3, 0, len, pk, OUT_NWK_TO_MCPS_UP, 0);
		}
	else
		{
		op_pk_fd_get(payLoad,1,&shortAddress);
		op_pk_fd_get(payLoad,2,&ESNAddress);
		endFlag = (shortAddress & 255);
		routerFlag = (shortAddress & (255<<8));
		if(routerFlag != g_nwk_my_short_address)
			{
			FOUT;
			}
		else if(g_nwk_end_address_pool[endFlag].ESNAddress != ESNAddress)
			{
			nwk_leave_net(source, 1, 0, 1);
			len = 1;
			pk = op_pk_create(0);
			op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,26,8);
			pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
			if(g_nwk_node_type != GATENODE)
				{
				Mode = 2;
				pklen += SHORTADDR_LEN;
				}
			else
				{
				Mode = 1;
				}
			pklen += 8*len;
			nwk_generate_packet(pklen, 0, 0, Mode, 2,g_nwk_my_short_address, ChildrenEndCast, 2, 0, len, pk, OUT_NWK_TO_MCPS_DOWN, 0);
			}
		}
	FOUT;
}

/*
	����·����Ϣ���մ���
*/
static void nwk_process_backup_router_info(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int i;
	int index = 1;
	int infoControl;
	int type;
	int addOrReduce;
	int number;
	
	int NWKAddress;
	int nextHop;
	int status;
	int routerSeq;
	
	int ESNAddress;
	int shortAddress;
	int l_nwk_capabilityInformation;
	
	int control;
	
	int l_nwk_syn_seq;
	
	Packet* mlme;
	
	FIN(nwk_process_backup_router_info(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	op_pk_fd_get(payLoad,0,&l_nwk_syn_seq);
	if(g_nwk_node_type != BACKUPNODE || l_nwk_syn_seq != g_nwk_syn_seq)
		{
		printf("pk_seq = %d, local_seq = %d\n", l_nwk_syn_seq, g_nwk_syn_seq);
		FOUT;
		}
	op_pk_fd_get(payLoad,1,&infoControl);
	type = (infoControl>>5)&3;
	addOrReduce = (infoControl>>4)&1;
	number = infoControl&0xf;
	printf("mainRouterInfoPro,type = %d,number = %d,addOrReduce = %d\n",type,number,addOrReduce);
	//op_prg_log_entry_write(g_nwk_debugger_log_handle, "mainRouterInfoPro,type = %d,number = %d,addOrReduce = %d\n",type,number,addOrReduce);
	switch(type)
	{
		case 0: 	for(i = 0;i<number;i++)
						{
						op_pk_fd_get(payLoad,++index,&NWKAddress);
						NWKAddress = NWKAddress<<8;
						op_pk_fd_get(payLoad,++index,&nextHop);
						nextHop = nextHop<<8;
						op_pk_fd_get(payLoad,++index,&status);
						op_pk_fd_get(payLoad,++index,&routerSeq);
						if(addOrReduce == 0)
							{
							generateRT(NWKAddress,nextHop,status,routerSeq);
							}
						else
							{
							deleteRouting(-1,NWKAddress);
							}
						}	
					break;
		case 1: 	for(i = 0;i<number;i++)
						{
						op_pk_fd_get(payLoad,++index,&ESNAddress);
						op_pk_fd_get(payLoad,++index,&shortAddress);
						op_pk_fd_get(payLoad,++index,&l_nwk_capabilityInformation);
						if(addOrReduce == 0)
							{
							generateCT(ESNAddress,shortAddress,l_nwk_capabilityInformation, NWK_CHILD_TABLE_LEGITIMATE);
							}
						else
							{
							DeleteCT(1, ESNAddress);
							}
						}
					break;
		case 2:	op_pk_fd_get(payLoad,0,&control);
					op_pk_fd_get(payLoad,1,&NWKAddress);
					op_pk_fd_get(payLoad,2,&ESNAddress);
					op_pk_fd_get(payLoad,3,&l_nwk_capabilityInformation);
					if(control == 1)//��·�ɵ���Ϣ����
						{
						op_pk_fd_get(payLoad,4,&nextHop);
						op_pk_fd_get(payLoad,5,&status);
						op_pk_fd_get(payLoad,6,&routerSeq);
						if(addOrReduce == 0)
							{
							generateRT(NWKAddress,nextHop,status,routerSeq);
							generateCT(ESNAddress,NWKAddress,l_nwk_capabilityInformation, NWK_CHILD_TABLE_LEGITIMATE);
							}
						else
							{
							deleteRouting(-1,NWKAddress);
							DeleteCT(1, ESNAddress);
							}
						}
					else//���ն˵���Ϣ����
						{
						if(addOrReduce == 0)
							{
							generateCT(ESNAddress,NWKAddress,l_nwk_capabilityInformation, NWK_CHILD_TABLE_LEGITIMATE);
							}
						else
							{
							DeleteCT(1, ESNAddress);
							}
						}
					break;
		case 3: 	op_pk_fd_get(payLoad,++index,&g_nwk_main_router_father);
					op_pk_fd_get(payLoad,++index,&g_nwk_main_router_band);
					op_pk_fd_get(payLoad,++index,&g_nwk_main_router_frequency);
					printf("g_nwk_main_router_father = %d,g_nwk_main_router_band = %d,mainPoint = %d\n",g_nwk_main_router_father,g_nwk_main_router_band,g_nwk_main_router_frequency);
					nwk_master_monitor();
					mlme = op_pk_create(0);
					op_pk_fd_set (mlme, 0, OPC_FIELD_TYPE_INTEGER, ASSOCIATE, 8);
					op_pk_fd_set (mlme, 1, OPC_FIELD_TYPE_INTEGER, g_nwk_main_router_father, 16);
					op_pk_fd_set (mlme, 2, OPC_FIELD_TYPE_INTEGER, 0, 8);
					op_pk_fd_set (mlme, 3, OPC_FIELD_TYPE_INTEGER, g_nwk_main_router_band, 8);
					op_pk_fd_set (mlme, 4, OPC_FIELD_TYPE_INTEGER, g_nwk_main_router_frequency, 8);
					op_pk_fd_set (mlme, 5, OPC_FIELD_TYPE_INTEGER, 1, 8);
					op_pk_send_delayed(mlme, OUT_NWK_TO_MLME, 0);
					
					//op_prg_log_entry_write(g_nwk_info_log_handle, "g_nwk_main_router_father = %d,g_nwk_main_router_band = %d,mainPoint = %d\n",g_nwk_main_router_father,g_nwk_main_router_band,g_nwk_main_router_frequency);
					break;
	}
	g_nwk_syn_seq++;
	//nwk_syn_seq_set_MAC();
	for(i = 0; i < g_nwk_routing_table.number; i++)
		{
		//op_prg_log_entry_write(g_nwk_debugger_log_handle, "destAddress = %d, nextHop = %d, status = %d, routerSeq = %d\n", g_nwk_routing_table.table[i].destAddress, g_nwk_routing_table.table[i].nextHop, g_nwk_routing_table.table[i].status, g_nwk_routing_table.table[i].routerSeq);
		}
	FOUT;
}

/*
	�����л�֪ͨ����Ľ��մ���
*/
static void nwk_process_backup_router_switch(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len, int port)
{
	int NWKAddress, ESNAddress;
	int i;
	FIN(nwk_process_backup_router_switch(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len, int port));
	op_pk_fd_get(payLoad,0,&NWKAddress);
	op_pk_fd_get(payLoad,1,&ESNAddress);
	if(port == 0)
		{
		if(g_nwk_node_type == GATENODE)
			{
			i = quiryNWKAddressFromMap(NWKAddress);
			if(i >= 0) nwk_update_mapping(i, 0);
			//printf("i = %d, ESNAddress = %d, NWKAddress = %d\n", i, ESNAddress, NWKAddress);
			nwk_update_mapping(ESNAddress, NWKAddress);
			g_nwk_router_address_pool[(NWKAddress >> 8) - 1].ESNAddress = ESNAddress;
			}
		i = CTinclude(NWKAddress);
		if(i >= 0)
			{
			Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].ESNAddress = ESNAddress;
			Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].status = NWK_CHILD_TABLE_LEGITIMATE;
			}
		}
	else
		{
		g_nwk_reassociate_leave_flag = 1;
		printf("xxx\n");
		nwk_leave_request(1);
		}
	FOUT;
}

/*
	����״̬����Ľ��մ���
*/
static void nwk_process_nwk_status(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int NWKAddress, status;
	FIN(nwkStatuspro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	op_pk_fd_get(payLoad,0,&NWKAddress);
	op_pk_fd_get(payLoad,1,&status);
	printf("Node %d received a nwkStatus pk, shortAddress = %d , Status = %d!\n", g_nwk_my_ESN_address, NWKAddress, status);
	//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d received a nwkStatus pk, shortAddress = %d , Status = %d!\n", g_nwk_my_ESN_address, NWKAddress, status);
	FOUT;
}


/*
	·�ɿ��Ʒ���Ľ��մ���
*/
static void nwk_process_routing_control(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int control, num, routerSeq, shortAddress;
	int i, index, next;
	FIN(nwk_process_routing_control(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	index = -1;
	op_pk_fd_get(payLoad,++index,&control);
	op_pk_fd_get(payLoad,++index,&num);
	for(i = 0;i < num;i++)
		{
		op_pk_fd_get(payLoad,++index,&routerSeq);
		op_pk_fd_get(payLoad,++index,&shortAddress);
		shortAddress = (shortAddress<<8);
		if(control == 0)//���
			{
			next = nwk_get_nexthop(shortAddress);
			generateRT(shortAddress, next, NWK_ROUTING_TABLE_Active, routerSeq);
			}
		else//ɾ��
			{
			deleteRouting(routerSeq, shortAddress);
			}
		}
	FOUT;
}

/*
	����ڵ�ָ��Ľ��մ���
*/
static void nwk_recover_node(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	FIN(nwk_recover_node(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	nwk_leave_net(g_nwk_my_short_address, 2, 0, 0);
	FOUT;
}

/*
	�������ݵĽ��մ���
*/
static void nwk_process_up_data(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int data;
	//int ete_delay;
	FIN(nwk_process_up_data(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	op_pk_fd_get(payLoad,0,&data);
	//op_pk_send(packet,1);
	//op_stat_write(ete_delay_up,op_sim_time () - op_pk_creation_time_get (packet));
	//printf("-------------------------------------------------------------------------delay = %f-------------------\n",op_sim_time () - op_pk_creation_time_get (payLoad));
	//op_stat_write(up_data_receive_num,++upReceiveNum);
	//op_stat_write(up_pk_loss_rate,upReceiveNum/upSendNum);
	g_pk_recv_up++;
	pk_stat_up(payLoad);
	//if(source == 256)
		//{
		//printf("ssssssssssssssssssssssssssssssssss\n");
		//ete_delay = op_sim_time () - op_pk_creation_time_get (payLoad);
		//op_stat_write (ete_delay_l_n, 		ete_delay);
		//}
	printf("----------------------------Node %d receive a up data = %d! g_pk_recv_up = %d  send = %d ----",g_nwk_my_ESN_address,data, g_pk_recv_up, g_pk_send_up);
	//op_pk_send(payLoad, OUT_NWK_TO_SINK_UP);
	//op_prg_log_entry_write(g_nwk_info_log_handle, "----------------------------Node %d receive a up data = %d!  g_pk_recv_up = %d  send = %d----len = %d-------------------\n",g_nwk_my_ESN_address,data, len, g_pk_recv_up, g_pk_send_up);
	FOUT;
}

/*
	�������ݵĽ��մ���
*/
static void nwk_process_down_data(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int data;
	FIN(nwk_process_down_data(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	op_pk_fd_get(payLoad,0,&data);
	//op_stat_write(ete_delay_down,op_sim_time () - op_pk_creation_time_get (packet));
	//printf("-------------------------------------------------------------------------delay = %f-------------------\n",op_sim_time () - op_pk_creation_time_get (payLoad));
	//op_stat_write(down_data_receive_num,++downReceiveNum);
	//op_stat_write(down_pk_loss_rate,downReceiveNum/downSendNum);
	printf("----------------------------Node %d receive a down data = %d!  ----",g_nwk_my_ESN_address,data);
	//op_pk_send(payLoad, OUT_NWK_TO_SINK_DOWN);
	if(dest < 17)//�鲥����
		{
		FOUT;
		}
	g_pk_recv_down++;
	pk_stat_down(payLoad);
	//op_prg_log_entry_write(g_nwk_info_log_handle, "----------------------------Node %d receive a down data = %d!  ----len = %d-------------------\n",g_nwk_my_ESN_address,data, len);
	FOUT;
}

/*
	�����ӽڵ����Ŀ
*/
static void generateCT(int ESNAddress,int NWKAddress,int l_nwk_capabilityInformation, int status)
{
	struct childrenTable T;
	int i;
	int len;
	FIN(gerenateCT(int ESNAddress,int NWKAddress,int l_nwk_capabilityInformation, int status) );
	if(g_nwk_node_type == BACKUPNODE && NWKAddress == g_nwk_my_short_address)
		{
		printf("CT my self need not add\n");
		FOUT;
		}
	T.ESNAddress = ESNAddress;
	printf("Node %d generateCT T.ESNAddress = %d, number = %d\n",g_nwk_my_ESN_address,T.ESNAddress, Network_Msg.NetworkCT[g_nwk_my_ESN_address].number);
	
	//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d generateCT T.ESNAddress = %d\n",g_nwk_my_ESN_address,T.ESNAddress);
	T.NWKAddress = NWKAddress;
	T.status = status;
	T.CapabilityInformation = l_nwk_capabilityInformation;
	T.JoinCount = NWK_PROXY_JOIN_TIMER;
	for(i = 0;i<Network_Msg.NetworkCT[g_nwk_my_ESN_address].number;i++)
	{
		//if(Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].ESNAddress == ESNAddress || (Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].NWKAddress == NWKAddress && NWKAddress > 0))
		if(Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].NWKAddress == NWKAddress && NWKAddress > 0)
		{
			Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i] = T;
			if(g_nwk_backup_router > 0)
				{
				//��ͬ�����м���Ϣ
				op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d push_synchronize_queue ready send a backup ct pk, NWKAddress = %d\n", g_nwk_my_ESN_address, NWKAddress);
				nwk_push_synchronize_queue(1, -1, -1, i, i + 1);
				}
			if(status == NWK_CHILD_TABLE_ILLEGAL)	FOUT;
			sendChildSetConfirm(ESNAddress, NWKAddress);
			FOUT;
		}
		if(Network_Msg.NetworkCT[g_nwk_my_ESN_address].number >= NWK_MAX_CHILD_NUM && Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].status == NWK_CHILD_TABLE_DELETED)
			{
			Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i] = T;
			if(g_nwk_backup_router > 0)
				{
				//��ͬ�����м���Ϣ
				op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d push_synchronize_queue ready send a backup ct pk, NWKAddress = %d\n", g_nwk_my_ESN_address, NWKAddress);
				nwk_push_synchronize_queue(1, -1, -1, i, i + 1);
				}
			if(status == NWK_CHILD_TABLE_ILLEGAL)	FOUT;
			sendChildSetConfirm(ESNAddress, NWKAddress);
			FOUT;
			}
	}
	for(i = 0;i<Network_Msg.NetworkCT[g_nwk_my_ESN_address].number;i++)
	{
		if(Network_Msg.NetworkCT[g_nwk_my_ESN_address].number >= NWK_MAX_CHILD_NUM && Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].status == NWK_CHILD_TABLE_ILLEGAL)
			{
			Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i] = T;
			if(g_nwk_backup_router > 0)
				{
				//��ͬ�����м���Ϣ
				op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d push_synchronize_queue ready send a backup ct pk, NWKAddress = %d\n", g_nwk_my_ESN_address, NWKAddress);
				nwk_push_synchronize_queue(1, -1, -1, i, i + 1);
				}
			if(status == NWK_CHILD_TABLE_ILLEGAL)	FOUT;
			sendChildSetConfirm(ESNAddress, NWKAddress);
			FOUT;
			}
	}
	Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[Network_Msg.NetworkCT[g_nwk_my_ESN_address].number] = T;
	//printf("Network_Msg.NetworkCT[g_nwk_my_ESN_address].number = %d \n", Network_Msg.NetworkCT[g_nwk_my_ESN_address].number);
	if(g_nwk_backup_router > 0)
		{
		//����Ӧ�ø�Ϊ��ͬ�����м���Ϣ
		op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d push_synchronize_queue ready send a backup ct pk, NWKAddress = %d\n", g_nwk_my_ESN_address, NWKAddress);
		nwk_push_synchronize_queue(1, -1, -1, Network_Msg.NetworkCT[g_nwk_my_ESN_address].number, Network_Msg.NetworkCT[g_nwk_my_ESN_address].number + 1);
		//nwk_send_backup_info(2,0,NWKAddress,0,1);
		}
	Network_Msg.NetworkCT[g_nwk_my_ESN_address].number++;
	len = 10;
	if(status == NWK_CHILD_TABLE_ILLEGAL)	FOUT;
	sendChildSetConfirm(ESNAddress, NWKAddress);
	FOUT;
}

static void sendChildSetConfirm(int ESNAddress, int NWKAddress)
{
	Packet*mlme;
	FIN(sendChildSetConfirm(int ESNAddress, int NWKAddress));
	//printf("setChild ESNAddress = %d, NWKAddress = %d", ESNAddress, NWKAddress);
	mlme = op_pk_create(0);//�����ӽڵ��ַԭ��
	op_pk_fd_set(mlme,0,OPC_FIELD_TYPE_INTEGER,ChildSEt,8);
	op_pk_fd_set(mlme,1,OPC_FIELD_TYPE_INTEGER,ESNAddress,48);
	op_pk_fd_set(mlme,2,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
	op_pk_fd_set(mlme,3,OPC_FIELD_TYPE_INTEGER,0,8);
	op_pk_send(mlme, OUT_NWK_TO_MLME);
	FOUT;

}


/*
	ɾ���ӽڵ��
*/

static void DeleteCT(int type, int address)
{
	int i;
	int control = 1;
	int len;
	Packet* mlme;
	FIN(DeleteCT(int type, int address));
	printf("Network_Msg.NetworkCT[%d].number = %d\n", g_nwk_my_ESN_address, Network_Msg.NetworkCT[g_nwk_my_ESN_address].number);
	if(address > 0)
	{
		for(i = 0;i<Network_Msg.NetworkCT[g_nwk_my_ESN_address].number;i++)
		{
			if((type == 1 && Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].ESNAddress == address) || (type == 2 && Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].NWKAddress == address))
			{
				Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].status = NWK_CHILD_TABLE_DELETED;
				len = 10;
				mlme = op_pk_create(0);//�����ӽڵ��ַԭ��
				op_pk_fd_set(mlme,0,OPC_FIELD_TYPE_INTEGER,ChildSEt,8);
				op_pk_fd_set(mlme,1,OPC_FIELD_TYPE_INTEGER,Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].ESNAddress,48);
				op_pk_fd_set(mlme,2,OPC_FIELD_TYPE_INTEGER,Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].NWKAddress,16);
				op_pk_fd_set(mlme,3,OPC_FIELD_TYPE_INTEGER,1,8);
				op_pk_send(mlme, OUT_NWK_TO_MLME);
				if(g_nwk_backup_router > 0) nwk_push_synchronize_queue(2, -1, -1, i, i + 1);
			}
		}
		
	}
	FOUT;
}


/*
	�������������ƿ���ʱ���ڵ����ӽڵ����������������������ӽڵ���м�ʱ
*/
static void updateCT()
{
	int i = 0;
	FIN(updateCT());
	for(i = 0;i<Network_Msg.NetworkCT[g_nwk_my_ESN_address].number;i++)
		{
		if(Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].status == NWK_CHILD_TABLE_ILLEGAL)
			{
			Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].JoinCount--;
			if(Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].JoinCount == 0) 
				{
				DeleteCT(1, Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].ESNAddress);
				}
			}
		}
	FOUT;
}


/*
	��ȡ������������������ӽڵ�����
*/
static int get_illegal_CT_number()
{
	int n = 0;
	int i;
	FIN(get_illegal_CT_number());
	for(i = 0;i<Network_Msg.NetworkCT[g_nwk_my_ESN_address].number;i++)
		{
		if(Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].status == NWK_CHILD_TABLE_ILLEGAL && Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].JoinCount > 0)
			{
			n++;
			}
			
		}
	FRET(n);
}


/*
	����·�ɱ���Ŀ
*/
static void generateRT(int destAddress,int nextHop,int status,int routerSeq)
{
	struct routerTable T;
	int i;
	FIN(gerenateRT(int destAddress,int nextHop,int status,int routerSeq));
	destAddress &= (255<<8);
	if(nextHop == 0 || destAddress == 0)	FOUT;
	printf("Node %d generateRT destAddress = %d, address = %d\n",g_nwk_my_ESN_address,destAddress, (destAddress>>8));
	//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d generateRT destAddress = %d\n",g_nwk_my_ESN_address,destAddress);
	for(i = 0;i<g_nwk_routing_table.number;i++)
	{
		if(g_nwk_routing_table.table[i].destAddress == (destAddress>>8))
		{
			if(g_nwk_routing_table.table[i].routerSeq <= routerSeq || routerSeq == 0)
			{
				g_nwk_routing_table.table[i].destAddress = (destAddress>>8);
				g_nwk_routing_table.table[i].nextHop = (nextHop>>8);
				g_nwk_routing_table.table[i].status = status;
				if(g_nwk_routing_table.table[i].routerSeq <= routerSeq)
					{
					g_nwk_routing_table.table[i].routerSeq = routerSeq;
					}
				else g_nwk_routing_table.table[i].routerSeq = g_nwk_routing_table.table[i].routerSeq + 1;
				g_nwk_routing_table.table[i].TimeoutCount = NWK_COUNT_RT_ACTIVE;
				/*
				����б��ݽڵ㣬����±��ݽڵ㱸����Ϣ
				*/
				if(g_nwk_backup_router > 0)
					{
					//����Ӧ�ø�Ϊ��ͬ�����м���Ϣ
					op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d push_synchronize_queue ready send a backup pk, NWKAddress = %d\n", g_nwk_my_ESN_address, destAddress);
					printf( "Node %d push_synchronize_queue ready send a backup pk, NWKAddress = %d\n", g_nwk_my_ESN_address, destAddress);
					nwk_push_synchronize_queue(1, i, i + 1, -1, -1);
					//nwk_send_backup_info(2,0,NWKAddress,0,1);
					}
				printf("--------------------------------------Node %d update a g_nwk_routing_table destAddress = %d,TimeoutCount = %d\n",g_nwk_my_ESN_address,g_nwk_routing_table.table[i].destAddress,g_nwk_routing_table.table[i].TimeoutCount);
			}
			FOUT;
		}
	}
	if(g_nwk_routing_table.number == 48)
		{
		printf("RT full\n");
		FOUT;
		}
	T.destAddress = (destAddress>>8);
	T.nextHop = (nextHop>>8);
	T.status = status;
	T.routerSeq = routerSeq;
	T.TimeoutCount = NWK_COUNT_RT_ACTIVE;
	g_nwk_routing_table.table[g_nwk_routing_table.number] = T;
	
	printf("--------------------------------------Node %d add a g_nwk_routing_table destAddress = %d,TimeoutCount = %d\n",g_nwk_my_ESN_address,g_nwk_routing_table.table[g_nwk_routing_table.number].destAddress,g_nwk_routing_table.table[g_nwk_routing_table.number].TimeoutCount);
	//op_prg_log_entry_write(g_nwk_info_log_handle, "--------------------------------------Node %d add a g_nwk_routing_table destAddress = %d,TimeoutCount = %d\n",g_nwk_my_ESN_address,g_nwk_routing_table.table[g_nwk_routing_table.number].destAddress,g_nwk_routing_table.table[g_nwk_routing_table.number].TimeoutCount);
	g_nwk_routing_table.number++;
	Network_Msg.NetworkRT[g_nwk_my_ESN_address] = g_nwk_routing_table;
	//printf("--------------------------------------Node %d add a g_nwk_routing_table destAddress = %d\n",g_nwk_my_ESN_address,Network_Msg.NetworkRT[g_nwk_my_ESN_address].table[Network_Msg.NetworkRT[g_nwk_my_ESN_address].number - 1].destAddress);
	if(g_nwk_backup_router > 0)
		{
		//����Ӧ�ø�Ϊ��ͬ�����м���Ϣ
		op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d push_synchronize_queue ready send a backup pk, NWKAddress = %d\n", g_nwk_my_ESN_address, destAddress);
		printf("Node %d push_synchronize_queue ready send a backup pk, NWKAddress = %d\n", g_nwk_my_ESN_address, destAddress);
		nwk_push_synchronize_queue(1, g_nwk_routing_table.number-1, g_nwk_routing_table.number, -1, -1);
		//nwk_send_backup_info(2,0,NWKAddress,0,1);
		}
	FOUT;
}



/*
	·�ɼ��������»�·��״̬����
	NWKAddress > 0 ָʾ���¶�Ӧ��·����Ŀ״̬�������������·�ɵļ�����
*/
static void UpdateRT(int NWKAddress, int status)
{
	int i, len, pklen;
	int model;
	Packet* pk;
	FIN(UpdateRT(int NWKAddress, int status));
	if(NWKAddress > 0)//·��״̬����
	{
		//printf("Node %d UpdateRT NWKAddress = %d\n",g_nwk_my_ESN_address,NWKAddress);
		//if((NWKAddress & 0x8000) > 0)
		//{
			//FOUT;
		//}
		if((NWKAddress & 0xff) > 0)//�յ��ն˽ڵ����������,���������·�ɽڵ��·��
			{
			NWKAddress &= 0xff00;
			//if(NWKAddress == g_nwk_my_short_address)
				//{
				//FOUT;
				//}
			}
		//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d UpdateRT NWKAddress = %d\n",g_nwk_my_ESN_address,NWKAddress);
		i = queryRTIndex(NWKAddress);
		if(i >= 0)
		{
			//printf("Node %d receive a data from %d ,updateRT,i = %d,g_nwk_routing_table.number = %d, status = %d\n",g_nwk_my_ESN_address,NWKAddress,i,g_nwk_routing_table.number, status);
			//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d receive a data from %d ,updateRT,i = %d,g_nwk_routing_table.number = %d, status = %d\n",g_nwk_my_ESN_address,NWKAddress,i,g_nwk_routing_table.number, status);
			g_nwk_routing_table.table[i].status = status;
			if(status == NWK_ROUTING_TABLE_Active)
				{
				g_nwk_routing_table.table[i].TimeoutCount = NWK_COUNT_RT_ACTIVE;
				}
			if(status == NWK_ROUTING_TABLE_TIMEOUT)
				{
				g_nwk_routing_table.table[i].TimeoutCount = NWK_COUNT_RT_TIMEOUT;
				}
			if(status == NWK_ROUTING_TABLE_DELETED && g_nwk_node_type == ROUTERNODE)
				{
				//����·��ɾ������
				len = 4;
				pk = op_pk_create (0);
				op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,1,8);
				op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER, g_nwk_routing_table.table[i].routerSeq, 8);
				op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER, g_nwk_routing_table.table[i].destAddress, 16);
				pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
				pklen += 8*len;
				nwk_generate_packet(pklen, 0, 1, 2, 1, g_nwk_my_short_address, GateAddress, 11, NWK_MAX_RADIUS-1, len, pk, OUT_NWK_TO_MCPS_UP, 0);
				}
		}
		else
			{
			printf("Node %d queryRTIndex error NWKAddress = %d\n", g_nwk_my_ESN_address, NWKAddress);
			//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d queryRTIndex error NWKAddress = %d\n", g_nwk_my_ESN_address, NWKAddress);
			}
	}
	else//·�ɼ���������
	{
		if(g_nwk_node_type == BACKUPNODE)
			{
			FOUT;
			}
		if(g_nwk_node_type == BACKUPNODE) FOUT;
		for(i = 0;i<g_nwk_routing_table.number;i++)
		{
			if(CTinclude(g_nwk_routing_table.table[i].destAddress<<8) > -1)//·��Ŀ�ĵ�ַ���ӽڵ㣬�ɱ������·�ɸ���
				{
				continue;
				}
			if(g_nwk_routing_table.table[i].status == NWK_ROUTING_TABLE_DELETED)	
				{
				printf("Node %d g_nwk_routing_table %d status = NWK_ROUTING_TABLE_DELETED\n", g_nwk_my_ESN_address, g_nwk_routing_table.table[i].destAddress);
				if(g_nwk_routing_table.table[i].destAddress == 255)
					{
					printf("->>>>Node %d g_nwk_routing_table %d i = %d,next = %d\n", g_nwk_my_ESN_address, g_nwk_routing_table.table[i].destAddress, i, g_nwk_routing_table.table[i].nextHop);
					}
				//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d g_nwk_routing_table %d status = NWK_ROUTING_TABLE_DELETED\n", g_nwk_my_ESN_address, g_nwk_routing_table.table[i].destAddress);
				continue;
				}
			--g_nwk_routing_table.table[i].TimeoutCount;
			//printf("Node %d g_nwk_routing_table Node %d g_nwk_routing_table.table.TimeoutCount = %d\n",g_nwk_my_ESN_address, g_nwk_routing_table.table[i].destAddress, g_nwk_routing_table.table[i].TimeoutCount);
			/*
			if(g_nwk_routing_table.table[i].TimeoutCount == RT_TimeoutCount / 2 && g_nwk_routing_table.table[i].status == NWK_ROUTING_TABLE_TIMEOUT)
				{
				len = 1;
				pk = op_pk_create (0);
				op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,2,8);
				op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER,g_nwk_routing_table.table[i].destAddress<<8,16);
				pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
				if(g_nwk_node_type == GATENODE)
					{
					model = 1;
					}
				else
					{
					model = 1;
					pklen += SHORTADDR_LEN;
					}
				pklen += 8*len;
				nwk_generate_packet(pklen, 0, 1, 2, 2, g_nwk_my_short_address, (g_nwk_routing_table.table[i].nextHop<<8), 11, 0, len, pk, OUT_NWK_TO_MCPS_DOWN, 0);//��������·��ά������
				printf("Node %d send second router seq to %d!\n", g_nwk_my_ESN_address, g_nwk_routing_table.table[i].destAddress);
				//op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d send second router seq to %d!\n", g_nwk_my_ESN_address, g_nwk_routing_table.table[i].destAddress);
				}
			*/
			if(g_nwk_routing_table.table[i].TimeoutCount <= 0)
			{
				if(g_nwk_routing_table.table[i].status == NWK_ROUTING_TABLE_Active)//����ά��
					{
					g_nwk_routing_table.table[i].status = NWK_ROUTING_TABLE_TIMEOUT;
					g_nwk_routing_table.table[i].TimeoutCount = NWK_COUNT_RT_TIMEOUT;
					len = 1;
					pk = op_pk_create (0);
					op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,2,8);
					op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER,g_nwk_routing_table.table[i].destAddress<<8,16);
					pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
					if(g_nwk_node_type == GATENODE)
						{
						model = 1;
						}
					else
						{
						model = 1;
						pklen += SHORTADDR_LEN;
						}
					pklen += 8*len;
					nwk_generate_packet(pklen, 0, 1, 2, 2, g_nwk_my_short_address, (g_nwk_routing_table.table[i].nextHop<<8), 11, 0, len, pk, OUT_NWK_TO_MCPS_DOWN, op_dist_uniform(47));//��������·��ά������
					//nwk_generate_packet(pklen, 0, 1, 2, 2, g_nwk_my_short_address, (g_nwk_routing_table.table[i].nextHop<<8), 11, 0, len, pk, OUT_NWK_TO_MCPS_DOWN, 0);//��������·��ά������
					printf("Node %d send a router seq to %d!\n", g_nwk_my_ESN_address, g_nwk_routing_table.table[i].destAddress);
					//op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d send a router seq to %d!\n", g_nwk_my_ESN_address, g_nwk_routing_table.table[i].destAddress);
					}
				else if(g_nwk_routing_table.table[i].status == NWK_ROUTING_TABLE_TIMEOUT)
					{
					printf("Node %d g_nwk_routing_table %d status = deleted\n", g_nwk_my_ESN_address, g_nwk_routing_table.table[i].destAddress);
					//op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d g_nwk_routing_table %d status = deleted\n", g_nwk_my_ESN_address, g_nwk_routing_table.table[i].destAddress);
					g_nwk_routing_table.table[i].status = NWK_ROUTING_TABLE_DELETED;
					if(g_nwk_node_type == GATENODE)	continue;
					len = 4;
					pk = op_pk_create (0);
					op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,1,8);
					op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER, g_nwk_routing_table.table[i].routerSeq, 8);
					op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER, g_nwk_routing_table.table[i].destAddress, 16);
					pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
					pklen += 8*len;
					nwk_generate_packet(pklen, 0, 1, 2, 1, g_nwk_my_short_address, GateAddress, 11, NWK_MAX_RADIUS-1, len, pk, OUT_NWK_TO_MCPS_UP, 0);//����·��ɾ��
					}
				else	printf("============\n");
			}
		}
	}
	Network_Msg.NetworkRT[g_nwk_my_ESN_address] = g_nwk_routing_table;
	FOUT;
}

/*
	·�ɱ��ɾ��
*/
static void deleteRouting(int routerSeq, int NWKAddress)
{
	int i;
	FIN(deleteRouting(int routerSeq, int NWKAddress));
	for(i = 0;i<g_nwk_routing_table.number;i++)
		{
			if(NWKAddress == g_nwk_routing_table.table[i].destAddress)
				{
				if(routerSeq >= g_nwk_routing_table.table[i].routerSeq)
					{
					g_nwk_routing_table.table[i].status = NWK_ROUTING_TABLE_DELETED;
					g_nwk_routing_table.table[i].TimeoutCount = 0;
					Network_Msg.NetworkRT[g_nwk_my_ESN_address] = g_nwk_routing_table;
					printf("Node %d delete router %d========\n", g_nwk_my_ESN_address, NWKAddress);
					//op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d delete router %d========\n", g_nwk_my_ESN_address, NWKAddress);
					if(g_nwk_backup_router > 0)
						{
						//����Ӧ�ø�Ϊ��ͬ�����м���Ϣ
						op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d push_synchronize_queue ready send a backup pk, NWKAddress = %d\n", g_nwk_my_ESN_address, NWKAddress);
						printf("Node %d push_synchronize_queue ready send a backup pk, NWKAddress = %d\n", g_nwk_my_ESN_address, NWKAddress);
						nwk_push_synchronize_queue(2, i, i + 1, -1, -1);
						}
					}
				
				break;
				}
		}
	FOUT;
}

static void UpdateBackup()
{
	FIN(UpdateBackup());
	if(g_nwk_backup_router <= 0)	FOUT;
	--g_nwk_synchronize_queue.count;
	if(g_nwk_synchronize_queue.count <= 0)
		{
		nwk_send_synchronize_queue();
		}
	FOUT;
}

/*
	��ѯ��Ӧ·����·�ɱ��λ�ã���������
*/
static int queryRTIndex(int NWKAddress)
{
	int i;
	FIN(queryRTIndex(int NWKAddress));
	NWKAddress = (NWKAddress>>8);
	for(i = 0;i<g_nwk_routing_table.number;i++)
	{	
		if(g_nwk_routing_table.table[i].destAddress==NWKAddress)
		{
			FRET(i);
		}
	}
	FRET(-1);
}


/*
	ͨ������ַ�����ӽڵ���еĶ̵�ַ
*/
static int CTNWKAddresss(int ESNAddress)
{
	int i = 0;
	FIN(CTNWKAddresss(int ESNAddress));
	for(i = 0;i < Network_Msg.NetworkCT[g_nwk_my_ESN_address].number;i++)
	{
		if(Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].ESNAddress == ESNAddress)	FRET(Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].NWKAddress);
	}
	FRET(-1);
}

/*
	�����ӽڵ���е���Ŀ״̬Ϊ�Ϸ�״̬
	����ӽڵ��Ǳ��ݽڵ㣬���������ģʽ
*/
static void UPCTStatus(int ESNAddress,int NWKAddress)
{
	int i = 0, len;
	int deviceType;
	Packet* mlme;
	FIN(UPCTStatus(int ESNAddress,int NWKAddress));
	//printf("Network_Msg.NetworkCT[g_nwk_my_ESN_address].number = %d\n",Network_Msg.NetworkCT[g_nwk_my_ESN_address].number);
	//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Network_Msg.NetworkCT[g_nwk_my_ESN_address].number = %d\n",Network_Msg.NetworkCT[g_nwk_my_ESN_address].number);
	for(i = 0;i < Network_Msg.NetworkCT[g_nwk_my_ESN_address].number;i++)
	{
		if(Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].ESNAddress == ESNAddress)
		{
			if(NWKAddress > 0)	Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].NWKAddress = NWKAddress;
			Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].status = NWK_CHILD_TABLE_LEGITIMATE;
			deviceType = ((Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].CapabilityInformation)>>4);
			len = 10;
			mlme = op_pk_create(0);//�����ӽڵ��ַԭ��
			op_pk_fd_set(mlme,0,OPC_FIELD_TYPE_INTEGER,ChildSEt,8);
			op_pk_fd_set(mlme,1,OPC_FIELD_TYPE_INTEGER,ESNAddress,48);
			op_pk_fd_set(mlme,2,OPC_FIELD_TYPE_INTEGER,Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].NWKAddress,16);
			op_pk_fd_set(mlme,3,OPC_FIELD_TYPE_INTEGER,0,8);
			op_pk_send(mlme, OUT_NWK_TO_MLME);
			if(g_nwk_backup_router > 0) nwk_push_synchronize_queue(1, -1, -1, i, i + 1);
			/*if(deviceType == 3)//���ݽڵ���Ҫ�������ͱ�����Ϣ
			{
				g_nwk_backup_router = NWKAddress;
				printf("mmmm\n");
				nwk_init_send_backup_info();
			}*/
			FOUT;
		}
	}
	FOUT;
}
/*
	�����ӽڵ���е��豸����Ϊ���ݽڵ�����
*/
static void UPCTDeviceType(int NWKAddress)
{
	int i = 0;
	FIN(UPCTDeviceType(int NWKAddress));
	//printf("Network_Msg.NetworkCT[g_nwk_my_ESN_address].number = %d\n",Network_Msg.NetworkCT[g_nwk_my_ESN_address].number);
	//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Network_Msg.NetworkCT[g_nwk_my_ESN_address].number = %d\n",Network_Msg.NetworkCT[g_nwk_my_ESN_address].number);
	for(i = 0;i < Network_Msg.NetworkCT[g_nwk_my_ESN_address].number;i++)
	{
		//printf("i = %d g_nwk_my_ESN_address = %d NWKAddress = %d  NWKAddress = %d \n", i, g_nwk_my_ESN_address, Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].NWKAddress, NWKAddress);
		if(Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].NWKAddress == NWKAddress)
		{
			Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].CapabilityInformation |= (BACKUPNODE<<4);
			g_nwk_backup_router = NWKAddress;
			nwk_syn_seq_set_MAC();
			nwk_init_send_backup_info();
			FOUT;
		}
	}
	FOUT;
}



/*
	��ӳ�����ͨ������ַ���Ҷ̵�ַ
*/
static int quiryNWKAddress(int MACAddress)
{
	FIN(quiryNWKAddress(int MACAddress));
	FRET(mapping[MACAddress].shortAddress);
}


/*
	����·�����к�
*/
static int quiryRouterSeq(int NWKAddress)
{
	int i;
	FIN(quiryRouterSeq(int NWKAddress));
	for(i = 0;i<g_nwk_routing_table.number;i++)
	{
		if(g_nwk_routing_table.table[i].destAddress == NWKAddress)
		{
			FRET(g_nwk_routing_table.table[i].routerSeq);
		}
	}
	FRET(-1);
}

/*
	���ӽڵ����ͨ���̵�ַ�������������������ӽڵ����Ŀ����-1
*/
static int CTinclude(int NWKAddress)
{
	int i = 0;
	FIN(CTinclude(int NWKAddress));
	//if(g_nwk_my_ESN_address == 5) printf("NWKAddress = %d\n", NWKAddress);
	//printf("Network_Msg.NetworkCT[g_nwk_my_ESN_address].number = %d\n", Network_Msg.NetworkCT[g_nwk_my_ESN_address].number);
	for(i = 0;i < Network_Msg.NetworkCT[g_nwk_my_ESN_address].number;i++)
	{
		//if(g_nwk_my_ESN_address == 5) printf("dest_address = %d\n", Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].NWKAddress);
		if(Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].NWKAddress == NWKAddress)	
		{
			FRET(i);
		}
	}
	FRET(-1);
}

/*
	���ؽڵ�ͨ���̵�ַ���Ҷ�ӦƵ��
*/
static int bandQuire(int NWKAddress)
{
	int i;
	FIN(bandQuire(int NWKAddress));
	for(i = 0;i<NWK_MAX_ROUTER_NUM;i++)
	{
		if(bandTable[i] == NWKAddress)
		{
			FRET(i);
		}
	}
	FRET(-1);
}

/*
	���鲥���в����Ƿ��ڶ�Ӧ�鲥����
*/
static int quiryMT(int multicastID)
{
	int ret;
	FIN(quiryMT(int multicastID));
	ret = (1<<multicastID) & g_nwk_my_multicast_table;
	if(ret > 0)		FRET(1);
	FRET(-1);
}

/*
	�������鲥���в��Ҷ�Ӧ�鲥��
*/

static int quiryChildMT(int dest)
{
	int ret;
	FIN(quiryChildMT(int dest));
	ret = (1<<dest) & g_nwk_children_multicast_table.group;
	if(ret > 0)		FRET(1);
	FRET(-1);
}

/*
	�����鲥��Ķ�ʱά��
*/
static void maintainCMT(int multicastMain)
{
	int len = 2;
	int lastMaintain;
	Packet* mlme;
	FIN(maintainCMT(int multicastMain));
	if(multicastMain >= 0)
		{
		g_nwk_children_multicast_table.maintain = (multicastMain | (g_nwk_children_multicast_table.maintain));
		printf("Node %d received a multicast maintain = %d\n", g_nwk_my_ESN_address, multicastMain);
		//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d received a multicast maintain = %d\n", g_nwk_my_ESN_address, multicastMain);
		}
	else
		{
		--g_nwk_children_multicast_table.timerCount;
		if(g_nwk_children_multicast_table.timerCount <= 0)
			{
			g_nwk_children_multicast_table.group = g_nwk_children_multicast_table.maintain;
			g_nwk_children_multicast_table.maintain = 0;
			lastMaintain = g_nwk_mulcast_maintain;
			g_nwk_mulcast_maintain = (g_nwk_children_multicast_table.group | g_nwk_my_multicast_table);
			g_nwk_children_multicast_table.timerCount = NWK_TIMER_MUTICAST_MAINTAIN;
			/*
			ͨ��ԭ������MAC�鲥����
			*/
			if(g_nwk_mulcast_maintain == lastMaintain)	FOUT;//�鲥ά����Ϣû�з����仯����Ҫͨ��ԭ��ı�MAC���鲥ά����Ϣ����
			mlme = op_pk_create(0);
			op_pk_fd_set (mlme, 0, OPC_FIELD_TYPE_INTEGER, Multicast, 8);
			op_pk_fd_set (mlme, 1, OPC_FIELD_TYPE_INTEGER, g_nwk_mulcast_maintain, 16);
			op_pk_send(mlme, OUT_NWK_TO_MLME);
			}
		}
	FOUT;
}


/*
	���Ҷ̵�ַ��ӳ����е�λ�ü�ESN����ַ����������/����ַ
*/
static int quiryNWKAddressFromMap(int NWKAddress)
{
	int i;
	FIN(quiryNWKAddressFromMap(int NWKAddress));
	for(i = 1;i < NWK_MAX_NODE_NUM;i++)
	{
		if(NWKAddress == mapping[i].shortAddress)
		{
			FRET(i);
		}
	}
	FRET(-1);
}




/*
	����Ǳ�ڸ��ڵ���ж�Ӧ���ڵ��λ�ã���������
*/

static int queryFT(int fatherAddress)
{
	//int i;
	FIN(queryFT(int fatherAddress));
	/*
	for(i = 0;i<MAX_g_nwk_father_table_NUM;i++)
	{
		if(potentialParent[g_nwk_my_ESN_address][i].short_addr == fatherAddress)	FRET(potentialParent[g_nwk_my_ESN_address][i].band);
	}*/
	FRET(-1);
}

/*
	������·��������
	����ֵ��·��������
*/
static int RouterNumber()
{
	int i;
	int num = 0;
	FIN(RouterNumber());
	for(i = 0;i < NWK_MAX_CHILD_NUM;i++)
		{
		if((Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].CapabilityInformation>>4) == ROUTERNODE && Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].status == NWK_CHILD_TABLE_LEGITIMATE)
			{
			num++;
			}
		}
	FRET(num);
}
/*
	�����ӽڵ�����
	����ֵ���ӽڵ�����
*/
static int CTNumber()
{
	int i;
	int num = 0;
	FIN(CTNumber());
	for(i = 0;i < NWK_MAX_CHILD_NUM;i++)
		{
		if(Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].status == NWK_CHILD_TABLE_LEGITIMATE)
			{
			num++;
			}
		}
	FRET(num);
}
/*
	������һ�����ն˽ڵ�����
*/
static int EndNumber()
{
	int i;
	int num = 0;
	FIN(EndNumber());
	for(i = 0;i < NWK_MAX_CHILD_NUM;i++)
		{
		if((Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].CapabilityInformation>>4) == ENDNODE && Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].status == NWK_CHILD_TABLE_LEGITIMATE)
			{
			num++;
			}
		}
	FRET(num);
}




/*
	�����ַ
*/
static int nwk_distribute_address(int deviceType, int shortAddress, int ESNAddress)
{
	int i;
	int n;
	int p1 = -1, p2 = -1;
	FIN(nwk_distribute_address(int deviceType, int shortAddress, int ESNAddress));
	if(g_nwk_node_type == GATENODE)	n = NWK_MAX_NODE_NUM;
	else if(g_nwk_node_type == ROUTERNODE)	n = NWK_MAX_CHILD_NUM;
	else	FRET(-1);
	switch(deviceType)
	{
		//��·�ɽڵ�����ַ
		case ROUTERNODE: 
				//printf("ESNAddress = %d mapping[ESNAddress].shortAddress = %d\n", ESNAddress, mapping[ESNAddress].shortAddress);
				if(mapping[ESNAddress].shortAddress > 0)
					{
					//printf("111\n");
					FRET(mapping[ESNAddress].shortAddress);
					}
				for(i = 0;i < NWK_MAX_ROUTER_NUM;i++)//����ԭ�ȵĵ�ַ
				{
					//printf("ESNAddress = %d\n", g_nwk_end_address_pool[i].ESNAddress);
					if(g_nwk_node_type == GATENODE && g_nwk_router_address_pool[i].ESNAddress == ESNAddress)
					{
						g_nwk_router_address_pool[i].ESNAddress = ESNAddress;
						g_nwk_router_address_pool[i].count = NWK_COUNT_ADDRESS_POOL;
						shortAddress = (i + 1)<<8;
						//printf("222\n");
						FRET(shortAddress);
					}
					if(p1 < 0 && g_nwk_node_type == GATENODE && (g_nwk_router_address_pool[i].ESNAddress == -1 && g_nwk_router_address_pool[i].old == 0))
						{
						p1 = i;
						}
					if(p2 < 0 && g_nwk_node_type == GATENODE && (g_nwk_router_address_pool[i].ESNAddress == -1 && g_nwk_router_address_pool[i].old == 1))
						{
						p2 = i;
						}
				}
				
				if(p1 >= 0)
					{
					g_nwk_router_address_pool[p1].ESNAddress = ESNAddress;
					g_nwk_router_address_pool[p1].count = NWK_COUNT_ADDRESS_POOL;
					shortAddress = (p1 + 1)<<8;
					FRET(shortAddress);
					}
				if(p2 >= 0)
					{
					g_nwk_router_address_pool[p2].ESNAddress = ESNAddress;
					g_nwk_router_address_pool[p2].count = NWK_COUNT_ADDRESS_POOL;
					shortAddress = (p2 + 1)<<8;
					FRET(shortAddress);
					}
				break;
		//�����ݽڵ�����ַ,���䷽ʽ���ն˽ڵ�һ��
		case BACKUPNODE: 
		//���ն˽ڵ�����ַ
		case ENDNODE: 
				for(i = 0;i < n;i++)
				{
					if(g_nwk_end_address_pool[i].ESNAddress == ESNAddress)//����ԭ�ȵĵ�ַ
					{
						g_nwk_end_address_pool[i].ESNAddress = ESNAddress;
						g_nwk_end_address_pool[i].count = NWK_COUNT_ADDRESS_POOL;
						if(g_nwk_node_type == ROUTERNODE)	shortAddress = (i + 1)|g_nwk_my_short_address;
						else						shortAddress = (i + 1)|(1<<15);
						FRET(shortAddress);
					}
					if(p1 < 0 && g_nwk_end_address_pool[i].ESNAddress == -1 && g_nwk_end_address_pool[i].old == 0)
						{
						p1 = i;
						}
					if(p2 < 0 && g_nwk_end_address_pool[i].ESNAddress == -1 && g_nwk_end_address_pool[i].old == 1)
						{
						p2 = i;
						}
				}
				if(p1 >= 0)
					{
					g_nwk_end_address_pool[p1].ESNAddress = ESNAddress;
					g_nwk_end_address_pool[p1].count = NWK_COUNT_ADDRESS_POOL;
					if(g_nwk_node_type == ROUTERNODE)	shortAddress = (p1 + 1)|g_nwk_my_short_address;
					else						shortAddress = (p1 + 1)|(1<<15);
					FRET(shortAddress);
					}
				if(p2 >= 0)
					{
					g_nwk_end_address_pool[p2].ESNAddress = ESNAddress;
					g_nwk_end_address_pool[p2].count = NWK_COUNT_ADDRESS_POOL;
					if(g_nwk_node_type == ROUTERNODE)	shortAddress = (p2 + 1)|g_nwk_my_short_address;
					else						shortAddress = (p2 + 1)|(1<<15);
					FRET(shortAddress);
					}
				break;
		default:FRET(-1);
	}
	FRET(-1);
}

/*
	����Ƶ��
	������������Ľڵ��ַ
	����ֵ�������Ƶ�κţ�-1��ʾ����ʧ��
*/
static int nwk_distribute_band(int NWKAddress)
{
	int i;
	FIN(nwk_distribute_band(int NWKAddress));
	for(i = 1;i<NWK_MAX_ROUTER_NUM;i++)
	{
		if(bandTable[i]==0 || bandTable[i]==NWKAddress)
		{
			bandTable[i] = NWKAddress;
			FRET(i);
		}
	}
	FRET(-1);
}

/*
	���µ�ַ������
*/
static void nwk_update_address(int NWKAddress)
{
	int i;
	int gateFlag, routerFlag, endFlag;
	FIN(nwk_update_address(int NWKAddress));
	if(g_nwk_node_type == ENDNODE)	FOUT;//�ն��豸����Ҫά����ַ��
	if(NWKAddress > 0)//��ַ״̬����
	{
		printf("Node %d UpdateAddress NWKAddress = %d\n",g_nwk_my_ESN_address,NWKAddress);
		//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d UpdateAddress NWKAddress = %d\n",g_nwk_my_ESN_address,NWKAddress);
		gateFlag = (NWKAddress>>15);
		routerFlag = (NWKAddress>>8);
		endFlag = (NWKAddress & ( ~(255 << 8) ) );
		if(g_nwk_node_type == GATENODE && gateFlag == 1)
			{
			endFlag = (NWKAddress & ( ~(1 << 15) ) );
			g_nwk_end_address_pool[endFlag - 1].count = NWK_COUNT_ADDRESS_POOL;
			}
		else if(g_nwk_node_type == GATENODE && gateFlag == 0 && routerFlag > 0)
			{
			g_nwk_router_address_pool[routerFlag - 1].count = NWK_COUNT_ADDRESS_POOL;
			}
		else if(g_nwk_node_type == ROUTERNODE && gateFlag == 0 && routerFlag == (g_nwk_my_short_address>>8) && endFlag > 0)
			{
			g_nwk_end_address_pool[endFlag - 1].count = NWK_COUNT_ADDRESS_POOL;
			}
		FOUT;
	}
	//���ؽڵ��·�ɵ�ַ���ն˵�ַ���м������ݼ�
	if(g_nwk_node_type == GATENODE)
		{
		for(i = 0;i < NWK_MAX_ROUTER_NUM;i++)
			{
			if(g_nwk_router_address_pool[i].count == 0)	continue;
			--g_nwk_router_address_pool[i].count;
			if(g_nwk_router_address_pool[i].count <= 0)
				{
				if(g_nwk_router_address_pool[i].ESNAddress >= 0)
					{
					g_nwk_router_address_pool[i].ESNAddress = -1;
					g_nwk_router_address_pool[i].old = 1;
					g_nwk_router_address_pool[i].count = NWK_COUNT_ADDRESS_POOL_OLD;
					}
				else
					{
					g_nwk_router_address_pool[i].old = 0;
					//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d g_nwk_router_address_pool[%d].old = 0\n", g_nwk_my_ESN_address, i);
					}
				printf("Node %d g_nwk_router_address_pool[%d].ESNAddress = -1\n", g_nwk_my_ESN_address, i);
				//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d g_nwk_router_address_pool[%d].ESNAddress = -1\n", g_nwk_my_ESN_address, i);
				}
			}
		for(i = 0;i < NWK_MAX_NODE_NUM;i++)
			{
			if(g_nwk_end_address_pool[i].count == 0)	continue;
			--g_nwk_end_address_pool[i].count;
			if(g_nwk_end_address_pool[i].count <= 0)
				{
				if(g_nwk_end_address_pool[i].ESNAddress >= 0)
					{
					g_nwk_end_address_pool[i].ESNAddress = -1;
					g_nwk_end_address_pool[i].old = 1;
					g_nwk_end_address_pool[i].count = NWK_COUNT_ADDRESS_POOL_OLD;
					}
				else
					{
					g_nwk_end_address_pool[i].old = 0;
					}
				
				printf("Node %d g_nwk_end_address_pool[%d].ESNAddress = -1\n", g_nwk_my_ESN_address, i);
				//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d g_nwk_end_address_pool[%d].ESNAddress = -1\n", g_nwk_my_ESN_address, i);
				}
			}
		}
	//·�ɽڵ���ն˵�ַ���м������ݼ�
	if(g_nwk_node_type == ROUTERNODE)
		{
		for(i = 0;i < NWK_MAX_NODE_NUM;i++)
			{
			if(g_nwk_end_address_pool[i].count == 0)	continue;
			--g_nwk_end_address_pool[i].count;
			if(g_nwk_end_address_pool[i].count <= 0)
				{
				if(g_nwk_end_address_pool[i].ESNAddress >= 0)
					{
					g_nwk_end_address_pool[i].ESNAddress = -1;
					g_nwk_end_address_pool[i].old = 1;
					g_nwk_end_address_pool[i].count = NWK_COUNT_ADDRESS_POOL_OLD;
					}
				else
					{
					g_nwk_end_address_pool[i].old = 0;
					}
				printf("Node %d g_nwk_end_address_pool[%d].ESNAddress = -1\n", g_nwk_my_ESN_address, i);
				//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d g_nwk_end_address_pool[%d].ESNAddress = -1, Address %d released\n", g_nwk_my_ESN_address, i, i & g_nwk_my_short_address);
				}
			}
		}
	FOUT;
}

/*
	����Ƶ�㣬�����������
	������0,���ؽڵ�һ��ֱ�����նˣ�1,����
		  2,���ؽڵ�һ��ֱ����·��
*/
static int nwk_distribute_frequency(int type)
{
	int i = 0;
	int min_fre_num = 1000;
	int min_fre_index = 0;
	FIN(nwk_distribute_frequency(int type));
	if(g_nwk_node_type == GATENODE && type == 0)
		{
		FRET(op_dist_uniform (GATE_FREQNUM));
		}
	else if(type == 2) 
		{
		for(i = 0;i < 4;i++)
			{
			if(frequency_distr[i] < min_fre_num)
				{
				min_fre_num = frequency_distr[i];
				min_fre_index = i;
				}
			}
		frequency_distr[min_fre_index]++;
		FRET(min_fre_index);
		}
	//if(deviceType == GATENODE)	FRET(op_dist_uniform (GATEFREQNUM));			
	//if(deviceType == ROUTERNODE)	FRET(op_dist_uniform (FREQNUM));
	FRET(0);
}

/*
	���͵�ַ��ͻ���ַ���
*/
static void nwk_send_conflict(int shortAddress)
{
	int len, pklen;
	int control = 0;
	Packet* pk;
	FIN(nwk_send_conflict(int shortAddress));
	len = 1;
	pk = op_pk_create(0);
	op_pk_fd_set (pk, 0, OPC_FIELD_TYPE_INTEGER, control, 8);
	pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + SHORTADDR_LEN + LENGTH_LEN;
	pklen += 8*len;
	nwk_generate_packet(pklen,0,1,2,2,g_nwk_my_short_address, shortAddress, 9, 0, len, pk, OUT_NWK_TO_MCPS_DOWN,0);
	FOUT;
}

/*
	��ַӳ�����Ϣ�޸�
*/
static void nwk_update_mapping(int ESN_address, int NWK_address)
{
	FIN(nwk_update_mapping(int ESN_address, int NWK_address));
	mapping[ESN_address].shortAddress = NWK_address;
	mapping[ESN_address].count = NWK_COUNT_MAPPING_TABLE;
	printf("Node %d maping[%d].shortAddress = %d\n", g_nwk_my_ESN_address, ESN_address, NWK_address);
	//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d maping[%d].ESNAddress = %d\n", ESN_address, NWK_address);
	FOUT;
}

/*
	��������Դ�ͷź���������
	������
		NWKAddress�������ڵ�Ķ̵�ַ,���ڱ��ڵ�̵�ַʱ��ʾ���ڵ�����;
		rejoin���Ƿ���Ҫ����������ֻ�б��ڵ�����ʱ������Ч;0������Ҫ,1����Ǳ�ڸ��ڵ���в����µĸ��ڵ����,2:����ԭ���ĸ��ڵ����½���,3:ʹ��ָ�����ڵ����;
		children���ӽڵ��Ƿ���������ʱδ��;
		assign��ָ�����ֽ��뷽ʽ��0�����·�����Դ��1��ʹ�þ���Դ
*/

static void nwk_leave_net(int NWKAddress,int rejoin,int children,int assign)
{
	int i;
	int GateIndex, EndIndex, routerIndex, ESNAddress;
	int index;
	int band;
	int len = 1;
	int pklen = 0;
	
	Packet* payLoad;
	FIN(nwk_leave_net(int NWKAddress,int rejoin,int children,int assign));
	if(NWKAddress == g_nwk_my_short_address)//���ڵ���������
		{
		if(assign == 1)	g_nwk_join_type = 0;
		else if(assign == 0) g_nwk_join_type = 1;
		if(rejoin == 1)
			{
			g_nwk_specify_father = 0;
			index = queryFT(g_nwk_my_father_short_address);
			if(index >= 0) potentialParent.father[g_nwk_my_ESN_address].father[index].times++;
			}
		else if(rejoin == 2)
			{
			g_nwk_specify_father = 1;
			}
		if(CTNumber() > 0)
			{
			payLoad = op_pk_create (0);
			op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,0x17,8);
			nwk_generate_packet(pklen,0,0,2,2,g_nwk_my_short_address,ChildrenCast,2,NWK_MAX_RADIUS-1, len, payLoad, OUT_NWK_TO_MCPS_DOWN,0);//���ӽڵ�㲥��������
			}
		op_intrpt_schedule_self(op_sim_time(),intrCode_ReJoin);
		}
	else
		{
		GateIndex = (NWKAddress>>15);//������
		if(GateIndex == 1)
		{
			EndIndex = (NWKAddress&32767);
			routerIndex = 0;
		}
		else
		{
			EndIndex = (NWKAddress&255);
			routerIndex = (NWKAddress>>8);
		}
		printf("EndIndex = %d,routerIndex = %d\n",EndIndex,routerIndex);
		//op_prg_log_entry_write(g_nwk_debugger_log_handle, "EndIndex = %d,routerIndex = %d\n",EndIndex,routerIndex);
		ESNAddress = quiryNWKAddressFromMap(NWKAddress);
		DeleteCT(2, NWKAddress);
		if(EndIndex > 0)
		{
			if(g_nwk_node_type == GATENODE)
			{
				printf("ENDAddress = %d\n",mapping[ESNAddress].shortAddress);
				//op_prg_log_entry_write(g_nwk_debugger_log_handle, "ENDAddress = %d\n",mapping[ESNAddress].shortAddress);
				mapping[ESNAddress].shortAddress = 0;
				mapping[ESNAddress].count = -1;
			}
		}
		else if(routerIndex > 0)
		{
			deleteRouting(-1,NWKAddress);
			if(g_nwk_node_type == GATENODE)
			{
				band = bandQuire(NWKAddress);
				//if(band>=0) bandTable[band] = 0;
				mapping[ESNAddress].shortAddress = -1;
				mapping[ESNAddress].count = -1;
				printf("band = %d,ESNAddress = %d,router = %d\n",band,ESNAddress,g_nwk_router_address_pool[routerIndex - 1]);
				//op_prg_log_entry_write(g_nwk_debugger_log_handle, "band = %d,ESNAddress = %d,router = %d\n",band,ESNAddress,g_nwk_router_address_pool[routerIndex - 1]);
				if(children == 1)
				{
					for(i = 0;i < NWK_MAX_NODE_NUM;i++)
					{
						if((mapping[i].shortAddress>>8) == routerIndex)
						{
							mapping[i].shortAddress = 0;
							mapping[i].count = -1;
						}
					}
				}
			}
		}
		}
	FOUT;
}

/*

*/
static void nwk_leave_set()
{
	FIN(nwk_leave_set());
	op_ima_obj_attr_set(g_nwk_nodeID,"g_node_status",0);
	if(g_nwk_node_type != ENDNODE) nwk_init_router();
	if(onlineNodes[g_nwk_my_ESN_address] == 1)
		{
		op_ima_obj_attr_set(g_nwk_nodeID,"g_node_short_address",0);
		onlineNodes[g_nwk_my_ESN_address] = 0;
		onlineNodesNUmber--;
		op_stat_write (online_number_gstathandle, 		onlineNodesNUmber);
		leave_node_number++;
		op_stat_write (leave_node_count, 		leave_node_number);
		}
	
	
	//op_prg_log_entry_write(g_nwk_debugger_log_handle, "leave set success\n");
	FOUT;
}

/*
	��¼��ͬ������
*/
static void nwk_push_synchronize_queue(int status, int rstart, int rend,int cstart, int cend)
{
	int index;
	int i = 0, n = 0;
	FIN(nwk_push_synchronize_queue(int status, int rstart, int rend,int cstart, int cend));
	if(g_nwk_synchronize_queue.p_end == g_nwk_synchronize_queue.p_start && g_nwk_synchronize_queue.number > 0)
		{
		printf("Node %d nwk_push_synchronize_queue full\n", g_nwk_my_ESN_address);
		//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d nwk_push_synchronize_queue full\n", g_nwk_my_ESN_address);
		FOUT;
		}
	index = g_nwk_synchronize_queue.p_end;
	
	if(rstart == -1 && cstart >= 0)
		{
		for(i = cstart;i < cend; i++)
			{
			if(i >= Network_Msg.NetworkCT[g_nwk_my_ESN_address].number)
				{
				cend = i;
				break;
				}
			if(Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].status == NWK_CHILD_TABLE_LEGITIMATE && status == 1)
				{
				n++;
				}
			else if(Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].status == NWK_CHILD_TABLE_ILLEGAL && status == 2)
				{
				n++;
				}
			}
		if(n == 0) 
			{
			//printf("1231341234\n");
			FOUT;
			}
		}
	if(g_nwk_synchronize_queue.number == 0)
		{
		index = g_nwk_synchronize_queue.p_start;
		g_nwk_synchronize_queue.p_end = g_nwk_synchronize_queue.p_start;
		if(g_nwk_synchronize_queue.p_end >= NWK_QUEUE_LEN)
			{
			g_nwk_synchronize_queue.p_end = 0;
			}
		g_nwk_synchronize_queue.count = 0;
		}
	g_nwk_synchronize_queue.table[index].sequence = g_nwk_synchronize_sequence;
	g_nwk_synchronize_queue.table[index].status = status;
	g_nwk_synchronize_queue.table[index].routerStart = rstart;
	g_nwk_synchronize_queue.table[index].routerEnd = rend;
	g_nwk_synchronize_queue.table[index].childStart = cstart;
	g_nwk_synchronize_queue.table[index].childEnd = cend;
	g_nwk_synchronize_queue.number++;
	g_nwk_synchronize_sequence++;
	g_nwk_synchronize_queue.p_end++;
	if(g_nwk_synchronize_queue.p_end >= NWK_QUEUE_LEN)
		{
		g_nwk_synchronize_queue.p_end = 0;
		}
	
	
	
	printf("Node %d nwk_push_synchronize_queue sucessful,index = %d, status = %d,sequence = %d , rstart = %d, cstart= %d, number = %d  \n", g_nwk_my_ESN_address, index, status, g_nwk_synchronize_sequence, rstart, cstart, g_nwk_synchronize_queue.number);
	
	if(g_nwk_synchronize_queue.number == 1)
		{
		printf("Node %d nwk want to send a backup pk\n", g_nwk_my_ESN_address);
		nwk_send_synchronize_queue();
		}
	
	FOUT;
}

/*
	����ͬ�������еļ�¼����ͬ����Ϣ
*/
static void nwk_send_synchronize_queue()
{
	int num, status,pstart, rstart, rend, cstart, cend;
	int rnum, cnum;
	int NWKAddress;
	FIN(nwk_send_synchronize_queue());
	num = g_nwk_synchronize_queue.number;
	if(num <= 0 || g_nwk_synchronize_queue.p_start == g_nwk_synchronize_queue.p_end)
		{
		//printf("Node %d does not have synchronize to send\n", g_nwk_my_ESN_address);
		//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d does not have synchronize to send\n", g_nwk_my_ESN_address);
		FOUT;
		}
	pstart = g_nwk_synchronize_queue.p_start;
	g_nwk_synchronize_queue.count = NWK_COUNT_BACKUP;
	status = g_nwk_synchronize_queue.table[pstart].status;
	rstart = g_nwk_synchronize_queue.table[pstart].routerStart;
	rend = g_nwk_synchronize_queue.table[pstart].routerEnd;
	cstart = g_nwk_synchronize_queue.table[pstart].childStart;
	cend = g_nwk_synchronize_queue.table[pstart].childEnd;
	if(status == 0)
		{
		printf("nwk_send_synchronize_queue status = 0, p_start = %d, seq = %d\n", g_nwk_synchronize_queue.p_start, g_nwk_synchronize_queue.table[pstart].sequence);
		nwk_send_backup_info(3,-1,-1,-1,-1);
		//op_prg_log_entry_write(g_nwk_debugger_log_handle, "nwk_init_send_backup_info\n");
		}
	else if(status == 1 || status == 2)
		{
		rnum = rend - rstart;
		cnum = cend - cstart;
		if(rnum == 1 && cnum == 1)
			{
			NWKAddress = Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[cstart].NWKAddress;
			if(rnum ==1 && NWKAddress != g_nwk_routing_table.table[rstart].destAddress)
				{
				printf("Node %d nwk_send_synchronize_queue error for CT and g_nwk_routing_table NWKAddress defferent!\n", g_nwk_my_ESN_address);
				//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d nwk_send_synchronize_queue error for CT and g_nwk_routing_table NWKAddress defferent!\n", g_nwk_my_ESN_address);
				}
			nwk_send_backup_info(2,status - 1,NWKAddress,-1,1);
			}
		else if(rnum >= 1 && cnum >= 1)
			{
			nwk_send_backup_info(0,status - 1,-1,rstart,rnum);
			nwk_send_backup_info(1,status - 1,-1,cstart,cnum);
			}
		else if(rnum >= 1 && cnum == 0)
			{
			nwk_send_backup_info(0,status - 1,-1,rstart,rnum);
			}
		else if(rnum == 0 && cnum >= 1)
			{
			nwk_send_backup_info(1,status - 1,-1,cstart,cnum);
			}
		else 
			{
			printf("rnum = %d, cnum = %d\n", rnum, cnum);
			}
		}

	FOUT;
}



/*
	��·����������
*/

static void nwk_init_send_backup_info()
{
	int i;
	int step = 0;
	int number;
	FIN(nwk_init_send_backup_info());
	printf("nwk_init_send_backup_info\n");
	//op_prg_log_entry_write(g_nwk_info_log_handle, "nwk_init_send_backup_info\n");
	nwk_push_synchronize_queue(0, -1, -1, -1, -1);
	number = g_nwk_routing_table.number;
	printf("nwk_init_send_backup_info, g_nwk_routing_table.number = %d,Network_Msg.NetworkCT.number = %d \n", g_nwk_routing_table.number, Network_Msg.NetworkCT[g_nwk_my_ESN_address].number);
	//op_prg_log_entry_write(g_nwk_info_log_handle, "nwk_init_send_backup_info\n");
	for(i = 0;i<number;i += step)
	{
		step = (number - i) > NWK_MAX_ROUTER_TABLE_INFO_NUM ? NWK_MAX_ROUTER_TABLE_INFO_NUM : (number - i);
		nwk_push_synchronize_queue(1, 0, step, -1, -1);
		//nwk_send_backup_info(0,0,0,i,step);
	}
	step = 0;
	number = Network_Msg.NetworkCT[g_nwk_my_ESN_address].number;
	for(i = 0;i<number;i += step)
	{
		step = (number - i) > NWK_MAX_CHILD_TABLE_INFO_NUM ? NWK_MAX_CHILD_TABLE_INFO_NUM : (number - i);
		nwk_push_synchronize_queue(1, -1, -1, 0, step);
		//nwk_send_backup_info(1,0,0,i,step);
	}
	FOUT;
}

/*
	���ͱ�������
*/
static void nwk_send_backup_info(int type,int addOrReduce,int NWKAddress,int start,int num)
{
	Packet* payLoad;
	int i;
	int index = 1;
	int len = 2;
	int pklen;
	int entry_len;
	int num_max;
	int number;
	
	int infoControl = 0;
	int band;
	
	int RTIndex = -1;
	int CTIndex = -1;
	FIN(nwk_send_backup_info(int type,int addOrReduce,int NWKAddress,int start,int num));
	payLoad = op_pk_create (0);
	printf("send a backup g_nwk_syn_seq = %d, type = %d", g_nwk_syn_seq, type);
	op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,g_nwk_syn_seq,8);
	switch(type)
	{
	case 0://g_nwk_routing_table Backup
			entry_len = 4;
			num = num > (g_nwk_routing_table.number - start)?(g_nwk_routing_table.number - start):num;
			num_max = num > NWK_MAX_ROUTER_TABLE_INFO_NUM ? NWK_MAX_ROUTER_TABLE_INFO_NUM:num;
			len += (num_max * entry_len);
			infoControl |= (type<<5)|(addOrReduce<<4)|(num_max);
			op_pk_fd_set(payLoad,1,OPC_FIELD_TYPE_INTEGER,infoControl,8);
			for(i = 0;i<num_max;i++)
			{
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,g_nwk_routing_table.table[start].destAddress,8);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,g_nwk_routing_table.table[start].nextHop,8);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,g_nwk_routing_table.table[start].status,4);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,g_nwk_routing_table.table[start].routerSeq,4);
				start++;
			}
			pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + SHORTADDR_LEN + LENGTH_LEN;
			pklen += 8*len;
			nwk_generate_packet(pklen,0,1,2,2,g_nwk_my_short_address,g_nwk_backup_router,13,0,len,payLoad,OUT_NWK_TO_MCPS_DOWN,0);
			printf("Node %d send a g_nwk_routing_table backup num = %d, g_nwk_syn_seq = %d\n",g_nwk_my_ESN_address,num_max, g_nwk_syn_seq);
			//op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d send a g_nwk_routing_table backup num = %d\n",g_nwk_my_ESN_address,num_max);
			break;
	case 1://CT Backup
			entry_len = 9;
			num = num > (Network_Msg.NetworkCT[g_nwk_my_ESN_address].number - start)?(Network_Msg.NetworkCT[g_nwk_my_ESN_address].number - start):num;
			num_max = num > NWK_MAX_CHILD_TABLE_INFO_NUM ? NWK_MAX_CHILD_TABLE_INFO_NUM:num;
			number = 0;
			if(num_max == 0)	break;
			//len += (number * entry_len);
			//infoControl |= (type<<5)|(addOrReduce<<4)|(number);
			//payLoad = op_pk_create (0);
			//op_pk_fd_set(payLoad,1,OPC_FIELD_TYPE_INTEGER,infoControl,8);
			for(i = 0;i<num_max;i++)
			{
				if(Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[start].status != NWK_CHILD_TABLE_LEGITIMATE)
				{
					start++;
					continue;
				}
				number++;
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[start].ESNAddress,48);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[start].NWKAddress,16);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[start].CapabilityInformation,8);
				start++;
			}
			if(number == 0)
				{
				//printf("33333\n");
				// ��mac������
				/*
				g_nwk_synchronize_queue.p_start++;
				if(g_nwk_synchronize_queue.p_start >= NWK_QUEUE_LEN)
					{
					g_nwk_synchronize_queue.p_start = 0;
					}	
				g_nwk_syn_seq++;
				g_nwk_synchronize_queue.number--;
				g_nwk_synchronize_queue.count = NWK_COUNT_BACKUP;
				printf("no send a backup pk, number = %d, p_start = %d\n", g_nwk_synchronize_queue.number, g_nwk_synchronize_queue.p_start);
				*/
				break;
				}
			len += (number * entry_len);
			infoControl |= (type<<5)|(addOrReduce<<4)|(number);
			op_pk_fd_set(payLoad,1,OPC_FIELD_TYPE_INTEGER,infoControl,8);
			pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + SHORTADDR_LEN + LENGTH_LEN;
			pklen += 8*len;
			nwk_generate_packet(pklen,0,1,2,2,g_nwk_my_short_address,g_nwk_backup_router,13,0,len,payLoad,OUT_NWK_TO_MCPS_DOWN,0);
			printf("Node %d send a CT backup num = %d\n",g_nwk_my_ESN_address,num_max);
			//op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d send a CT backup num = %d\n",g_nwk_my_ESN_address,num_max);
			break;
	case 2://g_nwk_routing_table and CT Backup
			entry_len = 0;
			for(i = 0;i<g_nwk_routing_table.number;i++)
			{
				if(g_nwk_routing_table.table[i].destAddress == NWKAddress)
				{
					RTIndex = i;
					entry_len += 40;
					break;
				}
			}
			for(i = 0;i<Network_Msg.NetworkCT[g_nwk_my_ESN_address].number;i++)
			{
				if(Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[i].NWKAddress == NWKAddress)
				{
					CTIndex = i;
					entry_len += 72;
					break;
				}
			}
			len += entry_len;
			infoControl |= (type<<5)|(addOrReduce<<4)|1;
			//payLoad = op_pk_create (0);
			op_pk_fd_set(payLoad,1,OPC_FIELD_TYPE_INTEGER,infoControl,8);
			if(RTIndex >= 0)
			{
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,g_nwk_routing_table.table[RTIndex].destAddress,16);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,g_nwk_routing_table.table[RTIndex].nextHop,16);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,g_nwk_routing_table.table[RTIndex].status,4);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,g_nwk_routing_table.table[RTIndex].routerSeq,4);
			}
			if(CTIndex >= 0)
			{
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[CTIndex].ESNAddress,48);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[CTIndex].NWKAddress,16);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,Network_Msg.NetworkCT[g_nwk_my_ESN_address].table[CTIndex].CapabilityInformation,8);
			}
			pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + SHORTADDR_LEN + LENGTH_LEN;
			pklen += 8*len;
			nwk_generate_packet(pklen,0,1,2,2,g_nwk_my_short_address,g_nwk_backup_router,13,0,len,payLoad,OUT_NWK_TO_MCPS_DOWN,0);
			printf("Node %d send a CRT backup\n",g_nwk_my_ESN_address);
			//op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d send a CRT backup\n",g_nwk_my_ESN_address);
			break;
	case 3://g_nwk_main_address's Info
			entry_len = 3;
			num_max = 1;
			len += (num_max * entry_len);
			infoControl |= (type<<5)|(addOrReduce<<4)|(num_max);
			band = g_nwk_father_table.Band;
			op_pk_fd_set(payLoad,1,OPC_FIELD_TYPE_INTEGER,infoControl,8);
			op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,g_nwk_my_father_short_address,16);
			op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,band,4);
			op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,g_nwk_my_frequency,4);
			pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + SHORTADDR_LEN + LENGTH_LEN;
			pklen += 8*len;
			nwk_generate_packet(pklen,0,1,2,2,g_nwk_my_short_address,g_nwk_backup_router,13,0,len,payLoad,OUT_NWK_TO_MCPS_DOWN,0);
			printf("Node %d send a Info backup num = %d\n",g_nwk_my_ESN_address,num_max);
			//op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d send a Info backup num = %d\n",g_nwk_my_ESN_address,num_max);
			
			
			
			break;
	}
	for(i = 0; i < g_nwk_routing_table.number; i++)
		{
		//op_prg_log_entry_write(g_nwk_debugger_log_handle, "destAddress = %d, nextHop = %d, status = %d, routerSeq = %d\n", g_nwk_routing_table.table[i].destAddress, g_nwk_routing_table.table[i].nextHop, g_nwk_routing_table.table[i].status, g_nwk_routing_table.table[i].routerSeq);
		}
	
	
	
	
	
	
	
	//printf("update\n");
	
	// ��mac������
	
	/*
	
	g_nwk_synchronize_queue.p_start++;
	if(g_nwk_synchronize_queue.p_start >= NWK_QUEUE_LEN)
		{
		g_nwk_synchronize_queue.p_start = 0;
		}
	g_nwk_syn_seq++;
	g_nwk_synchronize_queue.number--;
	g_nwk_synchronize_queue.count = NWK_COUNT_BACKUP;
	
	*/
	
	
	
	
	
	
	
	
	
	FOUT;
}

/*
	֪ͨ���������ڵ�ı��ݽڵ����
*/

static void nwk_send_3p_notice(int shortAddress, int band)
{
	Packet* pk;
	int len, pklen;
	FIN(nwk_send_3p_notice(int shortAddress, int band));
	pk = op_pk_create (0);
	op_pk_fd_set (pk, 0, OPC_FIELD_TYPE_INTEGER, 4, 8);// ������·����Ϣ���������Ϊ֪ͨ�������ڵ�����
	op_pk_fd_set (pk, 1, OPC_FIELD_TYPE_INTEGER, g_nwk_my_father_short_address, 16);
	len = 3;
	pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + SHORTADDR_LEN + LENGTH_LEN;
	pklen += 8*len;
	nwk_generate_packet(pklen, 0, 1, 2, 2, g_nwk_my_short_address, shortAddress, 12, 1, len, pk, OUT_NWK_TO_MCPS_DOWN, 0);
	printf("Node %d send a 3p notice pk to %d\n", g_nwk_my_ESN_address, shortAddress);
	FOUT;
}

/*
	��MAC����ԭ��֪ͨMAC����������·�ɽڵ�
*/
static void nwk_3p_master_monitor()
{

	Packet* mlme;
	FIN(nwk_3p_master_monitor());
	if(g_nwk_node_type != ROUTERNODE && g_nwk_node_type != GATENODE)	FOUT;
	mlme = op_pk_create (0);
	op_pk_fd_set (mlme, 0, OPC_FIELD_TYPE_INTEGER, MASTER_MONITOR, 8);
	op_pk_fd_set (mlme, 1, OPC_FIELD_TYPE_INTEGER, g_nwk_main_address, 16);
	op_pk_send(mlme, OUT_NWK_TO_MLME);
	FOUT;
}

/*
	���ڼ����·��(δʹ��)
*/
static void nwk_master_monitor()
{

	Packet* mlme;
	FIN(nwk_master_monitor());
	if(g_nwk_node_type != BACKUPNODE)	FOUT;
	op_intrpt_schedule_self(op_sim_time() + NWK_TIMER_MASTER_MONITOR, intrCode_MASTER_MONITOR);
	mlme = op_pk_create (0);
	op_pk_fd_set (mlme, 0, OPC_FIELD_TYPE_INTEGER, MASTER_MONITOR, 8);
	op_pk_fd_set (mlme, 1, OPC_FIELD_TYPE_INTEGER, g_nwk_main_address, 16);
	op_pk_send(mlme, OUT_NWK_TO_MLME);
	FOUT;
}

/*
	��·��ʧ���
*/
static void nwk_master_inactivated_process()
{
	FIN(nwk_master_inactivated_process());
	if(g_nwk_beacon_confirm_count == 0) 
		{
		nwk_send_master_status(g_nwk_main_router_father, 0, g_nwk_main_address, -1);
		g_nwk_beacon_confirm_count = NWK_COUNT_VIRIFY;
		printf("Node %d send a 3p verify pk to %d for mainRouter %d down\n", g_nwk_my_ESN_address, g_nwk_main_router_father, g_nwk_main_address);
		}
	else g_nwk_beacon_confirm_count--;
	
	FOUT;
}


/*
	���͵�����״̬����
*/
static void nwk_send_master_status(int dest, int request, int short_address, int status)
{
	Packet* payLoad;
	int len = 3;
	int pklen;
	FIN(nwk_send_master_status_req(int dest, int request, int short_address));
	if(request == 0)
		{
		payLoad = op_pk_create (0);
		op_pk_fd_set(payLoad,0, OPC_FIELD_TYPE_INTEGER,request,8);
		op_pk_fd_set(payLoad,1, OPC_FIELD_TYPE_INTEGER,short_address,16);
		pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + SHORTADDR_LEN + LENGTH_LEN;
		pklen += 8*len;
		nwk_generate_packet(pklen,0,1,2,2,g_nwk_my_short_address,dest,4,0,len,payLoad,OUT_NWK_TO_MCPS_DOWN,0);
		}
	else if(request == 1)
		{
		//printf("status = %d\n", status);
		request = ((request<<7)|status);
		payLoad = op_pk_create (0);
		op_pk_fd_set(payLoad,0, OPC_FIELD_TYPE_INTEGER,request,8);
		len = 1;
		pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + SHORTADDR_LEN + LENGTH_LEN;
		pklen += 8*len;
		nwk_generate_packet(pklen,0,1,2,2,g_nwk_my_short_address,dest,4,0,len,payLoad,OUT_NWK_TO_MCPS_DOWN,0);
		}
	FOUT;
}

/*
	��������֤������մ���
*/
static void nwk_master_status_process(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int control;
	int mainRouterAddress;
	int request, status;
	FIN(nwk_master_status_process(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	op_pk_fd_get(payLoad,0,&control);
	request = (control>>7);
	if(request == 0)
		{
		op_pk_fd_get(payLoad,1,&mainRouterAddress);
		/****************************************
		*****************************************
		Ҫ��MAC����������֤���
		*****************************************
		****************************************/
		
		nwk_keep_alive_request(mainRouterAddress, 1);
		
		}
	else if(request == 1)
		{
		status = control & (127);
		if(status == 0) // working
			{
			printf("Node %d recv a working res main states\n", g_nwk_my_ESN_address);
			FOUT;
			}
		else if(status == 0x7f && g_nwk_beacon_confirm > 0) // Fault
			{
			printf("Node %d recv a fault res main states\n", g_nwk_my_ESN_address);
			nwk_change_slave2master();
			}
		}
	FOUT;
}


/*
	����·�ɽڵ��滻���ڵ�
*/
static void nwk_change_slave2master()
{
	int len = 8;
	//int pklen;
	FIN(nwk_change_slave2master());
	if(g_nwk_node_type != BACKUPNODE)	FOUT;
	g_nwk_my_short_address = g_nwk_main_address;
	g_nwk_myBand = g_nwk_father_table.Band;
	g_nwk_node_type = ROUTERNODE;
	g_nwk_my_father_short_address = g_nwk_main_router_father;
	g_nwk_father_table.NWKAddress = g_nwk_main_router_father;
	g_nwk_father_table.Band = g_nwk_main_router_band;
	g_nwk_father_table.Frequency = g_nwk_main_router_frequency;
	/*
	payLoad = op_pk_create (0);
	op_pk_fd_set(payLoad,0, OPC_FIELD_TYPE_INTEGER,g_nwk_my_short_address,16);
	op_pk_fd_set(payLoad,0, OPC_FIELD_TYPE_INTEGER,g_nwk_my_ESN_address,48);
	pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
	pklen += 8*len;
	nwk_generate_packet(pklen,0,1,2,1,g_nwk_my_short_address,GateAddress,3,NWK_MAX_RADIUS - 1,len,payLoad,OUT_NWK_TO_MCPS_UP,0);
	*/
	op_ima_obj_attr_set(g_nwk_nodeID,"g_node_type",g_nwk_node_type);
	op_ima_obj_attr_set(g_nwk_nodeID,"g_node_short_address",g_nwk_my_short_address);
	op_ima_obj_attr_set(g_nwk_nodeID,"g_node_band",g_nwk_myBand);
	op_ima_obj_attr_set(g_nwk_nodeID,"g_node_frequency",g_nwk_my_frequency);
	nwk_leave_request(0);
	FOUT;
}

/*
	���������л���֪ͨ���������ϵ�֪ͨ�Ͷ��µĹ㲥
*/
static void nwk_slave2master_notice(int flag)
{
	int len, pklen;
	Packet* payLoad;
	FIN(nwk_slave2master_notice(int flag));
	payLoad = op_pk_create (0);
	if(flag == 0)
		{
		op_pk_fd_set(payLoad,0, OPC_FIELD_TYPE_INTEGER,g_nwk_my_short_address,16);
		op_pk_fd_set(payLoad,1, OPC_FIELD_TYPE_INTEGER,g_nwk_my_ESN_address,48);
		pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
		len = 8;
		pklen += 8 * len;
		nwk_generate_packet(pklen,0,1,2,1,g_nwk_my_short_address,GateAddress,3,NWK_MAX_RADIUS - 1,len,payLoad,OUT_NWK_TO_MCPS_UP,0);
		}
	else
		{
		printf("zzzzzz\n");
		if(CTNumber() <= 0) FOUT;
		op_pk_fd_set(payLoad,0, OPC_FIELD_TYPE_INTEGER,g_nwk_my_short_address,16);
		op_pk_fd_set(payLoad,1, OPC_FIELD_TYPE_INTEGER,g_nwk_my_ESN_address,48);
		pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
		len = 8;
		pklen += 8 * len;
		nwk_generate_packet(pklen,0,0,2,2,g_nwk_my_short_address,ChildrenCast,3,0,len,payLoad,OUT_NWK_TO_MCPS_DOWN,0);
		}
	FOUT;
}


/*
	��ѯ����ӳ���
*/
static int g_nwk_query_r2b_map(int mainRouter)
	{
	int i;
	int index;
	FIN(g_nwk_query_r2b_map(int mainRouter));
	index = g_nwk_r2b_Map.p;
	for(i = 0; i < g_nwk_r2b_Map.num; i++)
		{
		index += i;
		index %= NWK_MAX_CHILD_NUM;
		if(g_nwk_r2b_Map.entrys[index].Router == mainRouter)
			{
			FRET(g_nwk_r2b_Map.entrys[index].Backup);
			}
		}
	FRET(-1);
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
	void WSN_NWK_Process (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_WSN_NWK_Process_init (int * init_block_ptr);
	void _op_WSN_NWK_Process_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_WSN_NWK_Process_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_WSN_NWK_Process_alloc (VosT_Obtype, int);
	void _op_WSN_NWK_Process_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
WSN_NWK_Process (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (WSN_NWK_Process ());

		{
		/* Temporary Variables */
		int i = 0;
		/* End of Temporary Variables */


		FSM_ENTER ("WSN_NWK_Process")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (INIT) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "INIT", "WSN_NWK_Process [INIT enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [INIT enter execs]", state0_enter_exec)
				{
				
				PrgT_Log_Severity_Type   severity;
				Compcode                 status;
				unsigned int conf_log_limit = 10000;
				
				
				//-------------------------------My Parameters Declaration-------------------------------//
				g_nwk_nodeID = op_topo_parent(op_id_self());
				op_ima_obj_attr_get(g_nwk_nodeID,"g_node_ESN_address",&g_nwk_my_ESN_address);
				op_ima_obj_attr_get(g_nwk_nodeID,"g_node_type",&g_nwk_node_type);
				//op_ima_obj_attr_get(g_nwk_nodeID,"father",&g_nwk_my_father_short_address);
				op_ima_obj_attr_get(g_nwk_nodeID,"g_node_main_router_id",&g_nwk_main_routing_address);
				op_ima_obj_attr_get(g_nwk_nodeID,"g_test_type",&g_nwk_test_type);
				
				//printf("Node %d start!\n", g_nwk_my_ESN_address);
				
				g_nwk_join_type = 0;
				g_nwk_capability = 0;
				g_nwk_backup_router = 0;
				g_nwk_available_potential_father_number = 0;
				g_nwk_specify_father = 0;
				g_nwk_main_address = 0;
				g_nwk_syn_seq = 0;
				g_nwk_synchronize_sequence = 0;
				g_nwk_beacon_confirm_count = 0;
				g_nwk_reassociate_leave_flag = 0; 
				if(g_nwk_node_type != BACKUPNODE)	g_nwk_capability |= g_nwk_node_type<<4;
				else						g_nwk_capability |= ENDNODE<<4;
				g_nwk_my_short_address = 0;
				g_nwk_result = 0;
				g_nwk_join_time = 0;
				
				
				if(g_nwk_node_type == GATENODE )
					{
					g_nwk_my_short_address = GateAddress;
					nwk_init_gate();
					}
				else if(g_nwk_node_type == ROUTERNODE)
				{
					nwk_init_router();
				}
				
				g_nwk_info_log_handle = op_prg_log_handle_create(OpC_Log_Category_Configuration, "NWK", "info", conf_log_limit);
				
				status = op_prg_log_handle_severity_get (g_nwk_info_log_handle, &severity);
				if ((status != OPC_COMPCODE_FAILURE) && (severity != PrgC_Log_Severity_Information))
				   op_prg_log_handle_severity_set(&g_nwk_info_log_handle, PrgC_Log_Severity_Information);
				
				g_nwk_debugger_log_handle = op_prg_log_handle_create(OpC_Log_Category_Configuration, "NWK", "debugger", 1);
				
				status = op_prg_log_handle_severity_get (g_nwk_debugger_log_handle, &severity);
				if ((status != OPC_COMPCODE_FAILURE) && (severity != PrgC_Log_Severity_Debug ))
				   op_prg_log_handle_severity_set(&g_nwk_debugger_log_handle, PrgC_Log_Severity_Debug );
				
				
				
				
				
				//op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d start!\n", g_nwk_my_ESN_address);
				
				
				
				/* Initilaize the statistic handles to keep	*/
				/* track of traffic sinked by this process.	*/
				bits_rcvd_stathandle 		= op_stat_reg ("Traffic NWK.Traffic Received (bits)",			OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				bitssec_rcvd_stathandle 	= op_stat_reg ("Traffic NWK.Traffic Received (bits/sec)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				pkts_rcvd_stathandle 		= op_stat_reg ("Traffic NWK.Traffic Received (packets)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				pktssec_rcvd_stathandle 	= op_stat_reg ("Traffic NWK.Traffic Received (packets/sec)",	OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				ete_delay_stathandle		= op_stat_reg ("Traffic NWK.End-to-End Delay (seconds)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				
				bits_rcvd_gstathandle 		= op_stat_reg ("Traffic NWK.Traffic Received (bits)",			OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				bitssec_rcvd_gstathandle 	= op_stat_reg ("Traffic NWK.Traffic Received (bits/sec)",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				pkts_rcvd_gstathandle 		= op_stat_reg ("Traffic NWK.Traffic Received (packets)",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				pktssec_rcvd_gstathandle 	= op_stat_reg ("Traffic NWK.Traffic Received (packets/sec)",	OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				ete_delay_gstathandle		= op_stat_reg ("Traffic NWK.End-to-End Delay (seconds)",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				
				
				
				
				online_number_gstathandle	= op_stat_reg ("Traffic NWK.Online Number",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				
				rcvd_pkts = 0;
				
				send_up_num              = op_stat_reg ("Traffic NWK.Send_up_num",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				send_dowm_num            = op_stat_reg ("Traffic NWK.Send_dowm_num",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				recv_up_num              = op_stat_reg ("Traffic NWK.Recv_up_num",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				recv_dowm_num            = op_stat_reg ("Traffic NWK.Recv_dowm_num",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				recv_send_rate              = op_stat_reg ("Traffic NWK.Received-Send-Rate",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				
				recv_send_rate_down         = op_stat_reg ("Traffic NWK.Received-Send-Rate-Down",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				ete_delay_gstathandle_down		= op_stat_reg ("Traffic NWK.End-to-End Delay Down (seconds)",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				
				
				ete_delay_l_n		= op_stat_reg ("Traffic NWK.Node End-to-End Delay (seconds)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				
				multicast_count		= op_stat_reg ("Traffic NWK.Node mutilcast Received Count",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				leave_node_count	= op_stat_reg ("Traffic NWK.Node Leaved Count",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				join_time_lstathandle		= op_stat_reg ("Traffic NWK.join_time_lstathandle",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** state (INIT) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "INIT", "WSN_NWK_Process [INIT exit execs]")


			/** state (INIT) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [INIT trans conditions]", state0_trans_conds)
			FSM_INIT_COND (g_nwk_node_type == GATENODE)
			FSM_TEST_COND (g_nwk_node_type != GATENODE)
			FSM_TEST_LOGIC ("INIT")
			FSM_PROFILE_SECTION_OUT (state0_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 5, state5_enter_exec, nwk_online();, "g_nwk_node_type == GATENODE", "nwk_online()", "INIT", "Online", "tr_53", "WSN_NWK_Process [INIT -> Online : g_nwk_node_type == GATENODE / nwk_online()]")
				FSM_CASE_TRANSIT (1, 14, state14_enter_exec, nwk_delay_start();, "g_nwk_node_type != GATENODE", "nwk_delay_start()", "INIT", "init", "tr_17", "WSN_NWK_Process [INIT -> init : g_nwk_node_type != GATENODE / nwk_delay_start()]")
				}
				/*---------------------------------------------------------*/



			/** state (Offline) enter executives **/
			FSM_STATE_ENTER_FORCED (1, "Offline", state1_enter_exec, "WSN_NWK_Process [Offline enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [Offline enter execs]", state1_enter_exec)
				{
				nwk_init_RCtable();
				}
				FSM_PROFILE_SECTION_OUT (state1_enter_exec)

			/** state (Offline) exit executives **/
			FSM_STATE_EXIT_FORCED (1, "Offline", "WSN_NWK_Process [Offline exit execs]")


			/** state (Offline) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [Offline trans conditions]", state1_trans_conds)
			FSM_INIT_COND (NON_FATHER)
			FSM_TEST_COND (NONA_FATHER)
			FSM_TEST_LOGIC ("Offline")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 2, state2_enter_exec, nwk_scan_request(0.5);, "NON_FATHER", "nwk_scan_request(0.5)", "Offline", "WaitingScan", "tr_18", "WSN_NWK_Process [Offline -> WaitingScan : NON_FATHER / nwk_scan_request(0.5)]")
				FSM_CASE_TRANSIT (1, 3, state3_enter_exec, nwk_associate_request(0);, "NONA_FATHER", "nwk_associate_request(0)", "Offline", "Associating", "tr_34", "WSN_NWK_Process [Offline -> Associating : NONA_FATHER / nwk_associate_request(0)]")
				}
				/*---------------------------------------------------------*/



			/** state (WaitingScan) enter executives **/
			FSM_STATE_ENTER_UNFORCED (2, "WaitingScan", state2_enter_exec, "WSN_NWK_Process [WaitingScan enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [WaitingScan enter execs]", state2_enter_exec)
				{
				g_nwk_result = 0;
				printf("Node %d in WaitingScan\n", g_nwk_my_ESN_address);
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (5,"WSN_NWK_Process")


			/** state (WaitingScan) exit executives **/
			FSM_STATE_EXIT_UNFORCED (2, "WaitingScan", "WSN_NWK_Process [WaitingScan exit execs]")


			/** state (WaitingScan) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [WaitingScan trans conditions]", state2_trans_conds)
			FSM_INIT_COND (INTRPT_STRM_MLME)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("WaitingScan")
			FSM_PROFILE_SECTION_OUT (state2_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 13, state13_enter_exec, ;, "INTRPT_STRM_MLME", "", "WaitingScan", "From_MLME_Scan", "tr_65", "WSN_NWK_Process [WaitingScan -> From_MLME_Scan : INTRPT_STRM_MLME / ]")
				FSM_CASE_TRANSIT (1, 2, state2_enter_exec, ;, "default", "", "WaitingScan", "WaitingScan", "tr_72", "WSN_NWK_Process [WaitingScan -> WaitingScan : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (Associating) enter executives **/
			FSM_STATE_ENTER_UNFORCED (3, "Associating", state3_enter_exec, "WSN_NWK_Process [Associating enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [Associating enter execs]", state3_enter_exec)
				{
				g_nwk_result = 0;
				g_nwk_specify_father = 0;
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (7,"WSN_NWK_Process")


			/** state (Associating) exit executives **/
			FSM_STATE_EXIT_UNFORCED (3, "Associating", "WSN_NWK_Process [Associating exit execs]")


			/** state (Associating) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [Associating trans conditions]", state3_trans_conds)
			FSM_INIT_COND (INTRPT_STRM_MLME)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("Associating")
			FSM_PROFILE_SECTION_OUT (state3_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 11, state11_enter_exec, ;, "INTRPT_STRM_MLME", "", "Associating", "Associate_Result", "tr_54", "WSN_NWK_Process [Associating -> Associate_Result : INTRPT_STRM_MLME / ]")
				FSM_CASE_TRANSIT (1, 3, state3_enter_exec, ;, "default", "", "Associating", "Associating", "tr_73", "WSN_NWK_Process [Associating -> Associating : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (WAN) enter executives **/
			FSM_STATE_ENTER_UNFORCED (4, "WAN", state4_enter_exec, "WSN_NWK_Process [WAN enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [WAN enter execs]", state4_enter_exec)
				{
				g_nwk_specify_father = 0;
				//g_nwk_result = 1;
				/*
				if(JOINFAILT && NON_FATHER) printf("a\n");
				if(JOINFAILT && NONA_FATHER) printf("b\n");
				if(INTRPT_STRM_MCPS_UP) printf("c\n");
				
				if(FAILT) printf("d\n");
				if(INTRPT_SELF_JOIN) printf("e\n");
				*/
				}
				FSM_PROFILE_SECTION_OUT (state4_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (9,"WSN_NWK_Process")


			/** state (WAN) exit executives **/
			FSM_STATE_EXIT_UNFORCED (4, "WAN", "WSN_NWK_Process [WAN exit execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [WAN exit execs]", state4_exit_exec)
				{
				/*if(JOINFAILT && NON_FATHER)	printf("1\n");
				if(INTRPT_STRM_MCPS_UP)	printf("2\n");
				if(INTRPT_RESEND_JOIN)	printf("3\n");
				if(JOINFAILT && NONA_FATHER)	printf("4\n");
				if(JOINFAILT) printf("5555\n");*/
				}
				FSM_PROFILE_SECTION_OUT (state4_exit_exec)


			/** state (WAN) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [WAN trans conditions]", state4_trans_conds)
			FSM_INIT_COND (JOINFAILT && NONA_FATHER)
			FSM_TEST_COND (JOINFAILT && NON_FATHER)
			FSM_TEST_COND (INTRPT_STRM_MCPS_UP)
			FSM_TEST_COND (INTRPT_RESEND_JOIN)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("WAN")
			FSM_PROFILE_SECTION_OUT (state4_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 3, state3_enter_exec, nwk_associate_request(0);, "JOINFAILT && NONA_FATHER", "nwk_associate_request(0)", "WAN", "Associating", "tr_24", "WSN_NWK_Process [WAN -> Associating : JOINFAILT && NONA_FATHER / nwk_associate_request(0)]")
				FSM_CASE_TRANSIT (1, 1, state1_enter_exec, nwk_leave_request(0);, "JOINFAILT && NON_FATHER", "nwk_leave_request(0)", "WAN", "Offline", "tr_25", "WSN_NWK_Process [WAN -> Offline : JOINFAILT && NON_FATHER / nwk_leave_request(0)]")
				FSM_CASE_TRANSIT (2, 12, state12_enter_exec, ;, "INTRPT_STRM_MCPS_UP", "", "WAN", "From_MCPS_Join", "tr_62", "WSN_NWK_Process [WAN -> From_MCPS_Join : INTRPT_STRM_MCPS_UP / ]")
				FSM_CASE_TRANSIT (3, 4, state4_enter_exec, nwk_join_request();, "INTRPT_RESEND_JOIN", "nwk_join_request()", "WAN", "WAN", "tr_94", "WSN_NWK_Process [WAN -> WAN : INTRPT_RESEND_JOIN / nwk_join_request()]")
				FSM_CASE_TRANSIT (4, 4, state4_enter_exec, ;, "default", "", "WAN", "WAN", "tr_75", "WSN_NWK_Process [WAN -> WAN : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (Online) enter executives **/
			FSM_STATE_ENTER_UNFORCED (5, "Online", state5_enter_exec, "WSN_NWK_Process [Online enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (11,"WSN_NWK_Process")


			/** state (Online) exit executives **/
			FSM_STATE_EXIT_UNFORCED (5, "Online", "WSN_NWK_Process [Online exit execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [Online exit execs]", state5_exit_exec)
				{
				/*
				if(INTRPT_SELF_REJOIN)	printf("1\n");
				if(INTRPT_STRM_DATA)	printf("2\n");
				if(INTRPT_STRM_CON)	printf("3\n");
				if(INTRPT_STRM_MLME)	printf("4\n");
				if(INTRPT_SELF_UPDATE)	printf("5\n");
				if(INTRPT_SELF_JOIN)	printf("6\n");
				if(INTRPT_STRM_MCPS)	printf("7\n");
				if(REJOIN_SAMEFATHER)	printf("8\n");
				
				*/
				}
				FSM_PROFILE_SECTION_OUT (state5_exit_exec)


			/** state (Online) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [Online trans conditions]", state5_trans_conds)
			FSM_INIT_COND (REJOIN_SAMEFATHER)
			FSM_TEST_COND (INTRPT_STRM_CON)
			FSM_TEST_COND (INTRPT_STRM_DATA)
			FSM_TEST_COND (INTRPT_SELF_UPDATE)
			FSM_TEST_COND (INTRPT_STRM_MCPS)
			FSM_TEST_COND (INTRPT_STRM_MLME)
			FSM_TEST_COND (INTRPT_SELF_REJOIN)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("Online")
			FSM_PROFILE_SECTION_OUT (state5_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 4, state4_enter_exec, nwk_join_request();, "REJOIN_SAMEFATHER", "nwk_join_request()", "Online", "WAN", "tr_27", "WSN_NWK_Process [Online -> WAN : REJOIN_SAMEFATHER / nwk_join_request()]")
				FSM_CASE_TRANSIT (1, 7, state7_enter_exec, ;, "INTRPT_STRM_CON", "", "Online", "From_Control", "tr_38", "WSN_NWK_Process [Online -> From_Control : INTRPT_STRM_CON / ]")
				FSM_CASE_TRANSIT (2, 6, state6_enter_exec, ;, "INTRPT_STRM_DATA", "", "Online", "From_Data", "tr_40", "WSN_NWK_Process [Online -> From_Data : INTRPT_STRM_DATA / ]")
				FSM_CASE_TRANSIT (3, 10, state10_enter_exec, ;, "INTRPT_SELF_UPDATE", "", "Online", "TimerCount", "tr_42", "WSN_NWK_Process [Online -> TimerCount : INTRPT_SELF_UPDATE / ]")
				FSM_CASE_TRANSIT (4, 8, state8_enter_exec, ;, "INTRPT_STRM_MCPS", "", "Online", "From_MCPS", "tr_44", "WSN_NWK_Process [Online -> From_MCPS : INTRPT_STRM_MCPS / ]")
				FSM_CASE_TRANSIT (5, 9, state9_enter_exec, ;, "INTRPT_STRM_MLME", "", "Online", "From_MLME", "tr_46", "WSN_NWK_Process [Online -> From_MLME : INTRPT_STRM_MLME / ]")
				FSM_CASE_TRANSIT (6, 15, state15_enter_exec, nwk_leave_request(0);, "INTRPT_SELF_REJOIN", "nwk_leave_request(0)", "Online", "WaitingLeaving", "tr_83", "WSN_NWK_Process [Online -> WaitingLeaving : INTRPT_SELF_REJOIN / nwk_leave_request(0)]")
				FSM_CASE_TRANSIT (7, 5, state5_enter_exec, ;, "default", "", "Online", "Online", "tr_60", "WSN_NWK_Process [Online -> Online : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (From_Data) enter executives **/
			FSM_STATE_ENTER_FORCED (6, "From_Data", state6_enter_exec, "WSN_NWK_Process [From_Data enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [From_Data enter execs]", state6_enter_exec)
				{
				int type;
				int pklen = 0;
				int sourceMode;
				int destMode;
				int len;
				int dest;
				int NWKSeq = 0;
				int data;
				Packet* payLoad;
				
				Packet* packet = op_pk_get (op_intrpt_strm());
				op_pk_fd_get(packet,0,&len);
				op_pk_fd_get(packet,1,&type);
				op_pk_fd_get(packet,2,&dest);
				op_pk_fd_get(packet,3,&data);
				payLoad = op_pk_create(0);
				printf("Node %d date = %d, len = %d, dest = %d\n", g_nwk_my_ESN_address, data, len, dest);
				//op_prg_log_entry_write(g_nwk_debugger_log_handle, "date = %d, len = %d, dest = %d\n", data, len, dest);
				op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,data, len * 8);
				
				pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
				pklen += len*8;
				if(type == 7 && g_nwk_node_type != GATENODE && g_nwk_node_type != BACKUPNODE)
				{
					//printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Node %d get a upData node %d from data module!!!!!!!!!!!!!!!!!!",myMACAddress,dest);
					dest = GateAddress;
					destMode = 1;
					sourceMode = 2;
					g_pk_send_up++;
					op_stat_write(send_up_num, g_pk_send_up);
					nwk_generate_packet(pklen,0,1,sourceMode,destMode,g_nwk_my_short_address,dest,type,NWK_MAX_RADIUS-1,len,payLoad,OUT_NWK_TO_MCPS_UP,0);
					//op_stat_write(up_data_send_num,++upSendNum);
				}
				else if(g_nwk_node_type == GATENODE)
				{
					if(onlineNodes[dest] == 1) 
						{
						dest = quiryNWKAddress(dest);
						}
					else dest = -1;
					//printf("NWKAddress %d !!!!!!!!!!!!!!!!!!\n",dest);
					if(dest > 0)
					{
						destMode = 2;
						sourceMode = 1;
						g_pk_send_down++;
						op_stat_write(send_dowm_num, g_pk_send_down);
						nwk_generate_packet(pklen,0,1,sourceMode,destMode,g_nwk_my_short_address,dest,type,NWK_MAX_RADIUS-1,len,payLoad,OUT_NWK_TO_MCPS_DOWN,0);//����
						//nwk_generate_packet(pklen,1,0,sourceMode,destMode,g_nwk_my_short_address,1,type,NWK_MAX_RADIUS-1,len,payLoad,OUT_NWK_TO_MCPS_DOWN,0);//�鲥
						//op_stat_write(down_data_send_num,++downSendNum);
					}
				}
				else
				{
					op_pk_destroy(payLoad);
				}
				op_pk_destroy(packet);
				
				}
				FSM_PROFILE_SECTION_OUT (state6_enter_exec)

			/** state (From_Data) exit executives **/
			FSM_STATE_EXIT_FORCED (6, "From_Data", "WSN_NWK_Process [From_Data exit execs]")


			/** state (From_Data) transition processing **/
			FSM_TRANSIT_FORCE (5, state5_enter_exec, ;, "default", "", "From_Data", "Online", "tr_41", "WSN_NWK_Process [From_Data -> Online : default / ]")
				/*---------------------------------------------------------*/



			/** state (From_Control) enter executives **/
			FSM_STATE_ENTER_FORCED (7, "From_Control", state7_enter_exec, "WSN_NWK_Process [From_Control enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [From_Control enter execs]", state7_enter_exec)
				{
				int type;
				int pklen = 0;
				int sourceMode;
				int destMode;
				int len;
				int source;
				int dest;
				int NWKSeq = 0;
				int removeChildren;
				Packet* payLoad;
				
				Packet* packet = op_pk_get (op_intrpt_strm());
				op_pk_fd_get(packet,0,&len);
				op_pk_fd_get(packet,1,&type);
				op_pk_fd_get(packet,2,&dest);
				op_pk_fd_get(packet,3,&payLoad);
				pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + len;
				len /= 8;
				printf("Node %d received a control %d\n", g_nwk_my_ESN_address, type);
				//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d received a control %d, len = %d\n", g_nwk_my_ESN_address, type, len);
				switch(type)
				{
					case 1: break;
					case 2: printf("dest = %d !!!\n",dest);
							dest = quiryNWKAddress(dest);
							printf("dest = %d !!!\n",dest);
							if(dest == 0)	break;
							printf("Node %d send a leaveContr,dest = %d !!!\n",g_nwk_my_ESN_address,dest);
							if(g_nwk_node_type == GATENODE)	sourceMode = 1;
							else						sourceMode = 2;
							destMode = 2;
							source = g_nwk_my_short_address;
							pklen += SHORTADDR_LEN * 2;
							op_pk_fd_get(payLoad,0,&removeChildren);
							removeChildren &= 1;
							nwk_generate_packet(pklen,0,1,sourceMode,destMode,source,dest,type,NWK_MAX_RADIUS,len,payLoad,OUT_NWK_TO_MCPS_DOWN,0);
							nwk_leave_net(dest,1,removeChildren,0);
							//op_stat_write(contr_send_num,++sendNum);
							break;
					case 3: sourceMode = 2;
							destMode = 1;
							source = g_nwk_my_short_address;
							pklen += SHORTADDR_LEN;
							nwk_generate_packet(pklen,0,1,sourceMode,destMode,source,dest,type,NWK_MAX_RADIUS,len,payLoad,OUT_NWK_TO_MCPS_UP,0);
							//op_stat_write(contr_send_num,++sendNum);
							break;
					case 4: sourceMode = 2;
							destMode = 2;
							source = g_nwk_my_short_address;
							pklen += SHORTADDR_LEN * 2;
							nwk_generate_packet(pklen,0,1,sourceMode,destMode,source,dest,type,NWK_MAX_RADIUS,len,payLoad,OUT_NWK_TO_MCPS_UP,0);
							//op_stat_write(contr_send_num,++sendNum);
							break;
					case 5: dest = quiryNWKAddress(dest);
							if(dest > 0)
								{
								sourceMode = 1;
								destMode = 2;
								source = g_nwk_my_short_address;
								pklen += SHORTADDR_LEN;
								nwk_generate_packet(pklen,0,1,sourceMode,destMode,source,dest,type,NWK_MAX_RADIUS-1,len,payLoad,OUT_NWK_TO_MCPS_DOWN,0);
								}
							//op_stat_write(contr_send_num,++sendNum);
							break;
					case 6: sourceMode = 2;
							destMode = 3;
							source = g_nwk_my_short_address;
							pklen += SHORTADDR_LEN + LONGADDR_LEN;
							nwk_generate_packet(pklen,0,1,sourceMode,destMode,source,dest,type,NWK_MAX_RADIUS,len,payLoad,OUT_NWK_TO_MCPS_DOWN,0);
							//op_stat_write(contr_send_num,++sendNum);
							break;
					case 8: sourceMode = 2;//�����鲥����
							destMode = 2;
							source = g_nwk_my_short_address;
							pklen += SHORTADDR_LEN + SHORTADDR_LEN;
							printf("send a mutilcast data\n");
							nwk_generate_packet(pklen,1,0,sourceMode,destMode,source,dest,type,NWK_MAX_RADIUS,len,payLoad,OUT_NWK_TO_MCPS_DOWN,0);
							//op_stat_write(contr_send_num,++sendNum);
							break;
					case 9: break;//No address conflict
					case 10:sourceMode = 2;
							destMode = 1;
							source = g_nwk_my_short_address;
							pklen += SHORTADDR_LEN;
							nwk_generate_packet(pklen,0,1,sourceMode,destMode,source,dest,type,NWK_MAX_RADIUS-1,len,payLoad,OUT_NWK_TO_MCPS_UP,0);
							//op_stat_write(contr_send_num,++sendNum);
							break;
					case 11:sourceMode = 2;
							destMode = 1;
							source = g_nwk_my_short_address;
							pklen += SHORTADDR_LEN;
							nwk_generate_packet(pklen,0,1,sourceMode,destMode,source,dest,type,NWK_MAX_RADIUS,len,payLoad,OUT_NWK_TO_MCPS_UP,0);
							//op_stat_write(contr_send_num,++sendNum);
							break;
					case 12:sourceMode = 2;
							destMode = 1;
							source = g_nwk_my_short_address;
							pklen += SHORTADDR_LEN;
							nwk_generate_packet(pklen,0,1,sourceMode,destMode,source,dest,type,NWK_MAX_RADIUS,len,payLoad,OUT_NWK_TO_MCPS_UP,0);
							//op_stat_write(contr_send_num,++sendNum);
							break;
					case 13:sourceMode = 1;
							destMode = 2;
							source = g_nwk_my_short_address;
							pklen += SHORTADDR_LEN;
							nwk_generate_packet(pklen,0,1,sourceMode,destMode,source,dest,type,NWK_MAX_RADIUS,len,payLoad,OUT_NWK_TO_MCPS_DOWN,0);
							//op_stat_write(contr_send_num,++sendNum);
							break;
					case 14:sourceMode = 2;
							destMode = 1;
							source = g_nwk_my_short_address;
							pklen += SHORTADDR_LEN;
							nwk_generate_packet(pklen,0,1,sourceMode,destMode,source,dest,type,NWK_MAX_RADIUS,len,payLoad,OUT_NWK_TO_MCPS_UP,0);
							//op_stat_write(contr_send_num,++sendNum);
							break;
					default:op_pk_destroy(packet);
							printf("Contol_PacketType is invalid\n");
							//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Contol_PacketType is invalid\n");
				}
				}
				FSM_PROFILE_SECTION_OUT (state7_enter_exec)

			/** state (From_Control) exit executives **/
			FSM_STATE_EXIT_FORCED (7, "From_Control", "WSN_NWK_Process [From_Control exit execs]")


			/** state (From_Control) transition processing **/
			FSM_TRANSIT_FORCE (5, state5_enter_exec, ;, "default", "", "From_Control", "Online", "tr_39", "WSN_NWK_Process [From_Control -> Online : default / ]")
				/*---------------------------------------------------------*/



			/** state (From_MCPS) enter executives **/
			FSM_STATE_ENTER_FORCED (8, "From_MCPS", state8_enter_exec, "WSN_NWK_Process [From_MCPS enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [From_MCPS enter execs]", state8_enter_exec)
				{
				int framControl;
				int NWKControl;
				int type;
				int destMode;
				int dest;
				int sourceMode;
				int source;
				int length;
				int ies;
				int iepresent;
				int multicast;
				
				int len;
				int next;
				
				int ie_type;
				
				Packet* payload;
				Packet* packet = op_pk_get (op_intrpt_strm());
				
				op_pk_fd_get(packet,0,&framControl);
				op_pk_fd_get(packet,5,&length);
				op_pk_fd_get(packet,7,&NWKControl);
				op_pk_fd_get(packet,8,&payload);
				
				iepresent = 1&(framControl>>6);
				multicast = 1&(framControl>>8);
				type = 15 & (NWKControl>>12);
				destMode = 3 & (framControl>>4);
				sourceMode = 3 & framControl;
				if(destMode != 1)	op_pk_fd_get(packet,3,&dest);
				else				dest = GateAddress;
				if(sourceMode != 1)	op_pk_fd_get(packet,4,&source);
				else				source = GateAddress;
				
				len = length - (NWKCONTR_LEN / 8);
				
				printf("Node %d received a pk from mcps, type = %d, sourceMode = %d, destMode = %d, source = %d, dest = %d!\n", g_nwk_my_ESN_address, type, sourceMode, destMode, source, dest);
				op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d received a pk from mcps, type = %d, sourceMode = %d, destMode = %d, source = %d, dest = %d!\n", g_nwk_my_ESN_address, type, sourceMode, destMode, source, dest);
				
				//���е������ݽ��մ���
				if(multicast == 0 && (op_intrpt_strm() == IN_MCPS_DOWN_TO_NWK))
					{
				
					//����·��
					if(sourceMode == 2)		
						{
						UpdateRT(source, NWK_ROUTING_TABLE_Active);
						}
					
					if(iepresent == 1)
						{
						op_pk_fd_get(packet,60,&ie_type);
						//if(ie_type == 0)
							//{
							op_pk_fd_get(packet,6,&ies);
							//�鲥ά��
							maintainCMT(ies);
							
							//}
						/*
						else if(ie_type == 1)
							{
							// ����ͬ���Ľ�����������к�
							
							}
						*/
						len -= (IES_LEN / 8);
						}
					if(g_nwk_node_type != GATENODE && dest != g_nwk_my_short_address)
						{
						if(g_nwk_node_type != GATENODE)
							{
							//����
							switch(type)
								{
								case 3: nwk_process_backup_router_switch(payload, destMode, dest, sourceMode, source, len, 0);
										break;
								case 11:nwk_process_routing(payload, destMode, dest, sourceMode, source, len);
										break;
								}
						
							//ת��
							op_pk_fd_set (packet,8,OPC_FIELD_TYPE_PACKET,payload,len * 8);
							nwk_forward_packet(packet,OUT_NWK_TO_MCPS_UP);
							}
						}
					else
						{
						printf("Node %d received a pk from mcps ����, type = %d, source = %d, dest = %d!\n", g_nwk_my_ESN_address, type, source, dest);
						//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d received a pk from mcps, type = %d, source = %d, dest = %d!\n", g_nwk_my_ESN_address, type, source, dest);
						//���մ���
						switch(type)
							{
							case 0: nwk_process_Join_request(payload, destMode, dest, sourceMode, source, len);
									break;
							case 2: nwk_process_leave(payload, destMode, dest, sourceMode, source, len);
									break;
							case 3: nwk_process_backup_router_switch(payload, destMode, dest, sourceMode, source, len, 0);
									break;
							case 4: nwk_master_status_process(payload, destMode, dest, sourceMode, source, len);
									break;
							case 7: nwk_process_up_data(payload, destMode, dest, sourceMode, source, len);
							//printf("Receive a data\n");
									break;
							case 9: break;//No address conflict
							case 10:nwk_process_nwk_status(payload, destMode, dest, sourceMode, source, len);
									break;
							case 11:nwk_process_routing(payload, destMode, dest, sourceMode, source, len);
									break;
							case 12:nwk_process_main_router_request(payload, destMode, dest, sourceMode, source, len);
									break;
							default:printf("NWK_PacketType is invalid\n");
									op_prg_log_entry_write(g_nwk_debugger_log_handle, "NWK_PacketType is invalid\n");
							}
						op_pk_destroy(packet);
						op_pk_destroy(payload);
						}
					}
				//���е������ݽ��մ���
				//���յ����豸������
				else if(multicast == 0 && (op_intrpt_strm() == IN_MCPS_UP_TO_NWK))
					{
					if(destMode == 2 && (dest == g_nwk_my_short_address || dest == BroadCast || dest == ChildrenCast || (dest == RouterCast && g_nwk_node_type == ROUTERNODE) || (dest == ChildrenEndCast && g_nwk_node_type == ENDNODE)))
						{
						printf("Node %d received a pk from mcps ����, type = %d\n", g_nwk_my_ESN_address, type);
						//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d received a pk from mcps, type = %d\n", g_nwk_my_ESN_address, type);
						//���մ���
						switch(type)
							{
							case 1: nwk_process_access_confirm(payload, destMode, dest, sourceMode, source, len);
									break;
							case 2: nwk_process_leave(payload, destMode, dest, sourceMode, source, len);
									break;
							case 3: nwk_process_backup_router_switch(payload, destMode, dest, sourceMode, source, len, 1);
									break;
							case 4: nwk_master_status_process(payload, destMode, dest, sourceMode, source, len);
									break;
							case 5:	nwk_process_multicast_control(payload, destMode, dest, sourceMode, source, len);
									break;
							case 6: nwk_process_routing_control(payload, destMode, dest, sourceMode, source, len);
									break;
							case 8: nwk_process_down_data(payload, destMode, dest, sourceMode, source, len);
									break;
							case 9: addressconflict(payload, destMode, dest, sourceMode, source, len);break;//No address conflict
							case 11:nwk_process_routing(payload, destMode, dest, sourceMode, source, len);
									break;
							case 12:nwk_process_main_router_request(payload, destMode, dest, sourceMode, source, len);
									break;
							case 13:nwk_process_backup_router_info(payload, destMode, dest, sourceMode, source, len);
									break;
							case 14:nwk_recover_node(payload, destMode, dest, sourceMode, source, len);
									break;
							default:printf("NWK_PacketType is invalid\n");
									//op_prg_log_entry_write(g_nwk_debugger_log_handle, "NWK_PacketType is invalid\n");
							}
						op_pk_destroy(packet);
						op_pk_destroy(payload);
						}
					else if(destMode == 2 && (dest != g_nwk_my_short_address || (dest == BroadCast && g_nwk_node_type == ROUTERNODE && CTNumber() > 0) || (dest == RouterCast && g_nwk_node_type == ROUTERNODE && RouterNumber() > 0)))
						{
						if(g_nwk_node_type != GATENODE)
							{
							next = nwk_get_nexthop(dest);
							if(next < 0)
								{
								printf("next error, next = %d\n", next);
								op_prg_log_entry_write(g_nwk_debugger_log_handle, "next error, next = %d\n", next);
								op_pk_destroy(packet);
								}
							else
								{
								//����
								switch(type)
									{
									case 1: nwk_process_access_confirm(payload, destMode, dest, sourceMode, source, len);
											break;
									case 2: nwk_process_leave(payload, destMode, dest, sourceMode, source, len);
											break;
									case 5:	nwk_process_multicast_control(payload, destMode, dest, sourceMode, source, len);
											break;
									case 6: nwk_process_routing_control(payload, destMode, dest, sourceMode, source, len);
											break;
									case 9: break;//No address conflict
									//case 12:mainRouterPro(payload, destMode, dest, sourceMode, source, len);
											//break;
									case 14:nwk_recover_node(payload, destMode, dest, sourceMode, source, len);
											break;
									}
								//ת��
								printf("Node %d next = %d\n", g_nwk_my_ESN_address, next);
								//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d next = %d\n", g_nwk_my_ESN_address, next);
								op_pk_fd_set (packet,8,OPC_FIELD_TYPE_PACKET,payload,len * 8);
								nwk_forward_packet(packet,OUT_NWK_TO_MCPS_DOWN);
								}
							}
						}
					else if(destMode == 3 && dest == g_nwk_my_ESN_address)
						{
						if(type == 1)
							{
							nwk_process_access_confirm(payload, destMode, dest, sourceMode, source, len);
							}
						op_pk_destroy(packet);
						op_pk_destroy(payload);
						}
					else
						{
						printf("Node %d ���յ�Ŀ�Ľڵ�Ǳ��ڵ�����ݰ�!\n", g_nwk_my_ESN_address);
						op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d ���յ�Ŀ�Ľڵ�Ǳ��ڵ�����ݰ�!\n", g_nwk_my_ESN_address);
						op_pk_destroy(packet);
						op_pk_destroy(payload);
						}
					}
				//�����鲥���ݽ��մ���
				else if(multicast == 1)
					{
					printf("g_nwk_my_multicast_table : %d\n", g_nwk_my_multicast_table);
					printf("g_nwk_children_multicast_table : %d\n", g_nwk_children_multicast_table.group);
					if(((g_nwk_my_multicast_table>>(dest-1))&1)>0)
						{
							nwk_process_down_data(payload, destMode, dest, sourceMode, source, len);
							printf("----------------------------Node %d receive a multicast down data!  ----",g_nwk_my_ESN_address);
							printf("len = %d-------------------\n",len);
							multicast_receive_num++;
							op_stat_write (multicast_count, 		multicast_receive_num);
							op_prg_log_entry_write(g_nwk_info_log_handle, "----------------------------Node %d receive a multicast down data!  ----",g_nwk_my_ESN_address);
						}
					if(g_nwk_node_type == ROUTERNODE && ((g_nwk_children_multicast_table.group>>(dest - 1))&1) > 0)
						{
							op_pk_fd_set (packet,8,OPC_FIELD_TYPE_PACKET,payload,len);
							nwk_forward_packet(packet,OUT_NWK_TO_MCPS_DOWN);
						}
					else
						{
						op_pk_destroy(packet);
						op_pk_destroy(payload);
						}
					}
				else
					{
					printf("Node %d received a error type and error multicast pk!\n", g_nwk_my_ESN_address);
					op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d received a error type and error multicast pk!\n", g_nwk_my_ESN_address);
					}
				}
				FSM_PROFILE_SECTION_OUT (state8_enter_exec)

			/** state (From_MCPS) exit executives **/
			FSM_STATE_EXIT_FORCED (8, "From_MCPS", "WSN_NWK_Process [From_MCPS exit execs]")


			/** state (From_MCPS) transition processing **/
			FSM_TRANSIT_FORCE (5, state5_enter_exec, ;, "default", "", "From_MCPS", "Online", "tr_45", "WSN_NWK_Process [From_MCPS -> Online : default / ]")
				/*---------------------------------------------------------*/



			/** state (From_MLME) enter executives **/
			FSM_STATE_ENTER_FORCED (9, "From_MLME", state9_enter_exec, "WSN_NWK_Process [From_MLME enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [From_MLME enter execs]", state9_enter_exec)
				{
				int type;
				int shortAddress, res, flag;
				Packet* mlme = op_pk_get (op_intrpt_strm());
				
				op_pk_fd_get(mlme,0,&type);
				//printf("Node %d NWK receive a MLME type = %d\n", g_nwk_my_ESN_address, type);
				//op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d NWK receive a MLME type = %d\n", g_nwk_my_ESN_address, type);
				switch(type)
					{
					case ALIVE: 
							op_pk_fd_get(mlme,1,&shortAddress);
							op_pk_fd_get(mlme,2,&res);
							op_pk_fd_get(mlme,3,&flag);
							nwk_keep_alive(shortAddress, res, flag);
							op_pk_destroy(mlme); 
							break;
					case Conflict: op_pk_fd_get(mlme,1,&shortAddress);
							printf("Node %d receive a conflict address = %d\n", g_nwk_my_ESN_address, shortAddress);
							if(shortAddress == g_nwk_my_father_short_address)//���ڵ�MAC���к����򣬱��ڵ�����
								{
								nwk_leave_net(g_nwk_my_short_address, 1, 1, 0);
								}
							else
								{
								nwk_send_conflict(shortAddress);
								}
							op_pk_destroy(mlme); 
							break;
					/*case LEAVE:op_pk_fd_get(mlme,1,&res);
							nwk_leave_confirm(res);
							op_prg_log_entry_write(g_nwk_debugger_log_handle, "leave mlme res = %d\n", res);
							break;*/
					case SYN_SEQ_SET:op_pk_fd_get(mlme,1,&res);
							nwk_syn_seq_comfirn(res);
							op_pk_destroy(mlme); 
							break;
					case Multicast:op_pk_fd_get(mlme,1,&res);
							maintainCMT(res);
							op_pk_destroy(mlme); 
							break;
					case BEACON:op_pk_fd_get(mlme,1,&res);
							//if(g_nwk_my_ESN_address == 95) printf("recv a BEACON res = %d\n", res);
							nwk_beacon_confirm(res);
							break;
					case ASSOCIATE:op_pk_fd_get(mlme,1,&shortAddress);
							op_pk_fd_get(mlme,2,&res);
							op_pk_fd_get(mlme,3,&flag);
							if(flag == 1) 
								{
								if(res == 0)
									{
									printf("recv a associate res = %d, flag = %d\n", res, flag);
									/*
									
									
									
									*/
									
									nwk_send_3p_notice(g_nwk_main_router_father, -1);
									
									
									/*
									
									������
									
									*/
									//nwk_send_master_status(g_nwk_main_router_father, 0, g_nwk_main_address, 0);
									}
								else
									{
									nwk_leave_net(g_nwk_my_short_address, 1, 0, 0);
									}
								}
							else if(flag == 2 && g_nwk_main_address > 0)
								{
								nwk_slave2master_notice(0);
								nwk_slave2master_notice(1);
								g_nwk_main_address = 0;
								}
							op_pk_destroy(mlme); 
							break;
					case LEAVE:op_pk_fd_get(mlme,1,&res);
								flag = nwk_leave_confirm(res);
								op_pk_destroy(mlme); 
								if(flag == 1 && g_nwk_reassociate_leave_flag == 0)
									{
									mlme = op_pk_create(0);
									op_pk_fd_set (mlme, 0, OPC_FIELD_TYPE_INTEGER, ASSOCIATE, 8);
									op_pk_fd_set (mlme, 1, OPC_FIELD_TYPE_INTEGER, g_nwk_my_father_short_address, 16);
									op_pk_fd_set (mlme, 2, OPC_FIELD_TYPE_INTEGER, 0, 8);
									op_pk_fd_set (mlme, 3, OPC_FIELD_TYPE_INTEGER, g_nwk_father_table.Band, 8);
									op_pk_fd_set (mlme, 4, OPC_FIELD_TYPE_INTEGER, g_nwk_father_table.Frequency, 8);
									op_pk_fd_set (mlme, 5, OPC_FIELD_TYPE_INTEGER, 2, 8);
									op_pk_send_delayed(mlme, OUT_NWK_TO_MLME, 0);
									printf("Node %d send a nwk_change_slave2master father = %d, band = %d, frequency = %d\n", g_nwk_my_ESN_address, g_nwk_main_router_father, g_nwk_main_router_band, g_nwk_main_router_frequency);
									}
								else if(flag == 1 && g_nwk_reassociate_leave_flag == 1)
									{
									//printf("Node %d zxcv\n", g_nwk_my_ESN_address);
									g_nwk_reassociate_leave_flag = 0;
									g_nwk_specify_father = g_nwk_father_table.NWKAddress;
									nwk_associate_request(2);
									}
								break;
					default :op_pk_destroy(mlme); 
							printf("Node %d received a error typy = %d mlme\n", g_nwk_my_ESN_address, type);
							op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d received a error typy = %d mlme\n", g_nwk_my_ESN_address, type);
					}
				}
				FSM_PROFILE_SECTION_OUT (state9_enter_exec)

			/** state (From_MLME) exit executives **/
			FSM_STATE_EXIT_FORCED (9, "From_MLME", "WSN_NWK_Process [From_MLME exit execs]")


			/** state (From_MLME) transition processing **/
			FSM_TRANSIT_FORCE (5, state5_enter_exec, ;, "default", "", "From_MLME", "Online", "tr_47", "WSN_NWK_Process [From_MLME -> Online : default / ]")
				/*---------------------------------------------------------*/



			/** state (TimerCount) enter executives **/
			FSM_STATE_ENTER_FORCED (10, "TimerCount", state10_enter_exec, "WSN_NWK_Process [TimerCount enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [TimerCount enter execs]", state10_enter_exec)
				{
				switch(op_intrpt_code())
					{
					//�������ݼ�������
					case intrCode_Timer:	op_intrpt_schedule_self(op_sim_time() + NWK_TIMER,intrCode_Timer);
							UpdateRT(-1,-1);//����·�ɱ������
							UpdateBackup();//���±���·����ؼ�����
							nwk_update_address(-1);//���µ�ַ�ؼ�����
							maintainCMT(-1);//�����鲥ά��������
							//if(g_nwk_my_ESN_address == 3) printf("Network_Msg.NetworkCT[%d].number = %d",g_nwk_my_ESN_address, Network_Msg.NetworkCT[g_nwk_my_ESN_address].number);
							if(NWK_JOIN_CONTROL == 1) updateCT();
						/**
							
							������
						*/
							
						if(g_nwk_my_ESN_address == 37 && op_sim_time() > 7500 && g_nwk_backup_router >= 0 && op_sim_time() < 7515) 
							{
							printf("Node %d set band 50\n", g_nwk_my_ESN_address);
							op_ima_obj_attr_set(g_nwk_nodeID,"g_node_band",50);
							op_ima_obj_attr_set(g_nwk_nodeID,"g_node_frequency",5);
							op_ima_obj_attr_set(g_nwk_nodeID,"g_node_type",2);
							g_nwk_node_type = 2;
							g_nwk_capability = 0;
							g_nwk_capability |= g_nwk_node_type<<4;
							}
						if(g_nwk_my_ESN_address == 36 && op_sim_time() > 7800 && g_nwk_backup_router >= 0 && op_sim_time() < 7815) 
							{
							printf("Node %d set band 48\n", g_nwk_my_ESN_address);
							op_ima_obj_attr_set(g_nwk_nodeID,"g_node_band",48);
							op_ima_obj_attr_set(g_nwk_nodeID,"g_node_frequency",5);
							op_ima_obj_attr_set(g_nwk_nodeID,"g_node_type",2);
							g_nwk_node_type = 2;
							g_nwk_capability = 0;
							g_nwk_capability |= g_nwk_node_type<<4;
							}
						
						if(g_nwk_my_ESN_address == 27 && op_sim_time() > 7300 && g_nwk_backup_router >= 0 && op_sim_time() < 7315) 
							{
							printf("Node %d set band 49\n", g_nwk_my_ESN_address);
							op_ima_obj_attr_set(g_nwk_nodeID,"g_node_band",49);
							op_ima_obj_attr_set(g_nwk_nodeID,"g_node_frequency",5);
							op_ima_obj_attr_set(g_nwk_nodeID,"g_node_type",2);
							g_nwk_node_type = 2;
							g_nwk_capability = 0;
							g_nwk_capability |= g_nwk_node_type<<4;
							}
						if(g_nwk_my_ESN_address == 3 && op_sim_time() > 7400 && g_nwk_backup_router >= 0 && op_sim_time() < 7415) 
							{
							printf("Node %d set band 51\n", g_nwk_my_ESN_address);
							op_ima_obj_attr_set(g_nwk_nodeID,"g_node_band",51);
							op_ima_obj_attr_set(g_nwk_nodeID,"g_node_frequency",5);
							op_ima_obj_attr_set(g_nwk_nodeID,"g_node_type",2);
							g_nwk_node_type = 2;
							g_nwk_capability = 0;
							g_nwk_capability |= g_nwk_node_type<<4;
							}
						if(g_nwk_my_ESN_address == 46 && op_sim_time() > 7600 && g_nwk_backup_router >= 0 && op_sim_time() < 7615) 
							{
							printf("Node %d set band 52\n", g_nwk_my_ESN_address);
							op_ima_obj_attr_set(g_nwk_nodeID,"g_node_band",52);
							op_ima_obj_attr_set(g_nwk_nodeID,"g_node_frequency",5);
							op_ima_obj_attr_set(g_nwk_nodeID,"g_node_type",2);
							g_nwk_node_type = 2;
							g_nwk_capability = 0;
							g_nwk_capability |= g_nwk_node_type<<4;
							}
				
						
							break;
					//Ѱ����·��
					case intrCode_ChangeMainRouter:
							if(g_nwk_main_address <= 0)
								{
								op_prg_log_entry_write(g_nwk_info_log_handle, "to find main router %d!\n", g_nwk_main_routing_address);
								nwk_send_main_router_request();
								op_intrpt_schedule_self(op_sim_time() + 20 + op_dist_uniform (10),intrCode_ChangeMainRouter);
								}
							
							break;
					//Ǳ�ڸ��ڵ���ʱ��
					case intrCode_Father:
							g_nwk_available_potential_father_number = 0;
							break;
					default :printf("error intrpt_code %d!\n", op_intrpt_code());
							 op_prg_log_entry_write(g_nwk_debugger_log_handle, "error intrpt_code %d!\n", op_intrpt_code());
					}
				}
				FSM_PROFILE_SECTION_OUT (state10_enter_exec)

			/** state (TimerCount) exit executives **/
			FSM_STATE_EXIT_FORCED (10, "TimerCount", "WSN_NWK_Process [TimerCount exit execs]")


			/** state (TimerCount) transition processing **/
			FSM_TRANSIT_FORCE (5, state5_enter_exec, ;, "default", "", "TimerCount", "Online", "tr_43", "WSN_NWK_Process [TimerCount -> Online : default / ]")
				/*---------------------------------------------------------*/



			/** state (Associate_Result) enter executives **/
			FSM_STATE_ENTER_FORCED (11, "Associate_Result", state11_enter_exec, "WSN_NWK_Process [Associate_Result enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [Associate_Result enter execs]", state11_enter_exec)
				{
				Packet* l_nwk_packet = op_pk_get (op_intrpt_strm());
				int l_nwk_packet_type;
				int l_nwk_packet_field_short_address;
				int associateg_nwk_result;
				op_pk_fd_get(l_nwk_packet,0,&l_nwk_packet_type);
				//op_prg_log_entry_write(g_nwk_debugger_log_handle, "l_nwk_packet_type = %d\n", l_nwk_packet_type);
				if(l_nwk_packet_type == 1)
					{
					op_pk_fd_get(l_nwk_packet,1,&l_nwk_packet_field_short_address);
					op_pk_fd_get(l_nwk_packet,2,&associateg_nwk_result);
					op_prg_log_entry_write(g_nwk_debugger_log_handle, "l_nwk_packet_field_short_address = %d,g_nwk_my_father_short_address = %d, associateg_nwk_result = %d \n", l_nwk_packet_field_short_address, g_nwk_my_father_short_address, associateg_nwk_result);
					if(l_nwk_packet_field_short_address == g_nwk_my_father_short_address && associateg_nwk_result == 0)
						{
						g_nwk_result = 1;
						}
					else if(l_nwk_packet_field_short_address == g_nwk_my_father_short_address && associateg_nwk_result == 1)
						{
						g_nwk_result = 0;
						}
					else
						{
						printf("Node %d �յ��ǹ��������MLMEԭ��!\n", g_nwk_my_ESN_address);
						op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d �յ��ǹ��������MLMEԭ��!\n", g_nwk_my_ESN_address);
						}
					}
				else
					{
					printf("Node %d �յ��ǹ���ȷ�ϵ�ԭ��\n", g_nwk_my_ESN_address);
					op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d �յ��ǹ���ȷ�ϵ�ԭ��\n", g_nwk_my_ESN_address);
					}
				}
				FSM_PROFILE_SECTION_OUT (state11_enter_exec)

			/** state (Associate_Result) exit executives **/
			FSM_STATE_EXIT_FORCED (11, "Associate_Result", "WSN_NWK_Process [Associate_Result exit execs]")


			/** state (Associate_Result) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [Associate_Result trans conditions]", state11_trans_conds)
			FSM_INIT_COND (SUCCESS)
			FSM_TEST_COND (FAILT && NONA_FATHER)
			FSM_TEST_COND (FAILT && NON_FATHER)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("Associate_Result")
			FSM_PROFILE_SECTION_OUT (state11_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 4, state4_enter_exec, nwk_join_request();, "SUCCESS", "nwk_join_request()", "Associate_Result", "WAN", "tr_56", "WSN_NWK_Process [Associate_Result -> WAN : SUCCESS / nwk_join_request()]")
				FSM_CASE_TRANSIT (1, 3, state3_enter_exec, nwk_associate_request(0);, "FAILT && NONA_FATHER", "nwk_associate_request(0)", "Associate_Result", "Associating", "tr_57", "WSN_NWK_Process [Associate_Result -> Associating : FAILT && NONA_FATHER / nwk_associate_request(0)]")
				FSM_CASE_TRANSIT (2, 1, state1_enter_exec, ;, "FAILT && NON_FATHER", "", "Associate_Result", "Offline", "tr_58", "WSN_NWK_Process [Associate_Result -> Offline : FAILT && NON_FATHER / ]")
				FSM_CASE_TRANSIT (3, 3, state3_enter_exec, ;, "default", "", "Associate_Result", "Associating", "tr_59", "WSN_NWK_Process [Associate_Result -> Associating : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (From_MCPS_Join) enter executives **/
			FSM_STATE_ENTER_FORCED (12, "From_MCPS_Join", state12_enter_exec, "WSN_NWK_Process [From_MCPS_Join enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [From_MCPS_Join enter execs]", state12_enter_exec)
				{
				int framControl;
				int NWKControl;
				int type;
				int destMode;
				int dest;
				int sourceMode;
				int source;
				int length;
				
				int len;
				int res;
				
				Packet* payload;
				Packet* packet;
				g_nwk_result = 0;
				if(op_intrpt_strm() == IN_MCPS_UP_TO_NWK)
					{
					packet = op_pk_get (op_intrpt_strm());
					op_pk_fd_get(packet,0,&framControl);
					op_pk_fd_get(packet,3,&dest);
					op_pk_fd_get(packet,4,&source);
					op_pk_fd_get(packet,5,&length);
					op_pk_fd_get(packet,7,&NWKControl);
					op_pk_fd_get(packet,8,&payload);
					
					type = 15 & (NWKControl>>12);
					destMode = 7 & (framControl>>4);
					sourceMode = 7 & framControl;
					if(destMode != 1)	op_pk_fd_get(packet,3,&dest);
					else				dest = GateAddress;
					if(sourceMode != 1)	op_pk_fd_get(packet,4,&source);
					else				source = GateAddress;
					len = length - 2;
					if(destMode == 3 && dest == g_nwk_my_ESN_address && type == 1)
						{
						res = nwk_process_access_confirm(payload, destMode, dest, sourceMode, source, len);
						printf("Node %d received a accessConfirm pk in join process! res = %d\n", g_nwk_my_ESN_address, res);
						//op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d received a accessConfirm pk in join process!\n", g_nwk_my_ESN_address);
						if(res == 0) g_nwk_result = 1;
						else if(res == 1) 
							{
							g_nwk_result = 2;
							//op_intrpt_disable(OPC_INTRPT_SELF, intrCode_Join, OPC_TRUE);
							op_intrpt_schedule_self(op_sim_time() + NWK_Timer_RESEND_JOIN,intrCode_Resend_Join);
							}
						}
					else
						{
						printf("Node %d received a not accessConfirm pk in join process!\n", g_nwk_my_ESN_address);
						op_prg_log_entry_write(g_nwk_info_log_handle, "Node %d received a not accessConfirm pk in join process!\n", g_nwk_my_ESN_address);
						}
					}
				}
				FSM_PROFILE_SECTION_OUT (state12_enter_exec)

			/** state (From_MCPS_Join) exit executives **/
			FSM_STATE_EXIT_FORCED (12, "From_MCPS_Join", "WSN_NWK_Process [From_MCPS_Join exit execs]")


			/** state (From_MCPS_Join) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [From_MCPS_Join trans conditions]", state12_trans_conds)
			FSM_INIT_COND (FAILT || g_nwk_result == 2)
			FSM_TEST_COND (SUCCESS)
			FSM_TEST_LOGIC ("From_MCPS_Join")
			FSM_PROFILE_SECTION_OUT (state12_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 4, state4_enter_exec, ;, "FAILT || g_nwk_result == 2", "", "From_MCPS_Join", "WAN", "tr_63", "WSN_NWK_Process [From_MCPS_Join -> WAN : FAILT || g_nwk_result == 2 / ]")
				FSM_CASE_TRANSIT (1, 5, state5_enter_exec, nwk_online();, "SUCCESS", "nwk_online()", "From_MCPS_Join", "Online", "tr_64", "WSN_NWK_Process [From_MCPS_Join -> Online : SUCCESS / nwk_online()]")
				}
				/*---------------------------------------------------------*/



			/** state (From_MLME_Scan) enter executives **/
			FSM_STATE_ENTER_FORCED (13, "From_MLME_Scan", state13_enter_exec, "WSN_NWK_Process [From_MLME_Scan enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [From_MLME_Scan enter execs]", state13_enter_exec)
				{
				int i;
				int l_nwk_packet_type;
				Packet* l_nwk_packet;
				g_nwk_result = 0;
				g_nwk_available_potential_father_number = 0;
				//printf("===============\n");
				if(op_intrpt_strm() == IN_MLME_TO_NWK)
					{
					l_nwk_packet = op_pk_get (op_intrpt_strm());
					op_pk_fd_get(l_nwk_packet,0,&l_nwk_packet_type);
					if(l_nwk_packet_type == SCAN)
						{
						for(i = 0;i < NWK_MAX_FATHERTABLE_NUM;i++)
							{
							if(potentialParent.father[g_nwk_my_ESN_address].father[i].short_addr > 0)
								{
								g_nwk_available_potential_father_number++;
								}
							}
						if(g_nwk_available_potential_father_number > 0)
							{
							op_intrpt_schedule_self(op_sim_time() + NWK_TIMER_VALID_FATHER_TABLE,intrCode_Father);
							}
						printf("Node %d down father table, num = %d\n", g_nwk_my_ESN_address, g_nwk_available_potential_father_number);
						op_prg_log_entry_write(g_nwk_debugger_log_handle, "Node %d down father table, num = %d\n", g_nwk_my_ESN_address, g_nwk_available_potential_father_number);
						if(g_nwk_available_potential_father_number > 0)	g_nwk_result = 1;
						}
					else if(l_nwk_packet_type == LEAVE)
						{
						g_nwk_result = 2;
						}
					else
						{
						printf("WaitingScan receive a other MLME!\n");
						op_prg_log_entry_write(g_nwk_debugger_log_handle, "WaitingScan receive a other MLME!\n");
						}
					}
				}
				FSM_PROFILE_SECTION_OUT (state13_enter_exec)

			/** state (From_MLME_Scan) exit executives **/
			FSM_STATE_EXIT_FORCED (13, "From_MLME_Scan", "WSN_NWK_Process [From_MLME_Scan exit execs]")


			/** state (From_MLME_Scan) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [From_MLME_Scan trans conditions]", state13_trans_conds)
			FSM_INIT_COND (FAILT)
			FSM_TEST_COND (SUCCESS)
			FSM_TEST_COND (g_nwk_result == 2)
			FSM_TEST_LOGIC ("From_MLME_Scan")
			FSM_PROFILE_SECTION_OUT (state13_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, ;, "FAILT", "", "From_MLME_Scan", "Offline", "tr_66", "WSN_NWK_Process [From_MLME_Scan -> Offline : FAILT / ]")
				FSM_CASE_TRANSIT (1, 3, state3_enter_exec, nwk_associate_request(0);, "SUCCESS", "nwk_associate_request(0)", "From_MLME_Scan", "Associating", "tr_70", "WSN_NWK_Process [From_MLME_Scan -> Associating : SUCCESS / nwk_associate_request(0)]")
				FSM_CASE_TRANSIT (2, 2, state2_enter_exec, ;, "g_nwk_result == 2", "", "From_MLME_Scan", "WaitingScan", "tr_95", "WSN_NWK_Process [From_MLME_Scan -> WaitingScan : g_nwk_result == 2 / ]")
				}
				/*---------------------------------------------------------*/



			/** state (init) enter executives **/
			FSM_STATE_ENTER_UNFORCED (14, "init", state14_enter_exec, "WSN_NWK_Process [init enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (29,"WSN_NWK_Process")


			/** state (init) exit executives **/
			FSM_STATE_EXIT_UNFORCED (14, "init", "WSN_NWK_Process [init exit execs]")


			/** state (init) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [init trans conditions]", state14_trans_conds)
			FSM_INIT_COND (INTRPT_SELF_INIT)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("init")
			FSM_PROFILE_SECTION_OUT (state14_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, ;, "INTRPT_SELF_INIT", "", "init", "Offline", "tr_77", "WSN_NWK_Process [init -> Offline : INTRPT_SELF_INIT / ]")
				FSM_CASE_TRANSIT (1, 14, state14_enter_exec, ;, "default", "", "init", "init", "tr_79", "WSN_NWK_Process [init -> init : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (WaitingLeaving) enter executives **/
			FSM_STATE_ENTER_UNFORCED (15, "WaitingLeaving", state15_enter_exec, "WSN_NWK_Process [WaitingLeaving enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [WaitingLeaving enter execs]", state15_enter_exec)
				{
				/*
					�ȴ�MAC���������
				*/
				//op_prg_log_entry_write(g_nwk_debugger_log_handle, "waiting aaaaa!\n");
				}
				FSM_PROFILE_SECTION_OUT (state15_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (31,"WSN_NWK_Process")


			/** state (WaitingLeaving) exit executives **/
			FSM_STATE_EXIT_UNFORCED (15, "WaitingLeaving", "WSN_NWK_Process [WaitingLeaving exit execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [WaitingLeaving exit execs]", state15_exit_exec)
				{
				//op_prg_log_entry_write(g_nwk_debugger_log_handle, "waiting bbbb!\n");
				}
				FSM_PROFILE_SECTION_OUT (state15_exit_exec)


			/** state (WaitingLeaving) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [WaitingLeaving trans conditions]", state15_trans_conds)
			FSM_INIT_COND (INTRPT_STRM_MLME)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("WaitingLeaving")
			FSM_PROFILE_SECTION_OUT (state15_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 16, state16_enter_exec, ;, "INTRPT_STRM_MLME", "", "WaitingLeaving", "From_MLME_LEAVE", "tr_87", "WSN_NWK_Process [WaitingLeaving -> From_MLME_LEAVE : INTRPT_STRM_MLME / ]")
				FSM_CASE_TRANSIT (1, 15, state15_enter_exec, ;, "default", "", "WaitingLeaving", "WaitingLeaving", "tr_93", "WSN_NWK_Process [WaitingLeaving -> WaitingLeaving : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (From_MLME_LEAVE) enter executives **/
			FSM_STATE_ENTER_FORCED (16, "From_MLME_LEAVE", state16_enter_exec, "WSN_NWK_Process [From_MLME_LEAVE enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [From_MLME_LEAVE enter execs]", state16_enter_exec)
				{
				int l_nwk_packet_type;
				Packet* l_nwk_packet;
				int res;
				g_nwk_result = 0;
				printf("g_nwk_my_father_short_address = %d , g_nwk_specify_father = %d \n", g_nwk_my_father_short_address, g_nwk_specify_father);
				if(op_intrpt_strm() == IN_MLME_TO_NWK)
					{
					l_nwk_packet = op_pk_get (op_intrpt_strm());
					op_pk_fd_get(l_nwk_packet,0,&l_nwk_packet_type);
					if(l_nwk_packet_type == LEAVE)
						{
						op_pk_fd_get(l_nwk_packet,1,&res);
						g_nwk_result = nwk_leave_confirm(res);
						op_prg_log_entry_write(g_nwk_debugger_log_handle, "receive a Leave MLME! res = %d\n", res);
						}
					else
						{
						printf("WaitingLeaviing receive a other MLME!\n");
						op_prg_log_entry_write(g_nwk_debugger_log_handle, "WaitingLeaviing receive a other MLME!\n");
						}
					}
				if (SUCCESS&&g_nwk_specify_father==0) op_prg_log_entry_write(g_nwk_debugger_log_handle, "offline\n");
				}
				FSM_PROFILE_SECTION_OUT (state16_enter_exec)

			/** state (From_MLME_LEAVE) exit executives **/
			FSM_STATE_EXIT_FORCED (16, "From_MLME_LEAVE", "WSN_NWK_Process [From_MLME_LEAVE exit execs]")


			/** state (From_MLME_LEAVE) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_NWK_Process [From_MLME_LEAVE trans conditions]", state16_trans_conds)
			FSM_INIT_COND (SUCCESS&&g_nwk_specify_father==0)
			FSM_TEST_COND (REASSOCIATE)
			FSM_TEST_COND (FAILT)
			FSM_TEST_LOGIC ("From_MLME_LEAVE")
			FSM_PROFILE_SECTION_OUT (state16_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, nwk_leave_set();, "SUCCESS&&g_nwk_specify_father==0", "nwk_leave_set()", "From_MLME_LEAVE", "Offline", "tr_28", "WSN_NWK_Process [From_MLME_LEAVE -> Offline : SUCCESS&&g_nwk_specify_father==0 / nwk_leave_set()]")
				FSM_CASE_TRANSIT (1, 3, state3_enter_exec, nwk_associate_request(0);nwk_leave_set();, "REASSOCIATE", "nwk_associate_request(0);nwk_leave_set()", "From_MLME_LEAVE", "Associating", "tr_29", "WSN_NWK_Process [From_MLME_LEAVE -> Associating : REASSOCIATE / nwk_associate_request(0);nwk_leave_set()]")
				FSM_CASE_TRANSIT (2, 15, state15_enter_exec, ;, "FAILT", "", "From_MLME_LEAVE", "WaitingLeaving", "tr_88", "WSN_NWK_Process [From_MLME_LEAVE -> WaitingLeaving : FAILT / ]")
				}
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"WSN_NWK_Process")
		}
	}




void
_op_WSN_NWK_Process_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_WSN_NWK_Process_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_WSN_NWK_Process_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_WSN_NWK_Process_svar function. */
#undef g_nwk_result
#undef g_nwk_nodeID
#undef g_nwk_father_table
#undef g_nwk_my_father_short_address
#undef g_nwk_capability
#undef g_nwk_node_type
#undef g_nwk_my_ESN_address
#undef g_nwk_join_type
#undef g_nwk_children_table
#undef g_nwk_routing_table
#undef g_nwk_my_short_address
#undef g_nwk_end_address_pool
#undef g_nwk_routing_table_start
#undef g_nwk_children_table_start
#undef g_nwk_myBand
#undef g_nwk_PANID
#undef g_nwk_backup_router
#undef g_nwk_my_frequency
#undef g_nwk_my_routing_sequence
#undef g_nwk_my_multicast_table
#undef g_nwk_children_multicast_table
#undef g_nwk_main_routing_address
#undef g_nwk_main_address
#undef g_nwk_main_router_band
#undef g_nwk_main_router_frequency
#undef g_nwk_main_router_father
#undef g_nwk_synchronize_queue
#undef g_nwk_available_potential_father_number
#undef g_nwk_specify_father
#undef g_nwk_synchronize_sequence
#undef g_nwk_mulcast_maintain
#undef g_nwk_info_log_handle
#undef g_nwk_debugger_log_handle
#undef g_nwk_syn_seq
#undef g_nwk_macLeaveFlag
#undef bits_rcvd_stathandle
#undef bitssec_rcvd_stathandle
#undef pkts_rcvd_stathandle
#undef pktssec_rcvd_stathandle
#undef ete_delay_stathandle
#undef bits_rcvd_gstathandle
#undef bitssec_rcvd_gstathandle
#undef pkts_rcvd_gstathandle
#undef pktssec_rcvd_gstathandle
#undef ete_delay_gstathandle
#undef rcvd_pkts
#undef send_pkts
#undef recv_send_rate
#undef ete_delay_l_n
#undef online_number_gstathandle
#undef recv_send_rate_down
#undef ete_delay_gstathandle_down
#undef multicast_count
#undef leave_node_count
#undef g_nwk_test_type
#undef g_nwk_join_time
#undef join_time_lstathandle
#undef send_up_num
#undef send_dowm_num
#undef recv_up_num
#undef recv_dowm_num
#undef g_nwk_r2b_Map
#undef g_nwk_beacon_confirm
#undef g_nwk_beacon_confirm_count
#undef g_nwk_reassociate_leave_flag

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_WSN_NWK_Process_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_WSN_NWK_Process_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (WSN_NWK_Process)",
		sizeof (WSN_NWK_Process_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_WSN_NWK_Process_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	WSN_NWK_Process_state * ptr;
	FIN_MT (_op_WSN_NWK_Process_alloc (obtype))

	ptr = (WSN_NWK_Process_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "WSN_NWK_Process [INIT enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_WSN_NWK_Process_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	WSN_NWK_Process_state		*prs_ptr;

	FIN_MT (_op_WSN_NWK_Process_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (WSN_NWK_Process_state *)gen_ptr;

	if (strcmp ("g_nwk_result" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_result);
		FOUT
		}
	if (strcmp ("g_nwk_nodeID" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_nodeID);
		FOUT
		}
	if (strcmp ("g_nwk_father_table" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_father_table);
		FOUT
		}
	if (strcmp ("g_nwk_my_father_short_address" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_my_father_short_address);
		FOUT
		}
	if (strcmp ("g_nwk_capability" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_capability);
		FOUT
		}
	if (strcmp ("g_nwk_node_type" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_node_type);
		FOUT
		}
	if (strcmp ("g_nwk_my_ESN_address" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_my_ESN_address);
		FOUT
		}
	if (strcmp ("g_nwk_join_type" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_join_type);
		FOUT
		}
	if (strcmp ("g_nwk_children_table" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_children_table);
		FOUT
		}
	if (strcmp ("g_nwk_routing_table" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_routing_table);
		FOUT
		}
	if (strcmp ("g_nwk_my_short_address" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_my_short_address);
		FOUT
		}
	if (strcmp ("g_nwk_end_address_pool" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->g_nwk_end_address_pool);
		FOUT
		}
	if (strcmp ("g_nwk_routing_table_start" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_routing_table_start);
		FOUT
		}
	if (strcmp ("g_nwk_children_table_start" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_children_table_start);
		FOUT
		}
	if (strcmp ("g_nwk_myBand" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_myBand);
		FOUT
		}
	if (strcmp ("g_nwk_PANID" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_PANID);
		FOUT
		}
	if (strcmp ("g_nwk_backup_router" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_backup_router);
		FOUT
		}
	if (strcmp ("g_nwk_my_frequency" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_my_frequency);
		FOUT
		}
	if (strcmp ("g_nwk_my_routing_sequence" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_my_routing_sequence);
		FOUT
		}
	if (strcmp ("g_nwk_my_multicast_table" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_my_multicast_table);
		FOUT
		}
	if (strcmp ("g_nwk_children_multicast_table" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_children_multicast_table);
		FOUT
		}
	if (strcmp ("g_nwk_main_routing_address" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_main_routing_address);
		FOUT
		}
	if (strcmp ("g_nwk_main_address" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_main_address);
		FOUT
		}
	if (strcmp ("g_nwk_main_router_band" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_main_router_band);
		FOUT
		}
	if (strcmp ("g_nwk_main_router_frequency" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_main_router_frequency);
		FOUT
		}
	if (strcmp ("g_nwk_main_router_father" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_main_router_father);
		FOUT
		}
	if (strcmp ("g_nwk_synchronize_queue" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_synchronize_queue);
		FOUT
		}
	if (strcmp ("g_nwk_available_potential_father_number" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_available_potential_father_number);
		FOUT
		}
	if (strcmp ("g_nwk_specify_father" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_specify_father);
		FOUT
		}
	if (strcmp ("g_nwk_synchronize_sequence" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_synchronize_sequence);
		FOUT
		}
	if (strcmp ("g_nwk_mulcast_maintain" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_mulcast_maintain);
		FOUT
		}
	if (strcmp ("g_nwk_info_log_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_info_log_handle);
		FOUT
		}
	if (strcmp ("g_nwk_debugger_log_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_debugger_log_handle);
		FOUT
		}
	if (strcmp ("g_nwk_syn_seq" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_syn_seq);
		FOUT
		}
	if (strcmp ("g_nwk_macLeaveFlag" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->g_nwk_macLeaveFlag);
		FOUT
		}
	if (strcmp ("bits_rcvd_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->bits_rcvd_stathandle);
		FOUT
		}
	if (strcmp ("bitssec_rcvd_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->bitssec_rcvd_stathandle);
		FOUT
		}
	if (strcmp ("pkts_rcvd_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pkts_rcvd_stathandle);
		FOUT
		}
	if (strcmp ("pktssec_rcvd_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pktssec_rcvd_stathandle);
		FOUT
		}
	if (strcmp ("ete_delay_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ete_delay_stathandle);
		FOUT
		}
	if (strcmp ("bits_rcvd_gstathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->bits_rcvd_gstathandle);
		FOUT
		}
	if (strcmp ("bitssec_rcvd_gstathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->bitssec_rcvd_gstathandle);
		FOUT
		}
	if (strcmp ("pkts_rcvd_gstathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pkts_rcvd_gstathandle);
		FOUT
		}
	if (strcmp ("pktssec_rcvd_gstathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pktssec_rcvd_gstathandle);
		FOUT
		}
	if (strcmp ("ete_delay_gstathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ete_delay_gstathandle);
		FOUT
		}
	if (strcmp ("rcvd_pkts" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->rcvd_pkts);
		FOUT
		}
	if (strcmp ("send_pkts" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->send_pkts);
		FOUT
		}
	if (strcmp ("recv_send_rate" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->recv_send_rate);
		FOUT
		}
	if (strcmp ("ete_delay_l_n" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ete_delay_l_n);
		FOUT
		}
	if (strcmp ("online_number_gstathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->online_number_gstathandle);
		FOUT
		}
	if (strcmp ("recv_send_rate_down" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->recv_send_rate_down);
		FOUT
		}
	if (strcmp ("ete_delay_gstathandle_down" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ete_delay_gstathandle_down);
		FOUT
		}
	if (strcmp ("multicast_count" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->multicast_count);
		FOUT
		}
	if (strcmp ("leave_node_count" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->leave_node_count);
		FOUT
		}
	if (strcmp ("g_nwk_test_type" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_test_type);
		FOUT
		}
	if (strcmp ("g_nwk_join_time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_join_time);
		FOUT
		}
	if (strcmp ("join_time_lstathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->join_time_lstathandle);
		FOUT
		}
	if (strcmp ("send_up_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->send_up_num);
		FOUT
		}
	if (strcmp ("send_dowm_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->send_dowm_num);
		FOUT
		}
	if (strcmp ("recv_up_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->recv_up_num);
		FOUT
		}
	if (strcmp ("recv_dowm_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->recv_dowm_num);
		FOUT
		}
	if (strcmp ("g_nwk_r2b_Map" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_r2b_Map);
		FOUT
		}
	if (strcmp ("g_nwk_beacon_confirm" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_beacon_confirm);
		FOUT
		}
	if (strcmp ("g_nwk_beacon_confirm_count" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_beacon_confirm_count);
		FOUT
		}
	if (strcmp ("g_nwk_reassociate_leave_flag" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->g_nwk_reassociate_leave_flag);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}


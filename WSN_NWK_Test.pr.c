/* Process model C form file: WSN_NWK_Test.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char WSN_NWK_Test_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A op_runsim 7 61D05971 61D05971 1 DESKTOP-RD4S7T2 51133 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                  ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

//----------------------------------Node Type Define-------------------------------------//
#define ROUTERNODE		1
#define GATENODE		0
#define ENDNODE			2
#define BACKUPNODE		3

#define GateAddress			43690


//------------------------------Node Frequency Num Define---------------------------------//
#define GATEFREQNUM		2
#define FREQNUM			2

//-----------------------------Timer Parameters Define-----------------------------------//
#define UPtime						5
#define RTtime						100
#define CTTimercount				1
#define BackupTimercount			10
#define RT_ActiveCount				1200
#define RT_TimeoutCount				20
#define UPStatustime				20
#define JoinTime					60
#define Timer						10
#define MappingCount				120000

//---------------------------------Stream Port Define------------------------------------//
#define From_DATA		1
#define From_CON		2
#define From_MCPS		0
#define From_MLME		3
#define To_MLME			4
#define To_father		0
#define To_children		0

//------------------------------------Status Define----------------------------------------//
#define RT_Active				0
#define RT_Timeout				1
#define RT_Deleted				2
#define CT_Illegal				0
#define CT_Legitimate			1
#define CT_Deleted				2

//----------------------------- Interrupt Code Define------------------------------------//
#define intrCode_Timer				0
#define intrCode_Join				1
#define intrCode_ChangeMainRouter	2
#define intrCode_ReJoin				3

//----------------------------Upper Limit Parameters Define------------------------------//
#define MAXTTL								2
#define MAX_NODE							200
#define MAX_ROUTER							48
#define MAX_CHILD							50
#define MAX_RouterAddress					128
#define MAX_MULTICAST_NUM					16
#define MAX_FATHERTABLE_NUM					6
#define MAXRTInfo							9
#define MAXCTInfo							5
#define QUEUE_LEN							5


//-------------------------------Online Condition Define---------------------------------//
#define STRM_DATA			((op_intrpt_type() == OPC_INTRPT_STRM) &&(op_intrpt_strm() == From_DATA))
#define STRM_CON			((op_intrpt_type() == OPC_INTRPT_STRM) &&(op_intrpt_strm() == From_CON))
#define STRM_MCPS			((op_intrpt_type() == OPC_INTRPT_STRM) &&(op_intrpt_strm() == From_MCPS))
#define STRM_MLME			((op_intrpt_type() == OPC_INTRPT_STRM) &&(op_intrpt_strm() == From_MLME))
#define UpTimerCount		((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() != intrCode_ReJoin) && (op_intrpt_code() != intrCode_Join))
#define JoinTimer			((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == intrCode_Join))
#define REJOIN				((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == intrCode_ReJoin))


//--------------------------------Other Condition Define---------------------------------//
#define NON_FATHER		(fatherTable.number == 0)
#define NONA_FATHER		(fatherTable.number != 0)
#define SUCCESS			(result == 1)
#define FAILT			(result == 0)
#define JOINFAILT		((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == intrCode_Join))||(FAILT)


//---------------------------------Field Length Define-----------------------------------//
#define FRAMCONTR_LEN		16
#define MACSEQ_LEN			8
#define PANID_LEN			16
#define SHORTADDR_LEN		16
#define LONGADDR_LEN		48
#define NWKCONTR_LEN		16
#define LENGTH_LEN			8

//------------------------------Broadcast Address Define---------------------------------//
#define BroadCast			65535
#define RouterCast			65534
#define ChildrenCast		65533
#define ChildrenEndCast		65532



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


struct SynchronizeTable{
	int sequence;
	int status;
	int routerStart;
	int routerEnd;
	int childStart;
	int childEnd;
};

struct SynchronizeQueue{
	int number;
	struct SynchronizeTable table[QUEUE_LEN];
};

struct NetworkMsg{
	struct RTable NetworkRT[MAX_NODE];
	struct CTable NetworkCT[MAX_NODE];
};

struct Potential_Parent
{
	int short_addr;
	double band;
	int frequency;
	int router_cost;
	int depth;
	int times;
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
struct NetworkMsg Network_Msg;

struct mappingTable mapping[MAX_NODE];//网关节点使用的长短地址映射表

int routerAddressFlag[MAX_ROUTER];//网关节点保存的路由地址池

int bandTable[MAX_ROUTER];//网关节点维护的频段池

static void CRTinit();
static void initgate();
static void online();
static void scan();
static void associate();
static void join();


static void generatePk(int pklen,int multicast,int ar,int sourceMode,int destMode,int source,int dest,int type,int TTL,int NWKLen,Packet* PayLoad,int streamPort,double delay);
static int nextHop(int dest);
static void forward(Packet* pkptr,int streamPort);
static void JoinReqPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
static void accessConfirmPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
static void RoutingPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
static void multicastContrPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
static void leavePro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
static void mainRouterSnd();
static void mainRouterPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
static void mainRouterStatusPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
static void backupPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
static void switchPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
static void nwkStatuspro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
static void routingControl(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
static void nodeRecover(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
static void upDataPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);
static void downDataPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len);


/*
	表的增删改查
*/
static void generateCT(int ESNAddress,int NWKAddress,int CapabilityInformation, int status);
static void DeleteCT(int type, int ESNAddress);
static void generateRT(int destAddress,int nextHop,int status,int routerSeq);
static void UpdateRT(int NWKAddress, int status);
static void deleteRouting(int routerSeq, int NWKAddress);
static int queryRTIndex(int NWKAddress);
static int CTNWKAddresss(int ESNAddress);
static void UPCTStatus(int ESNAddress,int NWKAddress);
static int quiryNWKAddress(int MACAddress);
static int quiryRouterSeq(int NWKAddress);
static int CTinclude(int NWKAddress);
static int bandQuire(int NWKAddress);
static int quiryMT(int multicastID);
static int quiryChildMT(int dest);
static void maintainCMT(int multicastMain);
static int quiryNWKAddressFromMap(int NWKAddress);
static int queryFT(int fatherAddress);


/*
	资源的分配和释放
*/
static int distAddress(int deviceType, int shortAddress, int ESNAddress);
static int distBand(int NWKAddress);
static int distrFrequency(int deviceType);
static void leaveNet(int NWKAddress,int rejoin,int children,int assign);

/*
	主备信息同步
*/
static void synchronizeQueue(int status, int rstart, int rend,int cstart, int cend);
static void synchronizeSend();
static void initBackup();
static void tableBackup(int type,int addOrReduce,int NWKAddress,int start,int num);

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
	int	                    		result                                          ;	/* 用于标识扫描结果 */
	int	                    		nodeID                                          ;	/* 节点ID */
	struct FatherTable	     		fatherTable                                     ;	/* 潜在父节点表 */
	int	                    		father                                          ;	/* 选择的父节点 */
	int	                    		capability                                      ;	/* 节点能力 */
	int	                    		nodeType                                        ;	/* 节点类型 */
	int	                    		myESNAddress                                    ;	/* 节点长地址 */
	int	                    		joinType                                        ;	/* 节点入网类型 */
	struct CTable	          		CT                                              ;	/* 子节点表 */
	struct RTable	          		RT                                              ;	/* 路由表 */
	int	                    		myNWKAddress                                    ;	/* 网络层短地址 */
	int	                    		endAddressFlag[MAX_NODE]                        ;	/* 节点为终端节点分配的地址池 */
	int	                    		startRT                                         ;	/* 标识路由备份的进度 */
	int	                    		startCT                                         ;	/* 标识子节点表的进度 */
	int	                    		myBand                                          ;	/* 节点分配的频段号 */
	int	                    		PANID                                           ;	/* PAN标识符 */
	int	                    		backupRouter                                    ;	/* 备份节点接入标识 */
	int	                    		myFrequency                                     ;	/* 本地设备上行频点号 */
	int	                    		myRouterSeq                                     ;	/* 本地路由序列号 */
	int	                    		myMT                                            ;	/* 本地组播表 */
	struct childMulticastTable			childMT                                         ;	/* 下游节点组播表 */
	int	                    		mainRouter                                      ;	/* 主路由长地址 */
	int	                    		mainNWKAddress                                  ;	/* 主路由短地址 */
	int	                    		mainBand                                        ;	/* 主路由频段 */
	int	                    		mainFrequency                                   ;	/* 主路由上行频点 */
	int	                    		mainFather                                      ;	/* 主路由的父节点短地址 */
	struct SynchronizeQueue			syncQ                                           ;	/* 主备同步的同步队列 */
	} WSN_NWK_Test_state;

#define result                  		op_sv_ptr->result
#define nodeID                  		op_sv_ptr->nodeID
#define fatherTable             		op_sv_ptr->fatherTable
#define father                  		op_sv_ptr->father
#define capability              		op_sv_ptr->capability
#define nodeType                		op_sv_ptr->nodeType
#define myESNAddress            		op_sv_ptr->myESNAddress
#define joinType                		op_sv_ptr->joinType
#define CT                      		op_sv_ptr->CT
#define RT                      		op_sv_ptr->RT
#define myNWKAddress            		op_sv_ptr->myNWKAddress
#define endAddressFlag          		op_sv_ptr->endAddressFlag
#define startRT                 		op_sv_ptr->startRT
#define startCT                 		op_sv_ptr->startCT
#define myBand                  		op_sv_ptr->myBand
#define PANID                   		op_sv_ptr->PANID
#define backupRouter            		op_sv_ptr->backupRouter
#define myFrequency             		op_sv_ptr->myFrequency
#define myRouterSeq             		op_sv_ptr->myRouterSeq
#define myMT                    		op_sv_ptr->myMT
#define childMT                 		op_sv_ptr->childMT
#define mainRouter              		op_sv_ptr->mainRouter
#define mainNWKAddress          		op_sv_ptr->mainNWKAddress
#define mainBand                		op_sv_ptr->mainBand
#define mainFrequency           		op_sv_ptr->mainFrequency
#define mainFather              		op_sv_ptr->mainFather
#define syncQ                   		op_sv_ptr->syncQ

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	WSN_NWK_Test_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((WSN_NWK_Test_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif

/*
	初始化子节点表和路由表
*/
static void CRTinit()
{
	FIN(CRTinit());
	RT.number = 0;
	Network_Msg.NetworkCT[myESNAddress].number = 0;
	Network_Msg.NetworkRT[myESNAddress] = RT;
	startRT = 0;
	startCT = 0;
	if(joinType ==3 && backupRouter > 0)
		{
		printf("!!!!!!!\n");
		}
	FOUT;
}

/*
	初始化网关节点的表项
*/
static void initgate()
{
	int i ;
	FIN(initgate());
	for(i = 0;i < MAX_ROUTER; i++)
		{
		routerAddressFlag[i] = 0;
		bandTable[i] = 0;
		}
	for(i = 0;i < MAX_NODE; i++)
		{
		mapping[i].shortAddress = 0;
		mapping[i].count = -1;
		}
	mapping[0].shortAddress = GateAddress;
	mapping[0].count = -1;
	CRTinit();
	FOUT;
}

/*
	初始化路由节点
*/
static void initrouter()
{
	int i ;
	FIN(initrouter());
	for(i = 0;i < MAX_NODE; i++)
		{
		endAddressFlag[i] = 0;
		}
	backupRouter = -1;
	startRT = -1;
	startCT = -1;
	syncQ.number = 0;
	CRTinit();
	FOUT;
}

/*
	进入在线状态的打印
*/
static void online()
{
	FIN(online());
	op_intrpt_schedule_self(op_sim_time() + Timer,intrCode_Timer);
	printf("Node %d online!\n", myESNAddress);
	FOUT;
}

/*
	向MAC发送MLME-SCAN.request原语要求MAC进行信道扫描
*/
static void scan()
	{
	int len = 8;
	Packet* mlme;
	FIN(scan());
	mlme = op_pk_create(len);
	op_pk_fd_set (mlme, 0, OPC_FIELD_TYPE_INTEGER, 0, len);
	op_pk_send(mlme, To_MLME);
	FOUT;
	}

/*
	从潜在父节点表中选择一个合适的父节点后，向MAC发送MLME-ASSOCIATE.request
*/
static void associate()
	{
	int i;
	int len = 8;
	Packet* mlme;
	int minCost = 1000000;
	int index = -1;
	FIN(associate());
	for(i = 0;i < MAX_FATHERTABLE_NUM;i++)
		{
		//在潜在父节点表中挑选合适的父节点
		/*
		if()
			{
			minCost = ;
			index = i;
			}
		*/
		}
	if(index >= 0)
		{
		/*
		fatherTable.NWKAddress = ;
		fatherTable.PANID = ;
		fatherTable.DeviceType = ;
		fatherTable.Depth = ;
		fatherTable.RoutingCost = ;
		fatherTable.Band = ;
		fatherTable.Frequency = ;
		mlme = op_pk_create(48);
		op_pk_fd_set (mlme, 0, OPC_FIELD_TYPE_INTEGER, 1, 8);
		op_pk_fd_set (mlme, 1, OPC_FIELD_TYPE_INTEGER, fatherTable.NWKAddress, 16);
		op_pk_fd_set (mlme, 2, OPC_FIELD_TYPE_INTEGER, fatherTable.PANID, 8);
		op_pk_fd_set (mlme, 3, OPC_FIELD_TYPE_INTEGER, fatherTable.Band, 8);
		op_pk_fd_set (mlme, 4, OPC_FIELD_TYPE_INTEGER, fatherTable.Frequency, 8);
		op_pk_send(mlme, To_MLME);
		*/
		}
	else
		{
		printf("没有合适的父设备，关联失败！\n");
		}
	FOUT;
	}


/*
	构建入网请求分组，向MAC发送MLME-DATA.request，同时创建一个入网计时器
*/
static void join()
	{
	int pklen = 16 * 8;
	int len;
	int info;
	Packet* mcps;
	FIN(join());
	info = (joinType<<6) | capability;
	if(joinType == 0)
		{
		len = 7;
		mcps = op_pk_create(len*8);
		op_pk_fd_set (mcps, 0, OPC_FIELD_TYPE_INTEGER, myESNAddress, 48);
		op_pk_fd_set (mcps, 1, OPC_FIELD_TYPE_INTEGER, joinType, 2);
		op_pk_fd_set (mcps, 2, OPC_FIELD_TYPE_INTEGER, info, 6);
		}
	else
		{
		if(nodeType == ROUTERNODE)
			{
			len = 10;
			mcps = op_pk_create(len*8);
			op_pk_fd_set (mcps, 0, OPC_FIELD_TYPE_INTEGER, myESNAddress, 48);
			op_pk_fd_set (mcps, 1, OPC_FIELD_TYPE_INTEGER, joinType, 2);
			op_pk_fd_set (mcps, 2, OPC_FIELD_TYPE_INTEGER, info, 6);
			op_pk_fd_set (mcps, 3, OPC_FIELD_TYPE_INTEGER, myNWKAddress, 16);
			op_pk_fd_set (mcps, 4, OPC_FIELD_TYPE_INTEGER, myBand, 8);
			}
		else if(nodeType == ENDNODE || nodeType == BACKUPNODE)
			{
			len = 9;
			mcps = op_pk_create(len*8);
			op_pk_fd_set (mcps, 0, OPC_FIELD_TYPE_INTEGER, myESNAddress, 48);
			op_pk_fd_set (mcps, 1, OPC_FIELD_TYPE_INTEGER, joinType, 2);
			op_pk_fd_set (mcps, 2, OPC_FIELD_TYPE_INTEGER, info, 6);
			op_pk_fd_set (mcps, 3, OPC_FIELD_TYPE_INTEGER, myNWKAddress, 16);
			}
		}
	pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LONGADDR_LEN + LENGTH_LEN;
	pklen += len*8;
	generatePk(pklen,0,1,3,2,myESNAddress,father,0,0,len,mcps,To_father,myESNAddress);
	printf("Node %d send a joinSeq!type = %d!\n", myESNAddress, joinType);
	op_intrpt_schedule_self(op_sim_time() + JoinTime,intrCode_Join);
	FOUT;
	}

/*
	创建一个帧，通过MCPS原语发送给MAC
*/
static void generatePk(int pklen,int multicast,int ar,int sourceMode,int destMode,int source,int dest,int type,int TTL,int NWKLen,Packet* PayLoad,int streamPort,double delay)
{
	Packet* pkptr;
	int frameControl = 10752|(multicast<<8)|(ar<<10);
	int NWKControl = 0;
	FIN(generatePk(int pklen,int multicast,int ar,int sourceMode,int destMode,int source,int dest,int type,int TTL,int NWKLen,Packet* PayLoad,int streamPort,double delay));
	pkptr = op_pk_create (pklen);
	frameControl |= sourceMode;
	frameControl |= (destMode<<4);
	//set framControl,index = 0
	op_pk_fd_set (pkptr, 0, OPC_FIELD_TYPE_INTEGER, frameControl, FRAMCONTR_LEN);
	//set MACSequence,index = 1
	op_pk_fd_set (pkptr, 1, OPC_FIELD_TYPE_INTEGER, 0, MACSEQ_LEN);
	//set PANID,index = 2
	op_pk_fd_set (pkptr, 2, OPC_FIELD_TYPE_INTEGER, PANID, PANID_LEN);
	//set destAddress,index = 3
	if(destMode == 1)	op_pk_fd_set (pkptr,3,OPC_FIELD_TYPE_INTEGER,0,0);
	else if(destMode == 2)	op_pk_fd_set (pkptr,3,OPC_FIELD_TYPE_INTEGER,dest, LONGADDR_LEN);
	else op_pk_fd_set (pkptr,3,OPC_FIELD_TYPE_INTEGER,dest, SHORTADDR_LEN);
	//set srcAddress,index = 4
	if(sourceMode == 1)	op_pk_fd_set (pkptr,4,OPC_FIELD_TYPE_INTEGER,0,0);
	else if(sourceMode == 2)	op_pk_fd_set (pkptr,4,OPC_FIELD_TYPE_INTEGER,source,LONGADDR_LEN);
	else op_pk_fd_set (pkptr,4,OPC_FIELD_TYPE_INTEGER,source,SHORTADDR_LEN);
	op_pk_fd_set (pkptr,5,OPC_FIELD_TYPE_INTEGER,NWKLen + 2,8);
	//set NWKControl,index = 5
	NWKControl |= (type<<12);
	NWKControl |= (TTL<<8);
	op_pk_fd_set (pkptr,7,OPC_FIELD_TYPE_INTEGER,NWKControl,16);
	//set NWKPayLoad,index = 6
	op_pk_fd_set (pkptr,8,OPC_FIELD_TYPE_PACKET,PayLoad,NWKLen*8);
	op_pk_send_delayed(pkptr,streamPort,delay);
	FOUT;
}
/*
	查找路由表
*/
static int nextHop(int dest)
{
	int i = 0;
	int router;
	FIN(nextHop(int dest));
	router = dest&(255<<8);
	if(router == myNWKAddress)
		{
		i = CTinclude(dest);
		if(i >= 0 && Network_Msg.NetworkCT[myESNAddress].table[i].status == CT_Legitimate)
			{
			dest = Network_Msg.NetworkCT[myESNAddress].table[i].NWKAddress;
			FRET(dest);
			}
		else	FRET(-1);
		}
	for(i = 0;i < RT.number;i++)
	{
		if(((RT.table[i].destAddress)<<8) == router && RT.table[i].status == RT_Active)
		{
			FRET((RT.table[i].nextHop)<<8);
		}
	}
	FRET(-1);
}

/*
	使用状态超时的路由表条目的下一跳
*/
static int nextHopMaint(int dest)
{
	int i = 0;
	int router;
	FIN(nextHopMaint(int dest));
	router = dest&(255<<8);
	if(router == myNWKAddress)	FRET(dest);
	for(i = 0;i < RT.number;i++)
	{
		if(((RT.table[i].destAddress)<<8) == router && RT.table[i].status == RT_Timeout)
		{
			FRET((RT.table[i].nextHop)<<8);
		}
	}
	FRET(-1);
}

/*
	转发分组
*/
static void forward(Packet* pkptr,int streamPort)
{
	int NWKControl;
	int TTL;
	FIN(forward(Packet* pkptr));
	op_pk_fd_get(pkptr,7,&NWKControl);
	TTL = 15&(NWKControl>>8);
	NWKControl ^= (TTL<<8);
	TTL--;
	if(TTL<0)
	{
		op_pk_destroy(pkptr);
		printf("PK was destroyed becouse of TTL down!\n");
		FOUT;
	}
	NWKControl |= (TTL<<8);
	op_pk_fd_set(pkptr,7,OPC_FIELD_TYPE_INTEGER,NWKControl,16);
	op_pk_send(pkptr,streamPort);
	//op_stat_write(contr_send_num,++sendNum);
	printf("\n\nNode %d forward !\n\n",myESNAddress);
	FOUT;
}


/*
	入网请求分组的接收处理
*/
static void JoinReqPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int ESNAddress;
	int join;
	int CapabilityInformation;
	int deviceType;
	
	int control;
	int NWKAddress = 0;
	int band = -1;
	int frequency;
	
	int nexthop;
	int shortAddress = -1;
	int pklen;
	Packet* pk;
	FIN(JoinReqPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	op_pk_fd_get(payLoad,0,&ESNAddress);
	op_pk_fd_get(payLoad,1,&join);
	op_pk_fd_get(payLoad,2,&CapabilityInformation);
	deviceType = CapabilityInformation>>4;
	if(join != 0)
		{
		op_pk_fd_get(payLoad,3,&shortAddress);
		if(deviceType == ROUTERNODE)	op_pk_fd_get(payLoad,4,&band);
		}
	/*
		非代理入网请求
	*/
	if(destMode == 2 && sourceMode == 3)
		{
		//point = distPoint();
		if(nodeType == ROUTERNODE)
			{
			NWKAddress = distAddress(deviceType, shortAddress, ESNAddress);//分配地址
			//创建代理入网请求
			if(join == 0 && deviceType == ENDNODE)
				{
				pk = op_pk_create(72);
				op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,ESNAddress,48);
				op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,join,2);
				op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER,CapabilityInformation,6);
				op_pk_fd_set(pk,3,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
				len = 9;
				}
			else if(join == 0 && deviceType == ROUTERNODE)
				{
				pk = op_pk_create(56);
				op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,ESNAddress,48);
				op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,join,2);
				op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER,CapabilityInformation,6);
				len = 7;
				}
			else if(join != 0 && deviceType == ENDNODE)
				{
				pk = op_pk_create(72);
				op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,ESNAddress,48);
				op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,join,2);
				op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER,CapabilityInformation,6);
				op_pk_fd_set(pk,3,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
				len = 9;
				}
			else if(join != 0 && deviceType == ROUTERNODE)
				{
				pk = op_pk_create(80);
				op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,ESNAddress,48);
				op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,join,2);
				op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER,CapabilityInformation,6);
				op_pk_fd_set(pk,3,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
				op_pk_fd_set(pk,4,OPC_FIELD_TYPE_INTEGER,band,8);
				len = 10;
				}
			else
				{
				printf("Node %d received a error joinreq! \n", myESNAddress);
				FOUT;
				}
			pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
			pklen += len*8;
			generatePk(pklen,0,1,2,1,myNWKAddress,GateAddress,0,MAXTTL-1,len,pk,To_father,0);//发送代理入网请求
			generateCT(ESNAddress,NWKAddress,CapabilityInformation, CT_Illegal);
			//op_stat_write(contr_send_num,++sendNum);
			printf("Node %d send a sourceSeq!!!\n",myESNAddress);
			}
		else
			{
			//printf("deviceType = %d\n",deviceType);
			
			NWKAddress = distAddress(deviceType, shortAddress, ESNAddress);//分配地址
			if(NWKAddress == -1)
				{
				printf("NWKAddress distr failt!\n");
				FOUT;
				}
			frequency = distrFrequency(GATENODE);//分配频点
			if(deviceType == ROUTERNODE)
				{
				//The gateway node establishes a routing table for the directly connected router and sends a confirm
				generateRT(NWKAddress,NWKAddress,0,0);
				//Assign band
				band = distBand(NWKAddress);
				control = 31;
				control |= (joinType<<5);
				pk = op_pk_create(96);//create a accessConfirm_payLoad
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
				control = 26;
				control |= (joinType<<5);
				pk = op_pk_create(80);//create a accessConfirm_payLoad
				op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,control,8);
				op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INT64,ESNAddress,48);
				op_pk_fd_set(pk,3,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
				op_pk_fd_set(pk,5,OPC_FIELD_TYPE_INTEGER,frequency,8);
				len = 10;
				}
			else
				{
				printf("Node %d received a error joinreq! \n", myESNAddress);
				FOUT;
				}
			pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN  + LONGADDR_LEN + LENGTH_LEN;
			pklen += len*8;
			generatePk(pklen,0,1,1,3,myNWKAddress,source,1,0,len,pk,To_children,0);//发送非代理入网确认
			if(join == 2)//优先级低，暂时不做处理
				{
				
				
				//需要添加路由.
				
				
				}
			//op_stat_write(contr_send_num,++sendNum);
			printf("Node %d send a accessConfirm!!!\n",myESNAddress);
			//Build the mapping table
			mapping[ESNAddress].shortAddress = NWKAddress;
			mapping[ESNAddress].count = MappingCount;
			//printf("mapping[%d]= %d\n",ESNAddress-1,mapping[ESNAddress]);
			generateCT(ESNAddress,NWKAddress,CapabilityInformation, CT_Legitimate);
			}
		}
	
	/*
		代理入网请求
	*/
	else
		{
		printf("Node %d received a agent joinSeq\n", myESNAddress);
		if(nodeType != GATENODE)	FOUT;//只有网关节点能够接收处理代理入网请求
		frequency = distrFrequency(ROUTERNODE);
		NWKAddress = distAddress(deviceType, shortAddress, ESNAddress);//Assign network address;
		if(deviceType==ROUTERNODE)
			{
			nexthop = nextHop(source);
			printf("------------nexthop = %d\n",nexthop);
			if(nexthop <= 0) FOUT;
			//Assign band
			band = distBand(NWKAddress);
			generateRT(NWKAddress,nexthop,0,0);//为新入网路由节点建立路由表
			len = 12;
			control = 159;
			control |= (joinType<<5);
			pk = op_pk_create(8*len);//create a accessConfirm_payLoad
			op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,control,8);
			op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,ESNAddress,48);
			op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER,0,8);
			op_pk_fd_set(pk,3,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
			op_pk_fd_set(pk,4,OPC_FIELD_TYPE_INTEGER,band,8);
			op_pk_fd_set(pk,5,OPC_FIELD_TYPE_INTEGER,0,8);
			//Build the mapping table
			mapping[ESNAddress].shortAddress = NWKAddress;
			mapping[ESNAddress].count = MappingCount;
			}
		else
			{
			op_pk_fd_get(payLoad,3,&shortAddress);
			len = 10;
			control = 146;
			control |= (joinType<<5);
			pk = op_pk_create(8*len);//create a accessConfirm_payLoad
			op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,control,8);
			op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,ESNAddress,48);
			op_pk_fd_set(pk,5,OPC_FIELD_TYPE_INTEGER,0,8);
			//Build the mapping table
			mapping[ESNAddress].shortAddress = shortAddress;
			mapping[ESNAddress].count = MappingCount;
			}
		pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
		pklen += len*8;
		generatePk(pklen,0,1,1,2,myNWKAddress,source,1, MAXTTL-1, len,pk,To_children,0);//发送代理入网确认
		if(join == 2)//优先级低，暂时不做处理
				{
				
				
				//需要添加路由.
				
				
				}
		//op_stat_write(contr_send_num,++sendNum);
		printf("Node %d send a accessConfirm!!!\n",myESNAddress);
		//Build the mapping table
		mapping[ESNAddress].shortAddress = NWKAddress;
		mapping[ESNAddress].count = MappingCount;
		//printf("mapping[%d]= %d\n",ESNAddress-1,mapping[ESNAddress]);
		}
	FOUT;
}


/*
	入网确认的接收处理
*/

static void accessConfirmPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int permit,shortAddrComp,bandComp,frequencyComp,routerSeqComp,join;
	int ESNAddress,accessContr,NWKAddress,routerSeq,band;
	int pklen;
	int frequency;
	
	Packet* pk;
	FIN(accessConfirmPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	
	op_pk_fd_get(payLoad,0,&accessContr);
	printf("accessContr = %d\n", accessContr);
	op_pk_fd_get(payLoad,5,&frequency);
	permit = 1&(accessContr>>4);
	join = 3&(accessContr>>5);
	shortAddrComp = 1&(accessContr>>3);
	bandComp = 1&(accessContr>>2);
	frequencyComp = 1&(accessContr>>1);
	printf("Node %d receive a accessConfirm pk dest = %d\n",myESNAddress,dest);
	if(permit == 1)
	{
		//代理入网确认
		if(destMode == 2&&dest == myNWKAddress)
		{
			printf("Node %d received a agent access\n", myESNAddress);
			op_pk_fd_get(payLoad,1,&ESNAddress);
			if(shortAddrComp == 1)//待入网节点是路由节点
				{
				op_pk_fd_get(payLoad,3,&NWKAddress);
				op_pk_fd_get(payLoad,2,&routerSeq);
				op_pk_fd_get(payLoad,4,&band);
				if(routerSeq >= 0)		generateRT(NWKAddress,NWKAddress,0,routerSeq);
				else					generateRT(NWKAddress,NWKAddress,0,0);
				len = 6;
				pk = op_pk_create(8*len);
				op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,accessContr,8);
				op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER,routerSeq,8);
				op_pk_fd_set(pk,3,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
				op_pk_fd_set(pk,4,OPC_FIELD_TYPE_INTEGER,band,8);
				op_pk_fd_set(pk,5,OPC_FIELD_TYPE_INTEGER,frequency,8);
				}
			//printf("---------------------------NWKAddress = %d\n",NWKAddress);
			//printf("------------------------------ENDNWKAddress = %d\n",NWKAddress);
			else//待入网节点是终端设备
				{
				NWKAddress = CTNWKAddresss(ESNAddress);
				printf("Node %d NWKAddress dist = %d\n", myESNAddress, NWKAddress);
				if(NWKAddress <= 0)
					{
					printf("Node %d NWKAddress dist error\n", myESNAddress);
					FOUT;
					}
				accessContr |= (1<<3);
				len = 4;
				pk = op_pk_create(8*len);
				op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,accessContr,8);
				op_pk_fd_set(pk,3,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
				op_pk_fd_set(pk,5,OPC_FIELD_TYPE_INTEGER,frequency,8);
				}
			pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LONGADDR_LEN + LENGTH_LEN;
			pklen += len*8;
			generatePk(pklen,0,1,2,3,myNWKAddress,ESNAddress,1,0,len,pk,To_children,0);//发送非代理入网确认
			printf("Node %d send a accessConfirm pk dest = %d\n",myESNAddress,ESNAddress);
			//op_stat_write(contr_send_num,++sendNum);
			UPCTStatus(ESNAddress,NWKAddress);
			/*
				如果有备份节点，则更新备份节点备份信息
			*/
			if(backupRouter > 0)
			{
				tableBackup(2,0,NWKAddress,0,1);
			}
			printf("UPCTStatus\n");
		}
		//非代理入网确认
		else if(destMode == 3 && dest == myESNAddress)
		{
			printf("Node %d received a not agent access\n", myESNAddress);
			routerSeqComp = 1&accessContr;
			printf("Node %d shortAddrComp = %d\n", myESNAddress, shortAddrComp);
			if(shortAddrComp==1)	
				{
				op_pk_fd_get(payLoad,3,&NWKAddress);
				myNWKAddress = NWKAddress;
				}
			if(bandComp==1)			
				{
				op_pk_fd_get(payLoad,4,&band);
				myBand = band;
				}
			if(frequencyComp==1)		
				{
				op_pk_fd_get(payLoad,5,&frequency);
				myFrequency = frequency;
				}
			if(routerSeqComp==1)	
				{
				op_pk_fd_get(payLoad,2,&routerSeq);
				myRouterSeq = routerSeq;
				}
			if(join == joinType && NWKAddress != myNWKAddress)
				{
				if(join == 1)
					{
					
					}
				else if(join == 2)
					{
					
					}
				}
			
			/*
			
			将获得的资源通知MAC层
			
			*/
			op_intrpt_disable(OPC_INTRPT_SELF, intrCode_Join, OPC_TRUE);//拒绝下一次入网计时器中断
			printf("----------------\n");
			if(nodeType == ROUTERNODE)	printf("NodeBand = %d       ",myBand);
			printf("myNWKAddress = %d\n",myNWKAddress);
			//printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!band = %d,point = %d!!!!!!!!!!!!!!!!!!!!!!!!!!!\n",myBand,myPoint);
			if(nodeType == BACKUPNODE)		op_intrpt_schedule_self(op_sim_time() + op_dist_uniform (10),intrCode_ChangeMainRouter);//短暂等待后寻找主路由
		}	
	}
	//Rejection is not considered in the simulation
	else
	{
		printf("permit == 0\n");
	}
	FOUT;
}


/*
	路由维护分组的接收处理
*/
static void RoutingPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int control, routerSeq, shortAddress;
	int pklen;
	Packet* pk;
	FIN(RoutingPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	op_pk_fd_get(payLoad,0,&control);
	printf("Node %d receive a routing pk ,control = %d\n", myESNAddress, control);
	if(control == 0)//路由回复的接收处理
		{
		UpdateRT(source, RT_Active);
		}
	else if(control == 1)//路由删除的接收处理
		{
		op_pk_fd_get(payLoad,1,&routerSeq);
		op_pk_fd_get(payLoad,2,&shortAddress);
		if(routerSeq>=0)
			{
			deleteRouting(routerSeq,shortAddress);
			}
		if(nodeType == GATENODE)
			{
			//资源暂不释放
			/*
			if(mapping[ESNAddress]==NWKAddress)
				{
				mapping[ESNAddress] = 0;
				}
			*/
			}
			
		}
	else if(control == 2)//路由请求的接收处理
		{
		//构建路由回复分组
		
		len = 1;
		pk = op_pk_create(8*len);
		op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,0,8);
		pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + SHORTADDR_LEN + LENGTH_LEN;
		pklen += 8*len;
		generatePk(pklen, 0, 1, 2, 2, myNWKAddress, source, 11, MAXTTL-1 , len, pk, To_father, 0);//发送路由维护回复
		printf("Node %d send a router response pk to %d!\n", myESNAddress, source);
		
		}
	FOUT;
}


/*
	组播控制分组的接收处理
*/
static void multicastContrPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int i;
	int MulticastInformation,type,number;
	int muticastID;
	FIN(multicastContrPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	op_pk_fd_get(payLoad,0,&MulticastInformation);
	type = 1&MulticastInformation;
	number = 15&(MulticastInformation>>1);
	printf("Node %d receive a MulticastContr to dest=%d!\n",myESNAddress,dest);
	printf("%d\n",MulticastInformation);	
	for(i = 0;i<number;i++)
	{
		op_pk_fd_get(payLoad,i+1,&muticastID);
		if(dest == myNWKAddress)
		{
			if(type == 1)	myMT |= (1<<muticastID);
			else			myMT &= (~(1<<muticastID));
			printf("Node %d add/subtract a muticastID = %d\n",myESNAddress,muticastID);
		}
		else if(nodeType == ROUTERNODE)
		{
			if(type == 1)	childMT.group |= (1<<muticastID);
			printf("Node %d add/subtract a muticastID = %d to ChildMT\n",myESNAddress,muticastID);
		}
	}
	FOUT;
}

/*
	离网分组的接收处理
*/
static void leavePro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int leaveInfo;
	int rejoin;
	int request;
	int removeChildren;
	int replaceRource;
	
	int pklen;
	
	int control = 1;
	int routerSeq;
	int NWKAddress;
	Packet* pk;
	FIN(leavePro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	printf("Node %d received a leaveControl pk\n",myESNAddress);
	op_pk_fd_get(payLoad,0,&leaveInfo);
	replaceRource = (leaveInfo>>4)&1;
	rejoin = (leaveInfo>>2)&3;
	request = (leaveInfo>>1)&1;
	removeChildren = leaveInfo&1;
	if(request == 1)
		{
		printf("removeChildren == %d && Network_Msg.NetworkCT[myESNAddress].number = %d\n",removeChildren,Network_Msg.NetworkCT[myESNAddress].number);
		if(removeChildren == 1 && Network_Msg.NetworkCT[myESNAddress].number > 0)
			{
			len = 1;
			pk = op_pk_create(8*len);
			op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,0,8);
			pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + SHORTADDR_LEN + LENGTH_LEN;
			pklen += 8*len;
			generatePk(pklen, 0, 0, 2, 2,myNWKAddress, ChildrenEndCast, 2, 0, len, pk, To_children, 0);
			}
		leaveNet(myNWKAddress,rejoin,removeChildren,0);
		}
	else
		{
		leaveNet(source,rejoin,removeChildren,0);
		routerSeq = quiryRouterSeq(source);
		len = 4;
		pk = op_pk_create (len*8);//构建路由删除分组
		op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,control,8);
		op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,routerSeq,8);
		op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER,source,16);
		pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
		pklen += len*8;
		generatePk(pklen, 0, 1, 2, 1, myNWKAddress, GateAddress, 11, MAXTTL-1, len, pk, To_father, 0);//发送路由删除分组
		}
	FOUT;
}


/*
	主路由分组查询的发送
*/
static void mainRouterSnd()
{
	int len, pklen;

	Packet* pk;
	FIN(mainRouterSnd());
	len = 7;
	pk = op_pk_create(len*8);
	op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,1,8);
	op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,mainRouter,48);
	pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
	pklen += 8*len;
	generatePk(pklen,0,1,2,1,myNWKAddress,GateAddress,12,MAXTTL-1, len, pk, To_children,0);//发送主路由查询分组
	FOUT;
}

/*
	主路由分组的接收处理
*/
static void mainRouterPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int type, ESNAddress, NWKAddress, band;
	int pklen;
	Packet* pk;
	FIN(mainRouterPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	op_pk_fd_get(payLoad,0,&type);
	printf("mainRouterPro type = %d\n",type);
	if(type == 1 && nodeType == GATENODE)
	{
		op_pk_fd_get(payLoad,1,&ESNAddress);
		NWKAddress = mapping[ESNAddress].shortAddress;
		if(NWKAddress <= 0)
		{
			FOUT;
		}
		band = bandQuire(NWKAddress);
		len = 4;
		pk = op_pk_create(len*8);
		op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,2,8);
		op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
		op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER,band,8);
		pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
		pklen += 8*len;
		generatePk(pklen,0,1,1,2,myNWKAddress,source,12,MAXTTL-1, len, pk, To_children,0);//发送主路由信息回复
		printf("Node %d received a mainRouterQuery query router %d's NWKAddress = %d\n",myESNAddress,ESNAddress,NWKAddress);
	}
	else if(type == 2 && nodeType != GATENODE)
	{
		op_pk_fd_get(payLoad,1,&NWKAddress);
		op_pk_fd_get(payLoad,2,&band);
		printf("need rejoin!!!,fatherAddress = %d,band = %d\n",NWKAddress,band);
		mainNWKAddress = NWKAddress;
		mainBand = band;
		len = 1;
		capability |= nodeType<<4;
		pk = op_pk_create (8*len);
		op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,4,8);
		pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + SHORTADDR_LEN + LENGTH_LEN;
		pklen += 8*len;
		generatePk(pklen,0,1,2,2,myNWKAddress,father, 2, 0, len,pk,To_father,0);//发送离网通知
		printf("Node %d send a leave advice\n",myESNAddress);
		father = mainNWKAddress;
		leaveNet(myNWKAddress,1,0,1);
	}
	FOUT;
}

/*
	主路由状态分组
*/
static void mainRouterStatusPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int control;
	int request;
	int status;
	int NWKAddress;
	int pklen;
	
	Packet* pk;
	FIN(mainRouterPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	op_pk_fd_get(payLoad,0,&control);
	request = (control>>7)&1;
	if(request == 0 && nodeType == ROUTERNODE)//query
	{
		op_pk_fd_get(payLoad,1,&NWKAddress);
		if( CTinclude(NWKAddress) >= 0)
		{
			//向该节点发送保活消息
		}
	}
	else if(request == 1 && nodeType == BACKUPNODE)//response
	{
		status = control&127;
		if(status == 0)//工作状态
			{
		
			}
		else if(status == 127)//失活状态，主备切换
			{
			myNWKAddress = mainNWKAddress;
			myBand = mainBand;
			myFrequency = mainFrequency;
			len = 8;
			pk = op_pk_create(8*len);//发送主备切换分组通知网关节点
			op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,myNWKAddress,16);
			op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER,myESNAddress,48);
			pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
			pklen += 8*len;
			generatePk(pklen, 0, 1, 2, 1, myNWKAddress, GateAddress, 3, MAXTTL-1, len, pk, To_father, 0);
			}
	}
	FOUT;
}

/*
	备份路由信息接收处理
*/
static void backupPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int i;
	int index = 0;
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
	int capabilityInformation;
	
	int control;
	
	FIN(backupPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	op_pk_fd_get(payLoad,0,&infoControl);
	type = (infoControl>>5)&3;
	addOrReduce = (infoControl>>4)&1;
	number = infoControl&15;
	printf("mainRouterInfoPro,type = %d,number = %d,addOrReduce = %d\n",type,number,addOrReduce);
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
						op_pk_fd_get(payLoad,++index,&capabilityInformation);
						if(addOrReduce == 0)
							{
							generateCT(ESNAddress,shortAddress,capabilityInformation, CT_Legitimate);
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
					op_pk_fd_get(payLoad,3,&capabilityInformation);
					if(control == 1)//子路由的信息更新
						{
						op_pk_fd_get(payLoad,4,&nextHop);
						op_pk_fd_get(payLoad,5,&status);
						op_pk_fd_get(payLoad,6,&routerSeq);
						if(addOrReduce == 0)
							{
							generateRT(NWKAddress,nextHop,status,routerSeq);
							generateCT(ESNAddress,NWKAddress,capabilityInformation, CT_Legitimate);
							}
						else
							{
							deleteRouting(-1,NWKAddress);
							DeleteCT(1, ESNAddress);
							}
						}
					else//子终端的信息更新
						{
						if(addOrReduce == 0)
							{
							generateCT(ESNAddress,NWKAddress,capabilityInformation, CT_Legitimate);
							}
						else
							{
							DeleteCT(1, ESNAddress);
							}
						}
					break;
		case 3: 	op_pk_fd_get(payLoad,++index,&mainFather);
					op_pk_fd_get(payLoad,++index,&mainBand);
					op_pk_fd_get(payLoad,++index,&mainFrequency);
					printf("mainFather = %d,mainBand = %d,mainPoint = %d\n",mainFather,mainBand,mainFrequency);
					break;
	}
	FOUT;
}

/*
	主备切换通知分组的接收处理
*/
static void switchPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int NWKAddress, ESNAddress;
	int i;
	FIN(switchPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	op_pk_fd_get(payLoad,0,&NWKAddress);
	op_pk_fd_get(payLoad,1,&ESNAddress);
	if(nodeType == GATENODE)
		{
		
		}
	else
		{
		i = CTinclude(NWKAddress);
		if(i >= 0)
			{
			Network_Msg.NetworkCT[myESNAddress].table[i].ESNAddress = ESNAddress;
			Network_Msg.NetworkCT[myESNAddress].table[i].status = CT_Legitimate;
			}
		}
	FOUT;
}

/*
	网络状态分组的接收处理
*/
static void nwkStatuspro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int NWKAddress, status;
	FIN(nwkStatuspro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	op_pk_fd_get(payLoad,0,&NWKAddress);
	op_pk_fd_get(payLoad,1,&status);
	printf("Node %d received a nwkStatus pk, shortAddress = %d , Status = %d!\n", myESNAddress, NWKAddress, status);
	FOUT;
}



/*
	路由控制分组的接收处理
*/
static void routingControl(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int control, num, routerSeq, shortAddress;
	int i, index, next;
	FIN(routingControl(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	index = -1;
	op_pk_fd_get(payLoad,++index,&control);
	op_pk_fd_get(payLoad,++index,&num);
	for(i = 0;i < num;i++)
		{
		op_pk_fd_get(payLoad,++index,&routerSeq);
		op_pk_fd_get(payLoad,++index,&shortAddress);
		shortAddress = (shortAddress<<8);
		if(control == 0)//添加
			{
			next = nextHop(shortAddress);
			generateRT(shortAddress, next, RT_Active, routerSeq);
			}
		else//删除
			{
			deleteRouting(routerSeq, shortAddress);
			}
		}
	FOUT;
}

/*
	网络节点恢复的接收处理
*/
static void nodeRecover(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	FIN(nodeRecover(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	/*
		节点进入重新入网过程
	*/
	FOUT;
}

/*
	上行数据的接收处理
*/
static void upDataPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int data;
	FIN(upDataPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	op_pk_fd_get(payLoad,0,&data);
	//op_pk_send(packet,1);
	//op_stat_write(ete_delay_up,op_sim_time () - op_pk_creation_time_get (packet));
	//printf("-------------------------------------------------------------------------delay = %f-------------------\n",op_sim_time () - op_pk_creation_time_get (payLoad));
	//op_stat_write(up_data_receive_num,++upReceiveNum);
	//op_stat_write(up_pk_loss_rate,upReceiveNum/upSendNum);
	printf("----------------------------Node %d receive a up data = %d!  ----",myESNAddress,data);
	printf("len = %d-------------------\n",len);
	FOUT;
}

/*
	下行数据的接收处理
*/
static void downDataPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len)
{
	int data;
	int nexthop;
	int ESNAddress;
	FIN(downDataPro(Packet* payLoad, int destMode, int dest, int sourceMode, int source, int len));
	op_pk_fd_get(payLoad,0,&data);
	//op_stat_write(ete_delay_down,op_sim_time () - op_pk_creation_time_get (packet));
	//printf("-------------------------------------------------------------------------delay = %f-------------------\n",op_sim_time () - op_pk_creation_time_get (payLoad));
	printf("\n\nNode %d receivea data %f!\n\n",myESNAddress,op_sim_time());
	//op_stat_write(down_data_receive_num,++downReceiveNum);
	//op_stat_write(down_pk_loss_rate,downReceiveNum/downSendNum);
	printf("len = %d-------------------\n",len);
	FOUT;
}

/*
	建立子节点表条目
*/
static void generateCT(int ESNAddress,int NWKAddress,int CapabilityInformation, int status)
{
	struct childrenTable T;
	FIN(gerenateCT(int ESNAddress,int NWKAddress,int CapabilityInformation, int status) );
	T.ESNAddress = ESNAddress;
	printf("Node %d generateCT T.ESNAddress = %d\n",myESNAddress,T.ESNAddress);
	T.NWKAddress = NWKAddress;
	T.status = status;
	T.CapabilityInformation = CapabilityInformation;
	Network_Msg.NetworkCT[myESNAddress].table[Network_Msg.NetworkCT[myESNAddress].number] = T;
	Network_Msg.NetworkCT[myESNAddress].number++;
	FOUT;
}
/*
	删除子节点表
*/

static void DeleteCT(int type, int address)
{
	Packet* pk;
	int i,j;
	int NWKAddress;
	int childNodeType;
	
	int control = 1;
	int routerSeq;
	int pklen;
	FIN(DeleteCT(int type, int address));
	if(address > 0)
	{
		for(i = 0;i<Network_Msg.NetworkCT[myESNAddress].number;i++)
		{
			if((type == 1 && Network_Msg.NetworkCT[myESNAddress].table[i].ESNAddress == address) || (type == 2 && Network_Msg.NetworkCT[myESNAddress].table[i].NWKAddress == address))
			{
				Network_Msg.NetworkCT[myESNAddress].table[i].status = CT_Deleted;
			}
		}
		
	}
	FOUT;
}


/*
	建立路由表条目
*/
static void generateRT(int destAddress,int nextHop,int status,int routerSeq)
{
	struct routerTable T;
	int i;
	FIN(gerenateRT(int destAddress,int nextHop,int status,int routerSeq));
	destAddress &= (255<<8);
	if(nextHop == 0 || destAddress == 0)	FOUT;
	printf("Node %d generateRT destAddress = %d\n",myESNAddress,destAddress);
	for(i = 0;i<RT.number;i++)
	{
		if(RT.table[i].destAddress == destAddress)
		{
			if(RT.table[i].routerSeq <= routerSeq)
			{
				RT.table[i].destAddress = (destAddress>>8);
				RT.table[i].nextHop = (nextHop>>8);
				RT.table[i].status = status;
				RT.table[i].routerSeq = routerSeq;
				RT.table[i].TimeoutCount = RT_ActiveCount;
			}
			FOUT;
		}
	}
	T.destAddress = (destAddress>>8);
	T.nextHop = (nextHop>>8);
	T.status = status;
	T.routerSeq = routerSeq;
	T.TimeoutCount = RT_ActiveCount;
	RT.table[RT.number] = T;
	printf("--------------------------------------Node %d add a RT destAddress = %d,TimeoutCount = %d\n",myESNAddress,RT.table[RT.number].destAddress,RT.table[RT.number].TimeoutCount);
	RT.number++;
	Network_Msg.NetworkRT[myESNAddress] = RT;
	printf("--------------------------------------Node %d add a RT destAddress = %d\n",myESNAddress,Network_Msg.NetworkRT[myESNAddress].table[Network_Msg.NetworkRT[myESNAddress].number - 1].destAddress);
	FOUT;
}



/*
	路由计数器更新或路由状态更新
	NWKAddress > 0 指示更新对应的路由条目状态，否则更新所有路由的计数器
*/
static void UpdateRT(int NWKAddress, int status)
{
	int i, len, pklen;
	Packet* pk;
	FIN(UpdateRT(int NWKAddress, int status));
	if(NWKAddress > 0)//路由状态更新
	{
		printf("Node %d UpdateRT NWKAddress = %d\n",myESNAddress,NWKAddress);
		i = queryRTIndex(NWKAddress);
		if(i >= 0)
		{
			//printf("Node %d receive a data from %d ,updateRT,i = %d,RT.number = %d\n",myESNAddress,NWKAddress,i,RT.number);
			RT.table[i].status = status;
			if(status == RT_Active)
				{
				RT.table[i].TimeoutCount = RT_ActiveCount;
				}
			if(status == RT_Timeout)
				{
				RT.table[i].TimeoutCount = RT_TimeoutCount;
				}
			if(status == RT_Deleted)//不会使用到
				{
				//发送路由删除分组
				len = 4;
				pk = op_pk_create (len * 8);
				op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,1,8);
				op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER, RT.table[i].routerSeq, 8);
				op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER, RT.table[i].destAddress, 16);
				pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
				pklen += 8*len;
				generatePk(pklen, 0, 1, 2, 1, myNWKAddress, GateAddress, 11, MAXTTL-1, len, pk, To_father, 0);
				}
		}
	}
	else//路由计数器更新
	{
		for(i = 0;i<RT.number;i++)
		{
			if(RT.table[i].status == RT_Deleted)	
				{
				printf("Node %d RT %d status = RT_Deleted\n", myESNAddress, RT.table[i].destAddress);
				continue;
				}
			--RT.table[i].TimeoutCount;
			//printf("Node %d RT node %d RT.table.TimeoutCount = %d\n",myESNAddress, RT.table[i].destAddress, RT.table[i].TimeoutCount);
			if(RT.table[i].TimeoutCount <= 0)
			{
				if(RT.table[i].status == RT_Active)//主动维护
					{
					RT.table[i].status = RT_Timeout;
					RT.table[i].TimeoutCount = RT_TimeoutCount;
					len = 1;
					pk = op_pk_create (len * 8);
					op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,2,8);
					pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + SHORTADDR_LEN + LENGTH_LEN;
					pklen += 8*len;
					generatePk(pklen, 0, 1, 2, 2, myNWKAddress, (RT.table[i].destAddress<<8), 11, MAXTTL-1, len, pk, To_children, 0);//发送主动路由维护请求
					printf("Node %d send a router seq to %d!\n", myESNAddress, RT.table[i].destAddress);
					}
				else if(RT.table[i].status == RT_Timeout)
					{
					RT.table[i].status = RT_Deleted;
					if(nodeType == GATENODE)	continue;
					len = 4;
					pk = op_pk_create (len * 8);
					op_pk_fd_set(pk,0,OPC_FIELD_TYPE_INTEGER,1,8);
					op_pk_fd_set(pk,1,OPC_FIELD_TYPE_INTEGER, RT.table[i].routerSeq, 8);
					op_pk_fd_set(pk,2,OPC_FIELD_TYPE_INTEGER, RT.table[i].destAddress, 16);
					pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
					pklen += 8*len;
					generatePk(pklen, 0, 1, 2, 1, myNWKAddress, GateAddress, 11, MAXTTL-1, len, pk, To_father, 0);//发送路由删除
					}
				else	printf("============\n");
			}
		}
	}
	Network_Msg.NetworkRT[myESNAddress] = RT;
	FOUT;
}

/*
	路由表的删除
*/
static void deleteRouting(int routerSeq, int NWKAddress)
{
	int i;
	FIN(deleteRouting(int routerSeq, int NWKAddress));
	for(i = 0;i<RT.number;i++)
		{
			if(NWKAddress == RT.table[i].destAddress)
				{
				if(routerSeq >= RT.table[i].routerSeq)
					{
					RT.table[i].status = RT_Deleted;
					RT.table[i].TimeoutCount = 0;
					}
				Network_Msg.NetworkRT[myESNAddress] = RT;
				printf("Node %d delete router %d========\n", myESNAddress, NWKAddress);
				break;
				}
		}
	FOUT;
}


/*
	查询对应路由在路由表的位置，返回索引
*/
static int queryRTIndex(int NWKAddress)
{
	int i;
	FIN(queryRTIndex(int NWKAddress));
	NWKAddress = (NWKAddress>>8);
	for(i = 0;i<RT.number;i++)
	{	
		if(RT.table[i].destAddress==NWKAddress)
		{
			FRET(i);
		}
	}
	FRET(-1);
}


/*
	通过长地址查找子节点表中的短地址
*/
static int CTNWKAddresss(int ESNAddress)
{
	int i = 0;
	FIN(CTNWKAddresss(int ESNAddress));
	for(i = 0;i < Network_Msg.NetworkCT[myESNAddress].number;i++)
	{
		if(Network_Msg.NetworkCT[myESNAddress].table[i].ESNAddress == ESNAddress)	FRET(Network_Msg.NetworkCT[myESNAddress].table[i].NWKAddress);
	}
	FRET(-1);
}

/*
	更新子节点表中的条目状态为合法状态
*/
static void UPCTStatus(int ESNAddress,int NWKAddress)
{
	int i = 0;
	int deviceType;
	FIN(UPCTStatus(int ESNAddress,int NWKAddress));
	printf("Network_Msg.NetworkCT[myESNAddress].number = %d\n",Network_Msg.NetworkCT[myESNAddress].number);
	for(i = 0;i < Network_Msg.NetworkCT[myESNAddress].number;i++)
	{
		if(Network_Msg.NetworkCT[myESNAddress].table[i].ESNAddress == ESNAddress)
		{
			if(NWKAddress > 0)	Network_Msg.NetworkCT[myESNAddress].table[i].NWKAddress = NWKAddress;
			Network_Msg.NetworkCT[myESNAddress].table[i].status = CT_Legitimate;
			deviceType = ((Network_Msg.NetworkCT[myESNAddress].table[i].CapabilityInformation)>>4);
			if(deviceType == 3)//备份节点需要立即发送备份信息
			{
				backupRouter = NWKAddress;
				initBackup();
			}
			FOUT;
		}
	}
	FOUT;
}


/*
	在映射表中通过长地址查找短地址
*/
static int quiryNWKAddress(int MACAddress)
{
	FIN(quiryNWKAddress(int MACAddress));
	FRET(mapping[MACAddress].shortAddress);
}


/*
	查找路由序列号
*/
static int quiryRouterSeq(int NWKAddress)
{
	int i;
	FIN(quiryRouterSeq(int NWKAddress));
	for(i = 0;i<RT.number;i++)
	{
		if(RT.table[i].destAddress == NWKAddress)
		{
			FRET(RT.table[i].routerSeq);
		}
	}
	FRET(-1);
}

/*
	在子节点表中通过短地址查找索引，若不存在子节点表条目返回-1
*/
static int CTinclude(int NWKAddress)
{
	int i = 0;
	FIN(CTinclude(int NWKAddress));
	//printf("Network_Msg.NetworkCT[myESNAddress].number = %d\n", Network_Msg.NetworkCT[myESNAddress].number);
	for(i = 0;i < Network_Msg.NetworkCT[myESNAddress].number;i++)
	{
		if(Network_Msg.NetworkCT[myESNAddress].table[i].NWKAddress == NWKAddress)	
		{
			FRET(i);
		}
	}
	FRET(-1);
}

/*
	网关节点通过短地址查找对应频段
*/
static int bandQuire(int NWKAddress)
{
	int i;
	FIN(bandQuire(int NWKAddress));
	for(i = 0;i<MAX_ROUTER;i++)
	{
		if(bandTable[i] == NWKAddress)
		{
			FRET(i);
		}
	}
	FRET(-1);
}

/*
	在组播表中查找是否处于对应组播组中
*/
static int quiryMT(int multicastID)
{
	int ret;
	FIN(quiryMT(int multicastID));
	ret = (1<<multicastID) & myMT;
	if(ret > 0)		FRET(1);
	FRET(-1);
}

/*
	在下游组播表中查找对应组播组
*/

static int quiryChildMT(int dest)
{
	int ret;
	FIN(quiryChildMT(int dest));
	ret = (1<<dest) & childMT.group;
	if(ret > 0)		FRET(1);
	FRET(-1);
}

/*
	下游组播表的定时维护
*/
static void maintainCMT(int multicastMain)
{
	FIN(maintainCMT(int multicastMain));
	if(multicastMain >= 0)
		{
		childMT.maintain = (multicastMain | (childMT.maintain));
		}
	else
		{
		--childMT.timerCount;
		if(childMT.timerCount <= 0)
			{
			childMT.group = childMT.maintain;
			childMT.maintain = 0;
			/*
			通过原语设置MAC组播缓存
			*/
			}
		}
	FOUT;
}


/*
	查找短地址在映射表中的位置即ESN长地址，返回索引/长地址
*/
static int quiryNWKAddressFromMap(int NWKAddress)
{
	int i;
	FIN(quiryNWKAddressFromMap(int NWKAddress));
	for(i = 1;i < MAX_NODE;i++)
	{
		if(NWKAddress == mapping[i].shortAddress)
		{
			FRET(i);
		}
	}
	FRET(-1);
}




/*
	查找潜在父节点表中对应父节点的位置，返回索引
*/

static int queryFT(int fatherAddress)
{
	//int i;
	FIN(queryFT(int fatherAddress));
	/*
	for(i = 0;i<MAX_FATHERTABLE_NUM;i++)
	{
		if(potentialParent[myESNAddress][i].short_addr == fatherAddress)	FRET(potentialParent[myESNAddress][i].band);
	}*/
	FRET(-1);
}


/*
	分配地址
*/
static int distAddress(int deviceType, int shortAddress, int ESNAddress)
{
	int i;
	int n;
	FIN(distAddress(int deviceType, int shortAddress, int ESNAddress));
	if(nodeType == GATENODE)	n = MAX_NODE;
	else if(nodeType == ROUTERNODE)	n = MAX_CHILD;
	else	FRET(0);
	switch(deviceType)
	{
		case ROUTERNODE: for(i = 0;i < MAX_ROUTER;i++)
				{
					if(nodeType == GATENODE && routerAddressFlag[i] == 0)
					{
						routerAddressFlag[i] = 1;
						shortAddress = (i + 1)<<8;
						FRET(shortAddress);
					}
				}
				break;
		case ENDNODE: for(i = 0;i < n;i++)
				{
					if(endAddressFlag[i] == 0)
					{
						endAddressFlag[i] = 1;
						shortAddress = (i + 1)|myNWKAddress;
						FRET(shortAddress);
					}
				}
				break;
		case BACKUPNODE: for(i = 0;i < n;i++)
				{
					if(endAddressFlag[i] == 0)
					{
						endAddressFlag[i] = 1;
						shortAddress = (i + 1)|myNWKAddress;
						FRET(shortAddress);
					}
				}
				break;
		default:FRET(0);
	}
	FRET(0);
}

/*
	分配频段
*/
static int distBand(int NWKAddress)
{
	int i;
	FIN(distBand(int NWKAddress));
	for(i = 1;i<MAX_ROUTER;i++)
	{
		if(bandTable[i]==0)
		{
			bandTable[i] = NWKAddress;
			FRET(i);
		}
	}
	FRET(-1);
}

/*
	分配频点，采用随机分配
	参数：0,网关；1,路由
*/
static int distrFrequency(int deviceType)
{
	FIN(distrFrequency());
	if(deviceType == GATENODE)	FRET(op_dist_uniform (GATEFREQNUM));			
	if(deviceType == ROUTERNODE)	FRET(op_dist_uniform (FREQNUM));
	FRET(0);
}

/*
	离网的资源释放和是否重新入网操作
*/

static void leaveNet(int NWKAddress,int rejoin,int children,int assign)
{
	int i;
	int GateIndex, EndIndex, routerIndex, ESNAddress;
	int band;
	FIN(leaveNet(int NWKAddress,int rejoin,int children,int assign));
	
	if(NWKAddress == myNWKAddress)//本节点离网操作
		{
		if(rejoin == 1)	joinType = 0;
		else if(rejoin == 2) joinType = 1;
		op_intrpt_schedule_self(op_sim_time(),intrCode_ReJoin);
		}
	else
		{
		GateIndex = (NWKAddress>>15);//网关域
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
		ESNAddress = quiryNWKAddressFromMap(NWKAddress);
		DeleteCT(2, NWKAddress);
		if(EndIndex > 0)
		{
			if(nodeType == GATENODE)
			{
				printf("ENDAddress = %d\n",mapping[ESNAddress].shortAddress);
				mapping[ESNAddress].shortAddress = 0;
				mapping[ESNAddress].count = -1;
				endAddressFlag[EndIndex] = 0;
			}
		}
		else if(routerIndex > 0)
		{
			deleteRouting(-1,NWKAddress);
			if(nodeType == GATENODE)
			{
				band = bandQuire(NWKAddress);
				if(band>=0) bandTable[band] = 0;
				mapping[ESNAddress].shortAddress = 0;
				mapping[ESNAddress].count = -1;
				printf("band = %d,ESNAddress = %d,router = %d\n",band,ESNAddress,routerAddressFlag[routerIndex - 1]);
				routerAddressFlag[routerIndex - 1] = 0;
				if(children == 1)
				{
					for(i = 0;i < MAX_NODE;i++)
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
	记录序列号到同步队列
*/
static void synchronizeQueue(int status, int rstart, int rend,int cstart, int cend)
{
	int seq, index;
	FIN(synchronizeQueue(int status, int rstart, int rend,int cstart, int cend));
	index = syncQ.number;
	if(index >= QUEUE_LEN)
		{
		printf("Node %d synchronizeQueue full\n", myESNAddress);
		FOUT;
		}
	seq = syncQ.table[index - 1].sequence + 1;
	syncQ.table[index].sequence = seq;
	syncQ.table[index].status = status;
	syncQ.table[index].routerStart = rstart;
	syncQ.table[index].routerEnd = rend;
	syncQ.table[index].childStart = cstart;
	syncQ.table[index].childEnd = cend;
	FOUT;
}

/*
	按照同步队列中的记录发送同步信息
*/
static void synchronizeSend()
{
	int num, status, rstart, rend, cstart, cend;
	int rnum, cnum;
	int NWKAddress;
	FIN(synchronizeSend());
	num = syncQ.number;
	if(num <= 0)
		{
		printf("Node %d does not have synchronize to send\n", myESNAddress);
		}
	status = syncQ.table[0].status;
	rstart = syncQ.table[0].routerStart;
	rend = syncQ.table[0].routerEnd;
	cstart = syncQ.table[0].childStart;
	cend = syncQ.table[0].childEnd;
	if(status == 0)
		{
		printf("initBackup\n");
		}
	else if(status == 1 || status == 2)
		{
		rnum = rend - rstart;
		cnum = cend - cstart;
		if(rnum <= 1 && cnum == 1)
			{
			NWKAddress = Network_Msg.NetworkCT[myESNAddress].table[cstart].NWKAddress;
			if(rnum ==1 && NWKAddress != RT.table[rstart].destAddress)
				{
				printf("Node %d synchronizeSend error for CT and RT NWKAddress defferent!\n", myESNAddress);
				}
			tableBackup(2,status - 1,NWKAddress,-1,1);
			}
		else if(rnum >= 1 && cnum >= 1)
			{
			tableBackup(0,status - 1,-1,rstart,rnum);
			tableBackup(1,status - 1,-1,cstart,cnum);
			}
		else if(rnum > 1 && cnum == 0)
			{
			tableBackup(0,status - 1,-1,rstart,rnum);
			}
		else if(rnum == 0 && cnum > 1)
			{
			tableBackup(1,status - 1,-1,cstart,cnum);
			}
		}
	FOUT;
}



/*
	主路由新接入备份路由时的本地信息发送
*/

static void initBackup()
{
	int i;
	int step = 0;
	int number;
	FIN(initBackup());
	printf("initBackup\n");
	synchronizeQueue(0, -1, -1, -1, -1);
	number = RT.number;
	for(i = 0;i<number;i += step)
	{
		step = (number - i) > MAXRTInfo ? MAXRTInfo : (number - i);
		synchronizeQueue(1, 0, step, -1, -1);
		//tableBackup(0,0,0,i,step);
	}
	step = 0;
	number = Network_Msg.NetworkCT[myESNAddress].number;
	for(i = 0;i<number;i += step)
	{
		step = (number - i) > MAXCTInfo ? MAXCTInfo : (number - i);
		synchronizeQueue(1, -1, -1, 0, step);
		//tableBackup(1,0,0,i,step);
	}
	FOUT;
}

/*
	主路由本地表项变化时的本地信息发送
*/
static void tableBackup(int type,int addOrReduce,int NWKAddress,int start,int num)
{
	Packet* payLoad;
	int i;
	int index = 0;
	int len = 1;
	int pklen;
	int entry_len;
	int num_max;
	int number;
	
	int infoControl = 0;
	int band;
	
	int RTIndex = -1;
	int CTIndex = -1;
	FIN(tableBackup(int type,int addOrReduce,int NWKAddress,int start,int num));
	switch(type)
	{
	case 0://RT Backup
			entry_len = 5;
			num = num > (RT.number - start)?(RT.number - start):num;
			num_max = num > MAXRTInfo ? MAXRTInfo:num;
			len += num_max * entry_len;
			infoControl |= (type<<5)|(addOrReduce<<4)|(num_max);
			payLoad = op_pk_create (len*8);
			op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,infoControl,8);
			for(i = 0;i<num_max;i++)
			{
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,RT.table[start].destAddress,8);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,RT.table[start].nextHop,16);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,RT.table[start].status,4);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,RT.table[start].routerSeq,4);
				start++;
			}
			pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + SHORTADDR_LEN + LENGTH_LEN;
			pklen += 8*len;
			generatePk(pklen,0,1,2,2,myNWKAddress,backupRouter,13,0,len/8,payLoad,To_children,6);
			printf("Node %d send a RT backup num = %d\n",myESNAddress,num_max);
			break;
	case 1://CT Backup
			entry_len = 9;
			num = num > (Network_Msg.NetworkCT[myESNAddress].number - start)?(Network_Msg.NetworkCT[myESNAddress].number - start):num;
			num_max = num > MAXCTInfo ? MAXCTInfo:num;
			number = num_max;
			for(i = 0;i<num_max;i++)
			{
				if((Network_Msg.NetworkCT[myESNAddress].table[start + i].CapabilityInformation>>6) == 3)
				{
					number -= 1;
					break;
				}
			}
			if(number == 0)	break;
			len += number * entry_len;
			infoControl |= (type<<5)|(addOrReduce<<4)|(number);
			payLoad = op_pk_create (len*8);
			op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,infoControl,8);
			for(i = 0;i<num_max;i++)
			{
				if((Network_Msg.NetworkCT[myESNAddress].table[start + i].CapabilityInformation>>6) == 3)
				{
					start++;
					continue;
				}
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,Network_Msg.NetworkCT[myESNAddress].table[start].ESNAddress,48);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,Network_Msg.NetworkCT[myESNAddress].table[start].NWKAddress,16);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,Network_Msg.NetworkCT[myESNAddress].table[start].CapabilityInformation,8);
				start++;
			}
			pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + SHORTADDR_LEN + LENGTH_LEN;
			pklen += 8*len;
			generatePk(pklen,0,1,2,2,myNWKAddress,backupRouter,13,0,len/8,payLoad,To_children,6);
			printf("Node %d send a CT backup num = %d\n",myESNAddress,num_max);
			break;
	case 2://RT and CT Backup
			entry_len = 0;
			for(i = 0;i<RT.number;i++)
			{
				if(RT.table[i].destAddress == NWKAddress)
				{
					RTIndex = i;
					entry_len += 40;
					break;
				}
			}
			for(i = 0;i<Network_Msg.NetworkCT[myESNAddress].number;i++)
			{
				if(Network_Msg.NetworkCT[myESNAddress].table[i].NWKAddress == NWKAddress)
				{
					CTIndex = i;
					entry_len += 72;
					break;
				}
			}
			len += entry_len;
			infoControl |= (type<<5)|(addOrReduce<<4)|1;
			payLoad = op_pk_create (len*8);
			op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,infoControl,8);
			if(RTIndex >= 0)
			{
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,RT.table[RTIndex].destAddress,16);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,RT.table[RTIndex].nextHop,16);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,RT.table[RTIndex].status,4);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,RT.table[RTIndex].routerSeq,4);
			}
			if(CTIndex >= 0)
			{
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,Network_Msg.NetworkCT[myESNAddress].table[CTIndex].ESNAddress,48);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,Network_Msg.NetworkCT[myESNAddress].table[CTIndex].NWKAddress,16);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,Network_Msg.NetworkCT[myESNAddress].table[CTIndex].CapabilityInformation,8);
			}
			pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + SHORTADDR_LEN + LENGTH_LEN;
			pklen += 8*len;
			generatePk(pklen,0,1,2,2,myNWKAddress,backupRouter,13,0,len/8,payLoad,To_children,0);
			printf("Node %d send a CRT backup\n",myESNAddress);
			break;
	case 3://mainRouter's Info
			entry_len = 3;
			num_max = 1;
			len += num_max * entry_len;
			infoControl |= (type<<5)|(addOrReduce<<4)|(num_max);
			band = queryFT(father);
			payLoad = op_pk_create (len*8);
			op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,infoControl,8);
			op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,father,16);
			op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,band,4);
			op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,myFrequency,4);
			pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + SHORTADDR_LEN + LENGTH_LEN;
			pklen += 8*len;
			generatePk(pklen,0,1,2,2,myNWKAddress,backupRouter,13,0,len/8,payLoad,To_children,6);
			printf("Node %d send a Info backup num = %d\n",myESNAddress,num_max);
			break;
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
	void WSN_NWK_Test (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_WSN_NWK_Test_init (int * init_block_ptr);
	void _op_WSN_NWK_Test_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_WSN_NWK_Test_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_WSN_NWK_Test_alloc (VosT_Obtype, int);
	void _op_WSN_NWK_Test_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
WSN_NWK_Test (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (WSN_NWK_Test ());

		{
		/* Temporary Variables */
		int i = 0;
		/* End of Temporary Variables */


		FSM_ENTER ("WSN_NWK_Test")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (INIT) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "INIT", "WSN_NWK_Test [INIT enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK_Test [INIT enter execs]", state0_enter_exec)
				{
				
				//-------------------------------My Parameters Declaration-------------------------------//
				nodeID = op_topo_parent(op_id_self());
				op_ima_obj_attr_get(nodeID,"MACAddress",&myESNAddress);
				op_ima_obj_attr_get(nodeID,"NodeType",&nodeType);
				op_ima_obj_attr_get(nodeID,"father",&father);
				op_ima_obj_attr_get(nodeID,"MainRouter",&mainRouter);
				
				printf("Node %d start!\n", myESNAddress);
				
				joinType = 0;
				capability = 0;
				backupRouter = 0;
				if(nodeType != BACKUPNODE)	capability |= nodeType<<4;
				else						capability |= ENDNODE<<4;
				myNWKAddress = 0;
				result = 0;
				
				
				if(nodeType == GATENODE )
					{
					myNWKAddress = GateAddress;
					initgate();
					}
				else if(nodeType == ROUTERNODE)
				{
					initrouter();
				}
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** state (INIT) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "INIT", "WSN_NWK_Test [INIT exit execs]")


			/** state (INIT) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_NWK_Test [INIT trans conditions]", state0_trans_conds)
			FSM_INIT_COND (nodeType == GATENODE)
			FSM_TEST_COND (nodeType != GATENODE)
			FSM_TEST_LOGIC ("INIT")
			FSM_PROFILE_SECTION_OUT (state0_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 2, state2_enter_exec, online();, "nodeType == GATENODE", "online()", "INIT", "Online", "tr_53", "WSN_NWK_Test [INIT -> Online : nodeType == GATENODE / online()]")
				FSM_CASE_TRANSIT (1, 1, state1_enter_exec, join();, "nodeType != GATENODE", "join()", "INIT", "WAN", "tr_62", "WSN_NWK_Test [INIT -> WAN : nodeType != GATENODE / join()]")
				}
				/*---------------------------------------------------------*/



			/** state (WAN) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "WAN", state1_enter_exec, "WSN_NWK_Test [WAN enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"WSN_NWK_Test")


			/** state (WAN) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "WAN", "WSN_NWK_Test [WAN exit execs]")


			/** state (WAN) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_NWK_Test [WAN trans conditions]", state1_trans_conds)
			FSM_INIT_COND (JoinTimer)
			FSM_TEST_COND (STRM_MCPS)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("WAN")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, join();, "JoinTimer", "join()", "WAN", "WAN", "tr_63", "WSN_NWK_Test [WAN -> WAN : JoinTimer / join()]")
				FSM_CASE_TRANSIT (1, 8, state8_enter_exec, ;, "STRM_MCPS", "", "WAN", "From_MCPS_Join", "tr_64", "WSN_NWK_Test [WAN -> From_MCPS_Join : STRM_MCPS / ]")
				FSM_CASE_TRANSIT (2, 1, state1_enter_exec, ;, "default", "", "WAN", "WAN", "tr_69", "WSN_NWK_Test [WAN -> WAN : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (Online) enter executives **/
			FSM_STATE_ENTER_UNFORCED (2, "Online", state2_enter_exec, "WSN_NWK_Test [Online enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (5,"WSN_NWK_Test")


			/** state (Online) exit executives **/
			FSM_STATE_EXIT_UNFORCED (2, "Online", "WSN_NWK_Test [Online exit execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK_Test [Online exit execs]", state2_exit_exec)
				{
				//if(REJOIN)	printf("1\n");
				//if(STRM_DATA)	printf("2\n");
				//if(STRM_CON)	printf("3\n");
				//if(STRM_MLME)	printf("4\n");
				//if(UpTimerCount)	printf("5\n");
				//if(JoinTimer)	printf("6\n");
				}
				FSM_PROFILE_SECTION_OUT (state2_exit_exec)


			/** state (Online) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_NWK_Test [Online trans conditions]", state2_trans_conds)
			FSM_INIT_COND (STRM_CON)
			FSM_TEST_COND (STRM_DATA)
			FSM_TEST_COND (UpTimerCount)
			FSM_TEST_COND (STRM_MCPS)
			FSM_TEST_COND (STRM_MLME)
			FSM_TEST_COND (REJOIN)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("Online")
			FSM_PROFILE_SECTION_OUT (state2_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 4, state4_enter_exec, ;, "STRM_CON", "", "Online", "From_Control", "tr_38", "WSN_NWK_Test [Online -> From_Control : STRM_CON / ]")
				FSM_CASE_TRANSIT (1, 3, state3_enter_exec, ;, "STRM_DATA", "", "Online", "From_Data", "tr_40", "WSN_NWK_Test [Online -> From_Data : STRM_DATA / ]")
				FSM_CASE_TRANSIT (2, 7, state7_enter_exec, ;, "UpTimerCount", "", "Online", "TimerCount", "tr_42", "WSN_NWK_Test [Online -> TimerCount : UpTimerCount / ]")
				FSM_CASE_TRANSIT (3, 5, state5_enter_exec, ;, "STRM_MCPS", "", "Online", "From_MCPS", "tr_44", "WSN_NWK_Test [Online -> From_MCPS : STRM_MCPS / ]")
				FSM_CASE_TRANSIT (4, 6, state6_enter_exec, ;, "STRM_MLME", "", "Online", "From_MLME", "tr_46", "WSN_NWK_Test [Online -> From_MLME : STRM_MLME / ]")
				FSM_CASE_TRANSIT (5, 1, state1_enter_exec, join();, "REJOIN", "join()", "Online", "WAN", "tr_71", "WSN_NWK_Test [Online -> WAN : REJOIN / join()]")
				FSM_CASE_TRANSIT (6, 2, state2_enter_exec, ;, "default", "", "Online", "Online", "tr_60", "WSN_NWK_Test [Online -> Online : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (From_Data) enter executives **/
			FSM_STATE_ENTER_FORCED (3, "From_Data", state3_enter_exec, "WSN_NWK_Test [From_Data enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK_Test [From_Data enter execs]", state3_enter_exec)
				{
				int type;
				int pklen = 0;
				int sourceMode;
				int destMode;
				int len;
				int dest;
				int NWKSeq = 0;
				long long data;
				Packet* payLoad;
				
				Packet* packet = op_pk_get (op_intrpt_strm());
				op_pk_fd_get(packet,0,&len);
				op_pk_fd_get(packet,1,&type);
				op_pk_fd_get(packet,2,&dest);
				op_pk_fd_get(packet,3,&data);
				payLoad = op_pk_create(len*8);
				op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,data,len*8);
				pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LENGTH_LEN;
				pklen += len*8;
				if(type == 7 && nodeType != GATENODE && nodeType != BACKUPNODE)
				{
					//printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Node %d get a upData node %d from data module!!!!!!!!!!!!!!!!!!",myMACAddress,dest);
					dest = GateAddress;
					destMode = 1;
					sourceMode = 2;
					generatePk(pklen,0,1,sourceMode,destMode,myNWKAddress,dest,type,MAXTTL-1,len,payLoad,To_father,0);
					//op_stat_write(up_data_send_num,++upSendNum);
				}
				else if(nodeType == GATENODE)
				{
					dest = quiryNWKAddress(dest);
					//printf("NWKAddress %d !!!!!!!!!!!!!!!!!!\n",dest);
					if(dest != 0)
					{
						destMode = 2;
						sourceMode = 1;
						generatePk(pklen,0,1,sourceMode,destMode,myNWKAddress,dest,type,MAXTTL-1,len,payLoad,To_children,0);
						//op_stat_write(down_data_send_num,++downSendNum);
					}
				}
				
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** state (From_Data) exit executives **/
			FSM_STATE_EXIT_FORCED (3, "From_Data", "WSN_NWK_Test [From_Data exit execs]")


			/** state (From_Data) transition processing **/
			FSM_TRANSIT_FORCE (2, state2_enter_exec, ;, "default", "", "From_Data", "Online", "tr_41", "WSN_NWK_Test [From_Data -> Online : default / ]")
				/*---------------------------------------------------------*/



			/** state (From_Control) enter executives **/
			FSM_STATE_ENTER_FORCED (4, "From_Control", state4_enter_exec, "WSN_NWK_Test [From_Control enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK_Test [From_Control enter execs]", state4_enter_exec)
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
				//printf("get a control from control\n");
				op_pk_fd_get(packet,0,&len);
				op_pk_fd_get(packet,1,&type);
				op_pk_fd_get(packet,2,&dest);
				//payLoad = op_pk_create(len);
				op_pk_fd_get(packet,3,&payLoad);
				pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + len;
				printf("Node %d received a control %d\n", myESNAddress, type);
				switch(type)
				{
					case 1: break;
					case 2: dest = quiryNWKAddress(dest);
							if(dest == 0)	break;
							printf("Node %d send a leaveContr,dest = %d !!!\n",myESNAddress,dest);
							if(nodeType == GATENODE)	sourceMode = 1;
							else						sourceMode = 2;
							destMode = 2;
							source = myNWKAddress;
							pklen += SHORTADDR_LEN * 2;
							op_pk_fd_get(payLoad,0,&removeChildren);
							removeChildren &= 1;
							generatePk(pklen,0,1,sourceMode,destMode,source,dest,type,MAXTTL,len,payLoad,To_children,0);
							leaveNet(dest,1,removeChildren,0);
							//op_stat_write(contr_send_num,++sendNum);
							break;
					case 3: sourceMode = 2;
							destMode = 1;
							source = myNWKAddress;
							pklen += SHORTADDR_LEN;
							generatePk(pklen,0,1,sourceMode,destMode,source,dest,type,MAXTTL,len,payLoad,To_father,0);
							//op_stat_write(contr_send_num,++sendNum);
							break;
					case 4: sourceMode = 2;
							destMode = 2;
							source = myNWKAddress;
							pklen += SHORTADDR_LEN * 2;
							generatePk(pklen,0,1,sourceMode,destMode,source,dest,type,MAXTTL,len,payLoad,To_father,0);
							//op_stat_write(contr_send_num,++sendNum);
							break;
					case 5: sourceMode = 1;
							destMode = 2;
							source = myNWKAddress;
							pklen += SHORTADDR_LEN;
							printf("!!!!!!\n");
							generatePk(pklen,0,1,sourceMode,destMode,source,dest,type,MAXTTL-1,len,payLoad,To_children,0);
							//op_stat_write(contr_send_num,++sendNum);
							break;
					case 6: sourceMode = 2;
							destMode = 3;
							source = myNWKAddress;
							pklen += SHORTADDR_LEN + LONGADDR_LEN;
							generatePk(pklen,0,1,sourceMode,destMode,source,dest,type,MAXTTL,len,payLoad,To_children,0);
							//op_stat_write(contr_send_num,++sendNum);
							break;
					case 9: break;//No address conflict
					case 10:sourceMode = 2;
							destMode = 1;
							source = myNWKAddress;
							pklen += SHORTADDR_LEN;
							generatePk(pklen,0,1,sourceMode,destMode,source,dest,type,MAXTTL-1,len,payLoad,To_father,0);
							//op_stat_write(contr_send_num,++sendNum);
							break;
					case 11:sourceMode = 2;
							destMode = 1;
							source = myNWKAddress;
							pklen += SHORTADDR_LEN;
							generatePk(pklen,0,1,sourceMode,destMode,source,dest,type,MAXTTL,len,payLoad,To_father,0);
							//op_stat_write(contr_send_num,++sendNum);
							break;
					case 12:sourceMode = 2;
							destMode = 1;
							source = myNWKAddress;
							pklen += SHORTADDR_LEN;
							generatePk(pklen,0,1,sourceMode,destMode,source,dest,type,MAXTTL,len,payLoad,To_father,0);
							//op_stat_write(contr_send_num,++sendNum);
							break;
					case 13:sourceMode = 1;
							destMode = 2;
							source = myNWKAddress;
							pklen += SHORTADDR_LEN;
							generatePk(pklen,0,1,sourceMode,destMode,source,dest,type,MAXTTL,len,payLoad,To_children,0);
							//op_stat_write(contr_send_num,++sendNum);
							break;
					case 14:sourceMode = 2;
							destMode = 1;
							source = myNWKAddress;
							pklen += SHORTADDR_LEN;
							generatePk(pklen,0,1,sourceMode,destMode,source,dest,type,MAXTTL,len,payLoad,To_father,0);
							//op_stat_write(contr_send_num,++sendNum);
							break;
					default:op_pk_destroy(packet);
							printf("Contol_PacketType is invalid\n");
				}
				}
				FSM_PROFILE_SECTION_OUT (state4_enter_exec)

			/** state (From_Control) exit executives **/
			FSM_STATE_EXIT_FORCED (4, "From_Control", "WSN_NWK_Test [From_Control exit execs]")


			/** state (From_Control) transition processing **/
			FSM_TRANSIT_FORCE (2, state2_enter_exec, ;, "default", "", "From_Control", "Online", "tr_39", "WSN_NWK_Test [From_Control -> Online : default / ]")
				/*---------------------------------------------------------*/



			/** state (From_MCPS) enter executives **/
			FSM_STATE_ENTER_FORCED (5, "From_MCPS", state5_enter_exec, "WSN_NWK_Test [From_MCPS enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK_Test [From_MCPS enter execs]", state5_enter_exec)
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
				
				Packet* payload;
				Packet* packet = op_pk_get (op_intrpt_strm());
				
				op_pk_fd_get(packet,0,&framControl);
				op_pk_fd_get(packet,5,&length);
				op_pk_fd_get(packet,7,&NWKControl);
				op_pk_fd_get(packet,8,&payload);
				
				iepresent = 1&(framControl>>6);
				multicast = 1&(framControl>>8);
				type = 15 & (NWKControl>>12);
				destMode = 7 & (framControl>>4);
				sourceMode = 7 & framControl;
				if(destMode != 1)	op_pk_fd_get(packet,3,&dest);
				else				dest = GateAddress;
				if(sourceMode != 1)	op_pk_fd_get(packet,4,&source);
				else				source = GateAddress;
				
				len = length - 2;
				
				
				printf("Node %d received a pk from mcps, type = %d, sourceMode = %d, destMode = %d, source = %d, dest = %d!\n", myESNAddress, type, sourceMode, destMode, source, dest);
				
				//上行单播数据接收处理
				if(nodeType == GATENODE || type == 0 || type == 0)
					{
				
					//更新路由
					if(sourceMode == 2)		UpdateRT(source, RT_Active);
					
					if(iepresent == 1)
						{
						op_pk_fd_get(packet,6,&ies);
						//组播维护
						maintainCMT(ies);
						len -= 3;
						}
					if(dest != myNWKAddress)
						{
						if(nodeType != GATENODE)
							{
							//处理
							switch(type)
								{
								case 3: 
										break;
								case 11:RoutingPro(payload, destMode, dest, sourceMode, source, len);
										break;
								}
						
							//转发
							op_pk_fd_set (packet,8,OPC_FIELD_TYPE_PACKET,payload,len * 8);
							forward(packet,To_father);
							}
						}
					else
						{
						printf("Node %d received a pk from mcps, type = %d, source = %d, dest = %d!\n", myESNAddress, type, source, dest);
						//接收处理
						switch(type)
							{
							case 0: JoinReqPro(payload, destMode, dest, sourceMode, source, len);
									break;
							case 2: leavePro(payload, destMode, dest, sourceMode, source, len);
									break;
							case 3: switchPro(payload, destMode, dest, sourceMode, source, len);
									break;
							case 4: backupPro(payload, destMode, dest, sourceMode, source, len);
									break;
							case 7: upDataPro(payload, destMode, dest, sourceMode, source, len);
							//printf("Receive a data\n");
									break;
							case 9: break;//No address conflict
							case 10:nwkStatuspro(payload, destMode, dest, sourceMode, source, len);
									break;
							case 11:RoutingPro(payload, destMode, dest, sourceMode, source, len);
									break;
							case 12:mainRouterPro(payload, destMode, dest, sourceMode, source, len);
									break;
							default:printf("NWK_PacketType is invalid\n");
							}
						op_pk_destroy(packet);
						op_pk_destroy(payload);
						}
					}
				//下行单播数据接收处理
				else if(nodeType != GATENODE)
					{
					if(destMode == 2 && dest == myNWKAddress)
						{
						printf("Node %d received a pk from mcps, type = %d\n", myESNAddress, type);
						//接收处理
						switch(type)
							{
							case 1: accessConfirmPro(payload, destMode, dest, sourceMode, source, len);
									break;
							case 2: leavePro(payload, destMode, dest, sourceMode, source, len);
									break;
							case 4: backupPro(payload, destMode, dest, sourceMode, source, len);
									break;
							case 5:	multicastContrPro(payload, destMode, dest, sourceMode, source, len);
									break;
							case 6: routingControl(payload, destMode, dest, sourceMode, source, len);
									break;
							case 8: downDataPro(payload, destMode, dest, sourceMode, source, len);
									break;
							case 9: break;//No address conflict
							case 11:RoutingPro(payload, destMode, dest, sourceMode, source, len);
									break;
							case 12:mainRouterPro(payload, destMode, dest, sourceMode, source, len);
									break;
							case 13:backupPro(payload, destMode, dest, sourceMode, source, len);
									break;
							case 14:nodeRecover(payload, destMode, dest, sourceMode, source, len);
									break;
							default:printf("NWK_PacketType is invalid\n");
							}
						op_pk_destroy(packet);
						}
					else if(destMode == 2 && dest != myNWKAddress)
						{
						if(nodeType != GATENODE)
							{
							next = nextHop(dest);
							if(next < 0 && type == 11)
								{
								next = nextHopMaint(dest);
								}
							if(next < 0)
								{
								op_pk_destroy(packet);
								}
							else
								{
								//处理
								switch(type)
									{
									case 1: accessConfirmPro(payload, destMode, dest, sourceMode, source, len);
											break;
									case 2: leavePro(payload, destMode, dest, sourceMode, source, len);
											break;
									case 5:	multicastContrPro(payload, destMode, dest, sourceMode, source, len);
											break;
									case 6: routingControl(payload, destMode, dest, sourceMode, source, len);
											break;
									case 9: break;//No address conflict
									//case 12:mainRouterPro(payload, destMode, dest, sourceMode, source, len);
											//break;
									case 14:nodeRecover(payload, destMode, dest, sourceMode, source, len);
											break;
									}
								//转发
								printf("Node %d next = %d\n", myESNAddress, next);
								op_pk_fd_set (packet,8,OPC_FIELD_TYPE_PACKET,payload,len * 8);
								forward(packet,To_children);
								}
							}
						}
					else if(destMode == 3 && dest == myESNAddress)
						{
						if(type == 1)
							{
							accessConfirmPro(payload, destMode, dest, sourceMode, source, len);
							}
						op_pk_destroy(packet);
						op_pk_destroy(payload);
						}
					else
						{
						printf("Node %d 接收到目的节点非本节点的数据包!\n", myESNAddress);
						}
					}
				//下行组播数据接收处理
				else if(1 == 0)
					{
					if(((myMT>>dest)&1)>0)
						{
							downDataPro(payload, destMode, dest, sourceMode, source, len);
							printf("----------------------------Node %d receive a multicast down data!  ----",myESNAddress);
							printf("len = %d-------------------\n",len);
						}
					if(nodeType == ROUTERNODE && ((childMT.group>>dest)&1) > 0)
						{
							op_pk_fd_set (packet,6,OPC_FIELD_TYPE_PACKET,payload,len);
							forward(packet,To_children);
						}
					else
						{
						op_pk_destroy(packet);
						op_pk_destroy(payload);
						}
					}
				
				}
				FSM_PROFILE_SECTION_OUT (state5_enter_exec)

			/** state (From_MCPS) exit executives **/
			FSM_STATE_EXIT_FORCED (5, "From_MCPS", "WSN_NWK_Test [From_MCPS exit execs]")


			/** state (From_MCPS) transition processing **/
			FSM_TRANSIT_FORCE (2, state2_enter_exec, ;, "default", "", "From_MCPS", "Online", "tr_45", "WSN_NWK_Test [From_MCPS -> Online : default / ]")
				/*---------------------------------------------------------*/



			/** state (From_MLME) enter executives **/
			FSM_STATE_ENTER_FORCED (6, "From_MLME", state6_enter_exec, "WSN_NWK_Test [From_MLME enter execs]")

			/** state (From_MLME) exit executives **/
			FSM_STATE_EXIT_FORCED (6, "From_MLME", "WSN_NWK_Test [From_MLME exit execs]")


			/** state (From_MLME) transition processing **/
			FSM_TRANSIT_FORCE (2, state2_enter_exec, ;, "default", "", "From_MLME", "Online", "tr_47", "WSN_NWK_Test [From_MLME -> Online : default / ]")
				/*---------------------------------------------------------*/



			/** state (TimerCount) enter executives **/
			FSM_STATE_ENTER_FORCED (7, "TimerCount", state7_enter_exec, "WSN_NWK_Test [TimerCount enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK_Test [TimerCount enter execs]", state7_enter_exec)
				{
				switch(op_intrpt_code())
					{
					//计数器递减及处理
					case intrCode_Timer:	op_intrpt_schedule_self(op_sim_time() + Timer,intrCode_Timer);
							UpdateRT(-1,-1);
							break;
					//寻找主路由
					case intrCode_ChangeMainRouter:		
							printf("///////////\n");
							mainRouterSnd();
							break;
					default :printf("error intrpt_code %d!\n", op_intrpt_code());
					}
				}
				FSM_PROFILE_SECTION_OUT (state7_enter_exec)

			/** state (TimerCount) exit executives **/
			FSM_STATE_EXIT_FORCED (7, "TimerCount", "WSN_NWK_Test [TimerCount exit execs]")


			/** state (TimerCount) transition processing **/
			FSM_TRANSIT_FORCE (2, state2_enter_exec, ;, "default", "", "TimerCount", "Online", "tr_43", "WSN_NWK_Test [TimerCount -> Online : default / ]")
				/*---------------------------------------------------------*/



			/** state (From_MCPS_Join) enter executives **/
			FSM_STATE_ENTER_FORCED (8, "From_MCPS_Join", state8_enter_exec, "WSN_NWK_Test [From_MCPS_Join enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK_Test [From_MCPS_Join enter execs]", state8_enter_exec)
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
				int next;
				Packet* payload;
				Packet* packet;
				
				result = 0;
				if(STRM_MCPS)
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
					if(destMode == 3 && dest == myESNAddress && type == 1)
						{
						accessConfirmPro(payload, destMode, dest, sourceMode, source, len);
						printf("Node %d received a accessConfirm pk in join process!\n", myESNAddress);
						result = 1;
						}
					else
						{
						printf("Node %d received a not accessConfirm pk in join process!\n", myESNAddress);
						}
					}
				}
				FSM_PROFILE_SECTION_OUT (state8_enter_exec)

			/** state (From_MCPS_Join) exit executives **/
			FSM_STATE_EXIT_FORCED (8, "From_MCPS_Join", "WSN_NWK_Test [From_MCPS_Join exit execs]")


			/** state (From_MCPS_Join) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_NWK_Test [From_MCPS_Join trans conditions]", state8_trans_conds)
			FSM_INIT_COND (FAILT)
			FSM_TEST_COND (SUCCESS)
			FSM_TEST_LOGIC ("From_MCPS_Join")
			FSM_PROFILE_SECTION_OUT (state8_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, ;, "FAILT", "", "From_MCPS_Join", "WAN", "tr_65", "WSN_NWK_Test [From_MCPS_Join -> WAN : FAILT / ]")
				FSM_CASE_TRANSIT (1, 2, state2_enter_exec, online();, "SUCCESS", "online()", "From_MCPS_Join", "Online", "tr_66", "WSN_NWK_Test [From_MCPS_Join -> Online : SUCCESS / online()]")
				}
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"WSN_NWK_Test")
		}
	}




void
_op_WSN_NWK_Test_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_WSN_NWK_Test_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_WSN_NWK_Test_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_WSN_NWK_Test_svar function. */
#undef result
#undef nodeID
#undef fatherTable
#undef father
#undef capability
#undef nodeType
#undef myESNAddress
#undef joinType
#undef CT
#undef RT
#undef myNWKAddress
#undef endAddressFlag
#undef startRT
#undef startCT
#undef myBand
#undef PANID
#undef backupRouter
#undef myFrequency
#undef myRouterSeq
#undef myMT
#undef childMT
#undef mainRouter
#undef mainNWKAddress
#undef mainBand
#undef mainFrequency
#undef mainFather
#undef syncQ

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_WSN_NWK_Test_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_WSN_NWK_Test_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (WSN_NWK_Test)",
		sizeof (WSN_NWK_Test_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_WSN_NWK_Test_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	WSN_NWK_Test_state * ptr;
	FIN_MT (_op_WSN_NWK_Test_alloc (obtype))

	ptr = (WSN_NWK_Test_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "WSN_NWK_Test [INIT enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_WSN_NWK_Test_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	WSN_NWK_Test_state		*prs_ptr;

	FIN_MT (_op_WSN_NWK_Test_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (WSN_NWK_Test_state *)gen_ptr;

	if (strcmp ("result" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->result);
		FOUT
		}
	if (strcmp ("nodeID" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->nodeID);
		FOUT
		}
	if (strcmp ("fatherTable" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->fatherTable);
		FOUT
		}
	if (strcmp ("father" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->father);
		FOUT
		}
	if (strcmp ("capability" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->capability);
		FOUT
		}
	if (strcmp ("nodeType" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->nodeType);
		FOUT
		}
	if (strcmp ("myESNAddress" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->myESNAddress);
		FOUT
		}
	if (strcmp ("joinType" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->joinType);
		FOUT
		}
	if (strcmp ("CT" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->CT);
		FOUT
		}
	if (strcmp ("RT" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->RT);
		FOUT
		}
	if (strcmp ("myNWKAddress" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->myNWKAddress);
		FOUT
		}
	if (strcmp ("endAddressFlag" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->endAddressFlag);
		FOUT
		}
	if (strcmp ("startRT" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->startRT);
		FOUT
		}
	if (strcmp ("startCT" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->startCT);
		FOUT
		}
	if (strcmp ("myBand" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->myBand);
		FOUT
		}
	if (strcmp ("PANID" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->PANID);
		FOUT
		}
	if (strcmp ("backupRouter" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->backupRouter);
		FOUT
		}
	if (strcmp ("myFrequency" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->myFrequency);
		FOUT
		}
	if (strcmp ("myRouterSeq" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->myRouterSeq);
		FOUT
		}
	if (strcmp ("myMT" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->myMT);
		FOUT
		}
	if (strcmp ("childMT" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->childMT);
		FOUT
		}
	if (strcmp ("mainRouter" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->mainRouter);
		FOUT
		}
	if (strcmp ("mainNWKAddress" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->mainNWKAddress);
		FOUT
		}
	if (strcmp ("mainBand" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->mainBand);
		FOUT
		}
	if (strcmp ("mainFrequency" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->mainFrequency);
		FOUT
		}
	if (strcmp ("mainFather" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->mainFather);
		FOUT
		}
	if (strcmp ("syncQ" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->syncQ);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}


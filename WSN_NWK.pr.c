/* Process model C form file: WSN_NWK.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char WSN_NWK_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A modeler 7 619766AF 619766AF 1 DESKTOP-RD4S7T2 51133 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                    ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

//----------------------------------Node Type Define-------------------------------------//
#define NOTENDNODE		(nodeType != 2)
#define ROUTERNODE		(nodeType == 1)
#define GATENODE		(nodeType == 0)
#define ENDNODE			(nodeType == 2)
#define BACKUPNODE		(nodeType == 3)

//---------------------------------Node Status Define------------------------------------//
#define OfflineStatus				0
#define OnlineStatus				1
#define RejoinStatus				2
#define LeaveStatus					3
#define AssignStatus				4

//-----------------------------Timer Parameters Define-----------------------------------//
#define UPtime				5
#define RTtime				100
#define CTTimercount		50
#define BackupTimercount	10
#define RTTimercount		2
#define UPStatustime		20
#define DangerousCount		5

//----------------------------- Interrupt Code Define-----------------------------------//
#define intrCode_Timer				0
#define intrCode_KeepAlive			1
#define intrCode_ChildrenActive		5
#define intrCode_UpStatus			7
#define intrCode_UpRT				8

#define intrCode_FatherActive		3
#define intrCode_Status				2
#define intrCode_LeaveNet			6
#define intrCode_Backup				9
#define intrCode_BackupResponse		10

//---------------------------------Stream Port Define------------------------------------//
#define From_DATA		1
#define From_Contr		2
#define From_MAC		0
#define To_father		0
#define To_children		1


//----------------------------Upper Limit Parameters Define------------------------------//
#define MAXTTL				7
#define MAX_NODE			200
#define MAX_ROUTER			48
#define MAX_CHILD			50
#define MAX_RouterAddress	128
#define GATEPOINTNUM		4
#define POINTNUM			4
#define MAX_MULTICAST		20
#define MAXFT				6
#define MAXRTInfo			9
#define MAXCTInfo			5


//-----------------------------Transition Condition Define-------------------------------//
#define STRM_DATA	((op_intrpt_type() == OPC_INTRPT_STRM) &&(op_intrpt_strm() == From_DATA))
#define STRM_Contr	((op_intrpt_type() == OPC_INTRPT_STRM) &&(op_intrpt_strm() == From_Contr))
#define STRM_MAC	((op_intrpt_type() == OPC_INTRPT_STRM) &&(op_intrpt_strm() == From_MAC))
#define UpTimer		((op_intrpt_type() == OPC_INTRPT_SELF) &&(op_intrpt_code() == intrCode_Timer))
#define UPDATERT	((op_intrpt_type() == OPC_INTRPT_SELF) &&(op_intrpt_code() == intrCode_UpRT))
#define UpStatus	((op_intrpt_type() == OPC_INTRPT_SELF) &&(op_intrpt_code() == intrCode_UpStatus))
#define REMOTE_PRO	(op_intrpt_type() == OPC_INTRPT_REMOTE)


//---------------------------------Field Length Define-----------------------------------//
#define FRAMCONTR_LEN		16
#define MACSEQ_LEN			8
#define PANID_LEN			16
#define SHORTADDR_LEN		16
#define LONGADDR_LEN		48
#define NWKCONTR_LEN		24

//------------------------------Broadcast Address Define---------------------------------//
#define BroadCast			65535
#define RouterCast			65534
#define ChildrenCast		65533
#define ChildrenEndCast		65532


int mn = 0;

//----------------------------Network Parameters Declaration-----------------------------//
int PANID = 0;
int GateAddress = 43690;



//-----------------------------Static Parameters Declaration-----------------------------//
double downSendNum = 0;
double downReceiveNum = 0;
double upSendNum = 0;
double upReceiveNum = 0;

//----------------------------------Table Declaration------------------------------------//

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

struct Potential_Parent
{
	int short_addr;
	double band;
	int frequency;
	int router_cost;
	int depth;
	int times;
};

//--------------------------------Global Table Declaration------------------------------//
struct NetworkMsg Network_Msg;
extern struct Potential_Parent potentialParent[MAX_NODE][MAXFT];

//-------------------------------Init Function Declaration------------------------------//
static void CRTinit();

//------------------------------Forward Function Declaration----------------------------//
static void forward(Packet* pkptr,int streamPort);

//-----------------------------Generate Function Declaration----------------------------//
static void generatePk(int pklen,int multicast,int ar,int sourceMode,int destMode,int source,int dest,int type,int TTL,int NWKSeq,int NWKLen,Packet* PayLoad,int streamPort,double delay);
static void JoinReqPro(Packet* packet);
static void sourceSeqPro(Packet* packet);
static void accessConfirmPro(Packet* packet,int destMode);

//-------------------------------Data Function Declaration------------------------------//
static void upDataPro(Packet* packet);

//-------------------------------Table Function Declaration-----------------------------//
static void generateCT(int ESNAddress,int NWKAddress,int CapabilityInformation);
static void generateRT(int destAddress,int nextHop,int status,int routerSeq);
static void UpdateRT(int NWKAddress);
static void UpdateCT(int ESNAddress);
static void UpdateRouting(int control,int routerSeq,int NWKAddress);
static void multicastContrPro(Packet* packet);
static void leavePro(Packet* packet);
static void mainRouterPro(Packet* packet);
static void mainRouterQuery(Packet* packet);
static void mainRouterResponse(Packet* packet);
static void mainRouterStatusQuiry(Packet* packet);
static void mainRouterInfoPro(Packet* packet);

//------------------------------Inquire Function Declaration----------------------------//
static int nextHop(int dest);
static int queryRTIndex(int NWKAddress);
static int CTNWKAddresss(int ESNAddress);
static void UPCTStatus(int ESNAddress,int NWKAddress);
static int quiryNWKAddress(int MACAddress);
static int quiryRouterSeq(int NWKAddress);
static int CTinclude(int NWKAddress);
static int bandQuire(int NWKAddress);
static int quiryMT(int dest);
static int quiryChildMT(int dest);
static int quiryNWKAddressFromMap(int NWKAddress);
static int queryFT(int fatherAddress);

//-----------------------------Address Distribute Declaration---------------------------//
static int distAddress(int deviceType);
static int distBand(int NWKAddress);
static int distPoint();

//-----------------------------NodeStatus Process Declaration---------------------------//
static void leaveNet(int NWKAddress,int rejoin,int children,int assign);

//-----------------------------BackupInfo Function Declaration--------------------------//
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
	int	                    		node_status                                     ;
	int	                    		myCapabilityInformation                         ;
	int	                    		myNWKAddress                                    ;
	int	                    		myMACAddress                                    ;
	int	                    		myFatherAddress                                 ;
	int	                    		myBand                                          ;
	int	                    		myPoint                                         ;
	int	                    		NWK_Seq                                         ;
	int	                    		addressdist                                     ;
	struct CTable	          		CT                                              ;
	struct RTable	          		RT                                              ;
	int	                    		mappingTable[MAX_NODE]                          ;
	int	                    		EndAddressFlag[MAX_NODE]                        ;
	int	                    		RouterAddressFlag[MAX_RouterAddress]            ;
	Objid	                  		nodeID                                          ;
	int	                    		lastID                                          ;
	int	                    		bandTable[MAX_ROUTER]                           ;
	int	                    		myMT[MAX_MULTICAST]                             ;
	int	                    		childMT[MAX_MULTICAST]                          ;
	int	                    		nodeType                                        ;
	int	                    		Type                                            ;
	Stathandle	             		contr_send_num                                  ;
	int	                    		sendNum                                         ;
	Stathandle	             		down_data_send_num                              ;
	Stathandle	             		down_data_receive_num                           ;
	Stathandle	             		up_data_send_num                                ;
	Stathandle	             		up_data_receive_num                             ;
	Stathandle	             		down_pk_loss_rate                               ;
	Stathandle	             		up_pk_loss_rate                                 ;
	int	                    		mainRouter                                      ;
	Stathandle	             		ete_delay_down                                  ;
	Stathandle	             		ete_delay_up                                    ;
	int	                    		backupRouter                                    ;
	int	                    		mainNWKAddress                                  ;
	int	                    		mainFather                                      ;
	int	                    		mainBand                                        ;
	int	                    		mainPoint                                       ;
	int	                    		startRT                                         ;
	int	                    		startCT                                         ;
	int	                    		myRouterSeq                                     ;
	} WSN_NWK_state;

#define node_status             		op_sv_ptr->node_status
#define myCapabilityInformation 		op_sv_ptr->myCapabilityInformation
#define myNWKAddress            		op_sv_ptr->myNWKAddress
#define myMACAddress            		op_sv_ptr->myMACAddress
#define myFatherAddress         		op_sv_ptr->myFatherAddress
#define myBand                  		op_sv_ptr->myBand
#define myPoint                 		op_sv_ptr->myPoint
#define NWK_Seq                 		op_sv_ptr->NWK_Seq
#define addressdist             		op_sv_ptr->addressdist
#define CT                      		op_sv_ptr->CT
#define RT                      		op_sv_ptr->RT
#define mappingTable            		op_sv_ptr->mappingTable
#define EndAddressFlag          		op_sv_ptr->EndAddressFlag
#define RouterAddressFlag       		op_sv_ptr->RouterAddressFlag
#define nodeID                  		op_sv_ptr->nodeID
#define lastID                  		op_sv_ptr->lastID
#define bandTable               		op_sv_ptr->bandTable
#define myMT                    		op_sv_ptr->myMT
#define childMT                 		op_sv_ptr->childMT
#define nodeType                		op_sv_ptr->nodeType
#define Type                    		op_sv_ptr->Type
#define contr_send_num          		op_sv_ptr->contr_send_num
#define sendNum                 		op_sv_ptr->sendNum
#define down_data_send_num      		op_sv_ptr->down_data_send_num
#define down_data_receive_num   		op_sv_ptr->down_data_receive_num
#define up_data_send_num        		op_sv_ptr->up_data_send_num
#define up_data_receive_num     		op_sv_ptr->up_data_receive_num
#define down_pk_loss_rate       		op_sv_ptr->down_pk_loss_rate
#define up_pk_loss_rate         		op_sv_ptr->up_pk_loss_rate
#define mainRouter              		op_sv_ptr->mainRouter
#define ete_delay_down          		op_sv_ptr->ete_delay_down
#define ete_delay_up            		op_sv_ptr->ete_delay_up
#define backupRouter            		op_sv_ptr->backupRouter
#define mainNWKAddress          		op_sv_ptr->mainNWKAddress
#define mainFather              		op_sv_ptr->mainFather
#define mainBand                		op_sv_ptr->mainBand
#define mainPoint               		op_sv_ptr->mainPoint
#define startRT                 		op_sv_ptr->startRT
#define startCT                 		op_sv_ptr->startCT
#define myRouterSeq             		op_sv_ptr->myRouterSeq

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	WSN_NWK_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((WSN_NWK_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif

//----------------------------------------------------------------------------------------------------------------------------------
/*
	initiate function
*/
static void CRTinit()
{
	FIN(CRTinit());
	RT.number = 0;
	Network_Msg.NetworkCT[myMACAddress].number = 0;
	Network_Msg.NetworkRT[myMACAddress] = RT;
	startRT = 0;
	startCT = 0;
	FOUT;
}

//----------------------------------------------------------------------------------------------------------------------------------
/*
	forward a packet
*/
static void forward(Packet* pkptr,int streamPort)
{
	int NWKControl;
	int TTL;
	FIN(forward(Packet* pkptr));
	op_pk_fd_get(pkptr,5,&NWKControl);
	TTL = 15&(NWKControl>>16);
	NWKControl ^= (TTL<<16);
	TTL--;
	if(TTL<0)
	{
		op_pk_destroy(pkptr);
		//printf("PK was destroyed becouse of TTL down!\n");
		FOUT;
	}
	NWKControl |= (TTL<<16);
	op_pk_fd_set(pkptr,5,OPC_FIELD_TYPE_INTEGER,NWKControl,24);
	op_pk_send(pkptr,streamPort);
	op_stat_write(contr_send_num,++sendNum);
	printf("\n\nNode %d forward %f!\n\n",myMACAddress,op_sim_time());
	//lastID = 1;
	FOUT;
}

//----------------------------------------------------------------------------------------------------------------------------------
/*
	generate a packet!!!
*/
static void generatePk(int pklen,int multicast,int ar,int sourceMode,int destMode,int source,int dest,int type,int TTL,int NWKSeq,int NWKLen,Packet* PayLoad,int streamPort,double delay)
{
	Packet* pkptr;
	int frameControl = 10752|(multicast<<8)|(ar<<10);
	int NWKControl = 0;
	FIN(generateDataPk(int pklen,int sourceMode,int destMode,int source,int dest,int type,int TTL,int NWKSeq,int NWKLen,long long PayLoad));
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
	//set NWKControl,index = 5
	NWKControl |= (type<<20);
	NWKControl |= (TTL<<16);
	NWKControl |= (NWKSeq<<8);
	NWKControl |= NWKLen;
	op_pk_fd_set (pkptr,5,OPC_FIELD_TYPE_INTEGER,NWKControl,NWKCONTR_LEN);
	//set NWKPayLoad,index = 6
	op_pk_fd_set (pkptr,6,OPC_FIELD_TYPE_PACKET,PayLoad,NWKLen*8);
	op_pk_send_delayed(pkptr,streamPort,delay);
	//lastID = 1;
	FOUT;
}

//----------------------------------------------------------------------------------------------------------------------------------
/*
	processing all types of control packet!!!
*/

//Processing JoinReq
static void JoinReqPro(Packet* packet)
{
	Packet* payLoad;
	int ESNAddress;
	int dest;
	int source;
	int CapabilityInformation;
	int pklen;
	int deviceType;
	
	int NWKAddress = 0;
	int band;
	int point;
	FIN(JoinReqPro(Packet* packet));
	op_pk_fd_get(packet,3,&dest);
	op_pk_fd_get(packet,4,&source);
	op_pk_fd_get(packet,6,&payLoad);
	op_pk_fd_get(payLoad,0,&ESNAddress);
	op_pk_fd_get(payLoad,1,&CapabilityInformation);
	//printf("dest = %d,myNWKAddress = %d\n",dest,myNWKAddress);
	if(dest!=myNWKAddress|| ENDNODE || BACKUPNODE )
	{
		op_pk_destroy(packet);
		op_pk_destroy(payLoad);
		FOUT;
	}
	//Assign point
	point = distPoint();
	deviceType = CapabilityInformation>>6;
	if(ROUTERNODE)
	{
		NWKAddress = distAddress(deviceType);
		pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + 7;
		generatePk(pklen,0,1,2,1,myNWKAddress,GateAddress,10,MAXTTL-1,NWKAddress,7,payLoad,To_father,0);//generate a sourceSeq
		op_stat_write(contr_send_num,++sendNum);
		printf("Node %d send a sourceSeq!!!\n",myMACAddress);
	}
	else
	{
		//printf("deviceType = %d\n",deviceType);
		NWKAddress = distAddress(deviceType);
		pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN;
		if(deviceType==1)
		{
			//The gateway node establishes a routing table for the directly connected router and sends a confirm
			generateRT(NWKAddress,NWKAddress,0,0);
			//Assign band
			band = distBand(NWKAddress);
			payLoad = op_pk_create(96);//create a accessConfirm_payLoad
			op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,ESNAddress,48);
			op_pk_fd_set(payLoad,1,OPC_FIELD_TYPE_INTEGER,31,8);
			op_pk_fd_set(payLoad,2,OPC_FIELD_TYPE_INTEGER,0,8);
			op_pk_fd_set(payLoad,3,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
			op_pk_fd_set(payLoad,4,OPC_FIELD_TYPE_INTEGER,band,8);
			op_pk_fd_set(payLoad,5,OPC_FIELD_TYPE_INTEGER,point,8);
			pklen += 96;
		}
		else
		{
			payLoad = op_pk_create(80);//create a accessConfirm_payLoad
			op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INT64,ESNAddress,48);
			op_pk_fd_set(payLoad,1,OPC_FIELD_TYPE_INTEGER,26,8);
			op_pk_fd_set(payLoad,3,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
			op_pk_fd_set(payLoad,5,OPC_FIELD_TYPE_INTEGER,point,8);
			pklen += 80;
		}
		generatePk(pklen,0,1,1,3,myNWKAddress,source,1,MAXTTL-1,++NWK_Seq,7,payLoad,To_children,0);//generate a accessConfirm
		op_stat_write(contr_send_num,++sendNum);
		printf("Node %d send a accessConfirm!!!\n",myMACAddress);
		//Build the mapping table
		mappingTable[ESNAddress-1] = NWKAddress;
		//printf("mappingTable[%d]= %d\n",ESNAddress-1,mappingTable[ESNAddress-1]);
	}
	op_pk_destroy(packet);
	generateCT(ESNAddress,NWKAddress,CapabilityInformation);
	FOUT;
}

//Processing sourceSeq
static void sourceSeqPro(Packet* packet)
{
	Packet* payLoad;
	int ESNAddress;
	int source;
	int NWKControl;
	int CapabilityInformation;
	int pklen;
	int deviceType;
	int sourceDistr;
	
	int NWKAddress;
	int endAddress;
	int band;
	int nexthop;
	FIN(sourceSeqPro(Packet* packet));
	op_pk_fd_get(packet,4,&source);
	op_pk_fd_get(packet,5,&NWKControl);
	op_pk_fd_get(packet,6,&payLoad);
	if(GATENODE)
	{
		op_pk_fd_get(payLoad,0,&ESNAddress);
		op_pk_fd_get(payLoad,1,&CapabilityInformation);
		deviceType = CapabilityInformation>>6;
		//printf("deviceType = %d\n",deviceType);
		sourceDistr = CapabilityInformation>>4;
		pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN;
		if(deviceType==1)
		{
			nexthop = nextHop(source);
			//printf("------------nexthop = %d\n",nexthop);
			if(nexthop == 0) FOUT;
			//Assign network address;
			NWKAddress = distAddress(deviceType);
			//Assign band
			band = distBand(NWKAddress);
			generateRT(NWKAddress,nexthop,0,0);
			op_pk_destroy(payLoad);
			payLoad = op_pk_create(96);//create a accessConfirm_payLoad
			op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,ESNAddress,48);
			op_pk_fd_set(payLoad,1,OPC_FIELD_TYPE_INTEGER,31,8);
			op_pk_fd_set(payLoad,2,OPC_FIELD_TYPE_INTEGER,0,8);
			op_pk_fd_set(payLoad,3,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
			op_pk_fd_set(payLoad,4,OPC_FIELD_TYPE_INTEGER,band,8);
			op_pk_fd_set(payLoad,5,OPC_FIELD_TYPE_INTEGER,0,8);
			pklen += 96;
			//Build the mapping table
			mappingTable[ESNAddress-1] = NWKAddress;
		}
		else
		{
			endAddress = ((NWKControl>>8)&255)|source;
			op_pk_destroy(payLoad);
			payLoad = op_pk_create(80);//create a accessConfirm_payLoad
			op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,ESNAddress,48);
			op_pk_fd_set(payLoad,1,OPC_FIELD_TYPE_INTEGER,26,8);
			op_pk_fd_set(payLoad,3,OPC_FIELD_TYPE_INTEGER,0,16);
			op_pk_fd_set(payLoad,5,OPC_FIELD_TYPE_INTEGER,0,8);
			pklen += 80;
			//Build the mapping table
			mappingTable[ESNAddress-1] = endAddress;
		}
		printf("mappingTable[%d]= %d\n",ESNAddress-1,mappingTable[ESNAddress-1]);
		generatePk(pklen,0,1,1,2,myNWKAddress,source,1,MAXTTL-1,++NWK_Seq,7,payLoad,To_children,0);//generate a accessConfirm
		op_stat_write(contr_send_num,++sendNum);
		printf("Node %d send a accessConfirm!!!\n",myMACAddress);
		op_pk_destroy(packet);
	}
	else
	{
		//printf("------------source = %d\n",source);
		nexthop = nextHop(source);
		if(nexthop>0)
		{	
			op_pk_fd_set (packet,6,OPC_FIELD_TYPE_PACKET,payLoad,56);
			forward(packet,To_father);
			//printf("-------forward a sourceSeq!!!\n");
		}
	}
	FOUT;
}

static void accessConfirmPro(Packet* packet,int destMode)
{
	Packet* payLoad;
	int permit,shortAddrComp,bandComp,pointComp,routerSeqComp;
	int dest;
	int ESNAddress,accessContr,NWKAddress,routerSeq,band;
	int pklen;
	int point;
	int nexthop;
	FIN(accessConfirmPro(Packet* packet));
	op_pk_fd_get(packet,3,&dest);
	op_pk_fd_get(packet,6,&payLoad);
	op_pk_fd_get(payLoad,0,&ESNAddress);
	op_pk_fd_get(payLoad,1,&accessContr);
	permit = accessContr>>4;
	printf("Node %d receive a accessConfirm pk dest = %d\n",myMACAddress,dest);
	if(permit == 1)
	{
		if(destMode == 2&&dest == myNWKAddress)
		{
			printf("destMode == 2&&dest == myNWKAddress\n");
			pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + NWKCONTR_LEN + SHORTADDR_LEN + LONGADDR_LEN + 96;
			op_pk_fd_get(payLoad,3,&NWKAddress);
			//printf("---------------------------NWKAddress = %d\n",NWKAddress);
			if(NWKAddress == 0)	
			{
				NWKAddress = CTNWKAddresss(ESNAddress);
				//printf("------------------------------ENDNWKAddress = %d\n",NWKAddress);
			}
			else
			{
				op_pk_fd_get(payLoad,2,&routerSeq);
				if(routerSeq >= 0)		generateRT(NWKAddress,NWKAddress,0,routerSeq);
				else					generateRT(NWKAddress,NWKAddress,0,0);
			}
			point = distPoint();
			op_pk_fd_set(payLoad,5,OPC_FIELD_TYPE_INTEGER,point,8);
			op_pk_fd_set(payLoad,3,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
			generatePk(pklen,0,1,2,3,myNWKAddress,ESNAddress,1,0,++NWK_Seq,96,payLoad,To_children,0);
			printf("Node %d send a accessConfirm pk dest = %d\n",myMACAddress,ESNAddress);
			op_stat_write(contr_send_num,++sendNum);
			UPCTStatus(ESNAddress,NWKAddress);
			if(backupRouter > 0)
			{
				tableBackup(2,0,NWKAddress,0,1);
			}
			printf("UPCTStatus\n");
			op_pk_destroy(packet);
		}
		else if(destMode == 3&&dest == myMACAddress)
		{
			printf("destMode == 3&&dest == myMACAddress\n");
			shortAddrComp = 1&accessContr>>3;
			bandComp = 1&accessContr>>2;
			pointComp = 1&accessContr>>1;
			routerSeqComp = 1&accessContr;
			op_ima_obj_attr_get(nodeID,"father",&myFatherAddress);
			if(shortAddrComp==1)	op_pk_fd_get(payLoad,3,&NWKAddress);
			if(bandComp==1)			op_pk_fd_get(payLoad,4,&band);
			if(pointComp==1)		op_pk_fd_get(payLoad,5,&point);
			if(routerSeqComp==1)	op_pk_fd_get(payLoad,2,&routerSeq);
			myNWKAddress = NWKAddress;
			myBand = band;
			if(myBand == 0)	myBand = 48;
			myPoint = point;
			myRouterSeq = routerSeq;
			op_ima_obj_attr_set(nodeID,"NodeStatus",OnlineStatus);
			op_ima_obj_attr_set(nodeID,"NodeBand",myBand);
			op_ima_obj_attr_set(nodeID,"NodePoint",myPoint);
			node_status= OnlineStatus;
			op_ima_obj_attr_set(nodeID,"ShortAddress",myNWKAddress);
			op_intrpt_schedule_remote(op_sim_time(), intrCode_Status, op_id_from_name (op_topo_parent(op_id_self()) , OPC_OBJTYPE_QUEUE, "MAC")) ;
			printf("----------------\n");
			printf("NodeBand = %d       ",myBand);
			printf("myNWKAddress = %d\n",myNWKAddress);
			//printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!band = %d,point = %d!!!!!!!!!!!!!!!!!!!!!!!!!!!\n",myBand,myPoint);
			if(nodeType == 3)		op_intrpt_schedule_self(op_sim_time(),intrCode_UpStatus);
			op_pk_destroy(packet);
			op_pk_destroy(payLoad);
		}
		else
		{
			nexthop = nextHop(dest);
			if(nexthop>0)
			{	
				op_pk_fd_get(payLoad,3,&NWKAddress);
				if(nodeType == 1 )	generateRT(NWKAddress,nexthop,0,0);
				op_pk_fd_set (packet,6,OPC_FIELD_TYPE_PACKET,payLoad,80);
				forward(packet,To_children);
				//printf("-------forward a accessConfirmPro!!!\n");
			}
		}
	}
	//Rejection is not considered in the simulation
	else
	{
		printf("permit == 0\n");
		op_pk_destroy(packet);
		op_pk_destroy(payLoad);
	}
	FOUT;
}

static void RoutingUPPro(Packet* packet)
{
	Packet* payLoad;
	int control;
	int RouterSeq;
	int NWKAddress;
	int ESNAddress;
	FIN(RoutingUPPro(Packet* packet));
	printf("Node %d receive a RoutingUP pk!\n",myMACAddress);
	op_pk_fd_get(packet,6,&payLoad);
	op_pk_fd_get(payLoad,0,&control);
	op_pk_fd_get(payLoad,1,&RouterSeq);
	op_pk_fd_get(payLoad,2,&NWKAddress);
	op_pk_fd_get(payLoad,3,&ESNAddress);
	printf("RouterSeq = %d,control = %d\n",RouterSeq,control);
	if(control == 0)
	{
		UpdateRouting(0,RouterSeq,NWKAddress);
		if(ROUTERNODE)
		{
			op_pk_fd_set(packet,6,OPC_FIELD_TYPE_PACKET,payLoad,72);
			forward(packet,To_father);
		}
	}
	else
	{
		if(RouterSeq>=0)
		{
			UpdateRouting(1,RouterSeq,NWKAddress);
		}
		if(GATENODE)
		{
			if(mappingTable[ESNAddress - 1]==NWKAddress)
			{
				mappingTable[ESNAddress - 1] = 0;
			}
			op_pk_destroy(packet);
			op_pk_destroy(payLoad);
		}
		else if(ROUTERNODE)
		{
			op_pk_fd_set(packet,6,OPC_FIELD_TYPE_PACKET,payLoad,72);
			forward(packet,To_father);
		}
		else
		{
			op_pk_destroy(packet);
			op_pk_destroy(payLoad);
		}
	}
	FOUT;
}

static void multicastContrPro(Packet* packet)
{
	Packet* payLoad;
	int i;
	int dest;
	int MulticastInformation,type,number;
	int muticastID;
	FIN(multicastContrPro(Packet* packet));
	op_pk_fd_get(packet,3,&dest);
	op_pk_fd_get(packet,6,&payLoad);
	op_pk_fd_get(payLoad,0,&MulticastInformation);
	type = 1&MulticastInformation;
	number = 15&(MulticastInformation>>1);
	printf("Node %d receive a MulticastContr to dest=%d!\n",myMACAddress,dest);
	printf("%d\n",MulticastInformation);
	if((ENDNODE || BACKUPNODE)&&dest!=myNWKAddress)
	{
		op_pk_destroy(packet);
		op_pk_destroy(payLoad);
		FOUT;
	}	
	for(i = 0;i<number;i++)
	{
		op_pk_fd_get(payLoad,i+1,&muticastID);
		if(dest == myNWKAddress)
		{
			if(type == 1)	myMT[muticastID] = 1;
			else			myMT[muticastID] = 0;
			printf("Node %d add/subtract a muticastID = %d\n",myMACAddress,muticastID);
		}
		if(ROUTERNODE)
		{
			if(type == 1)	childMT[muticastID]++;
			else			childMT[muticastID] = childMT[muticastID] > 0 ? --childMT[muticastID] : 0;
			printf("Node %d add/subtract a muticastID = %d to ChildMT\n",myMACAddress,muticastID);
		}
	}
	if(ROUTERNODE && dest != myNWKAddress)
	{
		op_pk_fd_set (packet,6,OPC_FIELD_TYPE_PACKET,payLoad,8+number*16);
		forward(packet,To_children);
	}
	else
	{
		op_pk_destroy(packet);
		op_pk_destroy(payLoad);
	}
	FOUT;
}

static void leavePro(Packet* packet)
{
	Packet* payLoad;
	int frameControl;
	int sourceMode;
	int mutilcast;
	int source;
	int dest;
	int leaveInfo;
	int rejoin;
	int request;
	int removeChildren;
	
	int pklen;
	int nexthop;
	
	int control = 1;
	int routerSeq;
	int ESNAddress;
	FIN(leavePro(Packet* packet));
	printf("Node %d received a leaveControl pk\n",myMACAddress);
	op_pk_fd_get(packet,0,&frameControl);
	sourceMode = frameControl&3;
	mutilcast = (frameControl>>8)&1;
	op_pk_fd_get(packet,4,&source);
	op_pk_fd_get(packet,3,&dest);
	op_pk_fd_get(packet,6,&payLoad);
	op_pk_fd_get(payLoad,0,&leaveInfo);
	rejoin = (leaveInfo>>2)&1;
	request = (leaveInfo>>1)&1;
	removeChildren = leaveInfo&1;
	if(mutilcast == 0)
	{
		if(dest == myNWKAddress || dest == ChildrenEndCast)
		{
			if(request == 1)
			{
				printf("removeChildren == %d && Network_Msg.NetworkCT[myMACAddress].number = %d\n",removeChildren,Network_Msg.NetworkCT[myMACAddress].number);
				if(removeChildren == 1 && Network_Msg.NetworkCT[myMACAddress].number > 0)
				{
					op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,leaveInfo,8);
					pklen = 104;
					generatePk(pklen,0,0,2,2,myNWKAddress,ChildrenEndCast,2,0,++NWK_Seq,8,payLoad,To_children,0);
				}
				else
				{
					op_pk_destroy(payLoad);
				}
				leaveNet(myNWKAddress,rejoin,removeChildren,0);
			}
			else
			{
				leaveNet(source,rejoin,removeChildren,0);
				op_pk_destroy(payLoad);
				routerSeq = quiryRouterSeq(source);
				ESNAddress = CTinclude(source);
				payLoad = op_pk_create (72);
				op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,control,4);
				op_pk_fd_set(payLoad,1,OPC_FIELD_TYPE_INTEGER,routerSeq,4);
				op_pk_fd_set(payLoad,2,OPC_FIELD_TYPE_INTEGER,source,16);
				op_pk_fd_set(payLoad,3,OPC_FIELD_TYPE_INTEGER,ESNAddress,48);
				generatePk(152,0,1,2,1,myNWKAddress,GateAddress,11,MAXTTL,++NWK_Seq,9,payLoad,To_father,0);
				
			}
			op_pk_destroy(packet);
		}
		else
		{
			nexthop = nextHop(dest);
			ESNAddress = CTinclude(dest);
			if(nexthop>0||ESNAddress>0)
			{
				op_pk_fd_set (packet,6,OPC_FIELD_TYPE_PACKET,payLoad,8);
				leaveNet(dest,rejoin,removeChildren,0);
				forward(packet,To_children);
			}
		}
	}
	else
	{
		if(quiryChildMT(dest) > 0)
		{
			op_pk_fd_set (packet,6,OPC_FIELD_TYPE_PACKET,payLoad,8);
			//leaveNet(dest,rejoin,removeChildren,0);
			forward(packet,To_children);
		}
		if(quiryMT(dest) == 1)
		{
			if(request == 1)
			{
				if(removeChildren == 1 && Network_Msg.NetworkCT[myMACAddress].number > 0)
				{
					op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,leaveInfo,8);
					pklen = 104;
					generatePk(pklen,0,0,2,2,myNWKAddress,ChildrenEndCast,2,0,++NWK_Seq,8,payLoad,To_children,0);
				}
				leaveNet(myNWKAddress,rejoin,removeChildren,0);
			}
			else
			{
				leaveNet(source,rejoin,removeChildren,0);
				op_pk_destroy(payLoad);
			}
		}
		op_pk_destroy(packet);
	}
	FOUT;
}

static void mainRouterPro(Packet* packet)
{
	Packet* payload;
	int type;
	FIN(mainRouterPro(Packet* packet));
	op_pk_fd_get(packet,6,&payload);
	op_pk_fd_get(payload,0,&type);
	printf("mainRouterPro type = %d\n",type);
	if(type == 1)
	{
		op_pk_fd_set (packet,6,OPC_FIELD_TYPE_PACKET,payload,56);
		mainRouterQuery(packet);
	}
	else if(type == 2)
	{
		op_pk_fd_set (packet,6,OPC_FIELD_TYPE_PACKET,payload,32);
		mainRouterResponse(packet);
	}
	else
	{
		op_pk_destroy(payload);
		op_pk_destroy(packet);
	}
	FOUT;
}

static void mainRouterQuery(Packet* packet)
{
	int pklen;
	int source;
	
	int ESNAddress;
	int NWKAddress;
	int band;
	Packet* payload;
	FIN(mainRouterQuery(Packet* packet));
	if(nodeType != 0)
	{
		forward(packet,To_father);
	}
	else
	{
		op_pk_fd_get(packet,6,&payload);
		op_pk_fd_get(payload,1,&ESNAddress);
		NWKAddress = mappingTable[ESNAddress - 1];
		if(NWKAddress == 0)
		{
			op_pk_destroy(payload);
			op_pk_destroy(packet);
			FOUT;
		}
		band = bandQuire(NWKAddress);
		pklen = 112;
		op_pk_fd_get(packet,4,&source);
		payload = op_pk_create(32);
		op_pk_fd_set(payload,0,OPC_FIELD_TYPE_INTEGER,2,8);
		op_pk_fd_set(payload,1,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
		op_pk_fd_set(payload,2,OPC_FIELD_TYPE_INTEGER,band,8);
		generatePk(pklen,0,1,1,2,myNWKAddress,source,12,MAXTTL - 1,++NWK_Seq,32,payload,To_children,0);
		op_pk_destroy(packet);
		printf("Node %d received a mainRouterQuery query router %d's NWKAddress = %d\n",myMACAddress,ESNAddress,NWKAddress);
	}
	FOUT;
}


static void mainRouterResponse(Packet* packet)
{

	Packet* payload;
	int dest;
	int NWKAddress;
	int band;
	FIN(mainRouterQuery(Packet* packet));
	op_pk_fd_get(packet,3,&dest);
	printf("-------------------mainRouterResponse node %d dest %d\n--------------------",myMACAddress,dest);
	if(dest != myNWKAddress && nextHop(dest) == 0)
	{
		printf("destroy\n");
		op_pk_fd_get(packet,6,&payload);
		op_pk_destroy(payload);
		op_pk_destroy(packet);
	}
	else if(dest == myNWKAddress)
	{
		op_pk_fd_get(packet,6,&payload);
		op_pk_fd_get(payload,1,&NWKAddress);
		op_pk_fd_get(payload,2,&band);
		op_pk_destroy(payload);
		op_pk_destroy(packet);
		printf("need rejoin!!!,fatherAddress = %d,band = %d\n",NWKAddress,band);
		op_ima_obj_attr_set(nodeID,"father",NWKAddress);
		op_ima_obj_attr_set(nodeID,"NodeBand",band);
		mainNWKAddress = NWKAddress;
		payload = op_pk_create (8);
		op_pk_fd_set(payload,0,OPC_FIELD_TYPE_INTEGER,4);
		generatePk(104,0,1,2,2,myNWKAddress,myFatherAddress,2,0,++NWK_Seq,1,payload,To_father,0);
		printf("Node %d send a leave advice\n",myMACAddress);
		leaveNet(myNWKAddress,1,0,1);
		
	}
	else
	{
		forward(packet,To_children);
	}
	
	FOUT;
}

static void mainRouterStatusQuiry(Packet* packet)
{
	/*
	
	
	
	
	*/
}

static void mainRouterInfoPro(Packet* packet)
{
	int i;
	int index = 0;
	
	Packet* payLoad;
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
	
	FIN(mainRouterInfoPro(Packet* packet));
	op_pk_fd_get(packet,6,&payLoad);
	op_pk_fd_get(payLoad,0,&infoControl);
	type = (infoControl>>5)&3;
	addOrReduce = (infoControl>>4)&1;
	number = infoControl&15;
	printf("mainRouterInfoPro,type = %d,number = %d,addOrReduce = %d\n",type,number,addOrReduce);
	switch(type)
	{
		case 0: for(i = 0;i<number;i++)
				{
					op_pk_fd_get(payLoad,++index,&NWKAddress);
					op_pk_fd_get(payLoad,++index,&nextHop);
					op_pk_fd_get(payLoad,++index,&status);
					op_pk_fd_get(payLoad,++index,&routerSeq);
					if(addOrReduce == 0)
					{
						generateRT(NWKAddress,nextHop,status,routerSeq);
					}
					else
					{
						UpdateRouting(1,-1,NWKAddress);
					}
				}
				break;
		case 1: for(i = 0;i<number;i++)
				{
					op_pk_fd_get(payLoad,++index,&ESNAddress);
					op_pk_fd_get(payLoad,++index,&shortAddress);
					op_pk_fd_get(payLoad,++index,&capabilityInformation);
					if(addOrReduce == 0)
					{
						generateCT(ESNAddress,shortAddress,capabilityInformation);
					}
					else
					{
						UpdateCT(ESNAddress);
					}
				}
				break;
		case 2: op_pk_fd_get(payLoad,++index,&NWKAddress);
				op_pk_fd_get(payLoad,++index,&nextHop);
				op_pk_fd_get(payLoad,++index,&status);
				op_pk_fd_get(payLoad,++index,&routerSeq);
				op_pk_fd_get(payLoad,++index,&ESNAddress);
				op_pk_fd_get(payLoad,++index,&shortAddress);
				op_pk_fd_get(payLoad,++index,&capabilityInformation);
				if(addOrReduce == 0)
				{
					generateRT(NWKAddress,nextHop,status,routerSeq);
					generateCT(ESNAddress,shortAddress,capabilityInformation);
				}
				else
				{
					UpdateRouting(1,-1,NWKAddress);
					UpdateCT(ESNAddress);
				}
				break;
		case 3: op_pk_fd_get(payLoad,++index,&mainFather);
				op_pk_fd_get(payLoad,++index,&mainBand);
				op_pk_fd_get(payLoad,++index,&mainPoint);
				printf("mainFather = %d,mainBand = %d,mainPoint = %d\n",mainFather,mainBand,mainPoint);
				break;
	}
	
	FOUT;
}


static void backupPro(Packet* packet)
{
	Packet* payLoad;
	int control;
	int request;
	int status;
	int NWKAddress;
	
	FIN(backupPro(Packet* packet));
	op_pk_fd_get(packet,6,&payLoad);
	op_pk_fd_get(payLoad,0,&control);
	request = control&1;
	if(request == 0 && ROUTERNODE)//query
	{
		op_pk_fd_get(payLoad,1,&NWKAddress);
		if( CTinclude(NWKAddress) > 0)
		{
			//向该节点发送保活消息
		}
	}
	else if(request == 1 && BACKUPNODE)//response
	{
		status = control>>1;
		if(status == 0)
		{
			
		}
		else
		{
			myNWKAddress = mainFather;
			myBand = mainBand;
			myPoint = mainPoint;
			payLoad = op_pk_create(64);
			op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,myNWKAddress,16);
			op_pk_fd_set(payLoad,1,OPC_FIELD_TYPE_INTEGER,myMACAddress,48);
			generatePk(160,0,1,2,1,myNWKAddress,GateAddress,3,MAXTTL-1,++NWK_Seq,8,payLoad,To_father,0);
		}
	}
	FOUT;
}
//----------------------------------------------------------------------------------------------------------------------------------
/*
	processing all types of data packet!!!
*/
static void upDataPro(Packet* packet)
{
	Packet* payLoad;
	int dest;
	int NWKControl;
	int len;
	int data;
	FIN(upDataPro(Packet* packet));
	op_pk_fd_get(packet,3,&dest);
	op_pk_fd_get(packet,5,&NWKControl);
	op_pk_fd_get(packet,6,&payLoad);
	op_pk_fd_get(payLoad,0,&data);
	len = NWKControl&255;
	if(GateAddress == myNWKAddress)
	{
		//op_pk_send(packet,1);
		op_stat_write(ete_delay_up,op_sim_time () - op_pk_creation_time_get (packet));
		printf("-------------------------------------------------------------------------delay = %f-------------------\n",op_sim_time () - op_pk_creation_time_get (packet));
		op_stat_write(up_data_receive_num,++upReceiveNum);
		op_stat_write(up_pk_loss_rate,upReceiveNum/upSendNum);
		printf("----------------------------Node %d receive a up data = %d!  ----",myMACAddress,data);
		printf("len = %d-------------------\n",len);
		op_pk_destroy(packet);
		op_pk_destroy(payLoad);
		FOUT;
	}
	else
	{
		op_pk_fd_set (packet,6,OPC_FIELD_TYPE_PACKET,payLoad,len*8);
		forward(packet,To_father);
	}
	FOUT;
}

static void downDataPro(Packet* packet,int multicast)
{
	Packet* payLoad;
	int dest;
	int NWKControl;
	int len;
	int data;
	int nexthop;
	int ESNAddress;
	FIN(downDataPro(Packet* packet));
	op_pk_fd_get(packet,3,&dest);
	op_pk_fd_get(packet,5,&NWKControl);
	op_pk_fd_get(packet,6,&payLoad);
	op_pk_fd_get(payLoad,0,&data);
	len = NWKControl&255;
	printf("----------------------------Node %d receive a down data! dest = %d ----\n",myMACAddress,dest);
	if(multicast == 0)
	{
		if(dest == myNWKAddress)
		{
			//op_pk_send(packet,2);
			op_stat_write(ete_delay_down,op_sim_time () - op_pk_creation_time_get (packet));
			
			printf("-------------------------------------------------------------------------delay = %f-------------------\n",op_sim_time () - op_pk_creation_time_get (packet));
			printf("\n\nNode %d receivea data %f!\n\n",myMACAddress,op_sim_time());
			op_stat_write(down_data_receive_num,++downReceiveNum);
			op_stat_write(down_pk_loss_rate,downReceiveNum/downSendNum);
			printf("----------------------------Node %d receive a unicast down data = %d!  ----",myMACAddress,data);
			printf("len = %d-------------------\n",len);
			op_pk_destroy(packet);
			op_pk_destroy(payLoad);
			FOUT;
		}
		nexthop = nextHop(dest);
		ESNAddress = CTinclude(dest);
		if(nexthop>0||ESNAddress>0)
		{
			op_pk_fd_set (packet,6,OPC_FIELD_TYPE_PACKET,payLoad,len);
			forward(packet,To_children);
		}
		else
		{
			//printf("No next hop!ESNAddress = %d\n",ESNAddress);
			op_pk_destroy(packet);
			op_pk_destroy(payLoad);
		}
	}
	else
	{
		//printf("childMT[dest] = %d\n",childMT[dest]);
		if(myMT[dest] == 1)
		{
			//op_pk_send(packet,1);
			op_pk_destroy(packet);
			op_pk_destroy(payLoad);
			printf("----------------------------Node %d receive a multicast down data = %d!  ----",myMACAddress,data);
			printf("len = %d-------------------\n",len);
		}
		if(ROUTERNODE&&childMT[dest] > 0)
		{
			op_pk_fd_set (packet,6,OPC_FIELD_TYPE_PACKET,payLoad,len);
			forward(packet,To_children);
		}
	}
	FOUT;
}

//----------------------------------------------------------------------------------------------------------------------------------
/*
	generate a Children table
*/
static void generateCT(int ESNAddress,int NWKAddress,int CapabilityInformation)
{
	struct childrenTable T;
	FIN(gerenateCT(int ESNAddress,int CapabilityInformation));
	T.ESNAddress = ESNAddress;
	printf("Node %d generateCT T.ESNAddress = %d\n",myMACAddress,T.ESNAddress);
	T.NWKAddress = NWKAddress;
	T.TimeoutCount = CTTimercount;
	T.CapabilityInformation = CapabilityInformation;
	Network_Msg.NetworkCT[myMACAddress].table[Network_Msg.NetworkCT[myMACAddress].number] = T;
	Network_Msg.NetworkCT[myMACAddress].number++;
	FOUT;
}

/*
	generate a Router table
*/
static void generateRT(int destAddress,int nextHop,int status,int routerSeq)
{
	struct routerTable T;
	int i;
	FIN(gerenateRT(int destAddress,int nextHop,int status,int routerSeq));
	destAddress &= (255<<8);
	if(nextHop == 0 || destAddress == 0)	FOUT;
	printf("Node %d generateRT destAddress = %d\n",myMACAddress,destAddress);
	for(i = 0;i<RT.number;i++)
	{
		if(RT.table[i].destAddress == destAddress)
		{
			if(RT.table[i].routerSeq <= routerSeq)
			{
				RT.table[i].destAddress = destAddress;
				RT.table[i].nextHop = nextHop;
				RT.table[i].status = status;
				RT.table[i].routerSeq = routerSeq;
				RT.table[i].TimeoutCount = RTTimercount;
			}
			FOUT;
		}
	}
	T.destAddress = destAddress;
	T.nextHop = nextHop;
	T.status = status;
	T.routerSeq = routerSeq;
	T.TimeoutCount = RTTimercount;
	RT.table[RT.number] = T;
	printf("--------------------------------------Node %d add a RT destAddress = %d,TimeoutCount = %d\n",myMACAddress,RT.table[RT.number].destAddress,RT.table[RT.number].TimeoutCount);
	RT.number++;
	Network_Msg.NetworkRT[myMACAddress] = RT;
	printf("--------------------------------------Node %d add a RT destAddress = %d\n",myMACAddress,Network_Msg.NetworkRT[myMACAddress].table[Network_Msg.NetworkRT[myMACAddress].number - 1].destAddress);
	FOUT;
}

/*
	Update CT
*/

static void UpdateRT(int NWKAddress)
{
	int i,j;
	FIN(UpdateRT(int NWKAddress));
	if(NWKAddress > 0)
	{
		printf("Node %d UpdateRT NWKAddress = %d\n",myMACAddress,NWKAddress);
		i = queryRTIndex(NWKAddress);
		if(i >= 0)
		{
			printf("Node %d receive a data from %d ,updateRT,i = %d,RT.number = %d\n",myMACAddress,NWKAddress,i,RT.number);
			RT.table[i].TimeoutCount = RTTimercount;
			printf("no error\n");
		}
	}
	else
	{
		for(i = 0;i<RT.number;i++)
		{
			--RT.table[i].TimeoutCount;
			printf("Node %d RT.table.TimeoutCount = %d\n",myMACAddress,RT.table[i].TimeoutCount);
			if(RT.table[i].TimeoutCount <= 0)
			{
				for(j = i+1;j<RT.number;j++)
				{
					RT.table[j - 1] = RT.table[j];
				}
				RT.number--;
			}
		}
		Network_Msg.NetworkRT[myMACAddress] = RT;
	}
	FOUT;
}

/*
	Update CT
*/
static void UpdateCT(int ESNAddress)
{
	Packet* payLoad;
	int i,j;
	int NWKAddress;
	int childNodeType;
	
	int control = 1;
	int routerSeq;
	FIN(UpdateCT(int ESNAddress));
	if(ESNAddress > 0)
	{
		for(i = 0;i<Network_Msg.NetworkCT[myMACAddress].number;i++)
		{
			if(Network_Msg.NetworkCT[myMACAddress].table[i].ESNAddress == ESNAddress)
			{
				for(j = i+1;j<Network_Msg.NetworkCT[myMACAddress].number;j++)
				{
					Network_Msg.NetworkCT[myMACAddress].table[j - 1] = Network_Msg.NetworkCT[myMACAddress].table[j];
				}
				Network_Msg.NetworkCT[myMACAddress].number--;
			}
		}
		FOUT;
	}
	for(i = 0;i<Network_Msg.NetworkCT[myMACAddress].number;i++)
	{	
		--Network_Msg.NetworkCT[myMACAddress].table[i].TimeoutCount;
		if(Network_Msg.NetworkCT[myMACAddress].table[i].TimeoutCount<=0)
		{
			NWKAddress = Network_Msg.NetworkCT[myMACAddress].table[i].NWKAddress;
			ESNAddress = Network_Msg.NetworkCT[myMACAddress].table[i].ESNAddress;
			//printf("Childe node %d timercount = %d\n",ESNAddress,Network_Msg.NetworkCT[myMACAddress].table[i].TimeoutCount);
			childNodeType = 3&(Network_Msg.NetworkCT[myMACAddress].table[i].CapabilityInformation>>6);
			for(j = i+1;j<Network_Msg.NetworkCT[myMACAddress].number;j++)
			{
				Network_Msg.NetworkCT[myMACAddress].table[j - 1] = Network_Msg.NetworkCT[myMACAddress].table[j];
			}
			Network_Msg.NetworkCT[myMACAddress].number--;
			routerSeq = quiryRouterSeq(NWKAddress);
			if(childNodeType == 1)
			{
				UpdateRouting(1,routerSeq,NWKAddress);
			}
			if(GATENODE)
			{
				mappingTable[ESNAddress - 1] = 0;
				FOUT;
			}
			payLoad = op_pk_create (72);
			if(childNodeType == 2 || childNodeType == 3)
			{
				op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,control,4);
				op_pk_fd_set(payLoad,1,OPC_FIELD_TYPE_INTEGER,routerSeq,4);
				op_pk_fd_set(payLoad,2,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
				op_pk_fd_set(payLoad,3,OPC_FIELD_TYPE_INTEGER,ESNAddress,48);
			}
			else
			{
				op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,control,4);
				op_pk_fd_set(payLoad,1,OPC_FIELD_TYPE_INTEGER,routerSeq,4);
				op_pk_fd_set(payLoad,2,OPC_FIELD_TYPE_INTEGER,NWKAddress,16);
				op_pk_fd_set(payLoad,3,OPC_FIELD_TYPE_INTEGER,ESNAddress,48);
			}
			generatePk(152,0,1,2,1,myNWKAddress,GateAddress,11,MAXTTL,++NWK_Seq,9,payLoad,To_father,0);
			op_stat_write(contr_send_num,++sendNum);
			printf("Node %d send a updata pk\n",myMACAddress);
			FOUT;
		}
	}
	FOUT;
}

/*
	Routing update
*/
static void UpdateRouting(int control,int routerSeq,int NWKAddress)
{
	int i,j;
	int index,nexthop;
	FIN(UpdateRouting(int control,int routerSeq,int NWKAddress));
	if(control == 0)
	{
		index = queryRTIndex(NWKAddress);
		printf("index = %d\n",index);
		if(index < 0)
		{
			nexthop = nextHop(NWKAddress);
			if(nexthop <= 0)
			{
				if(CTinclude(NWKAddress) > 0)	nexthop = NWKAddress;
				else							FOUT;
			}
			generateRT(NWKAddress,nexthop,0,routerSeq);
		}
	}
	else
	{
		for(i = 0;i<RT.number;i++)
		{	
			if(RT.table[i].destAddress==NWKAddress && (routerSeq>=RT.table[i].routerSeq||routerSeq == -1))
			{
				for(j = i+1;j<RT.number;j++)
				{
					RT.table[j - 1] = RT.table[j];
				}
				RT.number--;
				printf("Node %d updata RT,address = %d,RT.number = %d\n",myMACAddress,NWKAddress,RT.number);
				break;
			}
		}
	}
	Network_Msg.NetworkRT[myMACAddress] = RT;
	FOUT;
}
//----------------------------------------------------------------------------------------------------------------------------------
/*
	Query next hop from Router table
*/
static int nextHop(int dest)
{
	int i = 0;
	int router;
	FIN(nextHop(int dest));
	router = dest&(255<<8);
	if(router == myNWKAddress)	FRET(dest);
	for(i = 0;i < RT.number;i++)
	{
		if(RT.table[i].destAddress == router && RT.table[i].status == 0)
		{
			FRET(RT.table[i].nextHop);
		}
	}
	FRET(0);
}

static int queryRTIndex(int NWKAddress)
{
	int i;
	FIN(queryRTIndex(int NWKAddress));
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
	Query CT NWKAddress
*/
static int CTNWKAddresss(int ESNAddress)
{
	int i = 0;
	FIN(CTNWKAddresss(int ESNAddress));
	for(i = 0;i < Network_Msg.NetworkCT[myMACAddress].number;i++)
	{
		if(Network_Msg.NetworkCT[myMACAddress].table[i].ESNAddress == ESNAddress)	FRET(Network_Msg.NetworkCT[myMACAddress].table[i].NWKAddress);
	}
	FRET(0);
}

/*
	Updata CT status
*/
static void UPCTStatus(int ESNAddress,int NWKAddress)
{
	int i = 0;
	int capabilityInformation;
	int deviceType;
	FIN(UPCTStatus(int ESNAddress,int NWKAddress));
	printf("Network_Msg.NetworkCT[myMACAddress].number = %d\n",Network_Msg.NetworkCT[myMACAddress].number);
	for(i = 0;i < Network_Msg.NetworkCT[myMACAddress].number;i++)
	{
		if(Network_Msg.NetworkCT[myMACAddress].table[i].ESNAddress == ESNAddress)
		{
			Network_Msg.NetworkCT[myMACAddress].table[i].NWKAddress = NWKAddress;
			capabilityInformation = Network_Msg.NetworkCT[myMACAddress].table[i].CapabilityInformation;
			deviceType = (capabilityInformation>>6)&3;
			printf("deviceType = %d\n",deviceType);
			if(deviceType == 3)
			{
				backupRouter = NWKAddress;
				initBackup();
			}
			FOUT;
		}
	}
	//Network_Msg.NetworkCT[myMACAddress] = CT;
	FOUT;
}

/*
	Find the network address through the mapping table!
*/
static int quiryNWKAddress(int MACAddress)
{
	FIN(quiryNWKAddress(int MACAddress));
	FRET(mappingTable[MACAddress - 1]);
}

/*
	Find the RouterSeq!
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
	Check whether the child node is included in the table through the network address,return ESNAddress or 0
*/
static int CTinclude(int NWKAddress)
{
	int i = 0;
	FIN(CTinclude(int NWKAddress));
	//printf("Network_Msg.NetworkCT[myMACAddress].number = %d\n", Network_Msg.NetworkCT[myMACAddress].number);
	for(i = 0;i < Network_Msg.NetworkCT[myMACAddress].number;i++)
	{
		//printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$Network_Msg.NetworkCT[myMACAddress].table[i].NWKAddress = %d,Network_Msg.NetworkCT[myMACAddress].table[i].ESNAddress = %d$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n",Network_Msg.NetworkCT[myMACAddress].table[i].NWKAddress,Network_Msg.NetworkCT[myMACAddress].table[i].ESNAddress);
		if(Network_Msg.NetworkCT[myMACAddress].table[i].NWKAddress == NWKAddress)	
		{
			FRET(Network_Msg.NetworkCT[myMACAddress].table[i].ESNAddress);
		}
	}
	FRET(0);
}

/*
	Quire band through NWKAddress
*/
static int bandQuire(int NWKAddress)
{
	int i;
	FIN(bandQuire(int NWKAddress));
	for(i = 0;i<MAX_ROUTER;i++)
	{
		if(bandTable[i]==NWKAddress)
		{
			FRET(i);
		}
	}
	FRET(-1);
}
static int quiryMT(int dest)
{
	FIN(quiryMT(int dest));
	
	FRET(0);
}

static int quiryChildMT(int dest)
{
	FIN(quiryChildMT(int dest));
	
	FRET(0);
}

static int quiryNWKAddressFromMap(int NWKAddress)
{
	int i;
	FIN(quiryNWKAddressFromMap(int NWKAddress));
	for(i = 0;i < MAX_NODE;i++)
	{
		if(NWKAddress == mappingTable[i])
		{
			FRET(i + 1);
		}
	}
	FRET(0);
}

static int queryFT(int fatherAddress)
{
	int i;
	FIN(queryFT(int fatherAddress));
	for(i = 0;i<MAXFT;i++)
	{
		if(potentialParent[myMACAddress][i].short_addr == fatherAddress)	FRET(potentialParent[myMACAddress][i].band);
	}
	FRET(-1);
}

//----------------------------------------------------------------------------------------------------------------------------------
/*
	Address distribute function
*/
static int distAddress(int deviceType)
{
	int i;
	int NWKAddress = 0;
	int n;
	FIN(distAddress(int deviceType));
	if(GATENODE)	n = MAX_NODE;
	else if(ROUTERNODE)	n = MAX_CHILD;
	else	FRET(0);
	switch(deviceType)
	{
		//Assign addresses to routing nodes
		case 1: for(i = 0;i < MAX_RouterAddress;i++)
				{
					if(GATENODE && RouterAddressFlag[i] == 0)
					{
						RouterAddressFlag[i] = 1;
						NWKAddress = (i + 1)<<8;
						//printf("NWKAddress dist = %d\n",NWKAddress);
						FRET(NWKAddress);
					}
				}
				break;
		//Assign addresses to end nodes
		case 2: for(i = 0;i < n;i++)
				{
					if(EndAddressFlag[i] == 0)
					{
						EndAddressFlag[i] = 1;
						NWKAddress = (i + 1)|myNWKAddress;
						FRET(NWKAddress);
					}
				}
				break;
		case 3: for(i = 0;i < n;i++)
				{
					if(EndAddressFlag[i] == 0)
					{
						EndAddressFlag[i] = 1;
						NWKAddress = (i + 1)|myNWKAddress;
						FRET(NWKAddress);
					}
				}
				break;
		default:FRET(0);
	}
	FRET(0);
}

/*
	Band distribute function
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
	Point distribute function
*/
static int distPoint()
{
	FIN(distPoint());
	if(GATENODE)	FRET(op_dist_uniform (GATEPOINTNUM));			
	FRET(op_dist_uniform (POINTNUM));
	FRET(0);
}

//----------------------------------------------------------------------------------------------------------------------------------
/*
	NodeStatus changed function
*/
static void leaveNet(int NWKAddress,int rejoin,int children,int assign)
{
	int i;
	int j;
	int GateIndex;
	int EndIndex;
	int routerIndex;
	int ESNAddress;
	int band;
	FIN(leaveNet(int NWKAddress,int rejoin,int children,int assign));
	if(myNWKAddress == NWKAddress)
	{
		printf("Node %d leaved,rejoin = %d and assign = %d\n",myMACAddress,rejoin,assign);
		CRTinit();
		node_status = OfflineStatus;
		if(rejoin == 1)
		{
			if(assign == 0)	op_ima_obj_attr_set(nodeID,"NodeStatus",RejoinStatus);
			else			op_ima_obj_attr_set(nodeID,"NodeStatus",AssignStatus);
		}
		else
		{
			op_ima_obj_attr_set(nodeID,"NodeStatus",LeaveStatus);
		}
		op_intrpt_schedule_remote(op_sim_time() + 0.1, intrCode_LeaveNet, op_id_from_name (op_topo_parent(op_id_self()) , OPC_OBJTYPE_QUEUE, "MAC")) ;
	}
	else
	{
		GateIndex = (NWKAddress>>15);
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
		for(i = 0;i < Network_Msg.NetworkCT[myMACAddress].number;i++)
		{
			if(Network_Msg.NetworkCT[myMACAddress].table[i].NWKAddress == NWKAddress)
			{
				if(EndIndex > 0)	EndAddressFlag[ESNAddress] = 0;
				for(j = i+1;j<Network_Msg.NetworkCT[myMACAddress].number;j++)
				{
					Network_Msg.NetworkCT[myMACAddress].table[j - 1] = Network_Msg.NetworkCT[myMACAddress].table[j];
				}
				Network_Msg.NetworkCT[myMACAddress].number--;
			}
		}
		if(EndIndex > 0)
		{
			if(GATENODE)
			{
				printf("ENDAddress = %d\n",mappingTable[ESNAddress - 1]);
				mappingTable[ESNAddress - 1] = 0;
			}
		}
		else if(routerIndex > 0)
		{
			UpdateRouting(1,-1,NWKAddress);
			if(GATENODE)
			{
				band = bandQuire(NWKAddress);
				if(band>=0) bandTable[band] = 0;
				mappingTable[ESNAddress - 1] = 0;
				printf("band = %d,ESNAddress = %d,router = %d\n",band,ESNAddress,RouterAddressFlag[routerIndex]);
				RouterAddressFlag[routerIndex - 1] = 0;
				if(children == 1)
				{
					for(i = 0;i < MAX_NODE;i++)
					{
						if((mappingTable[i]>>8) == routerIndex)
						{
							mappingTable[i] = 0;
						}
					}
				}
			}
		}
	}
	FOUT;
}

static void initBackup()
{
	int i;
	int step = 0;
	int number;
	FIN(initBackup());
	printf("initBackup\n");
	tableBackup(3,0,0,0,1);
	number = RT.number;
	for(i = 0;i<number;i += step)
	{
		step = (number - i) > MAXRTInfo ? MAXRTInfo : (number - i);
		tableBackup(0,0,0,i,step);
	}
	step = 0;
	number = Network_Msg.NetworkCT[myMACAddress].number;
	for(i = 0;i<number;i += step)
	{
		step = (number - i) > MAXCTInfo ? MAXCTInfo : (number - i);
		tableBackup(1,0,0,i,step);
	}
	FOUT;
}

static void tableBackup(int type,int addOrReduce,int NWKAddress,int start,int num)
{
	Packet* payLoad;
	int i;
	int index = 0;
	int len = 8;
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
			entry_len = 40;
			num = num > (RT.number - start)?(RT.number - start):num;
			num_max = num > MAXRTInfo ? MAXRTInfo:num;
			len += num_max * entry_len;
			pklen = len + (12 * 8);
			infoControl |= (type<<5)|(addOrReduce<<4)|(num_max);
			payLoad = op_pk_create (len);
			op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,infoControl,8);
			for(i = 0;i<num_max;i++)
			{
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,RT.table[start].destAddress,16);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,RT.table[start].nextHop,16);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,RT.table[start].status,4);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,RT.table[start].routerSeq,4);
				start++;
			}
			generatePk(pklen,0,1,2,2,myNWKAddress,backupRouter,13,0,++NWK_Seq,len/8,payLoad,To_children,6);
			printf("Node %d send a RT backup num = %d\n",myMACAddress,num_max);
			break;
	case 1://CT Backup
			entry_len = 72;
			num = num > (Network_Msg.NetworkCT[myMACAddress].number - start)?(Network_Msg.NetworkCT[myMACAddress].number - start):num;
			num_max = num > MAXCTInfo ? MAXCTInfo:num;
			number = num_max;
			for(i = 0;i<num_max;i++)
			{
				if((Network_Msg.NetworkCT[myMACAddress].table[start + i].CapabilityInformation>>6) == 3)
				{
					number -= 1;
					break;
				}
			}
			if(number == 0)	break;
			len += number * entry_len;
			pklen = len + (12 * 8);
			infoControl |= (type<<5)|(addOrReduce<<4)|(number);
			payLoad = op_pk_create (len);
			op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,infoControl,8);
			for(i = 0;i<num_max;i++)
			{
				if((Network_Msg.NetworkCT[myMACAddress].table[start + i].CapabilityInformation>>6) == 3)
				{
					start++;
					continue;
				}
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,Network_Msg.NetworkCT[myMACAddress].table[start].ESNAddress,48);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,Network_Msg.NetworkCT[myMACAddress].table[start].NWKAddress,16);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,Network_Msg.NetworkCT[myMACAddress].table[start].CapabilityInformation,8);
				start++;
			}
			generatePk(pklen,0,1,2,2,myNWKAddress,backupRouter,13,0,++NWK_Seq,len/8,payLoad,To_children,6);
			printf("Node %d send a CT backup num = %d\n",myMACAddress,num_max);
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
			for(i = 0;i<Network_Msg.NetworkCT[myMACAddress].number;i++)
			{
				if(Network_Msg.NetworkCT[myMACAddress].table[i].NWKAddress == NWKAddress)
				{
					CTIndex = i;
					entry_len += 72;
					break;
				}
			}
			len += entry_len;
			pklen = len + (12 * 8);
			infoControl |= (type<<5)|(addOrReduce<<4)|1;
			payLoad = op_pk_create (len);
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
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,Network_Msg.NetworkCT[myMACAddress].table[CTIndex].ESNAddress,48);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,Network_Msg.NetworkCT[myMACAddress].table[CTIndex].NWKAddress,16);
				op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,Network_Msg.NetworkCT[myMACAddress].table[CTIndex].CapabilityInformation,8);
			}
			generatePk(pklen,0,1,2,2,myNWKAddress,backupRouter,13,0,++NWK_Seq,len/8,payLoad,To_children,0);
			printf("Node %d send a CRT backup\n",myMACAddress);
			break;
	case 3://mainRouter's Info
			entry_len = 24;
			num_max = 1;
			len += num_max * entry_len;
			pklen = len + (12 * 8);
			infoControl |= (type<<5)|(addOrReduce<<4)|(num_max);
			band = queryFT(myFatherAddress);
			payLoad = op_pk_create (len);
			op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,infoControl,8);
			op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,myFatherAddress,16);
			op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,band,4);
			op_pk_fd_set(payLoad,++index,OPC_FIELD_TYPE_INTEGER,myPoint,4);
			generatePk(pklen,0,1,2,2,myNWKAddress,backupRouter,13,0,++NWK_Seq,len/8,payLoad,To_children,6);
			printf("Node %d send a Info backup num = %d\n",myMACAddress,num_max);
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
	void WSN_NWK (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_WSN_NWK_init (int * init_block_ptr);
	void _op_WSN_NWK_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_WSN_NWK_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_WSN_NWK_alloc (VosT_Obtype, int);
	void _op_WSN_NWK_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
WSN_NWK (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (WSN_NWK ());

		{


		FSM_ENTER ("WSN_NWK")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (0, "idle", state0_enter_exec, "WSN_NWK [idle enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (1,"WSN_NWK")


			/** state (idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (0, "idle", "WSN_NWK [idle exit execs]")


			/** state (idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("WSN_NWK [idle trans conditions]", state0_trans_conds)
			FSM_INIT_COND (STRM_DATA)
			FSM_TEST_COND (UpTimer)
			FSM_TEST_COND (STRM_MAC)
			FSM_TEST_COND (STRM_Contr)
			FSM_TEST_COND (REMOTE_PRO)
			FSM_TEST_COND (UpStatus)
			FSM_TEST_COND (UPDATERT)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("idle")
			FSM_PROFILE_SECTION_OUT (state0_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 5, state5_enter_exec, ;, "STRM_DATA", "", "idle", "From_DATA", "tr_15", "WSN_NWK [idle -> From_DATA : STRM_DATA / ]")
				FSM_CASE_TRANSIT (1, 3, state3_enter_exec, ;, "UpTimer", "", "idle", "UP_Timer", "tr_17", "WSN_NWK [idle -> UP_Timer : UpTimer / ]")
				FSM_CASE_TRANSIT (2, 4, state4_enter_exec, ;, "STRM_MAC", "", "idle", "From_MAC", "tr_23", "WSN_NWK [idle -> From_MAC : STRM_MAC / ]")
				FSM_CASE_TRANSIT (3, 6, state6_enter_exec, ;, "STRM_Contr", "", "idle", "From_Contr", "tr_26", "WSN_NWK [idle -> From_Contr : STRM_Contr / ]")
				FSM_CASE_TRANSIT (4, 7, state7_enter_exec, ;, "REMOTE_PRO", "", "idle", "remotePro", "tr_30", "WSN_NWK [idle -> remotePro : REMOTE_PRO / ]")
				FSM_CASE_TRANSIT (5, 8, state8_enter_exec, ;, "UpStatus", "", "idle", "UP_Status", "tr_32", "WSN_NWK [idle -> UP_Status : UpStatus / ]")
				FSM_CASE_TRANSIT (6, 9, state9_enter_exec, ;, "UPDATERT", "", "idle", "UpdateRTControl", "tr_34", "WSN_NWK [idle -> UpdateRTControl : UPDATERT / ]")
				FSM_CASE_TRANSIT (7, 0, state0_enter_exec, ;, "default", "", "idle", "idle", "tr_31", "WSN_NWK [idle -> idle : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (init) enter executives **/
			FSM_STATE_ENTER_FORCED (1, "init", state1_enter_exec, "WSN_NWK [init enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK [init enter execs]", state1_enter_exec)
				{
				op_intrpt_schedule_self(op_sim_time() + UPtime,intrCode_Timer);
				if(ROUTERNODE)	op_intrpt_schedule_self(op_sim_time()+RTtime,intrCode_UpRT);
				}
				FSM_PROFILE_SECTION_OUT (state1_enter_exec)

			/** state (init) exit executives **/
			FSM_STATE_EXIT_FORCED (1, "init", "WSN_NWK [init exit execs]")


			/** state (init) transition processing **/
			FSM_TRANSIT_FORCE (0, state0_enter_exec, ;, "default", "", "init", "idle", "tr_0", "WSN_NWK [init -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (INIT) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (2, "INIT", "WSN_NWK [INIT enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK [INIT enter execs]", state2_enter_exec)
				{
				int i;
				printf("start!\n");
				mn++;
				printf("mn = %d\n",mn);
				//-------------------------------My Parameters Declaration-------------------------------//
				nodeID = op_topo_parent(op_id_self());
				op_ima_obj_attr_get(nodeID,"NodeStatus",&node_status);
				op_ima_obj_attr_get(nodeID,"MACAddress",&myMACAddress);
				op_ima_obj_attr_get(nodeID,"MainRouter",&mainRouter);
				
				myCapabilityInformation = 0;
				myNWKAddress = 0;
				
				//----------------------------Dynamic Parameters Declaration-----------------------------//
				NWK_Seq = 0;
				addressdist = 0;
				myFatherAddress = 0;
				myRouterSeq = 0;
				myBand = -1;
				myPoint = -1;
				lastID = -1;
				backupRouter = 0;
				mainNWKAddress = 0;
				
				mainFather= 0;
				mainBand = -1;
				mainPoint = -1;
				
				op_ima_obj_attr_get(nodeID,"NodeType",&nodeType);
				
				sendNum = 0;
				contr_send_num=op_stat_reg("group1.pk_contr_number",OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				
				downSendNum = 0;
				down_data_send_num=op_stat_reg("group2.down_data_send_num",OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				downReceiveNum = 0;
				down_data_receive_num=op_stat_reg("group2.down_data_receive_num",OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				upSendNum = 0;
				up_data_send_num=op_stat_reg("group3.up_data_send_num",OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				upReceiveNum = 0;
				up_data_receive_num=op_stat_reg("group3.up_data_receive_num",OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				
				down_pk_loss_rate=op_stat_reg("group4.down_pk_loss_rate",OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				up_pk_loss_rate=op_stat_reg("group4.up_pk_loss_rate",OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				ete_delay_down=op_stat_reg("group4.ete delay down",OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				ete_delay_up=op_stat_reg("group4.ete delay up",OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				
				
				printf("myMACAddress = %d,myId = %d,myNodeType = %d,myNodeStatus = %d\n",myMACAddress,nodeID,nodeType,node_status);
				CRTinit();
				
				if(nodeType == 0)
				{
					myNWKAddress = 43690;
					myBand = 0;
					node_status = 1;
					for(i = 0;i<MAX_NODE;i++)
					{
						mappingTable[i] = 0;
						EndAddressFlag[i] = 0;
					}
					for(i = 0;i<MAX_RouterAddress;i++)
					{
						RouterAddressFlag[i] = 0;
					}
				}
				else if(nodeType == 1)
				{
					for(i = 0;i<MAX_CHILD;i++)
					{
						EndAddressFlag[i] = 0;
					}
				}
				for(i = 0;i<MAX_MULTICAST;i++)
				{
					myMT[i] = 0;
					childMT[i] = 0;
				}
				myCapabilityInformation |= (nodeType<<6);
				myCapabilityInformation |= (3<<4);//Power and resource allocation
				myCapabilityInformation |= (node_status<<2);
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (INIT) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "INIT", "WSN_NWK [INIT exit execs]")


			/** state (INIT) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "INIT", "init", "tr_13", "WSN_NWK [INIT -> init : default / ]")
				/*---------------------------------------------------------*/



			/** state (UP_Timer) enter executives **/
			FSM_STATE_ENTER_FORCED (3, "UP_Timer", state3_enter_exec, "WSN_NWK [UP_Timer enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK [UP_Timer enter execs]", state3_enter_exec)
				{
				UpdateRT(0);
				//UpdateCT(0);
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** state (UP_Timer) exit executives **/
			FSM_STATE_EXIT_FORCED (3, "UP_Timer", "WSN_NWK [UP_Timer exit execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK [UP_Timer exit execs]", state3_exit_exec)
				{
				op_intrpt_schedule_self(op_sim_time()+UPtime,intrCode_Timer);
				}
				FSM_PROFILE_SECTION_OUT (state3_exit_exec)


			/** state (UP_Timer) transition processing **/
			FSM_TRANSIT_FORCE (0, state0_enter_exec, ;, "default", "", "UP_Timer", "idle", "tr_18", "WSN_NWK [UP_Timer -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (From_MAC) enter executives **/
			FSM_STATE_ENTER_FORCED (4, "From_MAC", state4_enter_exec, "WSN_NWK [From_MAC enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK [From_MAC enter execs]", state4_enter_exec)
				{
				int framControl;
				int NWKControl;
				int type;
				int destMode;
				int sourceMode;
				int source;
				
				int multicast;
				
				double rcvd_power;
				double td_power;
				
				Packet* packet = op_pk_get (op_intrpt_strm());
				rcvd_power = op_td_get_dbl (packet, OPC_TDA_RA_RCVD_POWER) ;
				td_power = op_td_get_dbl (packet, OPC_TDA_RA_TX_POWER) ;
				op_pk_fd_get(packet,0,&framControl);
				op_pk_fd_get(packet,4,&source);
				op_pk_fd_get(packet,5,&NWKControl);
				multicast = 1&(framControl>>8);
				type = 15 & (NWKControl>>20);
				//printf("-------------Node %d receive a packet from MAC!,type = %d  ,td_power = %.16lf    rcvd_power = %.16lf \n",myMACAddress,type,td_power,rcvd_power);
				destMode = 7 & (framControl>>4);
				sourceMode = 7 & framControl;
				
				//Update RT with data
				if(sourceMode == 2)		UpdateRT(source);
				
				switch(type)
				{
					case 0: if(node_status == 1)	JoinReqPro(packet);
							break;
					case 1: if(nodeType != 0)	accessConfirmPro(packet,destMode);
							break;
					case 2: if(node_status == 1)	leavePro(packet);
							break;
					case 3: 
							break;
					case 4: if(BACKUPNODE)		backupPro(packet);
							break;
							break;
					case 5:	if(node_status == 1)	multicastContrPro(packet);
							//printf("Node %d NodeStatus = %d\n",myMACAddress,node_status);
							break;
					case 6:
							break;
					case 7: if(node_status == 1)	upDataPro(packet);
							//printf("Receive a data\n");
							break;
					case 8: if(node_status == 1)	downDataPro(packet,multicast);
							break;
					case 9: break;//No address conflict
					case 10:if(node_status == 1 && nodeType!=2 && nodeType!=3)	sourceSeqPro(packet);
							break;
					case 11:if(node_status == 1 && nodeType!=2 && nodeType!=3)	RoutingUPPro(packet);
							break;
					case 12:if(node_status == 1 && nodeType!=2)	mainRouterPro(packet);
							break;
					case 13:if(node_status == 1 && nodeType==3)	mainRouterInfoPro(packet);
							break;
					default:op_pk_destroy(packet);
							printf("NWK_PacketType is invalid\n");
				//}
				}
				}
				FSM_PROFILE_SECTION_OUT (state4_enter_exec)

			/** state (From_MAC) exit executives **/
			FSM_STATE_EXIT_FORCED (4, "From_MAC", "WSN_NWK [From_MAC exit execs]")


			/** state (From_MAC) transition processing **/
			FSM_TRANSIT_FORCE (0, state0_enter_exec, ;, "default", "", "From_MAC", "idle", "tr_24", "WSN_NWK [From_MAC -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (From_DATA) enter executives **/
			FSM_STATE_ENTER_FORCED (5, "From_DATA", state5_enter_exec, "WSN_NWK [From_DATA enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK [From_DATA enter execs]", state5_enter_exec)
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
				payLoad = op_pk_create(len);
				op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,data,len);
				pklen = FRAMCONTR_LEN + MACSEQ_LEN + PANID_LEN + SHORTADDR_LEN + NWKCONTR_LEN + len;
				len /=8;
				if(type == 7)
				{
					//printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Node %d get a upData node %d from data module!!!!!!!!!!!!!!!!!!",myMACAddress,dest);
					dest = GateAddress;
					destMode = 1;
					sourceMode = 2;
					generatePk(pklen,0,1,sourceMode,destMode,myNWKAddress,dest,type,MAXTTL,NWKSeq,len,payLoad,To_father,0);
					op_stat_write(up_data_send_num,++upSendNum);
				}
				else if(nodeType == 0)
				{
					//printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Node %d get a downData node %d from data module!!!!!!!!!!!!!!!!!!",myMACAddress,dest);
					dest = quiryNWKAddress(dest);
					//printf("NWKAddress %d !!!!!!!!!!!!!!!!!!\n",dest);
					if(dest != 0)
					{
						destMode = 2;
						sourceMode = 1;
						generatePk(pklen,0,1,sourceMode,destMode,myNWKAddress,dest,type,MAXTTL,NWKSeq,len,payLoad,To_children,0);
						op_stat_write(down_data_send_num,++downSendNum);
					}
				}
				
				}
				FSM_PROFILE_SECTION_OUT (state5_enter_exec)

			/** state (From_DATA) exit executives **/
			FSM_STATE_EXIT_FORCED (5, "From_DATA", "WSN_NWK [From_DATA exit execs]")


			/** state (From_DATA) transition processing **/
			FSM_TRANSIT_FORCE (0, state0_enter_exec, ;, "default", "", "From_DATA", "idle", "tr_16", "WSN_NWK [From_DATA -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (From_Contr) enter executives **/
			FSM_STATE_ENTER_FORCED (6, "From_Contr", state6_enter_exec, "WSN_NWK [From_Contr enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK [From_Contr enter execs]", state6_enter_exec)
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
				switch(type)
				{
					case 0: if(node_status == OnlineStatus)	break;
							sourceMode = 3;
							destMode = 2;
							source = myMACAddress;
							pklen += SHORTADDR_LEN + LONGADDR_LEN;
							generatePk(pklen,0,1,sourceMode,destMode,source,dest,type,0,NWKSeq,len,payLoad,To_father,0);
							op_stat_write(contr_send_num,++sendNum);
							printf("Node %d send a joinseq !!!\n",myMACAddress);
							break;
					case 1: break;
					case 2: dest = quiryNWKAddress(dest);
							if(dest == 0)	break;
							printf("Node %d send a leaveContr,dest = %d !!!\n",myMACAddress,dest);
							sourceMode = 2;
							destMode = 2;
							source = myNWKAddress;
							pklen += SHORTADDR_LEN * 2;
							op_pk_fd_get(payLoad,0,&removeChildren);
							removeChildren &= 1;
							generatePk(pklen,0,1,sourceMode,destMode,source,dest,type,MAXTTL,NWKSeq,len,payLoad,To_children,0);
							leaveNet(dest,1,removeChildren,0);
							op_stat_write(contr_send_num,++sendNum);
							break;
					case 3: sourceMode = 2;
							destMode = 1;
							source = myNWKAddress;
							pklen += SHORTADDR_LEN;
							generatePk(pklen,0,1,sourceMode,destMode,source,dest,type,MAXTTL,NWKSeq,len,payLoad,To_father,0);
							op_stat_write(contr_send_num,++sendNum);
							break;
					case 4: sourceMode = 2;
							destMode = 2;
							source = myNWKAddress;
							pklen += SHORTADDR_LEN * 2;
							generatePk(pklen,0,1,sourceMode,destMode,source,dest,type,MAXTTL,NWKSeq,len,payLoad,To_father,0);
							op_stat_write(contr_send_num,++sendNum);
							break;
					case 5: if(nodeType != 0)	break;
							sourceMode = 1;
							destMode = 2;
							source = myNWKAddress;
							pklen += SHORTADDR_LEN;
							generatePk(pklen,0,1,sourceMode,destMode,source,dest,type,MAXTTL,NWKSeq,len,payLoad,To_children,0);
							op_stat_write(contr_send_num,++sendNum);
							break;
					case 6: sourceMode = 2;
							destMode = 3;
							source = myNWKAddress;
							pklen += SHORTADDR_LEN + LONGADDR_LEN;
							generatePk(pklen,0,1,sourceMode,destMode,source,dest,type,MAXTTL,NWKSeq,len,payLoad,To_children,0);
							op_stat_write(contr_send_num,++sendNum);
							break;
					case 9: break;//No address conflict
					case 10:sourceMode = 2;
							destMode = 1;
							source = myNWKAddress;
							pklen += SHORTADDR_LEN;
							generatePk(pklen,0,1,sourceMode,destMode,source,dest,type,MAXTTL-1,NWKSeq,len,payLoad,To_father,0);
							op_stat_write(contr_send_num,++sendNum);
							break;
					case 11:sourceMode = 2;
							destMode = 1;
							source = myNWKAddress;
							pklen += SHORTADDR_LEN;
							generatePk(pklen,0,1,sourceMode,destMode,source,dest,type,MAXTTL,NWKSeq,len,payLoad,To_father,0);
							op_stat_write(contr_send_num,++sendNum);
							break;
					case 12:sourceMode = 2;
							destMode = 1;
							source = myNWKAddress;
							pklen += SHORTADDR_LEN;
							generatePk(pklen,0,1,sourceMode,destMode,source,dest,type,MAXTTL,NWKSeq,len,payLoad,To_father,0);
							op_stat_write(contr_send_num,++sendNum);
							break;
					case 13:sourceMode = 1;
							destMode = 2;
							source = myNWKAddress;
							pklen += SHORTADDR_LEN;
							generatePk(pklen,0,1,sourceMode,destMode,source,dest,type,MAXTTL,NWKSeq,len,payLoad,To_children,0);
							op_stat_write(contr_send_num,++sendNum);
							break;
					case 14:sourceMode = 2;
							destMode = 1;
							source = myNWKAddress;
							pklen += SHORTADDR_LEN;
							generatePk(pklen,0,1,sourceMode,destMode,source,dest,type,MAXTTL,NWKSeq,len,payLoad,To_father,0);
							op_stat_write(contr_send_num,++sendNum);
							break;
					default:op_pk_destroy(packet);
							printf("Contol_PacketType is invalid\n");
				}
				}
				FSM_PROFILE_SECTION_OUT (state6_enter_exec)

			/** state (From_Contr) exit executives **/
			FSM_STATE_EXIT_FORCED (6, "From_Contr", "WSN_NWK [From_Contr exit execs]")


			/** state (From_Contr) transition processing **/
			FSM_TRANSIT_FORCE (0, state0_enter_exec, ;, "default", "", "From_Contr", "idle", "tr_27", "WSN_NWK [From_Contr -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (remotePro) enter executives **/
			FSM_STATE_ENTER_FORCED (7, "remotePro", state7_enter_exec, "WSN_NWK [remotePro enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK [remotePro enter execs]", state7_enter_exec)
				{
				Packet* payLoad;
				int intrpt_code = op_intrpt_code ();
				switch(intrpt_code)
				{
					case intrCode_FatherActive: 
							op_ima_obj_attr_set(nodeID,"NodeStatus",0);
							printf("Node %d status = 0\n",myMACAddress);
							if(BACKUPNODE)
							{
								op_ima_obj_attr_set(nodeID,"NodeBand",mainBand);
								op_ima_obj_attr_set(nodeID,"NodePoint",mainPoint);
								op_ima_obj_attr_set(nodeID,"ShortAddress",mainFather);
								op_intrpt_schedule_remote(op_sim_time(), intrCode_Backup, op_id_from_name (op_topo_parent(op_id_self()) , OPC_OBJTYPE_QUEUE, "MAC")) ;
							}
							else
							{
								leaveNet(myNWKAddress,1,0,0);
							}
							break;
					case intrCode_BackupResponse:
							payLoad = op_pk_create(24);
							op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,1,8);
							op_pk_fd_set(payLoad,1,OPC_FIELD_TYPE_INTEGER,mainNWKAddress,16);
							generatePk(120,0,1,2,2,myNWKAddress,mainFather,4,0,++NWK_Seq,3,payLoad,To_father,0);
							break;
					default:printf("error intrpt_code for type of remote!\n");
				}
				}
				FSM_PROFILE_SECTION_OUT (state7_enter_exec)

			/** state (remotePro) exit executives **/
			FSM_STATE_EXIT_FORCED (7, "remotePro", "WSN_NWK [remotePro exit execs]")


			/** state (remotePro) transition processing **/
			FSM_TRANSIT_FORCE (0, state0_enter_exec, ;, "default", "", "remotePro", "idle", "tr_29", "WSN_NWK [remotePro -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (UP_Status) enter executives **/
			FSM_STATE_ENTER_FORCED (8, "UP_Status", state8_enter_exec, "WSN_NWK [UP_Status enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK [UP_Status enter execs]", state8_enter_exec)
				{
				Packet* payload;
				int pklen;
				
				printf("UpStatus! nodeType = %d,node_status = %d,mainNWKAddress = %d,myFatherAddress = %d\n",nodeType,node_status,mainNWKAddress,myFatherAddress);
				if(nodeType == 3 && node_status == 1 && mainNWKAddress != myFatherAddress)
				{
					pklen = 136;
					payload = op_pk_create(56);
					op_pk_fd_set(payload,0,OPC_FIELD_TYPE_INTEGER,1,8);
					op_pk_fd_set(payload,1,OPC_FIELD_TYPE_INTEGER,mainRouter,48);
					generatePk(pklen,0,1,2,1,myNWKAddress,GateAddress,12,MAXTTL - 1,++NWK_Seq,56,payload,To_father,0);
					printf("Node %d query mainRouter %d\n",myMACAddress,mainRouter);
					op_intrpt_schedule_self(op_sim_time()+UPStatustime,intrCode_UpStatus);
				}
				}
				FSM_PROFILE_SECTION_OUT (state8_enter_exec)

			/** state (UP_Status) exit executives **/
			FSM_STATE_EXIT_FORCED (8, "UP_Status", "WSN_NWK [UP_Status exit execs]")


			/** state (UP_Status) transition processing **/
			FSM_TRANSIT_FORCE (0, state0_enter_exec, ;, "default", "", "UP_Status", "idle", "tr_33", "WSN_NWK [UP_Status -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (UpdateRTControl) enter executives **/
			FSM_STATE_ENTER_FORCED (9, "UpdateRTControl", state9_enter_exec, "WSN_NWK [UpdateRTControl enter execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK [UpdateRTControl enter execs]", state9_enter_exec)
				{
				if(node_status == 1 && ROUTERNODE)
				{
					Packet* payLoad;
					int control = 0;
					payLoad = op_pk_create (72);
					op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,control,4);
					op_pk_fd_set(payLoad,1,OPC_FIELD_TYPE_INTEGER,myRouterSeq,4);
					op_pk_fd_set(payLoad,2,OPC_FIELD_TYPE_INTEGER,myNWKAddress,16);
					op_pk_fd_set(payLoad,3,OPC_FIELD_TYPE_INTEGER,myMACAddress,48);
					generatePk(152,0,1,2,1,myNWKAddress,GateAddress,11,MAXTTL,++NWK_Seq,9,payLoad,To_father,0);
				
					printf("Node %d send a updateRT control\n",myMACAddress);
				}
				}
				FSM_PROFILE_SECTION_OUT (state9_enter_exec)

			/** state (UpdateRTControl) exit executives **/
			FSM_STATE_EXIT_FORCED (9, "UpdateRTControl", "WSN_NWK [UpdateRTControl exit execs]")
				FSM_PROFILE_SECTION_IN ("WSN_NWK [UpdateRTControl exit execs]", state9_exit_exec)
				{
				op_intrpt_schedule_self(op_sim_time()+RTtime,intrCode_UpRT);
				}
				FSM_PROFILE_SECTION_OUT (state9_exit_exec)


			/** state (UpdateRTControl) transition processing **/
			FSM_TRANSIT_FORCE (0, state0_enter_exec, ;, "default", "", "UpdateRTControl", "idle", "tr_35", "WSN_NWK [UpdateRTControl -> idle : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (2,"WSN_NWK")
		}
	}




void
_op_WSN_NWK_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_WSN_NWK_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_WSN_NWK_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_WSN_NWK_svar function. */
#undef node_status
#undef myCapabilityInformation
#undef myNWKAddress
#undef myMACAddress
#undef myFatherAddress
#undef myBand
#undef myPoint
#undef NWK_Seq
#undef addressdist
#undef CT
#undef RT
#undef mappingTable
#undef EndAddressFlag
#undef RouterAddressFlag
#undef nodeID
#undef lastID
#undef bandTable
#undef myMT
#undef childMT
#undef nodeType
#undef Type
#undef contr_send_num
#undef sendNum
#undef down_data_send_num
#undef down_data_receive_num
#undef up_data_send_num
#undef up_data_receive_num
#undef down_pk_loss_rate
#undef up_pk_loss_rate
#undef mainRouter
#undef ete_delay_down
#undef ete_delay_up
#undef backupRouter
#undef mainNWKAddress
#undef mainFather
#undef mainBand
#undef mainPoint
#undef startRT
#undef startCT
#undef myRouterSeq

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_WSN_NWK_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_WSN_NWK_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (WSN_NWK)",
		sizeof (WSN_NWK_state));
	*init_block_ptr = 4;

	FRET (obtype)
	}

VosT_Address
_op_WSN_NWK_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	WSN_NWK_state * ptr;
	FIN_MT (_op_WSN_NWK_alloc (obtype))

	ptr = (WSN_NWK_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "WSN_NWK [INIT enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_WSN_NWK_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	WSN_NWK_state		*prs_ptr;

	FIN_MT (_op_WSN_NWK_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (WSN_NWK_state *)gen_ptr;

	if (strcmp ("node_status" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->node_status);
		FOUT
		}
	if (strcmp ("myCapabilityInformation" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->myCapabilityInformation);
		FOUT
		}
	if (strcmp ("myNWKAddress" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->myNWKAddress);
		FOUT
		}
	if (strcmp ("myMACAddress" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->myMACAddress);
		FOUT
		}
	if (strcmp ("myFatherAddress" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->myFatherAddress);
		FOUT
		}
	if (strcmp ("myBand" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->myBand);
		FOUT
		}
	if (strcmp ("myPoint" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->myPoint);
		FOUT
		}
	if (strcmp ("NWK_Seq" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->NWK_Seq);
		FOUT
		}
	if (strcmp ("addressdist" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->addressdist);
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
	if (strcmp ("mappingTable" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->mappingTable);
		FOUT
		}
	if (strcmp ("EndAddressFlag" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->EndAddressFlag);
		FOUT
		}
	if (strcmp ("RouterAddressFlag" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->RouterAddressFlag);
		FOUT
		}
	if (strcmp ("nodeID" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->nodeID);
		FOUT
		}
	if (strcmp ("lastID" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->lastID);
		FOUT
		}
	if (strcmp ("bandTable" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->bandTable);
		FOUT
		}
	if (strcmp ("myMT" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->myMT);
		FOUT
		}
	if (strcmp ("childMT" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->childMT);
		FOUT
		}
	if (strcmp ("nodeType" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->nodeType);
		FOUT
		}
	if (strcmp ("Type" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Type);
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
	if (strcmp ("down_data_send_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->down_data_send_num);
		FOUT
		}
	if (strcmp ("down_data_receive_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->down_data_receive_num);
		FOUT
		}
	if (strcmp ("up_data_send_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->up_data_send_num);
		FOUT
		}
	if (strcmp ("up_data_receive_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->up_data_receive_num);
		FOUT
		}
	if (strcmp ("down_pk_loss_rate" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->down_pk_loss_rate);
		FOUT
		}
	if (strcmp ("up_pk_loss_rate" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->up_pk_loss_rate);
		FOUT
		}
	if (strcmp ("mainRouter" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->mainRouter);
		FOUT
		}
	if (strcmp ("ete_delay_down" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ete_delay_down);
		FOUT
		}
	if (strcmp ("ete_delay_up" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ete_delay_up);
		FOUT
		}
	if (strcmp ("backupRouter" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->backupRouter);
		FOUT
		}
	if (strcmp ("mainNWKAddress" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->mainNWKAddress);
		FOUT
		}
	if (strcmp ("mainFather" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->mainFather);
		FOUT
		}
	if (strcmp ("mainBand" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->mainBand);
		FOUT
		}
	if (strcmp ("mainPoint" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->mainPoint);
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
	if (strcmp ("myRouterSeq" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->myRouterSeq);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}


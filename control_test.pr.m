MIL_3_Tfile_Hdr_ 145A 140A modeler 9 62389E8C 623B22EB 3A DESKTOP-RD4S7T2 51133 0 0 none none 0 0 none 5A4BCCF6 3499 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                         ЋЭg      @   D   H      :  2u  2y  2}  2  2  2  2  .           	   begsim intrpt             џџџџ      doc file            	nd_module      endsim intrpt             џџџџ      failure intrpts            disabled      intrpt interval         дВI­%У}џџџџ      priority              џџџџ      recovery intrpts            disabled      subqueue                     count    џџџ   
   џџџџ   
      list   	џџџ   
          
      super priority             џџџџ             int	\nodeType;       int	\myCapabilityInformation;       int	\myMACAddress;       int	\father;       int	\node_status;       int	\nodeID;       int	\contr_send_num;       int	\sendNum;       int	\result;       int	\g_test_type;       
int	\dest;       int	\status;           *   #define MUTILCAST 			3   #define LEAVE 				1   #define REJOIN		 		2               e#define Intr_Mutilcast_add			((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == addCode))   _#define Intr_Data					((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == dataCode))   h#define Intr_Mutilcast_wait			((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == startCode))   k#define Intr_Mutilcast_remove			((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == removeCode))   e#define Intr_RejoinStart			((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == startCode))   e#define Intr_Leave_Start			((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == startCode))   e#define Intr_Leave_Send				((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == leaveCode))               #define ROUTERNODE			1   #define GATENODE			0   #define ENDNODE				2   #define BACKUPNODE			3                   #define		JOINSEQ_TYPE		0   #define		LEAVE_TYPE			2   #define		MULTICAST_TYPE		5       #define		LEAVE_LEN			8   #define		MULTICASTCONTR_LEN	8   #define		DATA_LEN			10                   #define  	dataCode		0   #define		startCode		1   #define		removeCode		2   #define 	addCode			3   #define		leaveCode		4       #define		data_prid		60 * 60   i   )static void timer(double timer, int code)   {   $	FIN(timer(double timer, int code));   5	op_intrpt_schedule_self(op_sim_time() + timer,code);   	FOUT;   }           \static void contr_generate_pk(Packet* payLoad,int len,int type,int destAddress,double delay)   {   	Packet* packet;   	int pklen = len + 32;   Q	FIN(generate_pk(Packet* payLoad,int len,int type,int destAddress,double delay));   	packet = op_pk_create(0);   5	op_pk_fd_set(packet,0,OPC_FIELD_TYPE_INTEGER,len,8);   6	op_pk_fd_set(packet,1,OPC_FIELD_TYPE_INTEGER,type,8);   >	op_pk_fd_set(packet,2,OPC_FIELD_TYPE_INTEGER,destAddress,16);   :	op_pk_fd_set(packet,3,OPC_FIELD_TYPE_PACKET,payLoad,len);   $	op_pk_send_delayed(packet,0,delay);   +	//op_stat_write(contr_send_num,++sendNum);   	printf("Control send a PK\n");   R	//printf("user%d source make control packet,time=%f\n",my_address,op_sim_time());   	FOUT;   	   }   /*    *	ЗЂЫЭзщВЅЬэМгУќСюЛђзщВЅЩОГ§УќСю   #*	type = 0БэЪОЬэМг,type = 1БэЪОЩОГ§   J*	destЮЊФПЕФНкЕуЃЌnumБэЪОашвЊЬэМгЕФзщВЅзщЪ§СПЃЌarrБэЪОашвЊЬэМгЕФзщВЅзщЪ§зщ   */       Mstatic void contr_multicast_send(int type,int destAddress,int num, int arr[])   {   	Packet* payLoad;   	int multicastInfo;   	int i = 0;   1	FIN(multicastControl(int type,int destAddress));   	if(nodeType == 0)   	{    		multicastInfo = (num<<1)|type;   		payLoad = op_pk_create(0);   A		op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,multicastInfo,8);   		for(i = 0;i < num;i++)   			{   >			op_pk_fd_set(payLoad,i+1,OPC_FIELD_TYPE_INTEGER,arr[i],16);   			}   X		contr_generate_pk(payLoad,MULTICASTCONTR_LEN + (16*num),MULTICAST_TYPE,destAddress,0);   	}   	FOUT;   }       8static void generate_mutilcast_data_pk(int mutilcast_id)   {   	Packet* payLoad;   	int r;   		int len;   
	int type;   	int destAddress;   	int pklen;   3	FIN(generate_mutilcast_data_pk(int mutilcast_id));   5	op_ima_obj_attr_get(nodeID,"g_node_status",&status);   	if(status == 0) FOUT;   +	r = (int)(op_dist_uniform (DATA_LEN) + 1);   
	len = 10;   	pklen = len*8 + 32;   	if(nodeType == 0)   	{   		type = 8;   		destAddress = mutilcast_id;   	}   	else   	{	   		FOUT;   	}   	payLoad = op_pk_create(0);   7	//op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,10,8);   9	//op_pk_fd_set(payLoad,1,OPC_FIELD_TYPE_INTEGER,type,8);   A	//op_pk_fd_set(payLoad,2,OPC_FIELD_TYPE_INTEGER,destAddress,16);   8	op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,r,len*8);   0	contr_generate_pk(payLoad,len,8,destAddress,0);   	printf("Node %d source make mutilcast data packet data = %d len = %d, dest = %d,time=%f\n",myMACAddress, r, len, destAddress, op_sim_time());   	   	FOUT;   	   }           /*   *destAddressФПЕФЕижЗ, rejoin:0ВЛжиаТШыЭј 1бЁдёаТЕФИИНкЕужиаТШыЭј 2ЭЈЙ§ЕБЧАИИНкЕужиаТШыЭј replace:жЛгаrejoinзжЖЮВЛЮЊ0ЪБВХгааЇ 0БэЪОБЃСєОЩзЪдД 1 жиаТЗжХфзЪдД   *   */   Bstatic void leaveControl(int destAddress, int rejoin, int replace)   {   	Packet* payLoad;   	int leaveInfo;   $	FIN(leaveControl(int destAddress));   	if(nodeType == 0)   	{   +		leaveInfo = 2 | rejoin<<2 | replace << 4;   		payLoad = op_pk_create(0);   E		op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,leaveInfo,LEAVE_LEN);   @		contr_generate_pk(payLoad,LEAVE_LEN,LEAVE_TYPE,destAddress,0);   	}   	FOUT;   }                                                     
   init   
       
      &nodeID = op_topo_parent(op_id_self());   4op_ima_obj_attr_get(nodeID,"g_node_type",&nodeType);   ?op_ima_obj_attr_get(nodeID,"g_node_ESN_address",&myMACAddress);   7op_ima_obj_attr_get(nodeID,"g_test_type",&g_test_type);   //printf("control init\n");       
                     
   џџџџ   
          pr_state        В            
   mutilcast_idle   
       
      printf("mutilcast idle\n");   
       
       
           џџџџ             pr_state        В             
   send_muilticast_add   
       
      int arr[] = {1, 2};   "printf("send a add mutilcast!\n");   %contr_multicast_send(1,dest ,2, arr);   
                     
   џџџџ   
          pr_state        ў            
   mutilcast_init   
       
   	    //printf("in mutilcast init\n");   if(nodeType == GATENODE) {   	result = 1;   	dest = 512;   }       else{   	result = 0;   }   
                         џџџџ             pr_state        ў  Т          
   
leave_init   
       
      if(nodeType == GATENODE) {   	result = 1;   
	dest = 2;   }       else{   	result = 0;   }   
                         џџџџ             pr_state        В  Т          
   
leave_idle   
                     
      result = 0;   
           џџџџ             pr_state        В  :          
   
send_leave   
       
      "printf("send a leave control!\n");   leaveControl(dest, 0, 0);   
                     
   џџџџ   
          pr_state        *             
   send_muilticast_remove   
       
      int arr[] = {1, 2};   %contr_multicast_send(0,dest ,2, arr);   
                     
   џџџџ   
          pr_state        *  J          
   	send_data   
       
      generate_mutilcast_data_pk(2);   
       
      6op_intrpt_schedule_self(op_sim_time()+180,removeCode);   
       
   џџџџ   
          pr_state      	  ў  v          
   rejoin   
       
      if(nodeType == GATENODE) {   	result = 1;   /	op_intrpt_schedule_self(op_sim_time()+1600,0);   }       else{   	result = 0;   }   
                     
    џџџџ   
          pr_state      
  В  В          
   rejoin_idle   
                     
      result = 0;   
           џџџџ             pr_state        В  *          
   send_leave_rejoin   
       
      #printf("send a rejoin control!\n");   leaveControl(512, 1, 1);   
                     
   џџџџ   
          pr_state          :          
   idle   
                                       џџџџ             pr_state                            М    г    ј   §  љ     Р            
   tr_1   
       
   default   
       џџџџ          
    џџџџ   
          џџџџ                       pr_transition              y   й     Ѕ    Џ   Ѕ          
   tr_2   
       
   Intr_Mutilcast_add   
       
   timer(data_prid, dataCode)   
       
    џџџџ   
          џџџџ                       pr_transition               k  ?         №            
   tr_3   
       
   g_test_type == MUTILCAST   
       
   timer(1800,startCode);   
       
    џџџџ   
          џџџџ                       pr_transition              T   ќ       
  Ј  
          
   tr_4   
       
   "result == 1 && Intr_Mutilcast_wait   
       
   timer(0, addCode)   
       
    џџџџ   
          џџџџ                       pr_transition                          ѓ  П          
   tr_5   
       
   g_test_type == LEAVE   
       
   timer(1800,startCode);   
       
    џџџџ   
          џџџџ                       pr_transition              T  Я       У  З  Т          
   tr_6   
       
   result == 1 && Intr_Leave_Start   
       
   timer(0, leaveCode)   
       
    џџџџ   
          џџџџ                       pr_transition              
  Т     Р  К  ї    љ  п  И  У          
   tr_7   
       
   default   
       џџџџ          
    џџџџ   
          џџџџ                       pr_transition                     Е  б  Б  8          
   tr_8   
       
   Intr_Leave_Send   
       џџџџ          
    џџџџ   
          џџџџ                       pr_transition      	        Р  џ     П  -  Р  а          
   tr_9   
       џџџџ          џџџџ          
    џџџџ   
          џџџџ                       pr_transition      
        П   к     Р   Ћ  Н  	          
   tr_10   
       џџџџ          џџџџ          
    џџџџ   
          џџџџ                       pr_transition              ѕ   ж     М       Ђ          
   tr_11   
       
   Intr_Mutilcast_remove   
       џџџџ          
    џџџџ   
          џџџџ                       pr_transition              љ   з     4   Ѓ  О            
   tr_12   
       џџџџ          џџџџ          
    џџџџ   
          џџџџ                       pr_transition              ѕ  2     О    +  L          
   tr_13   
       
   	Intr_Data   
       џџџџ          
    џџџџ   
          џџџџ                       pr_transition              р  2       P  Ў            
   tr_14   
       џџџџ          џџџџ          
    џџџџ   
          џџџџ                       pr_transition             	             ђ  k          
   tr_15   
       
   g_test_type == REJOIN   
       
   timer(1800,startCode);   
       
    џџџџ   
          џџџџ                       pr_transition         
   
  м       Р  Њ  ї    љ  Я  И  Г          
   tr_16   
       
   default   
       џџџџ          
    џџџџ   
          џџџџ                       pr_transition         	   
  \         w  Ј  ­          
   tr_17   
       
   result == 1 && Intr_Leave_Start   
       џџџџ          
    џџџџ   
          џџџџ                       pr_transition         
       ѓ     А  М  Г            
   tr_18   
       
   Intr_Leave_Send   
       
   timer(0, leaveCode)   
       
    џџџџ   
          џџџџ                       pr_transition                 у           4          
   tr_19   
       
   default   
       џџџџ          
    џџџџ   
          џџџџ                       pr_transition               Н  ?       8   в  -   Ш  T    ?          
   tr_20   
       
   default   
       џџџџ          
    џџџџ   
          џџџџ                       pr_transition            
  Р  ш     Т    Н  И          
   tr_21   
       џџџџ          џџџџ          
    џџџџ   
          џџџџ                       pr_transition              ц   У        џ  в   ж     Щ  
   џ          
   tr_22   
       
   default   
       џџџџ          
    џџџџ   
          џџџџ                       pr_transition              щ       ђ  Е  р  }      џ  Ў          
   tr_23   
       
   default   
       џџџџ          
    џџџџ   
          џџџџ                       pr_transition         	   	  э  V     ђ  h  ш  C    9    f          
   tr_24   
       
   default   
       џџџџ          
    џџџџ   
          џџџџ                       pr_transition                                             
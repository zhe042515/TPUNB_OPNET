MIL_3_Tfile_Hdr_ 145A 140A modeler 9 613EB494 621F5F39 BA DESKTOP-RD4S7T2 51133 0 0 none none 0 0 none 466A3164 23F2 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                         ��g�      @   D   H      v  !m  !q  !�  !�  !�  !�  !�  j           	   begsim intrpt             ����      doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����             int	\nodeType;       int	\myCapabilityInformation;       int	\myMACAddress;       int	\father;       int	\node_status;       int	\nodeID;       Stathandle	\contr_send_num;       int	\sendNum;                  ;#define 	REMOTE_PRO	(op_intrpt_type() == OPC_INTRPT_REMOTE)   #define 	intrCode_Father		4   #define		intrCode_MainRouter 8       #define		MAX_PROBABILITY		3000   #define		REJOIN_PROBABILITY	2   #define		LEAVE_PROBABILITY	4       #define		JOINSEQ_TYPE		0   #define		LEAVE_TYPE			2   #define		MULTICAST_TYPE		5       #define		JOINSEQ_LEN			56   #define		LEAVE_LEN			8   #define		MULTICASTCONTR_LEN	40           #define		JOIN				1   #define		REJOIN				2   #define		LEAVE				3           extern int GateAddress;       Pstatic void generate_pk(Packet* payLoad,int len,int type,int dest,double delay);   static void JoinReq();   0static void multicastControl(int type,int dest);   #static void leaveControl(int dest);   N   Ostatic void generate_pk(Packet* payLoad,int len,int type,int dest,double delay)   {   	Packet* packet;   	int pklen = len + 32;   J	FIN(generate_pk(Packet* payLoad,int len,int type,int dest,double delay));   	packet = op_pk_create(0);   5	op_pk_fd_set(packet,0,OPC_FIELD_TYPE_INTEGER,len,8);   6	op_pk_fd_set(packet,1,OPC_FIELD_TYPE_INTEGER,type,8);   7	op_pk_fd_set(packet,2,OPC_FIELD_TYPE_INTEGER,dest,16);   :	op_pk_fd_set(packet,3,OPC_FIELD_TYPE_PACKET,payLoad,len);   $	op_pk_send_delayed(packet,0,delay);   +	//op_stat_write(contr_send_num,++sendNum);   	printf("Control send a PK\n");   R	//printf("user%d source make control packet,time=%f\n",my_address,op_sim_time());   	FOUT;   	   }       static void JoinReq()   {   	Packet* payLoad;   	double delay;   	FIN(JoinReq());   	delay = op_dist_uniform (2.0);   7	op_ima_obj_attr_get(nodeID,"NodeStatus",&node_status);   =	if(node_status == 0 || node_status == 2 || node_status == 4)   	{   /		op_ima_obj_attr_get(nodeID,"father",&father);   		payLoad = op_pk_create(0);   A		op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,myMACAddress,48);   K		op_pk_fd_set(payLoad,1,OPC_FIELD_TYPE_INTEGER,myCapabilityInformation,8);   =		generate_pk(payLoad,JOINSEQ_LEN,JOINSEQ_TYPE,father,delay);   3		printf("Node %d build a joinreq\n",myMACAddress);   	}   	FOUT;   }       /static void multicastControl(int type,int dest)   {   	Packet* payLoad;   	int multicastInfo;   *	FIN(multicastControl(int type,int dest));   	if(nodeType == 0)   	{   		multicastInfo = 4|type;   		payLoad = op_pk_create(0);   A		op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,multicastInfo,8);   6		op_pk_fd_set(payLoad,1,OPC_FIELD_TYPE_INTEGER,1,16);   6		op_pk_fd_set(payLoad,2,OPC_FIELD_TYPE_INTEGER,2,16);   @		generate_pk(payLoad,MULTICASTCONTR_LEN,MULTICAST_TYPE,dest,0);   		printf("&&&&&\n");   	}   	FOUT;   }       "static void leaveControl(int dest)   {   	Packet* payLoad;   	int leaveInfo;   	FIN(leaveControl(int dest));   	if(nodeType == 0)   	{   		leaveInfo = 26;   		payLoad = op_pk_create(0);   E		op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,leaveInfo,LEAVE_LEN);   3		generate_pk(payLoad,LEAVE_LEN,LEAVE_TYPE,dest,0);   	}   	/*   	else   	{   		leaveInfo = 0;   		payLoad = op_pk_create(0);   E		op_pk_fd_set(payLoad,0,OPC_FIELD_TYPE_INTEGER,leaveInfo,LEAVE_LEN);   3		generate_pk(payLoad,LEAVE_LEN,LEAVE_TYPE,dest,0);   	}   	*/   	FOUT;   }                                         �            
   init   
       J      &nodeID = op_topo_parent(op_id_self());   4op_ima_obj_attr_get(nodeID,"g_node_type",&nodeType);   ?op_ima_obj_attr_get(nodeID,"g_node_ESN_address",&myMACAddress);       if(nodeType != 0)   {   Z	//op_intrpt_schedule_self(op_sim_time()+op_dist_uniform (20.0)+op_dist_uniform (40.0),0);   }   else   {	   	printf("=================");   1	//op_intrpt_schedule_self(op_sim_time()+1600,1);   1	//op_intrpt_schedule_self(op_sim_time()+2000,2);   	   1	//op_intrpt_schedule_self(op_sim_time()+3000,3);   1	//op_intrpt_schedule_self(op_sim_time()+1600,4);   }       J                     
   ����   
          pr_state        v            
   idle   
       
       
                         ����             pr_state        �  �          
   Joinreq   
       
      6op_ima_obj_attr_get(nodeID,"NodeStatus",&node_status);   if(node_status==0)   {   	JoinReq();   /	op_intrpt_schedule_self(op_sim_time()+20.0,0);   }   
                     
   ����   
          pr_state        �  �          
   MulticastContr_add   
       
      7printf("Node %d send a mutilcastContr\n",myMACAddress);   multicastControl(1,512);   
                     
   ����   
          pr_state        �   Z          
   MulticastContr_subtract   
       
      7printf("Node %d send a mutilcastContr\n",myMACAddress);   multicastControl(0,512);   
                     
   ����   
          pr_state        �   Z          
   	RemotePro   
       
      $int intrpt_code = op_intrpt_code ();   :printf("Node %d control receive a remote\n",myMACAddress);   switch(intrpt_code)   {   "	case intrCode_Father : JoinReq();   				break;   	case intrCode_MainRouter :	   (			//myCapabilityInformation &= ~(3<<6);   ,			myCapabilityInformation |= (nodeType<<6);   			JoinReq();   				break;   C	default:printf("Control error intrpt_code for type of remote!\n");   }   
                     
   ����   
          pr_state        v  �          
   Leave   
       
      "printf("send a leave control!\n");   leaveControl(2);   
                     
   ����   
          pr_state                       K           }            
   tr_0   
       ����          ����          
    ����   
          ����                       pr_transition              �  $     �    �   �  �  6  {  
          
   tr_1   
       
   default   
       ����          
    ����   
          ����                       pr_transition              �  �     z    �  �          
   tr_5   
       
   8op_intrpt_type()==OPC_INTRPT_SELF && op_intrpt_code()==0   
       ����          
    ����   
          ����                       pr_transition              k  �     |    �  �          
   tr_6   
       
   8op_intrpt_type()==OPC_INTRPT_SELF && op_intrpt_code()==1   
       ����          
    ����   
          ����                       pr_transition              H  l       �  �            
   tr_7   
       ����          ����          
    ����   
          ����                       pr_transition      	        �  g     �  �  �            
   tr_9   
       ����          ����          
    ����   
          ����                       pr_transition      
        �   �     |    �   b          
   tr_10   
       
   8op_intrpt_type()==OPC_INTRPT_SELF && op_intrpt_code()==2   
       ����          
    ����   
          ����                       pr_transition              2   �     �   ]  l            
   tr_11   
       ����          ����          
    ����   
          ����                       pr_transition              �   �     �   X  p            
   tr_12   
       ����          ����          
    ����   
          ����                       pr_transition              �   �     �    �   \          
   tr_13   
       
   
REMOTE_PRO   
       ����          
    ����   
          ����                       pr_transition              h  k     j    f  �          
   tr_16   
       
   8op_intrpt_type()==OPC_INTRPT_SELF && op_intrpt_code()==4   
       ����          
    ����   
          ����                       pr_transition              �  g     �  �  z            
   tr_17   
       ����          ����          
    ����   
          ����                       pr_transition                   pk_contr_number           group1   bucket/default total/max value   linear        ԲI�%��}                            
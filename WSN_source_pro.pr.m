MIL_3_Tfile_Hdr_ 145A 140A modeler 9 613B5356 628B4F5A 13A DESKTOP-RD4S7T2 51133 0 0 none none 0 0 none DC98E645 2514 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                        Ф═gЅ      @   D   H      H  "­  "З  "Э  "Ч  #  #  #  <           	   begsim intrpt         
          
   doc file            	nd_module      endsim intrpt                       failure intrpts            disabled      intrpt interval         н▓IГ%ћ├}          priority                        recovery intrpts            disabled      subqueue                     count          
          
      list   	      
          
      super priority                              int	\TIME_OUT;       double	\time_out;       double	\BURST_PROBABILITY;       int	\nodeType;       int	\node_status;       int	\nodeID;       int	\myMACAddress;       int	\g_data_mode;              "#define		Poisson 			g_data_mode==1   ##define		Discrete 			g_data_mode==2   $#define		Concentrat			g_data_mode==3           #define		DATA_LEN			10   !#define		DATA_PERIOD_P		10*MINUTE   !#define		DATA_PERIOD_D		10*MINUTE   #define		DATA_PERIOD_C		1*HOUR   ##define		DATA_PERIOD_down	10*MINUTE       #define		SECOND				1   #define		MINUTE				60*SECOND   #define		HOUR				60*MINUTE           #define		INTRPT_PORT			1   #define		GATE_ADRESS			43690   #define		NODENUMBER			200       j#define INTRPT_SELF_INIT				((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == intrCode_Init))   h#define INTRPT_SELF_SEND				((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == INTRPT_PORT))   #define intrCode_Init				10       int	DATA_TYPE_BURST	= 4;       extern int GateAddress;       #static void generate_pk(int delay);   static void nwk_delay_start();   :   "static void generate_pk(int delay)   {   	Packet* packet;   	int r;   		int len;   
	int type;   
	int dest;   	int pklen;   	int status;   	OpT_Packet_Id pk_id;   	FIN(generate_pk(int delay));   5	op_ima_obj_attr_get(nodeID,"g_node_status",&status);   	if(status == 0) FOUT;   +	r = (int)(op_dist_uniform (DATA_LEN) + 1);   
	len = 10;   	pklen = len*8 + 32;   	if(nodeType == 0)   	{   		type = 8;   4		dest = (int) op_dist_uniform (NODENUMBER - 1) + 1;   ,		//dest = (int) (op_dist_uniform (50) + 1);   		//dest = 80;   			//FOUT;   	}   	else   	{	   		type = 7;   		dest = GATE_ADRESS;   			//FOUT;   	}   	packet = op_pk_create(0);   4	op_pk_fd_set(packet,0,OPC_FIELD_TYPE_INTEGER,10,8);   6	op_pk_fd_set(packet,1,OPC_FIELD_TYPE_INTEGER,type,8);   7	op_pk_fd_set(packet,2,OPC_FIELD_TYPE_INTEGER,dest,16);   7	op_pk_fd_set(packet,3,OPC_FIELD_TYPE_INTEGER,r,len*8);   	pk_id = op_pk_id(packet);   !	op_pk_send_delayed(packet,0, 0);   Ї	printf("Node %d source make data packet data = %d len = %d, dest = %d,time=%f, id = %d\n",myMACAddress, r, len, dest, op_sim_time(), pk_id);   	   	FOUT;   	   }       static void delay_start()   {   	FIN(delay_start());   O	op_intrpt_schedule_self(op_sim_time() + op_dist_uniform(60*10),intrCode_Init);   8	//op_intrpt_schedule_self(op_sim_time(),intrCode_Init);   	FOUT;   }       static void send_intrpt()   {   	FIN(send_intrpt());   4	op_intrpt_schedule_self(op_sim_time(),INTRPT_PORT);   	FOUT;   }                                       	      J  J          
   init   
       
   
   &nodeID = op_topo_parent(op_id_self());   &//printf("Data nodeID = %d\n",nodeID);   4op_ima_obj_attr_get(nodeID,"g_node_type",&nodeType);   ?op_ima_obj_attr_get(nodeID,"g_node_ESN_address",&myMACAddress);   <op_ima_obj_attr_get(nodeID,"g_node_data_mode",&g_data_mode);       Wif(nodeType == 0)	op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD_down, INTRPT_PORT);   _//else op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD + op_dist_uniform (300), INTRPT_PORT);   2//printf("Node %d source start!\n", myMACAddress);       
                     
          
          pr_state        v   м          
   make_poison   
       
       
                                          pr_state        v   Z          
   data_poison   
       
      ,if(op_dist_uniform(1) < 0.1) generate_pk(0);           
       
      X//op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD + op_dist_uniform (60),INTRPT_PORT);           Wif(nodeType == 0)	op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD_down, INTRPT_PORT);   \else op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD_P, INTRPT_PORT); //▓┤╦╔ио▓╝╗Ы└в╔бЙщн╚   
       
          
          pr_state        *  є          
   make_discrete   
       
       
                                          pr_state        *            
   data_discrete   
       
      '//printf("data send a data to NWK!\n");   generate_pk(0);               
       
      X//op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD + op_dist_uniform (60),INTRPT_PORT);           Wif(nodeType == 0)	op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD_down, INTRPT_PORT);   \else op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD_D, INTRPT_PORT); //▓┤╦╔ио▓╝╗Ы└в╔бЙщн╚   
       
          
          pr_state        v  є          
   delay_discrete   
                                                        pr_state        v  v          
   make_concentrat   
       
       
                                          pr_state        v  ■          
   data_concentrat   
       
      '//printf("data send a data to NWK!\n");   !generate_pk(op_dist_uniform(60));           
       
      X//op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD + op_dist_uniform (60),INTRPT_PORT);       Wif(nodeType == 0)	op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD_down, INTRPT_PORT);   ^else op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD_C + op_dist_uniform (60), INTRPT_PORT);   
       
          
          pr_state      	  ┬  :          J   nothing   J                                                        pr_state                     f   Ќ     l   ─  k   Z          
   tr_3   
       
   INTRPT_SELF_SEND   
                     
           
                                     pr_transition              ђ   џ     Ђ   Y  }   ┘          
   tr_4   
                                   
           
                                     pr_transition      	           C        x              
   tr_8   
       
   INTRPT_SELF_SEND   
                     
           
                                     pr_transition      
        3  M     5    1  Ї          
   tr_9   
                                   
           
                                     pr_transition              ═  а     ѕ  Ѕ  0  Ё          
   tr_10   
       
   INTRPT_SELF_INIT   
       
   send_intrpt()   
       
           
                                     pr_transition              e  :     l  h  k  ■          
   tr_14   
       
   INTRPT_SELF_SEND   
                     
           
                                     pr_transition              ђ  =     Ђ  §  }  }          
   tr_15   
                                   
           
                                     pr_transition               ▀  ^     G  L  w  ѕ          
   tr_26   
       
   Discrete   
       
   delay_start()   
       
           
                                     pr_transition               С       T  G  t   о          
   tr_25   
       
   Poisson   
       
   send_intrpt()   
       
           
                                     pr_transition               з  М     G  Q  o  z          
   tr_27   
       J   
Concentrat   J       
   send_intrpt()   
       
           
                                     pr_transition              ┬  |     ё  ё  е  ј  Ф  d  і  q          
   tr_12   
       
   default   
                     
           
                                     pr_transition              ▓   ═     ѕ   Л  е   Й  Џ   О  w   М          
   tr_1   
       
   default   
                     
           
                                     pr_transition               m  є     ;  Ђ  Y  s  S  Њ  -  Ѕ          
   tr_2   
       
   default   
                     
           
                                     pr_transition      !        Y  X     m  z  ]  c  б  Y  |  u          
   tr_11   
       
   default   
                     
           
                                     pr_transition      "       	  ѓ  Л     G  [  й  F          J   tr_34   J       J   default   J                     J           J                                     pr_transition      #   	   	  Б  Y     └  D  Ё  n  н  n  ┬  =          J   tr_35   J       J   default   J                     J           J                                     pr_transition      
   $                                    
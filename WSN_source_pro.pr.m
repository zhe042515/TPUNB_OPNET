MIL_3_Tfile_Hdr_ 145A 140A modeler 9 613B5356 6269F4DE 12E DESKTOP-RD4S7T2 51133 0 0 none none 0 0 none 684DA75F 167E 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                        Ф═gЅ      @   D   H      
   Z  ^  b  f  r  v  z  	З           	   begsim intrpt         
          
   doc file            	nd_module      endsim intrpt                       failure intrpts            disabled      intrpt interval         н▓IГ%ћ├}          priority                        recovery intrpts            disabled      subqueue                     count          
          
      list   	      
          
      super priority                              int	\TIME_OUT;       double	\time_out;       double	\BURST_PROBABILITY;       int	\nodeType;       int	\node_status;       int	\nodeID;       int	\myMACAddress;              #define		DATA_LEN			10   #define		DATA_PERIOD			1*60*60   !#define		DATA_PERIOD_down	1*10*60   #define		INTRPT_PORT			1   #define		GATE_ADRESS			43690   #define		NODENUMBER			200       j#define INTRPT_SELF_INIT				((op_intrpt_type() == OPC_INTRPT_SELF) && (op_intrpt_code() == intrCode_Init))   #define intrCode_Init				10       int	DATA_TYPE_BURST	= 4;       extern int GateAddress;       static void generate_pk();   static void nwk_delay_start();   3   static void generate_pk()   {   	Packet* packet;   	int r;   		int len;   
	int type;   
	int dest;   	int pklen;   	int status;   	OpT_Packet_Id pk_id;   	FIN(generate_pk());   5	op_ima_obj_attr_get(nodeID,"g_node_status",&status);   	if(status == 0) FOUT;   +	r = (int)(op_dist_uniform (DATA_LEN) + 1);   
	len = 10;   	pklen = len*8 + 32;   	if(nodeType == 0)   	{   		type = 8;   4		dest = (int) op_dist_uniform (NODENUMBER - 1) + 1;   ,		//dest = (int) (op_dist_uniform (50) + 1);   		//dest = 80;   			//FOUT;   	}   	else   	{	   		type = 7;   		dest = GATE_ADRESS;   			//FOUT;   	}   	packet = op_pk_create(0);   4	op_pk_fd_set(packet,0,OPC_FIELD_TYPE_INTEGER,10,8);   6	op_pk_fd_set(packet,1,OPC_FIELD_TYPE_INTEGER,type,8);   7	op_pk_fd_set(packet,2,OPC_FIELD_TYPE_INTEGER,dest,16);   7	op_pk_fd_set(packet,3,OPC_FIELD_TYPE_INTEGER,r,len*8);   	pk_id = op_pk_id(packet);   !	op_pk_send_delayed(packet,0, 0);   Ї	printf("Node %d source make data packet data = %d len = %d, dest = %d,time=%f, id = %d\n",myMACAddress, r, len, dest, op_sim_time(), pk_id);   	   	FOUT;   	   }       static void nwk_delay_start()   {   	FIN(nwk_delay_start());   Q	//op_intrpt_schedule_self(op_sim_time() + op_dist_uniform(60*10),intrCode_Init);   6	op_intrpt_schedule_self(op_sim_time(),intrCode_Init);   	FOUT;   }                                              ќ   м          
   init   
       
   	   &nodeID = op_topo_parent(op_id_self());   &//printf("Data nodeID = %d\n",nodeID);   4op_ima_obj_attr_get(nodeID,"g_node_type",&nodeType);   ?op_ima_obj_attr_get(nodeID,"g_node_ESN_address",&myMACAddress);       Wif(nodeType == 0)	op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD_down, INTRPT_PORT);   ]else op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD + op_dist_uniform (300), INTRPT_PORT);   2//printf("Node %d source start!\n", myMACAddress);       
                     
          
          pr_state        ■   м          
   make   
       
       
                                          pr_state        ■  є          
   data   
       
      '//printf("data send a data to NWK!\n");   generate_pk();       -//if(op_dist_uniform(1) < 0.1) generate_pk();           
       
      X//op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD + op_dist_uniform (60),INTRPT_PORT);           Wif(nodeType == 0)	op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD_down, INTRPT_PORT);   \else op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD + op_dist_uniform (60), INTRPT_PORT);       \//else op_intrpt_schedule_self(op_sim_time()+DATA_PERIOD, INTRPT_PORT); //▓┤╦╔ио▓╝╗Ы└в╔бЙщн╚   
       
          
          pr_state        J   м          
   delay   
                                                        pr_state                     {   ┐        ┬  W   ќ  l  '  ь   ╚          
   tr_1   
       
   default   
                     
           
                                     pr_transition              ║  B     З   ─  з  є          
   tr_3   
       
   Bop_intrpt_type()==OPC_INTRPT_SELF && op_intrpt_code()==INTRPT_PORT   
                     
           
                                     pr_transition                /     	  Ё     ┘          
   tr_4   
                                   
           
                                     pr_transition                Ы   к      ю   Н  O   о          
   tr_5   
                     
   nwk_delay_start()   
       
           
                                     pr_transition              а   ╚     \   Н     Л          
   tr_6   
       
   INTRPT_SELF_INIT   
                     
           
                                     pr_transition              T   v     ?   ╦     Љ  є   ї  a   Й  T   ╦          
   tr_7   
       
   default   
                     
           
                                     pr_transition                                             
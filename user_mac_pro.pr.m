MIL_3_Tfile_Hdr_ 145A 140A modeler 9 60585136 60589890 17 DESKTOP-RD4S7T2 51133 0 0 none none 0 0 none DC67544C 12C3 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                         ��g�      @   D   H      A  �  �  �  �  �  �  �  5           	   begsim intrpt         
   ����   
   doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����             Packet *	\mypacket;       int	\frame_type;       Boolean	\from_source;       Boolean	\from_receiver;       int	\receive_address;       int	\user_mac_id;       int	\user_node_id;       int	\ii;       char	\name[7];       int	\my_address;                                                          �   �          
   init   
       
      user_mac_id=op_id_self();   )user_node_id=op_topo_parent(user_mac_id);   for(ii=0;ii<7;ii++)   {    	name[ii]=0;   }   *op_ima_obj_hname_get(user_node_id,name,7);   my_address=0;   for(ii=0;ii<7;ii++)   {    "	if(name[ii]>=48 && name[ii]<=57)    	{    )		my_address = my_address*10+name[ii]-48;   	}   }   7//printf("user%d mac initialize succeed\n",my_address);   
                     
   ����   
          pr_state        J   �          
   
get_packet   
                     
      %if(op_intrpt_type()==OPC_INTRPT_STRM)   {    )	mypacket = op_pk_get (op_intrpt_strm());   	if(op_intrpt_strm()==0)    	{    		from_receiver =OPC_FALSE;     		from_source = OPC_TRUE;     	} else if(op_intrpt_strm()==1)    	{    2		printf("user%d received a stream\n",my_address);   		from_receiver = OPC_TRUE;     		from_source = OPC_FALSE;    =		op_pk_nfd_get(mypacket,"receive_address",&receive_address);   "		if(receive_address!=my_address)    		{    			op_pk_destroy(mypacket);   			from_receiver = OPC_FALSE;     			from_source = OPC_FALSE;    		}	   		else   		{   3			printf("user%d received a packet\n",my_address);   		}   	}   }   
           ����             pr_state        J   Z          
   uplink   
       J      1op_pk_nfd_get(mypacket,"frame_type",&frame_type);   %printf("frame_type=%d\n",frame_type);   if(frame_type==2)   {    	op_pk_destroy (mypacket);    Y	printf("user%d receive ack packet and destroy it,time = %f\n",my_address,op_sim_time());   }   if(frame_type==2)   {    %	//printf("user is ready to send\n");   S	int user_source_id = op_id_from_name(user_node_id,OPC_OBJTYPE_PROC,"user_source");   :	Prohandle user_source_pro = op_pro_root(user_source_id);    ?	op_intrpt_schedule_process(user_source_pro,op_sim_time()+5,1);   #	printf("user is ready to send\n");   }   J                     
   ����   
          pr_state        J  J          
   downlink   
       
      1op_pk_nfd_get(mypacket,"frame_type",&frame_type);   if(frame_type==1){    	op_pk_send (mypacket,0);   j	printf("user%d mac receive data packet from source and send to ap,time = %f\n",my_address,op_sim_time());   }   
                     
   ����   
          pr_state                        �   �      �   �  N   �          
   tr_0   
       ����          ����          
    ����   
          ����                       pr_transition                 �     :   �  C   b          
   tr_1   
       
   from_receiver   
       ����          
    ����   
          ����                       pr_transition              V   �     V   `  V   �          
   tr_2   
       ����          ����          
    ����   
          ����                       pr_transition                
     >   �  <  >          
   tr_4   
       
   from_source   
       ����          
    ����   
          ����                       pr_transition              N       M  =  O   �          
   tr_5   
       ����          ����          
    ����   
          ����                       pr_transition              �   �     U   �  �   �  �    N   �          
   tr_6   
       
   default   
       ����          
    ����   
          ����                       pr_transition                                             
MIL_3_Tfile_Hdr_ 145A 140A modeler 9 60584DCB 6058927C 9 DESKTOP-RD4S7T2 51133 0 0 none none 0 0 none DF58DD25 10AE 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                          ��g�      @   D   H      A  �  �  �  �  �  �  �  5           	   begsim intrpt         
   ����   
   doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����             Packet *	\mypacket;       int	\my_address;       int	\ap_mac_id;       int	\ap_node_id;       int	\ii;       Boolean	\from_source;       Boolean	\from_receiver;       int	\send_address;       int	\receive_address;       int	\frame_type;                                                         J   �          
   ap_mac   
       
       
       
      %if(op_intrpt_type()==OPC_INTRPT_STRM)   {    	printf("AP received a packet");   )	mypacket = op_pk_get (op_intrpt_strm());   	if(op_intrpt_strm()==0)   	{    		from_receiver = OPC_FALSE;    		from_source = OPC_TRUE;     	} else if(op_intrpt_strm()==1)    	{    		from_receiver = OPC_TRUE;     		from_source = OPC_FALSE;   >		op_pk_nfd_get(mypacket,"receive_address",&receive_address);    !		if(receive_address!=my_address)   		{    			op_pk_destroy(mypacket);   			from_receiver = OPC_FALSE;    			from_source = OPC_FALSE;    		}    	}   }   
           ����             pr_state        J   Z          
   send   
                                   
   ����   
          pr_state         �   �          
   init   
       
      ap_mac_id=op_id_self();   %ap_node_id=op_topo_parent(ap_mac_id);   <op_ima_obj_attr_get (ap_node_id, "ap_address", &my_address);   @//printf("ap mac initialize succeed,address = %d\n",my_address);   
                     
   ����   
          pr_state        J  J          
   receive   
       
      1op_pk_nfd_get(mypacket,"frame_type",&frame_type);   5op_pk_nfd_get(mypacket,"send_address",&send_address);   if(frame_type==1)   {   	op_pk_send(mypacket,1);   y	printf("ap mac receive data packet and send it to sink,time = %f\n",op_sim_time()); mypacket = op_pk_create_fmt("ack");    *	op_pk_nfd_set(mypacket, "frame_type", 2);   ;	op_pk_nfd_set(mypacket, "receive_address", send_address);    5	op_pk_nfd_set(mypacket, "send_address", my_address);   	op_pk_send(mypacket,0);    8	printf("ap send ack packet,time = %f\n",op_sim_time());   }   
                     
   ����   
          pr_state                       �   �      �   �  N   �          
   tr_0   
       ����          ����          
    ����   
          ����                       pr_transition               )   �     A   �  I   h          
   tr_1   
       J   from_source   J       ����          
    ����   
          ����                       pr_transition               Z   �     X   e  \   �          
   tr_2   
       ����          ����          
    ����   
          ����                       pr_transition                �   �     T   �  �   �  �   �  M   �          
   tr_3   
       
   default   
       ����          
    ����   
          ����                       pr_transition               
       D   �  B  R          
   tr_4   
       J   from_receiver   J       ����          
    ����   
          ����                       pr_transition               W       Y  T  U   �          
   tr_5   
       ����          ����          
    ����   
          ����                       pr_transition                                             
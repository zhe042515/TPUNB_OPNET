MIL_3_Tfile_Hdr_ 145A 140A modeler 9 60585339 60588F48 7 DESKTOP-RD4S7T2 51133 0 0 none none 0 0 none 910BF704 A54 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                           ��g�      @   D   H      �  0  4  8  <  H  L  P  �           	   begsim intrpt         
   ����   
   doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����             Packet *	\mypacket;       int	\my_address;       int	\user_source_id;       int	\user_node_id;       int	\ii;       char	\name[7];                                                          �   �          
   init   
       
      user_source_id=op_id_self();   ,user_node_id=op_topo_parent(user_source_id);   for(ii=0;ii<7;ii++)   	{    		name[ii]=0;   	}   *op_ima_obj_hname_get(user_node_id,name,7);   my_address=0;   for(ii=0;ii<7;ii++)   	{    #		if(name[ii]>=48 && name[ii]<=57)    		{    *			my_address = my_address*10+name[ii]-48;   		}   	}   ://printf("user%d source initialize succeed\n",my_address);   
                     
   ����   
          pr_state        J   �          
   make   
       J      $mypacket = op_pk_create_fmt("data");   %op_pk_total_size_set (mypacket,5000);   )op_pk_nfd_set(mypacket, "frame_type", 1);   .op_pk_nfd_set(mypacket, "receive_address", 1);   4op_pk_nfd_set(mypacket, "send_address", my_address);   *op_pk_send_delayed(mypacket,0,my_address);   Lprintf("user%d source make data packet,time=%f\n",my_address,op_sim_time());   J                         ����             pr_state                        �   �      �   �  J   �          
   tr_0   
       ����          ����          
    ����   
          ����                       pr_transition              �   �     [   �  �   �  �  '  9   �          
   tr_1   
       
   default   
       ����          
    ����   
          ����                       pr_transition                                             
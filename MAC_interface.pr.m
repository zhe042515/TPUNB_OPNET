MIL_3_Tfile_Hdr_ 145A 140A modeler 9 61C85BAB 62A6A919 54 DESKTOP-RD4S7T2 51133 0 0 none none 0 0 none 5C9B54D3 2F56 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                         ��g�      @   D   H      �  -2  -6  -:  ->  -J  -N  -R  �           	   begsim intrpt             ����      doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����          
   	int	\kkk;       Objid	\my_process_objid;       Objid	\my_node_objid;       int	\my_node_id;       int	\my_node_type;           C       /*   #define Port_MAC_MCPS_up			13   #define Port_MAC_MCPS_down			14       #define Port_MAC_MLME_up			5   #define Port_MAC_MLME_down			6   */   (#define		CEN_NODE				43690		//���Ľڵ�id               �#define STRM_NWK_MCPS			((op_intrpt_type() == OPC_INTRPT_STRM) &&((op_intrpt_strm() == Port_NWK_MCPS_up_in)||(op_intrpt_strm() == Port_NWK_MCPS_down_in)))   h#define STRM_NWK_MLME			((op_intrpt_type() == OPC_INTRPT_STRM) &&(op_intrpt_strm() == Port_NWK_MLME_in))   �#define STRM_MAC_MCPS			((op_intrpt_type() == OPC_INTRPT_STRM) &&(op_intrpt_strm() == PHY_MAC0 || op_intrpt_strm() == PHY_MAC2))   �#define STRM_MAC_MLME			((op_intrpt_type() == OPC_INTRPT_STRM) &&(op_intrpt_strm() == MLME_MAC0 || op_intrpt_strm() == MLME_MAC2))           #define Scan						0   #define Associate					1   #define Alive						2   #define ChildSEt					3   #define Conflict					4   #define Multicast					5   #define Leave						7   #define Syn_Seq_Set					8   #define BeaconConfirm				9           5/*------------------ Input Index ------------------*/   7//#define 	APP_MAC 					1		//�����ϲ㷢�����нڵ������   .#define 	PHY_MAC0 					2		//�������νڵ������   3#define 	MLME_MAC0 					3		//�������νڵ��MLME����   .#define 	PHY_MAC2 					0		//�������νڵ������   3#define 	MLME_MAC2 					1		//�������νڵ��MLME����   D#define		Port_NWK_MCPS_up_in			4		//����㴫��������֡(�Ӷ��ϵ�����)   E#define		Port_NWK_MCPS_down_in		5		//����㴫��������֡(�Ӷ��µ�����)   1#define 	Port_NWK_MLME_in			6		//����㴫����ԭ��       6#define		PHY_MAC_down_1				7		//�������εڶ��ŵ�������   6#define		PHY_MAC_down_2				8		//�������ε����ŵ�������   6#define		PHY_MAC_down_3				9		//�������ε����ŵ�������       8#define		MLME_MAC_down_1				10		//�������εڶ��ŵ���ԭ��   8#define		MLME_MAC_down_2				11		//�������ε����ŵ���ԭ��   8#define		MLME_MAC_down_3				12		//�������ε����ŵ���ԭ��                   6/* ------------------Output Index------------------ */   ,#define 	MAC_PHY_DOWN 					2		//�������νڵ�   1#define 	MLME_PHY_DOWN 					3		//MLME�������νڵ�   +#define 	MAC_PHY_UP 						0		//�������νڵ�   /#define 	MLME_PHY_UP 					1		//MLME�������νڵ�   C#define 	Port_NWK_MCPS_up_out			4		//��������������֡(���ϵ�����)   E#define 	Port_NWK_MCPS_down_out			5		//��������������֡(���µ�����)   3#define 	Port_NWK_MLME_out				6		//����������ԭ��           2#define 	MLME_PHY_DOWN_1 				7		//MLME�����ڶ��ŵ�   2#define 	MLME_PHY_DOWN_2 				8		//MLME���������ŵ�   2#define 	MLME_PHY_DOWN_3 				9		//MLME���������ŵ�                                                              �            
   init   
       
      kkk=0;   -my_node_objid = op_topo_parent(op_id_self());   Gop_ima_obj_attr_get (my_node_objid,	"g_node_ESN_address",	&my_node_id);   Bop_ima_obj_attr_get (my_node_objid,	"g_node_type",	&my_node_type);   
                     
   ����   
          pr_state        �            
   idle   
       
      /*Packet* pk;   	int type;   	type = 0;    my_process_objid = op_id_self();   4my_node_objid    = op_topo_parent(my_process_objid);   ?op_ima_obj_attr_get (my_node_objid,	"MACAddress",	&my_node_id);   3//printf("node id %d   kkk = %d\n",my_node_id,kkk);   	kkk++;   if( my_node_id == 2&& kkk == 5)   	{   	pk = op_pk_create(8);   6	op_pk_fd_set (pk, 0, OPC_FIELD_TYPE_INTEGER,type,8) ;   	op_pk_send(pk,MLME_PHY2);   7	printf("node %d send MLME_SCAN.request\n",my_node_id);       	}   */   
                         ����             pr_state           �          
   NWK_MCPS   
       
      Packet* pk;   +if(op_intrpt_strm() == Port_NWK_MCPS_up_in)   	{   #	pk = op_pk_get (op_intrpt_strm());   	op_pk_send(pk, MAC_PHY_UP);   	}   else   	{   #	pk = op_pk_get (op_intrpt_strm());   	op_pk_send(pk, MAC_PHY_DOWN);   	}       
                     
   ����   
          pr_state        �   �          
   NWK_MLME   
       
   6   Packet* pk;   Packet* pk2;   	int type;   "pk = op_pk_get (op_intrpt_strm());   op_pk_fd_get(pk,0,&type);   //����MAC����֡   K//printf("node %d IO recv a mlme from nwk, type = %d\n",my_node_id, type);	   switch(type)   	{   (	case Scan: op_pk_send(pk, MLME_PHY_UP);   Z			printf("Node %d NWK_MLME received a Scan type command, type = %d\n", my_node_id, type);   				break;   	case Associate:   			op_pk_fd_get(pk,5,&type);   -			if(type != 1) op_pk_send(pk, MLME_PHY_UP);   &			else op_pk_send(pk, MLME_PHY_DOWN);   _			printf("Node %d NWK_MLME received a Associate type command, type = %d\n", my_node_id, type);   				break;   -	case ChildSEt:op_pk_send(pk, MLME_PHY_DOWN);   ^			printf("Node %d NWK_MLME received a ChildSEt type command, type = %d\n", my_node_id, type);   				break;   ,	case Multicast:op_pk_send(pk, MLME_PHY_UP);   _			printf("Node %d NWK_MLME received a Multicast type command, type = %d\n", my_node_id, type);   				break;   %	case Alive:op_pk_fd_get(pk,2,&type);   /			if(type == 1) op_pk_send(pk, MLME_PHY_DOWN);   :			else op_pk_send(pk, MLME_PHY_UP);//�����Ը��ڵ���б���   [			printf("Node %d NWK_MLME received a Alive type command, type = %d\n", my_node_id, type);   				break;   %	case Leave:op_pk_fd_get(pk,1,&type);   			if(type == 0)   				{   				pk2 = op_pk_copy(pk);   "				op_pk_send(pk, MLME_PHY_DOWN);   !				op_pk_send(pk2, MLME_PHY_UP);   				}   			else   				{    				op_pk_send(pk, MLME_PHY_UP);   				pk2 = op_pk_create(0);   <				op_pk_fd_set (pk2, 0, OPC_FIELD_TYPE_INTEGER, Leave, 8);   8				op_pk_fd_set (pk2, 1, OPC_FIELD_TYPE_INTEGER, 2, 8);   &				op_pk_send(pk2,Port_NWK_MLME_out);   				}   			   [			printf("Node %d NWK_MLME received a Leave type command, type = %d\n", my_node_id, type);   				break;   .	case Syn_Seq_Set:op_pk_send(pk, MLME_PHY_UP);   ^			printf("Node %d NWK_MLME Leave a Syn_Seq_Set type command, type = %d\n", my_node_id, type);   				break;   a	default:printf("Node %d NWK_MLME received a error type command, type = %d\n", my_node_id, type);   			op_pk_destroy(pk);   	}       
                     
   ����   
          pr_state        �  �          
   MAC_MLME   
       J   A   Packet* pk;   int number;   	int type;   int result;   �if(op_intrpt_strm() == MLME_MAC0 || op_intrpt_strm() == MLME_MAC_down_1 || op_intrpt_strm() == MLME_MAC_down_2 || op_intrpt_strm() == MLME_MAC_down_3)   	{   "	pk = op_pk_get(op_intrpt_strm());   	op_pk_fd_get(pk,0,&type);	   D	printf("Node %d interface receive a MLME from down\n", my_node_id);   	switch(type)   		{   		case 1:	//	����ȷ��ԭ��   			{   			op_pk_fd_get(pk,2,&result);   f			printf("Node %d receive a associate confirm at %f,result is %d\n",my_node_id,op_sim_time(),result);   $			op_pk_send(pk,Port_NWK_MLME_out);   				break;   			}   4		case Alive:op_pk_send(pk,Port_NWK_MLME_out);break;   7		case Conflict:op_pk_send(pk,Port_NWK_MLME_out);break;   g		case Leave:op_pk_send(pk,Port_NWK_MLME_out);printf("Node %d down io mlme leave\n", my_node_id);break;   8		case Multicast:op_pk_send(pk,Port_NWK_MLME_out);break;   :		case Syn_Seq_Set:op_pk_send(pk,Port_NWK_MLME_out);break;   <		case BeaconConfirm:op_pk_send(pk,Port_NWK_MLME_out);break;   
		default:   			{   			op_pk_destroy(pk);   			}   		   		}   	}   &else if(op_intrpt_strm() == MLME_MAC2)   	{   "	pk = op_pk_get(op_intrpt_strm());   	op_pk_fd_get(pk,0,&type);	   &	//printf("receive a MLME from up\n");   	switch(type)   		{   		case 0:	//	ɨ��ȷ��ԭ��   			{   M			printf("Node %d receive a scan confirm at %f\n",my_node_id,op_sim_time());   			op_pk_fd_get(pk,1,&number);   #			printf("number is %d\n",number);   $			op_pk_send(pk,Port_NWK_MLME_out);   				break;   			}   		case 1:	//	����ȷ��ԭ��   			{   			op_pk_fd_get(pk,2,&result);   f			printf("Node %d receive a associate confirm at %f,result is %d\n",my_node_id,op_sim_time(),result);   $			op_pk_send(pk,Port_NWK_MLME_out);   				break;   			}   a		case Alive:printf("Node %d recv a alive\n", my_node_id);op_pk_send(pk,Port_NWK_MLME_out);break;   7		case Conflict:op_pk_send(pk,Port_NWK_MLME_out);break;   8		case Multicast:op_pk_send(pk,Port_NWK_MLME_out);break;   e		case Leave:op_pk_send(pk,Port_NWK_MLME_out);printf("Node %d up io mlme leave\n", my_node_id);break;   <		case BeaconConfirm:op_pk_send(pk,Port_NWK_MLME_out);break;   
		default:   			{   			op_pk_destroy(pk);   			}   		   		}   	}   J                     
   ����   
          pr_state          �          
   MAC_MCPS   
       
      Packet* pk;   �if(op_intrpt_strm() == PHY_MAC0 || op_intrpt_strm() == PHY_MAC_down_1 || op_intrpt_strm() == PHY_MAC_down_2 ||op_intrpt_strm() == PHY_MAC_down_3)   	{   D	//printf("node %d interface receive a msg from down\n",my_node_id);   "	pk = op_pk_get(op_intrpt_strm());   '	op_pk_send(pk,Port_NWK_MCPS_down_out);   	   	}   else   	{   C	//printf("node %d interface receive a msg from up\n",my_node_id);	   "	pk = op_pk_get(op_intrpt_strm());   %	op_pk_send(pk,Port_NWK_MCPS_up_out);   	   	}   
                     
   ����   
          pr_state          
             
        �    �            
   tr_0   
       
   1==1   
       ����          
    ����   
          ����                       pr_transition              I   �     �       �          
   tr_1   
       
   STRM_NWK_MCPS   
       ����          
    ����   
          ����                       pr_transition              �   �     �   �      �          
   tr_2   
       
   STRM_NWK_MLME   
       ����          
    ����   
          ����                       pr_transition              �   �        �  �            
   tr_3   
       ����          ����          
    ����   
          ����                       pr_transition              T   �        �  �            
   tr_4   
       ����          ����          
    ����   
          ����                       pr_transition              C  L     {      �          
   tr_5   
       
   STRM_MAC_MCPS   
       ����          
    ����   
          ����                       pr_transition              N  T       �  �            
   tr_6   
       ����          ����          
    ����   
          ����                       pr_transition              �  L     �      �          
   tr_7   
       
   STRM_MAC_MLME   
       ����          
    ����   
          ����                       pr_transition              �  U     �  �  �            
   tr_8   
       ����          ����          
    ����   
          ����                       pr_transition      	        �       �    �   �  �    �            
   tr_9   
       
   default   
       ����          
    ����   
          ����                       pr_transition         
                                    
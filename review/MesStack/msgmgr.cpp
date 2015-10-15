	
#include "msgmgr.h"


msgmgr::msgmgr()
{

}
	
msgmgr::~msgmgr()
{

}

void msgmgr::regist(int msg_id, msgmgr* msg)
{
	msg_list.insert(std::map<int, msgmgr*>::value_type(msg_id, msg));
}

void msgmgr::deliver_msg(int msg_id, char* msg_buf, int msg_len)
{
	std::map<int, msgmgr*>::iterator it = msg_list.begin();
	it = msg_list.find(msg_id);
	if(it != msg_list.end())
	{
		it->second->message_proc(msg_id, msg_buf, msg_len);
	}
}
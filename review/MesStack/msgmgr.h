
#ifndef __MSGMGR_H__
#define __MSGMGR_H__

#include <map>

class msgmgr
{
public:
	std::map <int, msgmgr*> msg_list;
public:
	msgmgr();

	~msgmgr();
	
	virtual void message_proc(int msg_id, char* msg_buf, int msg_size) = 0;

	void regist(int msg_id, msgmgr* msg);

	void deliver_msg(int msg_id, char* pmsg_buf, int msg_len);
};

#endif
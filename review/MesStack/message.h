

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "msgmgr.h"

class message 
{
private:
	
public:
	message();
	~message();


	//regist
	void register(int msg_id);

	//send message
	void send_message(int msg_id, char* msg_buf, int msg_size);

};
#endif
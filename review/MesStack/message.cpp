

#include "message.h"

message::message()
{

}

message::~message()
{

}

void message::register(int msg_id)
{
	regist(msg_id, this);
}

void send_messgae(int msg_id, char* msg_buf, int msg_size)
{
	deliver_msg(msg_id, msg_buf, msg_len);
}
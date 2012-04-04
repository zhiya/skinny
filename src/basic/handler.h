#ifndef __Include_basic_handler_H__
#define __Include_basic_handler_H__

typedef void (*defHandler)(int);

extern void handle_rap(int skt);
extern void handle_echo(int skt);
extern void handle_http(int stk);

#endif//__Include_basic_handler_H__


#ifndef MY_CHAT_CURRENTID
#define MY_CHAT_CURRENTID

#include <sys/syscall.h>
#include <pthread.h>
#include <unistd.h>

pid_t getCtid()
{
  return static_cast<pid_t>(::syscall(SYS_gettid));
}


#endif //MY_CHAT_CURRENTID
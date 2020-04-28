#ifndef UTILS_LOG_INC_LOG_H__H_
#define UTILS_LOG_INC_LOG_H__H_


void mylog(int is_fatal, const char *file, int line, const char *format, ...);


#define LOGT(args...) mylog(0, __FILE__, __LINE__, args)
#define LOGE(args...) mylog(1, __FILE__, __LINE__, args)

#endif  //  UTILS_LOG_INC_LOG_H__H_

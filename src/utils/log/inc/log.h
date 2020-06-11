#ifndef UTILS_LOG_INC_LOG_H__H_
#define UTILS_LOG_INC_LOG_H__H_


#ifdef __cplusplus
extern "C" {
#endif
void mylog(int is_fatal, const char *file, int line, const char *format, ...);


#define LOGT(args...) mylog(0, __FILE__, __LINE__, args)
#define LOGE(args...) mylog(1, __FILE__, __LINE__, args)

#ifdef __cplusplus
}
#endif
#endif  //  UTILS_LOG_INC_LOG_H__H_

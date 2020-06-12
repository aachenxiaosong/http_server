#ifndef _INC_UNI_AUTH_H_
#define _INC_UNI_AUTH_H_


typedef enum {
  AUTH_IDLE,
  AUTH_REGISTER,
  AUTH_UPDATE,
  AUTH_CHECK,
} AuthStatus;

int AuthInit(const char *config_path, const char *appkey, const char *secret, const char *udid);
void AuthFinit(void);
int AuthGetToken(char *token);
int IsAuthValid(void);


#endif  //  _INC_UNI_AUTH_H_


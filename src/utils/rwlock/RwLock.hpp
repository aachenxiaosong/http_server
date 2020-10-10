#ifndef UTILS_RWLOCK_RWLOCK_HPP
#define UTILS_RWLOCK_RWLOCK_HPP

#include <pthread.h>

class RwLock {
private:
    pthread_rwlock_t mLock;
public:
    RwLock() {
        pthread_rwlock_init(&mLock, NULL);
    }

    ~RwLock() {
        pthread_rwlock_destroy(&mLock);
    }

    void readLock() {
        pthread_rwlock_rdlock(&mLock);
    }

    void readUnlock() {
        pthread_rwlock_unlock(&mLock);
    }

    void writeLock() {
        pthread_rwlock_wrlock(&mLock);
    }

    void writeUnlock() {
        pthread_rwlock_unlock(&mLock);
    }
};

#endif
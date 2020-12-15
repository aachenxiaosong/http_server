#pragma once

#include <pthread.h>

class UniRwLock {
private:
    pthread_rwlock_t mLock;
public:
    UniRwLock() {
        pthread_rwlock_init(&mLock, NULL);
    }

    ~UniRwLock() {
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

//
// Created by egor on 19.02.2021.
//

#ifndef CO_WORK_RESPONSE_FORMAT_H
#define CO_WORK_RESPONSE_FORMAT_H
template<typename T>
struct ResponseFormat {
    std::string error;
    T data;
};

#endif // CO_WORK_RESPONSE_FORMAT_H

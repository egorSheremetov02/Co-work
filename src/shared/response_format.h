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

template<typename T>
void to_json(nlohmann::json &j, const ResponseFormat<T> &request) {
    j = nlohmann::json{{"error", request.error},
                       {"data",  request.data}};
}

template<typename T>
void from_json(nlohmann::json const &j, ResponseFormat<T> &request) {
    j.at("data").get_to(request.data);
    j.at("error").get_to(request.error);
}

#endif // CO_WORK_RESPONSE_FORMAT_H

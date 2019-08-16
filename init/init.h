//
// Created by GGG1235 on 2019-08-16.
//

#ifndef SIMPLEHTTPSERVER_INIT_H
#define SIMPLEHTTPSERVER_INIT_H

#define LOG_PATH "./log"
#define FILE_NAME "./log/log.txt"
#define FILE404 "./error/404.html"
#define FILE400 "./error/400.html"

#define BUF_SIZE 10240
#define SMALL_BUF 100

namespace init{
    void init(){
        system(opt::format("mkdir -p %s",LOG_PATH).c_str());
    }
}

#endif //SIMPLEHTTPSERVER_INIT_H

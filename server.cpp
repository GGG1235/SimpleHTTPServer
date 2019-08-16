//
// Created by GGG1235 on 2019-08-15.
//

#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdatomic.h>

#include "utils/output"
#include "utils/logs"
#include "utils/timestamp"
#include "init/init.h"

void request_handler(int sock);
void send_data(FILE *fp,char *ct,char *file_name);
std::string content_type(char *file);
void send_error404(FILE *fp);
void send_error400(FILE *fp);
void save_log(const std::string &file_name,std::string &&log);

std::mutex mtx;
std::string file_path;

int main(int argc,const char **argv,const char **envp){
    init::init();
    int serv_sock,clnt_sock;
    sockaddr_in serv_addr{},clnt_addr{};
    socklen_t clnt_adr_size;

    if (argc!=3){
        opt::error_ouputs("Usage : %s <Port> <FilePath> \n",argv[0]);
        save_log(FILE_NAME,opt::format("Usage : %s <Port> <FilePath>",argv[0]));
        exit(1);
    }

    file_path=std::string(argv[2]);

    serv_sock=socket(PF_INET,SOCK_STREAM,0);
    if (serv_sock==-1){
        save_log(FILE_NAME,"socket() error!");
        opt::error_handling("socket() error!");
    }

    memset(&serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));

    if (bind(serv_sock,(sockaddr*)&serv_addr, sizeof(serv_addr))==-1){
        save_log(FILE_NAME,"bind() error!");
        opt::error_handling("bind() error!");
    }

    if (listen(serv_sock,20)==-1){
        save_log(FILE_NAME,"listen() error!");
        opt::error_handling("listen() error!");
    }

    while(1){
        clnt_adr_size= sizeof(clnt_addr);
        clnt_sock=accept(serv_sock,(sockaddr*)&clnt_addr,&clnt_adr_size);
        if (clnt_sock==-1){
            save_log(FILE_NAME,"accpet() error!");
            opt::error_handling("accpet() error!");
        }

        std::cout<<opt::format("Connection Request: %s:%d ",inet_ntoa(clnt_addr.sin_addr),ntohs(clnt_addr.sin_port))<<std::endl;
        save_log(FILE_NAME,opt::format("Connection Request: %s:%d ",inet_ntoa(clnt_addr.sin_addr),ntohs(clnt_addr.sin_port)));

        std::thread th(request_handler,clnt_sock);
        th.detach();
    }

    close(serv_sock);
    return 0;
}

void request_handler(int sock){

    char req_line[SMALL_BUF];

    char method[10];
    char ct[15];
    char file_name[30];

    FILE *clnt_read=fdopen(sock,"r");
    FILE *clnt_write=fdopen(dup(sock),"w");
    fgets(req_line,SMALL_BUF,clnt_read);
    std::string req=opt::format("%s",req_line);
    std::string::size_type idx;
    if ((idx=req.find("HTTP/"))==std::string::npos){
        std::cerr<<opt::format("req_line error! req_line:%s",req.c_str())<<std::endl;
        save_log(FILE_NAME,opt::format("req_line error! req_line:%s",req.c_str()));
        send_error400(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return;
    }
    strcpy(method,strtok(req_line," /"));
    strcpy(file_name,strtok(nullptr," /"));
    strcpy(ct,content_type(file_name).c_str());
    if (strcmp(method,"GET")!=0&&strcmp(method,"POST")!=0){
        std::cerr<<opt::format("method error! method:%s",method)<<std::endl;
        save_log(FILE_NAME,opt::format("method error! method:%s",method));
        send_error400(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return;
    }
    fclose(clnt_read);
    std::cout<<opt::format("[%s]- %s",timestamp::time_stamp().c_str(),req.c_str())<<std::endl;
    save_log(FILE_NAME,opt::format("[%s]- %s",timestamp::time_stamp().c_str(),req.c_str()));
    send_data(clnt_write,ct,file_name);
}

void send_data(FILE *fp,char *ct,char *file_name){
    char protocol[]="HTTP/1.1 200 OK\r\n";
    char server[]="Server:Linux Web Server \r\n";
    char cnt_len[]="Content-length:2048\r\n";
    char cnt_type[SMALL_BUF];
    char buf[BUF_SIZE];
    FILE *send_file;

    sprintf(cnt_type,"Content-type:%s\r\n\r\n",ct);
    std::string path=file_path+"/"+std::string(file_name);
    send_file=fopen(path.c_str(),"r");
    if (send_file== nullptr){
        std::cerr<<opt::format("path error! path: %s",path.c_str())<<std::endl;
        save_log(FILE_NAME,opt::format("path error! path: %s",path.c_str()));
        send_error404(fp);
        return;
    }

    fputs(protocol,fp);
    fputs(server,fp);
    fputs(cnt_len,fp);
    fputs(cnt_type,fp);

    while(fgets(buf,BUF_SIZE,send_file)!= nullptr){
        fputs(buf,fp);
        fflush(fp);
    }
    fflush(fp);
    fclose(fp);
    fclose(send_file);
}

std::string content_type(char *file){
    char extension[SMALL_BUF];
    char file_name[SMALL_BUF];

    strcpy(file_name,file);
    strtok(file_name,".");
    strcpy(extension,strtok(nullptr,"."));

    if (!strcmp(extension,"html")||!strcmp(extension,"htm")){
        return "text/html";
    }else if(!strcmp(extension,"json")){
        return "text/json";
    }else{
        return "text/plain";
    }
}

void send_error404(FILE *fp){
    char buf[BUF_SIZE];
    char protocol[]="HTTP/1.1 404 Not Found\r\n";
    char server[]="Server:Linux Web Server \r\n";
    char cnt_len[]="Content-length:2048\r\n";
    char cnt_type[]="Content-type:text/html\r\n\r\n";
    char content[]="<html><head><meta charset=\"UTF-8\"><title>404 Not Found</title></head>"
                   "<body><font size=+5>"
                   "<br>页面不存在"
                   "</font></body>"
                   "</html>";

    FILE *file=fopen(FILE404,"r");
    if (file== nullptr){
        std::cerr<<opt::format("open 404.html error! path:%s",FILE404)<<std::endl;
        save_log(FILE_NAME,opt::format("open 404.html error! path:%s",FILE404));
        fputs(protocol, fp);
        fputs(server, fp);
        fputs(cnt_len, fp);
        fputs(cnt_type, fp);
        fputs(content,fp);
    } else {
        fputs(protocol, fp);
        fputs(server, fp);
        fputs(cnt_len, fp);
        fputs(cnt_type, fp);
        while (fgets(buf, BUF_SIZE, file) != nullptr) {
            fputs(buf, fp);
            fflush(fp);
        }
        fclose(file);
    }
    fflush(fp);
    fclose(fp);
}
void send_error400(FILE *fp){
    char buf[BUF_SIZE];
    char protocol[]="HTTP/1.1 400 Bad Request\r\n";
    char server[]="Server:Linux Web Server \r\n";
    char cnt_len[]="Content-length:2048\r\n";
    char cnt_type[]="Content-type:text/html\r\n\r\n";
    char content[]="<html><head><meta charset=\"UTF-8\"><title>400 Bad Request</title></head>"
                   "<body><font size=+5>"
                   "<br>请求无效.检查地址是否正确."
                   "</font></body>"
                   "</html>";

    FILE *file=fopen(FILE400,"r");
    if (file== nullptr){
        std::cerr<<opt::format("open 400.html error! path:%s",FILE400)<<std::endl;
        save_log(FILE_NAME,opt::format("open 400.html error! path:%s",FILE400));
        fputs(protocol, fp);
        fputs(server, fp);
        fputs(cnt_len, fp);
        fputs(cnt_type, fp);
        fputs(content,fp);
    } else {
        fputs(protocol, fp);
        fputs(server, fp);
        fputs(cnt_len, fp);
        fputs(cnt_type, fp);
        while (fgets(buf, BUF_SIZE, file) != nullptr) {
            fputs(buf, fp);
            fflush(fp);
        }
        fclose(file);
    }
    fflush(fp);
    fclose(fp);
}

void save_log(const std::string &file_name,std::string &&log){
    mtx.lock();
    log.append("\n");
    log::save(opt::format("[%s] %s",timestamp::time_stamp().c_str(),log.c_str()).c_str(),file_name.c_str());
    mtx.unlock();
}
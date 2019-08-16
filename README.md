# SimpleHTTPServer
C++多线程简易http服务器

```
├── CMakeLists.txt
├── README.md
├── html
│   ├── error
│   │   ├── 400.html
│   │   └── 404.html
│   ├── index.html
│   └── index.json
├── init
│   └── init.h
├── mthread
│   └── semaphore.h
├── server.cpp
└── utils
    ├── logs
    ├── output
    └── timestamp
```

基于Socket

socket->bind->listen->accept->read/write->close

![Socket](https://github.com/GGG1235/SimpleHTTPServer/blob/master/images/socket.png)

```shell script
mkdir build
cd build
cmake ..
make
./SimpleHTTPServer 8000 ../html
```

Or

```shell script
./make.sh
cd build
./build/SimpleHTTPServer 8000 ../html
```



#### 截图:
![img1](https://github.com/GGG1235/SimpleHTTPServer/blob/master/images/img1.png)
![img2](https://github.com/GGG1235/SimpleHTTPServer/blob/master/images/img2.png)
![img3](https://github.com/GGG1235/SimpleHTTPServer/blob/master/images/img3.png)
![img4](https://github.com/GGG1235/SimpleHTTPServer/blob/master/images/img4.png)
![img5](https://github.com/GGG1235/SimpleHTTPServer/blob/master/images/img5.png)

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

[Socket](./images/socket.png)

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
[img1](./images/img1.png)
[img2](./images/img2.png)
[img3](./images/img3.png)
[img4](./images/img4.png)
[img5](./images/img5.png)
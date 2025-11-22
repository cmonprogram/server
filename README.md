### build:
make clean && make server && sudo ./server
***
### buld tests:
make clean && make tests && ./run_tests
***
### usage:
sudo ./server [-t | -u] port
***
### install (run daemon udp at 8080):
sudo make install
***
### remove:
sudo make remove
***
# TODO
```
1. simple socket server on C
            1. [ok] console input
            2. [ok] udp server start
            3. tcp server start
2. [ok] daemon
3. buffer work (epoll?)
4. load test (to see improvement after epoll)
5. run on rockchip
6. try core-level acceleration (dpdk/AF_XDP/eBPF)
```

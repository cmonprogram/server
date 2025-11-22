### Build commands
| Command | Description |
| :---         |     :---:      | 
| `make clean && make server && sudo ./server` | build |
| `make clean && make tests && ./run_tests` | buld tests |
| `sudo ./server [-t \| -u] port` | usage |
| `sudo make install` | install (run daemon udp at 8080) |
| `sudo make remove` | remove |

### Server commands
| Command | Description |
| :---         |     :---:      | 
| `time` | prints timestamp |
| `exit` | close server |

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

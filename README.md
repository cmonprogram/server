### Build commands
| Command | Description |
| :---         |     :---:      | 
| `make clean && make server && sudo ./server` | build |
| `make clean && make tests && ./run_tests` | buld tests |
| `sudo ./server [-t \| -u] port` | usage |
| `sudo make install` | install (run daemon tcp at 8080) |
| `sudo make remove` | remove |

### Server commands
| Command | Description |
| :---         |     :---:      | 
| `time` | prints timestamp |
| `exit` | close server |
| `test_start` | starts timer on next 1000 packages |
| `test_end` | terminate test, print results |

### Examples
[EXAMPLE.md](EXAMPLE.md)

# TODO
```
1. [ok] simple socket server on C
            1. [ok] console input
            2. [ok] udp support
            3. [ok] tcp support
            4. [ok] command response
            5. [ok] http response
2. [ok] daemon
3. [ok] load test (to see improvement after epoll)
4. buffer work (epoll?)
5. run on rockchip
6. try core-level acceleration (dpdk/AF_XDP/eBPF)
```

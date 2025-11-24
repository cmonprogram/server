### Output example
```
$ ./server -t 8080
[ok] init server
[ok] create socket
[ok] bind socket
[server started] prort:8080
[get] time
[time] 1764016240066
[get] test
[err] wrong command
[get] exit
[exit]
```
```
$ echo -n "time" | nc -w 0 -t 127.0.0.1 8080
$ echo -n "test" | nc -w 0 -t 127.0.0.1 8080
$ echo -n "exit" | nc -w 0 -t 127.0.0.1 8080
```

### Test example
```
$ ./server -t 8080
[ok] init server
[ok] create socket
[ok] bind socket
[server started] prort:8080
[get] test_start
[test_start] on 1000 packages
[get 1/1000] test_package
...
[get 999/1000] test_package
[get 1000/1000] test_package
[test_end] 1000 packages by 2344ms
```
```
$ echo -n "test_start" | nc -w 0 -t 127.0.0.1 8080 ; for i in {1..1000}; do echo -n "test_package" | nc -w 0 -t 127.0.0.1 8080; done
```
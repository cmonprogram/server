#sudo make install
PORT="${1:-8080}"
echo "run on port" $PORT;

function run_paralell(){
    echo "paralell test";
    echo -n "test_start" | nc -w 0 -t 127.0.0.1 $PORT ; 
    (for i in {1..100}; do echo -n "test_package_p1" | nc -w 0 -t 127.0.0.1 $PORT; done;)&
    (for i in {1..100}; do echo -n "test_package_p2" | nc -w 0 -t 127.0.0.1 $PORT; done;)&
    (for i in {1..100}; do echo -n "test_package_p3" | nc -w 0 -t 127.0.0.1 $PORT; done;)&
    (for i in {1..100}; do echo -n "test_package_p4" | nc -w 0 -t 127.0.0.1 $PORT; done;)&
    (for i in {1..100}; do echo -n "test_package_p5" | nc -w 0 -t 127.0.0.1 $PORT; done;)&
    #(for i in {1..100}; do echo -n "test_package_p6" | nc -w 0 -t 127.0.0.1 $PORT; done;)&
    #(for i in {1..100}; do echo -n "test_package_p7" | nc -w 0 -t 127.0.0.1 $PORT; done;)&
    wait
    echo -n "test_end" | nc -w 1 -t 127.0.0.1 $PORT ; 
}

function run_sequential(){
    echo "sequential test";
    echo -n "test_start" | nc -w 0 -t 127.0.0.1 $PORT ; 
    for i in {1..500}; do echo -n "test_package_p0" | nc -w 0 -t 127.0.0.1 $PORT; done; 
    echo -n "test_end" | nc -w 1 -t 127.0.0.1 $PORT ; 
}

if [[ "$2" == "-s" ]]; then 
    run_sequential
elif [[ "$2" == "-p" ]]; then 
    run_paralell
else 
    run_paralell
    run_sequential
fi


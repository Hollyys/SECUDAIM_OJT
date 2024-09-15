  ## 240913 [DPDK]

libcap은 네트워크 인터페이스로부터 네트워크 정보를 커널을 통해 전달받는다. 커널은 general한 소프트웨어로 굳이 필요하지 않은 무거운 작업들이 포함된다. 따라서 처리 속도가 느리고 패킷 loss가 발생할 수 있다. dpdk의 경우 커널을 통하지 않고 직접 네트워크 디바이스로 부터 정보를 받는다. 커널과 달리 매우 가볍기 때문에 속도가 빠르다. 이때  dpdk의 특성상 네트워크 인터페이스를 점유해야한다. 나의 경우 vm환경에서 하나의 네트워크 인터페이스만 설정이 되어있어서 추가적인 인터페이스를 만들어주었다. dpdk를 사용하기 위한 사전 작업(build. 네트워크 디바이스 점유 등..)을 거친 다음 dpdk를 사용해야한다.

1. Build
    
    `lockard/3rdparty/dpdk` 에 위치한 Make파일을  실행하면 Ninja, meson을 활용하여 빌드함. ⇒ 네트워크 카드를 선점하기 위한 파이썬 파일이 생성됨.
    
2. 네트워크 카드 DPDK에게 넘겨주기 **`lockard/script/snc_control.sh:dpdk_load()`**
    1. `ifconfig` 현재 설정 되어있는 네트워크 디바이스 정보를 확인함
        
        ```bash
        enp0s3: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
                inet 10.0.2.15  netmask 255.255.255.0  broadcast 10.0.2.255
                inet6 fe80::1846:66a8:15ff:e439  prefixlen 64  scopeid 0x20<link>
                ether 08:00:27:c3:e1:89  txqueuelen 1000  (Ethernet)
                RX packets 153404  bytes 158017269 (158.0 MB)
                RX errors 0  dropped 0  overruns 0  frame 0
                TX packets 86391  bytes 10881562 (10.8 MB)
                TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
        
        enp0s8: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
                inet6 fe80::6024:ddd0:5503:9a8f  prefixlen 64  scopeid 0x20<link>
                ether 08:00:27:ff:85:57  txqueuelen 1000  (Ethernet)
                RX packets 0  bytes 0 (0.0 B)
                RX errors 0  dropped 0  overruns 0  frame 0
                TX packets 45  bytes 7790 (7.7 KB)
                TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
        
        enp0s9: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
                ether 08:00:27:84:0b:0b  txqueuelen 1000  (Ethernet)
                RX packets 68  bytes 7483 (7.4 KB)
                RX errors 0  dropped 0  overruns 0  frame 0
                TX packets 47381  bytes 8116188 (8.1 MB)
                TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
        
        lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
                inet 127.0.0.1  netmask 255.0.0.0
                inet6 ::1  prefixlen 128  scopeid 0x10<host>
                loop  txqueuelen 1000  (Local Loopback)
                RX packets 83737  bytes 9706056 (9.7 MB)
                RX errors 0  dropped 0  overruns 0  frame 0
                TX packets 83737  bytes 9706056 (9.7 MB)
                TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
        ```
        
    2. `python3 ./3rdparty/dpdk/dpdk-21.11/usertools/dpdk-devbind.py --status` 네트워크 디바이스 상태 출력
        
        ```bash
        Network devices using kernel driver
        ===================================
        0000:00:03.0 '82540EM Gigabit Ethernet Controller 100e' if=enp0s3 drv=e1000 unused=igb_uio,vfio-pci *Active*
        0000:00:08.0 '82540EM Gigabit Ethernet Controller 100e' if=enp0s8 drv=e1000 unused=igb_uio,vfio-pci
        0000:00:09.0 '82540EM Gigabit Ethernet Controller 100e' if=enp0s9 drv=e1000 unused=igb_uio,vfio-pci
        
        No 'Baseband' devices detected
        ==============================
        
        No 'Crypto' devices detected
        ============================
        
        No 'DMA' devices detected
        =========================
        
        No 'Eventdev' devices detected
        ==============================
        
        No 'Mempool' devices detected
        =============================
        
        No 'Compress' devices detected
        ==============================
        
        No 'Misc (rawdev)' devices detected
        ===================================
        
        No 'Regex' devices detected
        ===========================
        ```
        
    3. `sudo python3 ./3rdparty/dpdk/dpdk-21.11/usertools/dpdk-devbind.py --bind=igb_uio enp0s8` enp0s8 드라이버를 dpdk와 호환되는 드라이버인 igb_uio로 전환
        - `sudo python3 ${directory}dpdk-devbind.py --bind=igb_uio ${dev_name}`
    4. `python3 [dpdk-devbind.py](http://dpdk-devbind.py/) --status`  통해 dpdk가 네트워크 드라이버를 점유 했는지 확인
        
        ```bash
        Network devices using DPDK-compatible driver
        ============================================
        0000:00:08.0 '82540EM Gigabit Ethernet Controller 100e' drv=igb_uio unused=e1000,vfio-pci
        
        Network devices using kernel driver
        ===================================
        0000:00:03.0 '82540EM Gigabit Ethernet Controller 100e' if=enp0s3 drv=e1000 unused=igb_uio,vfio-pci *Active*
        0000:00:09.0 '82540EM Gigabit Ethernet Controller 100e' if=enp0s9 drv=e1000 unused=igb_uio,vfio-pci 
        
        No 'Baseband' devices detected
        ==============================
        
        No 'Crypto' devices detected
        ============================
        
        No 'DMA' devices detected
        =========================
        
        No 'Eventdev' devices detected
        ==============================
        
        No 'Mempool' devices detected
        =============================
        
        No 'Compress' devices detected
        ==============================
        
        No 'Misc (rawdev)' devices detected
        ===================================
        
        No 'Regex' devices detected
        ===========================
        ```
        
3. 네트워크 카드 커널에게 되돌려 주기 **`lockard/script/snc_control.sh:dpdk_unload()`**
    1. `ifconfig` 현재 설정 되어있는 네트워크 디바이스 정보를 확인함
        
        ```bash
        enp0s3: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
                inet 10.0.2.15  netmask 255.255.255.0  broadcast 10.0.2.255
                inet6 fe80::1846:66a8:15ff:e439  prefixlen 64  scopeid 0x20<link>
                ether 08:00:27:c3:e1:89  txqueuelen 1000  (Ethernet)
                RX packets 153512  bytes 158030384 (158.0 MB)
                RX errors 0  dropped 0  overruns 0  frame 0
                TX packets 86480  bytes 10893743 (10.8 MB)
                TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
        
        enp0s9: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
                inet6 fe80::546:8e81:e9aa:19d0  prefixlen 64  scopeid 0x20<link>
                ether 08:00:27:84:0b:0b  txqueuelen 1000  (Ethernet)
                RX packets 68  bytes 7483 (7.4 KB)
                RX errors 0  dropped 0  overruns 0  frame 0
                TX packets 47385  bytes 8116826 (8.1 MB)
                TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
        
        lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
                inet 127.0.0.1  netmask 255.0.0.0
                inet6 ::1  prefixlen 128  scopeid 0x10<host>
                loop  txqueuelen 1000  (Local Loopback)
                RX packets 83858  bytes 9719470 (9.7 MB)
                RX errors 0  dropped 0  overruns 0  frame 0
                TX packets 83858  bytes 9719470 (9.7 MB)
                TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
        ```
        
    2. `python3 ./3rdparty/dpdk/dpdk-21.11/usertools/dpdk-devbind.py --status` 네트워크 디바이스 상태 출력
        - 출력 예시 (2-d 와 같은 출력)
    3. `sudo python3 ./3rdparty/dpdk/dpdk-21.11/usertools/dpdk-devbind.py -b e1000 0000:00:08.0` enp0s8 드라이버를 dpdk와 호환되는 드라이버인 igb_uio로 전환
        - `sudo python3 ${directory}dpdk-devbind.py -b $driver_name $bus_name`
    4. `python3 [dpdk-devbind.py](http://dpdk-devbind.py/) --status`  통해 dpdk가 네트워크 드라이버를 점유 했는지 확인
        
        ```bash
        Network devices using kernel driver
        ===================================
        0000:00:03.0 '82540EM Gigabit Ethernet Controller 100e' if=enp0s3 drv=e1000 unused=igb_uio,vfio-pci *Active*
        0000:00:08.0 '82540EM Gigabit Ethernet Controller 100e' if=enp0s8 drv=e1000 unused=igb_uio,vfio-pci
        0000:00:09.0 '82540EM Gigabit Ethernet Controller 100e' if=enp0s9 drv=e1000 unused=igb_uio,vfio-pci
        
        No 'Baseband' devices detected
        ==============================
        
        No 'Crypto' devices detected
        ============================
        
        No 'DMA' devices detected
        =========================
        
        No 'Eventdev' devices detected
        ==============================
        
        No 'Mempool' devices detected
        =============================
        
        No 'Compress' devices detected
        ==============================
        
        No 'Misc (rawdev)' devices detected
        ===================================
        
        No 'Regex' devices detected
        ===========================
        ```
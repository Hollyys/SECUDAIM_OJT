# ⓟⓨⓣⓗⓞⓝ

## **240722 [Jupyter Setting]**

1. 가상머신 상의 주피터 노트북을 PC에서 접속
    - Server PW:
        
        'argon2:$argon2id$v=19$m=10240,t=10,p=8$eEPbD3IHECWMQApmAnT0Nw$hgO41IUBk1o80xfgFesB+PQnK6Y+Yuqrd9WjNJQzeh8’
        
    - 가상환경 실행코드
        
        ```bash
        source jup_notebook/bin/activate
        ```
        
    - [Jupyter notebook Home: Local Host](http://127.0.0.1:8888/tree)
    - [Jupyter notebook Home: 외부접속](http://192.168.1.180:8888/tree)
    - [jupyter notebook 서버설정, 원격접속 설정(feat. ubuntu 22.04) (tistory.com)](https://jongsky.tistory.com/65)
2. whereis jupyter-notebook
    - /home/crossrunway/jupy/jup_notebook/bin/jupyter-notebook
3. git issue
    
    ```bash
    $ git clone https://<user-id>:<token>@<repository url>
    ```
    
    Token인증 방식으로 바뀌어 Token 정보를 입력하여 clone
    
    ```bash
    $ git commit -m
    $ git add .
    $ git push origin main
    ```
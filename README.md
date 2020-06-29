# ComputerNetworkScreenSharing
----------------------------
C++ MFC screen sharing project

```
OS : Windows 10 64bit
Windows SDK : 10.0
IDE : Visual Studio 2019 (v142)
Framework : C++ MFC (v142)
```

Computer Network 및 C++ MFC 공부겸 화면 보드 공유 프로젝트

```
Server 1개에 N대의 Client가 1:N 양방향 데이터 전송 구현.

사용자는 Server 1개와 Client 10개를 구동할 수 있다.
```

```
reference:
    Listener Socket Server
    https://afsdzvcx123.tistory.com/98
    
    Socket Client
    https://afsdzvcx123.tistory.com/99

    dialog에 FormView 붙이기
    https://luckygg.tistory.com/177

    FormView 배경화면 RGB 변경하기
    http://egloos.zum.com/mindol/v/214184

    마우스 Drawing
    https://blog.naver.com/rkttndk/221434286381
```


Server/Client 데이터 전송 규격
```
server -> client 전송 데이터 24바이트
1 byte : event data. c == click, m == move
10 bytes : CPoint data. 좌표 데이터 (1234, 5678) -> x1234y5678
13 bytes : Client ID string. (12 bytes + \0)

client -> server 전송 데이터 11바이트
1 byte : event data. c == click, m == move
10 bytes : CPoint data. 좌표 데이터 (1234, 5678) -> x1234y5678
```

구조
```
Main dialog에서 11개(1개의 Server와 10개의 Client)의 Drawing View를 소유.

각 Drawing View는 Name을 지정.

Drawing View를 변경할경우
1. 기존 화면에 출력하고 있는 Drawing View를 최소화.
2. 변경할 Drawing View를 활성화 및 출력.
3. 변경된 Drawing View에서 저장된 PointData를 출력.

데이터 전송 방식 :
Server :
1. Server에서 Draw
2. Draw 좌표계를 연결되어 있는 Client한테 BroadCast.

Client :
1. Client에서 Draw 좌표계를 Server한테 전송.
2. Server에서 Client의 정보(IP:Port)를 기반으로 좌표계를 저장.
3. Server에 Draw
4. Server가 연결되어 있는 Client한테 BroadCast.
5. Client(Draw한 Client도 포함)가 Draw.
```


#  소스코드 설명
## inline code

MessageQueue.h
```
Log 데이터를 출력하기 위해 사용.

싱글턴 기반의 thread-safety 자료구조
```

PointData.h
```
CPoint의 데이터 x, y 좌표값, 클릭 좌표와
움직이는 좌표 판별 type, client의 id값을 소유.

x -> LONG 타입, x 좌표계
y -> LONG 타입, y 좌표계
type -> char 타입, 'c' : 클릭 좌표계, 'm' : 움직이는 좌표계
id -> char[12], client id 문자열(12자리)

PointData data;
data.x = (LONG)10;
data.y = (LONG)10;
data.id = "000000000000"
data.type = 'c';
```

DrawingQueue.h
```
Process(Server 혹은 Client)가 PointData를 보내기(Send)와 받기(Receive)를
구분하여 보다 직관적으로 데이터를 주고 받을 수 있게 처리.

활성화된 DrawingView가 Server/Client를 구분해 Server와 Client의 데이터가
충돌하는 문제 해결.

각 Process는 내부 변수로 Send, Receive, Draw를 가지고 있다.
thread-safety 자료구조
```

PointDataList.h
```
PointData를 Process 이름(DrawingView.h에 Name)을 기반(key)으로
순차적으로 저장.

싱글턴 기반의 thread-safety 자료구조
```

ClientMap.h
```
Server에서 Client를 구별하기 위해 Client가 접속 시도할 경우
Server에서 Client의 IP와 Port(Client가 Connect 할때마다 Port가 변경됨)를
일정한 패턴으로 저장.

싱글턴 기반의 thread-safety 자료구조
```

ConnectList.h
```
Process(Client)가 중복된 Server로 연결을 방지하기위해
각 Process(Client) 이름마다 연결한 Server의 IP:PORT 를 기록.

싱글턴 기반의 thread-safety 자료구조
```

## socket code

CClient
```
CSocket를 상속받은 Client 클래스.

ListenSocket에 Accept해온 Client를 저장.
```

CListenSocket
```
CAsyncSocket를 상속받은 Socket Server 클래스.

OnAccept에서 감지된 Client를 저장.
Client에게 데이터를 전송하기위한 BroadCast 기능.
```

CClientSocket
```
CSocket를 상속받은 Socket Client 클래스.

Socket Server에서 보내온 데이터를 받아
DrawingQueue::GetReceiveQueue()->Push 진행.
```

## view code

DrawingView
```
CFormView를 상속받은 그림 그리는 도화지 View

Name 변수로 각 Drawing View마다 각자의 이름을 지정.

기본 역할 :
    마우스 좌클릭, 좌클릭중 움직이는 마우스 포인터를 따라 화면에 그림을 그려주는 view.
Socket Server가 활성호 될 경우 DrawingQueue::GetSendQueue()->Push()을 통해
연결된 Client한테 데이터를 전달한다.

Socket Client가 활성화 될 경우 DrawingQueue::GetReceiveQueue()->Pop()을 통해
Server가 전달해준 데이터를 기반으로 화면에 그려준다.
```

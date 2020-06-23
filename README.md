# ComputerNetworkScreenSharing
----------------------------
C++ MFC screen sharing project

```
OS : Windows 10 64bit
Windows SDK : 10.0
IDE : Visual Studio 2019 (v142)
Framework : C++ MFC (v142)
```

Computer Network 및 C++ MFC 공부겸 화면공유 프로젝트

```
현재 오직 Server/Client가 1:1로 양방향 데이터 전송 구현
```

```
server -> client 전송 데이터 24바이트
1 byte : event data. c == click, m == move
10 bytes : CPoint data. 좌표 데이터 (1234, 5678) -> x1234y5678
13 bytes : Client ID string. (12 bytes + \0)

client -> server 전송 데이터 11바이트
1 byte : event data. c == click, m == move
10 bytes : CPoint data. 좌표 데이터 (1234, 5678) -> x1234y5678
```

#  소스코드 설명
## inline code

MessageQueue.h
```
싱글턴 기반 thread safe한 concurrent queue를 사용.
std::wstring 타입으로 Log정보를 기록에 사용.

#include "MessageQueue.h"
MessageQueue::GetInstance()->Push(L"test");
std::wstring wstr = MessageQueue::GetInstance()->Pop();
```

DrawingQueue.h
```
싱글턴 기반 thread safe한 concurrent queue를 사용.
PointData 타입으로 Send, Receive 2개의 queue를 가지고 사용.

#include
PointData data;
DrawingQueue::GetSendQueue()->Push(data);
DrawingQueue::GetReceiveQueue()->Push(data);
data = DrawingQueue::GetSendQueue()->Pop();
data = DrawingQueue::GetReceiveQueue()->Pop();
```

PointData.h
```
CPoint의 데이터 x, y 좌표값, 클릭 좌표와 움직이는 좌표 판별 type, client의 id값을 소유.
type -> 'c' : 클릭 좌표계, 'm' : 움직이는 좌표계
id -> client id 문자열(12자리)

PointData data;
data.x = (LONG)10;
data.y = (LONG)10;
data.id = "000000000000"
data.type = 'c';
```

ClientMap.h
```
C++ STL map 컨테이너를 이용해 <key, value> 페어를 저장
key : IP:Port (ex : 127.0.0.1:1482)
value : 12자리 문자열 (ex : 000000000001 ~~ 999999999999)

client가 server에 accept할 경우 server측에서 client의 ip와 port를 key로, 정수를 12자리 문자열로 변환해 저장.
```

## socket code

```
reference:
    https://afsdzvcx123.tistory.com/98
    https://afsdzvcx123.tistory.com/99
```

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

```
reference :
    dialog에 FormView 붙이기
    https://luckygg.tistory.com/177

    FormView 배경화면 이미지 변경하기
    http://egloos.zum.com/mindol/v/214184

    마우스 Drawing
    https://blog.naver.com/rkttndk/221434286381
```

DrawingView
```
CFormView를 상속받은 그림 그리는 도화지 View

기본 역할 :
    마우스 좌클릭, 좌클릭중 움직이는 마우스 포인터를 따라 화면에 그림을 그려주는 view.

Socket Client가 활성화 될 경우 DrawingQueue::GetReceiveQueue()->Pop()을 통해
Server가 전달해준 데이터를 기반으로 화면에 그려준다.
```

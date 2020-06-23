# ComputerNetworkScreenSharing
----------------------------
C++ MFC screen sharing project

```
OS : Windows 10 64bit
Windows SDK : 10.0
IDE : Visual Studio 2019 (v142)
Framework : C++ MFC (v142)
```

Computer Network �� C++ MFC ���ΰ� ȭ����� ������Ʈ

```
���� ���� Server/Client�� 1:1�� ����� ������ ���� ����
```

```
server -> client ���� ������ 24����Ʈ
1 byte : event data. c == click, m == move
10 bytes : CPoint data. ��ǥ ������ (1234, 5678) -> x1234y5678
13 bytes : Client ID string. (12 bytes + \0)

client -> server ���� ������ 11����Ʈ
1 byte : event data. c == click, m == move
10 bytes : CPoint data. ��ǥ ������ (1234, 5678) -> x1234y5678
```

#  �ҽ��ڵ� ����
## inline code

MessageQueue.h
```
�̱��� ��� thread safe�� concurrent queue�� ���.
std::wstring Ÿ������ Log������ ��Ͽ� ���.

#include "MessageQueue.h"
MessageQueue::GetInstance()->Push(L"test");
std::wstring wstr = MessageQueue::GetInstance()->Pop();
```

DrawingQueue.h
```
�̱��� ��� thread safe�� concurrent queue�� ���.
PointData Ÿ������ Send, Receive 2���� queue�� ������ ���.

#include
PointData data;
DrawingQueue::GetSendQueue()->Push(data);
DrawingQueue::GetReceiveQueue()->Push(data);
data = DrawingQueue::GetSendQueue()->Pop();
data = DrawingQueue::GetReceiveQueue()->Pop();
```

PointData.h
```
CPoint�� ������ x, y ��ǥ��, Ŭ�� ��ǥ�� �����̴� ��ǥ �Ǻ� type, client�� id���� ����.
type -> 'c' : Ŭ�� ��ǥ��, 'm' : �����̴� ��ǥ��
id -> client id ���ڿ�(12�ڸ�)

PointData data;
data.x = (LONG)10;
data.y = (LONG)10;
data.id = "000000000000"
data.type = 'c';
```

ClientMap.h
```
C++ STL map �����̳ʸ� �̿��� <key, value> �� ����
key : IP:Port (ex : 127.0.0.1:1482)
value : 12�ڸ� ���ڿ� (ex : 000000000001 ~~ 999999999999)

client�� server�� accept�� ��� server������ client�� ip�� port�� key��, ������ 12�ڸ� ���ڿ��� ��ȯ�� ����.
```

## socket code

```
reference:
    https://afsdzvcx123.tistory.com/98
    https://afsdzvcx123.tistory.com/99
```

CClient
```
CSocket�� ��ӹ��� Client Ŭ����.

ListenSocket�� Accept�ؿ� Client�� ����.
```

CListenSocket
```
CAsyncSocket�� ��ӹ��� Socket Server Ŭ����.

OnAccept���� ������ Client�� ����.
Client���� �����͸� �����ϱ����� BroadCast ���.
```

CClientSocket
```
CSocket�� ��ӹ��� Socket Client Ŭ����.

Socket Server���� ������ �����͸� �޾�
DrawingQueue::GetReceiveQueue()->Push ����.
```

## view code

```
reference :
    dialog�� FormView ���̱�
    https://luckygg.tistory.com/177

    FormView ���ȭ�� �̹��� �����ϱ�
    http://egloos.zum.com/mindol/v/214184

    ���콺 Drawing
    https://blog.naver.com/rkttndk/221434286381
```

DrawingView
```
CFormView�� ��ӹ��� �׸� �׸��� ��ȭ�� View

�⺻ ���� :
    ���콺 ��Ŭ��, ��Ŭ���� �����̴� ���콺 �����͸� ���� ȭ�鿡 �׸��� �׷��ִ� view.

Socket Client�� Ȱ��ȭ �� ��� DrawingQueue::GetReceiveQueue()->Pop()�� ����
Server�� �������� �����͸� ������� ȭ�鿡 �׷��ش�.
```

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
CPoint�� ������ x, y ��ǥ��, Ŭ�� ��ǥ�� �����̴� ��ǥ �Ǻ��� type�� ����.
type -> 'c' : Ŭ�� ��ǥ��, 'm' : �����̴� ��ǥ��

PointData data;
data.x = (LONG)10;
data.y = (LONG)10;
data.type = 'c';
```

## socket code

```
reference:
    https://afsdzvcx123.tistory.com/98
    https://afsdzvcx123.tistory.com/99
```

```
��ӵ� ������ : 
    12byte ������
    cx0123y0123 : Ŭ�� ���� (123, 123)
    mx0889y0452 : �����̴� ���� (889, 452)
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

Socket Server���� ������ ��ӵ� �����͸� �޾�
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
Server�� �������� ��ӵ� �����͸� ������� ȭ�鿡 �׷��ش�.
```

# ComputerNetworkScreenSharing
----------------------------
C++ MFC screen sharing project

```
OS : Windows 10 64bit
Windows SDK : 10.0
IDE : Visual Studio 2019 (v142)
Framework : C++ MFC (v142)
```

Computer Network �� C++ MFC ���ΰ� ȭ�� ���� ���� ������Ʈ

```
Server 1���� N���� Client�� 1:N ����� ������ ���� ����.

����ڴ� Server 1���� Client 10���� ������ �� �ִ�.
```

```
reference:
    Listener Socket Server
    https://afsdzvcx123.tistory.com/98
    
    Socket Client
    https://afsdzvcx123.tistory.com/99

    dialog�� FormView ���̱�
    https://luckygg.tistory.com/177

    FormView ���ȭ�� RGB �����ϱ�
    http://egloos.zum.com/mindol/v/214184

    ���콺 Drawing
    https://blog.naver.com/rkttndk/221434286381
```


Server/Client ������ ���� �԰�
```
server -> client ���� ������ 24����Ʈ
1 byte : event data. c == click, m == move
10 bytes : CPoint data. ��ǥ ������ (1234, 5678) -> x1234y5678
13 bytes : Client ID string. (12 bytes + \0)

client -> server ���� ������ 11����Ʈ
1 byte : event data. c == click, m == move
10 bytes : CPoint data. ��ǥ ������ (1234, 5678) -> x1234y5678
```

����
```
Main dialog���� 11��(1���� Server�� 10���� Client)�� Drawing View�� ����.

�� Drawing View�� Name�� ����.

Drawing View�� �����Ұ��
1. ���� ȭ�鿡 ����ϰ� �ִ� Drawing View�� �ּ�ȭ.
2. ������ Drawing View�� Ȱ��ȭ �� ���.
3. ����� Drawing View���� ����� PointData�� ���.

������ ���� ��� :
Server :
1. Server���� Draw
2. Draw ��ǥ�踦 ����Ǿ� �ִ� Client���� BroadCast.

Client :
1. Client���� Draw ��ǥ�踦 Server���� ����.
2. Server���� Client�� ����(IP:Port)�� ������� ��ǥ�踦 ����.
3. Server�� Draw
4. Server�� ����Ǿ� �ִ� Client���� BroadCast.
5. Client(Draw�� Client�� ����)�� Draw.
```


#  �ҽ��ڵ� ����
## inline code

MessageQueue.h
```
Log �����͸� ����ϱ� ���� ���.

�̱��� ����� thread-safety �ڷᱸ��
```

PointData.h
```
CPoint�� ������ x, y ��ǥ��, Ŭ�� ��ǥ��
�����̴� ��ǥ �Ǻ� type, client�� id���� ����.

x -> LONG Ÿ��, x ��ǥ��
y -> LONG Ÿ��, y ��ǥ��
type -> char Ÿ��, 'c' : Ŭ�� ��ǥ��, 'm' : �����̴� ��ǥ��
id -> char[12], client id ���ڿ�(12�ڸ�)

PointData data;
data.x = (LONG)10;
data.y = (LONG)10;
data.id = "000000000000"
data.type = 'c';
```

DrawingQueue.h
```
Process(Server Ȥ�� Client)�� PointData�� ������(Send)�� �ޱ�(Receive)��
�����Ͽ� ���� ���������� �����͸� �ְ� ���� �� �ְ� ó��.

Ȱ��ȭ�� DrawingView�� Server/Client�� ������ Server�� Client�� �����Ͱ�
�浹�ϴ� ���� �ذ�.

�� Process�� ���� ������ Send, Receive, Draw�� ������ �ִ�.
thread-safety �ڷᱸ��
```

PointDataList.h
```
PointData�� Process �̸�(DrawingView.h�� Name)�� ���(key)����
���������� ����.

�̱��� ����� thread-safety �ڷᱸ��
```

ClientMap.h
```
Server���� Client�� �����ϱ� ���� Client�� ���� �õ��� ���
Server���� Client�� IP�� Port(Client�� Connect �Ҷ����� Port�� �����)��
������ �������� ����.

�̱��� ����� thread-safety �ڷᱸ��
```

ConnectList.h
```
Process(Client)�� �ߺ��� Server�� ������ �����ϱ�����
�� Process(Client) �̸����� ������ Server�� IP:PORT �� ���.

�̱��� ����� thread-safety �ڷᱸ��
```

## socket code

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

DrawingView
```
CFormView�� ��ӹ��� �׸� �׸��� ��ȭ�� View

Name ������ �� Drawing View���� ������ �̸��� ����.

�⺻ ���� :
    ���콺 ��Ŭ��, ��Ŭ���� �����̴� ���콺 �����͸� ���� ȭ�鿡 �׸��� �׷��ִ� view.
Socket Server�� Ȱ��ȣ �� ��� DrawingQueue::GetSendQueue()->Push()�� ����
����� Client���� �����͸� �����Ѵ�.

Socket Client�� Ȱ��ȭ �� ��� DrawingQueue::GetReceiveQueue()->Pop()�� ����
Server�� �������� �����͸� ������� ȭ�鿡 �׷��ش�.
```

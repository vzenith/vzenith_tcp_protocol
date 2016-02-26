# TCP SDK设计

vzenith tcp protocol 的核心代码移植来自于google的代码，

## Session

只运行在`async_thread`单线程中

- 连接服务端
- 断开连接
- 维持心跳数据
- 发送和接收数据

## SessionManager

运行在`queue_thread`和`async_thread`中

- 创建并且管理Session
- 接收上一层任务，并且分发给相应的Session
	- 接收创建Session的任务
	- 接收需要发送的数据
	- 接收关闭Session的任务
- 接收Session的事件，传送给上一层
	- 接收Session的连接正常的消息
	- 接收Session的连接失败的消息
	- 接收Session的正常消息

## Queue Layer

运行在`queue_thread`和`sync_thread`中
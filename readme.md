
### Project ChatRoom

> 编程为什么有趣？作为回报，它的从业者期望得到什么样的快乐？
>
> 首先是一种创建事物的纯粹快乐。如同小孩在玩泥巴时感到愉快一样，成年人喜欢创建事物，特别是自己进行设计。我想这种快乐是上帝创造世界的折射，一种呈现在每片独特、崭新的树叶和雪花上的喜悦。
>
> 其次，快乐来自于开发对其他人有用的东西。内心深处，我们期望其他人使用我们的劳动成果，并能对他们有所帮助。从这个方面，这同小孩用粘土为"爸爸办公室"捏制铅笔盒没有本质的区别。
>
> 第三是整个过程体现出魔术般的力量--将相互啮合的零部件组装在一起，看到它们精妙地运行，得到预先所希望的结果。比起弹珠游戏或点唱机所具有的迷人魅力，程序化的计算机毫不逊色。
>
> 第四是学习的乐趣，来自于这项工作的非重复特性。人们所面临的问题，在某个或其它方面总有些不同。因而解决问题的人可以从中学习新的事物：有时是实践上的，有时是理论上的，或者兼而有之。
>
> 最后，乐趣还来自于工作在如此易于驾驭的介质上。程序员，就像诗人一样，几乎仅仅工作在单纯的思考中。程序员凭空地运用自己的想象，来建造自己的"城堡"。很少有这样的介质--创造的方式如此得灵活，如此得易于精炼和重建，如此得容易实现概念上的设想。（不过我们将会看到，容易驾驭的特性也有它自己的问题）然而程序毕竟同诗歌不同，它是实实在在的东西；可以移动和运行，能独立产生可见的输出；能打印结果，绘制图形，发出声音，移动支架。神话和传说中的魔术在我们的时代已变成了现实。在键盘上键入正确的咒语，屏幕会活动、变幻，显示出前所未有的或是已经存在的事物。
>
> -- 人月神话 焦油坑

请从今天开始体会构建「大型」软件的乐趣吧！

实现 GNU/Linux 环境下采用C/S模型的简易聊天室。

### 项目需求

#### 账号管理

- 实现登录、注册、注销
- 实现找回密码（提高）
- 实现验证码注册/找回（邮件/手机号等）（提高）
- 实现数据加密（提高）

#### 好友管理

- 实现好友的添加、删除、查询操作
- 实现显示好友在线状态
- 禁止不存在好友关系的用户间的私聊
- 实现屏蔽好友消息
- 实现好友间聊天

#### 群管理

- 实现群组的创建、解散
- 实现用户申请加入群组
- 实现用户查看已加入的群组
- 实现群组成员退出已加入的群组
- 实现群组成员查看群组成员列表
- 实现群主对群组管理员的添加和删除
- 实现群组管理员批准用户加入群组
- 实现群组管理员/群主从群组中移除用户
- 实现群组内聊天功能

#### 聊天功能

对于 **私聊和群组** 的聊天功能**均需要**实现：

- 实现查看历史消息记录
- 实现用户间在线聊天
- 实现在线用户对离线用户发送消息，离线用户上线后获得通知
- 实现文件发送的断点续传（提高）
- 实现在线发送文件
- 实现在线用户对离线用户发送文件，离线用户上线后获得通知/接收（提高）
- 实现用户在线时,消息的实时通知
  - 收到好友请求
  - 收到私聊
  - 收到加群申请
  - ...

#### 其他要求

- 使用 C++/Java/Golang 编程语言(No C-Language)
- 使用 I/O 多路复用完成本项目

  > C++/Golang – epoll ET 模式
  >
  > Java – netty 和 NIO
  >
- 使用数据库完成数据存储

  > C++/Golang --  Redis 或 levelDB
  >
  > Java -- Mysql/mariadb/任意RDBMS
  >
- 实现图形化界面（超级提高）
- 数据库中数据的存储和取用使用序列化和反序列化完成(Json/Proto等)(提高)
- 撰写用户文档
- 支持大量客户端同时访问
- 实现服务器日志，记录服务器的状态信息
- 实现具有高稳定性的客户端和服务器，防止在用户非法输入时崩溃或异常

  - 实现 TCP 心跳检测
- 界面美观

注：

- 功能和交互逻辑可参考 QQ、微信或 IRC。请注意检查功能间的逻辑关系是否合理。例如:

  - 不该允许已被管理员从群组中移除的用户在群组中发送消息
  - 当用户注销后，如果他们决定重新注册使用相同的用户名，他们将无法直接使用先前用户的信息
- 代码量预计 5000 行，请妥善安排完成计划。
- 请将源码合理的拆分在多个文件中，项目的构建使用

  > C++   ---  bazel（推荐）、cmake、ninja、makefile（不推荐）中任意一个
  >
  > Java  ---- Maven/Gradle 任意一个.
  >
- 项目引入外部的日志库或单元测试库。

  > C++  — gtest glog 等
  >
  > Java – Junit Log4j/log4j2等
  >
- 提高功能必须选择1，2个完成

对使用 Redis 的同学的特殊规则：

请把 Redis 仅作为服务端的存储后端。

下面的几种用法在本次项目中不被接受：

- 使用 Redis 的订阅发布
- 允许客户端链接 Redis
- 使用多个 Redis

请注意：
本项目每人最多仅允许启动一个 Redis 实例。且仅允许 服务端 连接 Redis。

违反上面的规定将无法通过验收。

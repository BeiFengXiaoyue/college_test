# 🎓 在线教育平台 (Online Education Platform)

基于 **Qt 6 / C++ / QML** 构建的现代化在线教育桌面应用，支持课程直播、录播回放、互动白板、弹幕系统和在线作业管理等核心教学功能。

---

## ✨ 功能特性

### 📚 课程管理
- 课程创建、编辑、发布、归档
- 课时管理（直播/录播类型）
- 课程搜索与分类浏览
- 教师专属管理面板

### 📺 直播课堂 (RTMP协议模拟)
- 基于 `Qt Multimedia` 的音视频推拉流
- 直播状态实时显示
- 支持摄像头/桌面捕获推流
- 低延迟播放体验

### ▶️ 录播回放
- 标准播放控制（播放/暂停/进度拖拽）
- 倍速播放（0.5x ~ 2.0x）
- 音量调节
- 学习进度自动追踪（每5秒同步）
- 完成标记

### ✏️ 互动白板
- 6种绘图工具：铅笔、直线、矩形、圆形、文字、橡皮擦
- 颜色选择器（18种预设颜色）
- 笔触大小调节（1-20px）
- 撤销/重做操作
- 绘制操作可记录与回放

### 💬 弹幕系统
- 滚动弹幕（从右向左）
- 顶部/底部固定弹幕（淡入淡出）
- 自定义颜色与字号
- 弹幕开关控制
- 弹幕数据持久化存储

### 📝 在线作业
- **教师端**：布置作业、设置截止日期、评分批改、写评语
- **学生端**：在线提价（文本+附件）、查看成绩
- 状态流转：未提价 → 已提价 → 已批改

### ⚡ 高性能并发
- `QThreadPool` 线程池统一管理
- `QtConcurrent` 异步数据库操作
- 界面永不阻塞

### 🗄️ 数据持久化
- SQLite 关系型数据库
- 8张核心业务表
- 完善的外键约束与索引
- WAL 模式提升并发性能

---

## 🏗️ 系统架构

```
┌─────────────────────────────────────────────────────┐
│                    QML 前端层                         │
│  7个页面 + 5个可复用组件 + 主题系统                    │
├─────────────────────────────────────────────────────┤
│                    C++ 服务层                         │
│  AuthService / CourseService / HomeworkService /     │
│  LiveStreamService                                   │
├─────────────────────────────────────────────────────┤
│                  C++ 核心管理层                       │
│  AppCore / ThreadPoolManager / ConfigManager         │
│  DatabaseManager + 5个 DAO                           │
├─────────────────────────────────────────────────────┤
│                  数据/存储层                          │
│  SQLite DB / 媒体文件 / JSON 配置文件                 │
└─────────────────────────────────────────────────────┘
```

### 数据流
```
QML Page  ──(Q_INVOKABLE)──>  C++ Service  ──> DAO ──> SQLite
                                         └──> QThreadPool (异步)
C++ Service  ──(信号)──> QML (属性绑定/Connections)
```

---

## 🛠️ 技术栈

| 层级 | 技术 | 说明 |
|------|------|------|
| **语言** | C++17 / QML / JavaScript | 混合编程 |
| **UI框架** | Qt Quick / Qt Quick Controls 2 | 响应式界面 |
| **多媒体** | Qt Multimedia (Qt 6) | 音视频推拉流与播放 |
| **数据库** | SQLite 3 (via Qt SQL) | 本地数据存储 |
| **并发** | QThreadPool / QtConcurrent | 线程池异步任务 |
| **网络** | Qt Network | RTMP 流传输 |
| **构建** | qmake | 项目构建系统 |

### 依赖模块
```
QT += quick quickcontrols2 multimedia multimediawidgets sql concurrent network
```

---

## 📦 构建指南

### 前置条件
- **Qt 6.2+**（推荐 6.5 LTS 或更高）
- **Qt Creator** 或命令行工具链
- C++17 兼容的编译器（MSVC 2022 / GCC 11+ / Clang 14+）

### 构建步骤

**方式一：Qt Creator**
1. 打开 `OnlineEduPlatform.pro`
2. 配置 Qt 6 kit
3. 点击「构建」或按 `Ctrl+R` 运行

**方式二：命令行**
```bash
# 进入项目目录
cd OnlineEduPlatform

# 创建构建目录
mkdir build && cd build

# 生成构建文件
qmake ../OnlineEduPlatform.pro

# 编译
nmake          # Windows (MSVC)
make           # Linux/macOS (GCC/Clang)
jom            # Windows (Qt JOM, 并行编译)

# 运行
./build/release/OnlineEduPlatform.exe     # Windows
./build/OnlineEduPlatform                # Linux/macOS
```

---

## 🚀 快速开始

### 测试账号
| 角色 | 用户名 | 密码 |
|------|--------|------|
| 👨‍🏫 教师 | `teacher` | `123456` |
| 👨‍🎓 学生 | `student` | `123456` |

### 教师工作流
1. 用 `teacher / 123456` 登录
2. 进入「课程列表」→ 点击「+ 新建课程」
3. 填写课程信息 → 发布课程
4. 进入课程详情 → 「+ 添加课时」
5. 进入直播教室 → 使用白板和弹幕进行教学
6. 布置作业 → 批改学生提交 → 打分写评语

### 学生工作流
1. 用 `student / 123456` 登录
2. 浏览已发布的课程列表
3. 进入课程 → 选择课时观看
4. 在直播教室发送弹幕互动
5. 提价作业（文本+附件）
6. 查看成绩与教师评语

---

## 📁 项目结构

```
OnlineEduPlatform/
├── OnlineEduPlatform.pro       # Qt qmake 项目文件
├── README.md                   # 项目文档（本文件）
├── 操作手册.md                  # 用户操作手册
├── src/                        # C++ 后端源码
│   ├── main.cpp                # 程序入口
│   ├── core/                   # 核心管理模块
│   │   ├── appcore.h/cpp       # 应用核心
│   │   ├── threadpoolmanager.* # 线程池管理
│   │   └── configmanager.*     # 配置管理
│   ├── database/               # 数据库层 (DAO)
│   │   ├── databasemanager.*   # 数据库连接
│   │   ├── userdao.*           # 用户数据访问
│   │   ├── coursedao.*         # 课程数据访问
│   │   ├── homeworkdao.*       # 作业数据访问
│   │   └── learningrecorddao.* # 学习记录数据访问
│   ├── media/                  # 媒体模块
│   │   ├── livestreammanager.* # 直播推拉流
│   │   ├── playbackmanager.*   # 回放管理
│   │   └── streamrecorder.*    # 录制功能
│   ├── models/                 # 数据模型
│   │   ├── usermodel.h
│   │   ├── coursemodel.h
│   │   ├── homeworkmodel.h
│   │   └── learningrecordmodel.h
│   └── services/               # 业务服务层
│       ├── authservice.*       # 认证服务
│       ├── courseservice.*     # 课程服务
│       ├── homeworkservice.*   # 作业服务
│       └── livestreamservice.* # 直播服务
├── qml/                        # QML 前端
│   ├── main.qml                # 应用入口
│   ├── theme/                  # 主题系统
│   │   ├── Theme.qml           # 全局主题定义
│   │   └── qmldir              # 单例注册
│   ├── pages/                  # 页面
│   │   ├── LoginPage.qml       # 登录页
│   │   ├── DashboardPage.qml   # 仪表盘
│   │   ├── CourseListPage.qml  # 课程列表
│   │   ├── CourseDetailPage.qml# 课程详情
│   │   ├── LiveClassroomPage.qml# 直播教室
│   │   ├── PlaybackPage.qml    # 录播回放
│   │   └── HomeworkPage.qml    # 作业管理
│   └── components/             # 可复用组件
│       ├── ActionCard.qml      # 快捷操作卡片
│       ├── VideoPlayer.qml     # 视频播放器
│       ├── LiveStreamView.qml  # 直播视图
│       ├── Whiteboard.qml      # 互动白板
│       ├── DanmakuOverlay.qml  # 弹幕系统
│       └── HomeworkSubmission.qml # 作业提价
├── resources/                  # 资源文件
│   ├── resources.qrc           # Qt 资源索引
│   └── sql/init.sql            # 数据库初始化脚本
└── .gitignore                  # Git 忽略规则
```

---

## 🗄️ 数据库设计

| 表名 | 说明 | 主要字段 |
|------|------|---------|
| `users` | 用户表 | id, username, password_hash, role, avatar |
| `courses` | 课程表 | id, title, teacher_id, status, category |
| `course_lessons` | 课时表 | id, course_id, type(live/replay), video_url |
| `homework` | 作业布置表 | id, lesson_id, deadline, max_score |
| `homework_submissions` | 作业提交表 | id, homework_id, student_id, score, comment |
| `learning_records` | 学习记录表 | user_id, lesson_id, progress, watch_position |
| `danmaku_messages` | 弹幕表 | lesson_id, user_id, content, timestamp, color |
| `whiteboard_records` | 白板记录表 | lesson_id, action_data, action_type |

---

## 📋 开发计划

- [x] 项目骨架搭建
- [x] 数据库设计与 DAO 层
- [x] 业务服务层
- [x] 媒体模块（直播/回放/录制）
- [x] QML 前端页面
- [x] 互动白板
- [x] 弹幕系统
- [x] 作业管理系统

### 未来计划
- [ ] 实时通信（WebSocket 白板同步）
- [ ] 屏幕共享功能
- [ ] 在线考试系统
- [ ] 课程统计分析面板
- [ ] 多语言国际化 (i18n)
- [ ] 单元测试覆盖

---

## 📄 许可证

本项目仅供学习交流使用。

---

## 👨‍💻 关于

基于 Qt 6 框架构建的在线教育桌面应用，展示了现代 C++ 与 QML 混合编程的最佳实践。

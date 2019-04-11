# linux下的网络调试

- 查看端口信息

  lsof -i:[port]

- 查看端口状态

  netstat -an | grep [port]

- 杀死进程

  kill -9 [PID]


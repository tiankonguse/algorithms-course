#!/usr/bin/env bash
# run.sh —— 启动算法教程静态服务（固定端口 8080）
# 用法:
#   ./run.sh            启动（端口被占则先停旧再起新）
#   ./run.sh stop       停止
#   ./run.sh status     查看状态

set -e
PORT=8080
DIR="$(cd "$(dirname "$0")" && pwd)"
PID_FILE="$DIR/.server.pid"
LOG_FILE="$DIR/.server.log"

kill_old() {
  if [ -f "$PID_FILE" ]; then
    PID=$(cat "$PID_FILE")
    if kill -0 "$PID" 2>/dev/null; then
      kill "$PID" 2>/dev/null || true
      sleep 0.3
    fi
    rm -f "$PID_FILE"
  fi
  if command -v lsof >/dev/null 2>&1 && lsof -ti tcp:"$PORT" >/dev/null 2>&1; then
    lsof -ti tcp:"$PORT" | xargs kill 2>/dev/null || true
    sleep 0.3
  fi
}

show_status() {
  if [ -f "$PID_FILE" ] && kill -0 "$(cat "$PID_FILE")" 2>/dev/null; then
    echo "运行中  PID=$(cat "$PID_FILE")  端口=$PORT  访问=http://localhost:$PORT/index.html"
  else
    echo "未运行"
  fi
}

case "${1:-start}" in
  stop)
    kill_old
    echo "已停止"
    ;;
  status)
    show_status
    ;;
  start|"")
    kill_old
    cd "$DIR"
    nohup python3 -m http.server "$PORT" >"$LOG_FILE" 2>&1 &
    PID=$!
    echo "$PID" >"$PID_FILE"
    echo "已启动  PID=$PID  端口=$PORT  访问=http://localhost:$PORT/index.html"
    echo "日志:  $LOG_FILE"
    echo "停止:  $0 stop"
    ;;
  *)
    echo "用法: $0 [start|stop|status]"
    exit 1
    ;;
esac
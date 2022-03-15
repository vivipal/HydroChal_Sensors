while true
do
pgrep -x IMU_exe >/dev/null && echo "IMU_exe found" || echo "IMU_exe not found" && ../executable/IMU_exe
pgrep -x logger_exe >/dev/null && echo "logger_exe found" || echo "logger_exe not found" && ../executable/logger_exe
pgrep -x WS_exe >/dev/null && echo "WS_exe found" || echo "WS_exe not found" && ../executable/WS_exe
pgrep -x rx_exe >/dev/null && echo "rx_exe found" || echo "rx_exe not found" && ../executable/rx_exe

sleep 2
done

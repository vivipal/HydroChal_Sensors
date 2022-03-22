
while IFS='#' read c1 c2
do line="$c1 $c2"
	if [ "$c1" != "" ]
	then
		echo "$c1"
		arr+=("$c1")
	fi
done < config.txt


# arr composition :

# IMU device
# IMUpath to save

# Logger binary data path
# Logger path to save log

# RX device path

# WS device
# WS path to save


while true
do
pgrep -x IMU_exe > /dev/null  && echo "IMU_exe found\n" || (echo "IMU_exe not found\n" && ../executable/IMU_exe $arr[0] $arr[1] >/dev/null&)
echo "\n"
pgrep -x logger_exe  > /dev/null && echo  "logger_exe found\n" || (echo "logger_exe not found\n" && ../executable/logger_exe $arr[2] $arr[3] >/dev/null&)
echo "\n"
pgrep -x rx_exe  > /dev/null && echo  "rx_exe found\n" || (echo "rx_exe not found\n" && ../executable/rx_exe $arr[4]>/dev/null&)
echo  "\n"
pgrep -x WS_exe > /dev/null  && echo  "WS_exe found\n" || (echo "WS_exe not found\n" && ../executable/WS_exe $arr[5] $arr[6] >/dev/null&)
echo "\n"

sleep 2
done

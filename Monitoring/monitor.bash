
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


pgrep -x IMU_exe > /dev/null  && echo -e "IMU_exe found" || (echo -e "IMU_exe not found" && ../executable/IMU_exe ${arr[0]} ${arr[1]} >/dev/null&)
pgrep -x logger_exe  > /dev/null && echo -e  "logger_exe found" || (echo -e "logger_exe not found" && ../executable/logger_exe ${arr[2]} ${arr[3]} >/dev/null&)
pgrep -x rx_exe  > /dev/null && echo -e  "rx_exe found" || (echo -e "rx_exe not found" && ../executable/rx_exe ${arr[4]} > /dev/null&)
pgrep -x WS_exe > /dev/null  && echo -e  "WS_exe found" || (echo -e "WS_exe not found" && ../executable/WS_exe ${arr[5]} ${arr[6]} >/dev/null&)


echo -e "\n"

sleep 2
done

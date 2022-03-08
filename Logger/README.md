# LOGGER

```console
gcc main2.c -o logger.exe

./logger.exe [path_to_save_the_log] [file1.bin] [file2.bin]

#example :
./logger.exe  ../Log/  WIMDA.bin IMU.BIN
./logger.exe  ../Log/  ../Sensors_data/*.bin

```


## OLD Logger

```console
gcc main.c -o logger.exe

./logger.exe [path_to_data_binaries] [path_to_save_the_log]

#example :
./logger.exe ../Sensors_data/ ../Log/

```

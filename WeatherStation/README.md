# Presentation

This code is used to read data from a weather Station connected via a serial RS232 connector.
It has been tested with the 'Airmar 22WX weather station'.

The program selects only wantable data from each sentences (WIMDA, GPRMC, HCHDG)
This can be easily changed by adapting this part of the code :

```c
// nombre d'info a recup pour chaque trame NMEA
#define WIMDA_len 2
#define HCHDG_len 3
#define GPRMC_len 9

// indices des infos a recup pour chaque trame NMEA
int WIMDA_indx[WIMDA_len] = {14,18};
int HCHDG_indx[HCHDG_len] = {0,3,4};
int GPRMC_indx[GPRMC_len] = {0,1,2,3,4,5,6,7,8};
```

The program finally stores last selected data from each sentence in a binary file (one for each sentence *WIMDA.bin*, *GPRMC.bin* and *HCHDG.bin*)


# How to compile it :

```console
gcc main.c -o getWS.exe
```

# How to run it :

```console
./getWS.exe

# OR by providing device and path to save data :

./getWS.exe /dev/[your_device] [path_to_save_the_file]
```

# Selected data

Here are the default selected data for each sentence

**GPRMC** :

| Indices |  Description | Selected |
| :-----: | :----------: | :------: |
| 1 | UTC Time (hhmmss) | YES|
| 2 | GPS status | YES |
| 3 | Lat | YES |
| 4 | N/S | YES |
| 5 | Lon | YES |
| 6 | E/W | YES |
| 7 | Speed over ground knot | YES |
| 8 | Course over ground degrees True | YES |
| 9 | Date (ddmmyy) | YES |
| 10 | Magnetic variation degress E/W | NO |
| 11 | E/Wardu | NO |
| 12 | Mode indicator | NO |

**WIMDA** :

| Indices | Description | Selected |
| :-----: | :---------: | :------: |
| 1 | Pressure inches mercury | NO |
| 2 | Inches| NO |
| 3  | Pressure bars| NO |
| 4 | Bars| NO |
| 5 | Air temperature °C| NO |
| 6 | Celsius| NO |
| 7  | Water Temperature| NO |
| 8 | Celsius| NO |
| 9 | Humidity % | NO |
| 10 |  | NO |
| 11 | Temperature de condensation| NO |
| 12 | Celsius| NO |
| 13 | Wind direction degrees True| NO |
| 14 | True| NO |
| 15 | Wind direction degrees Magnetic| YES |
| 16 | Magnetic| NO |
| 17 | Wind speed knots| NO |
| 18 | Knots| NO |
| 19 | Wind speed m/s| YES |
| 20 | M/s| NO |

**HCHDG** :

| Indices | Description | Selected |
| :-----: | :---------: | :------: |
| 1 | Magnetic heading | YES |
| 2 | Magnetic deviation | NO|
| 3 | E/W |NO |
| 4 | Magnetic Variation |YES  |
| 5 | E/W |YES  |

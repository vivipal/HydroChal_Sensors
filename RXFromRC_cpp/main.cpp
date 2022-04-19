#include <iostream>
#include <fstream>
#include <chrono>
#include "SBUS.h"

using std::cout;
using std::cerr;
using std::endl;
using std::cin;
using std::string;
using std::chrono::steady_clock;
using std::chrono::milliseconds;
using std::ofstream;
using std::ios;

static SBUS sbus;

struct Data_str {
  int flap = 0 ;
  int rudder = 0;
};

Data_str data;

static void onPacket(const sbus_packet_t &packet)
{
    static auto lastPrint = steady_clock::now();
    auto now = steady_clock::now();

    if (now - lastPrint > milliseconds(500))
    {
        data.flap = packet.channels[0];
        data.rudder = packet.channels[1];

        lastPrint = now;
    }
}

int main(int argc, char **argv)
{
    cout << "SBUS blocking receiver example" << endl;

    string ttyPath;
    string savingPath;

    if (argc == 3){
        ttyPath = argv[1];
        savingPath = argv[2];
    }else{
      std::cout << "Please enter device path and path to save the data" << '\n';
      return 0;
    }
    string rudder_filename = savingPath + "/rudder.bin";
    string flap_filename = savingPath + "/flap.bin";

    sbus.onPacket(onPacket);

    sbus_err_t err = sbus.install(ttyPath.c_str(), true);  // true for blocking mode
    if (err != SBUS_OK)
    {
        cerr << "SBUS install error: " << err << endl;
        return err;
    }

    cout << "SBUS installed" << endl;


    // blocks until data is available
    while ((err = sbus.read()) != SBUS_FAIL)
    {

      ofstream rudder_file(rudder_filename, ios::out | ios::binary);
      rudder_file.write(reinterpret_cast<const char *>(&data.rudder), sizeof(data.rudder));
      rudder_file.close();

      ofstream flap_file(flap_filename, ios::out | ios::binary);
      flap_file.write(reinterpret_cast<const char *>(&data.flap), sizeof(data.flap));
      flap_file.close();

      std::cout << " flap : " << data.flap << "\n rudder : " << data.rudder  << "\n\n";

      // desync means a packet was misaligned and not received properly
      if (err == SBUS_ERR_DESYNC)
      {
        cerr << "SBUS desync" << endl;
      }
    }

    cerr << "SBUS error: " << err << endl;

    return err;
}

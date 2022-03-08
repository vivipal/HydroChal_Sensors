#include <iostream>
#include <chrono>
#include "SBUS.h"

using std::cout;
using std::cerr;
using std::endl;
using std::cin;
using std::string;
using std::chrono::steady_clock;
using std::chrono::milliseconds;

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

    if (argc > 1)
        ttyPath = argv[1];
    else
    {
        cout << "Enter tty path: ";
        cin >> ttyPath;
    }

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

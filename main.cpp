#include <cstdlib>
#include <iostream>
#include <libssh/libssh.h>
#include "sshsession.h"
#include "picommand.h"

int main(int argc, const char * argv[]) {
    auto session = SSHSession("lalapi.local");
    try {
      bool connected = session.connect("pi", "raspberry");
      if(connected) {
        auto command = PiCommand();
        //command.showRemoteFiles(session.mSSHSession, "/home/pi/usbdrv/Doraemon\\ Stand\\ by\\ Me\\ Leaked\\ 720p\\ -\\ ENG\\ -\\ INA\\ SUB/");
        command.playOMXPlayer(session.mSSHSession, "/home/pi/usbdrv/Doraemon\\ Stand\\ by\\ Me\\ Leaked\\ 720p\\ -\\ ENG\\ -\\ INA\\ SUB/drmn.stnbme.bd720p-PaHe.in.mp4");
      }
    }

    catch (std::runtime_error &e){
      std::cout << e.what();
      return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

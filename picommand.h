#ifndef PICOMMAND_H
#define PICOMMAND_H

class PiCommand
{
  public:
    int showRemoteFiles(ssh_session session, std::string directory);
    int playOMXPlayer(ssh_session session, std::string filename);
};

#endif /* PICOMMAND_H */

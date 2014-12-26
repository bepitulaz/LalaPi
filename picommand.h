#ifndef PICOMMAND_H
#define PICOMMAND_H

class PiCommand
{
  public:
    int showRemoteFiles(ssh_session session, std::string directory);
};

#endif /* PICOMMAND_H */

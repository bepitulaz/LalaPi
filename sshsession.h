#ifndef SSHSESSION_H
#define SSHSESSION_H

#include <stdexcept>
class SSHConnectionException : public std::runtime_error 
{
  public:
    SSHConnectionException( const std::string &msg ) : std::runtime_error(msg) {};
};

class SSHSession 
{
  private: 
    bool connected = false;
    int verifyKnownHost(ssh_session session);
    
  public:
    ssh_session mSSHSession = ssh_new(); 
    SSHSession(const std::string &hostname);
    inline auto isConnected() const -> decltype(connected) { return connected; }
    bool connect(const char *username, const char *password);
    void disconnect(); 
    int authenticateUser(ssh_session session, const char *username, const char *password); 
    ~SSHSession();
};

#endif /* SSHSESSION_H */

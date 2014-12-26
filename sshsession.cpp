/**
    sshsession.cpp
    Purpose: Managing SSH session

    @author Asep Bagja Priandana
    @version 0.1
*/ 
#include <libssh/libssh.h>
#include <string>
#include <iostream>
#include "sshsession.h"

/**
    Setting up the libssh configuration.

    @param &hostname the reference of target's hostname 
*/
SSHSession::SSHSession(const std::string &hostname) {
  ssh_options_set(mSSHSession, SSH_OPTIONS_HOST, hostname.c_str());
}

/**
    Connecting to the target machine.

    @param none
    @return none
*/
bool SSHSession::connect(const char *username, const char *password) {
  auto rc = ssh_connect(mSSHSession);
  
  if (rc != SSH_OK) {
    auto error_msg = ssh_get_error(mSSHSession);
    throw std::runtime_error(error_msg);
  } else {
    auto isKnown = verifyKnownHost(mSSHSession); 
    
    if (isKnown) {
      connected = true;

      // authenticating the user
      rc = authenticateUser(mSSHSession, username, password);
      
      if (rc != SSH_AUTH_SUCCESS) {
        throw std::runtime_error(ssh_get_error(mSSHSession));
      } else {
        std::cout << "Successfully login" << std::endl;
        return true;
      }
    } else {
      throw std::runtime_error("Server not known.");
    }
  }
}

/**
    Disconnect from target machine.

    @param none
    @return none
*/
void SSHSession::disconnect() {
  ssh_disconnect(mSSHSession);
  connected = false;
}

/**
    Verifying the target host.

    @param session is the created ssh session
    @return the server status 
*/
int SSHSession::verifyKnownHost(ssh_session session) {
  unsigned char *hash = NULL;
  auto state = ssh_is_server_known(session);
  auto hlen = ssh_get_pubkey_hash(session, &hash);
  char buf[10];  
  char *hexa;

  if (hlen < 0)
    return -1;
  
  switch (state) {
    case SSH_SERVER_KNOWN_OK:
      return 1; // ok

    case SSH_SERVER_KNOWN_CHANGED: 
      std::cout << "Host key for server changed: it is now" << std::endl;
      ssh_print_hexa("Public key hash", hash, hlen);
      delete hash;
      return -1;

    case SSH_SERVER_FOUND_OTHER:
      std::cout << "The host key for this server was not found but an other type of key exists." << std::endl; 
      delete hash;
      return -1;

    case SSH_SERVER_FILE_NOT_FOUND:
      std::cout << "Could not find known host file." << std::endl;
      std::cout << "If you accept the host key here, the file will be automatically created." << std::endl; 
      delete hash;
      return -1;

    case SSH_SERVER_NOT_KNOWN:
      hexa = ssh_get_hexa(hash, hlen);
      std::cout << "The server is unknown. Do you trust the host key?" << std::endl;
      std::cout << "Public key hash: " << hexa << std::endl;
      delete hexa;
      if (fgets(buf, sizeof(buf), stdin) == NULL) {
        delete hash;
        return -1;
      }
      
      if (strncasecmp(buf, "yes", 3) != 0) {
        delete hash;
        return -1;
      }
   
      if (ssh_write_knownhost(session) < 0) {
        std::cout << "Error" << std::endl;
        delete hash;
        return -1;
      }
      break;

    case SSH_SERVER_ERROR:
      std::cout << "Error" << ssh_get_error(session) << std::endl;
      delete hash;
      return -1;
  }

  delete hash;
  return 0;  
}

/**
    Authenticating the user by using password

    @param session is current ssh password
    @param username should be null
    @param password is login password
    @return integer of the status
*/
int SSHSession::authenticateUser(ssh_session session, const char *username, const char *password) {
  return ssh_userauth_password(session, username, password);
}

/**
    Auto disconnect and erase the session.

    @param none
    @return none
*/
SSHSession::~SSHSession() {
  if (connected) {
    disconnect();
  }
  ssh_free(mSSHSession);
}

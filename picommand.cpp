/**
    picommand.cpp
    Purpose: List of remote command

    @author Asep Bagja Priandana
    @version 0.1
*/
#include <libssh/libssh.h>
#include <string>
#include "picommand.h"

/**
    Show list files and folders in folders

    @param session is a current authenticated session
    @param directory is a directory to be displayed
    @return status of command
*/
int PiCommand::showRemoteFiles(ssh_session session, std::string directory) {  
  ssh_channel channel;
  int rc;
  channel = ssh_channel_new(session);
  if (channel == NULL) return SSH_ERROR;
  rc = ssh_channel_open_session(channel);
  if (rc != SSH_OK)
  {
    ssh_channel_free(channel);
    return rc;
  }
  
  // prepare the command
  const auto command = std::string("ls -al ");
  const auto execute = command + directory;

  rc = ssh_channel_request_exec(channel, execute.c_str());
  if (rc != SSH_OK)
  {
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return rc;
  }

  char buffer[256];
  unsigned int nbytes;
  nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
  while (nbytes > 0)
  {
    if (fwrite(buffer, 1, nbytes, stdout) != nbytes)
    {
      ssh_channel_close(channel);
      ssh_channel_free(channel);
      return SSH_ERROR;
    }
    nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
  }
  if (nbytes < 0)
  {
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return SSH_ERROR;
  }

  ssh_channel_send_eof(channel);
  ssh_channel_close(channel);
  ssh_channel_free(channel);
  return SSH_OK;
}

/**
    Play mp4 file using OMXPlayer.

    @param session is a current authenticated ssh
    @param filename is a video file to be played
 */
int PiCommand::playOMXPlayer(ssh_session session, std::string filename) {
  ssh_channel channel;
  int rc;
  channel = ssh_channel_new(session);
  if (channel == NULL) return SSH_ERROR;
  rc = ssh_channel_open_session(channel);
  if (rc != SSH_OK)
  {
    ssh_channel_free(channel);
    return rc;
  }
  
  // prepare the command
  const auto command = std::string("omxplayer -o hdmi ");
  const auto execute = command + filename;

  rc = ssh_channel_request_exec(channel, execute.c_str());
  if (rc != SSH_OK)
  {
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return rc;
  }

  ssh_channel_send_eof(channel);
  //ssh_channel_close(channel);
  //ssh_channel_free(channel);  
  
  return SSH_OK;
}

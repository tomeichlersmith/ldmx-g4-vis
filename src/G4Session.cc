/**
 * @file G4Session.cxx
 * @brief Classes which redirect the output of G4cout and G4cerr
 * @author Tom Eichlersmith, University of Minnesota
 */

#include "G4Session.hh"

LoggedSession::LoggedSession(const std::string& coutFileName,
                             const std::string& cerrFileName) {
  coutFile_.open(coutFileName);
  if (not coutFile_.is_open()) {
    throw std::runtime_error("Unable to open log file '" + coutFileName + "'.");
  }

  cerrFile_.open(cerrFileName);
  if (not cerrFile_.is_open()) {
    throw std::runtime_error("Unable to open log file '" + cerrFileName + "'.");
  }
}

LoggedSession::~LoggedSession() {
  coutFile_.close();
  cerrFile_.close();
}

G4int LoggedSession::ReceiveG4cout(const G4String& message) {
  coutFile_ << message;
  coutFile_.flush();
  return 0;  // 0 return value == sucess
}

G4int LoggedSession::ReceiveG4cerr(const G4String& message) {
  cerrFile_ << message;
  cerrFile_.flush();
  return 0;  // 0 return value == sucess
}

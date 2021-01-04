#include <string>
#include <string_view>
#include <iostream>

#include <pcrecpp.h>

#include "utils.hpp"

pcrecpp::StringPiece Utils::ToStringPiece(std::string_view text_sv) {
  pcrecpp::StringPiece text_sp(text_sv.data(), text_sv.size());
  return text_sp;
}

std::string_view Utils::FromStringPiece(pcrecpp::StringPiece text_sp) {
  std::string_view text_sv(text_sp.data(), size_t(text_sp.size()));
  return text_sv;
}

void Utils::GlobalReplaceWithinMatch(std::string &text, pcrecpp::RE re, std::string pat, std::string repl){
  pcrecpp::StringPiece text_sp = Utils::ToStringPiece("WHat?");
  std::string piece;
  std::cerr << "WHAT" << std::endl  ;
  // const auto before = std::chrono::system_clock::now();
  // std::string rule_name = "MultiPeriodAbbreviationRegex" ;
  //reinterpret_cast<pcrecpp::StringPiece*>(&text)
  while (pcrecpp::RE("a").FindAndConsume(&text_sp, &piece))  {
    std::cerr << "Hey" ;
    std::cerr << "CC" << piece ;
     //std::endl; ApplyReplace(piece, "PeriodRule");
   }
  //   const std::chrono::duration<double> duration =
  //       (std::chrono::system_clock::now() - before) * 1000;

  //   if (debug_ == false) {
  //     std::cerr << "After applying " << rule_name << " (took " <<
  //     duration.count()
  //               << "ms) :\n"
  //               << text << "\n"
  //               << std::endl; // DEBUG
  //   }
}

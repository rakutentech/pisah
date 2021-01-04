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

void Utils::FindAndReplaceAll(std::string &text, std::string orig, std::string repl)
{
    size_t pos = text.find(orig);
    while( pos != std::string::npos)
    {
        text.replace(pos, orig.size(), repl);
        pos =text.find(orig, pos + repl.size());
    }
}

void Utils::GlobalReplaceWithinMatch(std::string &text, pcrecpp::RE re, std::string orig, std::string repl){
  pcrecpp::StringPiece text_sp = pcrecpp::StringPiece(text);
  std::string piece;
  std::string out;
  // const auto before = std::chrono::system_clock::now();
  // std::string rule_name = "MultiPeriodAbbreviationRegex" ;
  size_t end = text.length();
  size_t cur = 0;
  
  int ngroups = 16;
  int consumed;
  const pcrecpp::Arg* args[16];
  pcrecpp::Arg argpiece(&piece);
  //args[0] =  reinterpret_cast<pcrecpp::Arg*>(&piece);
  args[0] = &argpiece;
  bool matched = true;
  while (matched){
    matched = re.DoMatch(text_sp, pcrecpp::RE::UNANCHORED, &consumed, args, 1);
    if (!matched){
      break;
    }
    out.append(text, cur, consumed - piece.length());
    FindAndReplaceAll(piece, orig, repl);
    out.append(piece);
    cur += consumed;
    std::cerr << consumed << text_sp << "A" << piece << std::endl;
    text_sp.remove_prefix(consumed);
    //std::cerr << "\nOUT" << out <<std::endl;
  }
  out.append(text, cur, text.length() - cur );
  std::swap(text, out);
}

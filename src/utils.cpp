#include <algorithm>
#include <string>
#include <string_view>
#include <vector>
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


// Funcion tor replace substring within a matched group
// NOTE: make sure the complete pattern is captured.
// except look ahead and look behind.
// i.e., no traililing uncaptured characters in the regex.
void Utils::GlobalReplaceWithinMatch(std::string &text, pcrecpp::RE re, std::vector<std::pair<std::string,std::string>> replacement_list, bool inverse){
  pcrecpp::StringPiece text_sp = pcrecpp::StringPiece(text);
  std::string piece;
  std::string out;
  // const auto before = std::chrono::system_clock::now();
  // std::string rule_name = "MultiPeriodAbbreviationRegex" ;
  size_t end = text.length();
  size_t cur = 0;
  std::string orig, repl;
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
    //std::cerr << "MATCHED:" << consumed << piece.length() <<std::endl;

    for (auto repl_pat : replacement_list){
        if (inverse == false){
          orig = repl_pat.first;
          repl = repl_pat.second;
        }
        else{
          orig = repl_pat.second;
          repl = repl_pat.first;
        }
        FindAndReplaceAll(piece, orig, repl);
    }
    //std::cerr << "CHANGED:" << piece <<std::endl;

    out.append(piece);
    cur += consumed;
    text_sp.remove_prefix(consumed);
    //std::cerr << "\nOUT" << out <<std::endl;
  }
  out.append(text, cur, text.length() - cur );
  std::swap(text, out);
}


std::vector<std::string> Utils::FindAndConsumeAll(std::string &text, pcrecpp::RE re){
  pcrecpp::StringPiece text_sp = pcrecpp::StringPiece(text);

  std::vector<std::string> str_list;

  int ngroups = 16;
  int consumed;

  // create arg objets to store the output match
  std::string piece;
  //const pcrecpp::Arg* args[16];
  //pcrecpp::Arg argpiece(&piece);
  //args[0] = &argpiece;

  bool matched = true;
  while (re.FindAndConsume(&text_sp, &piece)){
    //matched = re.DoMatch(text_sp, pcrecpp::RE::UNANCHORED, &consumed, args, 1);
    //if (!matched){
    ///  break;
    //}

    //text_sp.remove_prefix(consumed);
    str_list.push_back(piece);
  }
  return str_list;

}

// trim from end (in place)
void Utils::RTrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

//find size of unicode character (1,2, or 4 bytes) based on the first character.
// https://stackoverflow.com/questions/40054732/c-iterate-utf-8-string-with-mixed-length-of-characters
int Utils::FindCodePointSize(std::string &s, int i){
  int cplen = 1;
  if((s[i] & 0xf8) == 0xf0) cplen = 4;
  else if((s[i] & 0xf0) == 0xe0) cplen = 3;
  else if((s[i] & 0xe0) == 0xc0) cplen = 2;
  if((i + cplen) > s.length()) cplen = 1;
  return cplen;
}
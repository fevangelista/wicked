#include <iostream>
#include <sstream>
#include <iterator>

#include "helpers.h"

using std::regex;
using std::string;
using std::sregex_token_iterator;

std::string to_string(const std::vector<std::string> &strvec,
                      const std::string &sep) {
  if (strvec.size() == 0)
    return std::string();

  std::ostringstream ss;

  std::copy(strvec.begin(), strvec.end() - 1,
            std::ostream_iterator<std::string>(ss, sep.c_str()));
  ss << strvec.back();

  return ss.str();
}

std::string to_string(const scalar_t r) {
  if (r.numerator() == 0) {
    return "0";
  }
  if (r.denominator() == 1) {
    return std::to_string(r.numerator());
  }
  return std::to_string(r.numerator()) + "/" + std::to_string(r.denominator());
}

std::string to_latex(const scalar_t r) {
  if (r.numerator() == 0) {
    return "0";
  }
  if (r.denominator() == 1) {
    return std::to_string(r.numerator());
  }
  return "\frac{" + std::to_string(r.numerator()) + "}{" +
         std::to_string(r.denominator()) + "}";
}


// trim from start
inline std::string& ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
inline std::string& rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
inline std::string& trim(std::string& s)
{
    return ltrim(rtrim(s));
}

///**
// * @brief readlines
// * @param file
// * @return
// */
//std::vector<std::string> readlines(const std::string& file)
//{
//    std::vector<std::string> result;
//    std::string line;
//    std::ifstream inputfile(file);
//    if (inputfile.is_open()) {
//        int k = 0;
//        while (! inputfile.eof() ) {
//            getline (inputfile,line);
//            result.push_back(line);
////            std::vector<string> split_line;
////            split(split_line,line, is_any_of(" ") );
////            foreach(string str,split_line){
////                if(str[0] == 'a'){
////                    if(str[1] == '+'){
////                        term.add(sqCreation,moCore,k);
////                    }else{
////                        term.add(sqAnnihilation,moCore,k);
////                    }
////                    k++;
////                }
////            }
//        }
//        inputfile.close();
//    }else{
//        std::cout << "Unable to open file: " << file << std::endl;
//        exit(1);
//    }
//    return result;
//}



std::vector<std::string> split(const std::string& s)
{
    // Delimiters are spaces (\s) and/or commas
    regex re("[\\s,]+");
    sregex_token_iterator it(s.begin(), s.end(), re, -1);
    sregex_token_iterator reg_end;

    std::vector<std::string> result;
    for (; it != reg_end; ++it) {
        result.push_back(it->str());
    }
    return result;
}

std::vector<std::string> findall(const string& s,const string& regex)
{
    std::vector<std::string> result;
    std::regex this_regex(regex);
    try {
        std::sregex_iterator next(s.begin(), s.end(), this_regex);
        std::sregex_iterator end;
        std::vector<std::string> matches;
        for (; next != end; ++next) {
            std::smatch match = *next;
            for (size_t k = 1; k < match.size(); ++k){
                result.push_back(match[k]);
            }
        }
    } catch (std::regex_error& e) {
        // Syntax error in the regular expression
    }
    return result;
}


std::vector<std::string> split_indices(const std::string& idx)
{
    std::istringstream f(idx);
    std::string s;
    std::vector<std::string> v;

    while (std::getline(f, s, ',')) {
        std::string trimmed = trim(s);
        v.push_back(trimmed);
    }

    return v;
}

#pragma once

#include "vhliboptimal.hpp"

namespace vhliboptimal {


class VHLibOptimalLogger {

    // ------------------------------------------------
    static void newlout();
    static void partout(const std::string & str);
    static void lineout(const std::string & str);

    // ------------------------------------------------

    static std::string fmt(const std::string & strparam,int value);
    static std::string fmt(const std::string & strparam1,int value,const std::string & strparam2);
    static std::string fmt(int value1,const std::string & strparam,int value2);


};

};

#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <iostream>

#ifndef NDEBUG
    #define ASSERT(condition)                                                                                                   \
        if (!(condition))                                                                                                       \
        {                                                                                                                       \
            std::cerr << "Assertion FAILED! Error in " << #condition <<                                                         \
            ". FILE: " << __FILE__ << ". LINE: " << __LINE__ << ". FUNCTION: " << __PRETTY_FUNCTION__<< "\n" << std::endl;      \
        }
#else
    #define ASSERT(condition) ;
#endif

#endif /* ERRORS_HPP */

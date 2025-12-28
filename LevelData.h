#pragma once
#include <vector>
#include <string>

namespace LevelData {

    const std::vector<std::string> MAP_1 = {
        //   01234567890123456789012345678901234567890123456789012345678901234567890123456789
		"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
		"W                           W           W            W                         W",  // 1
		"W                           W           W            W                         W",  // 2
		"W                           W           W            W                         W",  // 3
		"W                           W           W            W                         W",  // 4
		"W                           W           W            W             S           W",  // 5
		"W                           W           W            W                         W",  // 6
		"W                                       W            W                         W",  // 7
		"W                                       W            W                         W",  // 8
		"W                           W           W            W                         W",  // 9
		"W                           W           W            W                         W",  // 10
		"W                           W     ***   W            W                         W",  // 11
		"W                           W     *?*   W      WWWWWWWWWWWWWWWWWWWWWWWWWW      W",  // 12
		"W                           W     ***   W                                      W",  // 13
		"W                           W           W                                      W",  // 14
		"W                           W           W                                      W",  // 15
		"W                           W                                  4444            W",  // 16
		"W               T           W                                  4444            W",  // 17
		"W                           W                                  4444            W",  // 18
		"W                           W           W                      4444            W",  // 19
        "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",  // 20
    };

    const std::vector<std::string> MAP_2 = {
        //01234567890123456789012345678901234567890123456789012345678901234567890123456789
        "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
        "W  3  3    W                      W     W                                      W", // 1
        "W  3  3    W                      W     W                                      W", // 2
        "W  3  3    W                      W     W                                      W", // 3
        "W  3333    W                      W     W                                      W", // 4
        "W          W                      W  S  W                                      W", // 5
        "W          W                      W     W                                      W", // 6
        "W          W                      W     W                                      W", // 7
        "W          W                      W     W                                      W", // 8
        "W          W                      W     W                                      W", // 9
        "W          W                      W     W                                      W", // 10
        "W          WWWWWWWWWWWWWWWWWWWWWWWW     WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 11
        "W                                                                              W", // 12
        "W                                                                       4444   W", // 13
        "W     ***                                                               4444   W", // 14
        "W     *?*                                                               4444   W", // 15
        "W     ***                                                               4444   W", // 16
        "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW    WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 17
        "W                                 W     W                                      W", // 18
        "W                                 W     W                                      W", // 19
        "W                                 W     W                                      W", // 20
        "W                                 W     W                                      W", // 21
        "W                                 W     W                                      W", // 22
        "W                                 W     W                                      W", // 23
        "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"  // 24
    };

    const std::vector<std::string> MAP_3 = {
    //   01234567890123456789012345678901234567890123456789012345678901234567890123456789
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
    "W      3  3                                                                    W", // 1
    "W      3  3                                                                    W", // 2
    "W      3  3                    WWWW                                            W", // 3
    "W      3333                                                                    W", // 4
    "W                     WWW                                                      W", // 5
    "W                     W W                                                      W", // 6
    "W                     WWW               S                                      W", // 7
    "W                                                                              W", // 8
    "W                                              WWW                             W", // 9
    "W                                              W W                             W", // 10
    "W                                              WWW                             W", // 11
    "W             ***                                                              W", // 12
    "W             *?*                                                              W", // 13
    "W             ***           WWW                                                W", // 14
    "W                           W W                                                W", // 15
    "W                           WWW                                                W", // 16
    "W                                                          4444                W", // 17
    "W                                        H                 4444                W", // 18
    "W                WWWW                                      4444                W", // 19
    "W                                                          4444                W", // 20
    "W                                       WWW                                     W", // 21
    "W                                       WWW                                     W", // 22
    "W                                                                              W", // 23
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"  // 24
};
}
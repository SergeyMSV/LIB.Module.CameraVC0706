///////////////////////////////////////////////////////////////////////////////////////////////////
// ModGnssTest
//
// Created by Sergey Maslennikov
// Tel.:   +7-916-540-09-19
// E-mail: maslennikovserge@yandex.ru
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2016 04 05  |
// |      2     |   2016 11 15  |
// |            |               |
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef LIB_CONFIG_H
#define LIB_CONFIG_H

///////////////////////////////////////////////////////////////////////////////////////////////////
#define LIB_UTILS_LOG
#define LIB_UTILS_LOG_ENDL "\xd\xa"

#define LIB_MODULE_LOG
#define LIB_MODULE_LOG_LIBTEST
///////////////////////////////////////////////////////////////////////////////////////////////////
#define LIB_MODULE_CAMERA_VC0706_CONFIG
//#define LIB_MODULE_CAMERA_VC0706_AUTOBAUDING_SHORT
//#define LIB_MODULE_CAMERA_VC0706_SERIAL_PORT_SET_BAUDRATE
#define LIB_MODULE_CAMERA_VC0706_INITIALIZE_CONFIG
#define LIB_MODULE_CAMERA_VC0706_CHECK_CONNECT_TIME         1//s
#define LIB_MODULE_CAMERA_VC0706_CHECK_CONNECT_FAIL_TIME    LIB_MODULE_CAMERA_VC0706_CHECK_CONNECT_TIME + 3//s
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif//LIB_CONFIG_H

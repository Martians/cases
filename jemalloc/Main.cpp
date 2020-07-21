
//#define STRIP_FLAG_HELP 1
#include <gflags/gflags.h>
#include <cstdlib>
#include <iostream>

static bool ValidatePort(const char* flagname, int32_t value) {
    if (value > 0 && value < 32768)   // value is ok
        return true;
    printf("Invalid value for --%s: %d\n", flagname, (int)value);
    return false;
}

/**
 * 设置验证函数
 */
DEFINE_int32(limit, 25, "speed limit");
DEFINE_validator(limit, &ValidatePort);

DEFINE_string(lang, "english", "using language");
DEFINE_bool(big, true, "big or not");

/**
 * ./client -nobig
 * ./client -limit 99999
 * ./client  --undefok=ss -ss
 * ./client --flagfile=config
 * ./client --flagfile=../config -limit 500
 **/
int
main(int argc, char* argv[]) {
    /**
     * 直接覆盖参数默认值
     */
    FLAGS_lang = "chinese";
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    std::cout << "limit: " << FLAGS_limit << ", lang: " << FLAGS_lang << ", big: " << FLAGS_big;
    return 0;
}
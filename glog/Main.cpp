

/**
 * 根据指定的级别（INFO=1），删除所有glog信息，减小程序体积
 */
// #define GOOGLE_STRIP_LOG 1

// How To Use Google Logging Library (glog)
#include <cstdlib>
#include <glog/logging.h>
#include <gtest/gtest.h>
#include <iostream>

void glog_config(char *argv) {
  /**
   * 此标志要生效，必须放在init之前
   */
  FLAGS_log_dir = "./logs";
  google::InitGoogleLogging(argv);

  //================================================
  /**
   * 方式1：通过gflags进行设置
   */
  // 设置需要输出到 stderr 的最小level
  FLAGS_stderrthreshold = google::INFO;
  FLAGS_minloglevel = google::INFO;
  // FLAGS_log_prefix = false;

  /**
   * 最大 200M
   */
  FLAGS_max_log_size = 200;

  FLAGS_colorlogtostderr = true;
  /**
   * 小于该值的log，将被buffer；大于该值的log，优先级更高，将立即输出
   */
  FLAGS_logbuflevel = google::INFO;
  FLAGS_logbufsecs = 10;

  FLAGS_max_log_size = 200;
  FLAGS_stop_logging_if_full_disk = true;

  //================================================
  /**
   * 方式2：通过函数设置
   */
  google::SetStderrLogging(google::INFO);

  /**
   * 设定某个级别的输出目的地
   */
  google::SetLogDestination(google::INFO, "./logs");

  LOG(INFO) << "arg：----" << argv;

  LOG(INFO) << "11";
  LOG(INFO) << "22";

  LOG(WARNING) << "===";

  /**
   * 设置FLAGS_logbuflevel之后，这里用于等待 INFO
   * 日志的输出，可以发现时延迟输出的
   */
  sleep(30);
}

void glog_condition() {
  /**
   * 条件输出
   */
  if (1) {
    LOG_IF(INFO, 1 > 0) << "1 > 0";
    for (int i = 0; i < 10; i++) {
      // 每多少次输出一次, 必须在循环中？
      //      google::COUNTER 是从1开始计数
      LOG_FIRST_N(INFO, 5) << "Got the " << google::COUNTER << "th cookie";
      LOG_EVERY_N(INFO, 5) << "Every the " << google::COUNTER << "th cookie";
      LOG_IF_EVERY_N(INFO, 2 > 3, 2) << "log if every";
    }
  }

  /**
   * 类似于assert，但是不受 NDEBUG 影响
   *      c字符串版本：CHECK_STREQ、CHECK_STRCASEEQ等
   */
  CHECK(1 == 1) << "failed";
  CHECK_NE(1, 2) << ": The world must be ending!";
  CHECK_NE(1, 2);

  /**
   * 这两个不能用流，即：CHECK_NOTNULL(b) << " logs"
   */
  int *b = NULL;
  int c = 1;
  // CHECK_EQ(b, NULL);
  CHECK_EQ(b, static_cast<int *>(NULL));

  b = &c;
  CHECK_NOTNULL(b);
}

/**
 * 1. 这里通过在命令行，指定v的值，来指定显示什么内容
 *      VLOG实际上是 INFO 级别的，修改了日志级别会影响到VLOG
 *
 * 2. 更多设置：指定默认级别，以及在哪些文件中设置何种级别
 *      ./client -v 0 --vmodule=Main*=1
 *      ./client -v 0 --vmodule=Main*=0
 */
void glog_types() {
  /**
   * 值越小，级别越高；与 默认的glog是相反的
   */
  // FLAGS_v = 1;
  VLOG(-1) << "level -1";
  VLOG(1) << "level 1";
  VLOG(2) << "level 2";

  if (VLOG_IS_ON(1)) {
    VLOG(1) << "vlog <= 1";
  }
  /**
   * DFATAL，在debug模式下是 FATAL的意思
   *      生产环境下，定义了 NDEBUG 之后，降级为 ERROR
   */
  // LOG(DFATAL) << "dfatal";

  /**
   * debuhg 模式的支持，加了此macro时，将不会输出这些log了
   *      add_definitions(-DNDEBUG)
   */
  DLOG(INFO) << "debug mode：Found cookies";

  /**
   * 输出 errno信息：PLOG_IF、PLOG
   *    SYSLOG, SYSLOG_IF, SYSLOG_EVERY_N
   */
  errno = 2;
  PCHECK(false) << "Write NULL failed";
}

// log to file and stderr at the same time when seg fault
void segFaultHandle(const char *data, int size) {
  // std::ofstream fs("glog_dump.log", std::ios::app);
  // std::string str = std::string(data, size);
  // fs << str;
  // fs.close();
  LOG(ERROR) << std::string(data, size);
}

void glog_single_handle() {
  /**
   * 相当于注册了信号处理函数
   */
  google::InstallFailureSignalHandler();

  /**
   * 指定将错误重定向输出自己的函数中处理，不加是默认输出到 stderr
   */
  google::InstallFailureWriter(&segFaultHandle);

  /**
   * 触发信号
   */
  if (0) {
    int s = 0;
    std::cout << 8 / s << std::endl;
  }

  /**
   * 可以自定义 FATAL、check 失败时 的处理行为，而不是直接退出
   */
  // google::InstallFailureFunction(&YourFailureFunction);
}

/**
 * glog: file:///Users/darwin/base/code/github/3rd/glog/doc/glog.html
 *      http://rpg.ifi.uzh.ch/docs/glog.html，比较好的全局介绍，包括选项
 *      中文：http://www.yeolar.com/note/2014/12/20/glog/
 *
 *      https://www.jianshu.com/p/a576ed1c2cd3
 *
 *
 * 使用说明：
 *      1. 如果使用了gflags，编译时必须带上 gflags，
 *      2. 默认位置：/tmp/<program name>.<hostname>.<user name>.log.<severity
 * level>.<date>.<time>.<pid>
 *
 * 命令行：
 *      GLOG_logtostderr=1 ./client     # gflag 未安装情况下，可以使用此方式
 *      ./client --logtostderr=1
 *
 * 功能说明：
 *      1. 不支持滚动打印
 *      2. INFO, WARNING, ERROR, and FATAL，没有debug、trace（使用VLOG））
 *      3. <glog/raw_logging.h>, 不分配任何内存和加锁，是线程安全的
 */
int main(int argc, char *argv[]) {
  /**
   * 必须先用gflags解析一次才行
   */
  // google::ParseCommandLineFlags(&argc, &argv, true);
  // glog_config(argv[0]);
  // glog_condition();
  //
  // glog_types();
  // glog_single_handle();
  //
  // google::ShutdownGoogleLogging();

  FLAGS_log_dir = "./logs";
  google::InitGoogleLogging("ranger");
  google::SetStderrLogging(google::FATAL);
  // google::SetLogDestination(google::INFO, (FLAGS_log_dir + "/pbase_info_").c_str());
  // google::SetLogDestination(google::ERROR, (FLAGS_log_dir + "/pbase_error_").c_str());
  // google::SetLogDestination(google::WARNING, (FLAGS_log_dir + "/pbase_warning_").c_str());
  FLAGS_colorlogtostderr = true;
  FLAGS_logbufsecs = 30;
  FLAGS_max_log_size = 200;
  FLAGS_stop_logging_if_full_disk = true;
  google::InstallFailureSignalHandler();
  LOG(INFO) << "1111";

  google::ShutdownGoogleLogging();
  return 0;
}
#include "lib/hello-time.h"
#include "main/hello-greet.h"

//直接使用 project2下的相对目录
#include "main/print.h"

#include <iostream>
#include <string>

/**
 * example:
 *      https://docs.bazel.build/versions/1.2.0/tutorial/cpp.html#set-up-the-workspace
 *      bazel build //main:hello-world
 *
 */
int main(int argc, char** argv) {
  std::string who = "world";
  if (argc > 1) {
    who = argv[1];
  }
  std::cout << get_greet(who) << std::endl;
  print_localtime();

  std::cout << hello_print("call in project1: ")<< std::endl;
  return 0;
}

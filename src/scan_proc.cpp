#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "udp_client.h"

struct CoreInfo {
  std::string cpu_name;
  unsigned long user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;
};

std::vector<CoreInfo> readProcStat();
double calculateCoreUsage(const CoreInfo& prev, const CoreInfo& curr);

int main() {
  std::vector<CoreInfo> prev_stats, curr_stats;
  std::vector<double> cpuLoadVector;
  prev_stats = readProcStat();

  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    curr_stats = readProcStat();

    double total_usage = calculateCoreUsage(prev_stats[0], curr_stats[0]);
    std::cout << "All CPU usage: " << total_usage << "%" << std::endl;
    cpuLoadVector.push_back(total_usage);

    /*
    //for real /proc/stat file
    for (size_t i = 1; i < curr_stats.size(); ++i) {
      if (i < prev_stats.size()) {
        double core_usage = calculateCoreUsage(prev_stats[i], curr_stats[i]);
        std::cout << "CPU -" << i-1 << "- " << core_usage << "%" << std::endl;
        cpuLoadVector.push_back(core_usage);
      }
    }
    */

    //begin of emulation
    //emulation /proc/stat changing
    std::srand(std::time(0));
    for (size_t i = 1; i < curr_stats.size(); ++i) {
      if (i < prev_stats.size()) {
        //mixing strings
        int j = std::rand() % 11;
        double core_usage = calculateCoreUsage(prev_stats[i], curr_stats[j]);
        //normalization 0..100%
        if (core_usage > 100) {
          core_usage = std::rand() % 100;
        } else if (core_usage < 1 and core_usage > 0) {
          core_usage = std::rand() % 3;
        }
        std::cout << "CPU -" << i - 1 << "- " << core_usage << "%" << std::endl;
        cpuLoadVector.push_back(core_usage);
      }
    }
    std::cout << std::endl;
    //end of emulation

    prev_stats = curr_stats;
    sendVectorAsUDPClient(cpuLoadVector);
    cpuLoadVector.clear();
  };

  return 0;
}

std::vector<CoreInfo> readProcStat() {
  std::vector<CoreInfo> info;
  std::ifstream file(
      "./proc/stat");  //I use MacOS. There is no such file /proc/stat
  std::string line;

  while (std::getline(file, line)) {
    if (line.substr(0, 3) != "cpu") {
      continue;  //ignore strings starts without 'cpu'
    };

    std::istringstream iss(line);
    CoreInfo ci;
    iss >> ci.cpu_name;
    iss >> ci.user >> ci.nice >> ci.system >> ci.idle >> ci.iowait >> ci.irq >>
        ci.softirq >> ci.steal >> ci.guest >> ci.guest_nice;
    info.push_back(ci);
    return info;
  }

  file.close();
  line.clear();

  return info;
}

double calculateCoreUsage(const CoreInfo& prev, const CoreInfo& curr) {
  unsigned long prev_total = prev.user + prev.nice + prev.system + prev.idle +
                             prev.iowait + prev.irq + prev.softirq + prev.steal;
  unsigned long curr_total = curr.user + curr.nice + curr.system + curr.idle +
                             curr.iowait + curr.irq + curr.softirq + curr.steal;

  unsigned long prev_idle = prev.idle + prev.iowait;
  unsigned long curr_idle = curr.idle + curr.iowait;

  unsigned long total_diff = curr_total - prev_total;
  unsigned long idle_diff = curr_idle - prev_idle;

  double load{0.0};

  if (total_diff != 0) {
    load = 100.0 * (total_diff - idle_diff) / total_diff;
    load = round(load);
  }

  return load;
}
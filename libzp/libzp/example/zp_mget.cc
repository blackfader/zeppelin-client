/*
 * "Copyright [2016] qihoo"
 */
#include <unistd.h>

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "libzp/include/zp_client.h"


void usage() {
  std::cout << "usage:\n"
            << "      zp_cli host port cnt\n";
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    usage();
    return -1;
  }

  // client handle io operation
  std::cout << "create client" << std::endl;
  libzp::Client* client = new libzp::Client(argv[1], atoi(argv[2]), "parade");
  std::cout << "connect cluster" << std::endl;
  int cnt = atoi(argv[3]);
  // needs connect to cluster first
  libzp::Status s;
  /* operation
  s = cluster->CreateTable("test", 24);
  */
  if (!s.ok()) {
    std::cout << s.ToString() << std::endl;
    return -1;
  }
  std::vector<std::string> keys;
  for(int i = 0; i < cnt; i++) {
    std::string key = "key" + std::to_string(i);
    keys.push_back(key);
    std::string value(1000, 'a');
    s = client->Set(key, value);
    if (!s.ok()) {
      std::cout << s.ToString() << std::endl;
    } else {
      //std::cout << " - set ok, " << key << " -> " << value << std::endl;
      std::cout << " - set ok, " << std::endl;
    }
  }

  std::map<std::string, std::string> kvs;
  std::cout << "Mget all keys exist begin ------------------------------------" << std::endl;
  s = client->Mget(keys, &kvs);
  std::cout << "Result :" << s.ToString() << std::endl;
  for (auto& kv : kvs) {
    std::cout << " - " << kv.first << " -> " << kv.second.substr(0, 100) << std::endl;
  }
  std::cout << "Mget all keys exist end ------------------------------------" << std::endl;
  sleep(60);
  
  std::cout << "Mget all keys exist begin ------------------------------------" << std::endl;
  s = client->Mget(keys, &kvs);
  std::cout << "Result :" << s.ToString() << std::endl;
  for (auto& kv : kvs) {
    std::cout << " - " << kv.first << " -> " << kv.second.substr(0, 100) << std::endl;
  }
  std::cout << "Mget all keys exist end ------------------------------------" << std::endl;
  sleep(60);

  std::cout << "Mget some keys not exist begin ------------------------------------" << std::endl;
  kvs.clear();
  keys.push_back("not_exist");
  keys.push_back("not_exist1");
  s = client->Mget(keys, &kvs);
  std::cout << "Result :" << s.ToString() << std::endl;
  for (auto& kv : kvs) {
    if (kv.second.empty()) {
      std::cout << " - " << kv.first << " -> [emtpy] " << std::endl;
    } else {
      std::cout << " - " << kv.first << " -> " << kv.second.substr(0, 100) << std::endl;
    }
  }
  std::cout << "Mget some keys not exist end ------------------------------------" << std::endl;

  // Mget
  delete client;
}

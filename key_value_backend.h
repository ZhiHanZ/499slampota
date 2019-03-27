#ifndef CPP_KEY_VALUE_BACKEND_H_
#define CPP_KEY_VALUE_BACKEND_H_

#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <vector>

// lowest level key_value_backend implementation that stores all raw data
class KeyValueBackEnd {
 public:
  // constructor
  KeyValueBackEnd();
  // destructor
  virtual ~KeyValueBackEnd();
  // accessor: returns vector value for the 'key' argument
  // or null if the 'key' does not yet exist
  std::vector<std::string> Get(const std::string& key);
  // inserts pairs of 'key' and 'value' into the data_map_
  // will push back to the vector if the 'key' already exists
  void Put(const std::string& key, const std::string& chirp_value);
  // removes 'key' and associated 'chirp_value' from the data_map_
  void DeleteKey(const std::string& key);
  // returns size of the 'data_map_' for testing purposes
  int size();

 private:
  // a data_map_ that stores all information for the chirp project
  // contains keys of type string and values of vectors of strings
  std::map<std::string, std::vector<std::string> > data_map_;
  // a mutex to keep the database thread safe
  std::mutex mutex_lock;
};

#endif /*CPP_KEY_VALUE_BACKEND_H_*/
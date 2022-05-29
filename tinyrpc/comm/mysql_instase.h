#ifndef TINYRPC_COMM_MYSQL_INSTASE_H
#define TINYRPC_COMM_MYSQL_INSTASE_H

#include <mysql/mysql.h>
#include <memory>
#include <map>
#include "tinyrpc/net/mutex.h"
#include "tinyrpc/net/net_address.h"

namespace tinyrpc {

class MySQLThreadInit {
 public:

  MySQLThreadInit();
  
  ~MySQLThreadInit();

};

struct MySQLOption {
 public:
  explicit MySQLOption(const IPAddress& addr) : m_addr(addr) {};
  ~MySQLOption() {};

 public:
  IPAddress m_addr;
  std::string m_user;
  std::string m_passwd;
  std::string m_select_db;
  std::string m_char_set;
};

class MySQLInstase {
 public:

  typedef std::shared_ptr<MySQLInstase> ptr;

  MySQLInstase(const MySQLOption& option);

  ~MySQLInstase();

  bool isInitSuccess();

  int query(const std::string& sql);

  int commit();

  int begin();

  int rollBack();

  MYSQL_RES* storeResult();

  MYSQL_ROW fetchRow(MYSQL_RES* res);

  void freeResult(MYSQL_RES* res);

  long long numFields(MYSQL_RES* res);

  long long affectedRows();

  std::string getMySQLErrorInfo();

  int getMySQLErrno();

private:
  int reconnect();

 private:
  MySQLOption m_option;
  bool m_init_succ {false};
  bool m_in_trans {false};
  Mutex m_mutex;
  MYSQL* m_sql_handler {NULL};

};


class MySQLInstaseFactroy {
 public:
  MySQLInstaseFactroy() = default;

  ~MySQLInstaseFactroy() = default;

  MySQLInstase* GetMySQLInstase(const std::string& key);
 public:
  static MySQLInstaseFactroy* GetThreadMySQLFactory();
  

 private:
  std::map<std::string, MySQLInstase::ptr> m_conn_pools;

};



}



#endif
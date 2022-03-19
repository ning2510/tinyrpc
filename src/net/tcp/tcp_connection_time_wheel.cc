#include <queue>
#include <vector>
#include "abstract_slot.h"
#include "tcp_connection.h"
#include "tcp_connection_time_wheel.h"
#include "../timer.h"

namespace tinyrpc {

TcpTimeWheel::TcpTimeWheel(Reactor* reactor, int bucket_count, int inteval /*= 10*/) 
  : m_reactor(reactor)
  , m_bucket_count(bucket_count)
  , m_inteval(inteval) {

  for (int i = 0; i < bucket_count; ++i) {
    std::vector<TcpConnectionSlot::ptr> tmp;
    m_wheel.push(tmp);
  }

  m_event = std::make_shared<TimerEvent>(m_inteval * 1000, true, std::bind(&TcpTimeWheel::loopFunc, this));
  m_reactor->getTimer()->addTimerEvent(m_event);
}


TcpTimeWheel::~TcpTimeWheel() {
  m_reactor->getTimer()->delTimerEvent(m_event);
}

void TcpTimeWheel::loopFunc() {
  m_wheel.pop();
  std::vector<TcpConnectionSlot::ptr> tmp;
  m_wheel.push(tmp);
}

void TcpTimeWheel::fresh(TcpConnectionSlot::ptr slot) {
  m_wheel.back().emplace_back(slot);
}


}
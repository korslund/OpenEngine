#ifndef OENGINE_INPUT_DISPATCHER_H
#define OENGINE_INPUT_DISPATCHER_H

#include "dispatch_map.hpp"
#include <mangle/input/event.hpp>
#include <assert.h>

namespace OEngine {
namespace Input {

struct KeyBuffer : Mangle::Input::Event
{
  DispatchMap map;

  Type filter;

  std::vector<bool> states;

  /**
     Constructor. Takes the number of actions and passes it to
     FuncBinder.
  */
  KeyBuffer(int actions, Type filt=EV_ALL) : states(actions), filter(filt) {}

  void bind(unsigned int action, int key)
  {
    assert(action >= 0 && action < states.size());
    map.bind(key, action);
  }
  void unbind(unsigned int action, int key)
  {
    assert(action >= 0 && action < states.size());
    map.unbind(key, action);
  }
  bool isBound(int key) const { return map.isBound(key); }

  bool wasPressed(int action)
  {
    assert(action >= 0 && action < states.size());
    bool res = states[action];
    states[action] = false;
    return res;
  }

  /**
     Instigate an event. It is translated through the dispatch map and
     updates the given state bool.
   */
  typedef DispatchMap::OutList _O;
  void event(Type type, int index, const void* p)
  {
    if((type & filter) == 0) return;

    // No bindings, nothing happens
    if(!isBound(index))
      return;

    // Get the mapped actions and execute them
    const _O &list = map.getList(index);
    _O::const_iterator it;
    for(it = list.begin(); it != list.end(); it++)
      {
        int action = *it;
        assert(action >= 0 && action < states.size());
        states[action] = true;
      }
  }
};

typedef boost::shared_ptr<KeyBuffer> KeyBufferPtr;

}}
#endif

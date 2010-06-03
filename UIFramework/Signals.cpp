#include "Signals.h"

//
// This is a hack to prevent having to build and link the boost signals library.
//

#include "External/boost/libs/signals/src/connection.cpp"
#include "External/boost/libs/signals/src/named_slot_map.cpp"
#include "External/boost/libs/signals/src/signal_base.cpp"
#include "External/boost/libs/signals/src/slot.cpp"
#include "External/boost/libs/signals/src/trackable.cpp"
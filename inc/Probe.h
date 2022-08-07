#pragma once

#ifndef SCA_PROBE_H
#define SCA_PROBE_H

#include "Kernel.h"

#define SNOOP_B_TRANSPORT(socket, trans, delay)          \
  {                                                      \
    Scasrt::Kernel::GetInstance()->SnoopBTransportBgn(   \
        trans, socket.name(), sc_core::sc_time_stamp()); \
    socket->b_transport(trans, delay);                   \
    Scasrt::Kernel::GetInstance()->SnoopBTransportEnd(   \
        trans, socket.name(), sc_core::sc_time_stamp()); \
  }

#define SNOOP_NB_TRANSPORT_FW(status, socket, trans, phase, delay)      \
  {                                                                     \
    Scasrt::Kernel::GetInstance()->SnoopNbTransportFwBgn(               \
        trans, socket.name(), sc_core::sc_time_stamp(), phase);         \
    status = socket->nb_transport_fw(trans, phase, delay);              \
    Scasrt::Kernel::GetInstance()->SnoopNbTransportFwEnd(               \
        trans, socket.name(), sc_core::sc_time_stamp(), phase, status); \
  }

#define SNOOP_NB_TRANSPORT_FW_I(socket, trans, phase, delay)                  \
  {                                                                           \
    Scasrt::Kernel::GetInstance()->SnoopNbTransportFwBgn(                     \
        trans, socket.name(), sc_core::sc_time_stamp(), phase);               \
    tlm::tlm_sync_enum status = socket->nb_transport_fw(trans, phase, delay); \
    Scasrt::Kernel::GetInstance()->SnoopNbTransportFwEnd(                     \
        trans, socket.name(), sc_core::sc_time_stamp(), phase, status);       \
  }

#define SNOOP_NB_TRANSPORT_BW(status, socket, trans, phase, delay)      \
  {                                                                     \
    Scasrt::Kernel::GetInstance()->SnoopNbTransportBwBgn(               \
        trans, socket.name(), sc_core::sc_time_stamp(), phase);         \
    status = socket->nb_transport_bw(trans, phase, delay);              \
    Scasrt::Kernel::GetInstance()->SnoopNbTransportBwEnd(               \
        trans, socket.name(), sc_core::sc_time_stamp(), phase, status); \
  }

#endif

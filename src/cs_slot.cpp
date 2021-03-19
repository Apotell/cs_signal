/***********************************************************************
*
* Copyright (c) 2016-2021 Barbara Geller
* Copyright (c) 2016-2021 Ansel Sermersheim
*
* This file is part of CsSignal.
*
* CsSignal is free software, released under the BSD 2-Clause license.
* For license details refer to LICENSE provided with this project.
*
* CopperSpice is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* https://opensource.org/licenses/BSD-2-Clause
*
***********************************************************************/

#include "cs_signal.h"
#include "cs_slot.h"

CS_SIGNAL_NS::SlotBase::SlotBase()
{
}

CS_SIGNAL_NS::SlotBase::SlotBase(const SlotBase &)
{
}

CS_SIGNAL_NS::SlotBase::~SlotBase()
{
   try {
      // clean up possible sender connections
      auto receiverListHandle = m_possibleSenders.lock_read();

      for (auto &sender : *receiverListHandle) {
         auto senderListHandle = sender->m_connectList.lock_write();

         auto iter = senderListHandle->begin();

         while (iter != senderListHandle->end())   {

            if (iter->receiver == this) {
               iter = senderListHandle->erase(iter);
            } else {
               ++iter;
            }
         }
      }

   } catch (...) {
     // least of the worst options
     std::terminate();
   }
}

CS_SIGNAL_NS::SignalBase *&CS_SIGNAL_NS::SlotBase::get_threadLocal_currentSender()
{
#ifdef __APPLE__
   static __thread CS_SIGNAL_NS::SignalBase *threadLocal_currentSender = nullptr;
#else
   static thread_local CS_SIGNAL_NS::SignalBase *threadLocal_currentSender = nullptr;
#endif

   return threadLocal_currentSender;
}

bool CS_SIGNAL_NS::SlotBase::compareThreads() const
{
   return true;
}

void CS_SIGNAL_NS::SlotBase::queueSlot(PendingSlot data, ConnectionKind)
{
   // calls the slot immediately
   data();
}

CS_SIGNAL_NS::SignalBase *CS_SIGNAL_NS::SlotBase::sender() const
{
   return get_threadLocal_currentSender();
}

std::set<CS_SIGNAL_NS::SignalBase *> CS_SIGNAL_NS::SlotBase::internal_senderList() const
{
   std::set<SignalBase *> retval;

   auto receiverListHandle = m_possibleSenders.lock_read();

   for (auto &sender : *receiverListHandle) {
      retval.insert(const_cast<SignalBase *>(sender));
   }

   return retval;
}

CS_SIGNAL_NS::PendingSlot::PendingSlot(SignalBase *sender, std::unique_ptr<Internal::BentoAbstract> signal_Bento,
                  SlotBase *receiver, std::unique_ptr<Internal::BentoAbstract> slot_Bento,
                  std::unique_ptr<Internal::TeaCupAbstract> teaCup_Data)
   : m_sender(sender), m_signal_Bento(std::move(signal_Bento)), m_receiver(receiver),
     m_slot_Bento(std::move(slot_Bento)), m_teaCup_Data(std::move(teaCup_Data))
{
}

void CS_SIGNAL_NS::PendingSlot::operator()() const
{
   // invoke the slot
   m_slot_Bento->invoke(m_receiver, m_teaCup_Data.get());
}


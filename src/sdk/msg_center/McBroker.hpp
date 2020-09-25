/**************************************************************************
 * Copyright (C) 2017-2017  Unisound
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 **************************************************************************
 *
 * Description : uni_mc_service.h
 * Author      : chenxiaosong@unisound.com
 * Date        : 2020.04.01
 *
 **************************************************************************/
#ifndef SDK_MSG_CENTER_MC_BROKER_H_
#define SDK_MSG_CENTER_MC_BROKER_H_

#include "IMcMsgHandler.hpp"
#include "uni_iot.h"


Result McBrokerConnect(void);
void   McBrokerDisconnect(void);
Result McBrokerSend(const string &data);
Result McBrokerInit(void);
void   McBrokerFinal(void);
Result McBrokerAddHandler(IMcMsgHandler* handler);
Result McBrokerRemoveHandler(string *handler_name);

#endif  //  SDK_MSG_CENTER_MC_BROKER_H_

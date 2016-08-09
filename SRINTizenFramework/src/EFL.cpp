/*
 * EFL.cpp
 *
 *  Created on: Aug 9, 2016
 *      Author: Gilang
 */

#include "SRIN/EFL.h"

void SRIN::EFL::EvasSmartEventHandler(void* data, Evas_Object* obj,
		void* eventData)
{
	auto package = static_cast<EvasSmartEvent*>(data);

#if _DEBUG
	if(package->eventLabel)
		dlog_print(DLOG_DEBUG, LOG_TAG, "Event raised: %s", package->eventLabel);
#endif

	(*package)(obj, eventData);
}

void SRIN::EFL::EvasObjectEventHandler(void* data, Evas* evas, Evas_Object* obj,
		void* eventData)
{
	auto package = static_cast<EvasObjectEvent*>(data);

#if _DEBUG
	if(package->eventLabel)
		dlog_print(DLOG_DEBUG, LOG_TAG, "Event raised: %s", package->eventLabel);
#endif
	EvasEventSourceInfo source = { evas, obj };

	(*package)(&source, eventData);
}

void SRIN::EFL::EdjeSignalEventHandler(void* data, Evas_Object* obj,
		const char* emission, const char* source)
{
	auto package = static_cast<EdjeSignalEvent*>(data);

#if _DEBUG
	if(package->eventLabel)
		dlog_print(DLOG_DEBUG, LOG_TAG, "Event raised: %s", package->eventLabel);
#endif

	(*package)(obj, { emission, source });
}

void SRIN::EFL::EdjeObjectItemSignalEventHandler(void* data,
		Elm_Object_Item* it, const char* emission, const char* source)
{
	auto package = static_cast<ObjectItemEdjeSignalEvent*>(data);

#if _DEBUG
	if(package->eventLabel)
		dlog_print(DLOG_DEBUG, LOG_TAG, "Event raised: %s", package->eventLabel);
#endif

	(*package)(it, { emission, source });
}

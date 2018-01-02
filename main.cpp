#include <stdio.h>

#include <list>
#include <string>
#include <iostream>

#include "util.h"
#include "assert.h"
#include "DeckLinkAPI.h"
#include "DeviceProber.h"

std::list<IDeckLink*> collectDeckLinkDevices(void);
void freeDeckLinkDevices(std::list<IDeckLink*> deckLinkDevices);

std::list<DeviceProber*> createDeviceProbers(std::list<IDeckLink*> deckLinkDevices);
void freeDeviceProbers(std::list<DeviceProber*> deviceProbers);

void printStatusList(std::list<DeviceProber*> deviceProbers);
char* getDeviceName(IDeckLink* deckLink);

int main (UNUSED int argc, UNUSED char** argv)
{
	std::list<IDeckLink*> deckLinkDevices = collectDeckLinkDevices();
	std::list<DeviceProber*> deviceProbers = createDeviceProbers(deckLinkDevices);

	printStatusList(deviceProbers);

	freeDeviceProbers(deviceProbers);
	freeDeckLinkDevices(deckLinkDevices);

	return 0;
}

std::list<IDeckLink*> collectDeckLinkDevices(void)
{
	std::list<IDeckLink*>	deckLinkDevices;
	IDeckLinkIterator*				deckLinkIterator;

	deckLinkIterator = CreateDeckLinkIteratorInstance();
	if (deckLinkIterator == NULL)
	{
		std::cerr
			<< "A DeckLink iterator could not be created."
			<< "The DeckLink drivers may not be installed."
			<< std::endl;

		exit(1);
	}

	IDeckLink* deckLink = NULL;
	while (deckLinkIterator->Next(&deckLink) == S_OK)
	{
		deckLinkDevices.push_front(deckLink);
	}

	assert(deckLinkIterator->Release() == 0);

	return deckLinkDevices;
}

void freeDeckLinkDevices(std::list<IDeckLink*> deckLinkDevices)
{
	for(IDeckLink* deckLink: deckLinkDevices)
	{
		assert(deckLink->Release() == 0);
	}
}

std::list<DeviceProber*> createDeviceProbers(std::list<IDeckLink*> deckLinkDevices)
{
	std::list<DeviceProber*> deviceProbersList;

	for(IDeckLink* deckLink: deckLinkDevices)
	{
		deviceProbersList.push_front(new DeviceProber(deckLink));
	}

	return deviceProbersList;
}

void freeDeviceProbers(std::list<DeviceProber*> deviceProbers)
{
	for(DeviceProber* deviceProber : deviceProbers)
	{
		assert(deviceProber->Release() == 0);
	}
}

void printStatusList(std::list<DeviceProber*> deviceProbers)
{
	int deviceIndex = 0;

	for(DeviceProber* deviceProber : deviceProbers)
	{
		std::cout
			<< deviceIndex << ", " << deviceProber->GetDeviceName()
			<< ", CanAutodetect: " << deviceProber->CanAutodetect()
			<< std::endl;

		deviceIndex++;
	}

	if (deviceIndex == 0) {
				std::cerr << "No DeckLink devices found" << std::endl;
	}
}

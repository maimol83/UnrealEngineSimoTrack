// Fill out your copyright notice in the Description page of Project Settings.
#include "MyBlueprintFunctionLibrary.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include <iomanip>
#include "Engine/GameEngine.h"
#include <string>
#include <coroutine>
#include <fstream>
#include "Containers/UnrealString.h"

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Storage::Streams;
using namespace winrt::Windows::Devices::Bluetooth;
using namespace winrt::Windows::Foundation::Collections;
using namespace winrt::Windows::Devices::Bluetooth::Advertisement;
using namespace winrt::Windows::Devices::Bluetooth::GenericAttributeProfile;


std::wstring guidToString(GUID uuid)
{
	std::wstring guid;
	WCHAR* wszUuid = NULL;
	if (::UuidToString(&uuid, (RPC_WSTR*)&wszUuid) == RPC_S_OK)
	{
		guid = wszUuid;
		::RpcStringFree((RPC_WSTR*)&wszUuid);
	}

	return guid;
}
int l { 0 };
int d;
int hrbreakerglobal;
int y;
int o;
int u;
int h;
float lastcranktime3;
int addedcrankrev;
int fifthbit;
int hrm { 0 };
int hrcheckdisconnect;
int hrmvalue;

int UMyBlueprintFunctionLibrary::LoopBreaker(int x)
{
	d = x;
	return d;
}

int UMyBlueprintFunctionLibrary::HRMBreaker(int hrbreaker)
{
	hrbreakerglobal = hrbreaker;
	return hrbreakerglobal;
}

int UMyBlueprintFunctionLibrary::LoopCheck()
{
	return h;
}

IAsyncAction OpenDevice(unsigned long long deviceAddress)
{	
	if (l == 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("podkluchaus")));
		auto device = co_await BluetoothLEDevice::FromBluetoothAddressAsync(deviceAddress);
		auto services = co_await device.GetGattServicesAsync();
		for (GattDeviceService const& s : services.Services())
		{

			//std::wstring wideStringcad{ L"00002a5b-0000-1000-8000-00805f9b34fb" };  //cadence  ubrat)
			//std::wstring cadservice{ L"00001818-0000-1000-8000-00805f9b34fb" };  //cadence  ubrat)

			auto characteristics = co_await s.GetCharacteristicsAsync();
			std::wstring wideString{ L"00002a63-0000-1000-8000-00805f9b34fb" };
			
			for (GattCharacteristic const& c : characteristics.Characteristics())
			{
				if (c.CharacteristicProperties() == GattCharacteristicProperties::Notify && guidToString(c.Uuid()) == wideString)
				{
					auto status = co_await c.WriteClientCharacteristicConfigurationDescriptorAsync(GattClientCharacteristicConfigurationDescriptorValue::Notify);
						if (status == GattCommunicationStatus::Success)
						{
							//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("uspeshno_podkluchen")));
							double i = 0;
							
							for (int t{ 0 }; t < 10000000; ++t)
								{   
							std::this_thread::sleep_for(std::chrono::milliseconds(300));
							if (d == 5)
							{
								t = 100000000;
								l = 5;
								break;
								device.Close();

							}
							c.ValueChanged([&]
							(GattCharacteristic const& characteristic, GattValueChangedEventArgs eventArgs)
								{
							h = h + 1;
							if (h > 100000000)
							{
								h = 0;
							}
							DataReader reader = DataReader::FromBuffer(eventArgs.CharacteristicValue());
							byte firstbyte = reader.ReadByte(); //1
							reader.ReadByte(); //2
							int a = reader.ReadByte(); //3 power
							int b = reader.ReadByte();  //4 power
							y = (a + b * 255);
							int fbyte = firstbyte;
							switch (fbyte)

							{
							case 32:   //100000 - (2 - 2 - 0 - 0 - 0 - 4) crankrev 5,6,7,8
							case 34:   //100010
							case 40:   //101000
							case 42:   //101010
							{
								int CumulativeCranRev1 = reader.ReadByte(); //5 CumulativeCranRevolutions
								int CumulativeCranRev2 = reader.ReadByte(); //6 CumulativeCranRevolutions
								byte lastcranktime1 = reader.ReadByte(); //7 Last Crank Event Time
								byte  lastcranktime2 = reader.ReadByte(); //8 Last Crank Event Time
								lastcranktime3 = lastcranktime2 << 8 | lastcranktime1;
								addedcrankrev = CumulativeCranRev1 + CumulativeCranRev2;
							}
							break;

							case 33:   //100001 - (2 - 2 - 1 - 0 - 0 - 4) crankrev 6,7,8,9
							case 35:   //100011 - (2 - 2 - 1 - 0 - 0 - 4) crankrev 6,7,8,9
							case 41:   //101001 - (2 - 2 - 1 - 0 - 0 - 4) crankrev 6,7,8,9
							case 43:   //101011 - (2 - 2 - 1 - 0 - 0 - 4) crankrev 6,7,8,9
							{
								reader.ReadByte();  //5  Pedal Power Balance
								int CumulativeCranRev1 = reader.ReadByte(); //6 CumulativeCranRevolutions
								int CumulativeCranRev2 = reader.ReadByte(); //7 CumulativeCranRevolutions
								byte lastcranktime1 = reader.ReadByte(); //8 Last Crank Event Time
								byte  lastcranktime2 = reader.ReadByte(); //9 Last Crank Event Time
								lastcranktime3 = lastcranktime2 << 8 | lastcranktime1;
								addedcrankrev = CumulativeCranRev1 + CumulativeCranRev2;
							}
							break;

							case 38:   //100110 - (2 - 2 - 0 - 2 - 0 - 4) crankrev 7,8,9,10
							case 36:   //100100 - (2 - 2 - 0 - 2 - 0 - 4) crankrev 7,8,9,10
							case 44:   //101100 - (2 - 2 - 0 - 2 - 0 - 4) crankrev 7,8,9,10
							case 46:   //101110 - (2 - 2 - 0 - 2 - 0 - 4) crankrev 7,8,9,10
							{
								reader.ReadByte();  //5 Accumulated Torque
								reader.ReadByte();  //6 Accumulated Torque
								int CumulativeCranRev1 = reader.ReadByte(); //7 CumulativeCranRevolutions
								int CumulativeCranRev2 = reader.ReadByte(); //8 CumulativeCranRevolutions
								byte lastcranktime1 = reader.ReadByte(); //9 Last Crank Event Time
								byte lastcranktime2 = reader.ReadByte(); //10 Last Crank Event Time
								lastcranktime3 = lastcranktime2 << 8 | lastcranktime1;
								addedcrankrev = CumulativeCranRev1 + CumulativeCranRev2;
							}
							break;
							
							case 37:   //100101 - (2 - 2 - 1 - 2 - 0 - 4) crankrev 8,9,10,11 
							case 39:   //100111 - (2 - 2 - 1 - 2 - 0 - 4) crankrev 8,9,10,11 
							case 45:   //101101 - (2 - 2 - 1 - 2 - 0 - 4) ccrankrev 8,9,10,11 
							case 47:   //101111 - (2 - 2 - 1 - 2 - 0 - 4) crankrev 8,9,10,11 
							{
								reader.ReadByte();  //5  Pedal Power Balance
								reader.ReadByte();  //6 Accumulated Torque
								reader.ReadByte();  //7 Accumulated Torque
								int CumulativeCranRev1 = reader.ReadByte(); //8 CumulativeCranRevolutions
								int CumulativeCranRev2 = reader.ReadByte(); //9 CumulativeCranRevolutions
								byte lastcranktime1 = reader.ReadByte(); //10 Last Crank Event Time
								byte  lastcranktime2 = reader.ReadByte(); //11 Last Crank Event Time
								lastcranktime3 = lastcranktime2 << 8 | lastcranktime1;
								addedcrankrev = CumulativeCranRev1 + CumulativeCranRev2;
							}
							break;

							case 48:   //110000 - (2 - 2 - 0 - 0 - 6 - 4) crankrev 11,12,13,14
							case 50:   //110010 - (2 - 2 - 0 - 0 - 6 - 4) crankrev 11,12,13,14
							case 56:   //111000 - (2 - 2 - 0 - 0 - 6 - 4) crankrev 11,12,13,14
							case 58:   //111010 - (2 - 2 - 0 - 0 - 6 - 4) crankrev 11,12,13,14
							{
								reader.ReadByte();  //5 Wheel Rev
								reader.ReadByte();  //6 Wheel Rev
								reader.ReadByte();  //7  Wheel Rev
								reader.ReadByte();  //8 Wheel Rev
								reader.ReadByte();  //9 Wheel Rev
								reader.ReadByte();  //10 Wheel Rev
								int CumulativeCranRev1 = reader.ReadByte(); //11 CumulativeCranRevolutions
								int CumulativeCranRev2 = reader.ReadByte(); //12 CumulativeCranRevolutions
								byte lastcranktime1 = reader.ReadByte(); //13 Last Crank Event Time
								byte lastcranktime2 = reader.ReadByte(); //14 Last Crank Event Time
								lastcranktime3 = lastcranktime2 << 8 | lastcranktime1;
								addedcrankrev = CumulativeCranRev1 + CumulativeCranRev2;
							}
							break;

							case 49:   //110001 - (2 - 2 - 1- 0 - 6 - 4) crankrev 12,13,14,15
							case 51:   //110011 - (2 - 2 - 1- 0 - 6 - 4) crankrev 12,13,14,15
							case 57:   //111001 - (2 - 2 - 1- 0 - 6 - 4) crankrev 12,13,14,15
							case 59:   //111011 - (2 - 2 - 1- 0 - 6 - 4) crankrev 12,13,14,15
							{
								reader.ReadByte();  //5 Pedal Power Balance
								reader.ReadByte();  //6 Wheel Rev
								reader.ReadByte();  //7 Wheel Rev
								reader.ReadByte();  //8  Wheel Rev
								reader.ReadByte();  //9 Wheel Rev
								reader.ReadByte();  //10 Wheel Rev
								reader.ReadByte();  //11 Wheel Rev
								int CumulativeCranRev1 = reader.ReadByte(); //12 CumulativeCranRevolutions
								int CumulativeCranRev2 = reader.ReadByte(); //13 CumulativeCranRevolutions
								byte lastcranktime1 = reader.ReadByte(); //14 Last Crank Event Time
								byte lastcranktime2 = reader.ReadByte(); //15 Last Crank Event Time
								lastcranktime3 = lastcranktime2 << 8 | lastcranktime1;
								addedcrankrev = CumulativeCranRev1 + CumulativeCranRev2;
							}
							break;

							case 52:   //110100 - (2 - 2 - 0 - 2 - 6 - 4) crankrev 13,14,15,16
							case 54:   //110110 - (2 - 2 - 0 - 2 - 6 - 4) crankrev 13,14,15,16
							case 60:   //111100 - (2 - 2 - 0 - 2 - 6 - 4) crankrev 13,14,15,16
							case 62:   //111110
							{
								reader.ReadByte();  //5  Accumulated Torque
								reader.ReadByte();  //6  Accumulated Torque
								reader.ReadByte();  //7 Wheel Rev
								reader.ReadByte();  //8 Wheel Rev
								reader.ReadByte();  //9  Wheel Rev
								reader.ReadByte();  //10 Wheel Rev
								reader.ReadByte();  //11 Wheel Rev
								reader.ReadByte();  //12 Wheel Rev
								int CumulativeCranRev1 = reader.ReadByte(); //13 CumulativeCranRevolutions
								int CumulativeCranRev2 = reader.ReadByte(); //14 CumulativeCranRevolutions
								byte lastcranktime1 = reader.ReadByte(); //15 Last Crank Event Time
								byte lastcranktime2 = reader.ReadByte(); //16 Last Crank Event Time
								lastcranktime3 = lastcranktime2 << 8 | lastcranktime1;
								addedcrankrev = CumulativeCranRev1 + CumulativeCranRev2;
							}
							break;

							case 53:   //110101 - (2 - 2 - 1 - 2 - 6 - 4) crankrev 14,15,16,17
							case 55:   //110111 - (2 - 2 - 1 - 2 - 6 - 4) crankrev 14,15,16,17
							case 61:   //111101 - (2 - 2 - 1 - 2 - 6 - 4) crankrev 14,15,16,17
							case 63:   //111111 - (2 - 2 - 1 - 2 - 6 - 4) crankrev 14,15,16,17
							{
								reader.ReadByte();  //5  Pedal Power Balance
								reader.ReadByte();  //6  Accumulated Torque
								reader.ReadByte();  //7  Accumulated Torque
								reader.ReadByte();  //8 Wheel Rev
								reader.ReadByte();  //9 Wheel Rev
								reader.ReadByte();  //10  Wheel Rev
								reader.ReadByte();  //11 Wheel Rev
								reader.ReadByte();  //12 Wheel Rev
								reader.ReadByte();  //13 Wheel Rev
								int CumulativeCranRev1 = reader.ReadByte(); //13 CumulativeCranRevolutions
								int CumulativeCranRev2 = reader.ReadByte(); //14 CumulativeCranRevolutions
								byte lastcranktime1 = reader.ReadByte(); //15 Last Crank Event Time
								byte lastcranktime2 = reader.ReadByte(); //16 Last Crank Event Time
								lastcranktime3 = lastcranktime2 << 8 | lastcranktime1;
								addedcrankrev = CumulativeCranRev1 + CumulativeCranRev2;
							}
							break;
							}
								});
						}
					

					}

				}
			}
		}
		//co_return;
		
		
		//device.Close();
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("devclose")));
		l = 0;
	}

}

int UMyBlueprintFunctionLibrary::LoopStarter(int x, int z, int64 deviceAddress)
{

	return l;
}

int UMyBlueprintFunctionLibrary::HRMExitCheck()
{

	return hrm;
}

float UMyBlueprintFunctionLibrary::CadenceValues(int& cadencerev, int& fifthbitnew)
{
	cadencerev = addedcrankrev;
	fifthbitnew = fifthbit;
	return lastcranktime3;
}

int UMyBlueprintFunctionLibrary::PowerValues()
{
	return y;
}
int UMyBlueprintFunctionLibrary::Power(int64 deviceAddress)
{

BluetoothLEAdvertisementWatcher watcher;
watcher.ScanningMode(BluetoothLEScanningMode::Passive);
watcher.MaxOutOfRangeTimeout() = std::chrono::seconds(10000);
watcher.MinOutOfRangeTimeout() = std::chrono::seconds(50);
watcher.SignalStrengthFilter().OutOfRangeThresholdInDBm() = -127;
watcher.Received([&](BluetoothLEAdvertisementWatcher watcher, BluetoothLEAdvertisementReceivedEventArgs eventArgs)
	{
		watcher.Start();
	});
watcher.Stop();
watcher.Start();

int count = 0;
while ((count++ < 3) && deviceAddress == 0)
{
	std::this_thread::sleep_for(std::chrono::seconds(2));
}
if (deviceAddress != 0)
{

		OpenDevice(deviceAddress).get();
}

		return int(5);
}


IAsyncAction OpenHRMDevice(unsigned long long deviceAddress)
{
	if (hrm == 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("initiated")));
		auto device = co_await BluetoothLEDevice::FromBluetoothAddressAsync(deviceAddress);
		auto services = co_await device.GetGattServicesAsync();
		for (GattDeviceService const& s : services.Services())
		{

			//std::wstring wideStringcad{ L"00002a5b-0000-1000-8000-00805f9b34fb" };  //cadence  ubrat)
			//std::wstring cadservice{ L"00001818-0000-1000-8000-00805f9b34fb" };  //cadence  ubrat)

			auto characteristics = co_await s.GetCharacteristicsAsync();
			std::wstring wideString{ L"00002a37-0000-1000-8000-00805f9b34fb" };

			for (GattCharacteristic const& c : characteristics.Characteristics())
			{
				if (c.CharacteristicProperties() == GattCharacteristicProperties::Notify && guidToString(c.Uuid()) == wideString)
				{
					auto status = co_await c.WriteClientCharacteristicConfigurationDescriptorAsync(GattClientCharacteristicConfigurationDescriptorValue::Notify);
					if (status == GattCommunicationStatus::Success)
					{
						//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("success")));
					

						for (int t{ 0 }; t < 10000000; ++t)
						{
							std::this_thread::sleep_for(std::chrono::milliseconds(300));
							if (hrbreakerglobal == 5)
							{
								t = 100000000;
								hrm = 5;
								break;
								device.Close();

							}
							c.ValueChanged([&]
							(GattCharacteristic const& characteristic, GattValueChangedEventArgs eventArgs)
								{
									hrcheckdisconnect = hrcheckdisconnect + 1;
							if (hrcheckdisconnect > 100000000)
							{
								hrcheckdisconnect = 0;
							}
							DataReader reader = DataReader::FromBuffer(eventArgs.CharacteristicValue());
							byte firstbyte = reader.ReadByte(); //1
							hrmvalue = reader.ReadByte(); //hr value
							//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%d"),hrmvalue));
								});
						}


					}

				}
			}
		}
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("hrclose")));
		hrm = 0;
	}
}


int UMyBlueprintFunctionLibrary::HRMValues()
{
	return hrmvalue;
}

int UMyBlueprintFunctionLibrary::HRMCheck()
{
	return hrcheckdisconnect;
}
int UMyBlueprintFunctionLibrary::HRMData(int64 deviceAddress)
{

	BluetoothLEAdvertisementWatcher watcher;
	watcher.ScanningMode(BluetoothLEScanningMode::Passive);
	watcher.MaxOutOfRangeTimeout() = std::chrono::seconds(10000);
	watcher.MinOutOfRangeTimeout() = std::chrono::seconds(50);
	watcher.SignalStrengthFilter().OutOfRangeThresholdInDBm() = -127;
	watcher.Received([&](BluetoothLEAdvertisementWatcher watcher, BluetoothLEAdvertisementReceivedEventArgs eventArgs)
		{
			watcher.Start();
		});
	watcher.Stop();
	watcher.Start();

	int count = 0;
	while ((count++ < 3) && deviceAddress == 0)
	{
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
	if (deviceAddress != 0)
	{

		OpenHRMDevice(deviceAddress).get();
	}

	return int(5);
}


TArray<int64> UMyBlueprintFunctionLibrary::HelloUnreal(TArray<FString>& devicenamelist)
{
	
		std::string devicename;
		TArray<int64> arr;
		int64 deviceAddress = 0;
		BluetoothLEAdvertisementWatcher watcher;
		watcher.MaxOutOfRangeTimeout() = std::chrono::seconds(10000);
		watcher.MinOutOfRangeTimeout() = std::chrono::seconds(50);
		watcher.SignalStrengthFilter().OutOfRangeThresholdInDBm() = -127;
		watcher.ScanningMode(BluetoothLEScanningMode::Passive);
		watcher.Received([&](BluetoothLEAdvertisementWatcher watcher, BluetoothLEAdvertisementReceivedEventArgs eventArgs)
		{
			watcher.Start();
		
		//std::wstring powerservice{ L"00001818-0000-1000-8000-00805f9b34fb" };
		//std::wstring powerservice2{L"00001816-0000-1000-8000-00805f9b34fb"};
		for (int i{ 0 }; i < 10000;++i)
		{
			//for (GUID const& g : eventArgs.Advertisement().ServiceUuids())
			//{
				//if (guidToString(g) == powerservice or guidToString(g) ==powerservice2 )
				//{
					deviceAddress = eventArgs.BluetoothAddress();
					FString devicename = eventArgs.Advertisement().LocalName().c_str();
					if (devicename.Len() > 0)
					{
						devicenamelist.AddUnique(devicename);
						arr.AddUnique(deviceAddress);
					}
				//}
			//}
			
		}
		});
		watcher.Stop();
		watcher.Start();
		int count = 0;
			while ((count++ < 3) && deviceAddress == 0)
	{ std::this_thread::sleep_for(std::chrono::seconds(2));	}
		
		return  TArray(arr); 
	
}


TArray<int64> UMyBlueprintFunctionLibrary::HRM(TArray<FString>& hrmlist)
{

	std::string devicename;
	TArray<int64> arr;
	int64 hrmAddress = 0;
	BluetoothLEAdvertisementWatcher watcher;
	watcher.MaxOutOfRangeTimeout() = std::chrono::seconds(10000);
	watcher.MinOutOfRangeTimeout() = std::chrono::seconds(50);
	watcher.SignalStrengthFilter().OutOfRangeThresholdInDBm() = -127;
	watcher.ScanningMode(BluetoothLEScanningMode::Passive);
	watcher.Received([&](BluetoothLEAdvertisementWatcher watcher, BluetoothLEAdvertisementReceivedEventArgs eventArgs)
		{
			watcher.Start();
	std::wstring wideString1{ L"0000180d-0000-1000-8000-00805f9b34fb" };

	for (int i{ 0 }; i < 10000;++i)
	{
		//for (GUID const& g : eventArgs.Advertisement().ServiceUuids())
		//{
			//if (guidToString(g) == wideString1)
			//{
				hrmAddress = eventArgs.BluetoothAddress();
				FString devicename = eventArgs.Advertisement().LocalName().c_str();
				if (devicename.Len() > 0)
				{
					hrmlist.AddUnique(devicename);
					arr.AddUnique(hrmAddress);
				}
			//}
		//}
	}
		});
	watcher.Stop();
	watcher.Start();
	int count = 0;
	while ((count++ < 3) && hrmAddress == 0)
	{
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}

	return  TArray(arr);

}


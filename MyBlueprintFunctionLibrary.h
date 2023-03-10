// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#pragma warning(disable : 5205 4265 4268 4946)
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <mutex>
#include <thread>
#include <chrono>
#include <condition_variable>
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/AllowWindowsPlatformAtomics.h"
#include "Windows/PreWindowsApi.h"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Perception.Spatial.h>
#include <unknwn.h>
#include <winrt/Windows.Foundation.Collections.h>
#include "winrt/Windows.Storage.Streams.h"
#include "winrt/Windows.Devices.Bluetooth.h"
#include "winrt/Windows.Devices.Bluetooth.Advertisement.h"
#include "winrt/Windows.Devices.Bluetooth.GenericAttributeProfile.h"
#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformAtomics.h"
#include "Windows/HideWindowsPlatformTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UE5CORO_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
		UFUNCTION(BlueprintCallable, Category = "MyCategory")
			static TArray<int64> HelloUnreal(TArray<FString>& devicenamelist); //long long???
		
		UFUNCTION(BlueprintCallable, Category = "MyCategory")
			static TArray<int64> HRM(TArray<FString>& hrmlist); //long long???

		UFUNCTION(BlueprintCallable, Category = "MyCategory")
			static int Power(int64 deviceAddress); // int64 deviceaddress

		UFUNCTION(BlueprintCallable, Category = "MyCategory")
			static int HRMData(int64 deviceAddress); // int64 deviceaddress

		UFUNCTION(BlueprintCallable, Category = "MyCategory")
			static int LoopBreaker(int x); // 

		UFUNCTION(BlueprintCallable, Category = "MyCategory")
			static int HRMBreaker(int hrbreaker); // 


		UFUNCTION(BlueprintCallable, Category = "MyCategory")
			static int LoopStarter(int x, int z, int64 deviceAddress); //

		UFUNCTION(BlueprintCallable, Category = "MyCategory")
			static int HRMExitCheck(); // checks if HR device was closed to reconnect or exit the game


		UFUNCTION(BlueprintCallable, Category = "MyCategory")
			static int LoopCheck(); //
		UFUNCTION(BlueprintCallable, Category = "MyCategory")
			static int HRMCheck(); //
		UFUNCTION(BlueprintCallable, Category = "MyCategory")

			static int PowerValues(); // 
		UFUNCTION(BlueprintCallable, Category = "MyCategory")
			static int HRMValues(); // 
		UFUNCTION(BlueprintCallable, Category = "MyCategory")
			static float CadenceValues(int& cadencerev, int& fifthbitnew); // 


};
#include <iostream>
#include <cwchar>
#include <string>
#include <nvapi.h>

bool checkForRTX(NvAPI_Status& status) {
	// First get the handles for each GPU
	NvU32 gpuCount;
	NvPhysicalGpuHandle gpuHandles[NVAPI_MAX_PHYSICAL_GPUS];
	status = NvAPI_EnumPhysicalGPUs(gpuHandles, &gpuCount);
	if (status != NVAPI_OK) {
		std::cerr << "Failed to get GPU handles: " << status << std::endl;
		return false;
	}

	// Loop through all GPU handles we just got
	for (NvU32 i = 0; i < gpuCount; i++) {
		char szName[64] = { 0 };
		// retrieve the model name
		status = NvAPI_GPU_GetFullName(gpuHandles[i], szName);
		if (status != NVAPI_OK) {
			std::cerr << "Failed to get GPU #" << i << " full name: " << status << std::endl;
		} else {
			std::cerr << "GPU Found: " << szName << std::endl;
			// Check if GPU is RTX after retrieving model name
			std::string s = szName;
			size_t pos = s.find("RTX", 0); // search for keyword "RTX" starting at index 0
			if (pos != std::string::npos) {
				return true;
			}
		}
	}
	return false;
}


int main()
{
	// Initialize the NVIDIA API
	NvAPI_Status status = NvAPI_Initialize();
	if (status != NVAPI_OK) {
		std::cerr << "Failed to initialize NVIDIA API, make sure your system has NVIDIA Hardware, also try running this program with Administrator | Error : " << status << std::endl;
		return 1;
	}

	std::cerr << "Scanning for RTX GPU" << std::endl;

	// Check if the user has an RTX GPU
	if (checkForRTX(status) == false) {
		std::cerr << "This program only works on machines with RTX Graphics Cards!" << std::endl;
		return 1;
	}

	std::cerr << "RTX GPU Found!" << std::endl;
	std::cerr << "Starting Driver Session" << std::endl;

	// Start a session with a handle for it
	NvDRSSessionHandle hSession;
	status = NvAPI_DRS_CreateSession(&hSession);
	if (status != NVAPI_OK) {
		std::cerr << "Failed to create a driver settings session: " << status << std::endl;
		return 1;
	}

	// Enable the session to be global and handle all profiles on the machine, for some reason load settings is the function for this
	status = NvAPI_DRS_LoadSettings(hSession);
	if (status != NVAPI_OK) {
		std::cerr << "Failed to load system settings for driver session: " << status << std::endl;
		return 1;
	}

	std::cerr << "Attempting to locate Minecraft Driver Profile" << std::endl;
	// We need to find a profile called "Minecraft" and store that found Minecraft profile to a handle
	std::wstring pName = L"Minecraft";
	NvDRSProfileHandle minecraftProfile;
	status = NvAPI_DRS_FindProfileByName(hSession, (NvU16*)pName.c_str(), &minecraftProfile);
	if (status != NVAPI_OK) {
		std::cerr << "Failed to find driver profile \"Minecraft\": " << status << std::endl;
		return 1;
	}

	std::cerr << "Minecraft Driver Profile Found!" << std::endl;
	std::cerr << "Attempting to Modify Minecraft Driver Profile" << std::endl;

	// Then with that profile we need to set the setting with hex ID 0X00DE429A to 0x00000000, this is setting DXR ray tracing to false
	// This should basically disable render dragon, making the game smoother, have way more FPS, and prevent bugs like pink glitch
	NVDRS_SETTING setting;
	memset(&setting, 0, sizeof(setting));
	setting.version = NVDRS_SETTING_VER;
	setting.settingId = 0X00DE429A;
	setting.settingType = NVDRS_DWORD_TYPE;
	setting.u32CurrentValue = 0x00000000;

	status = NvAPI_DRS_SetSetting(hSession, minecraftProfile, &setting);
	if (status != NVAPI_OK) {
		std::cerr << "Failed to modify Minecraft Driver Profile: " << status << std::endl;
		return 1;
	}

	std::cerr << "Successfully Modified Minecraft Driver Profile Settings" << std::endl;
	std::cerr << "Attempting to Save the new Minecraft Driver Profile settings" << std::endl;

	// Save the changes to the driver profile
	status = NvAPI_DRS_SaveSettings(hSession);
	if (status != NVAPI_OK) {
		std::cerr << "Failed to save new Minecraft Driver Profile settings: " << status << std::endl;
		return 1;
	}

	// Clean up by ending session and unloading the nvidia api
	status = NvAPI_DRS_DestroySession(hSession);
	if (status != NVAPI_OK) {
		std::cerr << "Failed to destroy Minecraft driver settings session: " << status << std::endl;
		return 1;
	}
	status = NvAPI_Unload();
	if (status != NVAPI_OK) {
		std::cerr << "Failed to unload NVIDIA API: " << status << std::endl;
		return 1;
	}

	std::cerr << "Successfully Modified and Optimized Render Dragon! discord.gg/swim" << std::endl;
	std::cerr << "You may now close this window" << std::endl;

	std::getchar(); // small hack to prevent the console from closing so user can actually read what was output

	return 0;
}

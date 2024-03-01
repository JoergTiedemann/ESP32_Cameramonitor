#ifndef CONFIG_H
#define CONFIG_H

struct configData
{
	char projectName[32];
	char projectVersion[32];
	bool ShowFilePage;
	bool ShowFileFirmwarePage;
	bool ShowWebFirmwarePage;
	bool ShowDiagnosePage;
	char language[3];
	uint16_t FrameSize;
	uint16_t Quality;
	int16_t Brighness;
	bool VFlip;
	bool Mirror;
	bool whiteBal;
	uint16_t WbMode;
	bool AwbGain;
	uint16_t FirebaseUpdateIntervall;
	char FirebasePassword[60];
	bool ConnectToCloud;
	bool UseStream;
	char FirmwareURL[60];
};

extern uint32_t configVersion;
extern const configData defaults;

#endif
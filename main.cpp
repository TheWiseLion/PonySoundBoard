#include "Global Hook.h"
#include <Windows.h>
int main(){
	PrimaryHooks hook;
	
	hook.SetBoundKey(VK_F1,L"1.wav");
	hook.SetBoundKey(VK_F2,L"2.wav");
	hook.SetBoundKey(VK_F3,L"3.wav");
	hook.SetBoundKey(VK_F4,L"4.wav");
	hook.SetBoundKey(VK_F5,L"5.wav");
	hook.SetBoundKey(VK_F6,L"6.wav");
	hook.SetBoundKey(VK_F7,L"7.wav");
	hook.SetBoundKey(VK_F8,L"8.wav");
	hook.SetBoundKey(VK_F9,L"9.wav");
	hook.SetBoundKey(VK_F10,L"10.wav");
	hook.SetBoundKey(VK_F11,L"11.wav");
	hook.SetBoundKey(VK_F12,L"12.wav");
	hook.SetKeyBoardState(true);
	hook.printKeys();
	while(1){
		Sleep(1);
	}
	return 1337;
}
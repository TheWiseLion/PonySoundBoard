#ifndef __HooksManage__
#define __HooksManage__




#include <Windows.h>
#include <winuser.h>
#include <stdio.h>
#include <stdlib.h>
#include <mmsystem.h>
class PrimaryHooks;
PrimaryHooks * GlobalHookHandle;

HHOOK hKeyHook;
HHOOK hMouseHook;
HANDLE hThread;
DWORD dwThread;
bool MouseState;
bool KeyBoardState;
static bool Exist=false;
struct KeysAndFiles{
	int BoundKey;
	WCHAR* Location;
};
struct KeysAndFiles * Bound=NULL;
int BoundKeysNum;

	WCHAR* CharToWchar(char * text){ //null terminated
		WCHAR * returned=NULL;
		if(text){
			int Size=MultiByteToWideChar(CP_ACP,0,text,-1,NULL,NULL);
			if(Size){
				returned=(WCHAR*)malloc(sizeof(WCHAR)*(Size+1));
				MultiByteToWideChar(CP_ACP,0,text,-1,returned,Size);
			}
			returned[Size]='\0';//wtf really? this is broken
			//free(text);
		}
		return returned;
	}

	// This is the function that is "exported" from the
	// execuatable like any function is exported from a
	// DLL. It is the hook handler routine for low level
	// keyboard events.
	__declspec(dllexport) LRESULT CALLBACK MouseProc(int nCode,WPARAM wParam,LPARAM lParam){
		 if  (nCode == HC_ACTION){
		
			MOUSEHOOKSTRUCT hooked =    *((MOUSEHOOKSTRUCT*)lParam); 

			if(KeyBoardState){
				//MainCallBack(MousePost, hooked.pt.x,hooked.pt.y);
				//MainCallBack(KeyHook, wParam,lParam);
				
			}
			 /*if((wParam==WM_LBUTTONDOWN)||(wParam==WM_LBUTTONUP)){
			
			 }else if((wParam==WM_RBUTTONDOWN)||(wParam==WM_RBUTTONUP)){
			
			 }*/

		 }
			Sleep(1);
			return CallNextHookEx(hMouseHook,
			nCode,wParam,lParam);
		}


	
	__declspec(dllexport) LRESULT CALLBACK KeyEvent ( int nCode,      // The hook code
	 WPARAM wParam,  // The window message (WM_KEYUP, WM_KEYDOWN, etc.)
	 LPARAM lParam   // A pointer to a struct with information about the pressed key
		) {

		if  ((nCode == HC_ACTION) &&       // HC_ACTION means we may process this event

			((wParam == WM_SYSKEYDOWN) ||  // Only react if either a system key ...

			(wParam == WM_KEYDOWN)||(wParam == WM_KEYUP) ))        // ... or a normal key have been pressed.
		{
			//  This struct contains various information about
			//  the pressed key such as hardware scan code, virtual
			//  key code and further flags.
			KBDLLHOOKSTRUCT hooked =    *((KBDLLHOOKSTRUCT*)lParam);    //  dwMsg shall contain the information that would be stored

			if(KeyBoardState){
				if(wParam == WM_KEYDOWN){
					//MainCallBack(KeyHook,Down ,hooked.vkCode);
				}else if(wParam == WM_KEYUP){
					if(Bound !=NULL){
						for(int i=0; i<BoundKeysNum;i++){
							if(Bound[i].BoundKey==hooked.vkCode){
								//CharToWchar(Bound[i].Location)
								PlaySound(Bound[i].Location, NULL, SND_FILENAME|SND_ASYNC);
								//play mic sound?
									//virtual audio cable
								break;
							}

						}

					}
				}else{
					//?? system
				}
				

			}
			//if(wParam == WM_KEYUP){
			//
			//}else if(hooked.vkCode==0xA0||hooked.vkCode==0xA1){ //down not up
			//	
			//}else if(0x0D==hooked.vkCode){
			//	
			//}else if(0x70==hooked.vkCode){ //unhook
			//	UnhookWindowsHookEx(hMouseHook);
			//	CloseHandle(hThread);
			//	hThread=NULL;
			//	UnhookWindowsHookEx(hKeyHook);
			//
			//}else{
		
			////transcribe action
			//	/*WriteLetter(hooked.vkCode,GlobalCFILE);*/
			//}

	//  the return value of the CallNextHookEx routine is always
	//  returned by your HookProc routine. This allows other
	//  applications to install and handle the same hook as well.

		}
		Sleep(1);
		return CallNextHookEx(hKeyHook,nCode,wParam,lParam);
	}



	void MsgLoop()

	{

		MSG message;

		while (GetMessage(&message,NULL,0,0)) {
		
			TranslateMessage( &message );

			DispatchMessage( &message );
			if(hThread==NULL) break; //will cause hooks/threads to stop
		}

	}


		DWORD WINAPI KeyAndMouseHook(LPVOID lpParameter)//installs hooks
	{



	//  Get a module handle to our own executable. Usually,
	//  the return value of GetModuleHandle(NULL) should be
	//  a valid handle to the current application instance,
	//  but if it fails we will also try to actually load
	//  ourself as a library. The thread's parameter is the
	//  first command line argument which is the path to our
	//  executable.

		HINSTANCE hExe = GetModuleHandle(NULL);

		if (!hExe) hExe = LoadLibrary((LPCWSTR) lpParameter);

	//  Everything failed, we can't install the hook ... this
	//  never happened, but error handling is important.

		if (!hExe) return 1;

		hKeyHook = SetWindowsHookEx (  // install the hook:

			WH_KEYBOARD_LL,         // as a low level keyboard hook

			(HOOKPROC) KeyEvent,       // with the MouseProc function from this executable

			hExe,                     // and the module handle to our own executable
			NULL                       // and finally, the hook should monitor all threads.

		);

		//hMouseHook = SetWindowsHookEx (  // install the hook:

		//	WH_MOUSE_LL,         // as a low level keyboard hook

		//	(HOOKPROC) MouseProc,       // with the KeyEvent function from this executable

		//	hExe,                     // and the module handle to our own executable
		//	NULL                       // and finally, the hook should monitor all threads.

		//);

	//  Loop forever in a message loop and if the loop
	//  stops some time, unhook the hook. I could have
	//  added a signal handler for ctrl-c that unhooks
	//  the hook once the application is terminated by
	//  the user, but I was too lazy.

		MsgLoop();

		UnhookWindowsHookEx(hKeyHook);
		//UnhookWindowsHookEx(hMouseHook);

		return 0;

	}

class PrimaryHooks{
private:
	//global hooks passed between 
	HWND ACTIVEWINDOW;
	
	public:
		PrimaryHooks(){
			if(!Exist){
				
				BoundKeysNum=0;
				//generate Thread
				MouseState=0; //off by default
				KeyBoardState=0; //off by default 
				Exist=1;//ensure only one real obj
				hThread = CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)KeyAndMouseHook, NULL, NULL, &dwThread);
			}else{
				//why you double constructucting? 
			}
		};

		// its not actually privite but increases readability ....
		void SetMouseState(bool In){// one for sending mouse events (up/down)
			MouseState=In;
		}

		
		void SetKeyBoardState(bool In){// one for sending key events (up/down)
			KeyBoardState=In;
		}

		void SetBoundKey(int virtualKey, WCHAR* location){
			struct KeysAndFiles * destination;
			BoundKeysNum++;
			if(Bound==NULL){
				Bound=new KeysAndFiles[1];
				Bound[0].BoundKey=virtualKey;
				Bound[0].Location=location;
				//memcpy(Bound[0].Location,location,strlen(location));
			}else{
				destination=new KeysAndFiles[BoundKeysNum];
				memcpy(destination,Bound,BoundKeysNum*sizeof(KeysAndFiles));
				free(Bound);
				destination[BoundKeysNum-1].BoundKey=virtualKey;
				destination[BoundKeysNum-1].Location=location;
				//memcpy(destination[BoundKeysNum-1].Location,location,strlen(location));
				Bound=destination;
			}
		}
		
		void RemoveKey(int virtualKey){
			//no need yet
		}

		void printKeys(){
			for(int i=0; i<BoundKeysNum;i++)
				printf("%d\n",Bound[i].BoundKey);
		}

		~PrimaryHooks(){ //remove hooks, ends thread
		hThread=NULL;
		Sleep(1);
		}
};





#endif
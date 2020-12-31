#include "GL/glew.h"
#include "GL/freeglut.h"
#pragma once
typedef struct
{
	float x, y;
} vec2d_f;

typedef struct
{
	float x, y, z;
} vec3d_f;

typedef struct
{
	float x, y, z, w;
} vec4d_f;

static int base_address = 0x00400000;
static int player_base = base_address + 0x0010F4F4;

// Direct reading
static int entity_base = player_base + 0x4;
static int players_in_map = player_base + 0xC;

// Player structure
static int of_health = 0xF8;
static int of_armor = 0xFC;
static int of_name = 0x225;
static int of_posx = 0x4;
static int of_posy = 0x8;
static int of_posz = 0xC;
static int of_ang_left_right = 0x40;
static int of_ang_up_down = 0x44;
static int of_team = 0x32C;
static int of_viewmatrix = 0x501AE8;
static int of_posx_normal = 0x34;
static int of_posy_normal = 0x38;
static int of_posz_normal = 0x3C;

class Player {

	//private:
	//	WinFunc winFunc;

public:

	char name[20];
	int base;
	int armor;
	int health;
	int tbase;
	int idx;
	bool isUser;
	HANDLE phandle;
	//float matrix[16];
	vec2d_f screen;
	//vec3d_f position_head;
	//vec3d_f position_feet;
	vec3d_f position;
	//int team;

	Player()
	{}


	Player(HANDLE arghandle) //플레이어인 경우 
	{
		isUser = TRUE;
		phandle = arghandle;
		ReadProcessMemory(phandle, (LPCVOID)player_base, &base, sizeof(base), 0);
		ReadProcessMemory(phandle, (LPCVOID)(base + of_posx), &position.x, sizeof(position.x), 0);
		ReadProcessMemory(phandle, (LPCVOID)(base + of_posy), &position.y, sizeof(position.y), 0);
		ReadProcessMemory(phandle, (LPCVOID)(base + of_posz), &position.z, sizeof(position.z), 0);
		ReadProcessMemory(phandle, (LPCVOID)(base + of_health), &health, sizeof(health), 0);
		ReadProcessMemory(phandle, (LPCVOID)(base + of_armor), &armor, sizeof(armor), 0);
		ReadProcessMemory(phandle, (LPCVOID)(base + of_name), &name, sizeof(name), 0);
		ReadProcessMemory(phandle, (LPCVOID)(base + of_ang_left_right), &screen.x, sizeof(screen.x), 0);
		ReadProcessMemory(phandle, (LPCVOID)(base + of_ang_up_down), &screen.y, sizeof(screen.y), 0);
	}

	Player(HANDLE arghandle, int pidx)//플레이어 외 적의 경우 
	{
		isUser = FALSE;
		idx = pidx + 1;
		phandle = arghandle;
		ReadProcessMemory(phandle, (LPCVOID)entity_base, &tbase, sizeof(tbase), 0);
		ReadProcessMemory(phandle, (LPCVOID) (tbase + (int)(idx*4)), &base, sizeof(base), 0);
		ReadProcessMemory(phandle, (LPCVOID)(base + of_posx), &position.x, sizeof(position.x), 0);
		ReadProcessMemory(phandle, (LPCVOID)(base + of_posy), &position.y, sizeof(position.y), 0);
		ReadProcessMemory(phandle, (LPCVOID)(base + of_posz), &position.z, sizeof(position.z), 0);
		ReadProcessMemory(phandle, (LPCVOID)(base + of_health), &health, sizeof(health), 0);
		ReadProcessMemory(phandle, (LPCVOID)(base + of_name), &name, sizeof(name), 0);
	}

	void update() {
		if (isUser) {
			ReadProcessMemory(phandle, (LPCVOID)(base + of_ang_left_right), &screen.x, sizeof(screen.x), 0);
			ReadProcessMemory(phandle, (LPCVOID)(base + of_ang_up_down), &screen.y, sizeof(screen.y), 0);
		}
		else {
			ReadProcessMemory(phandle, (LPCVOID)entity_base, &tbase, sizeof(tbase), 0);
			ReadProcessMemory(phandle, (LPCVOID)(tbase + (int)(idx * 4)), &base, sizeof(base), 0);
		}
		ReadProcessMemory(phandle, (LPCVOID)(base + of_posx), &position.x, sizeof(position.x), 0);
		ReadProcessMemory(phandle, (LPCVOID)(base + of_posy), &position.y, sizeof(position.y), 0);
		ReadProcessMemory(phandle, (LPCVOID)(base + of_posz), &position.z, sizeof(position.z), 0);
		ReadProcessMemory(phandle, (LPCVOID)(base + of_health), &health, sizeof(health), 0);
		ReadProcessMemory(phandle, (LPCVOID)(base + of_name), &name, sizeof(name), 0);
		
	}
	//void Print();
};
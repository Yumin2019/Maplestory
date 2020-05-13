#pragma once

enum MAP_TYPE
{
	MT_MAIN,
	MT_MUSHROOM1,
	MT_MUSHROOM2,
	MT_GOLEM,
	MT_TRAININGROOM
};

enum PLAYER_MOTION
{
	PM_ALERT,
	PM_DIE,
	PM_HIT,
	PM_JUMP,
	PM_PRONE,
	PM_PRONE_ATTACK,
	PM_ROPE,
	PM_SIT,
	PM_STAB,
	PM_STAND,
	PM_SWING1,
	PM_SWING2,
	PM_SWING3,
	PM_WALK,
	PM_SLASH
	// ��ų���� �߰� �ٶ�
};

enum MONSTER_MOTION
{
	MM_STAND,
	MM_HIT,
	MM_DIE,
	MM_MOVE,
	MM_JUMP,
	MM_SKILL
};

enum PEN_COLOR
{
	PC_BLACK,
	PC_RED,
	PC_BLUE,
	PC_MAGENTA,
	PC_WHITE,
	PC_SIZE
};

enum FONT_TYPE
{
	FT_UI, // HPMP BAR�� ���
	FT_LV, // ������ ��
	FT_ITEM,
	FT_SIZE
};
enum CHANGE_SCENE
{
	CS_NONE,
	CS_CHANGE
};


enum SCENE_CLIP
{
	SC_NONE,
	SC_CURRENT,
	SC_NEXT
};

enum PANEL_TYPE
{
	PT_NONE,
	PT_HPBAR,
	PT_MPBAR,
	PT_HPMPBORDER,
	PT_EXPBORDER,
	PT_EXPBAR,
	PT_MONSTERHPBAR,
	PT_MONSTERHPBORDER,

	PT_MINIMAP_MAIN,
	PT_MINIMAP_MUSH1,
	PT_MINIMAP_MUSH2,
	PT_MINIMAP_TRAININGROOM,
	PT_MINIMAP_GOLEM,
	PT_MINIMAPBORDER,

	PT_MINIMAP_CIRCLE_MAIN,
	PT_MINIMAP_CIRCLE_MUSH1,
	PT_MINIMAP_CIRCLE_MUSH2,
	PT_MINIMAP_CIRCLE_GOLEM,
	PT_MINIMAP_CIRCLE_TRAININGROOM,
	PT_NAME,

	PT_QUICKSLOT,
	PT_QUICKSLOT_ALPHA,

	PT_SKILL,
	PT_ABILITY,
	PT_INVENTORY,
	PT_SHOP,
	PT_SCROLL

	// �������� ����, �׳� ������ �ִ� ������.

};

enum BUTTON_STATE
{
	BS_NONE,
	BS_MOUSEON,
	BS_CLICK
};


enum IMAGE_OPTION
{
	IO_ATLAS,
	IO_FRAME
};

enum MOVE_DIRECTION
{
	MD_NONE,
	MD_RIGHT = 1,
	MD_LEFT = -1
};

enum ANIMATION_OPTION
{
	AO_LOOP,
	AO_ONCE_RETURN, // 1 ��
	AO_ONCE_DESTROY,
	AO_TIME_RETURN, // TIME
	AO_TIME_DESTROY
};

enum OBJECT_TYPE
{
	OT_NONE,
	OT_MONSTER,
	OT_PLAYER,
	OT_NPC,
	OT_UI,
	OT_ITEM,
	OT_EFFECT,
	OT_MAP,
	OT_MOUSE
};

enum ATTACK_TYPE
{
	AT_NONE,
	AT_NORMAL_ATTACK_LEFT,
	AT_NORMAL_ATTACK_RIGHT,
	AT_SKILL_ATTACK_LEFT,
	AT_SKILL_ATTACK_RIGHT
};
enum EFFECT_TYPE
{
	// �����̴� ģ������ �ƴϸ� �����ϰ�, �����̴� ģ����� �¿� ������.
	ET_NONE,
	ET_SWING1,
	ET_SWING2,
	ET_SWING3,
	ET_PRONE_ATTACK_LEFT,
	ET_PRONE_ATTACK_RIGHT,
	ET_STAB,
	ET_SKILL, // �߰��� ���
	ET_SKILL_LEAF // �� �� �� 
};

enum MONSTER_NAME
{
	RED_SNAIL,
	BLUE_SNAIL,
	GOLEM,
	GREEN_MUSHROOM,
	BLUE_MUSHROOM,
	ORANGE_MUSHROOM,
	MUSHMOM,
	TREE

};

enum SHAPE_TYPE
{
	// �뵵�� ������ ó���Ͽ� �ӵ��� ���.

	ST_NONE,
	ST_CIRCLE,
	ST_MOUSE,

	ST_RECT_OBSTACLE, // OBSTACLE �÷��̾� ���.
	ST_RECT_TERRAIN, // MAGENTA �ٴ�

	ST_RECT_ROPE, // ROPE���� �ߴµ� �پ��ϰ� ���

	ST_RECT_PORTAL, // ��Ż

	ST_RECT_TURN, // ���� TURN �뵵�� ���.

	ST_RECT_MONSTER_ATTACK, // ���� ������ �뵵

	ST_RECT_NPC,

	ST_RECT_MONSTER, // ���� �´� �� vs �÷��̾� ������ �� �浹�� ���
	ST_RECT_SKILL_LEFT,
	ST_RECT_SKILL_RIGHT,
	ST_RECT_NORMAL_LEFT,
	ST_RECT_NORMAL_RIGHT,

	ST_PIXEL,

	ST_SKILL_UI_BUTTON,
	ST_INVENTORY_UI_BUTTON,
	ST_ABILITY_UI_BUTTON,
	ST_QUICKSLOT_UI,
	ST_SHOP_UI_BUTTON, 
	ST_MINIMAP,
	ST_ITEM_BLOCK,

	ST_SHOP_SCROLL,


	/*
	�ϴ�, ��������... flag�� ���� ���콺���� m_pPanel�� ��Ƽ� ó���� �ϴµ�, ���⿡��
	�߿��� ���� ZOrder �ٲٴ� ó���̴�. �Ϻ� flag�� Ŭ���� �Ǵ���, �ö���� �� �ǰ�,
	�Ϻδ� �ݵ�� �ö�;� �Ѵ�.

	���� ��, �κ��丮�� ������ ������ Ŭ���Ѵٰ� �ؼ� �ö���� �� ������, 
	size�� �ö�;� �Ѵ�.

	x ���� �ö���� �� �ȴ�. (�̰� �ö���ϱ� ����.. �ö�ͼ�, ó��, ���󰡴�...) �̰� �� ����...
	��ġ�µ�, �ϴ� �ö󰡼�, panel�� �����ϱ� ó���� �Ǵ� ���̴�.....

	������ ��ư���� ������ �ʿ伺�� �ִ�.

	size, XButton, = �⺻

	���� / �κ��丮 -> ui button���� ��������.. ĭ����... ����.. 

	�̤�

	���߿� �� �غ���.
	
	
	*/
	
};

enum ITEM_TAG
{
	IT_NONE = -1,
	IT_RED_POTION,
	IT_ORANGE_POTION,
	IT_WHITE_POTION,
	IT_BLUE_POTION,
	IT_SPEED_POTION,
	IT_WORRIOR_POTION,
	IT_APPLE,
	IT_EGG,
	IT_ORANGE,
	IT_SIZE,
	IT_MESO
};

enum COLLISION_STATE
{
	CS_START,
	CS_STAY,
	CS_END,
	CS_SIZE
};
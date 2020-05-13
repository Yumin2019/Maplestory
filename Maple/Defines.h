#pragma once


#define SAFE_DELETE(p) if(p){delete p; p = NULL;}
#define SAFE_DELETE_ARRAY(p) if(p){delete[] p; p = NULL;}

#define DECLARE_SINGLE(Type) \
private: \
static Type* m_pInst;\
public:\
static Type* GetInst()\
{\
if(!m_pInst) m_pInst = new Type; \
return m_pInst;\
}\
static void DestroyInst()\
{\
SAFE_DELETE(m_pInst); \
}\
private:\
Type();\
~Type();

// RGB DEFINE
#define OBSTACLE RGB(0, 255, 255)
#define ROPE RGB(0, 128, 0)
#define MONSTER_OBSTACLE RGB(128, 0, 0)
#define MAGENTA RGB(255, 0, 255)
#define WHITE RGB(255, 255, 255)
#define BLACK RGB(0, 0, 0)
#define BLUE RGB(0, 162, 232)
#define RED  RGB(237, 28, 36)

#define GET_SINGLE(Type) Type::GetInst()
#define DESTROY_SINGLE(Type) Type::DestroyInst()
#define DEFINITION_SINGLE(Type) Type* Type::m_pInst = NULL
#define SAFE_RELEASE(p) if(p) {p->Release(); p = NULL;}
#define GET_RS() CCore::GetInst()->GetRS()

#define FULL_WIDTH 1920
#define FULL_HEIGHT 1080

#define RS_WIDTH 1280
#define RS_HEIGHT 720

#define CAMERA_WIDTH2 640
#define CAMERA_HEIGTH2 360

#define UI_MAX_IDX 4
#define SCROLL_HEIGHT 21.6666666666f
#define SCROLL_IDX_MAX	15

#define BarX 578.f

#define HPBarY 624.f
#define MPBarY 640.f



#define FIND(Type) (Type*)GET_SINGLE(CCreateManager)
#define PEN GET_SINGLE(CPencil)

#define KEYDOWN(Key) CInput::GetInst()->KeyDown(Key)
#define KEYPRESS(Key) CInput::GetInst()->KeyPress(Key)
#define KEYUP(Key) CInput::GetInst()->KeyUp(Key)
#define KEYKEY(Key) KEYDOWN(Key) || KEYPRESS(Key)

// »ç¿ë¹ý FIND(TYPE)->FindObj("Key");

template<typename T>
void Safe_Delete_VecList(T& p)
{
	T::iterator iter;
	T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}

	p.clear();
}




template<typename T>
void Safe_Release_VecList(T& p)
{
	T::iterator iter;
	T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE((*iter));
	}

	p.clear();
}


template<typename T>
void Safe_Release_Map(T& p)
{
	T::iterator iter;
	T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE(iter->second);
	}

	p.clear();
}


template<typename T>
void Safe_Delete_Map(T& p)
{
	T::iterator iter;
	T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	p.clear();
}

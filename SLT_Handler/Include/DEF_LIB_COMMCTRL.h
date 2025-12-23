#pragma  once

#define MAX_SPREAD_USER_CONFIG			(50)

typedef struct _ST_SMB_INFO {
	unsigned int	hWnd;
	char			szCode[32];
	char			szMessage[1024];

	_ST_SMB_INFO()
	{
		memset(this, 0x00, sizeof(_ST_SMB_INFO));
	};
}ST_SMB_INFO, *LPST_SMB_INFO;

typedef struct _St_Keyboard
{
public:
	char	szFirstKey[128];
	char	szSecondKey[128];
	int		nKeyID;
}St_Keyboard;

typedef struct _ST_USER_CONFIG_STR
{
	char   szAxis[128];
	char   szItem[512];
	int    nUserConfigSet;

	_ST_USER_CONFIG_STR()
	{
		memset(this, 0x00, sizeof(_ST_USER_CONFIG_STR));	}

}ST_USER_CONFIG_STR, *LPST_USER_CONFIG_STR;

typedef struct _ST_USER_CONFIG_ARRAY
{
	ST_USER_CONFIG_STR StContent[50];	

	_ST_USER_CONFIG_ARRAY()
	{
		memset(this, 0x00, sizeof(_ST_USER_CONFIG_ARRAY));
	}
}ST_USER_CONFIG_ARRAY;

enum USER_LEVEL_TYPE
{
	USERLEVEL_OPERATOR = 0,
	USERLEVEL_TECHNICIAN,
	USERLEVEL_ENGINEER,
	USERLEVEL_SUPERVISOR,

	ID_EDIT_FOCUS = 1,
	PW_EDIT_FOCUS,
	NOT_EDIT_FOCUS,
};



#define ADMIN_ACCOUNT					"sw"
#define ADMIN_PASSWORD					"software"
#define PATH_LOGIN_FILE					"sys_data//accesslevel.dat"
#define MAX_LOGON_TEXT_LENGTH			(20)
#define MAX_KEY_VALUE					(36)

#define RETURN_RESOURCE_ERR    (-2)
#define RETURN_ERR			   (-1)
#define RETURN_OK			   (1)
#define RETURN_CANCEL		   (0)

#define	M_ICONINFORMATION		0x01
#define	M_ICONQUESTION			0x02
#define	M_ICONSTOP				0x04
#define	M_ICONERROR				0x08

#define	M_OK					0x10
#define	M_OKCANCEL				0x20
#define	M_YESNO					0x40
#define	M_ABORTRETRYIGNORE		0x80
#define	M_RETRYCANCEL			0x100


#define UM_SHOW_MSG_BOX_RSP (WM_USER+1000)

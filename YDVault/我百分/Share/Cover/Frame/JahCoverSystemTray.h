#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H
#define WM_ICON_NOTIFY WM_USER+123
//##ModelId=4140A4D201C7
class CJahCoverSystemTray 
{
public:
	//##ModelId=4140A4D20206
    CJahCoverSystemTray();
	//##ModelId=4140A4D20207
    CJahCoverSystemTray(CWnd* pParent, UINT uCallbackMessage, LPCTSTR szToolTip, HICON icon, UINT uID);
	//##ModelId=4140A4D20204
    virtual ~CJahCoverSystemTray();

public:
	//##ModelId=4140A4D201FC
    Create(CWnd* pParent, UINT uCallbackMessage, LPCTSTR szToolTip, HICON icon, UINT uID);
	//##ModelId=4140A4D201FA
    BOOL  SetIcon(UINT nIDResource);
	//##ModelId=4140A4D201F9
    void  HideIcon();
	//##ModelId=4140A4D201F6
    void  RemoveIcon();
	//##ModelId=4140A4D201F3
    BOOL SetMenuDefaultItem(UINT uItem, BOOL bByPos);
	//##ModelId=4140A4D201EF
    virtual LRESULT OnTrayNotification(WPARAM uID, LPARAM lEvent);

protected:
	//##ModelId=4140A4D201EA
	void			Initialise();
	//##ModelId=4140A4D201E9
    BOOL            m_bEnabled;   
	//##ModelId=4140A4D201E8
    BOOL            m_bHidden;    
	//##ModelId=4140A4D201E5
    NOTIFYICONDATA  m_tnd;
	//##ModelId=4140A4D201D2
    UINT			m_DefaultMenuItemID;
	//##ModelId=4140A4D201D1
    BOOL			m_DefaultMenuItemByPos;
};

#endif

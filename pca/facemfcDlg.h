// facemfcDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"

#include "./include/cv.h"  // include core library interface
#include "./include/highgui.h" // include GUI library interface
#include "./include/cvaux.h"
#include "./include/cxcore.h"
#include "./include/cvcam.h"

#include "KPCA.h"

// CfacemfcDlg ��ȭ ����
class CfacemfcDlg : public CDialog
{
// �����Դϴ�.
public:
	CfacemfcDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	


// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_FACEMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:

	afx_msg void OnBnClickedButton2();  // ĸ��(�� ��)
	afx_msg void OnBnClickedButton4();  // Adaboost
	
	void AddMsg(CString msg);
	void FaceDetection(IplImage *image, double scale);
	virtual BOOL DestroyWindow();

	CListBox m_List;

	HANDLE m_ThHandle; //thread
	bool m_isStop;

	IplImage* m_tempimage;
	IplImage* m_saveimage;
	IplImage* m_gray;
	IplImage* m_copy;
	CvCapture *m_capture2;	
		
	afx_msg void OnBnClickedButton1();
	int m_dbnum;
	CEdit m_db_num;
	afx_msg void OnBnClickedCancel();
};

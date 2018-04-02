

#pragma once

#include "GUI.h"

#include "Interfaces.h"

void CWindow::SetPosition(int x, int y)
{
	m_x = x; m_y = y;
}

void CWindow::SetSize(int w, int h)
{
	m_iWidth = w;
	m_iHeight = h;
}

void CWindow::SetTitle(std::string title)
{
	Title = title;
}

void CWindow::RegisterTab(CTab* Tab)
{
	if (Tabs.size() == 0)
	{
		SelectedTab = Tab;
	}
	Tab->parent = this;
	Tabs.push_back(Tab);
}

RECT CWindow::GetClientArea()
{
		RECT client;
	client.left = m_x + 5;
	client.top = m_y + 1 + 7;
	client.right = m_iWidth;
	client.bottom = m_iHeight - 2 - 8 - 26 + 500;
	return client;
}

RECT CWindow::GetClientArea1()
{
	RECT client;
	client.left = m_x + 8;
	client.top = m_y + 1 + 7;
	client.right = m_iWidth - 4 - 12 + 2;
	client.bottom = m_iHeight - 2 - 8 - 26 + 500;
	return client;
}

RECT CWindow::GetDragArea()
{
	RECT client;
	client.left = m_x + 23;
	client.top = m_y + 1;
	client.right = m_iWidth - 4 - 12;
	client.bottom = 29;
	return client;
}

#define UI_WIN_TOPHEIGHT	26
#define UI_WIN_TITLEHEIGHT	32
#define UI_TAB_WIDTH		130

RECT CWindow::GetTabArea()
{
	RECT client;
	client.left = m_x;
	client.top = m_y + UI_WIN_TOPHEIGHT + UI_WIN_TITLEHEIGHT;
	client.right = UI_TAB_WIDTH;
	client.bottom = m_iHeight - UI_WIN_TOPHEIGHT - UI_WIN_TITLEHEIGHT;
	return client;
}

void CWindow::Open()
{
	m_bIsOpen = true;
}

void CWindow::Close()
{
	m_bIsOpen = false;
}

void CWindow::Toggle()
{
	m_bIsOpen = !m_bIsOpen;
	if (m_bIsOpen)
		m_pEngine->ClientCmd_Unrestricted("cl_mouseenable 0");
	else
		m_pEngine->ClientCmd_Unrestricted("cl_mouseenable 1");
}

bool CWindow::isOpen()
{
	return m_bIsOpen;
}

CControl* CWindow::GetFocus()
{
	return FocusedControl;
}

// TABS ---------------------------------------------------------------------------------------------------

void CTab::SetTitle(std::string name)
{
	Title = name;
}

void CTab::RegisterControl(CControl* control)
{
	control->parent = parent;
	Controls.push_back(control);
}
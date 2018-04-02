
#include "GUI.h"

#include "DrawManager.h"
#include "MetaInfo.h"
#include "Menu.h"
#include <algorithm>
#include "tinyxml2.h"
#include "Controls.h"

#include <sstream>


float MenuAlpha = 0.05f;
float MenuAlpha3 = 0.05f;
float MenuAlpha5 = 0.05f;
float MenuAlpha7 = 0.05f;
float MenuAlpha9 = 0.05f;
float MenuAlpha11 = 0.05f;
float MenuAlpha13 = 0.05f;
float MenuAlpha15 = 0.05f;
float MenuAlpha17 = 0.05f;
float MenuAlpha19 = 0.05f;
float MenuAlpha21 = 0.05f;
float MenuAlpha23 = 0.05f;
float Globals::MenuAlpha24 = MenuAlpha23;
float Globals::MenuAlpha22 = MenuAlpha21;
float Globals::MenuAlpha20 = MenuAlpha19;
float Globals::MenuAlpha18 = MenuAlpha17;
float Globals::MenuAlpha16 = MenuAlpha15;
float Globals::MenuAlpha14 = MenuAlpha13;
float Globals::MenuAlpha12 = MenuAlpha11;
float Globals::MenuAlpha10 = MenuAlpha9;
float Globals::MenuAlpha8 = MenuAlpha7;
float Globals::MenuAlpha6 = MenuAlpha5;
float Globals::MenuAlpha2 = MenuAlpha;
float Globals::MenuAlpha4 = MenuAlpha3;

CGUI GUI;

CGUI::CGUI()
{

}

// Draws all windows 
void CGUI::Draw()
{
	bool ShouldDrawCursor = false;

	for (auto window : Windows)
	{
		if (window->m_bIsOpen)
		{
			ShouldDrawCursor = true;
			DrawWindow(window, 1);
		}
	}

	if (ShouldDrawCursor)
	{
		draw.rect(Mouse.x + 1, Mouse.y, 1, 17, Color(3, 6, 26, 255));
		for (int i = 0; i < 11; i++)
			draw.rect(Mouse.x + 2 + i, Mouse.y + 1 + i, 1, 1, Color(3, 6, 26, 255));
		draw.rect(Mouse.x + 8, Mouse.y + 12, 5, 1, Color(3, 6, 26, 255));
		draw.rect(Mouse.x + 8, Mouse.y + 13, 1, 1, Color(3, 6, 26, 255));
		draw.rect(Mouse.x + 9, Mouse.y + 14, 1, 2, Color(3, 6, 26, 255));
		draw.rect(Mouse.x + 10, Mouse.y + 16, 1, 2, Color(3, 6, 26, 255));
		draw.rect(Mouse.x + 8, Mouse.y + 18, 2, 1, Color(3, 6, 26, 255));
		draw.rect(Mouse.x + 7, Mouse.y + 16, 1, 2, Color(3, 6, 26, 255));
		draw.rect(Mouse.x + 6, Mouse.y + 14, 1, 2, Color(3, 6, 26, 255));
		draw.rect(Mouse.x + 5, Mouse.y + 13, 1, 1, Color(3, 6, 26, 255));
		draw.rect(Mouse.x + 4, Mouse.y + 14, 1, 1, Color(3, 6, 26, 255));
		draw.rect(Mouse.x + 3, Mouse.y + 15, 1, 1, Color(3, 6, 26, 255));
		draw.rect(Mouse.x + 2, Mouse.y + 16, 1, 1, Color(3, 6, 26, 255));
		for (int i = 0; i < 4; i++)
			draw.rect(Mouse.x + 2 + i, Mouse.y + 2 + i, 1, 14 - (i * 2), Color(167, 244, 66, 255));
		draw.rect(Mouse.x + 6, Mouse.y + 6, 1, 8, Color(167, 244, 66, 255));
		draw.rect(Mouse.x + 7, Mouse.y + 7, 1, 9, Color(167, 244, 66, 255));
		for (int i = 0; i < 4; i++)
			draw.rect(Mouse.x + 8 + i, Mouse.y + 8 + i, 1, 4 - i, Color(167, 244, 66, 255));
		draw.rect(Mouse.x + 8, Mouse.y + 14, 1, 4, Color(167, 244, 66, 255));
		draw.rect(Mouse.x + 9, Mouse.y + 16, 1, 2, Color(167, 244, 66, 255));
	}
}

// Handle all input etc
void CGUI::Update()
{
	//Key Array
	std::copy(keys, keys + 255, oldKeys);
	for (int x = 0; x < 255; x++)
	{
		//oldKeys[x] = oldKeys[x] & keys[x];
		keys[x] = (GetAsyncKeyState(x));
	}
	
	// Mouse Location
	POINT mp; GetCursorPos(&mp);
	Mouse.x = mp.x; Mouse.y = mp.y;

	RECT Viewport = { 0, 0, 0, 0 };
	int w, h;
	m_pEngine->GetScreenSize(w, h);
	Viewport.right = w; Viewport.bottom = h;
	RECT Screen = Viewport;

	// Window Binds
	for (auto& bind : WindowBinds)
	{
		if (GetKeyPress(miscconfig.iMenuKey))
		{
			bind.second->Toggle();
		}
	}

	// Stop dragging
	if (IsDraggingWindow && !GetKeyState(VK_LBUTTON))
	{
		IsDraggingWindow = false;
		DraggingWindow = nullptr;
	}

	// If we are in the proccess of dragging a window
	if (IsDraggingWindow && GetKeyState(VK_LBUTTON) && !GetKeyPress(VK_LBUTTON))
	{
		if (DraggingWindow)
		{
			DraggingWindow->m_x = Mouse.x - DragOffsetX;
			DraggingWindow->m_y = Mouse.y - DragOffsetY;
		}
	}

	// Process some windows
	for (auto window : Windows)
	{
		if (window->m_bIsOpen)
		{
			// Used to tell the widget processing that there could be a click
			bool bCheckWidgetClicks = false;

			// If the user clicks inside the window
			if (GetKeyPress(VK_LBUTTON))
			{
				if (IsMouseInRegion(window->GetClientArea()))
				{
					// User is selecting a new tab
					if (IsMouseInRegion(window->GetTabArea()))
					{
						/*// Loose focus on the control
						window->IsFocusingControl = false;
						window->FocusedControl = nullptr;

						int iTab = 0;
						int TabCount = window->Tabs.size();
						if (TabCount) // If there are some tabs
						{
						int TabSize = (window->m_iWidth - 4 - 12) / TabCount;
						int Dist = Mouse.x - (window->m_x + 8);
						while (Dist > TabSize)
						{
						if (Dist > TabSize)
						{
						iTab++;
						Dist -= TabSize;
						}
						}
						window->SelectedTab = window->Tabs[iTab];
						}*/
					}
					else
						bCheckWidgetClicks = true;
				}
			}
			if (IsMouseInRegion(window->GetDragArea()))
			{
				// Must be in the around the title or side of the window
				// So we assume the user is trying to drag the window
				IsDraggingWindow = true;
				DraggingWindow = window;
				DragOffsetX = Mouse.x - window->m_x;
				DragOffsetY = Mouse.y - window->m_y;

				// Loose focus on the control
				window->IsFocusingControl = false;
				window->FocusedControl = nullptr;
			}

			// Controls 
			if (window->SelectedTab != nullptr)
			{
				// Focused widget
				bool SkipWidget = false;
				CControl* SkipMe = nullptr;

				// this window is focusing on a widget??
				if (window->IsFocusingControl)
				{
					if (window->FocusedControl != nullptr)
					{
						CControl* control = window->FocusedControl;
						CGroupBox* group;
						if (control->FileControlType != UIControlTypes::UIC_GroupBox) group = control->parent_group ? (CGroupBox*)control->parent_group : nullptr;

						if (group != nullptr && control->FileControlType != UIControlTypes::UIC_GroupBox)
						{
							if ((group->group_tabs.size() > 0 && control->g_tab == group->selected_tab) || group->group_tabs.size() == 0)
							{
								// We've processed it once, skip it later
								SkipWidget = true;
								SkipMe = window->FocusedControl;

								POINT cAbs = window->FocusedControl->GetAbsolutePos();
								RECT controlRect = { cAbs.x, cAbs.y, window->FocusedControl->m_iWidth, window->FocusedControl->m_iHeight };
								window->FocusedControl->OnUpdate();

								if (window->FocusedControl->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
								{
									window->FocusedControl->OnClick();

									bCheckWidgetClicks = false;
								}
							}
						}
						else if (control->FileControlType == UIControlTypes::UIC_GroupBox || control->FileControlType != UIControlTypes::UIC_GroupBox && !control->parent_group)
						{
							// We've processed it once, skip it later
							SkipWidget = true;
							SkipMe = window->FocusedControl;

							POINT cAbs = window->FocusedControl->GetAbsolutePos();
							RECT controlRect = { cAbs.x, cAbs.y, window->FocusedControl->m_iWidth, window->FocusedControl->m_iHeight };
							window->FocusedControl->OnUpdate();

							if (window->FocusedControl->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
							{
								window->FocusedControl->OnClick();

								// If it gets clicked we loose focus
								window->IsFocusingControl = false;
								window->FocusedControl = nullptr;
								bCheckWidgetClicks = false;
							}
						}
					}
				}

				// Itterate over the rest of the control
				for (auto control : window->SelectedTab->Controls)
				{
					if (control != nullptr)
					{
						CGroupBox* group;
						if (control->FileControlType != UIControlTypes::UIC_GroupBox) group = control->parent_group ? (CGroupBox*)control->parent_group : nullptr;

						if (group != nullptr && control->FileControlType != UIControlTypes::UIC_GroupBox)
						{
							if (group->group_tabs.size() > 0 && control->g_tab == group->selected_tab || group->group_tabs.size() == 0)
							{
								if (SkipWidget && SkipMe == control)
									continue;

								POINT cAbs = control->GetAbsolutePos();
								RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
								control->OnUpdate();

								if (control->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
								{
									control->OnClick();
									bCheckWidgetClicks = false;

									// Change of focus
									if (control->Flag(UIFlags::UI_Focusable))
									{
										window->IsFocusingControl = true;
										window->FocusedControl = control;
									}
									else
									{
										window->IsFocusingControl = false;
										window->FocusedControl = nullptr;
									}
								}
							}
						}
						else if (control->FileControlType == UIControlTypes::UIC_GroupBox || control->FileControlType != UIControlTypes::UIC_GroupBox && !control->parent_group)
						{
							if (SkipWidget && SkipMe == control)
								continue;

							POINT cAbs = control->GetAbsolutePos();
							RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
							control->OnUpdate();

							if (control->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
							{
								control->OnClick();
								bCheckWidgetClicks = false;

								// Change of focus
								if (control->Flag(UIFlags::UI_Focusable))
								{
									window->IsFocusingControl = true;
									window->FocusedControl = control;
								}
								else
								{
									window->IsFocusingControl = false;
									window->FocusedControl = nullptr;
								}
							}
						}
					}
				}
				// We must have clicked whitespace
				if (bCheckWidgetClicks)
				{
					// Loose focus on the control
					window->IsFocusingControl = false;
					window->FocusedControl = nullptr;
				}
			}
		}
	}
}

// Returns 
bool CGUI::GetKeyPress(unsigned int key)
{
	if (keys[key] == true && oldKeys[key] == false)
		return true;
	else
		return false;
}

bool CGUI::GetKeyState(unsigned int key)
{
	return keys[key];
}

bool CGUI::IsMouseInRegion(int x, int y, int x2, int y2)
{
	if (Mouse.x > x && Mouse.y > y && Mouse.x < x2 && Mouse.y < y2)
		return true;
	else
		return false;
}

bool CGUI::IsMouseInRegion(RECT region)
{
	return IsMouseInRegion(region.left, region.top, region.left + region.right, region.top + region.bottom);
}

POINT CGUI::GetMouse()
{
	return Mouse;
}

bool CGUI::DrawWindow(CWindow* window, int menu)
{

#define UI_WIN_TOPHEIGHT	0
#define UI_WIN_TITLEHEIGHT	32

#define UI_TAB_WIDTH		90
#define UI_TAB_HEIGHT		32

	if (menu == 1)
	{
		int _x = window->m_x + 8;
		int _tab_x = window->m_x + 8 - 90;
		int _y = window->m_y + 29 * 2;
		int _width = window->m_iWidth - 16;
		int _height = window->m_iHeight - 236;

		int iProportion = window->m_iWidth - 4 - 12 + 90 - 2;
		iProportion = iProportion / 3;

		//Inner
		draw.rect(window->m_x + 120, window->m_y, window->m_iWidth - 100, window->m_iHeight, Color(21, 21, 21, 255));
		//draw.text(window->m_x + 642, window->m_y + 509, Color(255, 255, 255, 255), Render::Fonts::MenuBold, "yiffware.top");
		draw.gradient_horizontal(window->m_x + 127, window->m_y + 7, (window->m_iWidth - 392) / 2, 2, Color(80, 159, 216, 255), Color(159, 102, 209, 255));
		draw.gradient_horizontal(window->m_x + 322, window->m_y + 7, (window->m_iWidth - 292) / 2, 2, Color(159, 102, 209, 255), Color(204, 81, 194, 255));
		draw.gradient_horizontal(window->m_x + 522, window->m_y + 7, (window->m_iWidth - 221) / 2, 2, Color(204, 81, 194, 255), Color(204, 227, 53, 255));

		//draw.gradient_horizontal(window->m_x + 127, window->m_y + 7, (window->m_iWidth - 192) / 2, 2, Color(0 * (255 / 255.f), 160 * (255 / 255.f), 255 * (255 / 255.f), 255), Color(160 * (255 / 255.f), 0 * (255 / 255.f), 255 * (255 / 255.f), 255));
		//draw.gradient_horizontal(window->m_x + 195 + (window->m_iWidth - 330) / 2, window->m_y + 7, (window->m_iWidth - 24) / 2, 2, Color(160, 0 , 255), Color(255 * (255 / 255.f), 255 * (255 / 255.f), 0 * (255 / 255.f), 255));
		//draw.rect(window->m_x + 7, window->m_y + 128, (window->m_iWidth - 14), 2, Color(0 * (255 / 255.f), 0 * (255 / 255.f), 0 * (255 / 255.f), 200 * (255 / 255.f)));

		//Inner
		//Render::Outline(window->m_x + 7, window->m_y + 1 + 26, window->m_iWidth - 4 - 10, window->m_iHeight - 2 - 6 - 26, Color(0, 150, 0, MenuAlpha));
		//Render::Clear(window->m_x + 8, window->m_y + 1 + 27, window->m_iWidth - 4 - 12, window->m_iHeight - 2 - 8 - 26, Color(255, 255, 255, MenuAlpha));

		//draw.outline(window->m_x, window->m_y, window->m_iWidth, window->m_iHeight, Color(0, 0, 0, 255));
		draw.outline(window->m_x + 121, window->m_y + 1, window->m_iWidth - 102, window->m_iHeight - 2, Color(65, 65, 65, 255));
		draw.outline(window->m_x + 122, window->m_y + 2, window->m_iWidth - 104, window->m_iHeight - 4, Color(45, 45, 45, 255));
		draw.outline(window->m_x + 123, window->m_y + 3, window->m_iWidth - 106, window->m_iHeight - 6, Color(45, 45, 45, 255));
		draw.outline(window->m_x + 124, window->m_y + 4, window->m_iWidth - 108, window->m_iHeight - 8, Color(45, 45, 45, 255));
		draw.outline(window->m_x + 125, window->m_y + 5, window->m_iWidth - 110, window->m_iHeight - 10, Color(65, 65, 65, 255));

		// we are using the dimensions of the picture we got from Photoshop

		int tabcount = window->Tabs.size();
		if (tabcount) // If there are some tabs
		{
			bool isOut = true;

			for (int i = 0; i < tabcount; i++)
			{
				CTab *tab = window->Tabs[i];

				float xAxis;
				float yAxis;
				float yWinPos = _y;
				float yWinHeight = _height;

				float intercept = (yWinHeight - 40) / (tabcount + 1);
				int factor = i;

				yAxis = yWinPos + 16 + (factor * intercept) - 10 + 20;

				RECT TabDrawArea = { window->m_x + 135, window->m_y + UI_WIN_TITLEHEIGHT + UI_WIN_TOPHEIGHT + (i*UI_TAB_HEIGHT) , UI_TAB_WIDTH, UI_TAB_HEIGHT };

				RECT TextSize;
				TextSize = draw.get_text_size(tab->Title.c_str(), draw.fonts.font_icons);

				RECT ClickTabArea = { xAxis,
					yAxis,
					TextSize.right,
					TextSize.bottom };

				if (GetAsyncKeyState(VK_LBUTTON))
				{
					if (IsMouseInRegion(TabDrawArea))
					{
						window->SelectedTab = window->Tabs[i];
						window->IsFocusingControl = false;
						window->FocusedControl = nullptr;
					}
				}



				//Color txtColor = UI_COL_SHADOW;
				if (IsMouseInRegion(TabDrawArea) && window->SelectedTab != tab) {
					draw.text(TabDrawArea.left + 1, TabDrawArea.top + 8, tab->Title.c_str(), draw.fonts.font_icons, Color(140, 140, 140, 255));

				}
				else if (window->SelectedTab == tab) {
					draw.text(TabDrawArea.left + 1, TabDrawArea.top + 8, tab->Title.c_str(), draw.fonts.font_icons, Color(250, 250, 250, 255));
				}
				else {
					draw.text(TabDrawArea.left + 1, TabDrawArea.top + 8, tab->Title.c_str(), draw.fonts.font_icons, Color(83, 83, 83, 255));
				}

			}
		}
	}
	// Controls 
	if (window->SelectedTab != nullptr)
	{
		// Focused widget
		bool SkipWidget = false;
		CControl* SkipMe = nullptr;

		// this window is focusing on a widget??
		if (window->IsFocusingControl)
		{
			if (window->FocusedControl != nullptr)
			{
				CControl* control = window->FocusedControl;
				CGroupBox* group;
				if (control->FileControlType != UIControlTypes::UIC_GroupBox) group = control->parent_group ? (CGroupBox*)control->parent_group : nullptr;

				if (group != nullptr && control->FileControlType != UIControlTypes::UIC_GroupBox)
				{
					if (group->group_tabs.size() > 0 && control->g_tab == group->selected_tab || group->group_tabs.size() == 0)
					{
						SkipWidget = true;
						SkipMe = window->FocusedControl;
					}
				}
				else if (control->FileControlType == UIControlTypes::UIC_GroupBox || control->FileControlType != UIControlTypes::UIC_GroupBox && !control->parent_group)
				{

					SkipWidget = true;
					SkipMe = window->FocusedControl;
				}
			}
		}


		// Itterate over all the other controls
		for (auto control : window->SelectedTab->Controls)
		{
			if (SkipWidget && SkipMe == control)
				continue;

			if (control != nullptr && control->Flag(UIFlags::UI_Drawable))
			{
				CGroupBox* group;
				if (control->FileControlType != UIControlTypes::UIC_GroupBox) group = control->parent_group ? (CGroupBox*)control->parent_group : nullptr;

				if (group != nullptr && control->FileControlType != UIControlTypes::UIC_GroupBox)
				{
					if (group->group_tabs.size() > 0 && control->g_tab == group->selected_tab || group->group_tabs.size() == 0)
					{
						POINT cAbs = control->GetAbsolutePos();
						RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
						bool hover = false;
						if (IsMouseInRegion(controlRect))
						{
							hover = true;
						}
						control->Draw(hover);
					}
				}
				else if (control->FileControlType == UIControlTypes::UIC_GroupBox || control->FileControlType != UIControlTypes::UIC_GroupBox && !control->parent_group)
				{

					POINT cAbs = control->GetAbsolutePos();
					RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
					bool hover = false;
					if (IsMouseInRegion(controlRect))
					{
						hover = true;
					}
					control->Draw(hover);
				}
			}
		}

		// Draw the skipped widget last
		if (SkipWidget)
		{
			auto control = window->FocusedControl;

			if (control != nullptr && control->Flag(UIFlags::UI_Drawable))
			{
				CControl* control = window->FocusedControl;
				CGroupBox* group;
				if (control->FileControlType != UIControlTypes::UIC_GroupBox) group = control->parent_group ? (CGroupBox*)control->parent_group : nullptr;

				if (group != nullptr && control->FileControlType != UIControlTypes::UIC_GroupBox)
				{
					if (group->group_tabs.size() > 0 && control->g_tab == group->selected_tab || group->group_tabs.size() == 0)
					{
						POINT cAbs = control->GetAbsolutePos();
						RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
						bool hover = false;
						if (IsMouseInRegion(controlRect))
						{
							hover = true;
						}
						control->Draw(hover);
					}
				}
				else if (control->FileControlType == UIControlTypes::UIC_GroupBox || control->FileControlType != UIControlTypes::UIC_GroupBox && !control->parent_group)
				{

					POINT cAbs = control->GetAbsolutePos();
					RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
					bool hover = false;
					if (IsMouseInRegion(controlRect))
					{
						hover = true;
					}
					control->Draw(hover);
				}
			}
		}
		
	}
	

	return true;
}

void CGUI::RegisterWindow(CWindow* window)
{
	Windows.push_back(window);

	// Resorting to put groupboxes at the start
	for (auto tab : window->Tabs)
	{
		for (auto control : tab->Controls)
		{
			if (control->Flag(UIFlags::UI_RenderFirst))
			{
				CControl * c = control;
				tab->Controls.erase(std::remove(tab->Controls.begin(), tab->Controls.end(), control), tab->Controls.end());
				tab->Controls.insert(tab->Controls.begin(), control);
			}
		}
	}
}

void CGUI::BindWindow(unsigned char Key, CWindow* window)
{
	if (window)
		WindowBinds[Key] = window;
	else
		WindowBinds.erase(Key);
}

void CGUI::SaveWindowState(CWindow* window, std::string Filename)
{
	// Create a whole new document and we'll just save over top of the old one
	tinyxml2::XMLDocument Doc;

	// Root Element is called "ayy"
	tinyxml2::XMLElement *Root = Doc.NewElement("Supreme");
	Doc.LinkEndChild(Root);

	// If the window has some tabs..
	if (Root && window->Tabs.size() > 0)
	{
		for (auto Tab : window->Tabs)
		{
			// Add a new section for this tab
			tinyxml2::XMLElement *TabElement = Doc.NewElement(Tab->Title.c_str());
			Root->LinkEndChild(TabElement);

			// Now we itterate the controls this tab contains
			if (TabElement && Tab->Controls.size() > 0)
			{
				for (auto Control : Tab->Controls)
				{
					// If the control is ok to be saved
					if (Control && Control->Flag(UIFlags::UI_SaveFile) && Control->FileIdentifier.length() > 1 && Control->FileControlType)
					{
						// Create an element for the control
						tinyxml2::XMLElement *ControlElement = Doc.NewElement(Control->FileIdentifier.c_str());
						TabElement->LinkEndChild(ControlElement);

						if (!ControlElement)
						{
							return;
						}

						CCheckBox* cbx = nullptr; 
						CComboBox* cbo = nullptr;
						CKeyBind* key = nullptr;
						CSlider* sld = nullptr;
						CItemSelector* itms = nullptr;
						CMultiBox* mtbx = nullptr;
						CListBox* lsbox = nullptr;
						CColorSelector* clse = nullptr;

						// Figure out what kind of control and data this is
						switch (Control->FileControlType)
						{
						case UIControlTypes::UIC_CheckBox:
							cbx = (CCheckBox*)Control;
							ControlElement->SetText(cbx->GetState());
							break;
						case UIControlTypes::UIC_ComboBox:
							cbo = (CComboBox*)Control;
							ControlElement->SetText(cbo->GetIndex());
							break;
						case UIControlTypes::UIC_KeyBind:
							key = (CKeyBind*)Control;
							ControlElement->SetText(key->GetKey());
							break;
						case UIControlTypes::UIC_Slider:
							sld = (CSlider*)Control;
							ControlElement->SetText(sld->GetValue());
							break;
						case UIControlTypes::UIC_ItemSelector:
							itms = (CItemSelector*)Control;
							ControlElement->SetText(itms->GetInt());
							break;
						case UIControlTypes::UIC_ListBox:
							lsbox = (CListBox*)Control;
							ControlElement->SetText(lsbox->GetIndex());
							break;
						case UIControlTypes::UIC_MultiBox:
						{
							mtbx = (CMultiBox*)Control;
							std::string x;
							for (int i = 0; i < mtbx->items.size(); i++)
							{
								std::string status;
								status = mtbx->items[i].bselected ? "1" : "0";
								x = x + status;
							}
							ControlElement->SetText(x.c_str());
							break;
						}
						case UIControlTypes::UIC_ColorSelector:
							clse = (CColorSelector*)Control;
							char buffer[128];
							float r, g, b, a;
							r = clse->GetValue()[0];
							g = clse->GetValue()[1];
							b = clse->GetValue()[2];
							a = clse->GetValue()[3];
							sprintf_s(buffer, "%1.f %1.f %1.f %1.f", r, g, b, a);
							ControlElement->SetText(buffer);
							break;
						}
					}
				}
			}
		}
	}

	//Save the file
	if (Doc.SaveFile(Filename.c_str()) != tinyxml2::XML_NO_ERROR)
	{
		MessageBox(NULL, "bitch ass cant do shit!", "Supreme", MB_OK);
	}
	
}

void CGUI::LoadWindowState(CWindow* window, std::string Filename)
{
	// Lets load our meme
	tinyxml2::XMLDocument Doc;
	if (Doc.LoadFile(Filename.c_str()) == tinyxml2::XML_NO_ERROR)
	{
		tinyxml2::XMLElement *Root = Doc.RootElement();

		// The root "ayy" element
		if (Root)
		{
			// If the window has some tabs..
			if (Root && window->Tabs.size() > 0)
			{
				for (auto Tab : window->Tabs)
				{
					// We find the corresponding element for this tab
					tinyxml2::XMLElement *TabElement = Root->FirstChildElement(Tab->Title.c_str());
					if (TabElement)
					{
						// Now we itterate the controls this tab contains
						if (TabElement && Tab->Controls.size() > 0)
						{
							for (auto Control : Tab->Controls)
							{
								// If the control is ok to be saved
								if (Control && Control->Flag(UIFlags::UI_SaveFile) && Control->FileIdentifier.length() > 1 && Control->FileControlType)
								{
									// Get the controls element
									tinyxml2::XMLElement *ControlElement = TabElement->FirstChildElement(Control->FileIdentifier.c_str());

									if (ControlElement)
									{
										CCheckBox* cbx = nullptr;
										CComboBox* cbo = nullptr;
										CKeyBind* key = nullptr;
										CSlider* sld = nullptr;
										CItemSelector* itms = nullptr;
										CMultiBox* mtbx = nullptr;
										CListBox* lsbox = nullptr;
										CColorSelector* clse = nullptr;

										// Figure out what kind of control and data this is
										switch (Control->FileControlType)
										{
										case UIControlTypes::UIC_CheckBox:
											cbx = (CCheckBox*)Control;
											cbx->SetState(ControlElement->GetText()[0] == '1' ? true : false);
											break;
										case UIControlTypes::UIC_ComboBox:
											cbo = (CComboBox*)Control;
											cbo->SelectIndex(atoi(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_KeyBind:
											key = (CKeyBind*)Control;
											key->SetKey(atoi(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_Slider:
											sld = (CSlider*)Control;
											sld->SetValue(atof(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_ItemSelector:
											itms = (CItemSelector*)Control;
											itms->SetValue(atof(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_ListBox:
											lsbox = (CListBox*)Control;
											lsbox->SelectItem(atoi(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_MultiBox:
											mtbx = (CMultiBox*)Control;
											for (int i = 0; i < mtbx->items.size(); i++)
											{
												mtbx->items[i].bselected = ControlElement->GetText()[i] == '1' ? true : false;
											}
											break;
										case UIControlTypes::UIC_ColorSelector:
											clse = (CColorSelector*)Control;
											int r, g, b, a;
											std::stringstream ss(ControlElement->GetText());
											ss >> r >> g >> b >> a;
											clse->SetColor(r, g, b, a);
											break;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
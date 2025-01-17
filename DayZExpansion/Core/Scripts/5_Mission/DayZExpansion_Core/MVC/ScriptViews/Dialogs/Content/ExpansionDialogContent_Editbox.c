/**
 * ExpansionDialogContent_TextScroller.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionDialogContent_Editbox: ExpansionDialogContentBase
{
	ref ExpansionDialogContent_EditboxController m_EditboxController;
	
	EditBoxWidget dialog_editbox;
	
	void ExpansionDialogContent_Editbox(ExpansionDialogBase dialog)
	{
		m_Dialog = dialog;

		if (!m_EditboxController)
			m_EditboxController = ExpansionDialogContent_EditboxController.Cast(GetController());
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Core/GUI/layouts/mvc/dialogs/expansion_dialog_editbox.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionDialogContent_EditboxController;
	}
	
	override void OnShow()
	{
		super.OnShow();
	}
	
	string GetEditboxText()
	{
		return dialog_editbox.GetText();
	}
};

class ExpansionDialogContent_EditboxController: ExpansionViewController
{
	string EditBox;
}

class ExpansionMenuDialogContent_Editbox: ExpansionMenuDialogContentBase
{
	ref ExpansionMenuDialogContent_EditboxController m_EditboxController;
	
	EditBoxWidget dialog_editbox;
	
	void ExpansionMenuDialogContent_Editbox(ExpansionMenuDialogBase dialog)
	{
		m_Dialog = dialog;

		if (!m_EditboxController)
			m_EditboxController = ExpansionMenuDialogContent_EditboxController.Cast(GetController());
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Core/GUI/layouts/mvc/dialogs/expansion_menu_dialog_editbox.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionMenuDialogContent_EditboxController;
	}
	
	override void OnShow()
	{
		super.OnShow();
		
		SetFocus(dialog_editbox);
	}
	
	string GetEditboxText()
	{
		return dialog_editbox.GetText();
	}
	
	void SetEditboxText(string text)
	{
		dialog_editbox.SetText(text);
	}
	
	void SetTextColor(int color)
	{
		dialog_editbox.SetTextColor(color);
	}
};

class ExpansionMenuDialogContent_EditboxController: ExpansionViewController
{
	string EditBox;
}
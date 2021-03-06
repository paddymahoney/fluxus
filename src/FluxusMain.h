// Copyright (C) 2005 Dave Griffiths
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include "GLEditor.h"
#include "GLFileDialog.h"
#include "Repl.h"
#include "Interpreter.h"

using namespace fluxus;

#ifndef FLUXUS_MAIN
#define FLUXUS_MAIN

namespace fluxus
{

static const int NUM_EDITORS=10;
static const int DEFAULT_WIDTH=720;
static const int DEFAULT_HEIGHT=576;

class FluxusMain
{
public:
	FluxusMain(int x, int y);
	virtual ~FluxusMain();

	void Handle(unsigned char key, int button, int special, int state, int x, int y, int mod);
	void Render();
	void Reshape(int width, int height);

	bool KeyPressed(char b);

    wstring GetScriptFragment() { wstring temp=m_Script; m_Script=L""; return temp; }
    void LoadScript(const wstring &Filename);
    void SetSaveName(const wstring &s) { m_SaveName[m_CurrentEditor]=s; }
    void SaveScript();
    void SaveBackupScript();
	void HideScript() { m_HideScript=!m_HideScript; }
	void HideCursor();
	void SetCurrentEditor(int s) { m_CurrentEditor=s; }
	void Execute();

	Repl * GetRepl() { return (Repl*)m_Editor[9]; }
    void SwitchToRepl() { m_CurrentEditor = 9; }

	int m_OrigWidth;
	int m_OrigHeight;

private:

	void Pretty();

	GLEditor *m_Editor[NUM_EDITORS];
	int m_CurrentEditor;
	wstring m_SaveName[NUM_EDITORS];
	GLFileDialog *m_FileDialog;

	int m_Width;
	int m_Height;
	wstring m_Script;
	bool m_HideScript;
	bool m_ShowCursor;
	bool m_ShowFileDialog;
};

};

#endif


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

#include "PDataContainer.h"

using namespace Fluxus;

PDataContainer::PDataContainer() 
{
}

PDataContainer::PDataContainer(const PDataContainer &other) 
{
	Clear();
	for (map<string,PData*>::const_iterator i=other.m_PData.begin(); 
		i!=other.m_PData.end(); i++)
	{
		m_PData[i->first] = i->second->Copy();
	}
}

PDataContainer::~PDataContainer() 
{
	Clear();
}

void PDataContainer::Clear()
{
	for (map<string,PData*>::iterator i=m_PData.begin(); i!=m_PData.end(); i++)
	{
		delete i->second;
	}
}	

void PDataContainer::Resize(unsigned int size)
{
	for (map<string,PData*>::iterator i=m_PData.begin(); i!=m_PData.end(); i++)
	{
		i->second->Resize(size);
	}
}

	
unsigned int PDataContainer::Size() const
{
	if (!m_PData.empty())
	{
		return m_PData.begin()->second->Size();
	}
	
	return 0;
}

bool PDataContainer::GetDataInfo(const string &name, char &type, unsigned int &size) const
{
	map<string,PData*>::const_iterator i=m_PData.find(name);
	if (i==m_PData.end())
	{
		return false;
	}
	
	size=i->second->Size();
	
	//\todo: remove all this dynamic casting and store the char type inside pdata...
	TypedPData<dVector> *data = dynamic_cast<TypedPData<dVector>*>(i->second);	
	if (data) type='v';
	else
	{
		TypedPData<dColour> *data = dynamic_cast<TypedPData<dColour>*>(i->second);
		if (data) type='c';
		else 
		{
			TypedPData<float> *data = dynamic_cast<TypedPData<float>*>(i->second);
			if (data) type='f';
			else 
			{
				TypedPData<dMatrix> *data = dynamic_cast<TypedPData<dMatrix>*>(i->second);
				if (data) type='m';
			}
		}
	}
		
	return true;
}
	
void PDataContainer::AddData(const string &name, PData* pd)
{
	map<string,PData*>::iterator i=m_PData.find(name);
	if (i!=m_PData.end())
	{
		Trace::Stream<<"Primitive::AddData: pdata: "<<name<<" already exists"<<endl;
		return;
	}
	
	m_PData[name]=pd;
}

void PDataContainer::CopyData(const string &name, string newname)
{
	map<string,PData*>::iterator i=m_PData.find(name);
	if (i==m_PData.end())
	{
		Trace::Stream<<"Primitive::CopyData: pdata source: "<<name<<" doesn't exist"<<endl;
		return;
	}
	
	// delete the old one if it exists
	map<string,PData*>::iterator oldi=m_PData.find(newname);
	if (oldi!=m_PData.end())
	{
		delete oldi->second;
	}
	
	m_PData[newname]=i->second->Copy();
	
	PDataDirty();
}

void PDataContainer::RemoveDataVec(const string &name)
{
	map<string,PData*>::iterator i=m_PData.find(name);
	if (i==m_PData.end())
	{
		Trace::Stream<<"Primitive::RemovePDataVec: pdata: "<<name<<" doesn't exist"<<endl;
		return;
	}
	
	delete i->second;
	m_PData.erase(i);
}

PData* PDataContainer::GetDataRaw(const string &name)
{
	map<string,PData*>::iterator i=m_PData.find(name);
	if (i==m_PData.end())
	{
		return NULL;
	}
	
	return i->second;
}

const PData* PDataContainer::GetDataRawConst(const string &name) const
{
	map<string,PData*>::const_iterator i=m_PData.find(name);
	if (i==m_PData.end())
	{
		return NULL;
	}
	
	return i->second;
}

void PDataContainer::SetDataRaw(const string &name, PData* pd)
{
	map<string,PData*>::iterator i=m_PData.find(name);
	if (i==m_PData.end())
	{
		Trace::Stream<<"Primitive::SetDataRaw: pdata: "<<name<<" doesn't exist"<<endl;
		return;
	}
	delete i->second;
	i->second = pd;
	PDataDirty();
}

void PDataContainer::GetDataNames(vector<string> &names) const
{
	for (map<string,PData*>::const_iterator i=m_PData.begin(); i!=m_PData.end(); ++i)
	{
		names.push_back(i->first);
	}
}

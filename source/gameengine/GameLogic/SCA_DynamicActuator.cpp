/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2001-2002 by NaN Holding BV.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): none yet.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file gameengine/GameLogic/SCA_DynamicActuator.cpp
 *  \ingroup gamelogic
 * Adjust dynamics settings for this object
 */


#include "SCA_DynamicActuator.h"
#include "PHY_IPhysicsController.h"


SCA_DynamicActuator::SCA_DynamicActuator(SCA_IObject *gameobj,
                                         short dyn_operation,
                                         float setmass)
	: SCA_IActuator(gameobj, SCA_ACT_DYNAMIC),
	m_dyn_operation(dyn_operation),
	m_setmass(setmass)
{
}

SCA_DynamicActuator::~SCA_DynamicActuator()
{ 
	// there's nothing to be done here, really....
}

bool SCA_DynamicActuator::Update()
{
	// bool result = false;	/*unused*/
	KX_GameObject *obj = (KX_GameObject *) GetParent();
	bool bNegativeEvent = IsNegativeEvent();
	PHY_IPhysicsController *controller;
	RemoveAllEvents();

	if (bNegativeEvent)
		return false; // do nothing on negative events
	
	if (!obj)
		return false; // object not accessible, shouldnt happen
	controller = obj->GetPhysicsController();
	if (!controller)
		return false;	// no physic object

	switch (m_dyn_operation) {
		case KX_DYN_RESTORE_DYNAMICS:
		{
			// Child objects must be static, so we block changing to dynamic
			if (!obj->GetParent())
				controller->RestoreDynamics();
			break;
		}
		case KX_DYN_DISABLE_DYNAMICS:
		{
			controller->SuspendDynamics();
			break;
		}
		case KX_DYN_ENABLE_RIGID_BODY:
		{
			controller->SetRigidBody(true);
			break;
		}
		case KX_DYN_DISABLE_RIGID_BODY:
		{
			controller->SetRigidBody(false);
			break;
		}
		case KX_DYN_SET_MASS:
		{
			controller->SetMass(m_setmass);
			break;
		}
		case KX_DYN_RESTORE_PHYSICS:
		{
			controller->RestorePhysics();
			break;
		}
		case KX_DYN_DISABLE_PHYSICS:
		{
			controller->SuspendPhysics();
			break;
		}
	}

	return false;
}

CValue *SCA_DynamicActuator::GetReplica()
{
	SCA_DynamicActuator *replica = new SCA_DynamicActuator(*this);

	if (replica == NULL)
		return NULL;

	replica->ProcessReplica();
	return replica;
}


#ifdef WITH_PYTHON

/* ------------------------------------------------------------------------- */
/* Python functions                                                          */
/* ------------------------------------------------------------------------- */

/* Integration hooks ------------------------------------------------------- */

PyTypeObject SCA_DynamicActuator::Type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"SCA_DynamicActuator",
	sizeof(PyObjectPlus_Proxy),
	0,
	py_base_dealloc,
	0,
	0,
	0,
	0,
	py_base_repr,
	0,0,0,0,0,0,0,0,0,
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
	0,0,0,0,0,0,0,
	Methods,
	0,
	0,
	&SCA_IActuator::Type,
	0,0,0,0,0,0,
	py_base_new
};

PyMethodDef SCA_DynamicActuator::Methods[] = {
	{NULL,NULL} //Sentinel
};

PyAttributeDef SCA_DynamicActuator::Attributes[] = {
	KX_PYATTRIBUTE_SHORT_RW("mode", 0, 4, false, SCA_DynamicActuator, m_dyn_operation),
	KX_PYATTRIBUTE_FLOAT_RW("mass", 0.0f, FLT_MAX, SCA_DynamicActuator, m_setmass),
	KX_PYATTRIBUTE_NULL	//Sentinel
};

#endif // WITH_PYTHON


/* eof */
